#include "statedef.h"
#include "structdef.h"

NwayExtension::NwayExtension()
{
	clear();
}

NwayExtension::NwayExtension( const NwayExtension& t )
{
	this->call_state= t.call_state;
	this->id = t.id;
	this->callout_gateway = t.callout_gateway;
	this->callout_number = t.callout_number;
	this->extension_name = t.extension_name;
	this->extension_number = t.extension_number;
	this->extension_type = t.extension_type;
	this->group_id = t.group_id;
	this->is_allow_callout = t.is_allow_callout;
	this->login_state = t.login_state;
	this->password = t.password;
	this->reg_state = t.reg_state;
	this->callout_state = t.callout_state;
	this->is_record = t.is_record;
	this->is_disable = t.is_disable;
}

void NwayExtension::clear()
{
	id = 0;
	extension_type = 0;
	extension_number = "";
	extension_name = "";
	password = "";
	is_allow_callout = 0;
	group_id = 0;
	callout_number = "";
	callout_gateway = 0;
	reg_state = AGENT_REG_FAILED;
	login_state = AGENT_LOGIN_LOGOUT;
	call_state = CALLIN_STANDBY;
	callout_state = CALLOUT_INIT;
	is_record = false;
	is_disable = true;
}

NwayExtension& NwayExtension::operator=(const  NwayExtension& t )
{
	this->call_state= t.call_state;
	this->id = t.id;
	this->callout_gateway = t.callout_gateway;
	this->callout_number = t.callout_number;
	this->extension_name = t.extension_name;
	this->extension_number = t.extension_number;
	this->extension_type = t.extension_type;
	this->group_id = t.group_id;
	this->is_allow_callout = t.is_allow_callout;
	this->login_state = t.login_state;
	this->password = t.password;
	this->reg_state = t.reg_state;
	this->callout_state = t.callout_state;
	this->is_record = t.is_record;
	this->is_disable = t.is_disable;
	return *this;
}

NwayRing::NwayRing()
{
	id = 0;
	filename = "";
}

NwayRing::NwayRing( const NwayRing& t )
{
	this->id = t.id;
	this->filename = t.filename;
}

NwayRing& NwayRing::operator=(const  NwayRing& t )
{
	this->id = t.id;
	this->filename = t.filename;return *this;
}

NwayExtensionGroup::NwayExtensionGroup()
{
	id = 0;
	group_name = "";
}

NwayExtensionGroup::NwayExtensionGroup(const  NwayExtensionGroup& t )
{
	this->id = t.id;
	this->group_name = t.group_name;
}

NwayExtensionGroup& NwayExtensionGroup::operator=( const NwayExtensionGroup& t )
{
	this->id = t.id;
	this->group_name = t.group_name;return *this;
}

NwayGateway::NwayGateway()
{
	clear();
}

NwayGateway::NwayGateway(const  NwayGateway& t )
{
	this->id =t.id; 
	this->gateway_name =t.gateway_name;  
	this->gateway_url= t.gateway_url;  
	this->call_prefix =t.call_prefix; 
	this->max_call=t.max_call; 
}

void NwayGateway::clear()
{
	id = 0; 
	gateway_name =""  ;
	gateway_url= "";  
	call_prefix =""; 
	max_call=0; 
}

NwayGateway& NwayGateway::operator=( const NwayGateway& t )
{
	this->id =t.id; 
	this->gateway_name =t.gateway_name;  
	this->gateway_url= t.gateway_url;  
	this->call_prefix =t.call_prefix; 
	this->max_call=t.max_call; return *this;
}

NwayDialplanDetail::NwayDialplanDetail()
{

}

NwayDialplanDetail::NwayDialplanDetail(const  NwayDialplanDetail& t )
{
	this->id=t.id;
	this->dialplan_id =t.dialplan_id;
	this->dialplan_detail_tag=t.dialplan_detail_tag ;
	this->dialplan_detail_type_id=t.dialplan_detail_type_id ;
	this->dialplan_detail_data=t.dialplan_detail_data ;
	this->dialplan_detail_inline=t.dialplan_detail_inline ;
	this->dialplan_detail_group_id=t.dialplan_detail_group_id;
	this->dialplan_detail_order =t.dialplan_detail_order;
	this->dialplan_detail_break=t.dialplan_detail_break ;
	this->ring_id = t.ring_id;
}

void NwayDialplanDetail::clear()
{
	id=0;
	dialplan_id =0;
	dialplan_detail_tag="" ;
	dialplan_detail_type_id=0 ;
	dialplan_detail_data="" ;
	dialplan_detail_inline="" ;
	dialplan_detail_group_id=0;
	dialplan_detail_order =0;
	dialplan_detail_break=false ;
	ring_id = 0;

}

