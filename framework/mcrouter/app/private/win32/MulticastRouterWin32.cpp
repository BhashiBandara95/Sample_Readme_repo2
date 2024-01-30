/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2023 Aregtech UG. All rights reserved.
 * \file        mcrouter/app/private/MulticastRouterWin32.cpp
 * \ingroup     AREG SDK, Automated Real-time Event Grid Software Development Kit
 * \author      Artak Avetyan
 * \brief       Router, Multicast Router Service process
 ************************************************************************/
#include "mcrouter/app/MulticastRouter.hpp"

#ifdef WINDOWS

#pragma comment(lib, "areg.lib")
#pragma comment(lib, "areg-extend.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")

#include "areg/appbase/Application.hpp"
#include "areg/appbase/NEApplication.hpp"
#include "areg/base/NEUtilities.hpp"
#include "areg/base/File.hpp"
#include "areg/base/Process.hpp"
#include "areg/base/String.hpp"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif  // WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include <shellapi.h>

#include "mcrouter/resources/resource.hpp"

//////////////////////////////////////////////////////////////////////////
// Global functions, Begin
//////////////////////////////////////////////////////////////////////////
VOID WINAPI _win32ServiceMain(DWORD argc, LPTSTR * argv);
VOID WINAPI _win32ServiceCtrlHandler(DWORD);

namespace
{
#ifdef UNICODE
    /**
     * \brief   The name of the AREG mcrouter
     **/
    wchar_t * _serviceName  = NEMulticastRouterSettings::SERVICE_NAME_WIDE;
#else
    /**
     * \brief   The name of the AREG mcrouter
     **/
    char* _serviceName = NEMulticastRouterSettings::SERVICE_NAME_ASCII;
#endif // UNICODE

    /**
     * \brief   The AREG mcrouter display string.
     **/
    TCHAR _serviceDisplay[]
    {
          'A', 'R', 'E', 'G', ' '
        , 'M', 'u', 'l', 't', 'i', 'c', 'a', 's', 't', ' '
        , 'R', 'o', 'u', 't', 'e', 'r', ' '
        , 'S', 'e', 'r', 'v', 'i', 'c', 'e', '\0'
    };

    /**
     * \brief   The description to display in the list of services.
     **/
    TCHAR   _serviceDescribe[]
    {
          'A', 'R', 'E', 'G', ' '
        , 'T', 'C', 'P', ' ', '/', ' ', 'I', 'P', ' '
        , 'M', 'u', 'l', 't', 'i', 'c', 'a', 's', 't', ' '
        , 'R', 'o', 'u', 't', 'e', 'r', ' '
        , 'S', 'e', 'r', 'v', 'i', 'c', 'e'
        , 't', 'o', ' ', 'a', 'u', 't', 'o', 'm', 'a', 't', 'e', ' '
        , 'm', 'e', 's', 's', 'a', 'g', 'i', 'n', 'g', '.', '\0'
    };

    SERVICE_STATUS          _serviceStatus  { 0 };
    SERVICE_STATUS_HANDLE   _statusHandle   { nullptr };
    SERVICE_TABLE_ENTRY     _serviceTable[] { {_serviceName, &::_win32ServiceMain}, {nullptr, nullptr} };

    inline char ** _convertArguments( TCHAR ** argv, int argc )
    {
        char ** argvTemp = argc != 0 ? DEBUG_NEW char * [argc] : nullptr;
        if ( argvTemp != nullptr )
        {
            for ( int i = 0; i < static_cast<int>(argc); ++i )
            {
                TCHAR * entry = argv[i];
                uint32_t length = static_cast<uint32_t>(NEString::getStringLength<TCHAR>( entry ));
                uint32_t size = length + 1u;
                char * arg = DEBUG_NEW char[size];
                NEString::copyString<char, TCHAR>( arg, size, entry );
                argvTemp[i] = arg;
            }
        }
        return argvTemp;
    }

