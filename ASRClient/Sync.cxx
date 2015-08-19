

#include <stdlib.h>
#ifdef __linux__
    #include <pthread.h>
    #include <sys/time.h>
    #include <unistd.h>
#endif
#include "sync.h"

#include <stdio.h>
#include <errno.h>

#include "Types.h"

//=============================================================================
//	[ cMutex ]
//=============================================================================

// ----------------------------------------------------------------------------
cMutex::cMutex(
    const char szName[]
)
// ----------------------------------------------------------------------------
{
#ifdef __linux__
	(void) szName;
    pthread_mutexattr_t ptmat;
    for (;;)
    {
        if (pthread_mutexattr_init(&ptmat) != 0)                             break;
        if (pthread_mutexattr_settype(&ptmat, PTHREAD_MUTEX_RECURSIVE) != 0) break;
        if (pthread_mutex_init(&_mutex, &ptmat) == 0)	                     return;
        break;
    }
    pthread_mutexattr_destroy(&ptmat);
#elif defined IP_MANAGER
	if ((_mutex = RtCreateMutex( NULL, false, szName )) != NULL)   return;
#else
	if ((_mutex = CreateMutex( NULL, false, (LPCWSTR)szName )) != NULL)     return;
#endif
    exit(-1);
}

// ----------------------------------------------------------------------------
cMutex::~cMutex()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    pthread_mutex_destroy(&_mutex);
#elif defined IP_MANAGER
    RtCloseHandle(_mutex);
#else
    CloseHandle(_mutex);
#endif
}

// ----------------------------------------------------------------------------
bool
cMutex::lock(
    u32 timeoutMs
)
// ----------------------------------------------------------------------------
{
#ifdef __linux__
	(void) timeoutMs;
    return pthread_mutex_lock(&_mutex) == 0;
#elif defined IP_MANAGER
    return RtWaitForSingleObject(_mutex, timeoutMs) == WAIT_OBJECT_0;
#else
    return WaitForSingleObject(_mutex, timeoutMs) == WAIT_OBJECT_0;
#endif
}

// ----------------------------------------------------------------------------
bool cMutex::unlock()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    return pthread_mutex_unlock(&_mutex) == 0;
#elif defined IP_MANAGER
	return RtReleaseMutex(_mutex) != 0;
#else
	return ReleaseMutex(_mutex) != 0;
#endif
}

//=============================================================================
//	[ cGuardMutex ]
//=============================================================================

// ----------------------------------------------------------------------------
cGuardMutex::cGuardMutex(
    cMutex	&mutex,
    u32		timeoutMs
)
: _mutex(mutex)
// ----------------------------------------------------------------------------
{
    _mutex.lock(timeoutMs);
}

// ----------------------------------------------------------------------------
cGuardMutex::~cGuardMutex()
// ----------------------------------------------------------------------------
{
    _mutex.unlock();
}

//=============================================================================
//	[ cSemaphore ]
//=============================================================================

// ----------------------------------------------------------------------------
cSemaphore::cSemaphore(
	bool    locked
)
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    if (sem_init(&_semaphore, 0, (locked ? 0 : 1)) == 0)	return;
#elif defined IP_MANAGER
    if ((_semaphore = RtCreateSemaphore( NULL, (locked ? 0 : 1), 1, NULL )) != NULL) return;
#else
    if ((_semaphore = CreateSemaphore( NULL, (locked ? 0 : 1), 1, NULL )) != NULL)   return;
#endif
    exit(-1);
}

// ----------------------------------------------------------------------------
cSemaphore::~cSemaphore()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    sem_destroy(&_semaphore);
#elif defined IP_MANAGER
    RtCloseHandle(_semaphore);
#else
    CloseHandle(_semaphore);
#endif
}

// ----------------------------------------------------------------------------
bool
cSemaphore::wait(
	u32	timeoutMs
)
// ----------------------------------------------------------------------------
{
#ifdef __linux__
	(void) timeoutMs;
    return sem_wait(&_semaphore) == 0;
#elif defined IP_MANAGER
    return RtWaitForSingleObject(_semaphore, timeoutMs) == WAIT_OBJECT_0;
#else
    return WaitForSingleObject(_semaphore, timeoutMs) == WAIT_OBJECT_0;
#endif
}

// ----------------------------------------------------------------------------
bool
cSemaphore::post()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    return sem_post(&_semaphore) == 0;
#elif defined IP_MANAGER
	return RtReleaseSemaphore(_semaphore, 1, NULL) != 0;
#else
	return ReleaseSemaphore(_semaphore, 1, NULL) != 0;
#endif
}

