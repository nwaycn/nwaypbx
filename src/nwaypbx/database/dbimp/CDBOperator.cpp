/*************************************************************************
based for NwayPBX
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License： GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/

#include <stdlib.h>
#include "CDBOperator.h"
#include "../../common/log/log.h"
#include "../../common/nway-lib/NwayStr.h"
#include <stdio.h> 
//#include <cstdlib>
#include <string.h>

/*
void PQresultPrint(PGresult *res)
{
	int nFields = PQnfields(res);
	int nTuples = PQntuples(res);   //PQcmdTuples
	int i, j;
	for (i=0; i<nTuples; i++)
	{
		for (j=0; j<nFields; j++)
		{
			printf("%s ", PQgetvalue(res, i, j));
		}
		printf("/n");
	}
}
*/
int PQfieldIndex(PGresult* res,  const char* fieldname)
{
	int nFields = PQnfields(res);

	for (int i = 0; i < nFields; i++)
	{
		char* _fieldname = PQfname(res, i);
		if (strcmp(_fieldname, fieldname) == 0) return i;
	}
	return -1;
}
char* PQgetStringField(PGresult* res, int row, const char* fieldname)
{
	int nIndex = PQfieldIndex(res, fieldname);
	if (nIndex == -1) return NULL;
	return PQgetvalue(res, row, nIndex);
}
int PQgetIntField(PGresult* res, int row, const char* fieldname , int* value)
{
	int nIndex = PQfieldIndex(res, fieldname);
	if (nIndex == -1) return -1;
	char* szVal = PQgetvalue(res, row, nIndex);
	if (isNumber(szVal))
	{
		*value = atoi(szVal);
		return 0;
	}
	else *value = INT_MIN;
	return 1;
	
}
int PQgetInt64Field(PGresult* res, int row, const char* fieldname, nway_int64_t* value)
{
	int nIndex = PQfieldIndex(res, fieldname);
	if (nIndex == -1) return -1;
	char* szVal = PQgetvalue(res, row, nIndex);
	if (isNumber(szVal))
	{
		*value = atoi64(szVal);
		return 0;
	}
	else *value = LLONG_MIN;
	return 1;
}
int PQgetUInt64Field(PGresult* res, int row, const char* fieldname, nway_uint64_t* value)
{
	int nIndex = PQfieldIndex(res, fieldname);
	if (nIndex == -1) return -1;
	char* szVal = PQgetvalue(res, row, nIndex);
	if (isNumber(szVal))
	{
		*value = atoi64(szVal);
		
		return 0;
	}
	else *value = ULLONG_MAX;
	return 1;
}
int PQgetShortField(PGresult* res, int row, const char* fieldname, short* value)
{
	int nIndex = PQfieldIndex(res, fieldname);
	if (nIndex == -1) return -1;
	char* szVal = PQgetvalue(res, row, nIndex);
	if (isNumber(szVal))
	{
		int myval = atoi(szVal);
		if (myval < SHRT_MAX && myval > SHRT_MIN)
			*value = myval;
		else *value = SHRT_MIN;

		return 0;
	}
	else *value = SHRT_MIN;
	return 1;
}
int PQgetBoolField(PGresult* res, int row, const char* fieldname, bool* value)
{
	char* szVal = strdup(PQgetStringField(res, row, fieldname));
	if (szVal && strcmp(szVal,"t")==0)
	{
		*value = true;
	}
	else if (szVal && strcmp(szVal, "f") == 0)
	{
		*value = false;
	}
	else
	{
		free(szVal);
		return -1;
	}
	return 0;
	
}
int LoadBaseConfig( list<base_config>& lstBaseConfig )
{
	db_center* dbInstance = db_center::get_instance();
	//PGresult* res = NULL;
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "select id,config_name,config_param from call_base_config;");
		 
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				base_config bc;
				nway_int64_t id;
				int nReturn = 0;
				 
				PQgetUInt64Field(res, i, "id", &bc.id)  ;
				//if (PQgetStringField(res, i, "config_name") != NULL) bc.config_name = config_name;
				bc.config_name = PQgetStringField(res, i, "config_name");
				bc.config_param = PQgetStringField(res, i, "config_param");
				lstBaseConfig.push_back(bc);
			}
			

		}
		 
		/*while(cmd.FetchNext())
		{
			base_config bc;
			bc.id= cmd.Field("id").asNumeric();
			bc.config_name = cmd.Field("config_name").asString();
			bc.config_param = cmd.Field("config_param").asString();
			lstBaseConfig.push_back(bc);
			printf("LoadBaseConfig any \n");
		}*/
		
	}
	catch (...)
	{
		//PQerrorMessage(dbInstance->GetConn()); 
		LOGDBERR(__FILE__, __LINE__, "load base config error:%s\n" );
		nSuccess = -1; 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadExtensionGroup( list<NwayExtensionGroup>& lstExtensionGroup )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "select id,group_name  from call_extension_groups;");
		 

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				NwayExtensionGroup eg;
				nway_int64_t id;
				 
				 PQgetUInt64Field(res, i, "id", &eg.id) ;
				eg.group_name = PQgetStringField(res, i, "group_name");
				lstExtensionGroup.push_back(eg);
			}
		}
		
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		 
		sprintf(errmsg,"load extension group error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1; 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadGateway( list<NwayGateway>& lstGateway )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "select id,gateway_name,gateway_url,call_prefix,max_call  from call_gateway;");
		 
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				NwayGateway eg;
				nway_int64_t id;
				int max_call;
				 PQgetUInt64Field(res, i, "id", &eg.id) ;
				eg.gateway_name = PQgetStringField(res, i, "gateway_name");// cmd.Field("gateway_name").asString();
				eg.call_prefix = PQgetStringField(res, i, "call_prefix");//cmd.Field("call_prefix").asString();
				eg.gateway_url = PQgetStringField(res, i, "gateway_url");//cmd.Field("gateway_url").asString();
				PQgetIntField(res,i, "max_call",&eg.max_call) ;

				lstGateway.push_back(eg);
			}
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };

		sprintf(errmsg, "load gateway error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadDialplanDetails( list<NwayDialplanDetail>& lstDialplanDetail )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "select dialplan_id ,id,dialplan_detail_tag, dialplan_detail_data  , \
			dialplan_detail_inline  ,dialplan_detail_group_id  ,dialplan_detail_order  , \
			dialplan_detail_break  ,dialplan_detail_type_id,ring_id    from call_dialplan_details group by dialplan_id,id order by dialplan_detail_order;");
		 
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				NwayDialplanDetail eg;
				nway_int64_t n64Tmp;
				int nTmp;
				bool bTmp;
				if (PQgetInt64Field(res, i, "id", &n64Tmp) == 0) eg.id = n64Tmp;
				if (PQgetBoolField(res, i, "dialplan_detail_break", &bTmp) == 0) eg.dialplan_detail_break =bTmp;
				eg.dialplan_detail_data = PQgetStringField(res, i, "dialplan_detail_data") ;
				if (PQgetInt64Field(res, i, "dialplan_detail_group_id", &n64Tmp) == 0) eg.dialplan_detail_group_id = n64Tmp;
				eg.dialplan_detail_inline = PQgetStringField(res, i, "dialplan_detail_inline") ;
				if (PQgetIntField(res, i, "dialplan_detail_order", &nTmp) ==0) eg.dialplan_detail_order = nTmp;
				eg.dialplan_detail_tag = PQgetStringField(res, i, "dialplan_detail_tag") ;
				if (PQgetIntField(res, i, "dialplan_detail_type_id", &nTmp) == 0) eg.dialplan_detail_type_id = nTmp;
				if (PQgetInt64Field(res, i, "ring_id", &n64Tmp) == 0) eg.ring_id = n64Tmp;
				if (PQgetInt64Field(res, i, "dialplan_id", &n64Tmp) == 0) eg.dialplan_id = n64Tmp;


				lstDialplanDetail.push_back(eg);
			}
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load dialplan detail error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadDialplan( list<NwayDialplan>& lstDialplan )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();
	 
	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, dialplan_name, dialplan_context, dialplan_number, dialplan_order, \
			dialplan_description, dialplan_enabled, dialplan_continue \
			FROM call_dialplans where dialplan_enabled=True;");

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{

				NwayDialplan myobj;
				//myobj.id = cmd.Field("id").asNumeric();
				PQgetUInt64Field(res, i, "id", &myobj.id);
				myobj.dialplan_context = PQgetStringField(res,i, "dialplan_context" );
				PQgetBoolField(res,i, "dialplan_continue", &myobj.dialplan_continue);
				myobj.dialplan_name = PQgetStringField(res, i, "dialplan_name") ;
				myobj.dialplan_number = PQgetStringField(res, i, "dialplan_number") ;
				PQgetIntField(res,i,  "dialplan_order", &myobj.dialplan_order) ;
				 
				lstDialplan.push_back(myobj);
			}
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "dialplan load error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadIvrDetails( list<NwayIVRDetail>& lstIvrDeatail )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, ivr_menu_id, ivr_menu_option_digits, ivr_menu_option_param,  \
			ivr_menu_option_order,  ivr_menu_option_action_id \
			FROM call_ivr_menu_options order by ivr_menu_option_order;");

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{

				NwayIVRDetail myobj;
				PQgetUInt64Field(res, i, "id", &myobj.id);
				PQgetUInt64Field(res, i, "ivr_menu_id", &myobj.ivr_menu_id);
				PQgetIntField(res, i, "ivr_menu_option_action_id",&myobj.ivr_menu_option_action_id);  
				myobj.ivr_menu_option_digits = PQgetStringField(res, i,  "ivr_menu_option_digits" );
				PQgetIntField(res, i, "ivr_menu_option_order", &myobj.ivr_menu_option_order); 
				myobj.ivr_menu_option_param = PQgetStringField(res, i, "ivr_menu_option_param") ;

				lstIvrDeatail.push_back(myobj);
			}
		}
		 
	} 
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "Load ivr details error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadIvrs( list<NwayIVR>& lstIvr )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, ivr_menu_name, ivr_menu_extension, ivr_menu_confirm_macro,  \
			ivr_menu_confirm_key, ivr_menu_confirm_attempts, ivr_menu_timeout,  \
			ivr_menu_exit_data, ivr_menu_inter_digit_timeout, ivr_menu_max_failures, \
			ivr_menu_max_timeouts, ivr_menu_digit_len, ivr_menu_direct_dial, \
			ivr_menu_cid_prefix, ivr_menu_description, ivr_menu_call_crycle_order, \
			ivr_menu_enabled, ivr_menu_call_order_id, ivr_menu_greet_long_id, \
			ivr_menu_greet_short_id, ivr_menu_invalid_sound_id, ivr_menu_exit_sound_id, \
			ivr_menu_ringback_id, ivr_menu_exit_app_id \
			FROM call_ivr_menus where ivr_menu_enabled=True;");
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
        		NwayIVR myobj;
				 
				PQgetUInt64Field(res, i, "id", &myobj.id);
				PQgetShortField(res, i, "ivr_menu_call_crycle_order", &myobj.ivr_menu_call_crycle_order);
				PQgetUInt64Field(res, i, "ivr_menu_call_order_id", &myobj.ivr_menu_call_order_id);
				 
				myobj.ivr_menu_cid_prefix = PQgetStringField(res, i, "ivr_menu_cid_prefix") ;
				PQgetShortField(res, i, "ivr_menu_confirm_attempts", &myobj.ivr_menu_confirm_attempts);
				 
				myobj.ivr_menu_confirm_key = PQgetStringField(res, i, "ivr_menu_confirm_key") ;
				myobj.ivr_menu_confirm_macro = PQgetStringField(res, i, "ivr_menu_confirm_macro") ;
				PQgetShortField(res, i, "ivr_menu_digit_len", &myobj.ivr_menu_digit_len);
				 
				myobj.ivr_menu_direct_dial = PQgetStringField(res, i, "ivr_menu_direct_dial");

				PQgetIntField(res, i, "ivr_menu_exit_app_id", &myobj.ivr_menu_exit_app_id);
				 
				myobj.ivr_menu_exit_data = PQgetStringField(res, i, "ivr_menu_exit_data");
				PQgetUInt64Field(res, i, "ivr_menu_exit_sound_id", &myobj.ivr_menu_exit_sound_id);
				 
				myobj.ivr_menu_extension = PQgetStringField(res, i, "ivr_menu_extension") ;
				PQgetUInt64Field(res, i, "ivr_menu_greet_long_id", &myobj.ivr_menu_greet_long_id);
				PQgetUInt64Field(res, i, "ivr_menu_greet_short_id", &myobj.ivr_menu_greet_short_id);
				 

				PQgetIntField(res, i, "ivr_menu_inter_digit_timeout", &myobj.ivr_menu_inter_digit_timeout);
				PQgetUInt64Field(res, i, "ivr_menu_invalid_sound_id", &myobj.ivr_menu_invalid_sound_id);
				 

				PQgetShortField(res, i, "ivr_menu_max_failures", &myobj.ivr_menu_max_failures);
				PQgetIntField(res, i, "ivr_menu_max_timeouts", &myobj.ivr_menu_max_timeouts);
				 
				myobj.ivr_menu_name = PQgetStringField(res, i, "ivr_menu_name");
				PQgetUInt64Field(res, i, "ivr_menu_ringback_id", &myobj.ivr_menu_ringback_id);
				 

				PQgetIntField(res, i, "ivr_menu_timeout", &myobj.ivr_menu_timeout);
				lstIvr.push_back(myobj);
			}
		}
		 
	} 
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load ivr error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadRings( list<NwayRing>& lstRings )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, ring_path	FROM call_rings;");
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{


				NwayRing myobj;
				 
				PQgetUInt64Field(res, i, "id", &myobj.id);
				myobj.filename = PQgetStringField(res,i, "ring_path") ;
				lstRings.push_back(myobj);
				 
			}
		}
		 
	}
	catch (...)
	{

		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load rings error:%s\n", PQresultErrorMessage(res) );
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadOutsides( list<Outside_line>& lstOutsides )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, outside_line_name, outside_line_number, inside_line_number, \
			call_order_id, call_crycle_order, is_record, is_voice_mail \
			FROM call_outside_config;");
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{


				Outside_line myobj;
				 
				PQgetUInt64Field(res, i, "id", &myobj.id);
				 
				PQgetUInt64Field(res, i, "call_crycle_order", &myobj.call_crycle_order);
				 
				PQgetUInt64Field(res, i, "call_order_id", &myobj.call_order_id);

				myobj.inside_line_number = PQgetStringField(res,i,"inside_line_number") ;
				 
				PQgetBoolField(res, i, "is_record", &myobj.is_record);
				 
				PQgetBoolField(res, i, "is_voice_mail", &myobj.is_voice_mail);
				myobj.outside_line_name = PQgetStringField(res, i, "outside_line_name") ;
				myobj.outside_line_number =  PQgetStringField(res,i,"outside_line_number") ;
				lstOutsides.push_back(myobj);
			}
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load outsides error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadInOutMapping( list<In_Out_Mapping>& lstInOutMapping )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, outside_line_id, inside_line_id, order_number \
			FROM in_out_mapping;");
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{


				In_Out_Mapping myobj;
				PQgetUInt64Field(res, i, "inside_line_id", &myobj.inside_line_id);
				PQgetUInt64Field(res, i, "outside_line_id", &myobj.outside_line_id);
				PQgetUInt64Field(res, i, "order_number", &myobj.order_number);

				lstInOutMapping.push_back(myobj);
			}
		}
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load in out mapping error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int LoadCalloutGateway( list<NwayCalloutGateway>& lstCalloutGateways )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "SELECT id, name, gateway_id	FROM callout_gateways;");
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				NwayCalloutGateway myobj;			 
				PQgetUInt64Field(res, i, "id", &myobj.id);
				PQgetUInt64Field(res, i, "gateway_id", &myobj.gateway_id);			
				myobj.name = PQgetStringField(res,i, "name") ;
				lstCalloutGateways.push_back(myobj);
			}
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load gateway error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