NwayDialplanDetail& NwayDialplanDetail::operator=(const  NwayDialplanDetail& t )
{
	this->id=t.id;
	this->dialplan_id =t.dialplan_id;
	this->dialplan_detail_tag=t.dialplan_detail_tag ;
	this->dialplan_detail_type_id=t.dialplan_detail_type_id ;
	this->dialplan_detail_data=t.dialplan_detail_data ;
	this->dialplan_detail_inline=t.dialplan_detail_inline ;
	this->dialplan_detail_group_id=t.dialplan_detail_group_id;
	this->dialplan_detail_order =t.dialplan_detail_order;
	this->dialplan_detail_break=t.dialplan_detail_break ;
	this->ring_id = t.ring_id;
	return *this;
}

bool NwayDialplanDetail::operator<( const NwayDialplanDetail& t )
{
	return dialplan_detail_order < t.dialplan_detail_order;
}

NwayDialplan::NwayDialplan()
{

}

NwayDialplan::NwayDialplan( const NwayDialplan& t )
{
	id = t.id;

	dialplan_name = t.dialplan_name ;
	dialplan_context = t.dialplan_context;
	dialplan_number= t.dialplan_number ;
	dialplan_continue =t.dialplan_continue;
	dialplan_order = t.dialplan_order;

	dialplan_enabled=t.dialplan_enabled ;
	//this->re = t.re;
}

void NwayDialplan::clear()
{
	id = 0;

	dialplan_name = "" ;
	dialplan_context ="";
	dialplan_number="" ;
	dialplan_continue =false;
	dialplan_order =0;

	dialplan_enabled=false ;
	/*if (re)
	{
	free(re);
	re = NULL;
	}*/
}

NwayDialplan& NwayDialplan::operator=(const  NwayDialplan& t )
{
	id = t.id;

	dialplan_name = t.dialplan_name ;
	dialplan_context = t.dialplan_context;
	dialplan_number= t.dialplan_number ;
	dialplan_continue =t.dialplan_continue;
	dialplan_order = t.dialplan_order;

	dialplan_enabled=t.dialplan_enabled ;
	//this->re = t.re;
	return *this;
}

void NwayIVRDetail::clear()
{
	id=0;
	ivr_menu_id =0;  
	ivr_menu_option_digits="" ;
	ivr_menu_option_action_id = 0 ;
	ivr_menu_option_param ="";
	ivr_menu_option_order=0 ;
	ivr_menu_option_description ="";
}

NwayIVRDetail::NwayIVRDetail()
{
	clear();
}

NwayIVRDetail::NwayIVRDetail( const NwayIVRDetail& t )
{
	id=t.id;
	ivr_menu_id =t.ivr_menu_id;  
	ivr_menu_option_digits=t.ivr_menu_option_digits ;
	ivr_menu_option_action_id = t.ivr_menu_option_action_id ;
	ivr_menu_option_param =t.ivr_menu_option_param;
	ivr_menu_option_order=t.ivr_menu_option_order ;
	ivr_menu_option_description =t.ivr_menu_option_description;
}

NwayIVRDetail& NwayIVRDetail::operator=(const  NwayIVRDetail& t )
{
	id=t.id;
	ivr_menu_id =t.ivr_menu_id;  
	ivr_menu_option_digits=t.ivr_menu_option_digits ;
	ivr_menu_option_action_id = t.ivr_menu_option_action_id ;
	ivr_menu_option_param =t.ivr_menu_option_param;
	ivr_menu_option_order=t.ivr_menu_option_order ;
	ivr_menu_option_description =t.ivr_menu_option_description;return *this;
}

bool NwayIVRDetail::operator<( const NwayIVRDetail& t )
{
	return ivr_menu_option_order < t.ivr_menu_option_order;
}

void NwayIVR::clear()
{
	id =0;
	ivr_menu_name="" ;
	ivr_menu_extension ="";	 
	ivr_menu_greet_long_id=0 ; 
	ivr_menu_greet_short_id = 0; 
	ivr_menu_invalid_sound_id=0; 
	ivr_menu_exit_sound_id = 0; 
	ivr_menu_ringback_id=0; 
	ivr_menu_exit_app_id=0; 
	ivr_menu_confirm_macro="" ;
	ivr_menu_confirm_key="" ;
	ivr_menu_confirm_attempts =0; 
	ivr_menu_timeout=0 ; 

	ivr_menu_exit_data="" ;
	ivr_menu_inter_digit_timeout =0; 
	ivr_menu_max_failures =0; 
	ivr_menu_max_timeouts=0 ; 
	ivr_menu_digit_len =0; 
	ivr_menu_direct_dial=""; 

	ivr_menu_cid_prefix=""; 
	ivr_menu_description=""; 
	ivr_menu_call_crycle_order = 0; 
	ivr_menu_enabled =false; 
	ivr_menu_call_order_id =0; 
}

