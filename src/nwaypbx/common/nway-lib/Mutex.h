/*************************************************************************
based for NwayPBX
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License£º GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/
#ifndef _NWAY__Mutex_H
#define _NWAY__Mutex_H


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



