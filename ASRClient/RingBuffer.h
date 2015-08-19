#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H

#pragma once

#include <iostream>
#include <stdlib.h>
#include "sync.h"

class RingBuffer
{
public :
	RingBuffer(int size);
	~RingBuffer();

	int getSize();
	char *getBuff();

	int getReadOffset();
	int getWriteOffset();

	bool moveWriteOffset(int);
	bool moveReadOffset(int);

private :
	char * buffer;
	int size;
	int _read_offset;
	int _write_offset;
	cMutex mutex;
};


#endif
