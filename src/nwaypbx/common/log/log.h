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
#ifndef __SG_LOG_H
#define __SG_LOG_H
#pragma once
//#include "CRTLog.h"
//CCRTLog* crtlog= NULL;
#include "NwayLog.h"
int start_log();

int stop_log();

#define LOG(x) CNwayLog::GetInstance()->Write(x)
//#ifndef _DEBUG
//#define LOGDBG(x)
//#else
#define LOGDBG(x) CNwayLog::GetInstance()->Write("debug: %s",x)
//#endif
#define LOGERR(x) CNwayLog::GetInstance()->Write("error: %s",x)
#ifdef DATABASE_DEBUG
#define LOGDB(x)
#else
#define LOGDB(x) CNwayLog::GetInstance()->Write("db: %s",x)
#endif

#define LOGEX(x,y,z) CNwayLog::GetInstance()->Write("file: %s,%d %s",x,y,z)
#define LOGERREX(x,y,z)	CNwayLog::GetInstance()->Write("error: %s,%d %s",x,y,z)
#define LOGDBEX(x,y,z) CNwayLog::GetInstance()->Write("db: %s,%d %s",x,y,z)
#define LOGEXT(x,y,z,f) CNwayLog::GetInstance()->Write("file: %s,%d %s,number:%s",x,y,z,f)
#define LOGERREXT(x,y,z,f) CNwayLog::GetInstance()->Write("error: %s,%d %s,number:%s",x,y,z,f)
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
#define print_msg printf
#else
#define print_msg
#endif

#endif