NwayIVR::NwayIVR(const  NwayIVR& t )
{
	id =t.id;
	ivr_menu_name=t.ivr_menu_name;
	ivr_menu_extension =t.ivr_menu_extension;	 
	ivr_menu_greet_long_id=t.ivr_menu_greet_long_id ; 
	ivr_menu_greet_short_id = t.ivr_menu_greet_short_id; 
	ivr_menu_invalid_sound_id=t.ivr_menu_invalid_sound_id; 
	ivr_menu_exit_sound_id = t.ivr_menu_exit_sound_id; 
	ivr_menu_ringback_id=t.ivr_menu_ringback_id; 
	ivr_menu_exit_app_id=t.ivr_menu_exit_app_id; 
	ivr_menu_confirm_macro=t.ivr_menu_confirm_macro ;
	ivr_menu_confirm_key=t.ivr_menu_confirm_key ;
	ivr_menu_confirm_attempts =t.ivr_menu_confirm_attempts; 
	ivr_menu_timeout=t.ivr_menu_timeout; 

	ivr_menu_exit_data=t.ivr_menu_exit_data ;
	ivr_menu_inter_digit_timeout =t.ivr_menu_inter_digit_timeout; 
	ivr_menu_max_failures =t.ivr_menu_max_failures; 
	ivr_menu_max_timeouts=t.ivr_menu_max_timeouts ; 
	ivr_menu_digit_len =t.ivr_menu_digit_len; 
	ivr_menu_direct_dial=t.ivr_menu_direct_dial; 

	ivr_menu_cid_prefix=t.ivr_menu_cid_prefix; 
	ivr_menu_description=t.ivr_menu_description; 
	ivr_menu_call_crycle_order =t.ivr_menu_call_crycle_order; 
	ivr_menu_enabled =t.ivr_menu_enabled; 
	ivr_menu_call_order_id =t.ivr_menu_call_order_id; 
}

NwayIVR::NwayIVR()
{
	clear();
}

NwayIVR& NwayIVR::operator=(const  NwayIVR& t )
{
	id =t.id;
	ivr_menu_name=t.ivr_menu_name;
	ivr_menu_extension =t.ivr_menu_extension;	 
	ivr_menu_greet_long_id=t.ivr_menu_greet_long_id ; 
	ivr_menu_greet_short_id = t.ivr_menu_greet_short_id; 
	ivr_menu_invalid_sound_id=t.ivr_menu_invalid_sound_id; 
	ivr_menu_exit_sound_id = t.ivr_menu_exit_sound_id; 
	ivr_menu_ringback_id=t.ivr_menu_ringback_id; 
	ivr_menu_exit_app_id=t.ivr_menu_exit_app_id; 
	ivr_menu_confirm_macro=t.ivr_menu_confirm_macro ;
	ivr_menu_confirm_key=t.ivr_menu_confirm_key ;
	ivr_menu_confirm_attempts =t.ivr_menu_confirm_attempts; 
	ivr_menu_timeout=t.ivr_menu_timeout; 

	ivr_menu_exit_data=t.ivr_menu_exit_data ;
	ivr_menu_inter_digit_timeout =t.ivr_menu_inter_digit_timeout; 
	ivr_menu_max_failures =t.ivr_menu_max_failures; 
	ivr_menu_max_timeouts=t.ivr_menu_max_timeouts ; 
	ivr_menu_digit_len =t.ivr_menu_digit_len; 
	ivr_menu_direct_dial=t.ivr_menu_direct_dial; 

	ivr_menu_cid_prefix=t.ivr_menu_cid_prefix; 
	ivr_menu_description=t.ivr_menu_description; 
	ivr_menu_call_crycle_order =t.ivr_menu_call_crycle_order; 
	ivr_menu_enabled =t.ivr_menu_enabled; 
	ivr_menu_call_order_id =t.ivr_menu_call_order_id; return *this;
}

void base_config::clear()
{
	id=0 ;
	config_name="" ;
	config_param="" ;
	return;
}

base_config::base_config()
{
	clear();
}

base_config::base_config(const  base_config& t )
{
	id=t.id;
	config_name=t.config_name;
	config_param=t.config_param ;
}

