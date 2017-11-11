// *****************************************************
// Copyright (c) 2001-2011 iAnywhere Solutions, Inc.
// Portions copyright (c) 1996-2011 Sybase, Inc.
// All rights reserved. All unpublished rights reserved.
// *****************************************************
/* Include this file to restore the default structure packing setting. */

#if defined( __WATCOMC__ )
    #pragma pack(__pop);
#elif defined( _MSC_VER ) && _MSC_VER > 800
    #pragma pack( pop, pshpk )
#elif defined( __ALPHA__ ) || defined( _CFE )
    #pragma pack();
#elif defined( __MWERKS__ )
    #pragma options align= reset
#elif defined( __BORLANDC__ )
    #pragma option -a.
#elif (defined( __IBMC__ ) && ( __IBMC__ < 500 )) || (defined (__IBMCPP__) && ( __IBMCPP__ < 500 ) )
    #pragma pack( pop )
#elif (defined( __SUNPRO_C ) || defined( __SUNPRO_CC )) && ( !defined( ALIGN_WORDS ) || defined( FORCE_PACK ) )
    #if defined( _PUSH_PACK_SECOND )
        #undef _PUSH_PACK_SECOND
        #pragma pack (_PUSH_PACK_FIRST)
    #elif defined( _PUSH_PACK_FIRST )
        #undef _PUSH_PACK_FIRST
        #pragma pack( )
    #else
	#error Tried to pop a pack without a successful push
    #endif
#elif defined( HP ) && ( !defined( ALIGN_WORDS ) || defined( FORCE_PACK ) )
    #if defined( _PUSH_PACK_SECOND )
        #undef _PUSH_PACK_SECOND
        #pragma pack _PUSH_PACK_FIRST
    #elif defined( _PUSH_PACK_FIRST )
        #undef _PUSH_PACK_FIRST
        #pragma pack
    #else
	#error Tried to pop a pack without a successful push
    #endif
#elif defined( _AIX ) && !defined(__GNUC__)
    #if defined( _PUSH_PACK_SECOND )
        #undef _PUSH_PACK_SECOND
        #pragma options align=_PUSH_PACK_FIRST
    #elif defined( _PUSH_PACK_FIRST )
        #undef _PUSH_PACK_FIRST
        #pragma options align=reset
    #else
	#error Tried to pop a pack without a successful push
    #endif
#elif defined( __GNUC__ ) && (!defined( ALIGN_WORDS ) || defined( FORCE_PACK ))
    #pragma pack(pop)
#endif

#if defined ( _NO_PRAGMA_PUSH_PACK )
    #if defined( _PUSH_PACK )
	#undef _PUSH_PACK
    #else
	#error Tried to pop a pack without a successful push
    #endif
#endif
