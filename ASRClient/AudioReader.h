#pragma once

/* TODO :
HEADER INCLUDES TO BE DEFINED
*/

#include "sync.h"
#include "RingBuffer.h"


class AudioReader
{
public :
	static AudioReader& instance(){ static AudioReader e; return e; }
	~AudioReader();

	// to be runned on separated thread...
	static run();

private :
	AudioReader();
	RingBuffer * _pBuffer;
};