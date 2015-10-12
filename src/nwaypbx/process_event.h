#ifndef __NWAY_PROCESS_EVENT_H__
#define __NWAY_PROCESS_EVENT_H__

#include<stdio.h>
#ifdef WIN32
#include <io.h>
#else
//#include <syswait.h>
#include <unistd.h>
#include <sys/io.h>
#endif
#include <stdlib.h>
#include "esl/esl.h"
#include "nwaycti/nwaycti.h"
#include "nwaycti/DirectCall.h"
#include "nwaycti/NwayIVR.h"
#include "nwaycti/structdef.h"
#include "Check_Content.h"
#include "log/log.h"
#include "common/PcreCpp.h"
#include "common/NwayDef.h"
//char guuid[128];
#define disp_msg  //printf
#include <pthread.h>
#include <string>
#include <vector>
#include "common/NwayStr.h"
using namespace std;

void process_event(esl_handle_t *handle, esl_event_t *event, pthread_mutex_t& infoMutex, pthread_mutex_t& calloutMutex,list<Callout_Task>& lstCalloutTasks, list<NwayExtension>& lstExtension, list<Call_info>& lstCallinfo, list<NwayRing>& lstRings, string& BasePath ,pthread_mutex_t& configMutex);



#endif