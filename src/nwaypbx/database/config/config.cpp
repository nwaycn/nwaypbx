#include "config.h"  
  
void init_conf(HConf *hconf, char *file_path)  
{  
    int fd=open(file_path,O_RDONLY);  
    if(fd>2){   //确保文件存在  
        close(fd);  
        strcpy(hconf->file_path,file_path);  
    }else{  
        perror("can not open config file:");  
    }  
}  
  
int get_conf_value(HConf *hconf, char *key_name, char *value)  
{  
    int res;  
    int fd = open(hconf->file_path,O_RDONLY);  
    if(fd > 2){  
        res = 0;  
        char c;  
        char *ptrk=key_name;  
        char *ptrv=value;  
        while((read(fd,&c,1))==1){  
            if(c == (*ptrk)){  
                do{  
                ptrk ++;  
            read(fd,&c,1);  
                }while(c == (*ptrk));  
        if(c=='='&&(*ptrk)=='\0'){  
            while(1)  
            {  
            read(fd,&c,1);  
            if(c != '\n')  
            {  
                            (*ptrv) = c;  
                ptrv ++;  
            }else{  
                (*ptrv) = '\0';  
                break;  
            }  
            }  
            res = 1;  
            break;  
        }else{  
            do{  
            read(fd,&c,1);  
            }while(c != '\n');  
        }  
        }else{  
        do{  
            read(fd,&c,1);  
        }while(c != '\n');  
        }  
    }  
    close(fd);  
    }else{  
    res = -1;  
    }  
    return res;  
}  
  
int set_conf_value(HConf *hconf, char* key_name, char *value)  
{  
    int res;  
    int fd = open(hconf->file_path,O_RDONLY);  
    if(fd > 2){  
        char buf[800];  //存储文件内容  
        res = 0;  
        char c;  
        char *ptrk=key_name;  
        char *ptrv=value;  
        char *ptrb=buf;  
        while((read(fd,&c,1))==1){  
        *ptrb = c;  
        if(c == *ptrk){  
            do{  
            ptrb ++;  
            ptrk ++;  
            read(fd,&c,1);  
            *ptrb = c;  
        }while(c == *ptrk);  
        if(c == '=' && *ptrk == '\0'){//找到目标行  
            do{  
            ptrb ++;  
            *ptrb = *ptrv;  
            ptrv ++;  
            }while(*ptrv != '\0');   //将value赋值给buf  
            while((read(fd,&c,1)) == 1 && c != '\n');//等号右边原来的内容舍弃  
            ptrb ++;  
            *ptrb = '\n';  
        }else {    //非目标行全部读取  
            while((read(fd,&c,1)) == 1){  
                ptrb ++;  
            *ptrb = c;  
            if(c == '\n'){  
                break;  
            }  
            }  
        }  
        }else{        //非目标行全部读取  
            while((read(fd,&c,1)) == 1){  
            ptrb ++;  
            *ptrb = c;  
            if(c == '\n'){  
                break;  
            }  
        }  
        }  
        ptrb ++;  
    }  
    *ptrb = '\0';  //循环结束，buf末尾置'\0'  
    close(fd);  
    fd=open(hconf->file_path,O_WRONLY|O_TRUNC);//截短打开文件，写入buf  
    if(fd > 2){  
        write(fd,buf,strlen(buf));  
        close(fd);  
        res = 1;  
    }  
    }else{  
    res = -1;  
    }  
    return res;  
}  
