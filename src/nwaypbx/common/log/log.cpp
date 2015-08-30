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

