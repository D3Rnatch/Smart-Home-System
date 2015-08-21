
#pragma once

#ifndef SOCKET_ASR_H
#define SOCKET_ASR_H

// LINUX INCLUDES :
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
// #include <linux/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

/* TODO :
    - fix
*/



/*! \class SocketServer
* \brief Socket Wrapper implementation.
*/
class SocketServer
{
public :
/*! \brief SocketServer : constructor
*
* SocketServer : constructor
* \param port : server listening port
*/
    SocketServer(const unsigned short port);
    ~SocketServer();

    bool clisten();
    bool caccept();
    bool cbind();
	// todo
	bool cconnect();

    void stop();

/*! \brief crecv : receive socket wrapper function
*
* crecv : receive socket wrapper function. This function reads
* the amount specified by expectedSize. Blocking function.
*
* \param buffer : receive buffer
* \param expectedSize : size max to read
* \return read size : if -1 : client disconnected.
*/
    int crecv(void *buffer, size_t expectedSize);

/*! \brief creceive : receive socket wrapper function
*
* creceive : receive socket wrapper function. This function reads at max
* the amount specified by expectedSize.  Reads less, if less.
*
* \param buffer : receive buffer
* \param expectedSize : size max to read
* \return read size : if -1 : client disconnected.
*/
    int creceive(void *buffer, size_t expectedSize);

    int creceive(int *);

/*! \brief csend : sends string data through client socket.
*
*/
    bool csend(const std::string&);

/*! \brief csend : sends any data type through client socket.
*
*/
    bool csend(void *buffer,size_t size);

    bool csend(int value);
    int csend_r(char * buffer, size_t size);

    bool close();

private :
    short socket_fd;
    short client_fd;
    bool status;
    unsigned short port;
    std::ofstream log;
};


/*
#else

#include "stdafx.h"

class SocketServer
{
public :
    SocketServer(const unsigned short port);
    inline ~SocketServer() {}
    bool clisten();
    bool caccept();
    bool cbind();

    // todo
    void stop();

    int crecv(void* buffer, size_t expectedSize);
    int creceive(void *buffer,size_t expectedSize);
    bool csend(const std::string&);
    bool csend(void *buffer,size_t size);

    bool close();

private :
    short socket_fd;
    short client_fd;
    bool status;
    unsigned short port;

    // WSA STart
    WORD wVersionRequested;
    WSADATA wsaData;
};

*/
#endif // _H
