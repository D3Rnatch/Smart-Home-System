#ifndef _SYNC_H
#define _SYNC_H
#pragma once


#include "Types.h"

#ifdef __linux__
    #define INFINITE	    (u32(-1))
    #define WAIT_OBJECT_0   0
    #define WAIT_TIMEOUT    1
    #include <semaphore.h>
#else
    #include <windows.h>
#endif

#ifdef __linux__
    #include <pthread.h>
    #include <sys/time.h>
    #include <unistd.h>
#endif


/* TODO :
    -add multiple object linux version with pthread_cond_broadcast function.
*/


//=============================================================================
//	[ cMutex ]
//=============================================================================
class cMutex
{
public:
    cMutex(const char szName[] = NULL);
    virtual ~cMutex();

    bool lock(u32 timeoutMs = INFINITE);
    bool unlock();

private:
    // no assignment operator nor copy ctor
    cMutex(const cMutex&);
    cMutex& operator=(const cMutex&);

    #ifdef __linux__
        typedef pthread_mutex_t HANDLE;
    #endif

    HANDLE  _mutex;
};

//=============================================================================
//	[ cGuardMutex ]
//=============================================================================
class cGuardMutex
{
public:
    cGuardMutex(cMutex	&mutex,
				u32		timeoutMs = INFINITE);
    virtual ~cGuardMutex();

private:
    // no assignment operator nor copy ctor
    cGuardMutex(const cGuardMutex&);
    cGuardMutex& operator=(const cGuardMutex&);

    cMutex	&_mutex;
};

//=============================================================================
//	[ cSemaphore ]
//=============================================================================
class cSemaphore
{
public:
    cSemaphore(bool locked = false);
    virtual ~cSemaphore();

    bool wait(u32 timeoutMs = INFINITE);
    bool post();

    #ifdef __linux__
        typedef sem_t   HANDLE;
    #endif

private:
    // no assignment operator nor copy ctor
    cSemaphore(const cSemaphore&);
    cSemaphore& operator=(const cSemaphore&);

	HANDLE  _semaphore;
};

//=============================================================================
//	[ cEvent ]
//=============================================================================
class cEvent
{
public:
    cEvent();
    virtual ~cEvent();

    int  wait(u32 timeoutMs = INFINITE);
    void set();
    void reset();

private:
    // no assignment operator nor copy ctor
    cEvent(const cEvent&);
    cEvent& operator=(const cEvent&);

    #ifdef __linux__
	    typedef struct {
            pthread_mutex_t _mutex;
            pthread_cond_t  _cond;
            bool            _signaled;
	    }   HANDLE;
    #endif

	HANDLE  _evt;
};

//=============================================================================
//	[ cThread ]
//=============================================================================
class cThread
{
public:
    cThread() : _thread(0)
              , _fct(0)    {}
    virtual ~cThread()     {stop();}

    typedef int (*Fct_t)(void* param);
    bool start( Fct_t   fct,
                void*   param);
    bool stop();

private:

    // no assignment operator nor copy ctor
    cThread(const cThread&);
    cThread& operator=(const cThread&);

    #ifdef __linux__
	    typedef pthread_t   HANDLE;
        static void*        _run(void*  param);
    #else
        static DWORD WINAPI _run(void*  param);
    #endif

	HANDLE   _thread;
    Fct_t    _fct;
    void*    _param;
};
#endif
