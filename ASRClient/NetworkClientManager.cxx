

#include "NetworkClientManager.h"


NetworkClientManager::NetworkClientManager(const int port) : SocketServer(port)
{
}

NetworkClientManager::~NetworkClientManager()
{
	// to do

}

void NetworkClientManager::start_client()
{
	if(!this->cconnect()){
		std::cerr << "ERROR ON CONNECTION" << std::endl;
		this->connected = false;
	}
	else
		this->connected = true;
}

void NetworkClientManager :: close_client()
{

	// todo

}

int NetworkClientManager :: send_xml(std::string buf)
{
	if(this->connected)
		return this->csend_r(buf);
	return -1;
}

