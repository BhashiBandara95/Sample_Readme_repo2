/************************************************************************
 * \file        areg/component/private/TimerManager.cpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, The System Timer Manager.
 *              Controlling, triggering and stopping timer.
 *
 ************************************************************************/
#include "areg/component/private/TimerManager.hpp"

#include "areg/component/IETimerConsumer.hpp"
#include "areg/component/private/TimerEventData.hpp"
#include "areg/component/private/ExitEvent.hpp"

#include "areg/component/Timer.hpp"
#include "areg/base/Thread.hpp"
#include "areg/base/NEUtilities.hpp"

//////////////////////////////////////////////////////////////////////////
// TimerManager class implementation
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Implement Runtime
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_RUNTIME(TimerManager, DispatcherThread)

//////////////////////////////////////////////////////////////////////////
// Predefined constants
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   Timer Manager thread name
 **/
const char * const TimerManager::TIMER_THREAD_NAME        = "_AREG_TIMER_THREAD_NAME_";

//////////////////////////////////////////////////////////////////////////
// Static functions
//////////////////////////////////////////////////////////////////////////

TimerManager & TimerManager::getInstance( void )
{
    static TimerManager _timerManager;
    return _timerManager;
}

bool TimerManager::startTimerManager( void )
{
    return getInstance()._startTimerManagerThread( );
}

void TimerManager::stopTimerManager( void )
{
    return getInstance()._stopTimerManagerThread();
}

bool TimerManager::isTimerManagerStarted( void )
{
    return getInstance().isReady();
}

bool TimerManager::startTimer( Timer &timer )
{
    TimerManager & timerManager = getInstance();

    bool result = false;
    if (timerManager._registerTimer(timer) )
    {
        TimerManagingEventData data(timer, TimerManagingEventData::TimerStart);
        result = TimerManagingEvent::sendEvent(data, static_cast<IETimerManagingEventConsumer &>(timerManager), static_cast<DispatcherThread &>(timerManager));
    }

    return result;
}

bool TimerManager::startTimer(Timer &timer, const DispatcherThread & whichThread)
{
    TimerManager & timerManager = getInstance();

    bool result = false;
    if ( timerManager._registerTimer(timer, whichThread) )
    {
        TimerManagingEventData data(timer, TimerManagingEventData::TimerStart);
        result = TimerManagingEvent::sendEvent(data, static_cast<IETimerManagingEventConsumer &>(timerManager), static_cast<DispatcherThread &>(timerManager));
    }

    return result;
}

bool TimerManager::stopTimer( Timer &timer )
{
    return getInstance()._unregisterTimer( timer );
}

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////

TimerManager::TimerManager( void )
    : DispatcherThread            ( TimerManager::TIMER_THREAD_NAME )
    , IETimerManagingEventConsumer ( )

    , mTimerTable   ( )
    , mExpiredTimers( )
    , mLock         ( )
{
    ; // do nothing
}

TimerManager::~TimerManager( void )
{
    _removeAllTimers( );
}

//////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////
bool TimerManager::_registerTimer( Timer &timer )
{
    return _registerTimer(timer, DispatcherThread::getCurrentDispatcherThread());
}

bool TimerManager::_registerTimer(Timer &timer, const DispatcherThread & whichThread)
{
    bool result = false;
    if (timer.isValid() && whichThread.isValid() && whichThread.isRunning() )
    {
        TIMERHANDLE timerHandle = TimerManager::_createWaitableTimer(timer.hasName() ? timer.getName().getString() : NULL_STRING);
        if (timerHandle != static_cast<TIMERHANDLE>(NULL))
        {
            Lock lock(mLock);

            result = true;
            mTimerTable.registerObject(&timer, TimerInfo(timer, timerHandle, whichThread.getId()) );
            OUTPUT_DBG("Registered timer [ %s ]", timer.getName().getString());
        }
    }
#ifdef DEBUG
    else
    {
        OUTPUT_ERR("Either times [ %s ] is not valid or thread [ %s ] is not running, cancel timer.", timer.getName().getString(), whichThread.getName().getString());
    }
#endif // DEBUG

    return result;
}

