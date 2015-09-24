#include "init_sys.h"
#include "../database/config/config.h"
#include <string>
#include "../database/dbimp/CDBOperator.h"
using namespace std;
bool init_database()
{
	bool bRes = false;
	HConf *info = (HConf *)malloc(sizeof(HConf));
	init_conf(info, "./database.conf");
	char buf[50];
	get_conf_value(info, "host", buf);
	string host = buf;
	get_conf_value(info, "port", buf);
	string port = buf;

	get_conf_value(info, "dbname", buf);  ///Из:192.168.1.209@mccrm
	string dbname = buf;
	get_conf_value(info, "username", buf);
	string username = buf;
	get_conf_value(info, "password", buf);
	string password = buf;
	db_center* dbInstance = db_center::get_instance();
	DWORD dwStatus = 0;// dbInstance->Init(servername.c_str(), username.c_str(), password.c_str(), 4);
	dwStatus = dbInstance->Init(host.c_str(),port.c_str(), dbname.c_str(),username.c_str(), password.c_str(), 4);
	if (dwStatus == 0)
	{
		 
		bRes = true;
	}
	else printf("server:%s,user:%s, initlize database failed\n", dbname.c_str(), username.c_str());
	return bRes;
}

bool check_extension()
{
	db_center* dbInstance = db_center::get_instance();
	CExtension db_ext;
	list<NwayExtension> extlist;
	bool nway_result = false;
	int n_result = db_ext.LoadExtension(extlist);
	if (n_result == 0 && extlist.size() >0)
	{
		nway_result = true;
	}
	extlist.clear();//
	return false;
}

bool check_dialplan()
{
	return false;
}

bool check_dialplan_detail()
{
	return false;
}

bool check_base_config()
{
	return false;
}

bool check_ivr()
{
	return false;
}

bool check_ivr_detail()
{
	return false;
}

bool check_esl_inboud()
{
	return false;
}
