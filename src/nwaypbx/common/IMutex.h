
#ifndef _SOCKETS_IMutex_H
#define _SOCKETS_IMutex_H

class IMutex
{
public:
	virtual ~IMutex() {}

	virtual void Lock() const = 0;
	virtual void Unlock() const = 0;
};

#endif // _SOCKETS_IMutex_H


