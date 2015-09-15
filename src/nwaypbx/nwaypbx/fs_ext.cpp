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

#include "fs_ext.h"
#ifdef WIN32
#include "../unistd.h"
#else
#include "unistd.h"
#endif


int check_event_body(const char * eventbody, char * dtmfbuf, int maxdtmf, char * endchar, int * channel_execute)
{
	char tmp[128];
	unsigned int i, len;

	if (eventbody == NULL) return 0;
	len = strlen(eventbody);
	if (len>64) len = 64;
	strncpy(tmp, eventbody + strlen("Event-Name: "), len);
	tmp[len] = 0;
	for (i = 0; i<strlen(tmp); i++) if (tmp[i] == '\n' || tmp[i] == '\r') { tmp[i] = 0; break; }

	//disp_msg("Event-Name:[%s]", tmp);
	if (strcmp(tmp, "CHANNEL_EXECUTE_COMPLETE") == 0)
	{
		if (*channel_execute) return 1;
		//disp_msg("CHANNEL_EXECUTE_COMPLETE invalid");
		return 0;
	}
	if (strcmp(tmp, "CHANNEL_EXECUTE") == 0) *channel_execute = 1;
	if (strcmp(tmp, "CHANNEL_HANGUP") == 0) return -98;
	//disp_msg("event_body:%s", eventbody);
	//if(strcmp(tmp,"CHANNEL_PROGRESS")==0)

	//if(_strnicmp(eventbody,"Event-Name: DTMF",strlen("Event-Name: DTMF"))==0)
	if (strncasecmp(eventbody, "Event-Name: DTMF", strlen("Event-Name: DTMF")) == 0)
	{
		char*p;

		if (p = (char *)strstr(eventbody, "DTMF-Digit: "))
		{
			strcpy(tmp, p + strlen("DTMF-Digit: "));
			for (i = 0; i<strlen(tmp); i++)if (tmp[i] == '\n' || tmp[i] == '\r') { tmp[i] = 0; break; }
			if (strcmp(tmp, "%23") == 0) strcpy(tmp, "#");
			//disp_msg("dtmf:[%s]", tmp);
			if (dtmfbuf && strlen(dtmfbuf)<64) strcat(dtmfbuf, tmp);
			if (strlen(dtmfbuf) >= maxdtmf) return 2;
			len = strlen(endchar);
			//支持最大3个结束按键
			if (len>0) if (tmp[0] == endchar[0]) return 3;
			if (len>1) if (tmp[0] == endchar[1]) return 3;
			if (len>2) if (tmp[0] == endchar[2]) return 3;
		}
	}
	
	return 0;
}

int check_acm_body(const char * eventbody, char * uuid)
{
	char tmp[4096];
	unsigned int i, len;
	char*p;
	if (eventbody == NULL) return 0;
	len = strlen(eventbody);
	if (len>64) len = 64;
	strncpy(tmp, eventbody + strlen("Event-Name: "), len);
	tmp[len] = 0;
	for (i = 0; i<strlen(tmp); i++) if (tmp[i] == '\n' || tmp[i] == '\r') { tmp[i] = 0; break; }

	//disp_msg("Event-Name:[%s]", tmp);

	//	//disp_msg("event_body:%s", eventbody);
	if (strcmp(tmp, "CHANNEL_PROGRESS") != 0)
	{
		return 0;
	}




	if (p = (char *)strstr(eventbody, "Unique-ID: "))
	{
		strcpy(tmp, p + strlen("Unique-ID: "));
		for (i = 0; i<strlen(tmp); i++)if (tmp[i] == '\n' || tmp[i] == '\r') { tmp[i] = 0; break; }

		//disp_msg("acm UUID:[%s]", tmp);
		strcpy(uuid, tmp);
	}

	return 1;
}

int check_event(esl_handle_t * handle, int timer)
{
	int done = 0, c = 0;
	esl_status_t status;
	time_t exp = 0;
	char dtmf[128];
	int res = 0;
	int  channel_execute = 0;
	dtmf[0] = 0;
	printf("%s,%d check_event\n", __FILE__, __LINE__);
	while ((status = esl_recv_timed(handle, 1000)) != ESL_FAIL)
	{
		c++;
		//disp_msg("Waiting 1 seconds events.\n");
		if (timer>0 && c >= timer) { res = 100; break; }

		if (status == ESL_SUCCESS)
		{
			const char *type = esl_event_get_header(handle->last_event, "content-type");

			if (type)
			{
				if (strcasecmp(type, "text/disconnect-notice") == 0)
				{
					const char *dispo = esl_event_get_header(handle->last_event, "content-disposition");
					//disp_msg("Got a disconnection notice dispostion: [%s]", dispo ? dispo : "");
					if (dispo && strcmp(dispo, "linger") == 0)
					{
						res = -99;
						break;
					}
				}
				if (strcasecmp(type, "text/event-plain") == 0)
				{
					const char *eventbody = esl_event_get_body(handle->last_event);
					if ((res = check_event_body(eventbody, dtmf, 1, "#", &channel_execute)))
					{
						//disp_msg("check_event_body res=%d.",res);
						if (res<0)	break;
					}
				}

			}
		}
	}
	//disp_msg("check_event res=%d",res);
	return res;
}

