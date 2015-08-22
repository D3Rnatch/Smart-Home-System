#pragma once

#include "SocketServer.h"

class NetworkClientManager : public SocketClient
{
public :
	NetworkClientManager(const int);
	~NetworkClientManager();

	void start_client();
	void close_client();

	int send_xml(std::string);

	inline bool get_status() { return connected; }

private : 
	bool connected;
};