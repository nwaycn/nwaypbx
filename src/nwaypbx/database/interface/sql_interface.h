#ifndef IBMS_SQL_INTERFACE_H
#define IBMS_SQL_INTERFACE_H
  //dword
//#include "sqlapi/SQLAPI.h"
#include <SQLAPI.h>
#ifdef WIN32
#include <Windows.h>
#ifdef UNICODE
#pragma comment(lib,"sqlapiu.lib") //Ìí¼Ólib
#else
#pragma comment(lib,"sqlapi.lib") //Ìí¼Ólib
#pragma comment(lib,"libpcre.lib")
#pragma comment(lib,"esld.lib")
#endif
#else
#ifndef DWORD
typedef unsigned long DWORD;
#endif //DWORD
#endif //WIN32


//#define MUTEX_NAME "MCSD"
//#include "../../SGLibs/SHLib/Inc/shpa3api.h"
#include <string>
using namespace std;
class sql_interface
{
public:
	sql_interface();
	virtual ~sql_interface();
    DWORD Init(const char* dbname,const char* user_name,const char* pswd,unsigned short nDB);//nDB,0,mysql;1,sql server;2,oracle;3.sqlite;4,postgresql
 
	DWORD Connect();
	SAConnection* getConn();
	//DWORD ExecuteWithResult(char* s_sql,SACommand & oCmd);
protected:
private:
	SAConnection oCon;
	//CRITICAL_SECTION m_criticalSection;
	//HANDLE hMutex;
	//SACommand oCmd;
	string _dbname;
	string _username;
	string _pswd;
	unsigned short _nDB;
	bool bOpen;
	//HANDLE _handle;
	//boost::mutex _mutex; 
};


#endif