//=============================================================================
//	[ cEvent ]
//=============================================================================
// ----------------------------------------------------------------------------
cEvent::cEvent()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    _evt._signaled = false;
    if (    (pthread_mutex_init(&_evt._mutex, NULL) == 0)
         && (pthread_cond_init(&_evt._cond, NULL) == 0) )   return;
#elif defined IP_MANAGER
	_evt = RtCreateEvent(NULL, FALSE, FALSE, NULL);
    if (_evt)    return;
#else
	_evt = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (_evt)    return;
#endif
    exit(-1);
}

// ----------------------------------------------------------------------------
cEvent::~cEvent()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    pthread_mutex_lock(&_evt._mutex);
    pthread_cond_signal(&_evt._cond);
    pthread_mutex_unlock(&_evt._mutex);

    pthread_cond_destroy(&_evt._cond);
    pthread_mutex_destroy(&_evt._mutex);
#elif defined IP_MANAGER
    RtCloseHandle(_evt);
#else
    CloseHandle(_evt);
#endif
}

// ----------------------------------------------------------------------------
int
cEvent::wait(
    u32 timeoutMs
)
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    int  res = 0;
    pthread_mutex_lock(&_evt._mutex);
    if (_evt._signaled == false)
    {
        res = -1;
        if (timeoutMs == INFINITE)
        {
            //Infinite timeout
            res = ::pthread_cond_wait(&_evt._cond, &_evt._mutex);
        }
        else
        {
            //defined timeout
            div_t div_res = div(timeoutMs, 1000) ;
            struct timeval  crtTime;

            if (gettimeofday(&crtTime, NULL) == 0)
            {
                struct timespec waitTo;

                waitTo.tv_sec = crtTime.tv_sec + div_res.quot; // timeout in sec whereas timeoutMS in Ms.
                waitTo.tv_nsec = (crtTime.tv_usec  + div_res.rem * 1000 ) * 1000; //timeout in nsec
                res = ::pthread_cond_timedwait(&_evt._cond, &_evt._mutex, &waitTo);
            }
        }
    }

    if (res == 0)   _evt._signaled = false;
    pthread_mutex_unlock(&_evt._mutex);
    return (res == 0) ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
#elif defined IP_MANAGER
    return RtWaitForSingleObject(_evt, timeoutMs);
#else
    return WaitForSingleObject(_evt, timeoutMs);
#endif
}

// ----------------------------------------------------------------------------
void
cEvent::set()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    pthread_mutex_lock(&_evt._mutex);
    pthread_cond_signal(&_evt._cond);
    _evt._signaled = true;
    pthread_mutex_unlock(&_evt._mutex);
#elif defined IP_MANAGER
    RtSetEvent(_evt);
#else
    SetEvent(_evt);
#endif
}

// ----------------------------------------------------------------------------
void
cEvent::reset()
// ----------------------------------------------------------------------------
{
#ifdef __linux__
    pthread_mutex_lock(&_evt._mutex);
    pthread_cond_signal(&_evt._cond);
    _evt._signaled = false;
    pthread_mutex_unlock(&_evt._mutex);
#elif defined IP_MANAGER
    RtResetEvent(_evt);
#else
    ResetEvent(_evt);
#endif
}

//=============================================================================
//	[ cThread ]
//=============================================================================
// ----------------------------------------------------------------------------
bool
cThread::start(
    Fct_t   fct,
    void*   param
)
// ----------------------------------------------------------------------------
{
    if (_thread)    stop();
    _fct = fct;
    _param = param;
#ifdef __linux__
    if (pthread_create(&_thread, NULL, _run, this) != 0)   _thread = 0;
#elif defined IP_MANAGER
    _thread = RtCreateThread(NULL, 0, _run, this, 0, NULL);
#else
    _thread = CreateThread(NULL, 0, _run, this, 0, NULL);
#endif
    return (_thread != 0);
}

// ----------------------------------------------------------------------------
bool
cThread::stop()
// ----------------------------------------------------------------------------
{
    if (_thread != 0)
    {
#ifdef __linux__
        pthread_join(_thread, NULL);
#elif defined IP_MANAGER
        RtWaitForSingleObject(_thread, INFINITE);
        RtCloseHandle(_thread);
#else
        WaitForSingleObject(_thread, INFINITE);
        CloseHandle(_thread);
#endif
    }
    _thread = 0;
    _fct = 0;
    _param = NULL;
    return true;
}

// ----------------------------------------------------------------------------
#ifdef __linux__
    void*
#else
    DWORD WINAPI
#endif
cThread::_run(
    void *param
)
// ----------------------------------------------------------------------------
{
    cThread *This = (cThread*)param;
#ifdef __linux__
    return (void*)This->_fct(This->_param);
#else
    return This->_fct(This->_param);
#endif
}