    inline void _deleteArguments( char ** argv, int argc )
    {
        if ( argv != nullptr )
        {
            for ( int i = 0; i < static_cast<int>(argc); ++i )
                delete[] argv[i];
            delete[] argv;
        }
    }

} // namespace

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int result      = 0;
    char ** argvTemp = _convertArguments(argv, argc);
    const char * temp = *argvTemp;
    MulticastRouter & router = MulticastRouter::getInstance( );
    if (router.parseOptions(argc, argvTemp, NESystemService::ServiceOptionSetup, MACRO_ARRAYLEN(NESystemService::ServiceOptionSetup)) == false)
    {
        router.resetDefaultOptions( );
    }

    _deleteArguments( argvTemp, argc );

    switch ( router.getCurrentOption() )
    {
    case NESystemService::eServiceOption::CMD_Install:
        result = router.serviceInstall() ? 0 : -2;
        break;

    case NESystemService::eServiceOption::CMD_Uninstall:
        router.serviceUninstall();
        break;

    case NESystemService::eServiceOption::CMD_Console:
        ::_win32ServiceMain(argc, argv);
        break;

    case NESystemService::eServiceOption::CMD_Service:
        result = ::StartServiceCtrlDispatcher(_serviceTable) ? 0 : -1;
        break;

    case NESystemService::eServiceOption::CMD_Verbose:
    case NESystemService::eServiceOption::CMD_Help:
        break;

    default:
        ASSERT(false);  // unexpected
        break;
    }

    return result;
}

VOID WINAPI _win32ServiceMain( DWORD argc, LPTSTR * argv )
{
    try
    {
        NEMemory::zeroElement<SERVICE_STATUS>( _serviceStatus );
        _serviceStatus.dwServiceType            = SERVICE_WIN32_OWN_PROCESS;
        _serviceStatus.dwCurrentState           = SERVICE_STOPPED;
        _serviceStatus.dwControlsAccepted       = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP;
        _serviceStatus.dwWin32ExitCode          = NO_ERROR;
        _serviceStatus.dwServiceSpecificExitCode= 0;
        _serviceStatus.dwCheckPoint             = 0;
        _serviceStatus.dwWaitHint               = 0;

        MulticastRouter & router = MulticastRouter::getInstance( );
        router.setState( NESystemService::eSystemServiceState::ServiceStarting );

        char ** argvTemp = _convertArguments( argv, static_cast<int>(argc) );
        router.serviceMain( static_cast<int>(argc), argvTemp );
        _deleteArguments( argvTemp, static_cast<int>(argc) );

        router.setState( NESystemService::eSystemServiceState::ServiceStopped );
    }
    catch ( const std::exception& )
    {
        ASSERT(false);
    }
}

VOID WINAPI _win32ServiceCtrlHandler(DWORD CtrlCode)
{
    switch ( CtrlCode )
    {
    case SERVICE_CONTROL_STOP:
        MulticastRouter::getInstance().serviceStop();
        break;

    case SERVICE_CONTROL_PAUSE:
        MulticastRouter::getInstance().servicePause();
        break;

    case SERVICE_CONTROL_CONTINUE:
        MulticastRouter::getInstance().serviceContinue();
        break;

    default:
        break;  // do nothing
    }
}

//////////////////////////////////////////////////////////////////////////
// Global functions, End
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// MulticastRouter class implementation
//////////////////////////////////////////////////////////////////////////
bool MulticastRouter::_osIsValid( void ) const
{
    return (mSeMHandle != nullptr && mSvcHandle != nullptr);
}

void MulticastRouter::_osFreeResources( void )
{
    if ( mSvcHandle != nullptr )
    {
        ::CloseServiceHandle( reinterpret_cast<SC_HANDLE>(mSvcHandle) );
    }

    if ( mSeMHandle != nullptr )
    {
        ::CloseServiceHandle( reinterpret_cast<SC_HANDLE>(mSeMHandle) );
    }

    mSvcHandle = nullptr;
    mSeMHandle = nullptr;
}

