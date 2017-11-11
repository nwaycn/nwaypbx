/***************************************************************
* Author：		lihao
* Company：		nway							  			
* Last Update：	2011-03-17		 								
* Copyright(c)
*------------------------------------------------------------ 	
* FileName：		IVCLog.h
* Function Note：	Sys Log implementation.
* Update note：		
*****************************************************************/

//Level 1：正常流程不应该发生的故障：如new操作失败，文件IO操作失败，异常退出等等；
//Level 2：参数有效性判断，以及指针NULL判断为不符和规则；
//Level 3: 函数中途异常返回；
//Level 4: 命令消息的打印输出；
//Level 5: 程序流程中重要变量内容的调试输出；
//默认日志级别是3，按天产生日志，日志记录加时间戳;如果没有设置FileByDay|FileByName选项，默认为DisplayScreen即输出到屏幕

#ifndef  _IV_CLOG_H_
#define  _IV_CLOG_H_

#define MAX_LEVEL  256

#define TBUF_SIZE 102400
#define PATH_MAX_SIZE 256

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#define ANSI

#ifndef HAVE_STD_IOSTREAM
#define HAVE_STD_IOSTREAM
#endif

#include <string>
#include <stdarg.h>



#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

	class CLog_T
	{
	#if defined(WIN32)
		CRITICAL_SECTION	_mutex;
	#else
		pthread_mutex_t		_mutex;
	#endif
	protected:
		FILE *m_pf;
		string m_Suffix;
		string m_Prefix;

		unsigned m_LogOptions;
		unsigned m_LogLevel;
		string m_LogFileName;
		char m_strBuf[TBUF_SIZE];
		char m_LogPath[PATH_MAX_SIZE];
		bool m_bEnable;
		string m_DebugFileName;
		int m_LineNum;
		
		//把时间格式从20030910061630改为2003 09-10 10:30:30
		string FormatTime(string systemTime);
		void lock();
		void unlock();
		//写日志记录的头
		short BeginLog(unsigned level);
		//不加时间戳,每天只写一次

		short BeginLogHead(unsigned level);	
		//在文件名前加上路径
		string AddPath(const char *fileName);

		void SetFileAndLine(const char *fileName,int lineNum)
		{
			m_DebugFileName = fileName;
			m_LineNum = lineNum;
		}

	public:		
		// 调试输出选择
		enum Options {    
			FileByDay = 1,	//按天记日志
			DateAndTime = 2,//加上时间戳			
			TraceLevel = 4,//输出当前的级别
			FileAndLine = 16,//此选项暂时没用是为调试准备的
			DisplayScreen = 32,//在屏幕上显示
			FileByName = 64		//按指定文件名产生日志
		};	

		CLog_T();
		~CLog_T();
		CLog_T( unsigned level,unsigned options,const char *filename );
		void  ReturnFileName(char *filename);

		// 设置选项
		void SetOptions(unsigned options );	
		void ClearOptions(unsigned options );	
		unsigned GetOptions( );
		void AddOptions(unsigned options);
		
		// 设置等级
		void SetLevel(unsigned level );	
		unsigned GetLevel();
		
		//判断调试等级是否达到
		bool CanTrace(unsigned level );
		//设置日志文件名，只有在设置成FileByName时才有效
		short SetFile( const char *fileName );
		void SetLogPath( const char *logPath );
		//打开关闭日志输出
		void Enable(bool enable=true )
		{
			m_bEnable = enable;
		}
		//按天产生日志时设置文件名的前缀与后缀，例如prefix是ivsender, suffix是log，时间是2011-03-21则文件名为ivsender20110321.log
		void SetPrefixSuffix(const char *prefix, const char *suffix)
		{
			m_Prefix = prefix;
			m_Suffix = suffix;
		}
		//清空文件流
		void Flush()
		{
#ifdef WIN32
			fflush(m_pf);
#endif
		}
		//获取系统时间，屏蔽操作系统
		static string GetSystemTime();
		static string GetFileTime();
		//写日志的函数，书写格式与printf类似仅仅多了一个日志级别参数
	#ifdef ANSI	
		long Userlog(unsigned level,const char *fmt,...);
	#else
		long Userlog(unsigned level,const char *fmt,va_alist);
	#endif	

	#ifdef ANSI	
		long UserlogHead(unsigned level,const char *fmt,...);
	#endif
	};
		


#endif // (_IV_CLOG_H_)