base_config& base_config::operator=(const  base_config& t )
{
	id=t.id;
	config_name=t.config_name;
	config_param=t.config_param ;return *this;
}

void NwayCDR::clear()
{
	id =0 ;
	accountcode=""  ;
	xml_cdr ="" ;
	caller_id_name="" ;
	caller_id_number="" ;
	destination_number="" ;
	star_epoch=0 ;
	start_stamp="";
	a_answer_stamp =""; 
	a_answer_epoch=0 ; 

	a_end_epoch=0 ; 
	a_end_stamp="" ; 
	b_answer_stamp="" ; 
	b_answer_epoch =0; 

	b_end_epoch=0 ; 
	b_end_stamp="" ; 

	duration=0 ; 
	mduration=0 ; 
	billsec=0; 
	recording_file=""; 
	conference_name=""; 
	conference_id =0; 
	digites_dialed=""; 
	hangup_cause=""; 
	hangup_cause_id=0; 
	waitsec =0; 
	call_gateway_id=0; 
	hangup_direction=0; 
	called_number="";
}

NwayCDR::NwayCDR()
{
	clear();
}

NwayCDR::NwayCDR(const  NwayCDR& t )
{
	id =t.id ;
	accountcode=t.accountcode  ;
	xml_cdr =t.xml_cdr;
	caller_id_name=t.caller_id_name ;
	caller_id_number=t.caller_id_number ;
	destination_number=t.destination_number ;
	star_epoch=t.star_epoch ;
	start_stamp=t.start_stamp;
	a_answer_stamp =t.a_answer_stamp; 
	a_answer_epoch=t.a_answer_epoch; 

	a_end_epoch=t.a_end_epoch; 
	a_end_stamp=t.a_end_stamp ; 
	b_answer_stamp=t.b_answer_stamp; 
	b_answer_epoch =t.b_answer_epoch; 

	b_end_epoch=t.b_end_epoch ; 
	b_end_stamp=t.b_end_stamp ; 

	duration=t.duration ; 
	mduration=t.mduration; 
	billsec=t.billsec; 
	recording_file=t.recording_file; 
	conference_name=t.conference_name; 
	conference_id =t.conference_id; 
	digites_dialed=t.digites_dialed; 
	hangup_cause=t.hangup_cause; 
	hangup_cause_id=t.hangup_cause_id; 
	waitsec =t.waitsec; 
	call_gateway_id=t.call_gateway_id; 
	hangup_direction=t.hangup_direction; 
	called_number=t.called_number;
}

NwayCDR& NwayCDR::operator=(const  NwayCDR& t )
{
	id =t.id ;
	accountcode=t.accountcode  ;
	xml_cdr =t.xml_cdr;
	caller_id_name=t.caller_id_name ;
	caller_id_number=t.caller_id_number ;
	destination_number=t.destination_number ;
	star_epoch=t.star_epoch ;
	start_stamp=t.start_stamp;
	a_answer_stamp =t.a_answer_stamp; 
	a_answer_epoch=t.a_answer_epoch; 

	a_end_epoch=t.a_end_epoch; 
	a_end_stamp=t.a_end_stamp ; 
	b_answer_stamp=t.b_answer_stamp; 
	b_answer_epoch =t.b_answer_epoch; 

	b_end_epoch=t.b_end_epoch ; 
	b_end_stamp=t.b_end_stamp ; 

	duration=t.duration ; 
	mduration=t.mduration; 
	billsec=t.billsec; 
	recording_file=t.recording_file; 
	conference_name=t.conference_name; 
	conference_id =t.conference_id; 
	digites_dialed=t.digites_dialed; 
	hangup_cause=t.hangup_cause; 
	hangup_cause_id=t.hangup_cause_id; 
	waitsec =t.waitsec; 
	call_gateway_id=t.call_gateway_id; 
	hangup_direction=t.hangup_direction; 
	called_number=t.called_number;return *this;
}

void Outside_line::clear()
{
	id =0;
	outside_line_name=""; 
	outside_line_number=""; 
	inside_line_number="";
	is_record =false; 
	is_voice_mail=false; 
	call_order_id=0; 
	call_crycle_order=0;
}

Outside_line::Outside_line()
{
	clear();
}

Outside_line::Outside_line(const  Outside_line& t )
{
	id =t.id;
	outside_line_name=t.outside_line_name; 
	outside_line_number=t.outside_line_number; 
	inside_line_number=t.inside_line_number;
	is_record =t.is_record; 
	is_voice_mail=t.is_voice_mail; 
	call_order_id=t.call_order_id; 
	call_crycle_order=t.call_crycle_order;
}