bool TimerManager::_registerTimer(Timer &timer, ITEM_ID whichThreadId)
{
    Thread * thread = Thread::findThreadById(whichThreadId);
    return (thread != NULL ? _registerTimer(timer, *thread) : false);
}

bool TimerManager::_unregisterTimer( Timer &timer )
{
    Lock lock(mLock);

    TimerInfo timerInfo;
    bool result = false;
    mExpiredTimers.removeAllTimers(&timer);
    if ( mTimerTable.unregisterObject(&timer, timerInfo) )
    {
        ASSERT(timerInfo.getHandle() != NULL);
        TimerManager::_destroyWaitableTimer( timerInfo.getHandle(), true );
        result = true;

        OUTPUT_DBG("Successfully unregistered timer [ %s ].", timer.getName().getString());
    }

    return result;
}

void TimerManager::_removeAllTimers( void )
{
    Lock lock(mLock);

    Timer*    timer  = NULL;
    TimerInfo timerInfo;
    while ( mTimerTable.isEmpty() == false )
    {
        VERIFY(mTimerTable.unregisterFirstObject(timer, timerInfo));
        ASSERT((timer != NULL) && (timerInfo.getHandle() != NULL));
        TimerManager::_destroyWaitableTimer( timerInfo.getHandle( ), true );
    }

    mTimerTable.removeAll();
    mExpiredTimers.removeAll();
}

void TimerManager::processEvent( const TimerManagingEventData & data )
{
    switch(data.getAction())
    {
    case TimerManagingEventData::TimerStart:
        _startSystemTimer(data.getTimer());
        break;

    default:
        break; // ignore
    }
}

void TimerManager::_processExpiredTimers( void )
{
    mExpiredTimers.lockResource();
    while (mExpiredTimers.isEmpty() == false)
    {
        ExpiredTimerInfo expiredTime = mExpiredTimers.popTimer();
        mExpiredTimers.unlockResource();

        if (expiredTime.mTimer != NULL )
        {
            Lock lock(mLock);
            TimerInfo timerInfo;
            if (mTimerTable.findObject(expiredTime.mTimer, timerInfo))
            {
                Timer* currTimer = timerInfo.mTimer;
                if (currTimer->isValid() && timerInfo.isTimerExpired(expiredTime.mHighValue, expiredTime.mLowValue))
                {
                    if (timerInfo.isTimerActive())
                    {
                        timerInfo.mTimerState = TimerInfo::TimerPending;
                        mTimerTable.updateObject(currTimer, timerInfo);
                    }
                    else
                    {
                        OUTPUT_WARN("The Timer [ %s ] is not active anymore. Going to unregister", currTimer->getName().getString());
                        _unregisterTimer(*currTimer);
                    }

                    TimerEvent::sendEvent( *currTimer, timerInfo.mOwnThreadId );
                }
                else
                {
                    OUTPUT_ERR("The timer [ %p ] has expired, but it was ignored, becuase it is invalid. Ignoring sending event.", currTimer);
                }
            }
            else
            {
                OUTPUT_ERR("The timer [ %p ] did not find in the table or not active anymore.", expiredTime.mTimer);
            }
        }

        mExpiredTimers.lockResource();
    }

    mExpiredTimers.unlockResource();
}

void TimerManager::_timerExpired( Timer* whichTimer, unsigned int highValue, unsigned int lowValue )
{
    ExpiredTimerInfo expiredTimer(whichTimer, highValue, lowValue);
    mExpiredTimers.pushTimer( expiredTimer );
}

void TimerManager::_startSystemTimer( Timer* whichTimer )
{
    if (whichTimer != NULL)
    {
        Lock lock(mLock);

        TimerInfo timerInfo;

        if (mTimerTable.findObject(whichTimer, timerInfo))
        {
            ASSERT(timerInfo.mTimer == whichTimer);
            ASSERT(timerInfo.mHandle != NULL);
            if (_startSystemTimer(timerInfo) == false)
            {
                OUTPUT_ERR("Failed starting timer [ %s ]", whichTimer->getName().getString());
            }
        }
        else
        {
            OUTPUT_WARN("The timer object is not found in the table. Ignoring request to start timer");
        }
    }
    else
    {
        OUTPUT_ERR("Invalid NULL pointer ot Timer object. Ignoring starting timer.");
    }
}

