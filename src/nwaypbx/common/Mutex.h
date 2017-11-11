
#ifndef _SOCKETS_Mutex_H
#define _SOCKETS_Mutex_H


#ifndef _WIN32
#include <pthread.h>
#else

#include <windows.h>
#endif
#include "IMutex.h"


/** Mutex container class, used by Lock. 
	\ingroup threading */
class Mutex //: public IMutex
{
public:
	//Mutex(const char* threadName);
	~Mutex();
    Mutex();
	virtual void Lock() const;
	virtual void Unlock() const;

private:
	
#ifdef _WIN32
	HANDLE m_mutex;
#else
	mutable pthread_mutex_t m_mutex;
#endif
};


#ifdef SOCKETS_NAMESPACE
}
#endif
#endif // _SOCKETS_Mutex_H



