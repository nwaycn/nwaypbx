#include "nwaycti.h"
#include "inbound_global.h"
#include "../database/config/config.h"
#include "../database/interface/db_center.h"
#include <string>
esl_handle_t ghandle = {{0}};
using namespace std;
bool InitDatabase()
{
	bool bRes = false;
	HConf *info = (HConf *) malloc(sizeof(HConf));  
	init_conf(info,"./database.conf"); 
	char buf[50];  
	get_conf_value(info,"servername",buf);  ///如:192.168.1.209@mccrm
	string servername = buf;
	get_conf_value(info,"username",buf);
	string username = buf;
	get_conf_value(info,"password",buf);
	string password =buf;
	db_center* dbInstance = db_center::get_instance();
	DWORD dwStatus =  dbInstance->Init(servername.c_str(),username.c_str(),password.c_str(),4);
	if (dwStatus == 0)
	{
		//printf("server:%s,user:%s,pass:%s,initlize database successed\n",servername.c_str(),username.c_str(),password.c_str());
		bRes = true;
	}
	else printf("server:%s,user:%s, initlize database failed\n",servername.c_str(),username.c_str() );
	return bRes;
}

bool LoadConfigFromDb( list<base_config>& lstBaseConfig, list<NwayExtensionGroup>& lstExtensionGroup, \
    list<NwayExtension>& lstExtension,  list<NwayGateway>& lstGateway, list<NwayDialplanDetail>& lstDialplanDetail, \
    list<NwayDialplan>& lstDialplan,  list<NwayIVRDetail>& lstIvrDeatail, list<NwayIVR>& lstIvr, \
	list<NwayRing>& lstRings, list<Outside_line>& lstOutsides, list<In_Out_Mapping>& lstInOutMapping , list<NwayCalloutGateway>& lstCalloutGateways)
{
	int nResult = 0;
	//清空之前的配置数据
	lstBaseConfig.clear();
	lstExtensionGroup.clear();
	lstGateway.clear();
	lstDialplan.clear();
	lstDialplanDetail.clear();
	lstExtension.clear();
	lstIvr.clear();
	lstIvrDeatail.clear();
	lstRings.clear();
	lstCalloutGateways.clear();
	lstOutsides.clear();

	nResult = LoadBaseConfig(lstBaseConfig);
	nResult = LoadExtensionGroup(lstExtensionGroup);
	nResult = LoadGateway(lstGateway);
	nResult = LoadDialplan(lstDialplan);
	nResult = LoadDialplanDetails(lstDialplanDetail);
	nResult = LoadIvrDetails(lstIvrDeatail);
	nResult = LoadIvrs(lstIvr);
	nResult = LoadRings(lstRings);
	nResult = LoadOutsides(lstOutsides);
	nResult = LoadCalloutGateway(lstCalloutGateways);
	CExtension ext;
	ext.LoadExtension(lstExtension);
	return true;
}

