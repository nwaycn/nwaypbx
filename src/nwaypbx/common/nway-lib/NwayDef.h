#ifndef NWAY_DEFINE_H
#define NWAY_DEFINE_H
#include <time.h>
#ifdef _WIN32
// 64-bit integer
typedef __int64 nway_int64_t;
typedef unsigned __int64 nway_uint64_t;

#else
// 64-bit integer
typedef long long int nway_int64_t;
typedef unsigned long long int nway_uint64_t;

#endif 
#ifndef DWORD
#define DWORD unsigned long
#endif//dword
#ifndef NULL
#define  NULL 0
#endif //NULL
void NwaySleep(int ms);
void get_path(char* aPath);

int file_exist( const char* filename );
long long atoi64(const char* s);
time_t convert_string_to_time_t(const char* time_string);  
#endif
