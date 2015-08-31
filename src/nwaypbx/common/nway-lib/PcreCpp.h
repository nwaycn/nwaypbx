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
#ifndef _NWAY_PCRE_H_
#define _NWAY_PCRE_H_
#include <pcre.h>
#include <string>
#include <vector>
#include <string.h>
#include "NwayDef.h"
using namespace std;

const int VECSIZE = 30;

struct MatchResult
{
	nway_uint64_t id;
	vector<string> value;
};

class Pcre
{
public:
	Pcre();
	~Pcre();

	//Add a regrex, pass in name and regrex
	int AddRule(const nway_uint64_t& id, const string &patten);

	//clear all the regrex
	void ClearRules();

	//match all the regrex, also return all the string match to every regrex
	vector<MatchResult> MatchAllRule(const char content[]);

private:
	const char *error;
	int erroffset;
	int ovector[VECSIZE];
	vector<pcre*> re_arr;
	vector<nway_uint64_t> patten_id;
};
#endif