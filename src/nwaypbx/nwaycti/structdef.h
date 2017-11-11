#ifndef NWAY_STRUCT_DEF_H
#define NWAY_STRUCT_DEF_H
//
#include <string>
#include <list>
#include <time.h>
#include "../common/NwayDef.h"
#include "statedef.h"
#include <pcre.h>
using namespace std;
class NwayRing
{
public:
	nway_uint64_t id;
	string filename;
	NwayRing();
	NwayRing(const NwayRing& t);
	NwayRing& operator=(const NwayRing& t);
};

class NwayExtensionGroup
{
public:
	nway_uint64_t id;
	string group_name;
	NwayExtensionGroup();
	NwayExtensionGroup(const NwayExtensionGroup& t);
	NwayExtensionGroup& operator=(const NwayExtensionGroup& t);
};
//分机


class NwayExtension 
{
public:
	nway_uint64_t id;
	string extension_name;
	string extension_number;
	string password;//电话修改座席密码时可用，暂不支持
	//int is_reg;
	//int login_state;
	int is_allow_callout;
	nway_uint64_t group_id;
	string callout_number;
	int extension_type;
	bool is_record;
	bool is_disable;
	nway_uint64_t callout_gateway;
	NWAY_AGENT_REG_STATE reg_state;
	NWAY_AGENT_LOGIN_STATE login_state;
	NWAY_CALLIN_STATE call_state;
	NWAY_CALLOUT_STATE callout_state;
	void clear();
	 
	NwayExtension();
	 
	NwayExtension& operator=(const NwayExtension& t) ;
	
	NwayExtension(const NwayExtension& t);

};
//外呼网关
class NwayCalloutGateway{
public:
	nway_uint64_t id;// integer NOT NULL,
	string	name ;//character varying(255) NOT NULL,
	nway_uint64_t	gateway_id; //bigint
	NwayCalloutGateway();
	~NwayCalloutGateway();
	void Clear();
	NwayCalloutGateway(const NwayCalloutGateway& t);
	NwayCalloutGateway& operator=(const NwayCalloutGateway& t);
};
//网关
class NwayGateway 
{
public:
	nway_uint64_t id;//bigserial;// NOT NULL,
	string	gateway_name ;//character varying(255), -- 网关名称
	string	gateway_url; // character varying(255),
	string	call_prefix;// character varying(50), -- 出局冠字
	int	max_call;// integer, 
	NwayGateway();
	NwayGateway(const NwayGateway& t);
	NwayGateway& operator=(const NwayGateway& t);
	void clear();
};

//拨号明细
class NwayDialplanDetail 
{
public:
	nway_uint64_t id;
	nway_uint64_t dialplan_id ;
	string 	dialplan_detail_tag ;
	int 	dialplan_detail_type_id ;
	string 	dialplan_detail_data ;
	string	dialplan_detail_inline ;
	nway_uint64_t	dialplan_detail_group_id;
	nway_uint64_t ring_id;
	int	dialplan_detail_order ;
	bool	dialplan_detail_break ;
	NwayDialplanDetail();
	void clear();
	NwayDialplanDetail(const NwayDialplanDetail& t);
	NwayDialplanDetail& operator=(const NwayDialplanDetail& t);
	bool operator<(const NwayDialplanDetail& t);
	

};
//拨号计划
class NwayDialplan
{
public:
	nway_uint64_t id;
	 
	string	dialplan_name ;
	string 	dialplan_context ;
	string	dialplan_number ;
	bool	dialplan_continue ;
	int	dialplan_order ;
	
	bool	dialplan_enabled ;
	//pcre *re;

