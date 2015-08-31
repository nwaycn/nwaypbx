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
#ifndef _NWAY_IMutex_H
#define _NWAY_IMutex_H

class IMutex
{
public:
	virtual ~IMutex() {}

	virtual void Lock() const = 0;
	virtual void Unlock() const = 0;
};

#endif // _SOCKETS_IMutex_H


