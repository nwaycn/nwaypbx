
#ifndef _SOCKETS_Lock_H
#define _SOCKETS_Lock_H


class IMutex;

/** IMutex encapsulation class. 
	\ingroup threading */
class Lock
{
public:
	Lock(const IMutex&);
	~Lock();

private:
	const IMutex& m_mutex;
};




#endif // _SOCKETS_Lock_H

