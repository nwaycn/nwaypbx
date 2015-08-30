/*************************************************************************
based for NwayPBX
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License： GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/
#include "stdafx.h"
#include "log.h"
int start_log()
{
	CNwayLog* aLog = CNwayLog::GetInstance();
	return 0;
}
int stop_log()
{
	CNwayLog* aLog = CNwayLog::GetInstance();
	aLog->Release();
	return 0;
}

