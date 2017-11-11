// *****************************************************
// Copyright (c) 2001-2011 iAnywhere Solutions, Inc.
// Portions copyright (c) 1988-2011 Sybase, Inc.
// All rights reserved. All unpublished rights reserved.
// *****************************************************

#ifndef _SQLCA_H_INCLUDED
#define _SQLCA_H_INCLUDED
#define II_SQLCA

#include "sqlos.h"
#ifndef UNDER_PALM_OS_GARNET
    #include <stddef.h>
#endif

/******************************************************************/
/*  Define _SQL32 or _SQL64 based on target operating-system.     */
/******************************************************************/

#if defined( _SQL_OS_UNIX64 )
    #define _SQL64
#else
    #define _SQL32
#endif


/******************************************************************/
/*  Define _sqlfar macro (definition left for compatibility reasons */
/******************************************************************/

#define _sqlfar
#define _sqlfar_isfar	0

/*********************************************/
/* Version check for sqlpp-to-dblib interface*/
/*********************************************/

typedef unsigned short          a_sqlpp_version_number;

#define SQLPP_DBLIB_VERSION_INITIAL     1
#define SQLPP_DBLIB_VERSION_WSQL32      2
#define SQLPP_DBLIB_VERSION_REENTRANCY  3
#define SQLPP_DBLIB_VERSION_WSQL40      4
#define SQLPP_DBLIB_VERSION_SA50        4
#define SQLPP_DBLIB_VERSION_ASA6        6
#define SQLPP_DBLIB_VERSION_ASA7        7
#define SQLPP_DBLIB_VERSION_ASA8        8
#define SQLPP_DBLIB_VERSION_ASA9        9
#define SQLPP_DBLIB_VERSION_SA10        10
#define SQLPP_DBLIB_VERSION_SA11        11
#define SQLPP_DBLIB_VERSION_SA12        12
#define SQLPP_DBLIB_VERSION	        12

#if defined( _SQL_SQLPP_VERSION_MAJOR )  \
&&  _SQL_SQLPP_VERSION_MAJOR != SQLPP_DBLIB_VERSION
    #error Incorrect version of SQLPP used for this header file
#endif

/******************************************************************/
/*  Define structure packing macro based on environment           */
/******************************************************************/

#if ( defined( _SQL_OS_WINNT ) && !defined( UNDER_CE ) && !defined( _WIN64 ) ) \
||  defined( _SQL_OS_NETWARE )
    #define _SQL_PACK_STRUCTURES    1
#endif

/******************************************************************/
/*  Structure definitions                                         */
/******************************************************************/

#if defined( _SQL_PACK_STRUCTURES )
    #if defined( _MSC_VER ) && _MSC_VER > 800
	#pragma warning(push)
        #pragma warning(disable:4103)
    #endif
    #include "pshpk1.h"
#endif

typedef short		a_sql_len;
typedef unsigned short	a_sql_ulen;

typedef a_sql_int32     an_sql_code;
typedef char            an_sql_state[6];

struct sqlwarn{
    unsigned char   sqlwarn0;
    unsigned char   sqlwarn1;
    unsigned char   sqlwarn2;
    unsigned char   sqlwarn3;
    unsigned char   sqlwarn4;
    unsigned char   sqlwarn5;
    unsigned char   sqlwarn6;
    unsigned char   sqlwarn7;
    unsigned char   sqlwarn8;
    unsigned char   sqlwarn9;
};

#if defined( _SQL_PACK_STRUCTURES )
    #include "poppk.h"
    /* The SQLCA should be 4-byte aligned */
    #include "pshpk4.h"
#endif

