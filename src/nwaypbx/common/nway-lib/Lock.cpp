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

