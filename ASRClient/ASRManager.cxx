
#include "ASRManager.h"

ASRManager::ASRManager(const int port)
{
	this->_pNetClient = new NetworkClientManager(port);
	// this->_pParser = new XMLParserNamespace::XMLParser();
	this->_pASR = new ASR();
	this->_pInput = new AudioReader();
	this->_pBuffer = new RingBuffer(RING_BUF_SIZE);

	// once all is ready...
	this->pListener.start(ASRManager::ReceiveAudio,this);
	this->pSender.start(ASRManager::ManageAudio,this);
}

int ASRManager :: ReceiveAudio(void * j)
{
	ASRManager * This = static_cast <j> ();

	// main loop : todo
}

int ASRManager :: ManageAudio(void * j)
{
	ASRManager * This = static_cast <j> ();

	// main loop : todo
}


ASRManager::~ASRManager()
{

	// tod o
}

void ASRManager::close()
{
	// todo
}

void ASRManager :: stop_listening()
{
	// todo
}

void ASRManager :: start_listening()
{
	// todo
}