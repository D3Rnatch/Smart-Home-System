#pragma once

#include <fstream>
#include <iostream>

// Mutex, events and threads implementations...
#include "sync.h"

// For audio data coming in...
#include "RingBuffer.h"

// Node module
#include "ASR.h"
#include "AudioReader.h"
#include "XMLParser.h"
#include "NetworkClientManager.h"

#define RING_BUF_SIZE 2097152 // 2*1024*1024

class ASRManager
{
public :
	// destructor
	~ASRManager();

	// function to shutdown...
	void close();

	// function to stop listening...
	void stop_listening();

	// function to start listening...
	bool start_listening();

	// Signleton process since we have two threads...
	ASRManager& instance() { static ASRManager MANAGER(4242); return MANAGER; }

	// function for receiving audio input (thread0) sets dataReady event 
	static int ReceiveAudio(void *);

	// function for reading audio input (thread1) waits dataReady event
	static int ManageAudio(void *);

private :
	cEvent pause;
	cEvent dataReady; // event poped when data ready on circular buffer...
	ASRManager(const int);
	XMLParserNamespace::XMLParser * _pParser;
	ASR *_pASR;
	AudioReader * _pInput;
	NetworkClientManager * _pNetClient;
	RingBuffer * _pBuffer;
	cThread _pListener;
	cThread _pSender;
};