	//const char *error;
	//int erroffset;
	NwayDialplan();
	void clear();
	NwayDialplan(const NwayDialplan& t);
	NwayDialplan& operator=(const NwayDialplan& t);
};
//IVR明细
class NwayIVRDetail 
{
public:
	nway_uint64_t id;
	nway_uint64_t ivr_menu_id ;  
	string	ivr_menu_option_digits ;
	int	ivr_menu_option_action_id;
	string	ivr_menu_option_param ;
	int	ivr_menu_option_order ;
	string	ivr_menu_option_description ;
	NwayIVRDetail();
	void clear();
	NwayIVRDetail(const NwayIVRDetail& t);
	NwayIVRDetail& operator=(const NwayIVRDetail& t);
	bool operator<(const NwayIVRDetail& t);

};
//IVR
class NwayIVR 
{
public:
	nway_uint64_t id;
	string ivr_menu_name ;
	string	ivr_menu_extension ;	 
	nway_uint64_t	ivr_menu_greet_long_id ;//bigint,
	nway_uint64_t	ivr_menu_greet_short_id;// bigint,
	nway_uint64_t	ivr_menu_invalid_sound_id;// bigint,
	nway_uint64_t	ivr_menu_exit_sound_id;// bigint,
	nway_uint64_t	ivr_menu_ringback_id;// bigint,
	int	ivr_menu_exit_app_id;// integer,
	string	ivr_menu_confirm_macro ;
	string	ivr_menu_confirm_key ;
	int	ivr_menu_confirm_attempts ;// -- 尝试次数
	int	ivr_menu_timeout ;// -- 超时秒数
	 
	string	ivr_menu_exit_data ;
	int	ivr_menu_inter_digit_timeout ;// -- 中间不按键时的超时时间
	int	ivr_menu_max_failures ;//integer, -- 输错ivr的最大次数
	int	ivr_menu_max_timeouts ;//integer, -- ivr最大超时次数
	int ivr_menu_digit_len ;//integer, -- 数字按键最大长度
	string	ivr_menu_direct_dial;// character varying(200),
	 
	string	ivr_menu_cid_prefix;// character varying(200),
	string	ivr_menu_description;// text, -- 说明
	int	ivr_menu_call_crycle_order;// integer, -- 针对属于循环呼叫的，实时记录当前呼叫的子节点的order
	bool	ivr_menu_enabled ;//boolean,
	nway_uint64_t	ivr_menu_call_order_id ;//bigint,
	NwayIVR();
	void clear();
	NwayIVR(const NwayIVR& t);
	NwayIVR& operator=(const NwayIVR& t);
};
//base_config
//基本配置
class base_config 
{
public:
	nway_uint64_t id ;
	string config_name ;
	string config_param ;
	base_config();
	void clear();
	base_config(const base_config& t);
	base_config& operator=(const base_config& t);
};
//CDR
class NwayCDR
{
public:
	nway_uint64_t id  ;
	string accountcode  ;
	string xml_cdr  ;
	string  caller_id_name ;
	string caller_id_number ;
	string destination_number ;
	int star_epoch ;//是从Epoch（1970年1月1日00:00:00 UTC）开始所经过的秒数，不考虑闰秒。
	string start_stamp;
	string a_answer_stamp ;//timestamp without time zone,
	int a_answer_epoch ;//numeric,是从Epoch（1970年1月1日00:00:00 UTC）开始所经过的秒数，不考虑闰秒。

	int a_end_epoch ;//numeric,
	string a_end_stamp ;//timestamp without time zone,
	string b_answer_stamp ;//timestamp without time zone,
	int b_answer_epoch ;//numeric,是从Epoch（1970年1月1日00:00:00 UTC）开始所经过的秒数，不考虑闰秒。

	int b_end_epoch ;//numeric,
	string b_end_stamp ;//timestamp without time zone,

	int duration ;//numeric,作为整个a的计时
	int mduration ;//numeric,作为整个b的计时
	int billsec;// numeric,
	string recording_file;// character varying(255),
	string conference_name;// character varying(50),
	nway_uint64_t conference_id ;//bigint,
	string digites_dialed;// character varying(50),
	string hangup_cause;// character varying(200),
	nway_uint64_t hangup_cause_id;// bigint,
	int waitsec ;//integer,
	nway_uint64_t call_gateway_id;// bigint,
	int hangup_direction;//挂机方向
	string called_number;// bigint, -- 不管呼入还是呼出的号码,如有个运营商给的0596-6147000的号码，用户是打它，然后再转给具体的号码
	NwayCDR();
	void clear();
	NwayCDR(const NwayCDR& t);
	NwayCDR& operator=(const NwayCDR& t);
};

