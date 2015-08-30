# -*-coding:utf-8 -*-
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
def ConfigBasePath():
    sql = 'select count(id) from call_base_config where config_name=\'base_path\';'
    conn = GetDbConn()
    cur = conn.cursor()
    cur.execute(sql)
    rows = cur.fetchone()
    conn.commit()
    rownum = rows[0]
    curr_path = GetCurrentPath()
    if rownum > 0 :
        print('has any row')
        sql = 'update call_base_config set config_param=\'' + curr_path + '\' where config_name=\'base_path\';'
    else:
        print('no row')
        sql = 'insert into call_base_config(id,config_name,config_param)values(1,\'base_path\',\'' + curr_path + '\');'
    cur.close()
    cur = conn.cursor()
    cur.execute(sql)
    conn.commit()
    cur.close()

def ParseCvsFile(filename, groupid, rowid):
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
                    print('is a local number')
		    #代表是本地号码
                    pass
                else:
                    line = '0' + line
                print('is a long number')
                querycur.close()
                querysql = 'select count(id) from call_out_numbers where group_id=' + "%d" % groupid + ' and number=\'' + line + '\' and call_out_task_id=' +"%d" % rowid
                print(querysql)
                querycur = conn.cursor()
                querycur.execute(querysql)
                cnt = querycur.fetchone()
                conn.commit()
                querycur.close()
                if long(cnt[0]) > 0 :
                    #代表找到了该号码，则需要继续下一个
	            print('query a line >0 ,then continue')
                    #querycur.close()
                    continue

                insertsql = 'SELECT insert_out_number(' + "%d" % groupid + ',\'' + line + '\',' + "%d" % rowid +',0);'
                print(insertsql)
                querycur = conn.cursor()
                querycur.execute(insertsql)
                conn.commit()
                count += 1
                querycur.close()
            else:
                pass
        #读完一个上传的csv文件，更新这次提交的外呼的号码数量
    updatecur = conn.cursor()
    print(rowid)
    updatesql = 'update call_group_callout set total_number=' + "%d" % count + ', wait_number=' + "%d" % count+',has_parse_from_file=TRUE where id=' "%d" % rowid + ';'
    updatecur.execute(updatesql)
    conn.commit()
    print('processed success!')
    updatecur.close()
    conn.close()



def ParseCvsFromDb():
    querysql = 'select id,number_group_uploadfile,number_group_id from call_group_callout where has_parse_from_file=FALSE;'
    conn = GetDbConn()
    cur = conn.cursor()
    cur.execute(querysql)
    rows = cur.fetchall()
    conn.commit()
    groupid = 0
    for row in rows:
        rowid =  row[0]
        try:
            groupid = long(row[2])
        except Exception,ex:
            #失败的话，可以重新添加一个组
            strid = time.strftime("%Y%m%d%H%M%S") + "%d" % random.randint(0,100)
            groupid = long(strid)
            #更新callout表
            insertsql = 'insert into call_group(id,group_name)values(' + strid + ',\'' + strid + '\');'
            curist = conn.cursor
            curist.execute(insertsql)
            cur.close()
            updatesql = 'update call_group_callout set number_group_id=' + strid + ' where id=' + "%d" % rowid + ';'
            cur = conn.cursor()
            cur.execute(updatesql)

            conn.commit()
            cur.close()
            curist.close()

        ParseCvsFile(row[1], groupid, rowid)
    cur.close()
    conn.close()

if __name__ == '__main__':
    ConfigBasePath()
    #str='python- String function'
    #print '%s startwith t=%s' % (str,str.startswith('t'))
    #print '%s' % (str.replace('-',''))
    while True:

        #break
        try:
            ParseCvsFromDb()
        except Exception, ex:
            print Exception, ":", ex
        time.sleep(2)
