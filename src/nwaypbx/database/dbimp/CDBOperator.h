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
#ifndef __NWAY_PBX_BASE_CONFIG_H
#define __NWAY_PBX_BASE_CONFIG_H
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <vector>
#include <limits.h>  
#include "../interface/db_center.h"
//#include "../interface/db_struct.h"
#include "../../nwaypbx/statedef.h"
#include "../../nwaypbx/structdef.h"
//////////////////////////////////////////////////////////////////////////
/*用于加载诸如路径配置、其它一些配置，对应于表call_base_config
在此应用中不处理更新，只管读取
*/
//////////////////////////////////////////////////////////////////////////
using namespace std;
int LoadBaseConfig(list<base_config>& lstBaseConfig);
//所有的分机组，它不是固定的
int LoadExtensionGroup(list<NwayExtensionGroup>& lstExtensionGroup);
//所有的分机，它是可以更新的，故声明为类
class CExtension
{
public:
	CExtension();
	~CExtension();
	//////////////////////////////////////////////////////////////////////////
	//extension_reg_state的字符值为 registered,unregistered
	//extension_login_state的字符值为 success, logout,busy,leaved
	int LoadExtension(list<NwayExtension>& lstExtension);
	//0=success,1 = logout,2=busy,3=leaved
	int UpdateLoginState(nway_uint64_t& id,int nLogin);
	int UpdateRegState(nway_uint64_t& id, const char* szState);
private:
};
//网关相关的配置
int LoadCalloutGateway(list<NwayCalloutGateway>& lstCalloutGateways);
//网关相关的配置
int LoadGateway(list<NwayGateway>& lstGateway);
//拨号计划明细
int LoadDialplanDetails(list<NwayDialplanDetail>& lstDialplanDetail);

//拨号计划
int LoadDialplan(list<NwayDialplan>& lstDialplan);

//IVR明细
int LoadIvrDetails(list<NwayIVRDetail>& lstIvrDeatail);

//IVR模块
int LoadIvrs(list<NwayIVR>& lstIvr);

//彩铃相关
int LoadRings(list<NwayRing>& lstRings);

//外线配置
int LoadOutsides(list<Outside_line>& lstOutsides);

//内外线对应配置
int LoadInOutMapping(list<In_Out_Mapping>& lstInOutMapping);

//CDR模块
class CCallDetailRecord
{
public:
	CCallDetailRecord();
	~CCallDetailRecord();
	//caller_name,caller_number是aleg，called_number是具体的呼入或呼出的大号码，id是生成的cdr id
	int StartCall(const char* caller_name, const char* caller_number, const char* called_number, nway_uint64_t& id);
	int StartCall(const char* caller_name, const char* caller_number, const char* called_number, nway_uint64_t& id,bool bAutoCallout, nway_uint64_t& task_id);

	//可以认为是对aleg摘机相关，不一定是应答
	int A_AnswerCall(nway_uint64_t& id);
	//按的键应答
	int B_AnswerCall(nway_uint64_t& id, const char* dest_number, const char* digites_dail);

	int A_EndCall(nway_uint64_t& id, NWAY_HANGUP_CAUSE cause, int gatewayid, NWAY_HANGUP_DIRECTION direction);
	
	//b-cause可以不用管,以后有必要扩展b挂机理由时再说
	int B_EndCall(nway_uint64_t& id, const char* dest_number, NWAY_HANGUP_CAUSE cause);


	int SetRecordFile(nway_uint64_t& id,const char* filename);
	
	//写入计时，在挂机后进行，策略有，统计a-leg,b-leg,a和b最长通话时长
	int CountTime(nway_uint64_t& id);

	int SetDtmf(nway_uint64_t& id, const char* dtmf);


private:
};

//写入事件信息
int InsertCallEvent(nway_uint64_t& cdr_id, nway_uint64_t& ext_id,string& a_leg_number,string& b_leg_number,string& route_number, NWAY_CALL_EVENT iEvent);
//将所有事件转成某个ext_id,场景为当有ivr等时，直接来电不是某个具体分机，则需要强行指定一次
int UpdateCallEvent(nway_uint64_t& cdr_id, nway_uint64_t& ext_id);




class CDBCalloutTask{
public:
	CDBCalloutTask();
	~CDBCalloutTask();
	int GetTasks(list<Callout_Task>& lstCalloutTasks, list<NwayGateway>& lstGateways,list<NwayCalloutGateway>& lstCalloutGateways);//如果没发现，则置为暂停，查不到号码了则置为完成
	int UpdateTaskStatus(nway_uint64_t& id, nway_uint64_t& run_postion, int total,int success, int failed, int cancled);
	int UpdateTaskState(nway_uint64_t& id, int nState);//更新状态，1,等待呼叫， 2正在外呼，3已完成，4暂停
	int GetIdleAgentNumber(nway_uint64_t& groupid,int& nNumber );
	int GetCalloutNumbers(Callout_Task& nwayct, int nMaxnumber);

};

class CDBCalloutInfo{
public:
	CDBCalloutInfo();
	~CDBCalloutInfo();
	int StartCallout(nway_uint64_t& id,nway_uint64_t& cdrid);
	int ExtensionAnswer(nway_uint64_t& id,nway_uint64_t& extension_id,const char* record_file);
	int SetAlegAnswer(nway_uint64_t& id);
	int SetAlegHangup(nway_uint64_t& id, int nHangupid);
	int SetCalled(nway_uint64_t&id);
};

int DBGetClickDials(vector<Click_Dial>& vecClickDials);

int DBSetClickDialed(nway_uint64_t& id);

int DBGetConfigChanged(vector<int>& vecConfigs);

int DBSetConfigChanged(int orderid);
#endif
