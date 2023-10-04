#ifndef AREG_IPC_NECONNECTION_HPP
#define AREG_IPC_NECONNECTION_HPP
/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2023 Aregtech UG. All rights reserved.
 * \file        areg/ipc/NEConnection.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan
 * \brief       AREG Platform Connection constants.
 ************************************************************************/

/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/appbase/NEApplication.hpp"
#include "areg/base/NEMemory.hpp"
#include "areg/component/NEService.hpp"
#include "areg/component/Timer.hpp"
#include "areg/ipc/NERemoteService.hpp"

#include <string_view>

/************************************************************************
 * Dependencies
 ************************************************************************/
class StubAddress;
class ProxyAddress;
class RemoteMessage;
class Channel;

//////////////////////////////////////////////////////////////////////////
// NEConnection namespace declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   The connection namespace with shared objects and helper methods
 *          to create fixed communication messages.
 **/
namespace NEConnection
{
    /**
     * \brief   Fixed predefined message to start server connection
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageHelloServer( void );
    /**
     * \brief   Fixed predefined message to stop server connection
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageByeServer( void );
    /**
     * \brief   Fixed message to notify client connection acceptance
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageAcceptClient( void );
    /**
     * \brief   Fixed message to notify client connected rejection
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageRejectClient( void );
    /**
     * \brief   Fixed message to notify client disconnect
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageByeClient( void );
    /**
     * \brief   Fixed message to start service registration
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageRegisterService( void );
    /**
     * \brief   Fixed message to query service the connected instances. Only observers can query.
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageQueryInstances( void );
    /**
     * \brief   Fixed message to notify observers the connected instances. Only observers receive the message.
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageNotifyInstances(void);
    /**
     * \brief   Fixed message to register notification
     **/
    AREG_API const NEMemory::sRemoteMessage & getMessageRegisterNotify( void );

    /**
     * \brief   NEConnection::CLIENT_SEND_MESSAGE_THREAD
     *          Fixed name of client message sender thread
     **/
    constexpr std::string_view  CLIENT_SEND_MESSAGE_THREAD      { "CLIENT_SEND_MESSAGE_THREAD" };
    /**
     * \brief   NEConnection::CLIENT_RECEIVE_MESSAGE_THREAD
     *          Fixed name of client message receiver thread
     **/
    constexpr std::string_view  CLIENT_RECEIVE_MESSAGE_THREAD   { "CLIENT_RECEIVE_MESSAGE_THREAD" };
    /**
     * \brief   NEConnection::CLIENT_DISPATCH_MESSAGE_THREAD
     *          Fixed name of client message dispatcher thread
     **/
    constexpr std::string_view  CLIENT_DISPATCH_MESSAGE_THREAD  { "CLIENT_DISPATCH_MESSAGE_THREAD" };
    /**
     * \brief   NEConnection::SERVER_SEND_MESSAGE_THREAD
     *          Fixed name of server message sender thread
     **/
    constexpr std::string_view  SERVER_SEND_MESSAGE_THREAD      { "SERVER_SEND_MESSAGE_THREAD" };
    /**
     * \brief   NEConnection::SERVER_RECEIVE_MESSAGE_THREAD
     *          Fixed name of server message receiver thread
     **/
    constexpr std::string_view  SERVER_RECEIVE_MESSAGE_THREAD   { "SERVER_RECEIVE_MESSAGE_THREAD" };
    /**
     * \brief   NEConnection::SERVER_DISPATCH_MESSAGE_THREAD
     *          Fixed name of server message dispatcher thread
     **/
    constexpr std::string_view  SERVER_DISPATCH_MESSAGE_THREAD  { "SERVER_DISPATCH_MESSAGE_THREAD" };
    /**
     * \brief   NEConnection::CLIENT_CONNECT_TIMER_NAME
     *          Fixed name of client connection retry timer name
     **/
    constexpr std::string_view  CLIENT_CONNECT_TIMER_NAME       { "CLIENT_CONNECT_TIMER_NAME" };
    /**
     * \brief   NEConnection::SERVER_CONNECT_TIMER_NAME
     *          Fixed name of server connection retry timer name
     **/
    constexpr std::string_view  SERVER_CONNECT_TIMER_NAME       { "SERVER_CONNECT_TIMER_NAME" };
    /**
     * \brief   NEConnection::DEFAULT_REMOTE_SERVICE_HOST
     *          Default IP-address of remote routing service.
     *          The default values are used if failed to read and parse router configuration file.
     **/
    constexpr std::string_view  DEFAULT_REMOTE_SERVICE_HOST     { NERemoteService::DEFAULT_ROUTER_HOST };
    /**
     * \brief   NEConnection::DEFAULT_REMOTE_SERVICE_PORT
     *          Default connection port number of remote routing service
     *          The default values are used if failed to read and parse router configuration file.
     **/
    constexpr unsigned short    DEFAULT_REMOTE_SERVICE_PORT     { NERemoteService::DEFAULT_ROUTER_PORT };
    /**
     * \brief   NEConnection::DEFAULT_RETRY_CONNECT_TIMEOUT
     *          Default connect retry timer timeout value in milliseconds
     **/
    constexpr unsigned int      DEFAULT_RETRY_CONNECT_TIMEOUT   { NECommon::TIMEOUT_500_MS };  // 500 ms
    /**
     * \brief   NEConnection::DEFAULT_REMOTE_SERVICE_ENABLED
     *          Message router enable / disable default flag. If true, by default it is enabled.
     *          The default values are used if failed to read and parse router configuration file.
     **/
    constexpr  bool              DEFAULT_REMOTE_SERVICE_ENABLED { true };

