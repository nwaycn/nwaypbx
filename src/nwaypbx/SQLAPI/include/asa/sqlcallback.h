// *****************************************************
// Copyright (c) 2001-2011 iAnywhere Solutions, Inc.
// Portions copyright (c) 1988-2011 Sybase, Inc.
// All rights reserved. All unpublished rights reserved.
// *****************************************************

#ifndef _SQLCB_H_INCLUDED
#define _SQLCB_H_INCLUDED
#define II_SQLCB

#include "sqlos.h"

/******************************************************************/
/*  PLATFORM specific definitions                                 */
/******************************************************************/

#if defined( _SQL_OS_WINNT )
    #define _esqlentry_		__stdcall
#endif

#define _fd_ 
#define _sql_ptrchk_(p)	    ((void *) (p))
#define _sql_ptrtypechk_(p,t)   ((t *)p)

#ifndef _esqlentry_
    #define _esqlentry_
#endif

#ifdef __cplusplus
    #define SQLNULL		    0
    #define SQLFARNULL		    0
#else
    #define SQLNULL		    (void *)0
    #define SQLFARNULL		    (void *)0
#endif

/* Windows callback index */
typedef enum {
    DB_CALLBACK_START = 0,
    DB_CALLBACK_WAIT,
    DB_CALLBACK_FINISH,
    DB_CALLBACK_ASYNC_RESPONSE,
    DB_CALLBACK_TRANSLATE_IN,
    DB_CALLBACK_TRANSLATE_OUT,
    DB_CALLBACK_UNKNOWN_EVENT,	    /* For Macintosh, initially */
    DB_CALLBACK_MESSAGE,	    // Messages from the engine during a request
    DB_CALLBACK_CONN_DROPPED,
    DB_CALLBACK_DEBUG_MESSAGE,
    DB_CALLBACK_VALIDATE_FILE_TRANSFER,
    DB_MAX_CALLBACK
} a_db_callback_index;

/* Callback function type */
#define SQL_CALLBACK	    _esqlentry_

typedef int (SQL_CALLBACK *SQL_CALLBACK_PARM)();

/****************************************/
/*  Define sqldatetime                  */
/****************************************/
typedef struct sqldatetime {
    unsigned short	year;		/* e.g. 1992			*/
    unsigned char	month;		/* 0-11				*/
    unsigned char	day_of_week;	/* 0-6  0=Sunday, 1=Monday, ... */
    unsigned short	day_of_year;	/* 0-365			*/
    unsigned char	day;		/* 1-31				*/
    unsigned char	hour;		/* 0-23				*/
    unsigned char	minute;		/* 0-59				*/
    unsigned char	second;		/* 0-59				*/
    a_sql_uint32 	microsecond;	/* 0-999999			*/
} SQLDATETIME;

typedef SQLDATETIME	    DATETIME;

#endif

