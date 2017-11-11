// osdep.h

#if !defined(__OSDEP_H__)
#define __OSDEP_H__

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(_WINDOWS) || defined(_WINDOWS_)
#define SQLAPI_WINDOWS
#ifdef _MSC_VER
	#pragma warning(disable : 4290 4121 4995)
#endif
#if defined(__BORLANDC__) && (__BORLANDC__ > 0x520)
	#pragma warn -8059
	#pragma warn -8061
	#ifdef NDEBUG
		#pragma warn -8004
		#pragma warn -8057
	#endif
#endif
#endif

#ifdef SQLAPI_WINDOWS
#ifndef _WIN64
// MSVS 2005 works without this now
//#define _USE_32BIT_TIME_T 1
#else
#define SA_64BIT 1
#endif
#define _CRT_SECURE_NO_DEPRECATE 1
#include <windows.h>
#endif

#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
#include <limits.h>
#if !defined(SQLAPI_WINDOWS) || defined (CYGWIN)
#include <wchar.h>
#include <wctype.h>
#include <dlfcn.h>
#endif
#include <ctype.h>
#include <math.h>

#ifdef SA_64BIT
#if defined(SQLAPI_WINDOWS) || defined(SA_DB2_ODBC64)
#define ODBC64 // for DB2
#endif
#define SS_64BIT_SERVER // for OCI7
#define SYB_LP64 1 // for Sybase
#endif

#include <SQLAPI.h>

#ifdef SA_UNICODE
#ifdef SQLAPI_WINDOWS
#define SA_FMT_STR L"s"
#else
#define SA_FMT_STR L"S"
#include "utf8.h"
#include "utf16.h"
#endif
#define SA_UNICODE_WITH_UTF8 1
#else
#define SA_FMT_STR "s"
#undef SA_UNICODE_WITH_UTF8
#ifndef SQLAPI_WINDOWS
#include "utf16.h"
#endif
#endif

SAChar *sa_strlwr(SAChar *src);
SAChar *sa_strupr(SAChar *src);

#ifndef SHARED_OBJ_EXT
#if defined(SQLAPI_WINDOWS)
#	define SHARED_OBJ_EXT _TSA(".dll")
#else
#if defined(SQLAPI_AIX)
#ifdef __64BIT__
#	define SHARED_OBJ_EXT ".a(shr_64.o)"
#else
#	define SHARED_OBJ_EXT ".a(shr.o)"
#endif
#else
#if defined(SQLAPI_HPUX)
#	define SHARED_OBJ_EXT ".sl"
#else
#if defined(SQLAPI_OSX)
#  define SHARED_OBJ_EXT ".dylib"
#else
#  define SHARED_OBJ_EXT ".so"
#endif
#endif
#endif
#endif
#endif

#if defined(SQLAPI_AIX)
#  define EXT_DLOPEN_FLAGS RTLD_MEMBER
#else
#  define EXT_DLOPEN_FLAGS 0
#endif

#define SA_MAKELONG(a, b)      (((long)((short)(a))) | (((long)((short)(b))) << 16))

long SAExtractVersionFromString(const SAString &sVerString);
#ifdef SQLAPI_WINDOWS
void *SAGetVersionInfo(LPCTSTR sDLLName);
long SAGetFileVersionFromString(LPCTSTR sDLLName);
long SAGetProductVersion(LPCTSTR sDLLName);
DWORD SAGetLastErrorText(SAString& sMessage);
HMODULE SALoadLibraryFromList(SAString& sLibsList, SAString& sErrorMessage, SAString& sLibName);

#include <float.h>
#define sa_isnan(x) _isnan(x)
#else
#include <dlfcn.h>
void* SALoadLibraryFromList(SAString& sLibsList, SAString& sErrorMessage, SAString& sLibName, int flag);

#define sa_isnan(x) isnan(x)
#endif // defined(SQLAPI_WINDOWS)

#if (_MSC_VER >= 1300) && (WINVER < 0x0500)
// VC7 or later, building with pre-VC7 runtime libraries
extern "C" long _ftol2(double dblSource);
#endif

#include <samisc.h>
#include "saerrmsg.h"

// internal strtoull
#ifdef SQLAPI_WINDOWS
#	ifndef SA_INTERNAL_STRTOULL
#		if (_MSC_VER >= 1400)
#			define sa_strtoull(x,y,z) _strtoui64(x,y,z)
#		else
#			if defined(__MINGW32__) || (__BORLANDC__ > 0x630)
#				define sa_strtoull(x,y,z) strtoull(x,y,z)
#			else
#				define SA_INTERNAL_STRTOULL 1
#			endif
#		endif
#	endif
#else // ! SQLAPI_WINDOWS
#	if defined(SQLAPI_HPUX) || defined(SQLAPI_SCOOSR5)
#		define sa_strtoull(x,y,z) strtoul(x,y,z)
#	else
#		define sa_strtoull(x,y,z) strtoull(x,y,z)
#	endif
#endif // SQLAPI_WINDOWS

#ifdef SA_INTERNAL_STRTOULL
extern "C" sa_uint64_t sa_strtoull(const char *nptr, char **endptr, int base);
#endif

#endif // !defined(__OSDEP_H__)
