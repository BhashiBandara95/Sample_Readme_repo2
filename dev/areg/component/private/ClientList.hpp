#ifndef AREG_COMPONENT_PRIVATE_CLIENTLIST_HPP
#define AREG_COMPONENT_PRIVATE_CLIENTLIST_HPP
/************************************************************************
 * \file        areg/component/private/ClientList.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Client List declaration
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/base/TELinkedList.hpp"
#include "areg/component/private/ClientInfo.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class ProxyAddress;
class ServerInfo;

//////////////////////////////////////////////////////////////////////////
// ClientList class declaration
//////////////////////////////////////////////////////////////////////////
class ClientListImpl  : public TEListImpl<const ClientInfo &>
{
public:
    /**
     * \brief   Compares 2 values, returns true if they are equal.
     * \param   Value1  The right-side value to compare.
     * \param   Value2  The left-side value to compare.
     * \return  Returns true if 2 values are equal.
     **/
    inline bool implEqualValues( const ClientInfo & value1, const ClientInfo & value2) const
    {
        return ( value1.getAddress() == value2.getAddress() );
    }
};
/**
 * \brief   ClientList is a linked list object containing the list
 *          of Client Info objects related to one server Stub address.
 *          The object is used in Router Service to control clients.
 **/
class ClientList    : public TELinkedList<ClientInfo, const ClientInfo &, ClientListImpl>
{
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Default constructor. Creates empty list.
     **/
    ClientList( void );

    /**
     * \brief   Copy constructor.
     * \param   src     The source of data to copy.
     **/
    ClientList( const ClientList & src );

    /**
     * \brief   Destructor
     **/
    ~ClientList( void );

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Assigning operator. Copies the list of Client Info from given source
     * \param   src     The source of Client Info list
     **/
    const ClientList & operator = ( const ClientList & src );

//////////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////////
public:

    /**
     * \brief   Checks whether there is a Client Info with specified Proxy address
     *          exists in the list, and if exists, returns true.
     *          Otherwise, it returns false.
     * \param   client  The address of Proxy of client to check.
     * \return  Returns true, if there is a Client Info object in the list
     *          with specified Proxy address. Otherwise returns false.
     **/
    bool existClient( const ProxyAddress & client ) const;

    /**
     * \brief   Searches in the list Client Info object with specified address,
     *          and there is Client Info object with specified Proxy address,
     *          it will return valid Client Info. Otherwise, it will return
     *          invalid ClientInfo::INVALID_CLIENT_INFO object.
     *          Check validation of Client Info or call existClient() method.
     * \param   whichClient The address of Proxy of client to search.
     * \return  If there is a Client Info object in the list with specified address
     *          of Proxy of client, it will return valid object. Otherwise, it will return
     *          invalid ClientInfo::INVALID_CLIENT_INFO object.
     **/
    const ClientInfo & getClient( const ProxyAddress & whichClient ) const;

    /**
     * \brief   Registers client in the list by given address of Proxy and Server Info, 
     *          containing address of Stub and state of server, and returns 
     *          Client Info object of registered client.  If there is  already 
     *          Client Info object exists in the list, it will increase number
     *          of instances and return existing object. Otherwise, it will add new 
     *          Client Info element.
     * \param   whichClient The address of Proxy of client
     * \param   server      The Server Info object, containing Stub address.
     * \return  If there is already registered client in the list, it will increase
     *          the number of instances by one and will return existing Client Info
     *          object. Otherwise, it will create new Client Info entry with initial
     *          number of instances 1.
     **/
    const ClientInfo & registerClient( const ProxyAddress & whichClient, const ServerInfo & server );

    /**
     * \brief   It searches servicing Client entry in the list by the given address of Proxy.
     *          If requested proxy address found in the list of servicing client, on output, 
     *          the out_client contains information of unregistered client. Otherwise, it is unchanged.
     * \param   whichClient The address of Proxy to search
     * \param   out_client  If a servicing client with given proxy address exists, on output, this 
     *                      will contains information of unregistered client.
     * \return  Return true if found an entry with specified proxy and operation to unregister client succeeded.
     **/
    bool unregisterClient( const ProxyAddress & whichClient, ClientInfo & out_client );

    /**
     * \brief   Call to notify all pending clients the servicing server availability.
     *          The function changes states of waiting for connection clients and on output, 
     *          the out_clientList parameter contains list of all clients.
     * \param   whichServer     The servicing server helper object, which is available.
     * \param   out_clientList  On output it contains list of connected clients.
     **/
    void serverAvailable( const ServerInfo & whichServer, ClientList & out_clientList );

    /**
     * \brief   Called to change the state of connected clients when server is disconnected. 
     *          If there are connected clients, on output out_clientList parameter contains list of
     *          disconnected clients.
     * \param   out_clientList  On output, this contains list of disconnected client.
     **/
    void serverUnavailable( ClientList & out_clientList );
};

#endif  // AREG_COMPONENT_PRIVATE_CLIENTLIST_HPP
