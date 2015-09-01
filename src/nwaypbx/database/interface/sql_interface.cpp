/*************************************************************************
based for NwayPBX
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License£º GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include "sql_interface.h"
//#include "../../common/nway/errerCode.h"
#include "errerCode.h"
#include "../../common/log/log.h"
sql_interface::sql_interface():bOpen(false)
{
	//InitializeCriticalSection(&m_criticalSection);
	//hMutex = CreateMutex(NULL,FALSE,MUTEX_NAME);
}

sql_interface::~sql_interface()
{
	//DeleteCriticalSection(&m_criticalSection);
	PQfinish(_conn);
}

DWORD sql_interface::Init(const char* host, const char* port, const char* dbname,const char* user_name,const char* pswd,unsigned short nDB )
{
	this->_dbname = dbname;
	this->_username = user_name;
	this->_pswd = pswd;
	this->_nDB = nDB;
	this->_host = host;
	this->_port = port;
	//_handle = CreateMutex(NULL,TRUE,NULL);

	return Connect();
     
}
 

PGconn* sql_interface::getConn()
{
	//WaitForSingleObject(_handle,300);
	char outputinfo[256] = { 0 };
	while (false)
	{
		//SACommand cmd;
		PGresult* res = NULL;
		int status=0;
		//cmd.setConnection(&oCon);
		//cmd.setCommandText("select now();");
		res = PQexec(_conn, "select now();");
		//cmd.Param("v_date").setAsString() = "2011-6-1";
		status = PQresultStatus(res);
		try
		{
			if (status != PGRES_TUPLES_OK)
			{
				PQclear(res);
				PQfinish(_conn);
				Connect();
			}
			else
			{
				PQclear(res);
				break;
			}
		}
		catch (...)
		{
			sprintf(outputinfo, "%s, %d\tget connect failed\n",__FILE__,__LINE__);
			LOGDBERR(__FILE__, __LINE__, outputinfo);
			break;
		}
		
		
	}
	
	//ReleaseMutex(_handle);
	if (bOpen == false)
	{
		return NULL;
	}
	return _conn;
}

DWORD sql_interface::Connect()
{
	DWORD dwResult= SUCCESS;

	//try
	//{
	//	switch (_nDB)
	//	{
	//	case 0:
	//		oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_MySQL_Client);
	//		break;
	//	case 1:
	//		oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_SQLServer_Client);
	//		break;

	//	case 2:
	//		oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_Oracle_Client);
	//		break;
	//		//case 3:
	//		//	oCon.Connect(dbname,user_name,pswd,SA_SQLite_Client);
	//		//	break;
	//	case 4:
	//		{
	//			oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_PostgreSQL_Client);
	//			SACommand cmd;
	//			cmd.setConnection(&oCon);
	//			cmd.setCommandText("select id from public.test");
	//			cmd.Execute();
	//			/*while(cmd.FetchNext()){
	//				printf("id:%d\n", cmd.Field("id").asShort());
	//			}*/
	//			printf("%s, %d\tconnected to postgresql\n",__FILE__,__LINE__);
	//		}
	//		
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//catch (SAException &x)
	//{
	//	uResult = x.ErrNativeCode();
	//	string s;
	//	s = x.ErrText().GetBuffer(x.ErrText().GetLength());
	//	printf("%s, %d\tconnect to db error:%s\n",__FILE__,__LINE__,s.c_str());
	//}
	char conninfo[336] = { 0 };
	sprintf(conninfo, "host=%s dbname=%s user=%s password=%s port=%s connect_timeout=30", _host.c_str(), _dbname.c_str(), _username.c_str(),
		_pswd.c_str(), _port.c_str());
	_conn = PQconnectdb(conninfo);
	dwResult = PQstatus(_conn);
	if (PQstatus(_conn) != CONNECTION_OK)
	{
		sprintf(conninfo, "connected to postgresql is failed, result value=%d\n", PQstatus(_conn));
		LOGDBERR(__FILE__, __LINE__, conninfo);
	}
	if (dwResult == CONNECTION_OK)
	{
		bOpen = true;
	}
	return dwResult;

}

//DWORD sql_interface::ExecuteWithResult( char* s_sql,SACommand & oCmd )
//{
//	DWORD uResult = DB_SUCCESS;
//	return uResult;
//}
