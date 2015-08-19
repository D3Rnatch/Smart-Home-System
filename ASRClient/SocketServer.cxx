#include "SocketServer.h"

#include <cstdlib>
#include <iostream>


SocketServer :: SocketServer(const unsigned short Port)
{
	this->port = Port;
	this->status = false;
	this->log.open("readsizelog.csv");
}

bool SocketServer :: cbind()
{
	struct sockaddr_in serv_addr;
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(this->port);

  ::bind(socket_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  return true;
}

bool SocketServer :: clisten()
{
	if(::listen(this->socket_fd, 10) == -1){
      printf("Failed to listen\n");
	  printf("Error code is : -1\n");
      return false;
  }
	return true;
}
bool SocketServer :: caccept()
{
	for(;;){
		this->client_fd = ::accept(this->socket_fd, (struct sockaddr*)NULL ,NULL);
		break;
	}
	return true;
}

bool SocketServer :: csend(const std::string& message)
{
	char e[10];
	if(this->status){
        sprintf(e,"%d",message.size());
		//itoa(message.size(),e,10);
		std::string msg = e;
		msg += ":";
		msg += message;
		if( SOCKET_ERROR == ::send( this->client_fd, msg.c_str(), int(msg.size()), 0 ) )
		{
			return false;
		}
		else
			return true;
	}
	else
		return false;
}

bool SocketServer :: csend(int value)
{
    if(this->status)
    {
        if(SOCKET_ERROR == ::send( this->client_fd, &value, sizeof(value), 0) )
        {
            return false;
        }
        else
            return true;
    }
    else
        return false;
}

void SocketServer :: stop()
{
    shutdown(this->client_fd,2);
    shutdown(this->socket_fd,2);
//	closesocket(this->client_fd);
//	closesocket(this->socket_fd);
}

bool SocketServer :: csend(void *buffer,size_t size)
{
	return ( INVALID_SOCKET != this->client_fd )
		&& ( SOCKET_ERROR != ::send( this->client_fd, static_cast<const char*>(buffer), int(size), 0 ) );
}

int SocketServer :: csend_r(char * buffer, size_t size)
{
    int readsize = -1;
    if(INVALID_SOCKET != this->client_fd && SOCKET_ERROR != (readsize = ::send( this->client_fd, buffer, int(size), 0 )))
        return readsize;
    else
        return -1;
}

int SocketServer :: crecv(void* buffer, size_t expectedSize)
{
	if( INVALID_SOCKET != this->client_fd )
    {
      char* buf = static_cast< char* >( buffer );
      char* const bufStart = buf;
      do
      {
        int bytes_read;
		if( SOCKET_ERROR == ( bytes_read = ::recv( this->client_fd, buf, int(expectedSize-(buf-bufStart)), 0 ) ) )
        {
          return SOCKET_ERROR;
        }
        if( 0 == bytes_read )
        {
          // connection has been gracefully closed
          return 0;
        }
        buf += bytes_read;
      } while( size_t(buf-bufStart) < expectedSize );
      return expectedSize;
    }
    return -1;
}

int SocketServer :: creceive(void* buffer, size_t expectedSize)
{
		int readsize = 0;
	if(INVALID_SOCKET != this->client_fd){
        this->log << expectedSize << ";";
		char* buf = static_cast< char* >( buffer );
		readsize = ::recv(this->client_fd,buf,expectedSize,0);
		this->log << readsize << std::endl;
		return readsize;
	}
	return readsize;
}

int SocketServer :: creceive(int *value)
{
    int readsize = 0;
    if(INVALID_SOCKET != this->client_fd){
        readsize = ::recv(this->client_fd,value, sizeof(int),0);
        return readsize;
    }
    return readsize;
}

bool SocketServer :: close()
{
	return true;
}


/*
SocketServer::SocketServer(const unsigned short Port)
{
	this->port = Port;
	this->status = false;

   wVersionRequested = MAKEWORD(2, 2);

   int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return;
    }
}

bool SocketServer :: cbind()
{
	struct sockaddr_in serv_addr;
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(this->port);

  ::bind(socket_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  return true;
}

bool SocketServer :: clisten()
{
	if(::listen(this->socket_fd, 10) == -1){
      printf("Failed to listen\n");
	  int e = WSAGetLastError();
	  printf("Error code is : %d\n", e);
      return false;
  }
	return true;
}

bool SocketServer :: caccept()
{
	for(;;){
		this->client_fd = ::accept(this->socket_fd, (struct sockaddr*)NULL ,NULL);
		break;
	}
	return true;
}

bool SocketServer :: csend(const std::string& message)
{
	char e[10];
	if(this->status){
		_itoa_s(message.size(),e,10);
		std::string msg = e;
		msg += ":";
		msg += message;
		if( SOCKET_ERROR == ::send( this->socket_fd, msg.c_str(), int(msg.size()), 0 ) )
		{
			return false;
		}
		else
			return true;
	}
	else
		return false;
}

void SocketServer :: stop()
{
	closesocket(this->client_fd);
	closesocket(this->socket_fd);
}

bool SocketServer :: csend(void *buffer,size_t size)
{
	return ( INVALID_SOCKET != this->socket_fd )
		&& ( SOCKET_ERROR != ::send( this->socket_fd, static_cast<const char*>(buffer), int(size), 0 ) );
}

int SocketServer :: crecv(void* buffer, size_t expectedSize)
{
	if( INVALID_SOCKET != this->client_fd )
    {
      char* buf = static_cast< char* >( buffer );
      char* const bufStart = buf;
      do
      {
        int bytes_read;
		if( SOCKET_ERROR == ( bytes_read = ::recv( this->client_fd, buf, int(expectedSize-(buf-bufStart)), 0 ) ) )
        {
          return SOCKET_ERROR;
        }
        if( 0 == bytes_read )
        {
          // connection has been gracefully closed
          return 0;
        }
        buf += bytes_read;
      } while( size_t(buf-bufStart) < expectedSize );
      return expectedSize;
    }
    return -1;
}

int SocketServer :: creceive(void* buffer, size_t expectedSize)
{
		int readsize = 0;
	if(INVALID_SOCKET != this->client_fd){
		char* buf = static_cast< char* >( buffer );
		readsize = ::recv(this->client_fd,buf,expectedSize,0);
		return readsize;
	}
	return readsize;

	/*if( INVALID_SOCKET != this->socket_fd )
    {
      char* buf = static_cast< char* >( buffer );
      char* const bufStart = buf;
      do
      {
        int bytes_read;
		if( SOCKET_ERROR == ( bytes_read = ::recv( this->client_fd, buf, int(expectedSize-(buf-bufStart)), 0 ) ) )
        {
          return bytes_read;
        }
        if( 0 == bytes_read )
        {
          // connection has been gracefully closed
          return bytes_read;
        }
        buf += bytes_read;
      } while( size_t(buf-bufStart) < expectedSize );
      return expectedSize;
    }
    return false;
}

bool SocketServer :: close()
{
	return true;
} */
