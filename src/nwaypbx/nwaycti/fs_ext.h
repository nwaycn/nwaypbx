#ifndef __FREESWITCH_EXT_NWAY_H
#define __FREESWITCH_EXT_NWAY_H
#include<stdio.h>
//#include <io.h>
#include <stdlib.h>
#include "../esl/esl.h"
#include <vector>
#include <string>
#include "../common/PcreCpp.h"
//char guuid[128];
//originate {return_ring_ready=true}sofia/gateway/liangping/18621575908 &bridge(user/8001)同时振铃
//单方挂机后转给另一方
//transfer_after_bridge 

// execute_after_bridge_app

using namespace std;
//#define disp_msg  printf
int check_event_body(const char*eventbody,char*dtmfbuf,int maxdtmf,char* endchar,int *channel_execute);


int check_acm_body(const char*eventbody,char*uuid);
int check_event(esl_handle_t *handle,int timer);

int check_last_event(esl_handle_t *handle,int timer);

int check_acm_event(esl_handle_t *handle,int timer,char* uuid);

//播音之后的取按键检查函数
//返回: <0 表示对方挂机，=100 表示按键最大时间结束条件满足 =101 表示两个按键间的间隔时间结束条件条件满足,=3表示 按键结束条件,比如"#" 满足，=2表示 最大按键个数结束条件 满足
//参数:handle:会话handle
//filename:语音文件名称，多个文件以分号";"隔开,文件名称可以带.wav,扩展名,假如没有,默认是.pcm扩展名.可指定路径,假如没有,默认是语音程序的./data/system目录下
//enddtmf:按键结束条件,比如"#"表示按#号结束输入,""表示没有结束按键条件//支持最大3个结束按键 比如 EndDtmf="*0#" 表示按 0，* 或者#都可以结束
//MaxDtmf:最大按键个数结束条件,0表示没有按键个数结束条件
//MaxTimer:按键最大时间结束条件,单位秒,0表示没有最大时间结束条件
//TwoDtmfTimer:两个按键间的间隔时间结束条件,单位秒,0表示没有两个按键间的间隔时间结束条件
//outdtmf:收到的用户的按键(输出参数),包括结束按键的条件,比如"#"
//其他说明:假如只有播音,不收取按键 设置：MaxDtmf=0
int check_play_dtmf_event(esl_handle_t *handle,char*enddtmf,int MaxDtmf,int MaxTimer,int TwoDtmfTimer,char*outdtmf);
//播音取按键
//返回: <0 表示对方挂机，=100 表示按键最大时间结束条件满足 =101 表示两个按键间的间隔时间结束条件条件满足,=3表示 按键结束条件,比如"#" 满足，=2表示 最大按键个数结束条件 满足
//参数:handle:会话handle
//filename:语音文件名称，多个文件以分号";"隔开,文件名称可以带.wav,扩展名,假如没有,默认是.pcm扩展名.可指定路径,假如没有,默认是语音程序的./data/system目录下
//EndDtmf:按键结束条件,比如"#"表示按#号结束输入,""表示没有结束按键条件//支持最大3个结束按键 比如 EndDtmf="*0#" 表示按 0，* 或者#都可以结束
//MaxDtmf:最大按键个数结束条件,0表示没有按键个数结束条件
//MaxTimer:按键最大时间结束条件,单位秒,0表示没有最大时间结束条件
//TwoDtmfTimer:两个按键间的间隔时间结束条件,单位秒,0表示没有两个按键间的间隔时间结束条件
//dtmf:收到的用户的按键(输出参数),包括结束按键的条件,比如"#"
//invalidfile:输入无效时的提示语音
//Maxfailure:最多尝试次数
//说明:假如只有播音,不收取按键 设置：MaxDtmf=0
#define  playdir  "d:/data/system"
int play_get_dtmf(esl_handle_t *handle,const char*filename0,const char* invalidfile, const char*EndDtmf,int MaxDtmf,int MaxTimer,int TwoDtmfTimer,int MaxFailure,char*outdtmf);
//录音之后的取按键检查函数
//返回: <0 表示对方挂机，=100 表示按键最大时间结束条件满足 =101 表示两个按键间的间隔时间结束条件条件满足,=3表示 按键结束条件,比如"#" 满足，=2表示 最大按键个数结束条件 满足
//参数:handle:会话handle
//filename:语音文件名称，多个文件以分号";"隔开,文件名称可以带.wav,扩展名,假如没有,默认是.pcm扩展名.可指定路径,假如没有,默认是语音程序的./data/system目录下
//enddtmf:按键结束条件,比如"#"表示按#号结束输入,""表示没有结束按键条件//支持最大3个结束按键 比如 EndDtmf="*0#" 表示按 0，* 或者#都可以结束
//MaxDtmf:最大按键个数结束条件,0表示没有按键个数结束条件
//MaxTimer:按键最大时间结束条件,单位秒,0表示没有最大时间结束条件
//TwoDtmfTimer:两个按键间的间隔时间结束条件,单位秒,0表示没有两个按键间的间隔时间结束条件
//outdtmf:收到的用户的按键(输出参数),包括结束按键的条件,比如"#"
//其他说明:假如只有播音,不收取按键 设置：MaxDtmf=0