#ifdef __cplusplus
typedef struct _sqlca{
#else
typedef struct sqlca{
#endif
    unsigned char       sqlcaid[8];
    a_sql_int32         sqlcabc;
    an_sql_code         sqlcode;
    short               sqlerrml;
    unsigned char       sqlerrmc[70];
    unsigned char       sqlerrp[8];
    a_sql_int32         sqlerrd[6];
    struct sqlwarn	sqlwarn;
    an_sql_state	sqlstate;
} SQLCA;

#ifndef SQLCAPTR
    #define SQLCAPTR	    sqlcaptr
#endif

#ifndef SQLCODE
    #define SQLCODE         SQLCAPTR->sqlcode
    #define SQLWARN0        SQLCAPTR->sqlwarn.sqlwarn0
    #define SQLWARN1        SQLCAPTR->sqlwarn.sqlwarn1
    #define SQLWARN2        SQLCAPTR->sqlwarn.sqlwarn2
    #define SQLWARN3        SQLCAPTR->sqlwarn.sqlwarn3
    #define SQLWARN4        SQLCAPTR->sqlwarn.sqlwarn4
    #define SQLWARN5        SQLCAPTR->sqlwarn.sqlwarn5
    #define SQLWARN6        SQLCAPTR->sqlwarn.sqlwarn6
    #define SQLWARN7        SQLCAPTR->sqlwarn.sqlwarn7
    #define SQLWARN8        SQLCAPTR->sqlwarn.sqlwarn8
    #define SQLWARN9        SQLCAPTR->sqlwarn.sqlwarn9
    #define SQLSTATE        SQLCAPTR->sqlstate
    
    #define SQLIOCOUNT_F    sqlerrd[1]
    #define SQLCOUNT_F      sqlerrd[2]
    #define SQLIOESTIMATE_F sqlerrd[3]

    #define SQLIOCOUNT      SQLCAPTR->SQLIOCOUNT_F
    #define SQLCOUNT        SQLCAPTR->SQLCOUNT_F
    #define SQLIOESTIMATE   SQLCAPTR->SQLIOESTIMATE_F
#endif

#if defined( _SQL_PACK_STRUCTURES )
    #include "poppk.h"
    /* Everything else must be single-byte aligned */
    #include "pshpk1.h"
#endif

#define DECL_NCHAR char

#define DECL_VARCHAR( size )  \
        struct { a_sql_ulen len; \
                 unsigned char array[size+1];  \
        } 

#define DECL_NVARCHAR( size )  \
        struct { a_sql_ulen len; \
                 unsigned char array[size+1];  \
        } 

#define DECL_BINARY( size )  \
        struct { a_sql_ulen len; \
                 unsigned char array[size];  \
        } 

#define DECL_LONGVARCHAR( size )  \
	struct { a_sql_uint32	array_len;  \
		 a_sql_uint32	stored_len;  \
		 a_sql_uint32	untrunc_len;  \
		 char		array[size+1];  \
	}

#define DECL_LONGNVARCHAR( size )  \
	struct { a_sql_uint32	array_len;  \
		 a_sql_uint32	stored_len;  \
		 a_sql_uint32	untrunc_len;  \
		 char		array[size+1];  \
	}

#define DECL_LONGBINARY( size )  \
	struct { a_sql_uint32	array_len;  \
		 a_sql_uint32	stored_len;  \
		 a_sql_uint32	untrunc_len;  \
		 char		array[size];  \
	}

#define DECL_DECIMAL( prec, scale )  \
        struct { char   array[ ((prec)/2) + 1 ]; \
        }

#define DECL_FIXCHAR( size )  \
        struct { char   array[ size ]; \
	}

#define DECL_NFIXCHAR( size )  \
        struct { char   array[ size ]; \
	}

#define DECL_DATETIME  \
        struct sqldatetime

#define DECL_BIT \
        unsigned char

#if defined(__WATCOMC__) || defined(_MSC_VER) || defined(__BORLANDC__)

#define DECL_BIGINT             __int64
#define DECL_UNSIGNED_BIGINT    unsigned __int64

#elif defined(_SQL64)

#define DECL_BIGINT             long
#define DECL_UNSIGNED_BIGINT    unsigned long

#else

#define DECL_BIGINT             long long
#define DECL_UNSIGNED_BIGINT    unsigned long long

#endif

typedef struct VARCHAR {
    a_sql_ulen		len;
    char                array[1];
} VARCHAR;

typedef struct NVARCHAR {
    a_sql_ulen		len;
    char                array[1];
} NVARCHAR;

typedef struct BINARY {
    a_sql_ulen		len;
    char                array[1];
} BINARY;

typedef struct LONGVARCHAR {
    a_sql_uint32	array_len;  /* number of allocated bytes in array */
    a_sql_uint32	stored_len; /* number of bytes stored in array
				     * (never larger than array_len) */
    a_sql_uint32	untrunc_len;/* number of bytes in untruncated expression
				     * (may be larger than array_len) */
    char		array[1];   /* the data */
} LONGVARCHAR, LONGNVARCHAR, LONGBINARY;

/* used by LONGVARCHARSIZE and LONGBINARYSIZE */
#define _SQL_OFFSETOF(__typ,__id) ((size_t)&(((__typ*)0)->__id))

/* number of bytes to allocate for LONGVARCHAR or LONGBINARY structure
 * with n bytes in array */
#define LONGVARCHARSIZE( n )	( _SQL_OFFSETOF( LONGVARCHAR, array[0] ) + n )
#define LONGNVARCHARSIZE( n )	LONGVARCHARSIZE( n )
#define LONGBINARYSIZE( n )	LONGVARCHARSIZE( n )

typedef struct TYPE_DECIMAL {
    char                array[1];
} TYPE_DECIMAL;

typedef short int       a_sql_statement_number;

/* Supported values for port_type in a_server_address
   (used by db_locate_servers) */
#define PORT_TYPE_TCP     6

typedef struct a_server_address {
    a_sql_uint32	port_type;	/* Only returns PORT_TYPE_TCP  */
    a_sql_uint32	port_num;       /* TCP Port number             */
    char 		*name;		/* Name of the database engine */
    char 		*address;	/* IP address                  */
    char		*dbname;	/* Database name               */
} a_server_address;

/* Supported flags for db_lookup_servers_ex */
#define DB_LOOKUP_FLAG_NUMERIC			0x0001
#define DB_LOOKUP_FLAG_DATABASES		0x0002
#define DB_LOOKUP_FLAG_ADDRESS_INCLUDES_PORT	0x0004

/* Supported flags for fill_sqlda_ex */
#define FILL_SQLDA_FLAG_RETURN_DT_LONG		0x0001

struct a_dblib_info;

struct sqlda;

#include "sqlcallback.h"

#if defined( _SQL_PACK_STRUCTURES )
    #include "poppk.h"
    #if defined( _MSC_VER ) && _MSC_VER > 800
        #pragma warning(pop)
    #endif
#endif

/******************************************************************/
/*  Define __SMALLDATA__ based on other compiler macros           */
/******************************************************************/

#ifndef __SMALLDATA__
    #ifdef M_I86SM
	#define __SMALLDATA__
    #endif
    #ifdef M_I86MM
	#define __SMALLDATA__
    #endif
    #ifdef __TURBOC__
	#if defined( __TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
	    #define __SMALLDATA__
	#endif
    #endif
#endif


/******************************************************************/
/*  PLATFORM specific definitions                                 */
/******************************************************************/

typedef enum {
    SQLDA_CLEAR,
    SQLDA_NOCLEAR,
    SQLDA_NOEXEC
} a_descriptor_behaviour;

typedef enum {
    SQLDA_TYPE,
    SQLDA_LENGTH,
    SQLDA_RETURNED_LENGTH,
    SQLDA_PRECISION,
    SQLDA_SCALE,
    SQLDA_NULLABLE,
    SQLDA_INDICATOR
} a_descriptor_field;

typedef enum {
    DB_PROP_SERVER_ADDRESS,
    DB_PROP_DBLIB_VERSION,
    DB_PROP_CLIENT_CHARSET
} a_db_property;

typedef enum {
    DB_SET_PROP_CLIENT_API
} a_db_set_property;

/* Standard functions emitted by sqlpp require covers for some platforms */
#ifdef UNDER_PALM_OS_GARNET
    #ifndef memcpy
	#define memcpy	MemMove
    #endif
#endif

#ifndef II_SQL_FUNCS
#define II_SQL_FUNCS

#ifdef __cplusplus
    extern "C" {
#endif

/* external sqlca data structure */

extern SQLCA		sqlca;
extern SQLCA		*sqlcaptr;

/* Macros for multiple sqlca management */

#define db_set_sqlca( sqlca )	    sqlcaptr = sqlca
#define db_get_sqlca()		    sqlcaptr


/* Generate function pointers or function prototypes */

#ifndef UNDER_UL_SQLPP	/* UltraLite uses its own sqlpp function headers*/
#define UNDER_ASA_SQLPP
#ifndef _NO_FUNC_INFO
#ifdef _USE_ESQLDLL
#define FUNC_INFO( scope, rettype, call, fname, parms ) \
        extern rettype ( call * fname ) parms;
#include "sqlfuncs.h"
#undef FUNC_INFO
#else
#define FUNC_INFO( scope, rettype, call, fname, parms ) \
        scope rettype call fname parms;
#include "sqlfuncs.h"
#undef FUNC_INFO
#endif //_USE_ESQLDLL
#endif //_NO_FUNC_INFO
#endif //UNDER_UL_SQLPP
	
#ifdef __cplusplus
    }
#endif

#endif

#endif

