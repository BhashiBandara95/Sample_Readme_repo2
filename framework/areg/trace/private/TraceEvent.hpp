#ifndef AREG_TRACE_PRIVATE_TRACEEVENT_HPP
#define AREG_TRACE_PRIVATE_TRACEEVENT_HPP
/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2023 Aregtech UG. All rights reserved.
 * \file        areg/trace/private/TraceEvent.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan
 * \brief       AREG Platform, The logging thread, which is receiving logging events and performs log operations. 
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/component/TEEvent.hpp"
#include "areg/base/SharedBuffer.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class LogMessage;

//////////////////////////////////////////////////////////////////////////
// TraceEventData class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   The tracing (or logging) event is triggered each time when application
 *          needs a logging action to be performed. The event is forwarded to a logging thread
 *          for further processing, like configuring logging, output message, etc.
 *          The logging event contains data with the instruction to perform certain actions.
 *          This logging event data is automatically generated by system, when a certain API
 *          is called.
 **/
class AREG_API TraceEventData
{
//////////////////////////////////////////////////////////////////////////
// Internal types and constants
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   TraceEventData::eTraceAction
     *          The list of supported actions for logging.
     **/
    typedef enum class E_TraceAction
    {
          TraceUndefined                //!< Action is undefined, do nothing
        , TraceStartLogs                //!< Action to notify to start logging
        , TraceStopLogs                 //!< Action to notify to stop logging
        , TraceSetEnableLogs            //!< Action to notify to enable logging
        , TraceSetDisableLogs           //!< Action to notify to stop logging
        , TraceSaveScopes               //!< Action to notify to save scope list
        , TraceLogMessage               //!< Action to output logging message
        , TraceUpdateScopes
        , TraceQueryScopes
    } eTraceAction;

    /**
     * \brief   Converts and returns the string of TraceEventData::eTraceAction value
     **/
    static inline const char * getString( TraceEventData::eTraceAction action );

//////////////////////////////////////////////////////////////////////////
// Constructors / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Creates the logging even data with undefined action
     **/
    TraceEventData( void );

    /**
     * \brief   Creates the logging even data with specified action
     * \param   action  The action ID to set in event data
     **/
    explicit TraceEventData( TraceEventData::eTraceAction action );
    
    /**
     * \brief   Creates the logging even data with specified action and data
     * \param   action      The action ID to set in event data
     * \param   dataBuffer  The buffer of data set.
     **/
    TraceEventData( TraceEventData::eTraceAction action, const SharedBuffer & dataBuffer );

    /**
     * \brief   Creates the logging even data with specified action and logging message data
     * \param   action  The action ID to set in event data
     * \param   logData The buffer of logging message data set.
     **/
    TraceEventData( TraceEventData::eTraceAction action, const LogMessage & logData );

    /**
     * \brief   Copies logging event data from given source.
     * \param   src     The source to copy data.
     **/
    TraceEventData( const TraceEventData & src );

    /**
     * \brief   Copies logging event data from given source.
     * \param   src     The source to copy data.
     **/
    TraceEventData( TraceEventData && src ) noexcept;

    /**
     * \brief   Destructor
     **/
    ~TraceEventData( void ) = default;

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Copies data from given source
     * \param   src     The source of data to copy.
     **/
    TraceEventData & operator = ( const TraceEventData & src );

    /**
     * \brief   Moves data from given source
     * \param   src     The source of data to move.
     **/
    TraceEventData & operator = ( TraceEventData && src ) noexcept;

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////

    /**
     * \brief   Returns the value of action set in event data.
     **/
    inline TraceEventData::eTraceAction getTraceAction( void ) const;

    /**
     * \brief   Returns the streaming buffer for writing.
     **/
    inline SharedBuffer & getWritableStream( void );

    /**
     * \brief   Returns the streaming buffer for reading.
     **/
    inline const SharedBuffer & getReadableStream( void ) const;

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    TraceEventData::eTraceAction  mAction;
    SharedBuffer                  mDataBuffer;
};

//////////////////////////////////////////////////////////////////////////
// Tracing Event and Consumer declaration
//////////////////////////////////////////////////////////////////////////

/**
 * \brief   Declare TraceEvent class, derived from TEEvent object,
 *          and IETraceEventConsumer class, derived from TEEventConsumer object
 **/
DECLARE_EVENT(TraceEventData, TraceEvent, IETraceEventConsumer)

//////////////////////////////////////////////////////////////////////////
// TraceEventData class inline functions
//////////////////////////////////////////////////////////////////////////

inline TraceEventData::eTraceAction TraceEventData::getTraceAction( void ) const
{
    return mAction;
}

inline SharedBuffer & TraceEventData::getWritableStream( void )
{
    return mDataBuffer;
}

inline const SharedBuffer & TraceEventData::getReadableStream( void ) const
{
    return mDataBuffer;
}

inline const char * TraceEventData::getString( TraceEventData::eTraceAction action )
{
    switch ( action )
    {
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceUndefined);
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceStartLogs);
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceStopLogs);
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceSetEnableLogs);
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceSetDisableLogs);
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceSaveScopes);
    CASE_MAKE_STRING(TraceEventData::eTraceAction::TraceLogMessage);
    CASE_DEFAULT("ERR: Undefined TraceEventData::eTraceAction value!");
    }
}

#endif  // AREG_TRACE_PRIVATE_TRACEEVENT_HPP
