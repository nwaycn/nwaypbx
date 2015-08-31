/*
 * Copyright 2008 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

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

