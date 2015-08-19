#include "RingBuffer.h"


RingBuffer::RingBuffer(int size) : mutex("lock"),size(size)
{

	this->buffer = (char *)malloc(sizeof(char)*size);
	if (this->buffer == NULL) std::cout << " WARNING : ERROR ON SIZE ALLOCATION. " << std::endl;
	this->_read_offset = this->_write_offset = 0;

}

RingBuffer :: ~RingBuffer()
{
	cGuardMutex e(this->mutex);
	free(this->buffer);
}

int RingBuffer :: getSize () { return this->size; }

char * RingBuffer :: getBuff () { return this->buffer; }

int RingBuffer :: getReadOffset()
{
	cGuardMutex e(this->mutex);
	return this->_read_offset;
}


int RingBuffer :: getWriteOffset()
{
	cGuardMutex e(this->mutex);
	return this->_write_offset;
}

bool RingBuffer :: moveWriteOffset(int s) {
	if (this->mutex.lock()){

		this->_write_offset += s;
		//if (this->_write_offset >= this->size) this->_write_offset = 0;
		if (this->_write_offset >= this->size) this->_write_offset = this->_write_offset - this->size;

		// release of mutex
		this->mutex.unlock();

		return true;
	}
	return false;
}

bool RingBuffer :: moveReadOffset(int s)
{
	if (this->mutex.lock()){

		// size est la taille lisible du malloc donc un élement en +
		// Il faut donc prendre comme taille maximale : size -1 = dernière case du tableau.
		this->_read_offset += s;
		// if (this->_read_offset >= this->size) this->_read_offset = 0;
        if (this->_read_offset >= this->size) this->_read_offset = this->_read_offset - this->size;


		// release of mutex
		this->mutex.unlock();

		return true;
	}
	return false;
}
