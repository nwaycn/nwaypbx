#include "process_event.h"
#include <stdio.h>
#include <cstdlib>
void process_event( esl_handle_t *handle, esl_event_t *event, pthread_mutex_t& infoMutex, pthread_mutex_t& calloutMutex 
	,list<Callout_Task>& lstCalloutTasks, list<NwayExtension>& lstExtension, list<Call_info>& lstCallinfo, list<NwayRing>& lstRings
	, string& BasePath ,pthread_mutex_t& configMutex)
{
	//const char *uuid;
	string caller_id;
	string destination_number;
	//const char *dtmf;
	string a_uuid;
	char tmp_cmd[1024]={0};
	string strUUID;
	strUUID  = esl_event_get_header(event, "Caller-Unique-ID")?esl_event_get_header(event, "Caller-Unique-ID"):"";
	caller_id = esl_event_get_header(event, "Caller-Caller-ID-Number")?esl_event_get_header(event, "Caller-Caller-ID-Number"):"";
	destination_number = esl_event_get_header(event, "Caller-Destination-Number")?esl_event_get_header(event, "Caller-Destination-Number"):"";
	string event_subclass,contact,from_user;

	//esl_log(ESL_LOG_INFO, "EVENT: %d\n", event->event_id);
	switch (event->event_id) {
	case ESL_EVENT_CUSTOM:
		{
			event_subclass = esl_event_get_header(event, "Event-Subclass") ? esl_event_get_header(event, "Event-Subclass"):"";
			contact = esl_event_get_header(event, "contact")?esl_event_get_header(event, "contact"):"";
			from_user = esl_event_get_header(event, "from-user")?esl_event_get_header(event, "from-user"):"";
			//printf("%s, %d event_subclass=%s, contact=%s, from-user=%s\n", __FILE__,__LINE__,event_subclass.c_str(),contact.c_str(), from_user.c_str());
			if (event_subclass == "sofia::register")
			{
				//注册消息，更新到数据库和当前的分机列表中
				SetAgentRegState(from_user.c_str(),lstExtension, AGENT_REG_SUCCESS ,configMutex);
			}
			else
				if (event_subclass == "sofia::unregister")
				{
					SetAgentRegState(from_user.c_str(),lstExtension, AGENT_REG_FAILED ,configMutex);
				}
		}
		break;
	case ESL_EVENT_DTMF:
		{
			string dtmf = esl_event_get_header(event, "DTMF-Digit")?esl_event_get_header(event, "DTMF-Digit"):"";
			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			strUUID = esl_event_get_header(event, "Caller-Unique-ID")?esl_event_get_header(event, "Caller-Unique-ID"):"";
			//a_uuid = esl_event_get_header(event, "variable_a_leg_uuid");
			destination_number = esl_event_get_header(event, "Caller-Destination-Number");
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			/*const char *eventbody=esl_event_get_body(event);
			printf("body:\n%s\n",eventbody);*/
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				a_leg_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):""; 
				if (trim(a_leg_uuid).length() >12)
				{
					//b leg 按键
				}
				else
				{
					//a leg 按键有效
					bool bFound = false;
					list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
					//printf("call out b leg ESL_EVENT_DTMF!-------------------\n");
					for (; itTask != lstCalloutTasks.end(); itTask++)
					{
						Callout_Task& nwayct = *itTask;
						list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
						for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
						{
							Callout_info& nwayci = *itInfo;
							if (nwayci.call_a_leg_uuid == strUUID)
							{
								//找到这次外呼的列表
								 
								bFound = true;
								SetDtmf(nwayci.cdrid, dtmf.c_str());
								if (nwayct.after_ring_key == dtmf)
								{
									switch(nwayct.after_key_opt_id)
									{
									case 2:
										{
											//挂机
											//esl_execute(handle,"hangup",NULL,NULL);
											nway_hangup(handle,strUUID.c_str());
										}
										break;
									case 3:
										{
											//转座席
											// bridge to agent
											char szCmd[2014] = {0};
											string strExtension;
											int nResult = GetIdleAgent(nwayct.group_id,strExtension,lstExtension ,configMutex);//得到空闲的座席
											if (nResult == 0)
											{
												//有空闲座席
												if (get_uuid(handle,nwayci.call_b_leg_uuid))
												{
													//获得一个uuid并呼出去
													sprintf(szCmd,"bgapi originate {origination_uuid=%s,a_leg_uuid=%s,origination_caller_id_number=%s,is_callout=1,"
														"originate_timeout=15,task_id=%lld}user/%s  &park()\n\n",nwayci.call_b_leg_uuid.c_str(),
														nwayci.call_a_leg_uuid.c_str(),nwayci.number.c_str(), nwayct.id, strExtension.c_str() );
													//printf(szCmd);
													esl_send_recv(handle,szCmd);
													// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
													//	uuid[0]=0;
													if (handle->last_sr_event && handle->last_sr_event->body) 
													{
														//printf("[%s]\n", handle->last_sr_event->body);
														//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
													} 
													else 
													{
														//printf("[%s] last_sr_reply\n", handle->last_sr_reply);
														LOGERREX(__FILE__,__LINE__,szCmd);
													}
													nwayci.callout_state = CALLOUT_TRANS_AGENT;//转给座席
													//记录b呼的事件
													nway_uint64_t ext_id = 0;
													GetExtensionId(nwayci.extension_number,ext_id, lstExtension,configMutex);
													InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_B_LEG_CALL);
													UpdateCallEvent(nwayci.cdrid,ext_id);
													//////////////////////////////////////////////////////////////////////////
													//*itInfo = nwayci;
												}
												else
													{
														//在获得了uuid后可能会发生变化
														char szCmd[2048] = {0};
														sprintf(szCmd,"bgapi uuid_kill %s\n\n",nwayci.call_a_leg_uuid.c_str());
														esl_send_recv(handle,szCmd);// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
														//	uuid[0]=0;
														if (handle->last_sr_event && handle->last_sr_event->body) 
														{
															printf("[%s]\n", handle->last_sr_event->body);
															//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
														} 
														else 
														{
															printf("[%s] last_sr_reply\n", handle->last_sr_reply);
														}
												}//esl_execute(handle,"hangup",NULL,NULL);//应用Kill
												
											}
											else
											{
												//没有空闲座席的处理，暂为挂机
												//esl_execute(handle,"hangup",NULL,NULL);
												nway_hangup(handle,strUUID.c_str());
											}
										}
										break;
									default:
										;
									}
								}
								else
									nway_hangup(handle,strUUID.c_str());
								//*itInfo = nwayci;
								break;
							}
						}
						if (bFound)
						{
							break;
						}
					}
				}
			}
			//esl_log(ESL_LOG_INFO,"dtmf :%s\n",dtmf);
			//printf("ESL_EVENT_DTMF:inbound dtmf :%s\n",dtmf);
			//printf("ESL_EVENT_DTMF:origination_uuid: %s\n", esl_event_get_header(event, "origination_uuid"));
			break;
		}
	case ESL_EVENT_CHANNEL_ORIGINATE:
		{
			//创建初始
			string is_callout,a_leg_uuid,a_leg_answered;
			strUUID  = esl_event_get_header(event, "Caller-Unique-ID")?esl_event_get_header(event, "Caller-Unique-ID"):"";
			//caller_id = esl_event_get_header(event, "Caller-Caller-ID-Number");
			destination_number = esl_event_get_header(event, "Caller-Destination-Number");
			
			//当有这个时，如果是外呼的，则开始记录cdr
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				LOGEX(__FILE__,__LINE__,"one call out number originated!");
				//置cdr和外呼列表中的相关内容为相应的对应值
				printf("%s, %d\tis call out from :%s  to:%s\n\n",__FILE__,__LINE__,caller_id.c_str(), destination_number.c_str());
				bool bFound = false;
				list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();

				for (; itTask != lstCalloutTasks.end(); itTask++)
				{
					Callout_Task& nwayct = *itTask;
					list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
					for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
					{
						Callout_info& nwayci = *itInfo;
						printf("%s, %d\tfound a info uuid:%s,\tthe uuid:%s\n",__FILE__,__LINE__,nwayci.call_a_leg_uuid.c_str(),strUUID.c_str());
						if (nwayci.call_a_leg_uuid == strUUID)
						{
							//找到这次外呼的列表
							printf("%s, %d\tstart a call out cdr\n",__FILE__,__LINE__);
							nwayci.callout_state = CALLOUT_OUT;
							bFound = true;
							CCallDetailRecord cdr;
							cdr.StartCall(destination_number.c_str(),destination_number.c_str(),caller_id.c_str(),nwayci.cdrid,true,nwayct.id);//开始一个呼叫记录
							CDBCalloutInfo cci;
							cci.StartCallout(nwayci.id, nwayci.cdrid);
							//*itInfo = nwayci;
							break;
						}
					}
					if (bFound)
					{
						break;
					}
				}

			}
			else
			{
				//把两条腿的记录下用于处理当未接通时，一端挂机，另一端无法挂机的问题
				a_leg_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):""; 
				if (a_leg_uuid.length() >12)
				{
					a_leg_answered = esl_event_get_header(event, "variable_a_leg_answer")?esl_event_get_header(event, "variable_a_leg_answer"):"";
					list<Call_info>::iterator infoit = lstCallinfo.begin();
					for (; infoit != lstCallinfo.end(); infoit++)
					{
						Call_info& ci = *infoit;
						//printf("%s,%d: the call info uuid:%s,uuid:%s \n\n",__FILE__,__LINE__,ci.caller_uuid.c_str(),uuid);
						//处理挂机相关事宜
						if (ci.caller_uuid == a_leg_uuid)
						{
							//a leg挂机
							//不管哪方先挂机肯定有a leg挂机
							ci.trans_uuid = strUUID;
							ci.caller_state = CALLIN_TALKING;
							//*infoit = ci;
							break;

						}


					}
				}
				
			}
			//printf("ESL_EVENT_CHANNEL_ORIGINATE:inbound originate :%s\n",destination_number);
			//printf("ESL_EVENT_CHANNEL_ORIGINATE:origination_uuid: %s\n", esl_event_get_header(event, "origination_uuid"));
			//esl_execute(handle, "hangup", NULL, uuid);
			break;
		}
	case ESL_EVENT_CHANNEL_BRIDGE:
		{
			

		
		}
			break;
	case ESL_EVENT_CHANNEL_PARK:
		{

			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//esl_log(ESL_LOG_INFO, "_PARK %s\n", uuid);
			//printf("ESL_EVENT_CHANNEL_PARK:inbound park :%s\n",uuid);
			break;
		}
	case ESL_EVENT_CHANNEL_EXECUTE_COMPLETE:
		{

			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//esl_log(ESL_LOG_INFO, "Hangup %s\n", uuid);
			//printf("ESL_EVENT_CHANNEL_EXECUTE_COMPLETE:inbound EXECUTE_COMPLETE :%s\n",uuid);
			
		}
		break;
	case ESL_EVENT_CHANNEL_HANGUP:
		{
			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//esl_log(ESL_LOG_INFO, "ESL_EVENT_CHANNEL_HANGUP %s\n", uuid);
			
			printf("%s,%d: ESL_EVENT_CHANNEL_HANGUP:inbound  :%s\n",__FILE__,__LINE__, strUUID.c_str());
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			string bridged_uuid;
			bridged_uuid = esl_event_get_header(event, "other-leg-unique-id")?esl_event_get_header(event, "other-leg-unique-id"):"";
			printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
			string hangup_cause;
			hangup_cause = esl_event_get_header(event, "variable_sip_term_cause")?esl_event_get_header(event, "variable_sip_term_cause"):"";
			esl_log(ESL_LOG_INFO,"hangup cause:%s\n", hangup_cause.c_str());
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				string taskid,precision;//一个是任务id,另一个是是否是精准呼叫
				taskid = esl_event_get_header(event, "variable_task_id")?esl_event_get_header(event, "variable_task_id"):"";
				precision = esl_event_get_header(event, "variable_precision")?esl_event_get_header(event, "variable_precision"):"";
				a_leg_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):""; 
				if (trim(a_leg_uuid).length() >12)
				{
					//b leg hangup
					printf("%s, %d \thuangup: %s \n",__FILE__,__LINE__,strUUID.c_str());
					bool bFound = false;
					list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();

					for (; itTask != lstCalloutTasks.end(); itTask++)
					{
						Callout_Task& nwayct = *itTask;
						list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
						for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
						{
							Callout_info& nwayci = *itInfo;
							if (nwayci.call_b_leg_uuid == strUUID)
							{
								bFound = true;
								CCallDetailRecord cdr;
								//cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_UNANSWER, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
								cdr.B_EndCall(nwayci.cdrid, nwayci.extension_number.c_str(), HANGUP_CAUSE_NORMAL);
								string str_route_number = "";
								InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,str_route_number, CALL_EVENT_B_LEG_HANGUP);
								LOGEX(__FILE__,__LINE__," b leg not answer and hangup!");
							}
							if (bFound)
							{
								if (nwayci.call_state == CALLOUT_A_TALKING)
								{
									//b拒接，a接

								}
								break;
							}
						}
					}
				}
				else
				{
					//a leg hangup
					if (taskid.length()>1 && precision == "1")
					{
						//精准呼叫的某一方挂机
						bool bFound = false;
						list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
						printf("%s, %d \thuangup: %s \n",__FILE__,__LINE__,strUUID.c_str());
						for (; itTask != lstCalloutTasks.end(); itTask++)
						{
							Callout_Task& nwayct = *itTask;
							list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
							pthread_mutex_lock(&calloutMutex);
							for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
							{
								Callout_info& nwayci = *itInfo;
								if (nwayci.call_a_leg_uuid == strUUID)
								{
									bFound = true;
									//A leg hangup
									nwayci.bAlegHangup = true;
									switch(nwayci.callout_state)
									{
									case CALLOUT_OUT:
									
										//a未接
										{
											nwayci.bRemoveThis = false;
											CCallDetailRecord cdr;
											
											cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_UNANSWER, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
											string str_route_number = "";

											InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,str_route_number, CALL_EVENT_A_LEG_HANGUP);
											LOGEX(__FILE__,__LINE__," A leg not answer and hangup!");
											esl_log(ESL_LOG_INFO,"A leg not answer and hangup\n");
											CDBCalloutInfo cci;
											cci.SetAlegHangup(nwayci.id,HANGUP_CAUSE_UNANSWER);
										}
										break;
									case CALLOUT_A_TALKING:
										//a通b未通
										{
											CCallDetailRecord cdr;
											cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_NORMAL, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
											string str_route_number = "";
											InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,str_route_number, CALL_EVENT_A_LEG_HANGUP);
											LOGEX(__FILE__,__LINE__," A leg  answered and hangup!");
											esl_log(ESL_LOG_INFO,"A leg  answered and hangup\n");
											CDBCalloutInfo cci;
											cci.SetAlegHangup(nwayci.id,HANGUP_CAUSE_NORMAL);
										}
										break;
									case CALLOUT_TALKING:
										//双通
										{
											//a b都接通了
											CDBCalloutInfo cci;
											cci.SetAlegHangup(nwayci.id,HANGUP_CAUSE_NORMAL);
											InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_A_LEG_HANGUP);
											if (!nwayci.bBlegHangup)
											{ 
												//b leg 还没挂机
												//nwayci.bAlegHangup = true;
												CCallDetailRecord cdr;
												cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_NORMAL, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
												LOGEX(__FILE__,__LINE__," A leg and B leg  answered and A hangup!");
												esl_log(ESL_LOG_INFO,"A leg and B leg  answered and A hangupp\n");
											}
											else
											{
												//nwayci.bAlegHangup = true;
												CCallDetailRecord cdr;
												cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_NORMAL, nwayct.gateway_id, HANGUP_DIRECTION_B_LEG);
												LOGEX(__FILE__,__LINE__," A leg and B leg  answered and B hangup!");
												esl_log(ESL_LOG_INFO,"A leg and B leg  answered and B hangup\n");

											}
										}
										break;
									default:
										;
										
									}
									break;
								}
								else
									if (nwayci.call_a_leg_uuid == bridged_uuid)
									{
										bFound = true;
										//B leg hangup
										CCallDetailRecord cdr;
										//cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_UNANSWER, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
										cdr.B_EndCall(nwayci.cdrid, nwayci.number.c_str(), HANGUP_CAUSE_NORMAL);
										string str_route_number = "";
										InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,str_route_number, CALL_EVENT_B_LEG_HANGUP);
										LOGEX(__FILE__,__LINE__," b leg  hangup!");
										esl_log(ESL_LOG_INFO,"b leg hangup\n");
										break;
									}
							}
							pthread_mutex_unlock(&calloutMutex);
							if (bFound)
							{
								break;
							}
						}
					}
					else
					{
						bool bFound = false;
						list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
						printf("%s, %d \thuangup: %s \n",__FILE__,__LINE__,strUUID.c_str());
						for (; itTask != lstCalloutTasks.end(); itTask++)
						{
							Callout_Task& nwayct = *itTask;
							list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
							for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
							{
								Callout_info& nwayci = *itInfo;
								if (nwayci.call_a_leg_uuid == strUUID)
								{
									//找到这次外呼的列表
									nwayci.bAlegHangup = true;
									nwayci.callout_state = CALLOUT_SUCCESS;
									if (nwayci.callout_state == CALLOUT_OUT)
									{
										//aleg 没有应答
										nwayct.failed_number ++;
										CCallDetailRecord cdr;
										cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_UNANSWER, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
										string str_route_number = "";
										InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,str_route_number, CALL_EVENT_A_LEG_HANGUP);
										LOGEX(__FILE__,__LINE__," A leg not answer and hangup!");
										CDBCalloutInfo cci;
										cci.SetAlegHangup(nwayci.id,HANGUP_CAUSE_UNANSWER);
									}
									else
									{
										//a leg 有应答
										if (nwayci.callout_state == CALLOUT_A_TALKING)
										{
											//只有a leg answer
											CCallDetailRecord cdr;
											cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_NORMAL, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
											string str_route_number = "";
											InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,str_route_number, CALL_EVENT_A_LEG_HANGUP);
											LOGEX(__FILE__,__LINE__," A leg  answered and hangup!");
											CDBCalloutInfo cci;
											cci.SetAlegHangup(nwayci.id,HANGUP_CAUSE_NORMAL);
										}
										else if (nwayci.callout_state == CALLOUT_TALKING)
										{
											//a b都接通了
											CDBCalloutInfo cci;
											cci.SetAlegHangup(nwayci.id,HANGUP_CAUSE_NORMAL);
											InsertCallEvent(nwayci.cdrid,nwayci.answer_extension_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_A_LEG_HANGUP);
											if (!nwayci.bBlegHangup)
											{ 
												//b leg 还没挂机
												nwayci.bAlegHangup = true;
												CCallDetailRecord cdr;
												cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_NORMAL, nwayct.gateway_id, HANGUP_DIRECTION_A_LEG);
												LOGEX(__FILE__,__LINE__," A leg and B leg  answered and A hangup!");
											}
											else
											{
												//nwayci.bAlegHangup = true;
												CCallDetailRecord cdr;
												cdr.A_EndCall(nwayci.cdrid, HANGUP_CAUSE_NORMAL, nwayct.gateway_id, HANGUP_DIRECTION_B_LEG);
												LOGEX(__FILE__,__LINE__," A leg and B leg  answered and B hangup!");

											}
										
										}
									
									
									
									
										//*itInfo = nwayci;
									}
									bFound = true;
									break;
								}
							}
							if (bFound)
							{
								break;
							}
						}
					
					}
					
				}
			}
			else
			{
				pthread_mutex_lock(&infoMutex);
				list<Call_info>::iterator infoit = lstCallinfo.begin();
				for (; infoit != lstCallinfo.end(); infoit++)
				{
					Call_info& ci = *infoit;
					//printf("%s,%d: the call info uuid:%s,uuid:%s \n\n",__FILE__,__LINE__,ci.caller_uuid.c_str(),uuid);
					//处理挂机相关事宜
					
					if (ci.caller_uuid == strUUID)
					{
						//a leg挂机
						//不管哪方先挂机肯定有a leg挂机
						printf("%s,%d: HANGUP:%s  --------------------\n",__FILE__,__LINE__,strUUID.c_str());
						nway_uint64_t ext_id = 0;
						GetExtensionId(ci.trans_number,ext_id, lstExtension , configMutex);
						InsertCallEvent(ci.cdr_id, ext_id,ci.caller_number,ci.trans_number,ci.called_number, CALL_EVENT_A_LEG_HANGUP);

						CCallDetailRecord cdr;
						SetAgentLoginStateBusyOrIdle(ci.caller_number.c_str(),lstExtension,0 ,configMutex);//数据库内置分机状态闲
						printf("%s,%d   call info type:%d   ****\n", __FILE__,__LINE__,ci.call_info_type);
						switch(ci.call_info_type)
						{
						case 0:
							{
								
								if (ci.trans_state == CALLOUT_TALKING )
								{
									//b leg接通的情况下
									//普通通话中的a
									if (ci.bHangup)
									{
										//表示 b已挂机
										cdr.A_EndCall(ci.cdr_id, HANGUP_CAUSE_NORMAL,0, HANGUP_DIRECTION_B_LEG);
										
									}
									else
									{
										cdr.A_EndCall(ci.cdr_id, HANGUP_CAUSE_NORMAL,0, HANGUP_DIRECTION_A_LEG);
									}
								}
								else
									cdr.A_EndCall(ci.cdr_id, HANGUP_CAUSE_UNANSWER,0, HANGUP_DIRECTION_B_LEG);
								
							}
							break;
						case 1:
							{
								printf("%s,%d    *ivr hangup :  %s  ****\n",__FILE__,__LINE__,strUUID.c_str());
								if (ci.trans_state == CALLOUT_TALKING  )
								{
									//b leg接通的情况下
									//普通通话中的a
									if (ci.bHangup)
									{
										//表示 b已挂机
										cdr.A_EndCall(ci.cdr_id, HANGUP_CAUSE_IVR_NORMAL,0, HANGUP_DIRECTION_B_LEG);
									}
									else
									{
										cdr.A_EndCall(ci.cdr_id, HANGUP_CAUSE_IVR_NORMAL,0, HANGUP_DIRECTION_A_LEG);
									}
								}
								else
									cdr.A_EndCall(ci.cdr_id, HANGUP_CAUSE_IVR_UNANSWER,0, HANGUP_DIRECTION_A_LEG);
							}
							break;
						default:
							;

						}

						break;

					}
					else
						if (ci.trans_uuid == strUUID)
						{
							//b leg hangup
							SetAgentLoginStateBusyOrIdle(ci.trans_number.c_str(),lstExtension,0,configMutex);//数据库内置分机状态闲
							nway_uint64_t ext_id = 0;
							GetExtensionId(ci.trans_number,ext_id, lstExtension,configMutex);
							InsertCallEvent(ci.cdr_id, ext_id,ci.caller_number,ci.trans_number,ci.called_number, CALL_EVENT_B_LEG_HANGUP);
							if (ci.trans_state == CALLOUT_TALKING)
							{
								//接通挂机
								CCallDetailRecord cdr;
								printf("%s,%d B-Leg Hangup:%s\n\n" ,__FILE__,__LINE__,strUUID.c_str());
								cdr.B_EndCall(ci.cdr_id,ci.trans_number.c_str(), HANGUP_CAUSE_NORMAL);


							}
							else
							{
								nway_hangup(handle,ci.caller_uuid.c_str());
							}

							//ci.trans_number = "";
							ci.bHangup = true;
							//回写给列表，用于当a leg挂机时标明b已挂机
							//*infoit = ci;
							break;
						}
				}
				pthread_mutex_unlock(&infoMutex);
			}
			
		}

		// printf("ESL_EVENT_CHANNEL_HANGUP:origination_uuid: %s\n", esl_event_get_header(event, "origination_uuid"));
		break;
	case ESL_EVENT_CHANNEL_HANGUP_COMPLETE:
		{
			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//esl_log(ESL_LOG_INFO, "HANGUP_COMPLETE %s\n", uuid);
			strUUID = esl_event_get_header(event, "Caller-Unique-ID")?esl_event_get_header(event, "Caller-Unique-ID"):"";
			printf("%s, %d\tinbound HANGUP_COMPLETE :%s\n",__FILE__,__LINE__,strUUID.c_str());
			//做话长统计并删除相应的呼叫信息
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			string bridged_uuid;
			bridged_uuid = esl_event_get_header(event, "other-leg-unique-id")?esl_event_get_header(event, "other-leg-unique-id"):"";
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				printf("%s, %d\tcallout hangup:%s\n",__FILE__,__LINE__,strUUID.c_str());
				string taskid,precision;//一个是任务id,另一个是是否是精准呼叫
				taskid = esl_event_get_header(event, "variable_task_id")?esl_event_get_header(event, "variable_task_id"):"";
				precision = esl_event_get_header(event, "variable_precision")?esl_event_get_header(event, "variable_precision"):"";
				a_leg_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):""; 
				if (trim(a_leg_uuid).length() >12)
				{
					//b leg hangup complete
					bool bFound = false;
					list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
					pthread_mutex_lock(&calloutMutex);
					printf("%s, %d\t list the task \n",__FILE__,__LINE__);
					for (; itTask != lstCalloutTasks.end(); itTask++)
					{
						Callout_Task& nwayct = *itTask;
						list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
						for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
						{
							Callout_info& nwayci = *itInfo;
							printf("%s, %d\tb leg uuid:%s\n",__FILE__,__LINE__,nwayci.call_b_leg_uuid.c_str());
							if (nwayci.call_b_leg_uuid == strUUID)
							{
								//当Aleg 已挂机时
								if (nwayci.bAlegHangup)
								{
									CCallDetailRecord cdr;
									cdr.CountTime(nwayci.cdrid);
									
									printf("%s, %d \there removed the info number:%s \n",__FILE__,__LINE__,nwayci.number.c_str());
									//nwayct.lstCalloutInfo.erase(itInfo2);//移除当前的任务信息

								}
								//SetIdleAgent(nwayct.group_id,nwayci.extension_number,lstExtension);
								nwayci.bBlegHangup = true;
								printf("%s, %d\t set agent idle\n", __FILE__,__LINE__);
								//nwayci.
								bFound = true;
								break;
							}
						}
						if (bFound )
						{
							break;
						}
					}
					pthread_mutex_unlock(&calloutMutex);
				}
				else
				{
					if (taskid.length()>1 && precision == "1")
					{
						//精准呼叫的某一方挂机
						bool bFound = false;
						list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
						printf("%s, %d \thuangup: %s \n",__FILE__,__LINE__,strUUID.c_str());
						for (; itTask != lstCalloutTasks.end(); itTask++)
						{
							Callout_Task& nwayct = *itTask;
							list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
							pthread_mutex_lock(&calloutMutex);
							for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
							{
								Callout_info& nwayci = *itInfo;
								if (nwayci.call_a_leg_uuid == strUUID)
								{
									//不管其它怎么变换，分机挂机一定置闲

									
									CCallDetailRecord cdr;
									cdr.CountTime(nwayci.cdrid);
									nwayci.bAlegHangup = true;
									printf("%s, %d \there removed the info number:%s \n",__FILE__,__LINE__,nwayci.number.c_str());
									nwayci.callout_state = CALLOUT_SUCCESS;
									//if (nwayci.bRemoveThis)
									//{
									//	printf("%s, %d\t set agent idle\n", __FILE__,__LINE__);
									//	nwayci.bBlegHangup = true;
									//	//nwayci.callout_state = CALLOUT_INIT;
									//}
								    //SetIdleAgent(nwayct.group_id,nwayci.extension_number,lstExtension);
									bFound = true;
									break;
								}
								else
									if (nwayci.call_a_leg_uuid == bridged_uuid)
									{
										//B leg hangup
										//CCallDetailRecord cdr;
										//cdr.CountTime(nwayci.cdrid);
										//nwayci.callout_state = CALLOUT_SUCCESS;
										nwayci.bBlegHangup = true;
										bFound = true;
										break;
									}
							}
							pthread_mutex_unlock(&calloutMutex);
							if (bFound)
							{
								break;
							}

						}
					}
					else
					{
						bool bFound = false;
						list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
						pthread_mutex_lock(&calloutMutex);

						for (; itTask != lstCalloutTasks.end(); itTask++)
						{
							Callout_Task& nwayct = *itTask;
							list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
							for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
							{
								Callout_info& nwayci = *itInfo;
								if (nwayci.call_a_leg_uuid == strUUID)
								{
									//当Bleg 已挂机时
									/*if ((nwayci.bBlegHangup && nwayci.callout_state == CALLOUT_TALKING) || (nwayci.bAlegHangup && nwayci.callout_state == CALLOUT_A_TALKING) || nwayci.callout_state == CALLOUT_OUT || nwayci.callout_state == CALLOUT_PLAY_RING)
									{*/
										CCallDetailRecord cdr;
										cdr.CountTime(nwayci.cdrid);
										//list<Callout_info>::iterator itInfo2 = itInfo;
										string snumber ;
										snumber = nwayci.number;
										if (snumber.length() > 0)
										{
											printf("%s, %d\there removed the info number:%s \n",__FILE__,__LINE__,snumber.c_str());
										}
										else
											printf("%s, %d\tno a info number  \n",__FILE__,__LINE__);
										//itInfo++;
									
										nwayci.bAlegHangup = true;
										//nwayct.lstCalloutInfo.erase(itInfo2);//移除当前的任务信息
									
									/*}
									else
										nwayci.bAlegHangup = true;*/
									bFound = true;
									break;
								}
							}
							if (bFound )
							{
								break;
							}
						}
						pthread_mutex_unlock(&calloutMutex);
					}
				}
			}
			else
			{
				printf("%s,%d-----------------------------------------------\n",__FILE__,__LINE__);
				//string bridged_uuid;
				//bridged_uuid = esl_event_get_header(event, "variable_bridged_uuid")?esl_event_get_header(event, "variable_bridged_uuid"):"";
				//printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
				pthread_mutex_lock(&infoMutex);
				list<Call_info>::iterator infoit = lstCallinfo.begin();
				for (; infoit != lstCallinfo.end(); infoit++)
				{
					Call_info& ci = *infoit;
					if (ci.caller_uuid == strUUID && ci.caller_state == CALLIN_RINGING)
					{
						//uuid_kill
						if (ci.trans_uuid.length() >2)
						{
							char szCmd[2048] = {0};
							sprintf(szCmd,"bgapi uuid_kill %s\n\n",ci.trans_uuid.c_str());
							esl_send_recv(handle,szCmd);// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
							//	uuid[0]=0;
							if (handle->last_sr_event && handle->last_sr_event->body) 
							{
								printf("[%s]\n", handle->last_sr_event->body);
								//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
							} 
							else 
							{
								printf("[%s] last_sr_reply\n", handle->last_sr_reply);
							}
						}
						CCallDetailRecord cdr;
						cdr.CountTime(ci.cdr_id);
						list<Call_info>::iterator infoit2 = infoit;
						infoit++;
						lstCallinfo.erase(infoit2);
						printf("%s, %d\tthe lstcallinfo size:%d \n",__FILE__,__LINE__,lstCallinfo.size());
						break;

					}
					if (ci.caller_uuid == strUUID && ci.bHangup)
					{
						//找到 a leg的uuid后进处相应的处理,b已挂机
						CCallDetailRecord cdr;
						cdr.CountTime(ci.cdr_id);
						//lstCallinfo.remove(ci);
						
						list<Call_info>::iterator infoit2 = infoit;
						infoit++;
						lstCallinfo.erase(infoit2);
						printf("%s, %d\tthe lstcallinfo size:%d &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n",__FILE__,__LINE__,lstCallinfo.size());
						break;
					}
					else
					{
						//a先挂，然后b挂时，由b完全挂机后删除
						if (ci.trans_uuid == strUUID)
						{
							CCallDetailRecord cdr;
							cdr.CountTime(ci.cdr_id);
							//lstCallinfo.remove(ci);
							list<Call_info>::iterator infoit2 = infoit;
							infoit++;
							lstCallinfo.erase(infoit2);
							printf("%s, %d\tthe lstcallinfo size:%d  \n",__FILE__,__LINE__,lstCallinfo.size());
							break;
						}
						else 
						{
							if (ci.caller_uuid == strUUID)
							{ 
									if (ci.trans_state != CALLOUT_TALKING || ci.caller_state == CALLIN_RINGING)
									{
										//uuid_kill
										if (ci.trans_uuid.length() >2)
										{
											char szCmd[2048] = {0};
											sprintf(szCmd,"bgapi uuid_kill %s\n\n",ci.trans_uuid.c_str());
											esl_send_recv(handle,szCmd);// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
											//	uuid[0]=0;
											if (handle->last_sr_event && handle->last_sr_event->body) 
											{
												printf("[%s]\n", handle->last_sr_event->body);
												//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
											} 
											else 
											{
												printf("[%s] last_sr_reply\n", handle->last_sr_reply);
											}
										}

									}
								else
									printf("%s,%d ------------------------------------------------------------\n",__FILE__,__LINE__);
						}
					
						else
						{
							printf("%s, %d\tnot found:%s,   %s,%s------\n",__FILE__,__LINE__,ci.caller_uuid.c_str(),ci.trans_uuid.c_str(),strUUID.c_str());
							printf("%s, %d\t%d,trans state:%d \n",__FILE__,__LINE__,ci.caller_state,ci.trans_state);

						}
					}
				}
				}
				pthread_mutex_unlock(&infoMutex);
			}
		}

		// printf("ESL_EVENT_CHANNEL_HANGUP_COMPLETE:origination_uuid: %s\n", esl_event_get_header(event, "origination_uuid"));
		break;
	case ESL_EVENT_CHANNEL_ANSWER:
		{

			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//strUUID = uuid;
			//nway_uint64_t testid = atoi64("4857578448");
			//printf("%s, %d\tatoi64=%lld\n\n\n",__FILE__,__LINE__,testid);
			

			time_t long_time,newtime;
			time( &newtime );//Get time as long integer. 
			long_time = convert_string_to_time_t("2015/06/30 20:00:00");
			if (newtime > long_time)
			{
				return ;
			}

			a_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):"";
			destination_number = esl_event_get_header(event, "Caller-Destination-Number");
			//caller_id = esl_event_get_header(event, "Caller-Caller-ID-Number");
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			//const char *eventbody=esl_event_get_body(event);
			//printf("body:\n%s\n",eventbody);
			string bridged_uuid;
			
			bridged_uuid = esl_event_get_header(event, "other-leg-unique-id")?esl_event_get_header(event, "other-leg-unique-id"):"";
			printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				a_leg_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):""; 
				printf("%s, %d\tESL_EVENT_CHANNEL_ANSWER call out uuid:%s \n", __FILE__, __LINE__, strUUID.c_str());
				if (trim(a_leg_uuid).length() >12)
				{
					//外呼的b-leg answer
					
					//
					bool bFound = false;
					list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
					printf("%s, %d\tESL_EVENT_CHANNEL_ANSWER call out b leg answered!-------------------\n",__FILE__, __LINE__);
					for (; itTask != lstCalloutTasks.end(); itTask++)
					{
						Callout_Task& nwayct = *itTask;
						list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
						printf("%s, %d\tESL_EVENT_CHANNEL_ANSWER task id;%lld ------\n",__FILE__,__LINE__, nwayct.id);
						for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
						{
							Callout_info& nwayci = *itInfo;
							printf("%s, %d \t ESL_EVENT_CHANNEL_ANSWER info id:%lld ,a uuid:%s------\n", __FILE__, __LINE__, nwayci.id,nwayci.call_a_leg_uuid.c_str());
							if (nwayci.call_a_leg_uuid == a_leg_uuid)
							{
								//找到这次外呼的列表
								nwayci.callout_state = CALLOUT_TALKING;
								nwayci.call_b_leg_uuid = strUUID;//bleg 的uuid确认了
								bFound = true;
								CCallDetailRecord cdr;
								//cdr.StartCall(destination_number,destination_number,caller_id,nwayci.cdrid,true);//开始一个呼叫记录
								cdr.B_AnswerCall(nwayci.cdrid,destination_number.c_str(),"");
								//记录b通的事件
								nway_uint64_t ext_id = 0;
								GetExtensionId(nwayci.extension_number,ext_id, lstExtension,configMutex);
								InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_B_LEG_TALKING);
								UpdateCallEvent(nwayci.cdrid,ext_id);
								//////////////////////////////////////////////////////////////////////////
								CDBCalloutInfo cci;
								nway_uint64_t extension_id = 0;
								bool is_record = false;
								list<NwayExtension>::iterator itExtension = lstExtension.begin();
								for (; itExtension != lstExtension.end(); itExtension++)
								{
									NwayExtension& myExt = *itExtension;
									if (myExt.extension_number == caller_id)
									{
										extension_id = myExt.id;
										is_record = myExt.is_record;
										break;
									}
								}
								nwayci.answer_extension_id = extension_id;
								if (is_record)
								{
									//开始录音
									string strFile,strFullname;
									GetRecordFileName(destination_number,strFile,strFullname,BasePath);
									cci.ExtensionAnswer(nwayci.id, extension_id, strFile.c_str());
									cdr.SetRecordFile(nwayci.cdrid,strFile.c_str());
									sprintf(tmp_cmd,"api uuid_record %s start %s 9999 \n\n",a_uuid.c_str(), strFullname.c_str());
									esl_send_recv_timed(handle, tmp_cmd,1000);
								}
								else
									cci.ExtensionAnswer(nwayci.id, extension_id, "");//暂不添加录音相关
								printf("%s, %d\tAnswered a:%s,\tb:%s\n",__FILE__,__LINE__,nwayci.call_a_leg_uuid.c_str(),nwayci.call_b_leg_uuid.c_str());
								//*itInfo = nwayci;
								//找到对应的 a leg 才会brideg，否则会异常，即某a 对不相干的某b
								sprintf(tmp_cmd,"bgapi uuid_bridge %s %s \n\n",a_leg_uuid.c_str(), strUUID.c_str());
								printf("%s, %d\t%s\n", __FILE__,__LINE__, tmp_cmd);
								esl_send_recv(handle,tmp_cmd);
								if (handle->last_sr_event && handle->last_sr_event->body) 
								{
									printf("[%s]\n", handle->last_sr_event->body);
									//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
								} 
								else 
								{
									printf("[%s] last_sr_reply\n", handle->last_sr_reply);
									//printf("%s,   %d ,  bgapi uuid_bridge failed\n",__FILE__,__LINE__);
								}
								break;
							}
						}
						if (bFound)
						{
							break;
						}
					}
					
				}
				else
				{
					//只是a leg 接通，但bleg还没接通，需要外呼到b leg
					//接通率+1

					LOGEX(__FILE__,__LINE__,"one call out number originated!");
					string taskid,precision;//一个是任务id,另一个是是否是精准呼叫
					taskid = esl_event_get_header(event, "variable_task_id")?esl_event_get_header(event, "variable_task_id"):"";
					precision = esl_event_get_header(event, "variable_precision")?esl_event_get_header(event, "variable_precision"):"";
					//bridged_uuid = esl_event_get_header(event, "variable_bridged_uuid")?esl_event_get_header(event, "variable_bridged_uuid"):"";
					//置cdr和外呼列表中的相关内容为相应的对应值
					esl_log(ESL_LOG_INFO,"task_id=%s,precision=%s\n",taskid.c_str(),precision.c_str());
					if (taskid.length() >0 && precision.length() >0)
					{
						//精准呼叫
						nway_uint64_t tid = atoi64(taskid.c_str());//转换为64长整型数
						bool bFound = false;
						list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
						for (; itTask != lstCalloutTasks.end(); itTask++)
						{
							Callout_Task& nwayct = *itTask;
							list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
							for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
							{
								Callout_info& nwayci = *itInfo;
								if (nwayci.call_a_leg_uuid == strUUID)
								{
									//a leg接通
									esl_log(ESL_LOG_INFO,"A leg answered,b leg uuid:%s\n",bridged_uuid.c_str());
									nwayci.call_b_leg_uuid = bridged_uuid;
									//计算a 接通时间等
									nwayci.callout_state = CALLOUT_A_TALKING; //a leg 接
									bFound = true;
									CCallDetailRecord cdr;
									//cdr.StartCall(destination_number,destination_number,caller_id,nwayci.cdrid,true);//开始一个呼叫记录
									//记录a通的事件
									nway_uint64_t ext_id = 0;
									//GetExtensionId(nwayci.extension_number,ext_id, lstExtension);
									InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_A_LEG_TALKING);
									//UpdateCallEvent(nwayci.cdrid,ext_id);
									//////////////////////////////////////////////////////////////////////////
									cdr.A_AnswerCall(nwayci.cdrid);
									CDBCalloutInfo cci;
									//cci.StartCallout(nwayci.id, nwayci.cdrid);
									cci.SetAlegAnswer(nwayci.id);


								}else
								if (nwayci.call_a_leg_uuid == bridged_uuid)
								{
									//b leg接通
									nwayci.call_b_leg_uuid = bridged_uuid;
									esl_log(ESL_LOG_INFO,"Bleg answered,a leg uuid:%s\n",nwayci.call_a_leg_uuid.c_str());
									nwayci.callout_state = CALLOUT_TALKING;
									//nwayci.call_b_leg_uuid = strUUID;//bleg 的uuid确认了
									bFound = true;
									CCallDetailRecord cdr;
									//cdr.StartCall(destination_number,destination_number,caller_id,nwayci.cdrid,true);//开始一个呼叫记录
									cdr.B_AnswerCall(nwayci.cdrid,nwayci.number.c_str(),"");
									//记录b通的事件
									nway_uint64_t ext_id = 0;
									GetExtensionId(nwayci.extension_number,ext_id, lstExtension,configMutex);
									InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_B_LEG_TALKING);
									UpdateCallEvent(nwayci.cdrid,ext_id);
									//////////////////////////////////////////////////////////////////////////
									CDBCalloutInfo cci;
									nway_uint64_t extension_id = 0;
									bool is_record = false;
									list<NwayExtension>::iterator itExtension = lstExtension.begin();
									for (; itExtension != lstExtension.end(); itExtension++)
									{
										NwayExtension& myExt = *itExtension;
										if (myExt.extension_number == caller_id)
										{
											extension_id = myExt.id;
											is_record = myExt.is_record;
											break;
										}
									}
									nwayci.answer_extension_id = extension_id;
									if (is_record)
									{
										//开始录音
										string strFile,strFullname;
										GetRecordFileName(destination_number,strFile,strFullname,BasePath);
										cci.ExtensionAnswer(nwayci.id, extension_id, strFile.c_str());
										cdr.SetRecordFile(nwayci.cdrid,strFile.c_str());
										sprintf(tmp_cmd,"api uuid_record %s start %s  \n\n",a_uuid.c_str(), strFullname.c_str());
										esl_send_recv_timed(handle, tmp_cmd,1000);
									}
									else
										cci.ExtensionAnswer(nwayci.id, extension_id, "");//暂不添加录音相关

								}
								if (bFound)
								{
									break;
								}
							}
						}
					}
					else
					{
						printf("%s, %d   is call out from: %s  to: %s &uuid:%s\n\n",__FILE__, __LINE__, caller_id.c_str(), destination_number.c_str(),strUUID.c_str());
						bool bFound = false;
						list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();
						printf("%s, %d\tESL_EVENT_CHANNEL_ANSWER call out b leg answered!-------------------\n",__FILE__, __LINE__);
						for (; itTask != lstCalloutTasks.end(); itTask++)
						{
							Callout_Task& nwayct = *itTask;
							list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
							printf("%s,\t %d \t task id:%lld \t call out info size:%d \n",__FILE__, __LINE__, nwayct.id ,nwayct.lstCalloutInfo.size());
							for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
							{
								Callout_info& nwayci = *itInfo;
								printf("%s,\t %d \tanswer uuid:%s ,\t nwayci uuid:%s \t b leg uuid:%s \n ",__FILE__,__LINE__,strUUID.c_str(),nwayci.call_a_leg_uuid.c_str(),nwayci.call_b_leg_uuid.c_str());
								if (nwayci.call_a_leg_uuid == strUUID)
								{
									//找到这次外呼的列表
									printf("%s ,%d\t a leg answered,uuid:%s\n", __FILE__, __LINE__, strUUID.c_str());
									nwayci.callout_state = CALLOUT_A_TALKING; //a leg 接
									bFound = true;
									CCallDetailRecord cdr;
									//cdr.StartCall(destination_number,destination_number,caller_id,nwayci.cdrid,true);//开始一个呼叫记录
									//记录a通的事件
									nway_uint64_t ext_id = 0;
									//GetExtensionId(nwayci.extension_number,ext_id, lstExtension);
									InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_A_LEG_TALKING);
									//UpdateCallEvent(nwayci.cdrid,ext_id);
									//////////////////////////////////////////////////////////////////////////
									cdr.A_AnswerCall(nwayci.cdrid);
									CDBCalloutInfo cci;
									//cci.StartCallout(nwayci.id, nwayci.cdrid);
									cci.SetAlegAnswer(nwayci.id);
									nwayci.ring_times = 1;//第一次呼叫振铃

									//*itInfo = nwayci;
									//播放语音文件
									//nwayct.ring_id
									if (nwayct.concurr_type_id ==  CONCURR_FORECAST_CALL)
									{
										//不是精准呼叫才会播放语音
										string strFullfilename;
										int nResult = GetRing(nwayct.ring_id,strFullfilename,lstRings,BasePath,configMutex);
										printf("%s, %d \tcall out playback ring:%s\n",__FILE__,__LINE__,strFullfilename.c_str());
										esl_execute(handle,"playback",strFullfilename.c_str(),strUUID.c_str());
										nwayct.success_number++;
									}
								
									//*itTask = nwayct;
									break;
								}
							}
							if (bFound)
							{
								break;
							}
						}
					}
					
				}
				

			}
			else
			{
				//esl_log(ESL_LOG_INFO, "CHANNEL_ANSWER %s\n", uuid);
				printf("%s,%d\tESL_EVENT_CHANNEL_ANSWER:not is call out inbound CHANNEL_ANSWER :%s:%s\n",__FILE__,__LINE__,strUUID.c_str(),a_uuid.c_str());
				//string bridged_uuid;
				//bridged_uuid = esl_event_get_header(event, "variable_bridged_uuid")?esl_event_get_header(event, "variable_bridged_uuid"):"";
				//printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
				if ( a_uuid.length() > 2)
				{
					//在bridge的同时，更新cdr和callinfo信息
					//bridge a leg and b leg
					pthread_mutex_lock(&infoMutex);
					SetAgentLoginStateBusyOrIdle(destination_number.c_str(),lstExtension,2,configMutex);//数据库内置分机状态忙
					list<Call_info>::iterator infoit = lstCallinfo.begin();
					for (; infoit != lstCallinfo.end(); infoit++)
					{
						//printf("%s,%d: ready to answer b:%s\n\n" ,uuid);
						Call_info& ci = *infoit;
						if (ci.caller_uuid == a_uuid)
						{
							//找到 a leg的uuid后进处相应的处理
							//////////////////////////////////////////////////////////////////////////
/*
							sprintf(tmp_cmd,"bgapi uuid_bridge %s %s \n\n",strUUID.c_str(), a_uuid.c_str());
							printf(tmp_cmd);
							esl_send_recv(handle,tmp_cmd);
							if (handle->last_sr_event && handle->last_sr_event->body) 
							{
								 
							} 
							else 
							{
								 
								LOGERREX(__FILE__,__LINE__,tmp_cmd);
							}
							*/
							CCallDetailRecord cdr;
							nway_uint64_t extension_id = 0;
							bool is_record = false,is_a_record =false;
							list<NwayExtension>::iterator itExtension = lstExtension.begin();
							for (; itExtension != lstExtension.end(); itExtension++)
							{
								NwayExtension& myExt = *itExtension;
								if (myExt.extension_number == destination_number)
								{
									extension_id = myExt.id;
									is_record = myExt.is_record;
									break;
								}
							}
							//当被叫的不录音时，查看主叫号码是否是本地分机及是否需要录音
							if (!is_record)
							{
								printf("%s, %d\tnot found to record ,now to query caller_numer:%s\n", __FILE__, __LINE__, ci.caller_number.c_str());
								for (list<NwayExtension>::iterator itExtension2 = lstExtension.begin(); itExtension2 != lstExtension.end(); itExtension2++)
								{
									NwayExtension& myExt2 = *itExtension2;
									if (myExt2.extension_number == ci.caller_number)
									{
										printf("%s, %d\tnot found to record ,now to query caller_numer!\n", __FILE__, __LINE__ );
										is_a_record = myExt2.is_record;
										break;
									}
								}
							}
							if (is_record || is_a_record)
							{
								//开始录音
								string strFile,strFullname;
								if (is_record)
								{
									GetRecordFileName(destination_number,strFile,strFullname,BasePath);
								}
								else
									GetRecordFileName(ci.caller_number,strFile,strFullname,BasePath);
								
								//cci.ExtensionAnswer(nwayci.id, extension_id, strFile);
								cdr.SetRecordFile(ci.cdr_id,strFile.c_str());
								//esl_execute(handle,"record",strFullname.c_str(),a_uuid.c_str());
								sprintf(tmp_cmd,"api uuid_record %s start %s 9999 \n\n",strUUID.c_str(), strFullname.c_str());
								esl_send_recv_timed(handle, tmp_cmd,1000);
								//esl_execute(handle, "uuid_record", strFullname.c_str(), a_uuid.c_str());
							}
							
							cdr.B_AnswerCall(ci.cdr_id,destination_number.c_str()," ");

							ci.trans_number = destination_number;
							ci.trans_uuid = strUUID;
							ci.trans_state = CALLOUT_TALKING;// bleg is talking;
							//记录a通的事件
							nway_uint64_t ext_id = 0;
							GetExtensionId(ci.trans_number,ext_id, lstExtension ,configMutex);
							InsertCallEvent(ci.cdr_id, ext_id,ci.caller_number,ci.trans_number,ci.called_number, CALL_EVENT_B_LEG_TALKING);
							UpdateCallEvent(ci.cdr_id,ext_id);
							//////////////////////////////////////////////////////////////////////////
							//*infoit = ci;
							//printf("has answer the b leg\n\n");
							break;
						}
						else
							if (ci.caller_uuid == strUUID)
							{
								nway_uint64_t ext_id = 0;
								GetExtensionId(ci.trans_number,ext_id, lstExtension ,configMutex);
								InsertCallEvent(ci.cdr_id, ext_id,ci.caller_number,ci.trans_number,ci.called_number, CALL_EVENT_A_LEG_TALKING);
								//UpdateCallEvent(ci.cdr_id,ext_id);
							}
					}
					pthread_mutex_unlock(&infoMutex);
					
				}
				else
				{
					pthread_mutex_lock(&infoMutex);
					SetAgentLoginStateBusyOrIdle(destination_number.c_str(),lstExtension,2,configMutex);//数据库内置分机状态忙
					list<Call_info>::iterator infoit = lstCallinfo.begin();
					for (; infoit != lstCallinfo.end(); infoit++)
					{
						//printf("%s,%d: ready to answer b:%s\n\n" ,uuid);
						Call_info& ci = *infoit;
						if (ci.caller_uuid == strUUID)
							{
								nway_uint64_t ext_id = 0;
								GetExtensionId(ci.trans_number,ext_id, lstExtension , configMutex);
								InsertCallEvent(ci.cdr_id, ext_id,ci.caller_number,ci.trans_number,ci.called_number, CALL_EVENT_A_LEG_TALKING);
								//UpdateCallEvent(ci.cdr_id,ext_id);
								break;
							}
							
					}
					pthread_mutex_unlock(&infoMutex);
				}
			}
		}
		//printf("ESL_EVENT_CHANNEL_ANSWER:origination_uuid: %s\n", esl_event_get_header(event, "origination_uuid"));
		//esl_log(ESL_LOG_INFO,"ESL_EVENT_CHANNEL_ANSWER:origination_uuid: %s\n", esl_event_get_header(event, "origination_uuid"));
		break;
	
	case ESL_EVENT_CHANNEL_OUTGOING:
		{
			string is_callout;
			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//esl_log(ESL_LOG_INFO, "ESL_EVENT_CHANNEL_OUTGOING %s\n", uuid);
			//printf("ESL_EVENT_CHANNEL_OUTGOING:inbound ESL_EVENT_CHANNEL_OUTGOING :%s\n",uuid);
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			/*const char *eventbody=esl_event_get_body(event);
			printf("body:\n%s\n",eventbody);*/
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				LOGEX(__FILE__,__LINE__,"one call out number originated!");
			}
		}
		break;
	case ESL_EVENT_PLAYBACK_START:
		{
			//播放开始
			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			printf("%s, %d\tthis is into ESL_EVENT_PLAYBACK_START,uuid:%s\n", __FILE__, __LINE__, strUUID.c_str());
		}
		break;
	case ESL_EVENT_PLAYBACK_STOP:
		{
			//播放语音结束
			//uuid = esl_event_get_header(event, "Caller-Unique-ID");
			//a_uuid = esl_event_get_header(event, "variable_a_leg_uuid");
			destination_number = esl_event_get_header(event, "Caller-Destination-Number");
			strUUID = esl_event_get_header(event, "Caller-Unique-ID")?esl_event_get_header(event, "Caller-Unique-ID"):"";
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			/*const char *eventbody=esl_event_get_body(event);
			printf("body:\n%s\n",eventbody);*/
			printf("%s, %d\tthis is into ESL_EVENT_PLAYBACK_STOP,uuid:%s\n", __FILE__, __LINE__, strUUID.c_str());
			if (trim(is_callout).length() >0)
			{
				//表示有外呼
				a_leg_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):""; 
				if (trim(a_leg_uuid).length() >12)
				{
					//b leg 一般不播放语音
					printf("%s, %d\tthis is into ESL_EVENT_PLAYBACK_STOP,a_leg_uuid:%s\n", __FILE__, __LINE__, a_leg_uuid.c_str());
				}
				else
				{
					LOGEX(__FILE__,__LINE__,"one call out number originated!");
					//置cdr和外呼列表中的相关内容为相应的对应值
					//printf("is call out from :%s  to:%s\n\n",caller_id, destination_number);
					printf("%s, %d\tESL_EVENT_PLAYBACK_STOP\n", __FILE__, __LINE__);
					bool bFound = false;
					list<Callout_Task>::iterator itTask = lstCalloutTasks.begin();

					for (; itTask != lstCalloutTasks.end(); itTask++)
					{
						Callout_Task& nwayct = *itTask;
						list<Callout_info>::iterator itInfo= nwayct.lstCalloutInfo.begin();
						for (; itInfo != nwayct.lstCalloutInfo.end(); itInfo++)
						{
							Callout_info& nwayci = *itInfo;
							printf("%s, %d\tESL_EVENT_PLAYBACK_STOP,nwayci uuid:%s, uuid:%s\n", __FILE__, __LINE__, nwayci.call_a_leg_uuid.c_str(),strUUID.c_str());
							if (nwayci.call_a_leg_uuid == strUUID)
							{
								//找到这次外呼的列表
								
								nwayci.callout_state = CALLOUT_PLAY_RING;
								bFound = true;
								printf("%s, %d\tESL_EVENT_PLAYBACK_STOP,ring times:%d\n", __FILE__, __LINE__, nwayci.ring_times);

								if (nwayci.ring_times == 1)
								{
									printf("%s, %d\tESL_EVENT_PLAYBACK_STOP,concurr_type_id:%d\n", __FILE__, __LINE__, nwayct.concurr_type_id);
									switch(nwayct.concurr_type_id)// == 1)
									{
									case CONCURR_FORECAST_CALL:
										{
											//预测外呼
											printf("%s, %d\tESL_EVENT_PLAYBACK_STOP,after_ring_play:%d\n", __FILE__, __LINE__, nwayct.after_ring_play);
											switch(nwayct.after_ring_play)
											{
											case 1:
												{
													//playring
													
													nwayci.ring_times = 2;
													string strFullfilename;
													int nResult = GetRing(nwayct.second_ring_id,strFullfilename,lstRings,BasePath ,configMutex);
													if (nResult == 0)
													{
														esl_execute(handle,"playback",strFullfilename.c_str(),nwayci.call_a_leg_uuid.c_str());
														 
														printf("%s, %d\tESL_EVENT_PLAYBACK_STOP,playback:%s\n", __FILE__, __LINE__,strFullfilename.c_str());
													}
													//*itInfo = nwayci;
												}
												break;
											case 2:
												{
													//hangup
													//esl_execute(handle,"hangup",NULL,NULL);
													nway_hangup(handle,strUUID.c_str());
												}
												break;
											case 3:
												{
													// bridge to agent
													char szCmd[2014] = {0};
													string strExtension;
													int nResult = GetIdleAgent(nwayct.group_id,strExtension,lstExtension,configMutex);//得到空闲的座席
													printf("%s,%d\tPLAYBACK_STOP\textension:%s,result:%d\n",__FILE__,__LINE__,strExtension.c_str(),nResult);
													if (nResult == 0)
													{
														
														//有空闲座席
														if (get_uuid(handle,nwayci.call_b_leg_uuid)){
															sprintf(szCmd,"bgapi originate {origination_uuid=%s,a_leg_uuid=%s,origination_caller_id_number=%s,is_callout=1,"
																"originate_timeout=15,task_id=%lld}user/%s &park()\n\n",nwayci.call_b_leg_uuid.c_str(),
																nwayci.call_a_leg_uuid.c_str(),nwayci.number.c_str(),nwayct.id,strExtension.c_str());
															printf(szCmd);
															esl_send_recv(handle,szCmd);
															nwayci.extension_number = strExtension;
															SetBusyAgent(nwayct.group_id, strExtension,lstExtension , configMutex);
															// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
															//	uuid[0]=0;
															if (handle->last_sr_event && handle->last_sr_event->body) 
															{
																printf("[%s]\n", handle->last_sr_event->body);
																//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
															} 
															else 
															{
																printf("[%s] last_sr_reply\n", handle->last_sr_reply);
															}
															//记录b呼的事件
															nway_uint64_t ext_id = 0;
															GetExtensionId(nwayci.extension_number,ext_id, lstExtension ,configMutex);
															InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_B_LEG_CALL);
															UpdateCallEvent(nwayci.cdrid,ext_id);
															//////////////////////////////////////////////////////////////////////////
															nwayci.callout_state = CALLOUT_TRANS_AGENT;//转给座席
														}
														else
														{
															//在获得了uuid后可能会发生变化
															char szCmd[2048] = {0};
															sprintf(szCmd,"bgapi uuid_kill %s\n\n",nwayci.call_a_leg_uuid.c_str());
															esl_send_recv(handle,szCmd);// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
															//	uuid[0]=0;
															if (handle->last_sr_event && handle->last_sr_event->body) 
															{
																printf("[%s]\n", handle->last_sr_event->body);
																//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
															} 
															else 
															{
																printf("[%s] last_sr_reply\n", handle->last_sr_reply);
															}
														}
														//*itInfo = nwayci;
													}
													else
													{
														//没有空闲座席的处理，暂为挂机
														esl_execute(handle,"hangup",NULL,NULL);
													}
												}
												break;
											default:
												;
											}
										}
										break;
									
									case CONCURR_RING_CALL:
										{
											//语音群呼
											//不管后续处理，直接挂机
											//nway_hangup(handle,strUUID.c_str());
										}
										break;
									default:
										;
									}
								}
								else
									if (nwayci.ring_times == 2)
									{
										//第二次彩铃后
										switch(nwayct.concurr_type_id)// == 1)
										{
										case CONCURR_FORECAST_CALL:
											{
												//预测外呼
												switch(nwayct.after_ring_play)
												{
												case 1:
													{
														//playring
														//当第二次放音后，不再进行彩铃播放
													}
													break;
												case 2:
													{
														//hangup
														nway_hangup(handle,strUUID.c_str());
													}
													break;
												case 3:
													{
														// bridge to agent
														char szCmd[2014] = {0};
														string strExtension;

														int nResult = GetIdleAgent(nwayct.group_id,strExtension,lstExtension ,configMutex);//得到空闲的座席
														if (nResult == 0)
														{
															//有空闲座席
															if (get_uuid(handle,nwayci.call_b_leg_uuid))
															{

															sprintf(szCmd,"bgapi originate {origination_uuid=%s,a_leg_uuid=%s,origination_caller_id_number=%s,is_callout=1,"
																"originate_timeout=15,task_id=%lld}user/%s  &park()\n\n",nwayci.call_b_leg_uuid.c_str(),
																nwayci.call_a_leg_uuid.c_str(),nwayci.number.c_str(),nwayct.id,strExtension.c_str(),'%');
															printf(szCmd);
															esl_send_recv(handle,szCmd);
															// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
															//	uuid[0]=0;
															if (handle->last_sr_event && handle->last_sr_event->body) 
															{
																printf("[%s]\n", handle->last_sr_event->body);
																//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
															} 
															else 
															{
																printf("[%s] last_sr_reply\n", handle->last_sr_reply);
															}
															nwayci.extension_number = strExtension;
															SetBusyAgent(nwayct.group_id, strExtension,lstExtension , configMutex);
															//记录b呼的事件
															nway_uint64_t ext_id = 0;
															GetExtensionId(nwayci.extension_number,ext_id, lstExtension , configMutex);
															InsertCallEvent(nwayci.cdrid, ext_id,nwayci.number,nwayci.extension_number,nwayct.gateway_url, CALL_EVENT_B_LEG_CALL);
															UpdateCallEvent(nwayci.cdrid,ext_id);
															//////////////////////////////////////////////////////////////////////////
															nwayci.callout_state = CALLOUT_TRANS_AGENT;//转给座席
															}
															else
															{
																char szCmd[2048] = {0};
																sprintf(szCmd,"bgapi uuid_kill %s\n\n",nwayci.call_a_leg_uuid.c_str());
																esl_send_recv(handle,szCmd);// "bgapi originate {originate_timeout=30}sofia/internal/1005%192.168.1.102 &park()\n\n");
																//	uuid[0]=0;
																if (handle->last_sr_event && handle->last_sr_event->body) 
																{
																	printf("[%s]\n", handle->last_sr_event->body);
																	//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
																} 
																else 
																{
																	printf("[%s] last_sr_reply\n", handle->last_sr_reply);
																}
															}
														}
														else
														{
															//没有空闲座席的处理，暂为挂机
															nway_hangup(handle,strUUID.c_str());
														}
													}
													break;
												default:
													;
												}
											}
											break;
										
										case CONCURR_RING_CALL:
											{
												//语音群呼
												//不管后续处理，直接挂机
												nway_hangup(handle,strUUID.c_str());
											}
											break;
										default:
											;
										}
									}
								//nwayci.ring_times = 1;//第一次呼叫振铃

								
								break;
							}
						}
						if (bFound)
						{
							break;
						}
					}
				}


			}
		}
		break;
	case ESL_EVENT_CHANNEL_PROGRESS:
		{
			printf("%s, %d\tCHANNEL_PROGRESS:%s\n",__FILE__,__LINE__,destination_number.c_str());
			a_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):"";
			destination_number = esl_event_get_header(event, "Caller-Destination-Number");
			//caller_id = esl_event_get_header(event, "Caller-Caller-ID-Number");
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			/*const char *eventbody=esl_event_get_body(event);
			printf("body:\n%s\n",eventbody);*/
			string bridged_uuid;
			
			bridged_uuid = esl_event_get_header(event, "other-leg-unique-id")?esl_event_get_header(event, "other-leg-unique-id"):"";
			//printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
			if (trim(is_callout).length() >0)
			{
				
			}
			else
			{
				esl_log(ESL_LOG_INFO, "CHANNEL_PROGRESS %s\n", strUUID.c_str());
				
				string bridged_uuid;
				//bridged_uuid = esl_event_get_header(event, "variable_bridged_uuid")?esl_event_get_header(event, "variable_bridged_uuid"):"";
				printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
				if ( a_uuid.length() > 2)
				{
					//在bridge的同时，更新cdr和callinfo信息
					//bridge a leg and b leg
					pthread_mutex_lock(&infoMutex);
					SetAgentLoginStateBusyOrIdle(destination_number.c_str(),lstExtension,2 , configMutex);//数据库内置分机状态忙
					list<Call_info>::iterator infoit = lstCallinfo.begin();
					for (; infoit != lstCallinfo.end(); infoit++)
					{
						//printf("%s,%d: ready to answer b:%s\n\n" ,uuid);
						Call_info& ci = *infoit;
						if (ci.caller_uuid == a_uuid)
						{
							//找到 a leg的uuid后进处相应的处理
							//////////////////////////////////////////////////////////////////////////
							/*sprintf(tmp_cmd,"bgapi uuid_break %s  \n\n",  a_uuid.c_str());
							printf(tmp_cmd);
							esl_send_recv(handle,tmp_cmd);
							if (handle->last_sr_event && handle->last_sr_event->body) 
							{
								//printf("[%s]\n", handle->last_sr_event->body);
								//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
							} 
							else 
							{
								//printf("[%s] last_sr_reply\n", handle->last_sr_reply);
								//printf("%s,   %d ,  bgapi uuid_bridge failed\n",__FILE__,__LINE__);
								LOGERREX(__FILE__,__LINE__,tmp_cmd);
							}
							*/
							sprintf(tmp_cmd,"bgapi uuid_bridge %s %s \n\n", a_uuid.c_str(),strUUID.c_str());
							printf(tmp_cmd);
							esl_send_recv(handle,tmp_cmd);
							if (handle->last_sr_event && handle->last_sr_event->body) 
							{
								//printf("[%s]\n", handle->last_sr_event->body);
								//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
							} 
							else 
							{
								//printf("[%s] last_sr_reply\n", handle->last_sr_reply);
								//printf("%s,   %d ,  bgapi uuid_bridge failed\n",__FILE__,__LINE__);
								LOGERREX(__FILE__,__LINE__,tmp_cmd);
							}
							break;
						}
					}
							
					pthread_mutex_unlock(&infoMutex);
					
				}
				else
				{
					
				}
			}
		}
		break;
	case ESL_EVENT_CHANNEL_PROGRESS_MEDIA:
		{
			printf("%s, %d\tCHANNEL_PROGRESS_MEDIA:%s\n",__FILE__,__LINE__,destination_number.c_str());
			//printf("%s, %d\tCHANNEL_PROGRESS:%s\n",__FILE__,__LINE__,destination_number.c_str());
			a_uuid = esl_event_get_header(event, "variable_a_leg_uuid")?esl_event_get_header(event, "variable_a_leg_uuid"):"";
			destination_number = esl_event_get_header(event, "Caller-Destination-Number");
			//caller_id = esl_event_get_header(event, "Caller-Caller-ID-Number");
			string is_callout, a_leg_uuid;
			is_callout = esl_event_get_header(event, "variable_is_callout")?esl_event_get_header(event, "variable_is_callout"):""; // 可以为1则是正在外呼的
			/*const char *eventbody=esl_event_get_body(event);
			printf("body:\n%s\n",eventbody);*/
			//string bridged_uuid;

			//bridged_uuid = esl_event_get_header(event, "other-leg-unique-id")?esl_event_get_header(event, "other-leg-unique-id"):"";
			//printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
			if (trim(is_callout).length() >0)
			{

			}
			else
			{
				esl_log(ESL_LOG_INFO,  "CHANNEL_PROGRESS_MEDIA %s\n", strUUID.c_str());

				//string bridged_uuid;
				//bridged_uuid = esl_event_get_header(event, "variable_bridged_uuid")?esl_event_get_header(event, "variable_bridged_uuid"):"";
				//printf("%s, %d\tbridged_uuid:%s\n",__FILE__,__LINE__,bridged_uuid.c_str());
				if ( a_uuid.length() > 2)
				{
					//在bridge的同时，更新cdr和callinfo信息
					//bridge a leg and b leg
					pthread_mutex_lock(&infoMutex);
					SetAgentLoginStateBusyOrIdle(destination_number.c_str(),lstExtension,2 , configMutex);//数据库内置分机状态忙
					list<Call_info>::iterator infoit = lstCallinfo.begin();
					for (; infoit != lstCallinfo.end(); infoit++)
					{
						//printf("%s,%d: ready to answer b:%s\n\n" ,uuid);
						Call_info& ci = *infoit;
						if (ci.caller_uuid == a_uuid)
						{
							//找到 a leg的uuid后进处相应的处理
							//////////////////////////////////////////////////////////////////////////

							sprintf(tmp_cmd,"bgapi uuid_bridge %s %s \n\n",strUUID.c_str(), a_uuid.c_str());
							printf(tmp_cmd);
							esl_send_recv(handle,tmp_cmd);
							if (handle->last_sr_event && handle->last_sr_event->body) 
							{
								//printf("[%s]\n", handle->last_sr_event->body);
								//	strcpy(uuid,(handle.last_sr_event->body)+strlen("+OK Job-UUID: "));
							} 
							else 
							{
								//printf("[%s] last_sr_reply\n", handle->last_sr_reply);
								//printf("%s,   %d ,  bgapi uuid_bridge failed\n",__FILE__,__LINE__);
								LOGERREX(__FILE__,__LINE__,tmp_cmd);
							}
							break;
						}
					}

					pthread_mutex_unlock(&infoMutex);

				}
				else
				{

				}
			}
		}
		break;
	default:
		;
	}
}
