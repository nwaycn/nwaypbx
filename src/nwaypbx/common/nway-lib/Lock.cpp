#include "stdafx.h"
#include "IMutex.h"
#include "Lock.h"



Lock::Lock(const IMutex& m) : m_mutex(m)
{
	m_mutex.Lock();
}


Lock::~Lock()
{
	m_mutex.Unlock();
}

