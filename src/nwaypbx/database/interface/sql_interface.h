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
#ifndef NWAY_SQL_INTERFACE_H
#define NWAY_SQL_INTERFACE_H
  //dword
//#include "sqlapi/SQLAPI.h"
//#include <SQLAPI.h>
#include "libpq-fe.h"
#ifdef WIN32
#include <Windows.h>
#ifdef UNICODE
//#pragma comment(lib,"sqlapiu.lib") //Ìí¼Ólib
#else
//#pragma comment(lib,"sqlapi.lib") //Ìí¼Ólib
//#pragma comment(lib,"libpcre.lib")
//#pragma comment(lib,"esld.lib")
#endif

#else
#ifndef DWORD
typedef unsigned long DWORD;
#endif //DWORD
#endif //WIN32

 
 
#include <string>
using namespace std;
class sql_interface
{
public:
	sql_interface();
	virtual ~sql_interface();
    DWORD Init(const char* host, const char* port, const char* dbname,const char* user_name,const char* pswd,unsigned short nDB);//nDB,0,mysql;1,sql server;2,oracle;3.sqlite;4,postgresql
 
	DWORD Connect();
	PGconn* getConn();
	//DWORD ExecuteWithResult(char* s_sql,SACommand & oCmd);
protected:
private:
	PGconn* _conn;
	//CRITICAL_SECTION m_criticalSection;
	//HANDLE hMutex;
	//SACommand oCmd;
	string _dbname;
	string _username;
	string _pswd;
	string _host;
	string _port;
	unsigned short _nDB;
	bool bOpen;
	//HANDLE _handle;
	//boost::mutex _mutex; 
};


#endif

