/****************************************************************** 
 * 用途: 配置文件读写                                               * 
 * 作者:                                                    * 
 * 日期: 2013-3-28                                                * 
 *****************************************************************/  
  
#ifndef HCONF_H  
#define HCONF_H  
#ifdef WIN32  
#include "../../unistd.h" 
#else
#include <unistd.h> 
#endif

#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <string.h>  
  
typedef struct h_conf{  
    char file_path[200];  
}HConf;  
  
void init_conf(HConf *hconf, char *file_path);  
  
/* 
 * return: -1:fail to open file; 0: can not find the key; 1: success 
 */  
int get_conf_value(HConf *hconf, char *key_name, char *value);  
  
/* 
 * return: -1:fail to open file; 0: can not find the key; 1: success 
 */  
int set_conf_value(HConf *hconf, char *key_name, char *value);  
  
#endif  