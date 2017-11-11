#ifndef __SG_LOG_H____
#define __SG_LOG_H____
//////////////////////////////////////////////////////////////////////////
//用于生成跨平台的日志库
//author:lihao
//date:2011-3-16
#include <stdexcept>
#include <string>
#include <iostream>
#include "IVCLog.h"
using namespace std;
#ifndef MAX_PATH
#define MAX_PATH 260
#endif
//#ifdef _UNICODE
//typedef wstring TString;//如果是UNICODE模式，那么字符串使用宽字符串的
//#else
//typedef string TString;
//#endif



#define MAX_FILE_SIZE 24*1024
class CNwayLog
{
public:
	



	static CNwayLog* GetInstance();
	void Release();
	virtual ~CNwayLog();
public:
	int Write(const char* fmt,...);
protected:
	int GetFileName(string &strFilename);//按年月日时分秒产生日志文件
	int GetTimeStr(string & strTimeStr);//获得时间字符串，到ms
	int Close();
	int Open();
private:
	CNwayLog();//构造函数作为私有，从而让日志只出现一次
	string _strFilename;
	CLog_T* _log;
	bool bOpen;//日志文件是否打开
	
};
#endif

