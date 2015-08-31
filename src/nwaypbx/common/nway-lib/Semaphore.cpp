#include "stdafx.h"
#include "Semaphore.h"


#ifdef SOCKETS_NAMESPACE
namespace SOCKETS_NAMESPACE {
#endif


// ---------------------------------------------------------------
#ifdef _WIN32
//Semaphore::Semaphore()
//{
//	m_handle = ::CreateSemaphore((LPSECURITY_ATTRIBUTES)NULL, 1, 1, (LPCTSTR)NULL);
//}
Semaphore::Semaphore(value_t start_val)
{
	m_handle = ::CreateSemaphore((LPSECURITY_ATTRIBUTES)NULL, start_val, 1, (LPCTSTR)NULL);
}


Semaphore::~Semaphore()
{
	::CloseHandle(m_handle);
}


int Semaphore::Post()
{
	return (::ReleaseSemaphore(m_handle, 1, (LPLONG)NULL) != 0) ? 0 : -1;
}


int Semaphore::Wait()
{
	return (WaitForSingleObject(m_handle, INFINITE) == WAIT_OBJECT_0) ? 0 : -1;
}


int Semaphore::TryWait()
{
	return -1; // %! not implemented
}


int Semaphore::GetValue(int& i)
{
	return 0; // %! not implemented
}

// ---------------------------------------------------------------
#else

Semaphore::Semaphore(value_t start_val)
{
	sem_init(&m_sem, 0, start_val);
}


Semaphore::~Semaphore()
{
	sem_destroy(&m_sem);
}


int Semaphore::Post()
{
	return sem_post(&m_sem);
}


int Semaphore::Wait()
{
	return sem_wait(&m_sem);
}


int Semaphore::TryWait()
{
	return sem_trywait(&m_sem);
}


int Semaphore::GetValue(int& i)
{
	return sem_getvalue(&m_sem, &i);
}
//Semaphore::Semaphore()
//{
//	sem_init(&m_sem, 0, 1);
//}
#endif






