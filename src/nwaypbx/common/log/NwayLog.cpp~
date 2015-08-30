#include "stdafx.h"
#include "NwayLog.h"
//#include "../common/File.h"
//
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//
CNwayLog* logInstance = NULL;
CNwayLog* CNwayLog::GetInstance()
{
	if (logInstance == NULL)
	{
		logInstance = new CNwayLog();
		logInstance->Open();
	}
	return logInstance;
}

void CNwayLog::Release()
{
	if (logInstance)
	{
		logInstance->Close();
		delete logInstance;
		logInstance = NULL;
	}
}

CNwayLog::~CNwayLog()
{

}

int CNwayLog::Write( const char* fmt,... )
{
	//输出的字符串拼装
	char s[4096];
	string strOutput;
	int nStatus = -1;
	va_list args;
	va_start(args,fmt);	 
	//_vstprintf(s,fmt,args);
	vsprintf(s,fmt,args);
	va_end (args);
	if (_log)
	{
		_log->Userlog(CLog_T::DateAndTime,s);
	}
	return nStatus;
	
}

int CNwayLog::GetFileName( string &strFilename )
{
	/*struct timeb tp; 
	struct tm * t;
	ftime(&tp);
	t= localtime(&(tp.time));
	char szTemp[MAX_PATH]={0};
	get_path(szTemp);
	strFilename = szTemp;
	strFilename += "/log";
	CDirOpr cdo;
	if (cdo.isExist(strFilename) != 0)
	{
		cdo.createDir(strFilename);
	}
	memset(szTemp,0,MAX_PATH);
	sprintf(szTemp,"/log_%4.2i%2.2i%2.2i%2.2i%2.2i%2.2i.log",t->tm_year+1900,t->tm_mon+1,t->tm_yday,t->tm_hour,t->tm_min,t->tm_sec);
	strFilename += szTemp; 	*/
	return 0;
}

CNwayLog::CNwayLog():_log(NULL)
{

}

int CNwayLog::Close()
{
	//_file.fclose();
	if (_log)
	{
		delete _log;
		_log = NULL;
	}
	bOpen = false;
	return 0;
}

int CNwayLog::Open()
{
	//_log.SetLevel(CLog_T::FileByDay);

	_log = new CLog_T();
	if (_log)
	{
		bOpen = true;
		return 0;
	}
	return -1;//表示没有打开日志文件
}

int CNwayLog::GetTimeStr( string & strTimeStr )
{
	struct timeb tp; 
	struct tm * t;
	ftime(&tp);
	t= localtime(&(tp.time));
	char szTemp[MAX_PATH]={0};
	
	memset(szTemp,0,MAX_PATH);
	sprintf(szTemp,"%4.2i-%2.2i-%2.2i %2.2i:%2.2i:%2.2i.%3.2i ",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,tp.millitm);
	strTimeStr = szTemp;
	return 0;
}

