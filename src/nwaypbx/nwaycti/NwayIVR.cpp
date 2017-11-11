#include "NwayIVR.h"

int NwayIVRCall( esl_handle_t& handle,nway_uint64_t& cdrid, const char* str_extension,const char* aleg_uuid,  const char* basepath,
	list<NwayDialplan>& lstDialplans,list<NwayDialplanDetail>& lstDialplanDetails,list<NwayRing>& lstRings, 
	list<NwayExtension>& lstExtensions,
	vector<MatchResult>& matchs , list<NwayIVR>& lstIVRs, list<NwayIVRDetail>& lstIVRDetails, bool& hasAnswer,
	list<Call_Task>& lstCallTask, pthread_mutex_t& taskMutex , const char* origination_number, pthread_mutex_t& configMutex)
{
	int nStatus = 0;
	//进入后，获取IVR相关信息
	NwayIVR myIVR;
	bool bFound =false;
	char dtmf[128];
	//得到相应的ivr节点
	list<NwayIVR>::iterator itIVR = lstIVRs.begin();
	printf("%s, %d\tsearch IVR record\n",__FILE__,__LINE__);
	for ( ; itIVR != lstIVRs.end(); itIVR++)
	{
		myIVR = *itIVR;
		if (myIVR.ivr_menu_extension == str_extension)
		{
			//找到了ivr节点
			bFound = true;
			printf("%s, %d\tfound a IVR record\n",__FILE__,__LINE__);
			break;
		}
	}
	if (bFound)
	{
		//找到了ivr进行处理
		//必须先接通来电
		esl_execute(&handle, "answer", NULL, NULL);
		CCallDetailRecord cdr;
		cdr.A_AnswerCall(cdrid);
		printf("%s, %d\tanswer the call in\n",__FILE__,__LINE__);
		do
		{
			if(check_event(&handle,2)<0) break;
			//播放欢迎致电xx公司，按xx键转某
			string ringfullname;
			string invalidname;
			int nStatus = GetRing(myIVR.ivr_menu_greet_long_id, ringfullname, lstRings, basepath, configMutex);
			if (nStatus != 0) break;//没有语音彩铃，不能进行ivr故退出
			nStatus = GetRing(myIVR.ivr_menu_invalid_sound_id , invalidname, lstRings, basepath, configMutex);
			int i=1;
			printf("%s, %d\tivr ring:%s,invalid ring:%s\n",__FILE__,__LINE__,ringfullname.c_str(),invalidname.c_str());
			while (i++ < myIVR.ivr_menu_max_failures)
			{
			
				if(play_get_dtmf(&handle,ringfullname.c_str(),invalidname.c_str(), myIVR.ivr_menu_confirm_key.c_str(),myIVR.ivr_menu_digit_len,myIVR.ivr_menu_max_timeouts,myIVR.ivr_menu_timeout,myIVR.ivr_menu_max_failures, dtmf)<0) break;
				//获取到了dtmf
				//为了比较方便，dtmf存入string中
				//printf("play_get_dtmf\n");
				string strDtmf = dtmf;
				list<NwayIVRDetail> myIVRDetails;
				GetIVRDetails(myIVR.id, lstIVRDetails, myIVRDetails, configMutex);
				list<NwayIVRDetail>::iterator itDetail = myIVRDetails.begin();
				NwayIVRDetail myDetail;
				bFound = false;
				for (; itDetail != myIVRDetails.end(); itDetail++)
				{
					myDetail = *itDetail;
					if (myDetail.ivr_menu_option_digits == strDtmf)
					{
						bFound = true;
						//printf("found detail\n");
						break;
					}
				}
				if (bFound)
				{
					//找到具体的IVR明细，进行相应的处理
					switch(myDetail.ivr_menu_option_action_id)
					{
					case CALL_OPERATION_BRIDGE:
						{
							//需要判断$1,$0之类的符号所代表的被叫号码
							char dial_string[1024]={0};
							char new_uuid[128] = {0};
							get_uuid(&handle,new_uuid);
							string extension_number;
							//printf("")
							get_nway_dialstring_uuid_ex(myDetail.ivr_menu_option_param.c_str(),dial_string, aleg_uuid,new_uuid, 1,matchs,origination_number,extension_number);
							printf("%s,%d: dail_string:%s\n",__FILE__,__LINE__,dial_string);
							Call_Task ct;
							ct.task_operate = CALL_OPERATION_ORIGINATE;
							ct.task_cmd = dial_string;
							ct.a_uuid = aleg_uuid;
							ct.b_uuid = new_uuid;
							ct.a_leg_answer = true;
							//////////////////////////////////////////////////////////////////////////
							//here insert a list for call task ,and wait for process it in other thread
							pthread_mutex_lock(&taskMutex); 
							lstCallTask.push_back(ct);
							pthread_mutex_unlock(&taskMutex);
							//////////////////////////////////////////////////////////////////////////
							string shortRing;
							nStatus = GetRing(myIVR.ivr_menu_greet_short_id , shortRing, lstRings, basepath , configMutex);
							printf("%s,-+++++++++++++++++++++++++\n\n",shortRing.c_str());
							esl_execute(&handle,"playback",shortRing.c_str() , NULL);//播放转接短语音
							nway_uint64_t ext_id = 0;
							GetExtensionId( extension_number,ext_id, lstExtensions ,configMutex);
							string str_origination_number = origination_number;
							string str_extension_number = extension_number;
							string s_str_extension = str_extension;
							InsertCallEvent(cdrid,ext_id,str_origination_number, str_extension_number, s_str_extension  , CALL_EVENT_A_LEG_CALL);

						}
						break;
					default:
						//以后可扩展sub ivr,当前版本先不处理
						;
					}
					break; //退出while循环
				}
				else
				{
					//没找到，则播放出错的语音，并继续循环到顶部
					esl_execute(&handle,"playback",invalidname.c_str() , NULL);//播放无效语音
				}
			}
		}while(false);


	}
	return nStatus;
}
