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
#ifndef __NWAY_CTI_STATE_H
#define __NWAY_CTI_STATE_H


//挂机理由
typedef enum NWAY_HANGUP_CAUSE{
	HANGUP_CAUSE_NORMAL=1, //普通挂机
	HANGUP_CAUSE_IVR_NORMAL,  //IVR普通挂机
	HANGUP_CAUSE_UNANSWER,   //无人应答
	HANGUP_CAUSE_IVR_UNANSWER,  //ivr无人应答
	HANGUP_CAUSE_NO_FEE,    //费用不足
	HANGUP_CAUSE_BUSY,      //对方忙
	HANGUP_CAUSE_VOICEMAIL,  //语音信箱
	HANGUP_CAUSE_UNKNOWN_NUMBER,//无此号码
	HANGUP_CAUSE_NOT_CALL,       //无法呼出
	HANGUP_CAUSE_NOT_CALLED      //无法接听
};
//挂机方向，即谁先挂机
enum NWAY_HANGUP_DIRECTION{
	HANGUP_DIRECTION_A_LEG = 0,    //A leg一般为主叫，挂机
	HANGUP_DIRECTION_B_LEG,    //B leg 一般为被叫，挂机
	HANGUP_DIRECTION_TRANFER   //用于b leg转接给c
};
//呼入转接坐席状态
enum NWAY_CALLIN_STATE{
	CALLIN_STANDBY = 0,    //空闲
	CALLIN_RINGING,        //振铃
	CALLIN_TALKING,			//通话中
	CALLIN_PENGING,			//挂断中
	CALLIN_IVRING,			//IVR导航 中
	CALLIN_VOICEMAIL,		//语音信箱
	CALLIN_BUSY,			//正忙
	CALLIN_WAIT				//等待
};
//呼出号码状态
enum NWAY_CALLOUT_STATE{
	CALLOUT_INIT = 0,		//初始化
	CALLOUT_OUT ,			//呼出
	CALLOUT_A_TALKING,      //a leg 接通
	CALLOUT_TALKING,		//通话中
	CALLOUT_SUCCESS,		//成功
	CALLOUT_FAILED,			//外呼失败
				//结束呼叫
	CALLOUT_PLAY_RING,      //播放彩铃
	CALLOUT_SECOND_PLAY_RING, //播放二次彩铃
	CALLOUT_TRANS_AGENT      //

};
enum NWAY_AGENT_REG_STATE{
	AGENT_REG_SUCCESS = 0,  //已注册
	AGENT_REG_FAILED        //未注册
};

enum NWAY_AGENT_LOGIN_STATE{
	AGENT_LOGIN_SUCCESS = 0,  //上线
	AGENT_LOGIN_LOGOUT,       //下线
	AGENT_LOGIN_BUSY,         //忙
	AGENT_LOGIN_LEAVED        //离开
};

enum NWAY_CALL_DIRECTION{
	CALL_DIRECTION_NORMAL = 0, //普通呼叫
	CALL_DIRECTION_CALLOUT 
};

enum NWAY_CALL_OPERATION{
	CALL_OPERATION_SET = 1, //设置
	CALL_OPERATION_ANSWER,
	CALL_OPERATION_DTMF,
	CALL_OPERATION_BRIDGE,
	CALL_OPERATION_PLAYRING,
	CALL_OPERATION_SLEEP,
	CALL_OPERATION_SUBMENU,
	CALL_OPERATION_HANGUP,
	CALL_OPERATION_AGENT_LOGIN,
	CALL_OPERATION_AGETN_LOGOUT,
	CALL_OPERATION_ORIGINATE
};
typedef enum NWAY_CALLOUT_CONCURR_TYPE{
	CONCURR_FORECAST_CALL = 1, //预测外呼
	CONCURR_PRECISION_CALL,
	CONCURR_RING_CALL

};

typedef enum NWAY_CALL_EVENT{
	CALL_EVENT_CALL_IN = 1, //呼入
	CALL_EVENT_A_LEG_CALL,
	CALL_EVENT_A_LEG_TALKING,
	CALL_EVENT_A_LEG_HANGUP,
	CALL_EVENT_B_LEG_TALKING,
	CALL_EVENT_B_LEG_HANGUP,
	CALL_EVENT_B_LEG_CALL
};

#endif