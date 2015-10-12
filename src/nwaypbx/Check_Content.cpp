#include "Check_Content.h"

int CheckIsInside( string dest_number, NwayExtension& ext, list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex)
{
	int nStatus = -1;
	list<NwayExtension>::iterator it = lstExtension.begin();
	pthread_mutex_lock(&configMutex);
	for (;it != lstExtension.end(); it++)
	{
		NwayExtension& ext2 = *it;
		if (ext2.extension_number == dest_number)
		{
			nStatus = 0;
			ext =ext2;
			break;

		}
		ext.clear();
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int CheckIdleAgent( const nway_uint64_t& agent_group_id,list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex)
{
	int nCount = 0;
	list<NwayExtension>::iterator it = lstExtension.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtension.end(); it++)
	{
		NwayExtension& ext = *it;
		if (ext.group_id == agent_group_id && ext.call_state == CALLIN_STANDBY && ext.login_state == AGENT_LOGIN_SUCCESS && ext.reg_state == AGENT_REG_SUCCESS)
		{
			nCount++;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nCount;
}

int GetIdleAgent( const nway_uint64_t& agent_group_id, string& ext_number,list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex)
{
	int nStatus = -1;
	list<NwayExtension>::iterator it = lstExtension.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtension.end(); it++)
	{
		NwayExtension& ext = *it;
		char szTmp[2048] = {0};
		sprintf(szTmp,"%s,%d\tGetIdleAgent: number:%s, group id:%lld,ext.callout_state:%d, ext.call_state:%d, ext.login_state:%d, ext.reg_state:%d\n",__FILE__,__LINE__,ext.extension_number.c_str(),ext.group_id,ext.callout_state,ext.call_state,ext.login_state,ext.reg_state);
		LOG(szTmp);
		if (ext.group_id == agent_group_id && ext.callout_state == CALLOUT_INIT && ext.call_state == CALLIN_STANDBY && ext.login_state == AGENT_LOGIN_SUCCESS && ext.reg_state == AGENT_REG_SUCCESS)
		{
			ext_number = ext.extension_number;
			//ext_number +="@nway.vicp.net";
			ext.callout_state = CALLOUT_OUT;
			nStatus = 0;
			//*it = ext;
			break;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetBindPhones( string& caller_number,string& dest_number, list<NwayExtension>& lstBinds, list<NwayExtension>& lstExtensions )
{
	//如何取绑定号码，暂时未知
	return 0;
}

int GetCalloutNumbers( const nway_uint64_t& agent_group_id, list<Callout_info>& lstCalloutinfo , pthread_mutex_t& configMutex )
{
	return 0;
}

int GetGateway( const string& ext_number, string& gateway, list<NwayGateway>& lstGateway, list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	nway_uint64_t gatewayid=0;
	list<NwayExtension>::iterator itext = lstExtension.begin();
	pthread_mutex_lock(&configMutex);
	for (; itext != lstExtension.end(); itext++)
	{
		NwayExtension& ng = *itext;
		if (ng.extension_number == ext_number)
		{
			gatewayid =ng.callout_gateway;
			break;
		}
	}
	if (gatewayid > 0 )
	{
		list<NwayGateway>::iterator it = lstGateway.begin();
		for (; it != lstGateway.end();it++)
		{
			NwayGateway& ng2 = *it;
			if (ng2.id == gatewayid)
			{
				gateway = ng2.gateway_url;
				break;
			}
		}
	}
	else
		nStatus = -1;
	pthread_mutex_unlock(&configMutex);
	return nStatus;
	

}

int GetRing( nway_uint64_t id , string& fullname, list<NwayRing>& lstRings , const string& basepath, pthread_mutex_t& configMutex)
{
	int nStatus = -1;
	list<NwayRing>::iterator it = lstRings.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstRings.end(); it++)
	{
		NwayRing& nr = *it;
		if (nr.id == id)
		{
			nStatus = 0;
			if (basepath.at(basepath.length()-1) != '/')
			{
				fullname = basepath +"/"+ nr.filename;
			}
			else
				fullname = basepath + nr.filename;
			break;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int SetIdleAgent( const nway_uint64_t& agent_group_id, string& ext_number,list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	list<NwayExtension>::iterator it = lstExtension.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtension.end(); it++)
	{
		NwayExtension& ext = *it;
		if (ext.group_id == agent_group_id && ext.extension_number == ext_number)
		{
			//ext_number = ext.extension_number;
			ext.callout_state = CALLOUT_INIT;
			ext.login_state = AGENT_LOGIN_SUCCESS;
			//ext.reg_state == AGENT_REG_SUCCESS;
			CExtension dbext;
			dbext.UpdateLoginState(ext.id, 0);
			//*it = ext;
			break;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetBasePath( string& pth, list<base_config>& lstbc )
{
	int nStatus=0;
	list<base_config>::iterator it = lstbc.begin();
	for(; it != lstbc.end(); it ++)
	{
		base_config& bc = *it;
		if (bc.config_name == "base_path")
		{
			pth = bc.config_param;
			nStatus = 1;
		}
	}
	return nStatus;
}

int GetOutlineLen(list<base_config>& lstbc, pthread_mutex_t& configMutex)
{
	int nStatus=8;
	char sztmp[20];
	list<base_config>::iterator it = lstbc.begin();
	pthread_mutex_lock(&configMutex);
	for(; it != lstbc.end(); it ++)
	{
		base_config& bc = *it;
		if (bc.config_name == "outline_len")
		{
			//////////////////////////////////////////////////////////////////////////
			//指定号码的长度，
			strncpy(sztmp,bc.config_param.c_str(),20);
			nStatus = atoi(sztmp);
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetInline( const string& OutsideNumber, string& InsideNumber, list<Outside_line>& lstOutsideline , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	list<Outside_line>::iterator it = lstOutsideline.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstOutsideline.end(); it++ )

	{
		Outside_line& ol = *it;
		printf("%s,%d      --outside line:%s \n",__FILE__, __LINE__,OutsideNumber.c_str());
		if (OutsideNumber == ol.outside_line_number)
		{
			InsideNumber = ol.inside_line_number;
			nStatus = 1;
			break;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetExtension( const string& strExtension,NwayExtension& ext, list<NwayExtension>& lstExtensions , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	list<NwayExtension>::iterator it = lstExtensions.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtensions.end(); it++)
	{
		NwayExtension& ext2 = *it;
		if (ext2.extension_number == strExtension)
		{
			nStatus =1;
			ext = ext2;
			break;
		}
		
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetIVRDetails( nway_uint64_t id, list<NwayIVRDetail>& lstIvrDetails, list<NwayIVRDetail>& myDetails, pthread_mutex_t& configMutex )
{
	int nStatus;
	list<NwayIVRDetail>::iterator it = lstIvrDetails.begin();
	pthread_mutex_lock(&configMutex);
	for (; it!= lstIvrDetails.end(); it++)
	{
		NwayIVRDetail& myde = *it;
		if (myde.ivr_menu_id == id)
			myDetails.push_back(myde);
	}
	myDetails.sort();
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int SetAgentLoginState( nway_uint64_t id,int LoginState )
{
	CExtension nwayext;
	 
	int i = nwayext.UpdateLoginState(id,LoginState);
	 
	return i;
}

int SetAgentLoginStateBusyOrIdle( const char* strExtension,list<NwayExtension>& lstExtensions, int LoginState , pthread_mutex_t& configMutex)
{
	CExtension nwayext;
	list<NwayExtension>::iterator it = lstExtensions.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtensions.end(); it++)
	{
		NwayExtension& ne = *it;
		if (ne.extension_number == strExtension)
		{
			//表示找到了相关的分机
			nwayext.UpdateLoginState(ne.id, LoginState);
			break;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return 0;
}

int GetCalloutTask( list<Callout_Task>& lstCalloutTasks, list<NwayExtension>& lstExtensions, list<NwayGateway>& lstGateways,list<NwayCalloutGateway>& lstCalloutGateways , pthread_mutex_t& configMutex )
{
	int nStatus = 0;
	CDBCalloutTask dbct;
	nStatus = dbct.GetTasks(lstCalloutTasks,lstGateways,lstCalloutGateways);//获取到了所有的外呼任务
	list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
	for (; itTask != lstCalloutTasks.end();itTask ++)
	{
		Callout_Task& nwayct =*itTask;
		//更新当前的任务信息：当前的号码id,接通数，失败数，如果是取消，则是取消数
		//printf("%s,%d the call out task:%s \n\n",__FILE__,__LINE__,nwayct.callout_name);
		//pthread_mutex_lock(configMutex);
		if (nwayct.callout_state_id == 4)
		{
			//如果是取消
			int nCancle= nwayct.total_number - nwayct.success_number - nwayct.failed_number;
			dbct.UpdateTaskStatus(nwayct.id,nwayct.run_position,nwayct.total_number,nwayct.success_number,nwayct.failed_number,nCancle);

		}
		if (nwayct.callout_state_id == 1)
		{
			dbct.UpdateTaskState(nwayct.id,2);
			dbct.UpdateTaskStatus(nwayct.id,nwayct.run_position,nwayct.total_number,nwayct.success_number,nwayct.failed_number,0);

		}
		if (nwayct.callout_state_id == 2)
		{
			dbct.UpdateTaskStatus(nwayct.id,nwayct.run_position,nwayct.total_number,nwayct.success_number,nwayct.failed_number,0);

		}
		//获取外呼号码
		//first , get idle agent
		int nIdleAgent = 0; //空闲座席
		int nMaxGwCall = 0;//每个网关的最大呼叫数量
		string gwurl = "";//网关的url
		string call_prefix;
		list<NwayCalloutGateway>::iterator coGwit = lstCalloutGateways.begin();
		pthread_mutex_lock(&configMutex);
		for(; coGwit != lstCalloutGateways.end(); coGwit++)
		{
			NwayCalloutGateway& nwCalloutGw = *coGwit;
			if (nwCalloutGw.id == nwayct.callout_gateway_id)
			{
				list<NwayGateway>::iterator gwit = lstGateways.begin();
				for (; gwit != lstGateways.end(); gwit++)
				{
					NwayGateway& nwaygw = *gwit;
					if (nwaygw.id == nwCalloutGw.gateway_id)
					{
						nMaxGwCall = nwaygw.max_call;
						gwurl = nwaygw.gateway_url;
						call_prefix = nwaygw.call_prefix;
						nwayct.gateway_id = nwaygw.id;
						nwayct.call_prefix = nwaygw.call_prefix;
						nwayct.gateway_url = nwaygw.gateway_url;
						//////////////////////////////////////////////////////////////////////////
						//*itTask = nwayct;
						break;
					}
				}
				break;
			}
			
		}
		
		dbct.GetIdleAgentNumber(nwayct.group_id, nIdleAgent);
		//if (CONCURR_FORECAST_CALL == nwayct.concurr_type_id)
		//{
		//	//预测外呼
		//}
		//printf("%s,%d  has idle agent :%d \n",__FILE__, __LINE__, nIdleAgent);
		if (nIdleAgent >0 )
		{
			//有空闲座席或外呼通道才会进入此处理
			switch(nwayct.concurr_type_id)
			{
			case CONCURR_FORECAST_CALL:
				{
					//预测外呼
					int nGetIdleNumber = 0;
					nGetIdleNumber =int( nIdleAgent * nwayct.concurr_number) ;
					if (nGetIdleNumber > nMaxGwCall - nwayct.lstCalloutInfo.size())
					{
						nGetIdleNumber =  nMaxGwCall - nwayct.lstCalloutInfo.size();
					}
					if (nGetIdleNumber > nwayct.max_concurr_number - nwayct.lstCalloutInfo.size())
					{
						nGetIdleNumber = nwayct.max_concurr_number - nwayct.lstCalloutInfo.size();
					}
					//printf("%s, %d\tTo get callout phone number:%d\n", __FILE__, __LINE__,nGetIdleNumber);
					dbct.GetCalloutNumbers(nwayct,nGetIdleNumber);
				}
				break;
			case CONCURR_PRECISION_CALL:
				{
					//精准外呼
					int nGetIdleNumber = 0;
					nGetIdleNumber = nIdleAgent;
					if (nGetIdleNumber > nMaxGwCall - nwayct.lstCalloutInfo.size())
					{
						nGetIdleNumber =  nMaxGwCall - nwayct.lstCalloutInfo.size();
					}
					if (nGetIdleNumber > nwayct.max_concurr_number - nwayct.lstCalloutInfo.size())
					{
						nGetIdleNumber = nwayct.max_concurr_number - nwayct.lstCalloutInfo.size();
					}
					//printf("%s, %d\tTo get callout phone number:%d\n", __FILE__, __LINE__,nGetIdleNumber);
					dbct.GetCalloutNumbers(nwayct,nGetIdleNumber);
				}
				break;
			case CONCURR_RING_CALL:
				{
					//语音外呼
					int nGetIdleNumber = 0;
					nGetIdleNumber = nwayct.max_concurr_number - nwayct.lstCalloutInfo.size(); //应是减去当前正在呼叫的量
					//printf("%s, %d\tTo get callout phone number:%d\n", __FILE__, __LINE__,nGetIdleNumber);
					dbct.GetCalloutNumbers(nwayct,nGetIdleNumber);
				}
				break;
			default:
				;
			}
			//将当前的run_position更新回队列中及数据库中
			//printf("%s,%d   this task run postion:%lld \n",__FILE__,__LINE__,nwayct.run_position);
			//*itTask = nwayct;
			
			if (nwayct.lstCalloutInfo.size() == 0)
			{
				//没有数据时则从当前列表中移除
				list<Callout_Task>::iterator itTask2 = itTask;
				printf("%s, %d\tremove a task id:%lld\n",__FILE__, __LINE__, nwayct.id);
				itTask++;
				lstCalloutTasks.erase(itTask2);
			
			}

		}
		pthread_mutex_unlock(&configMutex);
		
	}
	return nStatus;
}

int SetCallNumberCalled( nway_uint64_t& id )
{
	CDBCalloutInfo dbci;
	return dbci.SetCalled(id);
}

int SetBusyAgent( const nway_uint64_t& agent_group_id, string& ext_number,list<NwayExtension>& lstExtension , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	list<NwayExtension>::iterator it = lstExtension.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtension.end(); it++)
	{
		NwayExtension& ext = *it;
		if (ext.group_id == agent_group_id && ext.login_state == AGENT_LOGIN_SUCCESS && ext.reg_state == AGENT_REG_SUCCESS && ext.extension_number == ext_number) 
		{
			ext_number = ext.extension_number;
			ext.callout_state = CALLOUT_OUT;
			CExtension dbext;
			dbext.UpdateLoginState(ext.id, 2);
			//*it = ext;
		}
	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetRecordFileName( string &strExtension,string &strFile,string &strFullName,  string& basePath )
{
	int dwStatus = 0;
	char szFilename[MAX_PATH];
 

	struct timeb tp; 
	struct tm * t;
	ftime(&tp);
	t= localtime(&(tp.time));
	char szTemp[MAX_PATH]={0};

	memset(szTemp,0,MAX_PATH);


	sprintf(szFilename,"%04d%02d%02d%02d%02d%02d%03d.mp3", t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,tp.millitm);
	strFullName = basePath ;
	strFullName += "/";
	 
 
	//添加年份月份路径
	strFile ="records/";
	sprintf(szTemp,"%04d/",t->tm_year+1900);
	strFile += szTemp;
	 

	sprintf(szTemp,"%02d/",t->tm_mon+1);
	strFile += szTemp;
	 

	strFile += strExtension;
	strFile += "/";
	 
	strFile += szFilename;
	//以上为相对路径名
	//绝对路径名
	strFullName += strFile;
	 
	return dwStatus;
}

int GetExtensionId( const string& strExtension,nway_uint64_t& id,list<NwayExtension>& lstExtensions , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	list<NwayExtension>::iterator it = lstExtensions.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtensions.end(); it++)
	{
		NwayExtension& ext2 = *it;
		if (ext2.extension_number == strExtension)
		{
			nStatus =1;
			id = ext2.id;
			break;
		}

	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int GetClickDials( vector<Click_Dial>& vecClickDials )
{
	return DBGetClickDials(vecClickDials);
}

int SetClickDialed( nway_uint64_t& id )
{
	return DBSetClickDialed(id);
}

int SetAgentRegState( const char* strExtension, list<NwayExtension>& lstExtensions, NWAY_AGENT_REG_STATE RegState , pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	list<NwayExtension>::iterator it = lstExtensions.begin();
	pthread_mutex_lock(&configMutex);
	for (; it != lstExtensions.end(); it++)
	{
		NwayExtension& ext2 = *it;
		if (ext2.extension_number == strExtension)
		{
			nStatus =1;
			CExtension dbext;
			if (RegState == AGENT_REG_SUCCESS)
			{
				ext2.reg_state = AGENT_REG_SUCCESS;
				dbext.UpdateRegState(ext2.id,"REGED");
			}
			else
			{
				ext2.reg_state = AGENT_REG_FAILED;
				dbext.UpdateRegState(ext2.id,"NOREGED");
			}
			
			break;
		}

	}
	pthread_mutex_unlock(&configMutex);
	return nStatus;
}

int SetDtmf( nway_uint64_t& id, const char* dtmf )
{
	CCallDetailRecord cdr;
	return cdr.SetDtmf(id,dtmf);
}

int GetConfigChanged( vector<int>& vecConfigs )
{
	return DBGetConfigChanged(vecConfigs);
}

int SetConfigChanged( int orderid )
{
	return DBSetConfigChanged(orderid);
}

int ReLoadExtensions( list<NwayExtension>& lstExtensions  )
{
	int nResult = 0;
	//查当前的分机是不是被禁用，如果没有新的分机，则需要添加，禁用的分机找到后删掉
	list<NwayExtension> myExtensions;
	CExtension ext;
	ext.LoadExtension(myExtensions);
	list<NwayExtension>::iterator itmyExt = myExtensions.begin();
	for (; itmyExt != myExtensions.end(); itmyExt++ )
	{
		NwayExtension& myExt = * itmyExt;
		list<NwayExtension>::iterator itExt = lstExtensions.begin();
		bool bFound = false;
		for (; itExt != lstExtensions.end(); itExt++)
		{
			NwayExtension& ext = *itExt;
			if (ext.id = myExt.id)
			{
				ext.callout_gateway = myExt.callout_gateway;
				ext.callout_number = myExt.callout_number;
				ext.extension_name = myExt.extension_name;
				ext.extension_number = myExt.extension_number;
				ext.extension_type = myExt.extension_type;
				ext.group_id = myExt.group_id;
				ext.is_allow_callout = myExt.is_allow_callout;
				ext.is_disable = myExt.is_disable;
				ext.is_record = myExt.is_record;
				//ext.
				bFound = true;
			}
			if (!bFound)
			{
				//如果没找到，需要添加这个分机进了列
				lstExtensions.push_back(myExt);
			}
		}
	}
	return nResult;
}

