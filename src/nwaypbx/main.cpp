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
#include<stdio.h>
#ifdef WIN32
#include <io.h>
#else
//#include <syswait.h>
#include <unistd.h>
#include <sys/io.h>
#endif
#include <stdlib.h>
#include <esl.h>

#include "nwaypbx/structdef.h"
#include "nwaypbx/init_sys.h"
#include "common/log/log.h"
#include "common/nway-lib/PcreCpp.h"
//char guuid[128];
#define disp_msg  //printf
#include <pthread.h>
#include <string>
#include <vector>
#include "common/nway-lib/NwayStr.h"
 
using namespace std;
list<base_config> lstBaseConfig;
list<NwayExtensionGroup> lstExtensionGroup;
list<NwayExtension> lstExtension;
list<NwayCalloutGateway> lstCalloutGateways;
list<NwayGateway> lstGateway;
list<NwayDialplanDetail> lstDialplanDetail;
list<NwayDialplan> lstDialplan;

list<NwayIVRDetail> lstIvrDeatail;
list<NwayIVR> lstIvr;
list<NwayRing> lstRings;
list<Outside_line> lstOutsides;
list<In_Out_Mapping> lstInOutMapping;
list<Call_info> lstCallinfo;
list<Call_Task> lstCallTask; //用于系统中执行由outbound中产生的相关的第三方执行命令
list<Callout_Task> lstCalloutTasks; //用于系统中外呼任务的获取
pthread_mutex_t taskMutex; //即时呼叫的互斥变量
pthread_mutex_t infoMutex; //用于处理callinfo的互斥变量
pthread_mutex_t calloutMutex; // 外呼任务的互斥变量
pthread_mutex_t configMutex;  //基本配置模块的互斥变量
pthread_mutex_t clickDialMutex; //点击呼叫模块的互斥变量
Pcre regexPcre;   ///唯一的一个pcre处理模块

string BasePath;//主要是彩铃路径和存储路径
int nOutsideLen = 8;

int init_nwaypbx()
{
	int nway_res = 0;
	do
	{
		if (!init_database())
		{
			nway_res = 1;
			break;
		}
		else
			LOGEX(__FILE__, __LINE__, "init database successed");
		check_extension();
		if (!check_dialplan())
		{
			nway_res = 1;
			break;
		}
		else
			LOGEX(__FILE__, __LINE__, "init check_dialplan testing successed");
		check_dialplan_detail();
		if (!check_base_config())
		{
			nway_res = 1;
			break;
		}
		else
			LOGEX(__FILE__, __LINE__, "init check_base_config testing successed");
		check_ivr();
		check_ivr_detail();
		if (!check_esl_inboud())
		{
			nway_res = 1;
			break;
		}
		else
			LOGEX(__FILE__, __LINE__, "init esl inbound test successed");
	} while (false);
	return nway_res;
}

static inline void LogIntoFile(const char* eventname,const char* uuid1,const char* uuid2, const char* uuid3)
{
	 
}

static int event_callback(Call_info& ci, esl_handle_t* pHandle ,  esl_event_t *event, nway_uint64_t& cdrid, bool hasAnswer)
{
	 
	return 0;
}
//检查来电信息
//来电网关
//来电号码
//呼到的号码
static void nwaycc_callback(esl_socket_t server_sock, esl_socket_t client_sock, struct sockaddr_in *addr )
{
	 

}

void *Inbound_Init(void *arg)
{
	 
}
void *Task_Process(void *arg)
{

	 
 
}
//////////////////////////////////////////////////////////////////////////
//外呼处理
void *CallOut_Task_Process(void *arg)
{

	 
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void *ClickDial_Process(void *arg)
{

	 
}
void *Config_Process(void *arg)
{
	 
}
int main(void)
{
	bool bSuccess=false;
	start_log();
	if (init_nwaypbx()!0)
	{
		LOGEX(__FILE__, __LINE__, "init nway pbx failed");
		return 1;
	}
	return 0;
}


