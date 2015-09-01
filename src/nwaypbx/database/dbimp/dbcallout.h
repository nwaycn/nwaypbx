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
#ifndef __NWAY_CALLOUT_H
#define __NWAY_CALLOUT_H

//////////////////////////////////////////////////////////////////////////
//用于外呼相关表的操作
//2014-9-29
//author:li hao
//////////////////////////////////////////////////////////////////////////
#include "../interface/db_center.h"
#include "../../nwaycti/statedef.h"
#include "../../nwaycti/structdef.h"
#include <list>
using namespace std;
class CDBCallout
{
public:
	CDBCallout();
	~CDBCallout();
protected:
	int GetCalloutInfo(list<Callout_info>& lstcallinfo);
private:
};
#endif