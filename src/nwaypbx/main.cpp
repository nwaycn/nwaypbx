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
#include "nwaypbx/nwaydbload.h"
#include "common/log/log.h"
#include "nwaypbx/DirectCall.h"
#include "nwaypbx/NwayIVR.h"
#include "common/nway-lib/PcreCpp.h"
//char guuid[128];
#define disp_msg  //printf
#include <pthread.h>
#include <string>
#include <vector>
#include "common/nway-lib/NwayStr.h"
#include "Check_Content.h"
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
//检查来电信息
//来电网关
//来电号码
//呼到的号码
static void nwaycc_callback(esl_socket_t server_sock, esl_socket_t client_sock, struct sockaddr_in *addr)
{
	char  uuid[128];

	char cmd_tmp[128], dtmf[128];
	char recordfilename[128];
	Call_info ci;
	esl_handle_t handle = { { 0 } };

	esl_attach_handle(&handle, client_sock, addr);

	//disp_msg("Connected! %d", handle.sock);

	esl_filter(&handle, "unique-id", esl_event_get_header(handle.info_event, "caller-unique-id"));

	strcpy(uuid, esl_event_get_header(handle.info_event, "caller-unique-id"));

	disp_msg("handle.sock=%d,UUID=%s", handle.sock, uuid);
	//////////////////////////////////////////////////////////////////////////
	//originate {origination_uuid=xxxxx}user/60001 &bridge({origination_uuid=yyyyyyyy}user/60002)

	esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "CHANNEL_CREATE CHANNEL_DESTROY CHANNEL_STATE CHANNEL_CALLSTATE CHANNEL_PROGRESS_MEDIA CHANNEL_OUTGOING SESSION_HEARTBEAT CHANNEL_ANSWER CHANNEL_ORIGINATE CHANNEL_PROGRESS CHANNEL_HANGUP "
		"CHANNEL_BRIDGE CHANNEL_UNBRIDGE CHANNEL_OUTGOING CHANNEL_HANGUP_COMPLETE CHANNEL_EXECUTE CHANNEL_EXECUTE_COMPLETE DTMF CUSTOM "
		"TALK OUTBOUND_CHAN INBOUND_CHAN RECORD_START RECORD_STOP PLAYBACK_START PLAYBACK_STOP  conference::maintenance");

	//This will send a command and place its response event on handle->last_sr_event and handle->last_sr_reply
	//esl_send_recv(&handle, "linger");//Tells FreeSWITCH not to close the socket connect when a channel hangs up. Instead, it keeps the socket connection open until the last event related to the channel has been received by the socket client. 

	/*esl_execute(&handle, "answer", NULL, NULL);
	if(check_event(&handle,2)<0)*/
	string caller_uuid;
	string content_type;
	string event_name;
	string call_direction;
	string b_uuid;
	string channel_state;
	string caller_destination_number;
	string caller_username;
	string caller_number;
	string channel_call_state;
	string gateway_name = "";
	string is_callout = "";
	int nStatus = 0;
	bool hasAnswer = false;
	CCallDetailRecord cdr;//通话清单操作
						  //////////////////////////////////////////////////////////////////////////
						  //我用的只有origination_uuid、Unique-ID。现在的处理是首先接受channel_create事件，记录下号码跟uuid的对应关系。然后后面接收到progress\answer\hangup的时候，直接匹配Unique-ID，就能对应上了。
						  //////////////////////////////////////////////////////////////////////////
	caller_uuid = esl_event_get_header(handle.info_event, "Caller-Unique-ID") ? esl_event_get_header(handle.info_event, "Caller-Unique-ID") : "";
	content_type = esl_event_get_header(handle.info_event, "Content-Type") ? esl_event_get_header(handle.info_event, "Content-Type") : "";
	event_name = esl_event_get_header(handle.info_event, "Event-Name") ? esl_event_get_header(handle.info_event, "Event-Name") : "";
	call_direction = esl_event_get_header(handle.info_event, "Call-Direction") ? esl_event_get_header(handle.info_event, "Call-Direction") : "";
	b_uuid = esl_event_get_header(handle.info_event, "Unique-ID") ? esl_event_get_header(handle.info_event, "Unique-ID") : "";
	channel_state = esl_event_get_header(handle.info_event, "Channel-State") ? esl_event_get_header(handle.info_event, "Channel-State") : "";
	caller_destination_number = esl_event_get_header(handle.info_event, "Caller-Destination-Number") ? esl_event_get_header(handle.info_event, "Caller-Destination-Number") : "";
	caller_username = esl_event_get_header(handle.info_event, "Caller-Username") ? esl_event_get_header(handle.info_event, "Caller-Username") : "";
	caller_number = esl_event_get_header(handle.info_event, "Caller-Caller-ID-Number") ? esl_event_get_header(handle.info_event, "Caller-Caller-ID-Number") : "";
	channel_call_state = esl_event_get_header(handle.info_event, "Channel-Call-State") ? esl_event_get_header(handle.info_event, "Channel-Call-State") : "";
	//printf(esl_event_get_header(handle.info_event, "Gateway-Name"));
	//printf(esl_event_get_header(handle.info_event, "variable_is_callout"));
	gateway_name = esl_event_get_header(handle.info_event, "Gateway-Name") ? esl_event_get_header(handle.info_event, "Gateway-Name") : "";
	is_callout = esl_event_get_header(handle.info_event, "variable_is_callout") ? esl_event_get_header(handle.info_event, "variable_is_callout") : ""; // 可以为1则是正在外呼的
																																					   //////////////////////////////////////////////////////////////////////////
																																					   //处理call info
	if (call_direction == "inbound")
	{
		ci.call_direction = CALL_DIRECTION_NORMAL;
	}
	else
		ci.call_direction = CALL_DIRECTION_CALLOUT;

	ci.id = 0;
	ci.caller_uuid = caller_uuid;
	ci.caller_state = CALLIN_RINGING;
	ci.caller_number = caller_number;
	ci.cdr_id = 0;
	ci.called_number = caller_destination_number;
	ci.caller_callin_tm = time(NULL);

	//////////////////////////////////////////////////////////////////////////
	//生成一条新的通话记录
	nway_uint64_t cdrid = 0;
	//CCallDetailRecord cdr;
	nStatus = cdr.StartCall(caller_username.c_str(), caller_number.c_str(), caller_destination_number.c_str(), cdrid);
	ci.cdr_id = cdrid;
	//lstCallinfo.push_back(ci);
	//生成一条记录日志
	printf("%s,%d  has a call in from %s to %s \n\n", __FILE__, __LINE__, caller_number.c_str(), caller_destination_number.c_str());
	if (is_callout == "1")
	{
		//走外呼流程的，暂不处理
		printf("%s, %d\tcaller_destination_number=%s, caller_number=%s\n ", __FILE__, __LINE__, caller_destination_number.c_str(), caller_number.c_str());
		printf("%s, %d\tthis is a call out task\n\n", __FILE__, __LINE__);

		esl_disconnect(&handle);
	}
	else
	{
		//不是群呼
		//限制并发数
		if (lstCallinfo.size() >30)
		{
			esl_execute(&handle, "hangup", NULL, NULL);
			return;
		}
		//外线呼入的
		//呼入的，走dialplan匹配
		//加一个主叫长度判断，从外呼入的，需要另外配对，相当于处理did或public.xml中的流程

		string strExtension = caller_destination_number;

		if (caller_number.length() > 6 && caller_destination_number.length() >6)
		{
			//外线的超过6位的呼叫到sip或e1中继的超过6位的小号上
			//由外呼入的
			nStatus = GetInline(caller_destination_number, strExtension, lstOutsides, configMutex);
			if (nStatus == 1)
			{
				//以上已是有返回值
			}
			else
			{
				//没有配外线呼入的号码的
				strExtension = "";
			}
			NwayExtension ext;
			nStatus = GetExtension(strExtension, ext, lstExtension, configMutex);
			printf("%s,%d  process the extension transfer:%s\n", __FILE__, __LINE__, strExtension.c_str());

			if (nStatus == 1)
			{
				//找到了分机
				vector<MatchResult> results = regexPcre.MatchAllRule(strExtension.c_str());
				switch (ext.extension_type)
				{
				case 1:
					//直线分机
				{
					ci.call_info_type = 0;
					ci.trans_number = caller_destination_number;
					pthread_mutex_lock(&infoMutex);
					lstCallinfo.push_back(ci);
					pthread_mutex_unlock(&infoMutex);
					if (results.size()>0)
					{
						printf("%s, %d\tdirect call process,a-leg: %s\n", __FILE__, __LINE__, caller_uuid.c_str());
						DirectCall(handle, cdrid, results[0].id, strExtension.c_str(), caller_uuid.c_str(), BasePath.c_str(), \
							lstDialplan, lstDialplanDetail, lstRings, lstExtension, results, hasAnswer, lstCallTask, taskMutex, caller_number.c_str(), configMutex);
					}
					else
					{
						printf("%s, %d\tmatchResult results size <0\n", __FILE__, __LINE__);
						esl_execute(&handle, "hangup", NULL, NULL);
					}
					InsertCallEvent(ci.cdr_id, ext.id, ci.caller_number, ci.trans_number, ci.trans_number, CALL_EVENT_A_LEG_CALL);
				}
				break;
				case 2:
					//ivr分机
				{
					ci.call_info_type = 1;
					ci.called_number = caller_destination_number;
					pthread_mutex_lock(&infoMutex);
					lstCallinfo.push_back(ci);
					pthread_mutex_unlock(&infoMutex);
					if (results.size()>0)
					{
						printf("%s, %d\tivr all process\n", __FILE__, __LINE__);
						//hasAnswer = true;
						NwayIVRCall(handle, cdrid, strExtension.c_str(), caller_uuid.c_str(), BasePath.c_str(), lstDialplan, \
							lstDialplanDetail, lstRings, lstExtension, results, lstIvr, lstIvrDeatail, hasAnswer, lstCallTask, taskMutex, caller_number.c_str(), configMutex);
					}
					else
					{
						printf("%s, %d\tmatchResult results size <0\n", __FILE__, __LINE__);
						esl_execute(&handle, "hangup", NULL, NULL);
					}
					InsertCallEvent(ci.cdr_id, ext.id, ci.caller_number, ci.trans_number, ci.called_number, CALL_EVENT_A_LEG_CALL);
				}
				break;
				default:
					;
				}
			}

		}
		else
		{
			//查询由内呼外的
			//查询主叫是分机
			NwayExtension dest_ext, caller_ext;  //caller_ext为来电分机，需要查询 ,dest_ext是b leg 分机
			int caller_ext_status = 0;
			int dest_ext_status = 0;

			caller_ext_status = GetExtension(caller_number, caller_ext, lstExtension, configMutex);
			dest_ext_status = GetExtension(caller_destination_number, dest_ext, lstExtension, configMutex);
			if (caller_ext_status == 1 && dest_ext_status == 1)
			{
				//代表分机打分机
				strExtension = caller_destination_number;
				vector<MatchResult> results = regexPcre.MatchAllRule(strExtension.c_str());
				switch (dest_ext.extension_type)
				{
				case 1:
					//直线分机
				{
					ci.call_info_type = 0;
					pthread_mutex_lock(&infoMutex);
					lstCallinfo.push_back(ci);
					pthread_mutex_unlock(&infoMutex);
					esl_execute(&handle, "pre_answer", NULL, NULL);
					if (results.size()>0)
					{
						//printf("direct call process,a-leg: %s\n" , caller_uuid.c_str());
						DirectCall(handle, cdrid, results[0].id, strExtension.c_str(), caller_uuid.c_str(), BasePath.c_str(), \
							lstDialplan, lstDialplanDetail, lstRings, lstExtension, results, hasAnswer, lstCallTask, taskMutex, caller_number.c_str(), configMutex);
					}
					else
					{
						printf("%s, %d\tmatchResult results size <0\n", __FILE__, __LINE__);
						esl_execute(&handle, "hangup", NULL, NULL);
					}

				}
				break;
				case 2:
					//ivr分机
				{
					ci.call_info_type = 1;
					pthread_mutex_lock(&infoMutex);
					lstCallinfo.push_back(ci);
					pthread_mutex_unlock(&infoMutex);
					if (results.size()>0)
					{
						printf("%s, %d\tivr all process\n", __FILE__, __LINE__);
						//hasAnswer = true;
						NwayIVRCall(handle, cdrid, strExtension.c_str(), caller_uuid.c_str(), BasePath.c_str(), lstDialplan, \
							lstDialplanDetail, lstRings, lstExtension, results, lstIvr, lstIvrDeatail, hasAnswer, lstCallTask, taskMutex, caller_number.c_str(), configMutex);
					}
					else
					{
						printf("%s, %d\tmatchResult results size <0\n", __FILE__, __LINE__);
						esl_execute(&handle, "hangup", NULL, NULL);
					}
				}
				break;
				default:
					;
				}
			}
			else
			{
				if (caller_ext_status == 1 && dest_ext_status == 0)
				{
					//主叫是分机，被叫不是分机，有可能是相关的快捷按键或拨号规则
					printf("%s,%d, call out from %s to %s \n", __FILE__, __LINE__, caller_number.c_str(), caller_destination_number.c_str());
					if (caller_destination_number.length()>1)
					{
						//意味着是外线，进行外线路由
						vector<MatchResult> results = regexPcre.MatchAllRule(caller_destination_number.c_str());
						ci.call_info_type = 0;
						pthread_mutex_lock(&infoMutex);
						lstCallinfo.push_back(ci);
						pthread_mutex_unlock(&infoMutex);
						if (results.size()>0)
						{
							printf("%s, %d\tcall out to %s \n", __FILE__, __LINE__, caller_destination_number.c_str());
							DirectCall(handle, cdrid, results[0].id, strExtension.c_str(), caller_uuid.c_str(), BasePath.c_str(), \
								lstDialplan, lstDialplanDetail, lstRings, lstExtension, results, hasAnswer, lstCallTask, taskMutex, caller_number.c_str(), configMutex);
						}
						else
						{
							printf("%s, %d\toutside line matchResult results size <0\n", __FILE__, __LINE__);
							esl_execute(&handle, "hangup", NULL, NULL);
						}
					}
				}
				else
				{
					//主叫不是分机，被叫也不是分机，同时由先前的条件过滤了
					printf("%s,%d  cannot found this call or dest number ,so hangup it\n", __FILE__, __LINE__);
					esl_execute(&handle, "hangup", NULL, NULL);
				}
			}


		}
		//获取分机，得到它是何种类型的分机


		while ((esl_recv(&handle)) == ESL_SUCCESS) {
			const char *type = esl_event_get_header(handle.last_event, "content-type");
			if (type && !strcasecmp(type, "text/event-plain")) {
				//event_callback();
				event_callback(ci, &handle, handle.last_ievent, cdrid, hasAnswer);
			}
		}


	}
	//通过比对被叫来确认走什么流程，但需要额外加个参数，如果是主动外呼的，则需要和座席相对接

	esl_disconnect(&handle);

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
int init_nwaypbx()
{
	bool bSuccess = false;
	start_log();
	//bSuccess = InitDatabase();

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
		pthread_mutex_init(&taskMutex, NULL);
		pthread_mutex_init(&infoMutex, NULL);
		pthread_mutex_init(&calloutMutex, NULL);
		pthread_mutex_init(&configMutex, NULL);
		pthread_mutex_init(&clickDialMutex, NULL);
		bSuccess = LoadConfigFromDb(lstBaseConfig, lstExtensionGroup, lstExtension, \
			lstGateway, lstDialplanDetail, lstDialplan, \
			lstIvrDeatail, lstIvr, lstRings, lstOutsides, lstInOutMapping, lstCalloutGateways);
		if (bSuccess)
		{
			int nStatus = GetBasePath(BasePath, lstBaseConfig);
			nOutsideLen = GetOutlineLen(lstBaseConfig, configMutex);//给的号码的长度
																	//初始化正则验证模块Pcre
			list<NwayDialplan>::iterator it = lstDialplan.begin();
			for (; it != lstDialplan.end(); it++)
			{
				NwayDialplan ndobj = *it;
				printf("%s,  %d the pcre number%s\n", __FILE__, __LINE__, ndobj.dialplan_number.c_str());
				regexPcre.AddRule(ndobj.id, ndobj.dialplan_number);
			}
			//////////////////////////////////////////////////////////////////////////
			//init inbound module
			int ret = 0;
			pthread_t pthid1, pthid2, pthid3, pthid4, pthid5;
			ret = pthread_create(&pthid1, NULL, Inbound_Init, NULL);
			if (ret)       // 非0则创建失败
			{
				perror("createthread 1 failed.\n");
				return 1;
			}
			//////////////////////////////////////////////////////////////////////////
			ret = pthread_create(&pthid2, NULL, Task_Process, NULL);
			if (ret)       // 非0则创建失败
			{
				perror("createthread 2 failed.\n");
				return 1;
			}
			ret = pthread_create(&pthid3, NULL, CallOut_Task_Process, NULL);
			if (ret)       // 非0则创建失败
			{
				perror("createthread 3 failed.\n");
				return 1;
			}
			ret = pthread_create(&pthid4, NULL, Config_Process, NULL);
			if (ret)       // 非0则创建失败
			{
				perror("createthread 4 failed.\n");
				return 1;
			}
			ret = pthread_create(&pthid5, NULL, ClickDial_Process, NULL);
			if (ret)       // 非0则创建失败
			{
				perror("createthread 5 failed.\n");
				return 1;
			}
			//////////////////////////////////////////////////////////////////////////
			LOGEX(__FILE__, __LINE__, "load regex rule module successed");
			esl_global_set_default_logger(ESL_LOG_LEVEL_INFO);
			esl_listen_threaded("localhost", 8040, (esl_listen_callback_t)nwaycc_callback,NULL, 100000);
			//////////////////////////////////////////////////////////////////
			//Inbound 启动
			pthread_join(pthid1, NULL);
			pthread_join(pthid2, NULL);
			pthread_join(pthid3, NULL);
			pthread_join(pthid4, NULL);
			pthread_join(pthid5, NULL);

		}
		else
			LOGERREX(__FILE__, __LINE__, "read config from database failed");


		pthread_mutex_destroy(&infoMutex);
		pthread_mutex_destroy(&taskMutex);
		pthread_mutex_destroy(&calloutMutex);
		pthread_mutex_destroy(&configMutex);
		pthread_mutex_destroy(&clickDialMutex);
	} while (false);
	return nway_res;
}
int main(void)
{
	bool bSuccess=false;
	start_log();
	if (init_nwaypbx()!= 0)
	{
		LOGEX(__FILE__, __LINE__, "init nway pbx failed");
		return 1;
	}
	return 0;
}