bool get_uuid(esl_handle_t * pHandle, char * uuid)
{
	esl_send_recv(pHandle, "api create_uuid\n\n");
	if (pHandle->last_sr_event && pHandle->last_sr_event->body)
	{
		//disp_msg("uuid:[%s]****************************************************\n", pHandle->last_sr_event->body);
		strcpy(uuid, pHandle->last_sr_event->body);
		return true;
		//strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
	}
	else
	{
		//disp_msg("[%s] last_sr_reply\n", pHandle->last_sr_reply);
		return false;
	}
	return false;
}

bool nway_hangup(esl_handle_t * pHandle, const char * uuid)
{
	char szCmd[2048] = { 0 };
	sprintf(szCmd, "bgapi uuid_kill %s\n\n", uuid);
	esl_send_recv(pHandle, szCmd);// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
								  //	uuid[0]=0;
	if (pHandle->last_sr_event && pHandle->last_sr_event->body)
	{
		printf("[%s]\n", pHandle->last_sr_event->body);
		//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
		return true;
	}
	else
	{
		printf("[%s] last_sr_reply\n", pHandle->last_sr_reply);
		return false;
	}
}

bool nway_isdigit(const char * str)
{
	int len = strlen(str);
	bool bResult = true;
	for (int i = 0; i< len; i++)
	{
		if (!isdigit(str[i]))
		{
			bResult = false;
			break;
		}
	}
	return bResult;
}

bool nway_bridge(esl_handle_t* pHandle, const char * a_uuid, const char * b_uuid)
{
	char szCmd[2048] = { 0 };
	sprintf(szCmd, "bgapi uuid_bridge %s %s\n\n", a_uuid, b_uuid);
	esl_send_recv(pHandle, szCmd);// 
								  //	uuid[0]=0;
	if (pHandle->last_sr_event && pHandle->last_sr_event->body)
	{
		printf("[%s]\n", pHandle->last_sr_event->body);
		//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
		return true;
	}
	else
	{
		printf("[%s] last_sr_reply\n", pHandle->last_sr_reply);
		return false;
	}
}

bool nway_playring(esl_handle_t * pHandle, const char * uuid, const char * ring_file)
{
	esl_execute(pHandle, "playback", ring_file, uuid);
}

void get_nway_dialstring(const char * srcstr, char * desstr, vector<MatchResult>& matchs)
{
	char beginstr[200] = { 0 };
	char endstr[200] = { 0 };
	char convertstr[30] = { 0 };
	const char* dpos = strchr(srcstr, '$');//
	const char* apos = strchr(srcstr, '@');//
	bool bDollar = false;
	bool bAt = false;
	int nBegin = 0;
	int nEnd = 0;
	int nConvert = 0;
	if (dpos && apos)
	{

		int len1 = dpos - srcstr;
		strncpy(beginstr, srcstr, len1);
		apos = strchr(dpos, '@'); //so: @{domain_name}
		if (apos)
		{
			strncpy(convertstr, dpos + 1, apos - dpos - 1);
		}
		else
		{
			 
			strncpy(convertstr, dpos + 1, 1);
		}
		if (nway_isdigit(convertstr))
		{
			 
			nConvert = atoi(convertstr);
			 
			if (matchs.size()>0)
			{
				sprintf(desstr, "%s%s%s\0", beginstr, matchs[0].value[nConvert].c_str(), apos);
			}
			else
				strncpy(desstr, srcstr, strlen(srcstr));
		}

		else
		{
			strncpy(desstr, srcstr, strlen(srcstr));

		}



	}
	else
	{
		if (dpos)
		{
			int len1 = dpos - srcstr;
			strncpy(beginstr, srcstr, len1);
			//strcpy(convertstr,dpos+1);
			strncpy(convertstr, dpos + 1, 1);
			if (nway_isdigit(convertstr))
			{

				nConvert = atoi(convertstr);
				 
				if (matchs.size()>0)
				{
					sprintf(desstr, "%s%s\0", beginstr, matchs[0].value[nConvert].c_str());
					 
				}
				else
					strncpy(desstr, srcstr, strlen(srcstr));
			}

			else
			{
				strncpy(desstr, srcstr, strlen(srcstr));

			}
		}
		else
			strncpy(desstr, srcstr, strlen(srcstr));
	}

	//printf(desstr);

}
}