Outside_line& Outside_line::operator=(const  Outside_line& t )
{
	id =t.id;
	outside_line_name=t.outside_line_name; 
	outside_line_number=t.outside_line_number; 
	inside_line_number=t.inside_line_number;
	is_record =t.is_record; 
	is_voice_mail=t.is_voice_mail; 
	call_order_id=t.call_order_id; 
	call_crycle_order=t.call_crycle_order;return *this;
}

void In_Out_Mapping::clear()
{
	outside_line_id=0;
	inside_line_id=0 ; 
	order_number=0; 
}

In_Out_Mapping::In_Out_Mapping()
{
	clear();

}

In_Out_Mapping::In_Out_Mapping( const In_Out_Mapping& t )
{
	outside_line_id=t.outside_line_id;
	inside_line_id=t.inside_line_id; 
	order_number=t.order_number; 
}

In_Out_Mapping& In_Out_Mapping::operator=(const  In_Out_Mapping& t )
{
	outside_line_id=t.outside_line_id;
	inside_line_id=t.inside_line_id; 
	order_number=t.order_number; return *this;
}
Call_info::Call_info()
{
	clear();
}
void Call_info::clear()
{
	id =0;

	callout_id=0;  
	call_direction = CALL_DIRECTION_NORMAL;
	waitsec = 0; 
	to_agent_group =""; 
	ring_path=""; 

	caller_uuid="";  
	caller_number="";
	called_number="";  
	lstBind.clear(); 
	trans_uuid="";  
	trans_number=""; 
	caller_state = CALLIN_STANDBY;  
	trans_state = CALLOUT_INIT;  
	caller_hangup = HANGUP_CAUSE_NOT_CALLED; 
	trans_hangup = HANGUP_CAUSE_NOT_CALLED;
	hangup_direction = HANGUP_DIRECTION_A_LEG;

	time(& caller_callin_tm);  
	time(&caller_answer_tm);  
	time(&caller_hangup_tm);  
	time(&trans_callout_tm) ;  
	time(&trans_answer_tm);   
	time(&trans_hangup_tm);  

	cdr_id = 0; 
	call_info_type = 0;
	bHangup = false;
}

Call_info::Call_info(const  Call_info& t )
{
	id =t.id;

	callout_id=t.callout_id;  
	call_direction = t.call_direction;
	waitsec =t.waitsec; 
	to_agent_group =t.to_agent_group; 
	ring_path=t.ring_path; 

	caller_uuid=t.caller_uuid;  
	caller_number=t.caller_number;
	called_number=t.called_number;  
	lstBind = t.lstBind; 
	trans_uuid=t.trans_uuid;  
	trans_number=t.trans_number; 
	caller_state = t.caller_state;  
	trans_state = t.trans_state;  
	caller_hangup = t.caller_hangup; 
	trans_hangup = t.trans_hangup;
	hangup_direction = t.hangup_direction;

	caller_callin_tm= t.caller_callin_tm;  
	caller_answer_tm =t.caller_answer_tm;  
	caller_hangup_tm=t.caller_hangup_tm;  
	trans_callout_tm =t.trans_callout_tm ;  
	trans_answer_tm = t.trans_answer_tm;   
	trans_hangup_tm = t.trans_hangup_tm;  
	call_info_type = t.call_info_type;
	cdr_id = t.cdr_id; 
	bHangup = t.bHangup;
}

Call_info& Call_info::operator=( const Call_info& t )
{
	id =t.id;

	callout_id=t.callout_id;  
	call_direction = t.call_direction;
	waitsec =t.waitsec; 
	to_agent_group =t.to_agent_group; 
	ring_path=t.ring_path; 

	caller_uuid=t.caller_uuid;  
	caller_number=t.caller_number;
	called_number=t.called_number;  
	lstBind = t.lstBind; 
	trans_uuid=t.trans_uuid;  
	trans_number=t.trans_number; 
	caller_state = t.caller_state;  
	trans_state = t.trans_state;  
	caller_hangup = t.caller_hangup; 
	trans_hangup = t.trans_hangup;
	hangup_direction = t.hangup_direction;

	caller_callin_tm= t.caller_callin_tm;  
	caller_answer_tm =t.caller_answer_tm;  
	caller_hangup_tm=t.caller_hangup_tm;  
	trans_callout_tm =t.trans_callout_tm ;  
	trans_answer_tm = t.trans_answer_tm;   
	trans_hangup_tm = t.trans_hangup_tm;  

	cdr_id = t.cdr_id;
	call_info_type = t.call_info_type;
	bHangup = t.bHangup;
	return *this;
}

