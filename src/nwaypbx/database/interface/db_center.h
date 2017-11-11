#ifndef IBMS_DB_CENTER_H
#define IBMS_DB_CENTER_H
#include "../../common/Mutex.h"
#include "sql_interface.h"
class db_center
{
public:
	static db_center* get_instance();//a instance of this class
	void release();
	virtual ~db_center();
	DWORD Init(const char* dbname,const char* user_name,const char* pswd,unsigned short nDB);//nDB,0,mysql; 1,sql server; 2,oracle;3.sqlite
	sql_interface* get_interface();
	DWORD Close();
	SAConnection* GetConn();
	void Lock();
	void Unlock();
protected:
private:
	db_center();
	Mutex lock;
	sql_interface* psql_itfs;
};

#endif