int check_record_dtmf_event(esl_handle_t *handle,char*enddtmf,int MaxDtmf,int MaxTimer,int TwoDtmfTimer,char*outdtmf);
//录音取按键函数
//返回: <0 表示对方挂机，=100 表示按键最大时间结束条件满足 =101 表示两个按键间的间隔时间结束条件条件满足,=3表示 按键结束条件,比如"#" 满足，=2表示 最大按键个数结束条件 满足
//参数:handle:会话handle
//uuid： 会话的id
//filename:语音文件名称，多个文件以分号";"隔开,文件名称可以带.wav,扩展名,假如没有,默认是.pcm扩展名.可指定路径,假如没有,默认是语音程序的./data/system目录下
//EndDtmf:按键结束条件,比如"#"表示按#号结束输入,""表示没有结束按键条件//支持最大3个结束按键 比如 EndDtmf="*0#" 表示按 0，* 或者#都可以结束
//MaxDtmf:最大按键个数结束条件,0表示没有按键个数结束条件
//MaxTimer:按键最大时间结束条件,单位秒,0表示没有最大时间结束条件
//TwoDtmfTimer:两个按键间的间隔时间结束条件,单位秒,0表示没有两个按键间的间隔时间结束条件
//outdtmf:收到的用户的按键(输出参数),包括结束按键的条件,比如"#"
int record_get_dtmf(esl_handle_t *handle,char*uuid,char*filename,char*EndDtmf,int MaxDtmf,int MaxTimer,int TwoDtmfTimer,char*outdtmf);

void get_nway_dialstring(const char* srcstr,char* desstr, vector<MatchResult>& matchs);
void get_nway_dialstring_uuid(const char* srcstr,char* desstr, const char* auuid,const char* new_uuid,int a_leg_answer,
	vector<MatchResult>& matchs, const char* origination_number);
void get_nway_dialstring_uuid_ex(const char* srcstr,char* desstr, const char* auuid,const char* new_uuid,int a_leg_answer,
	vector<MatchResult>& matchs, const char* origination_number,string& extension_number);
bool nway_isdigit(const char* str);
bool get_uuid(esl_handle_t* pHandle,char* uuid);
bool get_uuid(esl_handle_t* pHandle,string& uuid);
bool nway_hangup(esl_handle_t* pHandle, const char* uuid);
//int get_uuid( char* uuid )
//{
//	uuid[0]=0;
//	CInboundHandle* ihInstance = CInboundHandle::GetInstance();//
//	esl_handle_t* ghandle = ihInstance->GetHandle();
//	esl_send_recv(ghandle,"api create_uuid\n\n");
//	if (*ghandle.last_sr_event && *ghandle.last_sr_event->body)
//	{
//
//		strcpy(uuid,*ghandle.last_sr_event->body);
//		return 0;
//	}
//	return 1;
//}
//
//
//	if(check_event(&handle,2)<0) goto END;
//	
//	if(play_get_dtmf(&handle,"hello.alaw","#",0,0,0,dtmf)<0) goto END;	//hello.alaw:"欢迎致电我公司，我们公司是转移提供400服务公司"	
//	while(1)
//	{
//		if(play_get_dtmf(&handle,"menu.alaw","*0#",5,20,0,dtmf)<0) goto END;//menu.alaw:"请输入分机号码，按#号确认，留言请按星号，人工服务请按0,参加会议请按#号"		
//		if(dtmf[0]!=0) 
//		{
//			break;
//		}
//	}
//	switch(dtmf[0])
//	{
//	case '*'://留言
//		{
//			if(play_get_dtmf(&handle,"startrecord.alaw","#",0,0,0,dtmf)<0)   goto END;	//startrecord.alaw:请在听到滴的一声之后开始录音，按任意键结束录音,滴.....
//			sprintf(recordfilename,"z:/%s.alaw",uuid);//录制到 z:/目录下 .alaw 扩展名 录制为 8K16bit 的alaw 格式语音，.wav扩展名录制为 8K16bit 的线性pcm格式语音
//			if(record_get_dtmf(&handle,uuid,recordfilename,"#",1,30,0,dtmf)<0) goto END;	
//			if(play_get_dtmf(&handle,recordfilename,"#",0,0,0,dtmf)<0)   goto END;	
//			if(play_get_dtmf(&handle,"bye.alaw","#",0,0,0,dtmf)<0)   goto END;	//bye.alaw:"谢谢使用，再见"
//			break;
//		}
//	case '0'://人工服务
//		{
//			esl_execute(&handle, "bridge", "user/1000@${domain_name}", NULL); 
//			
//			if(check_event(&handle,0)<0) goto END;
//			break;
//		}
//
//	case '#'://参加会议
//		{
//			esl_execute(&handle, "conference", "3000@default", NULL);	
//			if(check_event(&handle,0)<0) goto END;
//			break;
//		}
//
//	default: 
//		{
//			strtok(dtmf,"#");//去掉#号键
//			sprintf(cmd_tmp,"user/%s@${domain_name}",dtmf);////${domain_name}=192.168.1.236
//			esl_execute(&handle, "bridge",cmd_tmp , NULL);
//			if(check_event(&handle,0)<0) goto END;
//			break;
//		}
//	}
//	disp_msg("ivr hangup.....\n");
//	sprintf(cmd_tmp,"api uuid_kill %s",uuid);
//	esl_send_recv_timed(&handle, cmd_tmp,1000);
//
//END:	
//	disp_msg("check_last_event start.....\n");
//	check_last_event(&handle,10);
//	disp_msg("Disconnected!");
//	esl_disconnect(&handle);
//
#endif