CExtension::CExtension()
{

}

CExtension::~CExtension()
{

}


int CExtension::LoadExtension( list<NwayExtension>& lstExtension )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(), "select id,extension_name,extension_number,callout_number,extension_type, \
						   group_id, extension_login_state, extension_reg_state, callout_gateway, \
						   is_allow_callout,extension_pswd,is_record,is_disable from call_extension order by id;");
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{

				NwayExtension ne;
				PQgetUInt64Field(res, i, "id", &ne.id);

				PQgetUInt64Field(res, i, "callout_gateway", &ne.callout_gateway);

				ne.callout_number = PQgetStringField(res, i, "callout_number");
				ne.extension_name = PQgetStringField(res, i, "extension_name");
				ne.extension_number = PQgetStringField(res, i, "extension_number");
				PQgetIntField(res, i, "extension_type", &ne.extension_type);

				PQgetUInt64Field(res, i, "group_id", &ne.group_id);
				PQgetIntField(res, i, "is_allow_callout", &ne.is_allow_callout);
				PQgetBoolField(res, i, "is_record", &ne.is_record);
				PQgetBoolField(res, i, "is_disable", &ne.is_disable);

				string strTmp;
				strTmp = PQgetStringField(res, i, "extension_reg_state");
				if (strTmp == "REGED")
				{
					ne.reg_state = AGENT_REG_SUCCESS;
				}
				else
					//暂时不用管注册，认为开了分机就是注册成功的
					ne.reg_state = AGENT_REG_FAILED;
				//////////////////////////////////////////////////////////////////////////
				strTmp = PQgetStringField(res, i, "extension_login_state");
				if (strTmp == "success")
					ne.login_state = AGENT_LOGIN_SUCCESS;
				else if (strTmp == "busy")
					ne.login_state = AGENT_LOGIN_BUSY;
				else if (strTmp == "leaved")
					ne.login_state = AGENT_LOGIN_LEAVED;
				else
					ne.login_state = AGENT_LOGIN_LOGOUT;
				ne.password = PQgetStringField(res, i, "extension_pswd");
				ne.call_state = CALLIN_STANDBY; //空闲状态
				printf("load extension:%s\n", ne.extension_number.c_str());
				lstExtension.push_back(ne);
			}
		}
		 
		//cmd.setCommandText("update call_extension set extension_login_state=\'success\';");

		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "load extensions error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CExtension::UpdateLoginState( nway_uint64_t& id,int nLogin )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		strSQL = "update call_extension set extension_login_state=\'";
		switch (nLogin)
		{
		case 0:
			{
				strSQL += "success";
			}
			break;
		case 1:
			{
				strSQL += "logout";
			}
			break;
		case 2:
			strSQL += "busy";
			break;
		case 3:
			strSQL += "leaved";
			break;
		default:
			;
		}
		strSQL += "\' where id=";
		char sTmp[200];
		sprintf(sTmp,"%lld\0",id);
		strSQL += sTmp;
		res = PQexec(dbInstance->GetConn(), strSQL.c_str());
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "update extension error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CExtension::UpdateRegState( nway_uint64_t& id, const char* szState )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{		
		char sTmp[200];
		sprintf(sTmp,"update call_extension set extension_reg_state=\'%s\' where id=%lld;\n", szState, id);
		 
		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "update extension register error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

