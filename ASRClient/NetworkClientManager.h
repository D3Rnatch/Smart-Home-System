#pragma once

#include "SocketServer.h"

class NetworkClientManager : public SocketServer
{
public :
	NetworkClientManager();
	~NetworkClientManager();

	void start_client();
	void close_client();

	int send_xml(std::string);
};