void Callout_info::clear()
{
	id = 0;// NOT NULL,
	group_id = 0;// bigint,
	number = "";//号码
	is_called = 0 ;//integer DEFAULT 0, -- 是否呼叫过了
	call_state = 0;// integer DEFAULT 0, -- 呼叫状态

	hangup_reason_id = 0;// bigint,
	answer_extension_id = 0;// bigint,
	record_file = "";// character varying(255), -- 录音文件
	wait_sec = 0 ;//integer, -- 等待时长
	gateway_url = "";
	call_prefix = "";
	callout_state = CALLOUT_INIT;
	ring_times = 0;
	call_a_leg_uuid = "";
	call_b_leg_uuid ="";
	cdrid = 0;
	bAlegHangup = false;//
	bBlegHangup = false;
	bRemoveThis = true;
	extension_number = "";
}

Callout_info::Callout_info()
{
	clear();
}

Callout_info::Callout_info( const Callout_info& t )
{
	id = t.id;// NOT NULL,
	group_id = t.group_id;// bigint,
	number = t.number;//号码
	is_called = t.is_called ;//integer DEFAULT 0, -- 是否呼叫过了
	call_state = t.call_state;// integer DEFAULT 0, -- 呼叫状态

	hangup_reason_id = t.hangup_reason_id;// bigint,
	answer_extension_id = t.answer_extension_id;// bigint,
	record_file = t.record_file;// character varying(255), -- 录音文件
	wait_sec =t.wait_sec ;//integer, -- 等待时长
	call_prefix = t.call_prefix;
	gateway_url = t.gateway_url;
	callout_state = t.callout_state;
	ring_times = t.ring_times;
	call_a_leg_uuid = t.call_a_leg_uuid;
	call_b_leg_uuid = t.call_b_leg_uuid;
	cdrid = t.cdrid;
	bAlegHangup = t.bAlegHangup;
	bBlegHangup = t.bBlegHangup;
	extension_number = t.extension_number;
	bRemoveThis = t.bRemoveThis;
}

Callout_info& Callout_info::operator=( const Callout_info& t )
{
	id = t.id;// NOT NULL,
	group_id = t.group_id;// bigint,
	number = t.number;//号码
	is_called = t.is_called ;//integer DEFAULT 0, -- 是否呼叫过了
	call_state = t.call_state;// integer DEFAULT 0, -- 呼叫状态

	hangup_reason_id = t.hangup_reason_id;// bigint,
	answer_extension_id = t.answer_extension_id;// bigint,
	record_file = t.record_file;// character varying(255), -- 录音文件
	wait_sec =t.wait_sec ;//integer, -- 等待时长
	call_prefix = t.call_prefix;
	gateway_url = t.gateway_url;
	callout_state = t.callout_state;
	ring_times = t.ring_times;
	call_a_leg_uuid = t.call_a_leg_uuid;
	call_b_leg_uuid = t.call_b_leg_uuid;
	cdrid = t.cdrid;
	bAlegHangup = t.bAlegHangup;
	bBlegHangup = t.bBlegHangup;
	bRemoveThis = t.bRemoveThis;
	extension_number = t.extension_number;
	//////////////////////////////////////////////////////////////////////////
	return *this;
}

Call_Task::Call_Task()
{
	Clear();
}

Call_Task::Call_Task( const Call_Task& t )
{
	this->task_operate = t.task_operate;

	this->task_cmd = t.task_cmd;
	this->a_uuid = t.a_uuid;
	this->b_uuid = t.b_uuid;
	a_leg_answer = t.a_leg_answer;
}

void Call_Task::Clear()
{
	task_operate = CALL_OPERATION_SUBMENU; // so : originate , playback, bridge
	a_uuid ="";  //
	b_uuid = "";
	task_cmd = ""; //i
	a_leg_answer = false;
}

Call_Task& Call_Task::operator=( const Call_Task& t )
{
	this->task_operate = t.task_operate;

	this->task_cmd = t.task_cmd;
	this->a_uuid = t.a_uuid;
	this->b_uuid = t.b_uuid;
	a_leg_answer = t.a_leg_answer;
	return *this;
}

Callout_Task::Callout_Task()
{
	Clear();
}

