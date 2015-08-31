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
#ifndef _NWAY_Lock_H
#define _NWAY_Lock_H


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

