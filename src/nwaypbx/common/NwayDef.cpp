#include "stdafx.h"
#include "NwayDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#endif
#include <iostream>
#include <string>
//#include <boost/algorithm/string.hpp>
//#include <boost/filesystem/path.hpp>
//#include <boost/filesystem/operations.hpp>
//#include <boost/format.hpp>
using namespace std;
/*void get_path(char* aPath)
{

	string fullpath = boost::filesystem::initial_path<boost::filesystem::path>().string();
	
	strcpy(aPath,fullpath.c_str());

}*/
int file_exist( const char* filename )
{
#ifdef WIN32
	FILE *stream;

	if( (stream  = fopen( filename, "r" )) == NULL ) 
	{
		return 1;
	}	
	else
	{
		fclose(stream);
		return 0;
	}
	return 0;
#endif
	/*
	FILE *stream;

	if( (stream  = fopen( filename, "r" )) == NULL ) 
	{
		return 1;
	}	
	else
	{
                char szTmp[255];
                strcpy(szTmp,filename);
		char* ptr=strstr(szTmp,".");
               if (ptr != NULL)
                  return 0;
                else
			return 1;
		fclose(stream);
		return 1;
	}
*/
}
void NwaySleep(int ms)
{
#ifdef _WIN32
	::Sleep(ms);
#else
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
#endif
}
long long atoi64(const char* s)
{
	long long ret = 0;
#ifdef WIN32
	ret =_atoi64(s);
#else
	ret = strtoll (s,0,0);
#endif
	return ret;
}
time_t convert_string_to_time_t(const char* time_string)  
{  
	struct tm tm1;  
	time_t time1;  
	int i = sscanf(time_string, "%d/%d/%d %d:%d:%d" ,       
		&(tm1.tm_year),   
		&(tm1.tm_mon),   
		&(tm1.tm_mday),  
		&(tm1.tm_hour),  
		&(tm1.tm_min),  
		&(tm1.tm_sec),  
		&(tm1.tm_wday),  
		&(tm1.tm_yday));  

	tm1.tm_year -= 1900;  
	tm1.tm_mon --;  
	tm1.tm_isdst=-1;  
	time1 = mktime(&tm1);  

	return time1;  
}  