CCallDetailRecord::CCallDetailRecord()
{

}

CCallDetailRecord::~CCallDetailRecord()
{

}

int CCallDetailRecord::StartCall( const char* caller_name, const char* caller_number, const char* called_number, nway_uint64_t& id)
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		 
		 
		char szCmd[4000] = { 0 };
		sprintf(szCmd, "SELECT * from insertnewcall( \
			\'%s\', \
			\'%s\', \
			\'%s\', \
			0,  \
			%lld,\
			0   \
			); ", caller_name, caller_number , called_number, 0
			);
		res = PQexec(dbInstance->GetConn(), szCmd);

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				PQgetUInt64Field(res, i, "cdrid", &id);
				break;
			}
		}

		PQclear(res);
		
		sprintf(szCmd,"INSERT INTO call_in_out_event("
			"aleg_number, bleg_number,  event_id, event_time, "
			"is_read,cdr_id)"
			"VALUES (\'%s\',\'%s\', 1,current_timestamp,False,%lld)\n\0",caller_number,called_number,id);
		res = PQexec(dbInstance->GetConn(), szCmd);

	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "start call error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::StartCall( const char* caller_name, const char* caller_number, const char* called_number, nway_uint64_t& id,bool bAutoCallout, nway_uint64_t& task_id  )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		short nAC = 0;
		if (bAutoCallout)  nAC = 1;
		else nAC = 0;
		char szCmd[4000] = { 0 };
		sprintf(szCmd, "SELECT * from insertnewcall( \
			\'%s\', \
			\'%s\', \
			\'%s\', \
			%d,  \
			%lld,\
			0   \
			); ", caller_name, caller_number, called_number, nAC, task_id
			);
		res = PQexec(dbInstance->GetConn(), szCmd);

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				PQgetUInt64Field(res, i, "cdrid", &id);
				break;
			}
		}

		PQclear(res);
		
		char szCmd[4000]={0};
		sprintf(szCmd,"INSERT INTO call_in_out_event("
			"aleg_number, bleg_number,  event_id, event_time, "
			"is_read,cdr_id)"
			"VALUES (\'%s\',\'%s\', 1,current_timestamp,False,%lld)\n\0",caller_number,called_number,id);
		res = PQexec(dbInstance->GetConn(), szCmd);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "start call error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::A_AnswerCall( nway_uint64_t& id )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		strSQL = "update call_cdr set a_answer_stamp=current_timestamp,a_leg_called=True where id=";
		char sTmp[200];
		sprintf(sTmp,"%lld \n",id);

		strSQL += sTmp;
		printf(strSQL.c_str());
		res = PQexec(dbInstance->GetConn(), strSQL.c_str());
		
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "update call error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::B_AnswerCall( nway_uint64_t& id, const char* dest_number, const char* digites_dail )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		strSQL = "update call_cdr set b_leg_called=True, b_answer_stamp=current_timestamp,destination_number=\'" ;
		strSQL += dest_number;
		strSQL += "\' where id=";
		char sTmp[200];
		sprintf(sTmp,"%lld \n",id);
		
		strSQL += sTmp;
		res = PQexec(dbInstance->GetConn(), strSQL.c_str());
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "update call error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::A_EndCall( nway_uint64_t& id, NWAY_HANGUP_CAUSE cause,int gatewayid, NWAY_HANGUP_DIRECTION direction )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	PGresult* res = NULL;
	dbInstance->Lock();

	try
	{
		string strSQL;
		char sTmp[200];
		strSQL = "update call_cdr set a_end_stamp=current_timestamp,hangup_cause=\'";//destination_number=\'" + dest_number+"\' " ;
		switch(cause)
		{
		case HANGUP_CAUSE_NORMAL:   //普通挂机
			strcpy(sTmp,"NORMAL");
			break;
		case HANGUP_CAUSE_IVR_NORMAL:  //IVR普通挂机
			strcpy(sTmp,"IVR_NORMAL");
			break;
		case HANGUP_CAUSE_UNANSWER:   //无人应答
			strcpy(sTmp,"UNANSWER");
			break;
		case HANGUP_CAUSE_IVR_UNANSWER:  //ivr无人应答
			strcpy(sTmp,"IVR_UNANSWER");
			break;
		case HANGUP_CAUSE_NO_FEE:   //费用不足
			strcpy(sTmp,"NO_FEE");
			break;
		case HANGUP_CAUSE_BUSY:   //对方忙
			strcpy(sTmp,"BUSY");
			break;
		case HANGUP_CAUSE_VOICEMAIL:  //语音信箱
			strcpy(sTmp,"VOICEMAIL");
			break;
		case HANGUP_CAUSE_UNKNOWN_NUMBER://无此号码
			strcpy(sTmp,"UNKNOWN_NUMBER");
			break;
		case HANGUP_CAUSE_NOT_CALL:       //无法呼出
			strcpy(sTmp,"CAN_NOT_CALL");
			break;
		case HANGUP_CAUSE_NOT_CALLED:      //无法接听
			strcpy(sTmp,"NOT_CALLED");
			break;
		default:
			;
		}
		strSQL += sTmp ;
		strSQL	+= "\'";
		if (gatewayid > 0 )
		{
			sprintf(sTmp,", hangup_cause_id=%d, hangup_direction=%d,call_gateway_id=%d  where id=%lld;\0",cause, direction , gatewayid,id);

		}
		else
			sprintf(sTmp,", hangup_cause_id=%d, hangup_direction=%d  where id=%lld;\0",cause, direction ,id);
		
		strSQL += sTmp;

		res = PQexec(dbInstance->GetConn(), strSQL.c_str());
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "update A_EndCall error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::B_EndCall( nway_uint64_t& id, const char* dest_number, NWAY_HANGUP_CAUSE cause )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		strSQL = "update call_cdr set b_end_stamp=current_timestamp,destination_number=\'";
		strSQL += dest_number;
		strSQL += "\' ";
		//where id=";
		char sTmp[200];
		sprintf(sTmp," where id=%lld \n\0",id);
		strSQL += sTmp;
		printf(strSQL.c_str());
		res = PQexec(dbInstance->GetConn(), strSQL.c_str());
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "update B_EndCall error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::SetRecordFile( nway_uint64_t& id,const char* filename )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		strSQL = "update call_cdr set recording_file=\'";
		strSQL	+= filename;
		strSQL += "\' ";
		//where id=";
		char sTmp[200];
		sprintf(sTmp," where id=%lld\0",id);
		strSQL += sTmp;

		res = PQexec(dbInstance->GetConn(), strSQL.c_str());
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "set record file error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::CountTime( nway_uint64_t& id )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char szCmd[1000] = { 0 };
		sprintf(szCmd, "SELECT count_time(%lld); " , id);
		PQexec(dbInstance->GetConn(), szCmd);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "count time error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CCallDetailRecord::SetDtmf( nway_uint64_t& id, const char* dtmf )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{

		char szCmd[4000]={0};
		sprintf(szCmd,"update call_cdr set input_key=\'%s\' where cdr_id=%lld;\n",dtmf, id);
		res = PQexec(dbInstance->GetConn(), szCmd);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "SetDtmf error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
 
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int InsertCallEvent( nway_uint64_t& cdr_id, nway_uint64_t& ext_id,string& a_leg_number,string& b_leg_number,string& route_number, NWAY_CALL_EVENT iEvent )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		/*if (ext_id > 0)
			strSQL += ", extension_id";
		if (route_number.length() > 0 )
		{
			strSQL += ", router_number";
		}*/
		char szCmd[4000]={0};
		if (ext_id >0 && route_number.length() > 0)
		{
			sprintf(szCmd,"INSERT INTO call_in_out_event("
				"aleg_number, bleg_number,  event_id, event_time, "
				"is_read,cdr_id ,extension_id, router_number)"
				"VALUES (\'%s\',\'%s\', %d,current_timestamp,False,%lld,%lld,\'%s\')\n\0",
				a_leg_number.c_str(),b_leg_number.c_str(),iEvent,cdr_id,ext_id,route_number.c_str());
		}
		else
		{
			if (ext_id >0 )
			{
				sprintf(szCmd,"INSERT INTO call_in_out_event("
					"aleg_number, bleg_number,  event_id, event_time, "
					"is_read,cdr_id ,extension_id, router_number)"
					"VALUES (\'%s\',\'%s\', %d,current_timestamp,False,%lld,%lld, \'%s\')\n\0",
					a_leg_number.c_str(),b_leg_number.c_str(),iEvent,cdr_id,ext_id, route_number.c_str());
			}
			else
				if (route_number.length() > 0)
				{
					sprintf(szCmd,"INSERT INTO call_in_out_event("
						"aleg_number, bleg_number,  event_id, event_time, "
						"is_read,cdr_id ,extension_id, router_number)"
						"VALUES (\'%s\',\'%s\', %d,current_timestamp,False,%lld,%lld, \'%s\')\n\0",
						a_leg_number.c_str(),b_leg_number.c_str(),iEvent, cdr_id,ext_id,route_number.c_str());
				}
				else
				{
					sprintf(szCmd,"INSERT INTO call_in_out_event("
						"aleg_number, bleg_number,  event_id, event_time, "
						"is_read,cdr_id)"
						"VALUES (\'%s\',\'%s\', %d,current_timestamp,False,%lld)\n\0",a_leg_number.c_str(),b_leg_number.c_str(),iEvent,cdr_id);
				}
			
		}
		
		res = PQexec(dbInstance->GetConn(), szCmd);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "insert call_in_out_event error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int UpdateCallEvent( nway_uint64_t& cdr_id, nway_uint64_t& ext_id )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		
		char szCmd[4000]={0};
		sprintf(szCmd,"update call_in_out_event set extension_id=%lld where cdr_id=%lld\n\0",ext_id,cdr_id);
		res = PQexec(dbInstance->GetConn(), szCmd);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "UpdateCallEvent error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
		 
	}
	dbInstance->Unlock();
	return nSuccess;
}

