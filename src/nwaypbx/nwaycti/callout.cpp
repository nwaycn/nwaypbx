#include "callout.h"
#include "inbound_global.h"
#include "nwaycti.h"
//////////////////////////////////////////////////////////////////////////
//用于实现外呼
//////////////////////////////////////////////////////////////////////////
/*int CalloutWorkThreadRealTime(void* arg)
{
	CInboundHandle* ihInstance = CInboundHandle::GetInstance();
	char cmd_tmp[2048]={0};
	esl_handle_t* pHandle = ihInstance->GetHandle();
	esl_status_t res =esl_connect(pHandle,"127.0.0.1",8021,NULL,"CludeCon");
	if (res != 0)
	{
		printf("esl connect failed: res = %d\r\n",res);
		return 0;
	}
	else{
		printf("esl connect success\r\n");

	}
	if(strlen(called)>4)
		sprintf(cmd_tmp,"bgapi originate  \
	{origination_uuid=%s,originate_timeout=60}sofia/gateway/gw1/%s %s\n\n",uuid,called,system_caller);
	else
		sprintf(cmd_tmp,"bgapi originate   \
	{origination_uuid=%s,originate_timeout=30}sofia/internal/%s%c%s %s\n\n",uuid,called,'%',FreeSwitchserverip,system_caller);
	res=esl_send_recv(&handle,cmd_tmp);

	disp_msg("CallOutThread esl_send_recv:[%s],res=%d",cmd_tmp,res);
	if (handle.last_sr_event && handle.last_sr_event->body)
	{
		disp_msg("[%s],index=%d", handle.last_sr_event->body,index);
	}
	else
	{
		disp_msg("[%s],index=%d,last_sr_reply", handle.last_sr_reply,index);
	}
	disp_msg("CallOutThread uuid=[%s]",uuid);
	esl_filter(&handle, "unique-id", uuid);
	esl_events(&handle, ESL_EVENT_TYPE_PLAIN, "SESSION_HEARTBEAT CHANNEL_ANSWER CHANNEL_ORIGINATE  \
		CHANNEL_PROGRESS CHANNEL_HANGUP "   \
		"CHANNEL_BRIDGE CHANNEL_UNBRIDGE CHANNEL_OUTGOING CHANNEL_EXECUTE    \
		CHANNEL_EXECUTE_COMPLETE DTMF CUSTOMconference::maintenance");
	esl_send_recv(&handle, "linger");
}


int check_makecall_body(const char*eventbody,char*dtmfbuf,int maxdtmf,char* endchar,int *channel_execute)
{
	
		char tmp[128];
	unsigned int i,len;
	if (eventbody==NULL) return 0;
	len=strlen(eventbody);
	if(len>64) len=64;
	strncpy(tmp,eventbody+strlen("Event-Name: "),len);
	tmp[len]=0;
	for(i=0;i<strlen(tmp);i++) if(tmp[i]=='\n' || tmp[i]=='\r') {tmp[i]=0;break;}
	disp_msg("Event-Name:[%s]", tmp);
	if(strcmp(tmp,"CHANNEL_EXECUTE")==0) *channel_execute=1;
	if(strcmp(tmp,"CHANNEL_HANGUP")==0) return -98;
	if(strcmp(tmp,"CHANNEL_PROGRESS")==0) return EV_ALERTING;
	if(strcmp(tmp,"CHANNEL_ANSWER")==0) return EV_CONNECTED;
	return 0;
}
int check_makecall_event(esl_handle_t *handle,int timer)
{
	int done = 0,c=0;
	esl_status_t status;
	time_t exp = 0;
	char dtmf[128];
	int res=0;
	int channel_execute=0;
	dtmf[0]=0;
	time_t time1=time(NULL);
	while((status = esl_recv_timed(handle, 1000)) != ESL_FAIL)
	{
		c++;
		if(timer>0 && (time(NULL)-time1>=timer)){res=100;break;}
		if (status == ESL_SUCCESS)
		{
			const char *type = esl_event_get_header(handle->last_event, "content-type");
			if (type)
			{
				if(strcasecmp(type, "text/disconnect-notice")==0)
				{
					const char *dispo = esl_event_get_header(handle->last_event, "content-disposition");
					disp_msg("Got a disconnection notice dispostion: [%s]", dispo ? dispo : "");
					if (dispo && strcmp(dispo, "linger")==0)
					{
						res=-99;
						break;
					}
				}
				if(strcasecmp(type, "text/event-plain")==0)
				{
					const char *eventbody=esl_event_get_body(handle->last_event);
					if((res=check_makecall_body(eventbody,dtmf,1,"#",&channel_execute)))
					{
						disp_msg("check_makecall_body res=%d.",res);
						if(res) break;
					}
				}
			}
		}
	}
	disp_msg("check_makecall_event res=%d",res);
	return res;
}

*/
//////////////////////////////////////////////////////////////////////////
/*
在外联模式callback函数中，可以通过下面的办法来知道是用户拨入还是系统拨出：
int dir=1;//call in
if(esl_event_get_header(handle.info_event, "call-direction"))
{
disp_msg("dir:%s",esl_event_get_header(handle.info_event, "call-direction"));
if(strcmp(esl_event_get_header(handle.info_event,"call-direction"),"outbound")==0) dir=0;
}
*/
//////////////////////////////////////////////////////////////////////////