//外线配置
class Outside_line 
{
public:
	nway_uint64_t id ;
	string outside_line_name;// character varying(50), -- 外线名称
	string	outside_line_number;// character varying(50), -- 外线号码
	string	inside_line_number;// character varying(50) DEFAULT NULL::character varying, -- 内线号码
	bool	is_record ;//integer DEFAULT 0, -- 是否录音，0不录，1录
	bool	is_voice_mail;// integer, -- 是否语音信箱，0不开通，1开通
	nway_uint64_t	call_order_id;// bigint DEFAULT 0, -- 呼叫顺序，外线可以直转内线号码，而不用配置多余的ivr
	nway_uint64_t	call_crycle_order;// bigint DEFAULT 0, -- 循环呼叫的当前呼叫到的值
	Outside_line();
	Outside_line(const Outside_line& t);
	void clear();
	Outside_line& operator=(const Outside_line& t);
};

//内外线对应
//类似绑定号码
//具体使用中，发现用这个类不是太好，暂不用
//2014-10-29
class In_Out_Mapping
{
public:
	nway_uint64_t outside_line_id;// bigint;// DEFAULT 0,
	nway_uint64_t	inside_line_id ;//bigint DEFAULT 0,
	nway_uint64_t	order_number;// integer DEFAULT 0, 
	In_Out_Mapping();
	void clear();
	In_Out_Mapping(const In_Out_Mapping& t);
	In_Out_Mapping& operator=(const In_Out_Mapping& t);
};
//呼入处理
class Call_info 
{
public:
	nway_uint64_t id;
	//以下变量为当direction为CALL_DIRECTION_CALLOUT时，随呼叫带过来的相应的参数，即精准呼叫是直接呼叫座席，非精准呼叫
	//是等待几秒后再呼座席
	nway_uint64_t callout_id; //如果是外呼的，则记录外呼的id，用于在呼出后将呼出状态修改
	NWAY_CALL_DIRECTION call_direction;
	int waitsec;//等待秒数
	string to_agent_group;//要转的座席组或号
	string ring_path;//彩铃
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	string caller_uuid; //认为是主叫（来电，外呼部分的客户号码）
	string caller_number;
	string called_number; //呼叫号码
	list<NwayExtension> lstBind;//要呼转出的号码全记在此处
	string trans_uuid; //转出uuid
	string trans_number;//转出号码
	NWAY_CALLIN_STATE caller_state; //呼入号码的状态
	NWAY_CALLOUT_STATE trans_state; //呼转的状态
	NWAY_HANGUP_CAUSE caller_hangup; 
	NWAY_HANGUP_CAUSE trans_hangup;
	NWAY_HANGUP_DIRECTION hangup_direction;
	int call_info_type; //0普通呼入 ，1 IVR 呼入，2 外呼
	bool bHangup; //bleg 先挂机

	time_t   caller_callin_tm; //呼入时间
	time_t   caller_answer_tm; //呼入接通
	time_t   caller_hangup_tm; //呼入挂机
	time_t   trans_callout_tm ; //呼出呼转号码时间
	time_t   trans_answer_tm;  //
	time_t   trans_hangup_tm; //

	nway_uint64_t cdr_id;//保存于cdr时的id,更新时需要cdr_ID
	Call_info();
	void clear();
	Call_info(const Call_info& t);
	Call_info& operator=(const Call_info& t);

};
//呼出信息

class Callout_info 
{
public:

	//////////////////////////////////////////////////////////////////////////
	nway_uint64_t id;// NOT NULL,
	nway_uint64_t group_id;// bigint,
	string	number;//号码
	int	is_called ;//integer DEFAULT 0, -- 是否呼叫过了
	int	call_state;// integer DEFAULT 0, -- 呼叫状态
	//	start_time timestamp without time zone,
	//	answer_time timestamp without time zone,
	//	hangup_time timestamp without time zone,
	nway_uint64_t	hangup_reason_id;// bigint,
	nway_uint64_t	answer_extension_id;// bigint,
	
	string	record_file;// character varying(255), -- 录音文件
	int	wait_sec ;//integer, -- 等待时长

	//////////////////////////////////////////////////////////////////////////
	string gateway_url;//gateway url
	string call_prefix;//呼叫前缀
	int ring_times;//彩铃播放次数，默认1
	string call_a_leg_uuid; //呼叫时aleg的uuid
	string call_b_leg_uuid; //呼叫时的bleg uuid

	nway_uint64_t cdrid;
	bool bAlegHangup; //主叫挂机
	bool bBlegHangup; //b leg hangup
	bool bRemoveThis;//在呼叫失败时是否删除这个呼叫

