#include "stdafx.h"
#include "db_center.h"
//#include "../../../common/Common.h"
db_center* dbinstance =NULL;
db_center::db_center():psql_itfs(NULL)
{

}

db_center* db_center::get_instance()
{
	if (dbinstance == NULL)
	{
		dbinstance = new db_center();

	}
	return dbinstance;
}

DWORD db_center::Init( const char* dbname,const char* user_name,const char* pswd,unsigned short nDB )
{
	if (psql_itfs)
	{
		delete psql_itfs;
		psql_itfs = NULL;
	}
	if (psql_itfs == NULL)
	{
		psql_itfs = new sql_interface();
		if (psql_itfs )
		{
			return psql_itfs->Init(dbname,user_name,pswd,nDB);
		}
	}
	return 1;
}

db_center::~db_center()
{
	
}

sql_interface* db_center::get_interface()
{
    
		return psql_itfs;

}

DWORD db_center::Close()
{
	if (psql_itfs)
	{
		delete psql_itfs;
		psql_itfs = NULL;
	}
	return 0;
}

SAConnection* db_center::GetConn()
{
	return psql_itfs->getConn();
}

void db_center::Lock()
{
	lock.Lock();
}

void db_center::Unlock()
{
	lock.Unlock();
}

void db_center::release()
{
	try
	{
		if (psql_itfs)
		{

			delete psql_itfs;
			psql_itfs = NULL;
		}
		if (dbinstance)
		{
			delete dbinstance;
			dbinstance = NULL;
		}
	}
	catch(...)
	{
		;
	}
	
}
