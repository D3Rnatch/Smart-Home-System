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


class ASRManager
{
public :
	ASRManager(const int);
	~ASRManager();

	void close();

	void stop_listening();

	bool start_listening();

private :
	XMLParserNamespace::XMLParser * _pParser;
	ASR *_pASR;
	AudioReader * _pInput;
	NetworkClientManager * _pNetClient;
	RingBuffer * _pBuffer;
};