# -*-coding:utf-8 -*-
#导入号码段，必须是csv文件，只有号码段一个字段
__author__ = 'lihao'
import os,sys
import psycopg2
import time
import csv
import random

def GetCurrentPath():
    return os.getcwd()
def GetDbConn():
    conn = psycopg2.connect(database="nwaycc", user="postgres", password="nway2013", host="127.0.0.1", port="5432")
    return conn


def ParseCvsFile(filename):
   # print(filename)
    ext = filename.split('.')[-1]
    count = 0
    conn = GetDbConn()
    #只处理csv后缀名的文件
    if ext == 'csv':
        fullFileName = GetCurrentPath() + '/' + filename #全路径
        print('init reader')
        reader = csv.reader(open(fullFileName, 'rb'))
        #reader = csv.reader(file(fullFileName), 'rb')
        print('read from csv file')
        for csvline in reader:
            line = '-'.join(csvline)
            print('---------------------------------------------------------------------------------------')
            line.replace('-', '')
            if line.isdigit():
                #全是数字才会走下去
                c = line[0:1]
                #不管首字母是什么都一样去查询号码段
                line.lstrip('0')
                #if c == '0':
                    #首字母为0则去掉0
                    #line
                #else:
                    #查找是本地还是外地的
                print(line)
                numpara = line[0:7]
                querycur = conn.cursor()
                querysql = 'select count(number_paragraph) from number_paragraph where number_paragraph=\'' + numpara + '\''
                querycur.execute(querysql)
                row = querycur.fetchone()
                conn.commit()
                #queryresult =''.join(row[0])
                if long(row[0]) >0:
                    print('has a number_paragraph')
		    #代表是本地号码
                    pass
                else:
                    line = line
#		    print('is a long number')
                querycur.close()
               
                insertsql = 'insert into number_paragraph(number_paragraph)values(\'' + line + '\');'
                print(insertsql)
                querycur = conn.cursor()
                querycur.execute(insertsql)
                conn.commit()
                count += 1
                querycur.close()
            else:
                pass
        #读完一个上传的csv文件，更新这次提交的外呼的号码数量
    
    conn.close()



def ParseCvsFromFile(filename):
     
    conn = GetDbConn()
    
    ParseCvsFile(filename)


if __name__ == '__main__':
    #ConfigBasePath()
    #str='python- String function'
    #print '%s startwith t=%s' % (str,str.startswith('t'))
    #print '%s' % (str.replace('-',''))
    #while True:

        #break
    try:
        ParseCvsFromFile('number.csv')
    except Exception, ex:
        print Exception, ":", ex
    time.sleep(2)