int DBGetClickDials( vector<Click_Dial>& vecClickDials )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(),"SELECT id, caller_number, is_agent, is_immediately, trans_number, time_plan, \
			account_number, is_called\
			FROM call_click_dial where is_called=False and is_immediately=True;");

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{

				Click_Dial cd;
				//cd.id = cmd.Field("id").asNumeric();
				PQgetUInt64Field(res, i, "id", &cd.id);
				cd.account_number = PQgetStringField(res,i,"account_number");
				cd.caller_number = PQgetStringField(res,i,"caller_number" );
				cd.trans_number = PQgetStringField(res,i,"trans_number") ;
				vecClickDials.push_back(cd);
			}
			
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "DBGetClickDials error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int DBSetClickDialed( nway_uint64_t& id )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{

		char szCmd[4000]={0};
		sprintf(szCmd,"update call_click_dial set is_called=True where id=%lld\n\0",id);
		res = PQexec(dbInstance->GetConn(), szCmd);
		printf("%s, %d sql:%s\n",__FILE__,__LINE__,szCmd);
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "DBSetClickDialed error:%s\n", PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int DBGetConfigChanged( vector<int>& vecConfigs )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		res = PQexec(dbInstance->GetConn(),"SELECT config_order "
						   "FROM call_load_config where is_load=True;");

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{

				int config_id;
				 
				PQgetIntField(res, i, "config_order", &config_id);
				vecConfigs.push_back(config_id);
				nSuccess = 1;
			}
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n",__FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int DBSetConfigChanged( int orderid )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char szCmd[1024]={0};
		sprintf(szCmd,"update call_load_config set is_load=False where config_order=%d;\n", orderid);
		res = PQexec(dbInstance->GetConn(), szCmd);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