bool TimerManager::_startSystemTimer( TimerInfo &timerInfo )
{
    bool result = false;
    if (timerInfo.canStartTimer())
    {
        result = TimerManager::_startSystemTimer(timerInfo, mTimerTable);
    }
    else
    {
        OUTPUT_ERR("Ignoring firing timer because it is not in Idle state. The current state of timer [ %s ] is [ %s ]"
                    , timerInfo.getTimer()->getName().getString()
                    , TimerInfo::getString(timerInfo.mTimerState));
    }
    return result;
}

bool TimerManager::postEvent( Event& eventElem )
{
    bool result = false;
    if ( RUNTIME_CAST(&eventElem, TimerManagingEvent) != NULL)
    {
        result = EventDispatcher::postEvent(eventElem);
    }
    else
    {
        OUTPUT_ERR("Not a TimerManagingEvent type event, cannot Post. Destroying event type [ %s ]", eventElem.getRuntimeClassName());
        eventElem.destroy();
    }

    return result;
}

bool TimerManager::runDispatcher( void )
{
    IESynchObject* synchObjects[] = {&mEventExit, &mEventQueue};
    MultiLock multiLock(synchObjects, 2, false);

    _timerThreadStarts( );
    int whichEvent  = static_cast<int>(EVENT_ERROR);
    const ExitEvent& exitEvent = ExitEvent::getExitEvent();
    do 
    {
        whichEvent = multiLock.lock(IESynchObject::WAIT_INFINITE, false, true);
        Event* eventElem = whichEvent == static_cast<int>(EVENT_QUEUE) ? pickEvent() : NULL;
        if ( static_cast<const Event *>(eventElem) != static_cast<const Event *>(&exitEvent) )
        {
            if ( whichEvent == static_cast<int>(EVENT_QUEUE) )
            {
                // proceed one external event.
                if (prepareDispatchEvent(eventElem))
                {
                    dispatchEvent(*eventElem);
                }
                 
                postDispatchEvent(eventElem);

                ASSERT(static_cast<EventQueue &>(mInternalEvents).isEmpty());
            }
            else if (whichEvent == static_cast<int>(MultiLock::LOCK_INDEX_COMPLETION))
            {
                _processExpiredTimers();
            }
        }
        else
        {
            OUTPUT_DBG("Received exit event. Going to exit System Timer Thread!");
            whichEvent = static_cast<int>(EVENT_EXIT);
        }

    } while (whichEvent == static_cast<int>(EVENT_QUEUE) || (whichEvent == MultiLock::LOCK_INDEX_COMPLETION));

    removeAllEvents( );
    _timerThreadExits( );
    ASSERT(static_cast<EventQueue &>(mInternalEvents).isEmpty());

    return (whichEvent == static_cast<int>(EVENT_EXIT));
}

void TimerManager::_timerThreadStarts( void )
{
    TimerManagingEvent::addListener(static_cast<IETimerManagingEventConsumer &>(self()), static_cast<DispatcherThread &>(self()));

    mHasStarted = true;
    mEventStarted.setEvent();
}

void TimerManager::_timerThreadExits( void )
{
    TimerManagingEvent::removeListener(static_cast<IETimerManagingEventConsumer &>(self()), static_cast<DispatcherThread &>(self()));

    _removeAllTimers( );
    mHasStarted = false;
    mEventStarted.resetEvent();
}

bool TimerManager::_startTimerManagerThread( void )
{
    bool result = false;
    if ( isReady() == false )
    {
        ASSERT(isRunning() == false);
        result = createThread(Thread::WAIT_INFINITE) && waitForDispatcherStart(IESynchObject::WAIT_INFINITE);
#ifdef _DEBUG
        if ( result == false )
        {
            OUTPUT_ERR("Failed to create [ %s ] System Timer Thread", TimerManager::TIMER_THREAD_NAME);
        }
#endif  // _DEBUG
    }
    else
    {
        result = true;
    }

    return result;
}

void TimerManager::_stopTimerManagerThread( void )
{
    destroyThread(Thread::WAIT_INFINITE);
}
