// *****************************************************
// Copyright (c) 2001-2011 iAnywhere Solutions, Inc.
// Portions copyright (c) 1996-2011 Sybase, Inc.
// All rights reserved. All unpublished rights reserved.
// *****************************************************
/* Include this file to cause 2-byte structure packing. */

#if defined( __WATCOMC__ )
    #pragma pack(__push,2);
#elif defined( _MSC_VER ) && _MSC_VER > 800
    #pragma warning( disable : 4103 )
    #pragma pack( push, pshpk, 2 )
#elif defined( __ALPHA__ ) || defined( _CFE ) // SGI's C preprocessor
    #if !defined( _NO_PRAGMA_PUSH_PACK )
	#define _NO_PRAGMA_PUSH_PACK
    #endif
    #pragma pack(2);
#elif defined( MAC )
    #if defined( __MWERKS__ )
	#pragma options align= mac68k
    #else
        #error "Need a pshpk2.h for this Mac compiler"
    #endif
#elif defined( __BORLANDC__ )
    #pragma option -a2
#elif (defined( __IBMC__ ) && ( __IBMC__ < 500 )) || (defined (__IBMCPP__) && ( __IBMCPP__ < 500 ))
    #pragma pack( push, 2 )
#elif (defined( __SUNPRO_C ) || defined( __SUNPRO_CC )) && ( !defined( ALIGN_WORDS ) || defined( FORCE_PACK ) )
    #if !defined( _PUSH_PACK_FIRST )
        #define _PUSH_PACK_FIRST 2
    #elif !defined( _PUSH_PACK_SECOND )
        #define _PUSH_PACK_SECOND 2
    #else
	#error Tried to push a pack at too great a depth
    #endif
    #pragma pack(2)
#elif defined( HP ) && ( !defined( ALIGN_WORDS ) || defined( FORCE_PACK ) )
    #if !defined( _PUSH_PACK_FIRST )
        #define _PUSH_PACK_FIRST 2
    #elif !defined( _PUSH_PACK_SECOND )
        #define _PUSH_PACK_SECOND 2
    #else
	#error Tried to push a pack at too great a depth
    #endif
    #pragma pack 2
#elif defined( _AIX ) && !defined(__GNUC__)
    #if !defined( _PUSH_PACK_FIRST )
        #define _PUSH_PACK_FIRST twobyte
    #elif !defined( _PUSH_PACK_SECOND )
        #define _PUSH_PACK_SECOND twobyte
    #else
	#error Tried to push a pack at too great a depth
    #endif
    #pragma options align=twobyte
#elif defined( __GNUC__ ) && (!defined( ALIGN_WORDS ) || defined( FORCE_PACK ))
    #pragma pack(push, 2)
#endif

#if defined( _NO_PRAGMA_PUSH_PACK )
    #if defined( _PUSH_PACK )
	#error Tried to push a pack at too great a depth
    #else
	#define _PUSH_PACK
    #endif
#endif