CDBCalloutTask::CDBCalloutTask()
{

}

CDBCalloutTask::~CDBCalloutTask()
{

}

int CDBCalloutTask::GetTasks( list<Callout_Task>& lstCalloutTasks , list<NwayGateway>& lstGateways,list<NwayCalloutGateway>& lstCalloutGateways)
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		string strSQL;
		
		//strSQL = "count_time";
		strSQL = "SELECT id, callout_name, number_group_id, number_group_uploadfile, run_position, \
			time_rule_id, start_time, stop_time, ring_id, after_ring_play, \
			ring_timeout, group_id, call_project_id, concurr_type_id, concurr_number, \
			callout_state_id, total_number, wait_number, success_number, \
			failed_number, cancel_number, has_parse_from_file, callout_gateway_id, \
			max_concurr_number, second_ring_id, second_after_ring_opt, after_ring_key, \
			after_key_opt_id,outside_line_number \
			FROM call_group_callout  t where t.start_time < now() and t.stop_time >now() \
			and (callout_state_id=1 or callout_state_id=2 or callout_state_id=4) and has_parse_from_file=True ;";

		//printf("%s,%d    string:%s \n\n", __FILE__, __LINE__, strSQL.c_str());
		res = PQexec(dbInstance->GetConn(), strSQL.c_str());

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				//找到相关的定时任务
				//每条记录应先从列表中检索一遍看存在与否
				Callout_Task ct;
				 
				PQgetUInt64Field(res, i, "id", &ct.id);
				ct.callout_name = PQgetStringField(res,i,"callout_name");
				 
				PQgetIntField(res, i, "after_key_opt_id", &ct.after_key_opt_id);
				ct.after_ring_key = PQgetStringField(res, i, "after_ring_key") ;
				PQgetIntField(res, i, "after_ring_play", &ct.after_ring_play);
				PQgetUInt64Field(res, i, "call_project_id", &ct.call_project_id);
				PQgetUInt64Field(res, i, "callout_gateway_id", &ct.callout_gateway_id);
				
				PQgetUInt64Field(res, i, "callout_state_id", &ct.callout_state_id);
				PQgetIntField(res, i, "cancel_number", &ct.cancel_number);
				
				PQgetIntField(res, i, "concurr_number", &ct.concurr_number);
				PQgetUInt64Field(res, i, "concurr_type_id", &ct.concurr_type_id);
				PQgetIntField(res, i, "failed_number", &ct.failed_number);
				PQgetUInt64Field(res, i, "group_id", &ct.group_id);

				PQgetIntField(res, i, "max_concurr_number", &ct.max_concurr_number);
				PQgetUInt64Field(res, i, "number_group_id", &ct.number_group_id);
				PQgetUInt64Field(res, i, "ring_id", &ct.ring_id);
				PQgetIntField(res, i, "ring_timeout", &ct.ring_timeout);
				PQgetUInt64Field(res, i, "run_position", &ct.run_position);
				PQgetIntField(res, i, "second_after_ring_opt", &ct.second_after_ring_opt);
				PQgetUInt64Field(res, i, "second_ring_id", &ct.second_ring_id);
				PQgetIntField(res, i, "success_number", &ct.success_number);
				PQgetUInt64Field(res, i, "time_rule_id", &ct.time_rule_id);
				PQgetIntField(res, i, "total_number", &ct.total_number);
				PQgetIntField(res, i, "wait_number", &ct.wait_number);
				 
				ct.outside_line_number = PQgetStringField(res, i, "outside_line_number") ;
				 
				list<NwayCalloutGateway>::iterator coGwit = lstCalloutGateways.begin();
				for (; coGwit != lstCalloutGateways.end(); coGwit++)
				{
					NwayCalloutGateway& nwCalloutGw = *coGwit;
					if (nwCalloutGw.id == ct.callout_gateway_id)
					{
						list<NwayGateway>::iterator gwit = lstGateways.begin();
						for (; gwit != lstGateways.end(); gwit++)
						{
							NwayGateway& nwaygw = *gwit;
							if (nwaygw.id == nwCalloutGw.gateway_id)
							{
								ct.gateway_max_line = nwaygw.max_call;
								ct.gateway_url = nwaygw.gateway_url;
								ct.call_prefix = nwaygw.call_prefix;
								break;
							}
						}
						break;
					}

				}
				//////////////////////////////////////////////////////////////////////////

				//处理暂停事宜
				list<Callout_Task>::iterator it = lstCalloutTasks.begin();
				bool bFound = false;
				for (; it != lstCalloutTasks.end(); it++)
				{
					Callout_Task& nwayCt = *it;
					printf("%s,%d compare the list task:%s and this task:%s \n\n", __FILE__, __LINE__, nwayCt.callout_name.c_str(), ct.callout_name.c_str());
					if (nwayCt.id == ct.id)
					{
						if (ct.callout_state_id == 4 || trim(ct.gateway_url).length() < 3)
						{
							//处理暂停
							nwayCt.callout_state_id = 4;
							//*it = nwayCt;
						}
						//*it = nwayCt;
						bFound = true;
						break;
					}
				}
				//if ()
				//{
				//	printf("%s,%d the call out gateway:%s\n\n" , __FILE__, __LINE__, ct.gateway_url.c_str());
				//	//暂停，否则会让检测线程不停处理这个任务
				//	nwayCt.callout_state_id = 4; 
				//	continue;
				//}
				if (!bFound)
				{
					printf("%s, %d  add a call out task id: %lld ;\n", __FILE__, __LINE__, ct.id);
					lstCalloutTasks.push_back(ct);
				}

			}
		}
		//id = cmd.Param('cdrid').setAsNumeric();
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutTask::UpdateTaskStatus( nway_uint64_t& id, nway_uint64_t& run_postion, int total,int success, int failed, int cancled )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};
		 
		sprintf(sTmp,"update call_group_callout set run_position=%lld,success_number=%d,failed_number=%d,wait_number=%d,cancel_number=%d  where id=%lld \n",run_postion,success,failed,total-success-failed, cancled,id);
		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutTask::UpdateTaskState( nway_uint64_t&id, int nState )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};

		sprintf(sTmp,"update call_group_callout set callout_state_id=%d  where id=%lld ;\n",nState , id);
		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutTask::GetIdleAgentNumber( nway_uint64_t& groupid,int& nNumber )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char szTmp[1024] = {0};

		//strSQL = "count_time";
		sprintf(szTmp, "SELECT count(id) as idlecount \
			FROM call_extension t where (t.extension_login_state='success' or t.extension_login_state='') and t.group_id =%lld;\n",groupid) ;


		res = PQexec(dbInstance->GetConn(), szTmp);

		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{
				PQgetIntField(res, i, "idlecount",& nNumber);
			}
			 
		}
		 
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutTask::GetCalloutNumbers( Callout_Task& nwayct, int nMaxnumber )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();
	nway_uint64_t i64Maxid = nwayct.run_position;
	try
	{
		char szTmp[1024] = {0};

		//strSQL = "count_time";
		sprintf(szTmp, "SELECT id, group_id, \"number\", is_called, call_state, start_time, answer_time, \
			hangup_time, hangup_reason_id, answer_extension_id, record_file, wait_sec \
			FROM call_out_numbers where group_id=%lld and call_out_task_id=%lld and is_called=0  and id >%lld order by id limit %d offset 0;\n", nwayct.number_group_id,  nwayct.id, nwayct.run_position, nMaxnumber) ;
		res = PQexec(dbInstance->GetConn(), szTmp);
		int nCount = 0;
		if (PQresultStatus(res) == PGRES_TUPLES_OK)
		{
			int nTuples = PQntuples(res);
			for (int i = 0; i < nTuples; i++)
			{

				Callout_info ci;
				PQgetUInt64Field(res, i, "id", &ci.id);
				 
				ci.group_id = nwayct.id;
				ci.number = PQgetStringField(res,i,"number") ;
				 
				PQgetIntField(res, i, "wait_sec",& ci.wait_sec);
				ci.callout_state = CALLOUT_INIT;
				i64Maxid = ci.id;
				string strNumber;
				strNumber = trim(ci.number);
				if (strNumber.length() < 2 || strNumber == "110" || strNumber == "119" || strNumber == "96110")
				{
					//以后可进一步扩展为可配置
					continue;
				}
				sprintf(szTmp, "%s,%d    get a call out number:%s\n\n", __FILE__, __LINE__, ci.number.c_str());
				LOG(szTmp);
				nwayct.lstCalloutInfo.push_back(ci);
				nCount++;
			}

		}

		nwayct.run_position = i64Maxid;//更新position,由于order by故而是最大的，除非数据库中随意手工更改过
		//id = cmd.Param('cdrid').setAsNumeric();
		 
		if (nCount == 0)
		{
			//号码获取不到了，则是可以认为完成这个任务了
			sprintf(szTmp,"Update call_group_callout set callout_state_id=3 where id=%lld ;\n",nwayct.id);
		}
		else
		{
			sprintf(szTmp,"Update call_group_callout set callout_state_id=2 where id=%lld ;\n",nwayct.id);
		}
		PQclear(res);
		res = PQexec(dbInstance->GetConn(), szTmp);

	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutInfo::StartCallout( nway_uint64_t& id,nway_uint64_t& cdrid )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};

		sprintf(sTmp,"update call_out_numbers set is_called=1,start_time=current_timestamp,cdr_id=%lld  where id=%lld \n\0", id, cdrid);
		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

