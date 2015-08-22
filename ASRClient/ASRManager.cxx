
#include "ASRManager.h"

ASRManager::ASRManager(const int port) : stop(false)
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
	ASRManager * This = static_cast <ASRManager *> (j);

	// main loop : todo
}

int ASRManager :: ManageAudio(void * j)
{
	ASRManager * This = static_cast <ASRManager *> (j);
	std::ofstream log;
	log.open("ASRManager_managelog.logfile");
	
	bool ready = This->_pNetClient->start_client();
	if(ready == false) {
		log << "error on setting, connexion failed." << std::endl;
		log << "thread ending." << std::endl;
		return -1;
	}
	// once this is made....
	// we start logging stuff
	log << " ASRManager :: ManageAudio : thread started. \n Client is connected." << std::endl;
	while(This->stop!=true)
	{
		// waiting for there is data event...
		This->dataReady.wait();

		if(This->stop) break;

		// Now we got data...
		std::string out_asr;
		// todo : make ASR recognition block (see how to do so). 
			while(1)
			{

			int toRead = This->_pBuffer.getWriteOffset();
			int actRead = This->_pBuffer.getReadOffset();

			int readsize = 0;

				if (actRead != toRead)
				{
					// log << "data ready." << actRead << ":" << toRead << std::endl;
					if(actRead > toRead)
					{
						readsize = This->_pBuffer.getSize() - actRead;
					}
					else if (actRead < toRead )
					{
						readsize = toRead - actRead;
					}

					// log << "readsize : " << readsize  << "\n";
					max_d = std::max(max_d, readsize);

					if(readsize != 0)
					{
						// output.write(&This->_pBuffer.getBuff()[actRead], readsize);
						// log << "before handle data" << std::endl;
						This->_pASR->make_recognition(&This->_pBuffer()[actRead],readsize); // this needs to be real time !!!
						// to be studied...
						//This->pipe->handle_data(&This->_pBuffer.getBuff()[actRead], readsize);
						This->_pBuffer.moveReadOffset(readsize);
					}
				}

				else{
					// log << "out of while." << std::endl;
					break;
				}
			}
		
		
		// once recognition is made, then we prse it into xml and then push to main system.
		std::string u = This->_pParser->parse_string(out_asr);
		
		// send through socket...
		This->_pNetClient->send_xml(u);
	}

	log << "Ending thread now, because of signal stop." << std::endl;
	return 0;
}


ASRManager::~ASRManager()
{

	// to do
}

void ASRManager::close()
{
	// to do
}

void ASRManager :: stop_listening()
{
	// todo
}

void ASRManager :: start_listening()
{
	// todo
}