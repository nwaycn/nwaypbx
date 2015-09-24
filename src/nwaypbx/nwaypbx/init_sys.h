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


#ifndef __NWAYPBX_INIT_SYSTEM_H___
#define __NWAYPBX_INIT_SYSTEM_H___

/*
Initialize database
testing it's can used 
*/
bool init_database();

/*
check this system extension number and status
*/
bool check_extension();

/*
check dialplans is valid;
*/
bool check_dialplan();

/*
check dialplan detail is valid
*/
bool check_dialplan_detail();

/*
check base config is valid
it is for ring path and record path
*/
bool check_base_config();

/*
check ivr is valid
this is check but not to process
*/
bool check_ivr();

/*
check ivr detail is valid
it to check when check_ivr is true
*/
bool check_ivr_detail();

/*
check esl inbound
*/
bool check_esl_inboud();



#endif
