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
#include "PcreCpp.h"
#include <stdio.h>

Pcre::Pcre()
{
	re_arr.clear();
	patten_id.clear();
}

Pcre::~Pcre()
{
	for(int i=0; i<re_arr.size(); i++)
	{
		pcre_free(re_arr[i]);
	}
}

//Add a regrex patten and compile it.
int Pcre::AddRule(const nway_uint64_t&  id, const string &patten)
{
	pcre *re = pcre_compile(patten.c_str(), 0, &error, &erroffset, NULL);
	if(re == NULL)
	{
		printf("%s, %d\tpcre compile failed, offset %d: %s\n", __FILE__,__LINE__,erroffset, error);
		return -1;
	}
	else
	{
		re_arr.push_back(re);
		patten_id.push_back(id);
	}
}

//clear all the rule
void Pcre::ClearRules()
{
	for(int i=0; i<re_arr.size(); i++)
	{
		pcre_free(re_arr[i]); 
	}
	re_arr.clear();
	patten_id.clear();
}

//match all regrex, if any match, return the matched patten name and it's values
vector<MatchResult> Pcre::MatchAllRule(const char content[])
{
	vector<MatchResult> result_arr;
	int length = strlen(content);
	char buf[512];
	for(int i=0; i<re_arr.size(); i++)
	{
		MatchResult result;
		result.id = patten_id[i];
		int cur = 0;
		int rc;
		while(cur<length && (rc = pcre_exec(re_arr[i], NULL, content, length, cur, PCRE_NOTEMPTY, ovector, VECSIZE)) >= 0)
		{
			for(int j=0; j<rc; j++)
			{
				memset(buf, 0, sizeof(buf));
				strncpy(buf, content+ovector[2*j], ovector[2*j+1]-ovector[2*j]);
				result.value.push_back(buf);
			}
			cur = ovector[1];
		}
		if(result.value.size() > 0)
			result_arr.push_back(result);
	}
	return result_arr;
}