Callout_Task::Callout_Task( const Callout_Task& t )
{
	id =t.id ;
	callout_name= t.callout_name;
	number_group_id = t.number_group_id; //呼叫号码组
	number_group_uploadfile= t.number_group_uploadfile;// character varying(255),
	run_position = t.run_position;//按order by后，运行的id位置
	time_rule_id= t.time_rule_id ;//时间计划，暂不用
	start_time = t.start_time; //开始时间
	stop_time = t.stop_time ;//结束时间
	ring_id = t.ring_id;//彩铃
	after_ring_play = t.after_ring_play ;// 彩铃后的操作，和call_after_opt对应，主要是继续彩铃，挂机，转座席
	ring_timeout = t.ring_timeout;// -- 振铃时长,当到时未接听则挂机
	group_id = t.group_id;// -- 呼叫的座席组
	call_project_id = t.call_project_id;
	concurr_type_id = t.concurr_type_id ;// -- 并发类型，0为按在线坐席数量的比例，1为指定值
	concurr_number = t.concurr_number;//  -- 并发倍数，按并发类型处理并发数
	callout_state_id = t.callout_state_id;//  等待呼叫，正在呼叫，暂停，完成
	total_number = t.total_number ;//总号码数量，不用在本应用中改
	wait_number = t.wait_number ;//integer DEFAULT 0, -- 等待数量,总量-接通-失败
	success_number = t.success_number;// integer DEFAULT 0, -- 接通数量, 接通+本应用中每次的接通的
	failed_number = t.failed_number ;//integer DEFAULT 0, -- 接通失败数量
	cancel_number = t.cancel_number ;//integer DEFAULT 0, -- 取消的数量,暂不用管
	has_parse_from_file = t.has_parse_from_file;// 当上传了文件后，是否从文件中解析了内容插到数据表中，解析结束后置为true
	callout_gateway_id = t.callout_gateway_id;// 外呼网关的id
	max_concurr_number = t.max_concurr_number;//    -- 最大并发数，前一个concurr_number为并发倍数
	second_ring_id = t.second_ring_id;//-- 由after_ring_play定为播放彩铃生效
	second_after_ring_opt  = t.second_after_ring_opt;//integer DEFAULT 0, -- 第二次再播放后的操作，和call_after_opt对应
	after_ring_key = t.after_ring_play;// -- 播放语音时按键中止播放
	after_key_opt_id = t.after_key_opt_id; 
	lstCalloutInfo = t.lstCalloutInfo;

	gateway_url = t.gateway_url;
	call_prefix = t.call_prefix;
	gateway_max_line = t.gateway_max_line;
	outside_line_number = t.outside_line_number;
	gateway_id = t.gateway_id;
}

void Callout_Task::Clear()
{
	id =0 ;
	callout_name="";
	number_group_id = 0; //呼叫号码组
	number_group_uploadfile="";// character varying(255),
	run_position = 0;//按order by后，运行的id位置
	time_rule_id= 0 ;//时间计划，暂不用
	start_time =""; //开始时间
	stop_time="" ;//结束时间
	ring_id = 0;//彩铃
	after_ring_play = 0 ;// 彩铃后的操作，和call_after_opt对应，主要是继续彩铃，挂机，转座席
	ring_timeout = 30;// -- 振铃时长,当到时未接听则挂机
	group_id = 0;// -- 呼叫的座席组
	call_project_id = 0;
	concurr_type_id = 0 ;// -- 并发类型，0为按在线坐席数量的比例，1为指定值
	concurr_number = 0;//  -- 并发倍数，按并发类型处理并发数
	callout_state_id =0;//  等待呼叫，正在呼叫，暂停，完成
	total_number = 0 ;//总号码数量，不用在本应用中改
	wait_number = 0 ;//integer DEFAULT 0, -- 等待数量,总量-接通-失败
	success_number = 0;// integer DEFAULT 0, -- 接通数量, 接通+本应用中每次的接通的
	failed_number = 0 ;//integer DEFAULT 0, -- 接通失败数量
	cancel_number = 0 ;//integer DEFAULT 0, -- 取消的数量,暂不用管
	has_parse_from_file = false;// 当上传了文件后，是否从文件中解析了内容插到数据表中，解析结束后置为true
	callout_gateway_id = 0;// 外呼网关的id
	max_concurr_number = 10;//    -- 最大并发数，前一个concurr_number为并发倍数
	second_ring_id =0;//-- 由after_ring_play定为播放彩铃生效
	second_after_ring_opt  = 0;//integer DEFAULT 0, -- 第二次再播放后的操作，和call_after_opt对应
	after_ring_key ="";// -- 播放语音时按键中止播放
	after_key_opt_id = 0; 
	gateway_url = "";
	call_prefix = "";
	gateway_max_line = 1;
	outside_line_number = "";
	gateway_id = 0;
}

