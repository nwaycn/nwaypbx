#ifndef __UTF16_H__
#define __UTF16_H__

#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

#define UTF16_IGNORE_ERROR	0x01

typedef uint16_t UTF16;
typedef uint32_t UTF32;

#ifdef __cplusplus
extern "C" {
#endif

size_t utf16_to_utf32(const UTF16** in, size_t insize, UTF32* out, size_t outsize, int flags);
size_t utf32_to_utf16(const UTF32** in, size_t insize, UTF16* out, size_t outsize, int flags);
size_t utf16_strlen(const UTF16* in);

#ifdef __cplusplus
}
#endif

#endif  // __UTF16_H__
