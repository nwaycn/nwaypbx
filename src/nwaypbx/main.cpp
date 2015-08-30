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
#include "esl/esl.h"
#include "nwaycti/nwaycti.h"
#include "nwaycti/DirectCall.h"
#include "nwaycti/NwayIVR.h"
#include "nwaycti/structdef.h"
#include "Check_Content.h"
#include "log/log.h"
#include "common/PcreCpp.h"
//char guuid[128];
#define disp_msg  //printf
#include <pthread.h>
#include <string>
#include <vector>
#include "common/NwayStr.h"
#include "process_event.h"
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
	bSuccess = InitDatabase();
	pthread_mutex_init(&taskMutex, NULL); 
	pthread_mutex_init(&infoMutex, NULL); 
	pthread_mutex_init(&calloutMutex,NULL);
	pthread_mutex_init(&configMutex, NULL);
	pthread_mutex_init(&clickDialMutex, NULL);
	if (bSuccess)
	{
		//只有数据库初始化成功才有意义，否则没必要执行
		bSuccess = LoadConfigFromDb(  lstBaseConfig,   lstExtensionGroup,  lstExtension, \
			lstGateway,   lstDialplanDetail,   lstDialplan, \
			lstIvrDeatail,   lstIvr,   lstRings, lstOutsides, lstInOutMapping, lstCalloutGateways);


		if (bSuccess)
		{
			int nStatus = GetBasePath(BasePath,lstBaseConfig);
			nOutsideLen = GetOutlineLen(lstBaseConfig ,configMutex);//给的号码的长度
			//初始化正则验证模块Pcre
			list<NwayDialplan>::iterator it = lstDialplan.begin();
			for (; it!= lstDialplan.end(); it++)
			{
				NwayDialplan ndobj = *it;
				printf("%s,  %d the pcre number%s\n",__FILE__, __LINE__,ndobj.dialplan_number.c_str());
				regexPcre.AddRule(ndobj.id,ndobj.dialplan_number);
			}
			//////////////////////////////////////////////////////////////////////////
			//init inbound module
			int ret = 0;
			pthread_t pthid1, pthid2,pthid3,pthid4,pthid5;
			ret= pthread_create(&pthid1,NULL, Inbound_Init, NULL);
			if(ret)       // 非0则创建失败
			{
				perror("createthread 1 failed.\n");
				return 1;
			}
			//////////////////////////////////////////////////////////////////////////
			ret= pthread_create(&pthid2,NULL, Task_Process, NULL);
			if(ret)       // 非0则创建失败
			{
				perror("createthread 2 failed.\n");
				return 1;
			}
			ret= pthread_create(&pthid3,NULL, CallOut_Task_Process, NULL);
			if(ret)       // 非0则创建失败
			{
				perror("createthread 3 failed.\n");
				return 1;
			}
			ret= pthread_create(&pthid4,NULL, Config_Process, NULL);
			if(ret)       // 非0则创建失败
			{
				perror("createthread 4 failed.\n");
				return 1;
			}
			ret= pthread_create(&pthid5,NULL, ClickDial_Process, NULL);
			if(ret)       // 非0则创建失败
			{
				perror("createthread 5 failed.\n");
				return 1;
			}
			//////////////////////////////////////////////////////////////////////////
			LOGEX(__FILE__,__LINE__,"load regex rule module successed");
			esl_global_set_default_logger(ESL_LOG_LEVEL_INFO);
			esl_listen_threaded("localhost", 8040, nwaycc_callback,100000);
			//////////////////////////////////////////////////////////////////
			//Inbound 启动
			pthread_join(pthid1,NULL);    
			pthread_join(pthid2,NULL);
			pthread_join(pthid3,NULL);
			pthread_join(pthid4,NULL);
			pthread_join(pthid5,NULL);

		}
		else
			LOGERREX(__FILE__,__LINE__,"read config from database failed");
	}
	else
		LOGERREX(__FILE__,__LINE__,"initialize database failed");

	pthread_mutex_destroy(&infoMutex);  
	pthread_mutex_destroy(&taskMutex);  
	pthread_mutex_destroy(&calloutMutex);
	pthread_mutex_destroy(&configMutex);
	pthread_mutex_destroy(&clickDialMutex);
	return 0;
}


