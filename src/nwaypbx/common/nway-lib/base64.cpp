/*
 * Copyright 2008 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */
#include "stdafx.h"
#include <stdlib.h>
#include <string.h>

#include "base64.hpp"

static const char base64o[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64i[81] = "\76XXX\77\64\65\66\67\70\71\72\73\74\75XXXXXXX\00\01\02\03\04\05\06\07\10\11\12\13\14\15\16\17\20\21\22\23\24\25\26\27\30\31XXXXXX\32\33\34\35\36\37\40\41\42\43\44\45\46\47\50\51\52\53\54\55\56\57\60\61\62\63";

Base64EncodedBuffer ::Base64EncodedBuffer( const void * plainBuffer, size_t len )
{
	mLength = len * 4 / 3 + 16;
	mBuffer = (char*)malloc( mLength );
	memset( mBuffer, 0, mLength );

	char * s = (char*)plainBuffer;
	size_t n = len;

	char * t = mBuffer;

	register size_t i;
	register unsigned long m;

	t[0] = '\0';
	for (; n > 2; n -= 3, s += 3) {
		m = ((unsigned long)((unsigned char*)s)[0] << 16) |
				((unsigned long)((unsigned char*)s)[1] << 8) |
				(unsigned long)((unsigned char*)s)[2];
		for (i = 4; i > 0; m >>= 6)
			t[--i] = base64o[m & 0x3F];
		t += 4;
	}
	t[0] = '\0';
	if (n > 0) {
		m = 0;
		for (i = 0; i < n; i++)
			m = (m << 8) | *s++;
		for (; i < 3; i++)
			m <<= 8;
		for (i++; i > 0; m >>= 6)
			t[--i] = base64o[m & 0x3F];
		for (i = 3; i > n; i--)
			t[i] = '=';
		t[4] = '\0';
	}

	mLength = strlen( mBuffer );
}

Base64EncodedBuffer :: ~Base64EncodedBuffer()
{
	free( mBuffer );
	mBuffer = NULL;
}

const char * Base64EncodedBuffer :: getBuffer()
{
	return mBuffer;
}

size_t Base64EncodedBuffer :: getLength()
{
	return mLength;
}

//-------------------------------------------------------------------

Base64DecodedBuffer :: Base64DecodedBuffer( const char * encodedBuffer, size_t len )
{
	mLength = len * 3 / 4 + 16;
	mBuffer = (char*)malloc( mLength );
	memset( mBuffer, 0, mLength );

	const char * s = encodedBuffer;
	char * t = mBuffer;
	size_t l = len;
	size_t * n = &mLength;

	register int i, j, c;
	register unsigned long m;

	if (n) *n = 0;

	for (;;) {
		for (i = 0; i < 256; i++) {
			m = 0;
			j = 0;
			while (j < 4) {
				c = *s++;
				if (c == '=' || !c) {
					i *= 3;
					switch (j)
					{
						case 2:
							*t++ = (char)((m >> 4) & 0xFF);
							i++;
							break;
						case 3:
							*t++ = (char)((m >> 10) & 0xFF);
							*t++ = (char)((m >> 2) & 0xFF);
							i += 2;
					}
					if (n) *n += i;
					return;
				}
				c -= '+';
				if (c >= 0 && c <= 79) {
					m = (m << 6) + base64i[c];
					j++;
				}
			}
			*t++ = (char)((m >> 16) & 0xFF);
			*t++ = (char)((m >> 8) & 0xFF);
			*t++ = (char)(m & 0xFF);
			if (l < 3) {
				if (n) *n += i;
				return;
			}
			l -= 3;
		}
		if (n) *n += 3 * 256;
	}
}

Base64DecodedBuffer :: ~Base64DecodedBuffer()
{
	free( mBuffer );
	mBuffer = NULL;
}

const void * Base64DecodedBuffer :: getBuffer()
{
	return mBuffer;
}

size_t Base64DecodedBuffer :: getLength()
{
	return mLength;
}

