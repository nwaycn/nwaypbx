/*************************************************************************
based for NwayPBX
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License£º GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/
#ifndef __NWAY_STRING_OPT__H
#define __NWAY_STRING_OPT__H


#include <iostream>
#include <vector>
#include <string>
using namespace std;
string trim(const string& str);
 

int split(const string& str, vector<string>& ret_, string sep = ",");
	 

string replace(const string& str, const string& src, const string& dest);
 

#endif