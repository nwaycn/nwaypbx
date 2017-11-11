// *****************************************************
// Copyright (c) 2001-2011 iAnywhere Solutions, Inc.
// Portions copyright (c) 1988-2011 Sybase, Inc.
// All rights reserved. All unpublished rights reserved.
// *****************************************************

#ifndef _SQLOS_H_INCLUDED
#define _SQLOS_H_INCLUDED

/******************************************************************/
/*  NOTE: _SQL_OS_<operating-system> will be defined by the       */
/*        preprocessor -o option.                                 */
/*      - also use macros that are defined in various environments*/
/*        by various compilers to set the appropriate _SQL_OS     */
/*        macro for those files that are not preprocessed         */
/******************************************************************/

#if !defined( UNIX ) && ( defined( _SQL_OS_UNIX ) || defined( _SQL_OS_UNIX64 ) || defined( __unix__ ) || defined( __APPLE__ ) || defined( __sun ) || defined( __hpux ) || defined( FREEBSD ) || defined( _AIX ) || defined( __linux__ ) )
#if !defined( ULPLAT_UNIXBASE )
#define UNIX
#endif
#endif

#if !defined( UNIX64 ) && defined( UNIX ) && ( defined( _SQL_OS_UNIX64 ) || defined( __LP64__ ) || defined( __64BIT__ ) || defined( __sparcv9 ) || defined( __amd64 ) )
#define UNIX64
#endif

#if defined( UNDER_PALM_OS )
    #define _SQL_OS_PALM
#endif

#if defined( WINNT ) \
 || defined( _WINNT ) \
 || defined( __NT__ ) \
 || defined( WIN32 ) \
 || defined( _WIN32 ) \
 || defined( __WIN32__ ) \
 || defined( UNDER_CE )
    #define _SQL_OS_WINNT
#endif

#if defined( UNIX64 )
    #define _SQL_OS_UNIX64
#elif defined( UNIX ) \
|| defined( UNDER_UNIX ) \
|| defined( UNDER_VXW )  \
|| defined( ULPLAT_UNIXBASE )
    #define _SQL_OS_UNIX
#endif

/******************************************************************/
/*  Make sure that one of the _SQL_OS_ macros has been defined    */
/******************************************************************/

#if !defined( _SQL_OS_WINNT )	\
&&  !defined( _SQL_OS_PALM )	\
&&  !defined( _SQL_OS_UNIX )	\
&&  !defined( _SQL_OS_UNIX64 )
    #error The appropriate _SQL_OS macro has not been defined
#endif

/******************************************************/
/*       Define the variant of UNIX being used        */
/******************************************************/
#if defined( _SQL_OS_UNIX ) || defined( _SQL_OS_UNIX64 )
#if defined( _AIX ) || defined( __aix__ )
#define _SQL_OS_UNIX_VARIANT_AIX
#elif defined( __sun ) || defined( __sun__ )
#define _SQL_OS_UNIX_VARIANT_SUN
#elif defined( __hpux ) || defined( __hpux__ )
#define _SQL_OS_UNIX_VARIANT_HPUX
#elif defined( __linux__ )
#define _SQL_OS_UNIX_VARIANT_LINUX
#elif defined( __APPLE__ )
// (This is also on for iPhone.)
#define _SQL_OS_UNIX_VARIANT_MACOSX
#elif defined( FREEBSD )
#define _SQL_OS_UNIX_VARIANT_FREEBSD
#elif defined( UNDER_VXW )
// n/a
#else
#error Unknown UNIX variant used
#endif
#endif

/****************************************************************************/
/*                           Check for bitness mismatch                     */
/****************************************************************************/

#if ( defined( _SQL_OS_UNIX_VARIANT_LINUX ) && defined( __INTEL_COMPILER ) ) || ( defined( _SQL_OS_UNIX_VARIANT_HPUX ) && ( defined( __HP_cc ) || defined( __HP_aCC ) ) )
#if defined( UNIX64 ) && !defined( __LP64__ )
#error "UNIX64 defined but 64-bit compilation not requested"
#elif !defined( UNIX64 ) && defined( __LP64__ )
#error "64-bit compilation requested but UNIX64 is not defined"
#endif
#endif

#if ( defined( _SQL_OS_UNIX_VARIANT_LINUX ) || defined( _SQL_OS_UNIX_VARIANT_SUN ) ) && ( defined( __SUNPRO_C ) || defined( __SUNPRO_CC ) )
#if defined( UNIX64 ) && ! ( defined( __amd64 ) || defined( __sparcv9 ) )
#error "UNIX64 defined but 64-bit compilation not requested"
#elif !defined( UNIX64 ) && ( defined( __amd64 ) || defined( __sparcv9 ) )
#error "64-bit compilation requested but UNIX64 is not defined"
#endif
#endif

 
/****************************************/
/*  Define a_sql_int32/a_sql_uint32     */
/****************************************/

#if defined( _SQL_OS_UNIX64 ) || defined( _SQL_OS_UNIX_VARIANT_AIX )
    typedef int			a_sql_int32;
    typedef unsigned int	a_sql_uint32;
    typedef long		a_sql_int64;
    typedef unsigned long	a_sql_uint64;
#else
#if defined( _SQL_OS_UNIX_VARIANT_MACOSX )
    typedef int			a_sql_int32;
    typedef unsigned int	a_sql_uint32;
#else
    typedef long		a_sql_int32;
    typedef unsigned long	a_sql_uint32;
#endif
    #if ( defined( _MSC_VER ) && _MSC_VER < 1300 ) || ( defined(__BORLANDC__) && __BORLANDC__ <= 0x0560 )
	typedef __int64			a_sql_int64;
	typedef unsigned __int64	a_sql_uint64;
    #else
	typedef long long		a_sql_int64;
	typedef unsigned long long	a_sql_uint64;
    #endif
#endif


#endif