bool MulticastRouter::_osOpenService(void)
{
    if ( mSeMHandle == nullptr )
    {
        mSeMHandle = reinterpret_cast<void *>(::OpenSCManager( nullptr, nullptr, SC_MANAGER_ALL_ACCESS ));
    }
    
    if ( (mSeMHandle != nullptr) && (mSvcHandle == nullptr) )
    {
        mSvcHandle = reinterpret_cast<void *>(::OpenService( reinterpret_cast<SC_HANDLE>(mSeMHandle), ::_serviceName, SERVICE_ALL_ACCESS ));
    }
    
    return ( mSvcHandle != nullptr );
}

bool MulticastRouter::_osCcreateService(void)
{
    if ( mSeMHandle == nullptr )
    {
        mSeMHandle = reinterpret_cast<void *>(::OpenSCManager( nullptr, nullptr, SC_MANAGER_ALL_ACCESS ));
    }
    
    if ( (mSeMHandle != nullptr) && (mSvcHandle == nullptr) )
    {
        TCHAR szPath[MAX_PATH];
        if( ::GetModuleFileName( nullptr, szPath, MAX_PATH ) )
        {
            TEString<TCHAR> modulePath{ _T('\"') };
            modulePath += szPath;
            modulePath += _T("\" --service");

#define DEVELOPMENT_PENDING     0

#if defined(DEVELOPMENT_PENDING) && (DEVELOPMENT_PENDING != 0)
            DWORD startType = SERVICE_DEMAND_START;
#else   // defined(DEVELOPMENT_PENDING) && (DEVELOPMENT_PENDING != 0)
            DWORD startType = SERVICE_AUTO_START;
#endif  // defined(DEVELOPMENT_PENDING) && (DEVELOPMENT_PENDING != 0)

            mSvcHandle = reinterpret_cast<void *>(::CreateService(reinterpret_cast<SC_HANDLE>(mSeMHandle), _serviceName, _serviceDisplay, SERVICE_ALL_ACCESS
                                                                , SERVICE_WIN32_OWN_PROCESS, startType, SERVICE_ERROR_NORMAL, modulePath.getString()
                                                                , nullptr, nullptr, nullptr, nullptr, nullptr));
            if ( mSvcHandle != nullptr )
            {
                SERVICE_DESCRIPTION description;
                description.lpDescription = _serviceDescribe;
                ::ChangeServiceConfig2(reinterpret_cast<SC_HANDLE>(mSvcHandle), SERVICE_CONFIG_DESCRIPTION, &description);

                SERVICE_FAILURE_ACTIONS_FLAG actionFlag = {1};
                actionFlag.fFailureActionsOnNonCrashFailures = TRUE;
                ::ChangeServiceConfig2(reinterpret_cast<SC_HANDLE>(mSvcHandle), SERVICE_CONFIG_FAILURE_ACTIONS_FLAG, &actionFlag);

                constexpr uint32_t count{ 5 };
                SERVICE_FAILURE_ACTIONS failures = {0};
                SC_ACTION actions[count];

                failures.dwResetPeriod  = INFINITE;
                failures.lpRebootMsg    = nullptr;
                failures.lpCommand      = nullptr;
                failures.cActions       = count;
                failures.lpsaActions    = actions;

                // first failure
                actions[0].Delay        = NECommon::WAIT_1_SECOND;
                actions[0].Type         = SC_ACTION_RESTART;

                // first second
                actions[1].Delay        = NECommon::WAIT_1_SECOND;
                actions[1].Type         = SC_ACTION_RESTART;

                // third failure
                actions[2].Delay        = NECommon::WAIT_1_SECOND;
                actions[2].Type         = SC_ACTION_RESTART;

                // fourth failure
                actions[3].Delay        = NECommon::WAIT_1_SECOND;
                actions[3].Type         = SC_ACTION_RESTART;

                actions[count - 1].Delay= NECommon::DO_NOT_WAIT;
                actions[count - 1].Type = SC_ACTION_NONE;

                ::ChangeServiceConfig2(reinterpret_cast<SC_HANDLE>(mSvcHandle), SERVICE_CONFIG_FAILURE_ACTIONS, &failures);

#if !defined(DEVELOPMENT_PENDING) || (DEVELOPMENT_PENDING == 0)
                SERVICE_DELAYED_AUTO_START_INFO delay = {1};
                delay.fDelayedAutostart = FALSE;
                ::ChangeServiceConfig2(reinterpret_cast<SC_HANDLE>(mSvcHandle), SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &delay);
#endif // !defined(DEVELOPMENT_PENDING) || (DEVELOPMENT_PENDING == 0)
            }
        }
    }

    return ( mSvcHandle != nullptr );
}

