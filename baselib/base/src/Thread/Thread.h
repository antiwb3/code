//---------------------------------------------------------------------------
// File:	Thread.h
// Date:	2013.04.14
// Code:	Wang Bin
// Desc:
// modify: 
//---------------------------------------------------------------------------

#ifndef THREAD_H
#define THREAD_H

typedef void THREAD_FUNCTION(void *pvArg);

#ifdef WIN32
#include <WTypes.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#ifndef ENGINE_API
	#define ENGINE_API
#endif

class ENGINE_API Thread
{
public:
    int             Create(THREAD_FUNCTION *pfnThread, void *pvArg);
    int             Destroy(int bFocreEnd = false);

public:
	Thread();
	~Thread();

    void            ThreadFunction();

private:
    #ifdef WIN32
    HANDLE          m_ThreadHandle;
    #else
    pthread_t       m_ThreadHandle;  
    #endif

	THREAD_FUNCTION* m_pfnThread;
    void *			 m_pvThreadArg;
};

ENGINE_API int  Thread_Sleep(unsigned uMilliseconds);

#ifdef WIN32

inline void *Thread_GetSelfId()
{
    return (void *)((char *)NULL + GetCurrentThreadId());   // for no 64 bit check warning
}

#else     //linux

inline void *Thread_GetSelfId()
{
    return (void *)pthread_self();
}

#endif

#endif //THREAD_H