    /**
     * \brief   NEConnection::CreateConnectRequest
     *          Initializes connection request message.
     * \param   target  The ID of the target to send the connect message request.
     **/
    AREG_API RemoteMessage createConnectRequest(const ITEM_ID & target);
    /**
     * \brief   NEConnection::CreateDisconnectRequest
     *          Initializes disconnect request message.
     * \param   source  The ID of the source that sends the disconnect message request.
     * \param   target  The ID of the target to send the disconnect message request.
     **/
    AREG_API RemoteMessage createDisconnectRequest( const ITEM_ID & source, const ITEM_ID & target );
    /**
     * \brief   NEConnection::CreateConnectNotify
     *          Initializes connection notification message
     * \param   source  The ID of the source that sends the connect notification message.
     * \param   target  The ID of the target to send the connect notification message.
     **/
    AREG_API RemoteMessage createConnectNotify( const ITEM_ID & source, const ITEM_ID & target );
    /**
     * \brief   NEConnection::CreateRejectNotify
     *          Initializes connection rejected message
     * \param   source  The ID of the source that sends the connection rejected message.
     * \param   target  The ID of the target to send the connection rejected message.
     **/
    AREG_API RemoteMessage createRejectNotify( const ITEM_ID & source, const ITEM_ID & target );
    /**
     * \brief   NEConnection::CreateDisconnectNotify
     *          Initializes disconnect notification message sent by server to clients.
     * \param   source  The ID of the source that sends the disconnect notification message.
     * \param   target  The ID of the target to send the disconnect notification message.
     **/
    AREG_API RemoteMessage createDisconnectNotify( const ITEM_ID & source, const ITEM_ID & target );
    /**
     * \brief   NEConnection::createRouterRegisterService
     *          Initializes Stub register message at router.
     * \param   stub    The address of remote Stub service, which is registering.
     * \param   source  The ID of the source that sends the request message to register service provider.
     * \param   target  The ID of the target to send the request message to register service provider.
     * \return  Returns register stub message to send.
     * \see     createRouterUnregisterService
     **/
    AREG_API RemoteMessage createRouterRegisterService( const StubAddress & stub, const ITEM_ID & source, const ITEM_ID & target);
    /**
     * \brief   NEConnection::createRouterUnregisterService
     *          Initializes Stub unregister message at router.
     * \param   stub    The address of remote Stub service, which is registering.
     * \param   reason  The unregister / disconnect reason.
     * \param   source  The ID of the source that sends the request message to unregister service provider.
     * \param   target  The ID of the target to send the request message to unregister service provider.
     * \return  Returns unregister stub message to send.
     * \see     createRouterRegisterService
     **/
    AREG_API RemoteMessage createRouterUnregisterService( const StubAddress & stub, NEService::eDisconnectReason reason, const ITEM_ID & source, const ITEM_ID & target);
    /**
     * \brief   NEConnection::createRouterRegisterClient
     *          Initializes Proxy register message at router.
     * \param   proxy   The address of remote Proxy, which is registering.
     * \param   source  The ID of the source that sends the request message to register service consumer.
     * \param   target  The ID of the target to send the request message to register service consumer.
     * \return  Returns register proxy message to send.
     * \see     createRouterUnregisterClient
     **/
    AREG_API RemoteMessage createRouterRegisterClient( const ProxyAddress & proxy, const ITEM_ID & source, const ITEM_ID & target );
    /**
     * \brief   NEConnection::createRouterUnregisterClient
     *          Initializes Proxy unregister message at router.
     * \param   proxy   The address of remote Proxy, which is registering.
     * \param   reason  The unregister / disconnect reason.
     * \param   source  The ID of the source that sends the request message to unregister service consumer.
     * \param   target  The ID of the target to send the request message to unregister service consumer.
     * \return  Returns unregister proxy message to send.
     * \see     createRouterRegisterClient
     **/
    AREG_API RemoteMessage createRouterUnregisterClient( const ProxyAddress & proxy, NEService::eDisconnectReason reason, const ITEM_ID & source, const ITEM_ID & target);
    /**
     * \brief   NEConnection::createServiceRegisteredNotification
     *          Initializes Stub registered notification message to broadcast.
     * \param   stub    The address of remote Stub to notify registering.
     * \param   source  The ID of the source that sends the service provider is registered notification message.
     * \param   target  The ID of the target to send the service provider is registered notification message.
     * \return  Returns registered stub notification message to broadcast.
     * \see     createServiceUnregisteredNotification
     **/
    AREG_API RemoteMessage createServiceRegisteredNotification( const StubAddress & stub, const ITEM_ID & source, const ITEM_ID & target );
    /**
     * \brief   NEConnection::createServiceUnregisteredNotification
     *          Initializes Stub service unregistering notification message to broadcast.
     * \param   stub    The address of remote Stub to notify unregistering.
     * \param   reason  The unregister / disconnect reason.
     * \param   source  The ID of the source that sends the service provider is unregistered notification message.
     * \param   target  The ID of the target to send the service provider is unregistered notification message.
     * \return  Returns unregister stub notification message to broadcast.
     * \see     createServiceRegisteredNotification
     **/
    AREG_API RemoteMessage createServiceUnregisteredNotification( const StubAddress & stub, NEService::eDisconnectReason reason, const ITEM_ID & source, const ITEM_ID & target);
    /**
     * \brief   NEConnection::createServiceClientRegisteredNotification
     *          Initializes service proxy registering notification message to broadcast.
     * \param   proxy   The address of remote Proxy to notify registering.
     * \param   source  The ID of the source that sends the service consumer is registered notification message.
     * \param   target  The ID of the target to send the service consumer is registered notification message.
     * \return  Returns service proxy registering notification message to broadcast.
     * \see     createServiceClientUnregisteredNotification
     **/
    AREG_API RemoteMessage createServiceClientRegisteredNotification( const ProxyAddress & proxy, const ITEM_ID & source, const ITEM_ID & target);
    /**
     * \brief   NEConnection::createServiceClientUnregisteredNotification
     *          Initializes service proxy unregistering notification message to broadcast.
     * \param   proxy   The address of remote Proxy to notify unregistering.
     * \param   reason  The unregister / disconnect reason.
     * \param   source  The ID of the source that sends the service consumer is unregistered notification message.
     * \param   target  The ID of the target to send the service consumer is unregistered notification message.
     * \return  Returns service proxy unregistering notification message to broadcast
     * \see     createServiceClientRegisteredNotification
     **/
    AREG_API RemoteMessage createServiceClientUnregisteredNotification( const ProxyAddress & proxy, NEService::eDisconnectReason reason, const ITEM_ID & source, const ITEM_ID & target);
    /**
     * \brief   NEConnection::isMessageHelloServer
     *          Checks whether specified message is a connect request.
     * \param   msgHelloServer      The instance of remote message to check.
     * \return  Returns true if the message is connect request.
     **/
    AREG_API bool isMessageHelloServer( const RemoteMessage & msgHelloServer );
    /**
     * \brief   NEConnection::isMessageByeServer
     *          Checks whether specified message is a disconnect request.
     * \param   msgByeServer        The instance of remote message to check.
     * \return  Returns true if the message is disconnect request.
     **/
    AREG_API bool isMessageByeServer( const RemoteMessage & msgByeServer );
    /**
     * \brief   NEConnection::isMessagNotifyClient
     *          Checks whether specified message is client notification request.
     * \param   msgNotifyClient     The instance of remote message to check.
     * \return  Returns true if the message is a client notification request.
     **/
    AREG_API bool isMessagNotifyClient( const RemoteMessage & msgNotifyClient );
    /**
     * \brief   NEConnection::isMessageRegisterService
     *          Checks whether specified message is service notification request.
     * \param   msgRegisterService  The instance of remote message to check.
     * \return  Returns true if the message is a service notification request.
     **/
    AREG_API bool isMessageRegisterService( const RemoteMessage & msgRegisterService );
}

#endif  // AREG_IPC_NECONNECTION_HPP
