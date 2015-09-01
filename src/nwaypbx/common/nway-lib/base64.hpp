/*************************************************************************
based for NwayPBX
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License£º GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/

#ifndef __base64_hpp__
#define __base64_hpp__

#include <sys/types.h>

class Base64EncodedBuffer {
public:
	Base64EncodedBuffer( const void * plainBuffer, size_t len );
	~Base64EncodedBuffer();

	const char * getBuffer();
	size_t getLength();

private:
	char * mBuffer;
	size_t mLength;
};

class Base64DecodedBuffer {
public:
	Base64DecodedBuffer( const char * encodedBuffer, size_t len );
	~Base64DecodedBuffer();

	const void * getBuffer();
	size_t getLength();

private:
	char * mBuffer;
	size_t mLength;
};

#endif

