#include "stdafx.h"
#include "Mutex.h"




Mutex::Mutex()
{
#ifdef _WIN32
	m_mutex = ::CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&m_mutex, NULL);
#endif
}


Mutex::~Mutex()
{
#ifdef _WIN32
	::CloseHandle(m_mutex);
#else
	pthread_mutex_destroy(&m_mutex);
#endif
}


void Mutex::Lock() const
{
#ifdef _WIN32
	DWORD d = WaitForSingleObject(m_mutex, INFINITE);
	/// \todo check 'd' for result
#else
	pthread_mutex_lock(&m_mutex);
#endif
}


void Mutex::Unlock() const
{
#ifdef _WIN32
	::ReleaseMutex(m_mutex);
#else
	pthread_mutex_unlock(&m_mutex);
#endif
}


