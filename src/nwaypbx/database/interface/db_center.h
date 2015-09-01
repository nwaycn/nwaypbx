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
#ifndef IBMS_DB_CENTER_H
#define IBMS_DB_CENTER_H
#include "../../common/nway-lib/Mutex.h"
#include "../../common/nway-lib/NwayDef.h"
#include "sql_interface.h"
class db_center
{
public:
	static db_center* get_instance();//a instance of this class
	void release();
	virtual ~db_center();
	DWORD Init(const char* host, const char* port, const char* dbname,const char* user_name,const char* pswd,unsigned short nDB);//nDB,0,mysql; 1,sql server; 2,oracle;3.sqlite
	sql_interface* get_interface();
	DWORD Close();
	PGconn* GetConn();
	void Lock();
	void Unlock();
protected:
private:
	db_center();
	Mutex lock;
	sql_interface* psql_itfs;
};

#endif
