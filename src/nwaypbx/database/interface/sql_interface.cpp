#include "stdafx.h"
#include <stdio.h>
#include "sql_interface.h"
//#include "../../common/nway/errerCode.h"
#include "errerCode.h"
sql_interface::sql_interface():bOpen(false)
{
	//InitializeCriticalSection(&m_criticalSection);
	//hMutex = CreateMutex(NULL,FALSE,MUTEX_NAME);
}

sql_interface::~sql_interface()
{
	//DeleteCriticalSection(&m_criticalSection);
     oCon.Disconnect();
}

DWORD sql_interface::Init(const char* dbname,const char* user_name,const char* pswd,unsigned short nDB )
{
	this->_dbname = dbname;
	this->_username = user_name;
	this->_pswd = pswd;
	this->_nDB = nDB;
	//_handle = CreateMutex(NULL,TRUE,NULL);

	return Connect();
     
}
 

SAConnection* sql_interface::getConn()
{
	//WaitForSingleObject(_handle,300);
	while (false)
	{
		SACommand cmd;
		int id=0;
		cmd.setConnection(&oCon);
		cmd.setCommandText("select id from public.test;");
		//cmd.Param("v_date").setAsString() = "2011-6-1";
		cmd.Execute();
 
		id = cmd.Field("id").asShort();
		cmd.Close();
		try
		{
			if (id <= 1)
			{
				oCon.Disconnect();
				Connect();
			}
			else
			{
				break;
			}
		}
		catch (...)
		{
			printf("%s, %d\tget connect failed\n",__FILE__,__LINE__);
			break;
		}
		
		
	}
	
	//ReleaseMutex(_handle);
	if (bOpen == false)
	{
		return NULL;
	}
	return &oCon;
}

DWORD sql_interface::Connect()
{
	DWORD uResult= SUCCESS;

	try
	{
		switch (_nDB)
		{
		case 0:
			oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_MySQL_Client);
			break;
		case 1:
			oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_SQLServer_Client);
			break;

		case 2:
			oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_Oracle_Client);
			break;
			//case 3:
			//	oCon.Connect(dbname,user_name,pswd,SA_SQLite_Client);
			//	break;
		case 4:
			{
				oCon.Connect(_dbname.c_str(),_username.c_str(),_pswd.c_str(),SA_PostgreSQL_Client);
				SACommand cmd;
				cmd.setConnection(&oCon);
				cmd.setCommandText("select id from public.test");
				cmd.Execute();
				/*while(cmd.FetchNext()){
					printf("id:%d\n", cmd.Field("id").asShort());
				}*/
				printf("%s, %d\tconnected to postgresql\n",__FILE__,__LINE__);
			}
			
			break;
		default:
			break;
		}
	}
	catch (SAException &x)
	{
		uResult = x.ErrNativeCode();
		string s;
		s = x.ErrText().GetBuffer(x.ErrText().GetLength());
		printf("%s, %d\tconnect to db error:%s\n",__FILE__,__LINE__,s.c_str());
	}
	if (uResult == 0)
	{
		bOpen = true;
	}
	return uResult;

}

//DWORD sql_interface::ExecuteWithResult( char* s_sql,SACommand & oCmd )
//{
//	DWORD uResult = DB_SUCCESS;
//	return uResult;
//}
