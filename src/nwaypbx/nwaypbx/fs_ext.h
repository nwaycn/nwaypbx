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

#ifndef __NWAY_PBX_FREESWITCH_EXT__
#define __NWAY_PBX_FREESWITCH_EXT__
#include <esl.h>
#include <vector>
#include "../common/nway-lib/PcreCpp.h"

using namespace std;
//在这里有一部分代码是移自于厦门-余洪涌某一个公开的源代码文件中

int check_event_body(const char*eventbody, char*dtmfbuf, int maxdtmf, char* endchar, int *channel_execute);


int check_acm_body(const char*eventbody, char*uuid);
int check_event(esl_handle_t *handle, int timer);

int check_last_event(esl_handle_t *handle, int timer);

int check_acm_event(esl_handle_t *handle, int timer, char* uuid);

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
int check_play_dtmf_event(esl_handle_t *handle, char*enddtmf, int MaxDtmf, int MaxTimer, int TwoDtmfTimer, char*outdtmf);
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
//#define  playdir  "d:/data/system"
int play_get_dtmf(esl_handle_t *handle, const char*filename0, const char* invalidfile, const char*EndDtmf, int MaxDtmf, int MaxTimer, int TwoDtmfTimer, int MaxFailure, char*outdtmf);
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

int check_record_dtmf_event(esl_handle_t *handle, char*enddtmf, int MaxDtmf, int MaxTimer, int TwoDtmfTimer, char*outdtmf);
// 录 音取按键函数
// 返回: <0 表示对方挂机，=100 表示按键最大时间结束条件满足 =101 表示两个按键间的间隔时间结束条件条件满足,=3表示 按键结束条件,比如"#" 满足，=2表示 最大按键个数结束条件 满足
// 参数:handle:会话handle
// uuid： 会话的id
// filename:语音文件名称，多个文件以分号";"隔开,文件名称可以带.wav,扩展名,假如没有,默认是.pcm扩展名.可指定路径,假如没有,默认是语音程序的./data/system目录下
// EndDtmf:按键结束条件,比如"#"表示按#号结束输入,""表示没有结束按键条件//支持最大3个结束按键 比如 EndDtmf="*0#" 表示按 0，* 或者#都可以结束
// MaxDtmf:最大按键个数结束条件,0表示没有按键个数结束条件
// MaxTimer:按键最大时间结束条件,单位秒,0表示没有最大时间结束条件
// TwoDtmfTimer:两个按键间的间隔时间结束条件,单位秒,0表示没有两个按键间的间隔时间结束条件
// outdtmf:收到的用户的按键(输出参数),包括结束按键的条件,比如"#"
int record_get_dtmf(esl_handle_t *handle, char*uuid, char*filename, char*EndDtmf, int MaxDtmf, int MaxTimer, int TwoDtmfTimer, char*outdtmf);

bool get_uuid(esl_handle_t* pHandle, char* uuid);

bool nway_hangup(esl_handle_t* pHandle, const char* uuid);

bool nway_isdigit(const char* str);

bool nway_bridge(esl_handle_t* pHandle, const char* a_uuid, const char* b_uuid);

bool nway_playring(esl_handle_t* pHandle, const char* uuid, const char* ring_file);

void get_nway_dialstring(const char* srcstr, char* desstr, vector<MatchResult>& matchs);
#endif