void MulticastRouter::_osDeleteService( void )
{
    if ( mSvcHandle != nullptr )
    {
        ::DeleteService( reinterpret_cast<SC_HANDLE>(mSvcHandle) );
    }
}

bool MulticastRouter::_osRegisterService( void )
{
    if ( mSystemServiceOption == NESystemService::eServiceOption::CMD_Service )
    {
        _statusHandle = ::RegisterServiceCtrlHandler( _serviceName, _win32ServiceCtrlHandler );
    }

    return (_statusHandle != nullptr);
}

bool MulticastRouter::_osSetState( NESystemService::eSystemServiceState newState )
{
    bool result{ true };

    _serviceStatus.dwControlsAccepted   = 0;
    _serviceStatus.dwWin32ExitCode      = 0;

    if ( newState != mSystemServiceState )
    {
        switch ( newState )
        {
        case NESystemService::eSystemServiceState::ServiceStopped:
            _serviceStatus.dwCurrentState       = SERVICE_STOPPED;
            _serviceStatus.dwControlsAccepted   = 0;
            _serviceStatus.dwCheckPoint         = 7;
            _serviceStatus.dwWin32ExitCode      = ERROR_SUCCESS;
            break;

        case NESystemService::eSystemServiceState::ServiceStarting:
            _serviceStatus.dwCurrentState       = SERVICE_START_PENDING;
            _serviceStatus.dwCheckPoint         = 1;
            break;

        case NESystemService::eSystemServiceState::ServiceStopping:
            _serviceStatus.dwCurrentState       = SERVICE_STOP_PENDING;
            _serviceStatus.dwCheckPoint         = 6;
            break;

        case NESystemService::eSystemServiceState::ServiceRunning:
            _serviceStatus.dwCurrentState       = SERVICE_RUNNING;
            _serviceStatus.dwControlsAccepted   = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP;
            _serviceStatus.dwCheckPoint         = 2;
            break;

        case NESystemService::eSystemServiceState::ServiceContinuing:
            _serviceStatus.dwCurrentState       = SERVICE_CONTINUE_PENDING;
            _serviceStatus.dwCheckPoint         = 5;
            break;

        case NESystemService::eSystemServiceState::ServicePausing:
            _serviceStatus.dwCurrentState       = SERVICE_PAUSE_PENDING;
            _serviceStatus.dwCheckPoint         = 3;
            break;

        case NESystemService::eSystemServiceState::ServicePaused:
            _serviceStatus.dwCurrentState       = SERVICE_PAUSED;
            _serviceStatus.dwControlsAccepted   = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP;
            _serviceStatus.dwCheckPoint         = 4;
            break;

        default:
            ASSERT(false);
        }

        mSystemServiceState = newState;
        if ( _statusHandle != nullptr )
        {
            result = ::SetServiceStatus( _statusHandle, &_serviceStatus ) != FALSE;
        }
    }

    return result;
}

bool MulticastRouter::_osWaitUserInput(char* buffer, unsigned int bufSize)
{
    return(gets_s(buffer, bufSize) != nullptr);
}

#endif // WINDOWS