	string extension_number;
	//////////////////////////////////////////////////////////////////////////
	NWAY_CALLOUT_STATE callout_state;
	Callout_info();
	void clear();
	Callout_info(const Callout_info& t);
	Callout_info& operator=(const Callout_info& t);
};

//呼出任务
class Callout_Task{
public:
	nway_uint64_t id ;
	string	callout_name;
	nway_uint64_t number_group_id; //呼叫号码组
	string	number_group_uploadfile;// character varying(255),
	nway_uint64_t	run_position;//按order by后，运行的id位置
	nway_uint64_t	time_rule_id ;//时间计划，暂不用
	string	start_time; //开始时间
	string	stop_time ;//结束时间
	nway_uint64_t	ring_id;//彩铃
	int	after_ring_play ;// 彩铃后的操作，和call_after_opt对应，主要是继续彩铃，挂机，转座席
	int	ring_timeout;// -- 振铃时长,当到时未接听则挂机
	nway_uint64_t	group_id;// -- 呼叫的座席组
	nway_uint64_t	call_project_id;
	nway_uint64_t	concurr_type_id ;// -- 并发类型，0为按在线坐席数量的比例，1为指定值
	int	concurr_number;//  -- 并发倍数，按并发类型处理并发数
	nway_uint64_t	callout_state_id;//  等待呼叫，正在呼叫，暂停，完成
	int	total_number ;//总号码数量，不用在本应用中改
	int	wait_number ;//integer DEFAULT 0, -- 等待数量,总量-接通-失败
	int	success_number;// integer DEFAULT 0, -- 接通数量, 接通+本应用中每次的接通的
	int	failed_number ;//integer DEFAULT 0, -- 接通失败数量
	int	cancel_number ;//integer DEFAULT 0, -- 取消的数量,暂不用管
	bool	has_parse_from_file;// 当上传了文件后，是否从文件中解析了内容插到数据表中，解析结束后置为true
	nway_uint64_t	callout_gateway_id;// 外呼网关的id
	int	max_concurr_number;//    -- 最大并发数，前一个concurr_number为并发倍数
	nway_uint64_t	second_ring_id ;//-- 由after_ring_play定为播放彩铃生效
	int	second_after_ring_opt ;//integer DEFAULT 0, -- 第二次再播放后的操作，和call_after_opt对应
	string	after_ring_key ;// -- 播放语音时按键中止播放
	int	after_key_opt_id ;// -- 按键后的操作，和call_after_opt对应
	string outside_line_number;//外显号码
	//////////////////////////////////////////////////////////////////////////
	//不为数据库内字段，网关信息由数据库查询时获得
	nway_uint64_t gateway_id;//外呼网关对应的具体的真实网关的id
	string gateway_url;//gateway url
	string call_prefix;//呼叫前缀
	int gateway_max_line;//网关的最大呼叫能力
	//////////////////////////////////////////////////////////////////////////
	list<Callout_info> lstCalloutInfo;//针对本次任务的呼叫清单
	Callout_Task();
	void Clear();
	Callout_Task(const Callout_Task& t);
	Callout_Task& operator=(const Callout_Task& t);
};


typedef struct Call_Task 
{
public:
	NWAY_CALL_OPERATION task_operate; // so : originate , playback, bridge
	string a_uuid;  //
	string b_uuid;
	string task_cmd; //it is a command for running 
	bool a_leg_answer;//a leg is answered?

	Call_Task();
	void Clear();
	Call_Task(const Call_Task& t);
	Call_Task& operator=(const Call_Task& t);
};
typedef struct Click_Dial
{
	nway_uint64_t id ;//bigserial NOT NULL,
	string	caller_number;// character(50), -- 客户电话，如手机号，不管3721都加0
	bool	is_agent ;//boolean DEFAULT false, -- 是否属于voip内线
	bool	is_immediately;// boolean DEFAULT false, -- 是否立即执行
	string	trans_number;// character varying(50), -- 转接号码
	time_t	time_plan;// timestamp without time zone, -- 当is_immediately为FALSE时生效，定时呼叫
	string	account_number ;//character varying(50), -- 计费帐户
	bool	is_called;// boolean DEFAULT false, -- 是否呼叫
	Click_Dial();
	~Click_Dial();
	void Clear();
	Click_Dial(const Click_Dial& t);
	Click_Dial& operator=(const Click_Dial& t);
};
#endif