#ifndef __NWAY_CHECK_CONTENT_H
#define __NWAY_CHECK_CONTENT_H
#include "nwaycti/nwaycti.h"
#include "log/log.h"
#include <list>
#include <string>
#include <vector>
#include <pthread.h>
using namespace std;
int CheckIsInside(string dest_number, NwayExtension& ext, list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex);//查找是否是内线号码
int CheckIdleAgent(const nway_uint64_t& agent_group_id,list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex);//查找空闲的座席数量，空闲和挂机后转为空闲

int GetIdleAgent(const nway_uint64_t& agent_group_id, string& ext_number,list<NwayExtension>& lstExtension, pthread_mutex_t& configMutex); //获取一个空闲的号码

int SetIdleAgent(const nway_uint64_t& agent_group_id, string& ext_number,list<NwayExtension>& lstExtension, pthread_mutex_t& configMutex); //设置一个座席闲

int SetBusyAgent(const nway_uint64_t& agent_group_id, string& ext_number,list<NwayExtension>& lstExtension, pthread_mutex_t& configMutex);//设置一个座席忙

int GetBindPhones(string& caller_number,string& dest_number, list<NwayExtension>& lstBinds, list<NwayExtension>& lstExtensions);//通过来电和目标号码决定呼转到哪些号码，did是直对一个号码
int GetCalloutNumbers(const nway_uint64_t& agent_group_id, list<Callout_info>& lstCalloutinfo , pthread_mutex_t& configMutex );//获得针对某个座席组的外呼号码

int GetGateway(const string& ext_number, string& gateway, list<NwayGateway>& lstGateway, list<NwayExtension>& lstExtension, pthread_mutex_t& configMutex);//在内线外呼时获取对应的网关

int GetRing(nway_uint64_t id , string& fullname, list<NwayRing>& lstRings, const string& basepath, pthread_mutex_t& configMutex);  //通过id返回一个语音文件的全路径

int GetCalloutTask(list<Callout_Task>& lstCalloutTasks, list<NwayExtension>& lstExtensions, list<NwayGateway>& lstGateways,list<NwayCalloutGateway>& lstCalloutGateways, pthread_mutex_t& configMutex); //定时更新外呼任务

int GetBasePath(string& pth, list<base_config>& lstbc);

int GetOutlineLen(list<base_config>& lstbc, pthread_mutex_t& configMutex);

int SetDtmf(nway_uint64_t& id, const char* dtmf);

//按被叫获取一个内线号码
int GetInline(const string& OutsideNumber, string& InsideNumber, list<Outside_line>& lstOutsideline, pthread_mutex_t& configMutex);

int GetExtension(const string& strExtension,NwayExtension& ext, list<NwayExtension>& lstExtensions, pthread_mutex_t& configMutex);
int GetExtensionId(const string& strExtension,nway_uint64_t& id,list<NwayExtension>& lstExtensions, pthread_mutex_t& configMutex);

int GetIVRDetails(nway_uint64_t id, list<NwayIVRDetail>& lstIvrDetails, list<NwayIVRDetail>& myDetails, pthread_mutex_t& configMutex);

int SetAgentLoginState(nway_uint64_t id,int LoginState );//0,success, 1,logout, 2,busy, 3,leaved
int SetAgentRegState(const char* strExtension, list<NwayExtension>& lstExtensions, NWAY_AGENT_REG_STATE RegState, pthread_mutex_t& configMutex);// 0:REGED, 1:NOREGED
int SetAgentLoginStateBusyOrIdle(const char* strExtension,list<NwayExtension>& lstExtensions, int LoginState, pthread_mutex_t& configMutex);//0,success, 1,logout, 2,busy, 3,leaved);
int SetCallNumberCalled(nway_uint64_t& id );
//////////////////////////////////////////////////////////////////////////
//strFile是去除basePath的路径，如records/xxx/xxx/xxx-xxxx-xxx-xxx.wav
//strFullName是全文件路径/home/voices/records/xxx/xxx/xxx-xxx-xxx-xxx.wav
int GetRecordFileName( string &strExtension, string &strFile, string &strFullName,  string& basePath ); 
//获取点击回呼的列表
int GetClickDials(vector<Click_Dial>& vecClickDials);

int SetClickDialed(nway_uint64_t& id);

int GetConfigChanged(vector<int>& vecConfigs);
int SetConfigChanged(int orderid);

int ReLoadExtensions(list<NwayExtension>& lstExtensions );

#endif