CDBCalloutInfo::CDBCalloutInfo()
{

}

CDBCalloutInfo::~CDBCalloutInfo()
{

}

int CDBCalloutInfo::ExtensionAnswer( nway_uint64_t& id,nway_uint64_t& extension_id,const char* record_file )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};

		sprintf(sTmp,"update call_out_numbers set answer_extension_id=%lld,record_file=\'%s\' where id=%lld \n",extension_id,record_file,id );
		
		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutInfo::SetAlegAnswer( nway_uint64_t& id )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};

		sprintf(sTmp,"update call_out_numbers set answer_time=current_timestamp   where id=%lld \n",id);

		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutInfo::SetAlegHangup( nway_uint64_t& id, int nHangupid )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};

		sprintf(sTmp,"update call_out_numbers set hangup_time=current_timestamp, hangup_reason_id=%d   where id=%lld \n",nHangupid,id);

		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}

int CDBCalloutInfo::SetCalled( nway_uint64_t& id )
{
	db_center* dbInstance = db_center::get_instance();
	PGresult* res = NULL;
	int nSuccess=0;
	dbInstance->Lock();

	try
	{
		char sTmp[2000] = {0};

		sprintf(sTmp,"update call_out_numbers set is_called=1 where id=%lld \n", id);

		res = PQexec(dbInstance->GetConn(), sTmp);
	}
	catch (...)
	{
		char errmsg[1024] = { 0 };
		sprintf(errmsg, "%s error:%s\n", __FUNCTION__, PQresultErrorMessage(res));
		LOGDBERR(__FILE__, __LINE__, errmsg);
		nSuccess = -1;
	}
	PQclear(res);
	dbInstance->Unlock();
	return nSuccess;
}
