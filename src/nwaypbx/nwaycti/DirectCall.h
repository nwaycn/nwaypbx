#ifndef __NWAYCIT_DIRECT_CALL_H
#define __NWAYCIT_DIRECT_CALL_H
#include "../common/NwayDef.h"
#include "../esl/esl.h"
#include <list>
#include "structdef.h"
#include "statedef.h"
#include "../Check_Content.h"
#include <string>
#include "../common/PcreCpp.h"
#include <vector>
#include <pthread.h>
using namespace std;
//////////////////////////////////////////////////////////////////////////
//针对直呼进行相应的处理
/////////////////////////////////////////////////////////////////////////
int DirectCall(esl_handle_t& handle,nway_uint64_t& cdrid, nway_uint64_t& diplanid,const char* str_extension,const char* aleg_uuid, const char* basepath,\
	list<NwayDialplan>& lstDialplans,list<NwayDialplanDetail>& lstDialplanDetails,list<NwayRing>& lstRings, list<NwayExtension>& lstExtensions,\
	vector<MatchResult>& matchs, bool& hasAnswer , list<Call_Task>& lstCallTask, pthread_mutex_t& taskMutex , const char* origination_number , pthread_mutex_t& configMutex);
#endif