Callout_Task& Callout_Task::operator=( const Callout_Task& t )
{
	id =t.id ;
	callout_name= t.callout_name;
	number_group_id = t.number_group_id; //呼叫号码组
	number_group_uploadfile= t.number_group_uploadfile;// character varying(255),
	run_position = t.run_position;//按order by后，运行的id位置
	time_rule_id= t.time_rule_id ;//时间计划，暂不用
	start_time = t.start_time; //开始时间
	stop_time = t.stop_time ;//结束时间
	ring_id = t.ring_id;//彩铃
	after_ring_play = t.after_ring_play ;// 彩铃后的操作，和call_after_opt对应，主要是继续彩铃，挂机，转座席
	ring_timeout = t.ring_timeout;// -- 振铃时长,当到时未接听则挂机
	group_id = t.group_id;// -- 呼叫的座席组
	call_project_id = t.call_project_id;
	concurr_type_id = t.concurr_type_id ;// -- 并发类型，0为按在线坐席数量的比例，1为指定值
	concurr_number = t.concurr_number;//  -- 并发倍数，按并发类型处理并发数
	callout_state_id = t.callout_state_id;//  等待呼叫，正在呼叫，暂停，完成
	total_number = t.total_number ;//总号码数量，不用在本应用中改
	wait_number = t.wait_number ;//integer DEFAULT 0, -- 等待数量,总量-接通-失败
	success_number = t.success_number;// integer DEFAULT 0, -- 接通数量, 接通+本应用中每次的接通的
	failed_number = t.failed_number ;//integer DEFAULT 0, -- 接通失败数量
	cancel_number = t.cancel_number ;//integer DEFAULT 0, -- 取消的数量,暂不用管
	has_parse_from_file = t.has_parse_from_file;// 当上传了文件后，是否从文件中解析了内容插到数据表中，解析结束后置为true
	callout_gateway_id = t.callout_gateway_id;// 外呼网关的id
	max_concurr_number = t.max_concurr_number;//    -- 最大并发数，前一个concurr_number为并发倍数
	second_ring_id = t.second_ring_id;//-- 由after_ring_play定为播放彩铃生效
	second_after_ring_opt  = t.second_after_ring_opt;//integer DEFAULT 0, -- 第二次再播放后的操作，和call_after_opt对应
	after_ring_key = t.after_ring_play;// -- 播放语音时按键中止播放
	after_key_opt_id = t.after_key_opt_id; 
	lstCalloutInfo = t.lstCalloutInfo;
	gateway_url = t.gateway_url;
	call_prefix = t.call_prefix;
	gateway_max_line = t.gateway_max_line;
	outside_line_number = t.outside_line_number;
	gateway_id = t.gateway_id;
	return *this;
}

NwayCalloutGateway::NwayCalloutGateway()
{
	Clear();
}

NwayCalloutGateway::NwayCalloutGateway( const NwayCalloutGateway& t )
{
	this->gateway_id=t.gateway_id;
	this->id = t.id;
	this->name = t.name;
}

NwayCalloutGateway::~NwayCalloutGateway()
{
	 

}

void NwayCalloutGateway::Clear()
{
	this->gateway_id= 0;
	this->id = 0;
	this->name = "";
}

NwayCalloutGateway& NwayCalloutGateway::operator=( const NwayCalloutGateway& t )
{
	this->gateway_id=t.gateway_id;
	this->id = t.id;
	this->name = t.name;
	return *this;
}

Click_Dial::Click_Dial()
{

}

Click_Dial::Click_Dial( const Click_Dial& t )
{
	this->account_number = t.account_number;
	this->caller_number = t.caller_number;
	this->id = t.id;
	this->is_agent = t.is_agent;
	this->is_called = t.is_called;
	this->is_immediately = t.is_immediately;
	this->time_plan = t.time_plan;
	this->trans_number = t.trans_number;
}

Click_Dial::~Click_Dial()
{

}

void Click_Dial::Clear()
{
	this->account_number = "";
	this->caller_number = "";
	this->id = 0;
	this->is_agent = false;
	this->is_called = false;
	this->is_immediately = false;
	this->time_plan =time(NULL);
	this->trans_number = "";
}

Click_Dial& Click_Dial::operator=( const Click_Dial& t )
{
	this->account_number = t.account_number;
	this->caller_number = t.caller_number;
	this->id = t.id;
	this->is_agent = t.is_agent;
	this->is_called = t.is_called;
	this->is_immediately = t.is_immediately;
	this->time_plan = t.time_plan;
	this->trans_number = t.trans_number;
	return *this;
}
