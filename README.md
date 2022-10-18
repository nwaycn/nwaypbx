# NWayRtcSDK介绍

## iOS版本：

https://github.com/nwaycn/NWayRtcDemo-IOS

## android版本：

https://github.com/nwaycn/NWayRtcDemo_Android

## AMR版本：

https://github.com/nwaycn/NWayRtcDemo_ARM


## 能力及标准：
```
1 支持标准SIP RFC3261协议。多平台支持(andorid,ios. mac ,windows,linux,嵌入式linux arm)。

2 音频：
  A 支持编码解码格式：ISAC、G722、G711 ,L16,OPUS，ILBC
  B 音频前处理3A算法，AEC (Acoustic Echo Cancellation)，ANS (Automatic Noise Suppression)，AGC (Automatic Gain Control)。
C 支持动态抖动缓冲区和错误隐藏（丢包补偿）算法NetEQ。
D 支持JitterBuff，FEC，NACK。

3 视频：
   A 支持编码解码格式：VP8,VP9,H264,H265，支持硬件编解码。
   B 支持QOS：
NACK、FEC、SVC、JitterBuffer、IDR Request、PACER、Sender Side BWE、VFR（动态帧率调整策略）、AVSync（音视频同步）、动态分辨率调整。

 4功能：
   A：支持单人,多人视频通话,视频会议。
   B：支持主流SIP服务器，如：opensips，freeswitch。
   C：支持传输原生编码好的音视频数据。
   D：支持ICE P2P模式和服务器转发模式。
   E：支持移动平台Push Notification，配合我们定制的opensips服务器可以很好的唤醒sdk接受呼叫
   F：服务端支持分布式，集群部署。
```


mobile(wechat) +8618621575908



This project is replaced by FSGUI, FSGUI use Go lang develop,but it is not open source , http://www.6fok.com/topics/33  and https://github.com/nwaycn/FSGui

# nwaypbx
I removed code for database operation ,because the sqlapi library is a business library.

Our plan is to develop a first based on the Chinese PBX, and the world's first c/c++ with ESL control FreeSWITCH as a PBX, it is an open source project.

License： GPL

author: Li hao 

email: lihao@nway.com.cn


You can see demo:  http://121.40.240.104:8088/admin/  ,the username:lihao, password:123456

Now we will develop a new version by GO LANG,It is in https://github.com/nwaycn/nwaypbx/tree/master/src/go_fs/fs_control

Since the v0.1.0 version of the current binary uses a part of the third party's commercial library, it does not conform to the spirit of open source, so we will be it as a  open source pbx from modify we current code

1. need you to install postgresql

you can look this:http://nway.com.cn/t8.html  or this

第一步、安装Postgresql

CentOS 或debian 源码安装 PostgreSQL 9.3.5

安装PostgreSQL前，确认Internet连接正常，以便下载安装文件。

先使用 yum -y update 指令升级系统到最新版本。

本安装将PostgreSQL的数据文件与执行文件分离，如果你打算设置到不同的路径，注意修改对应的执行命令和数据库初始化脚本。

＃ 修改防火墙设置，打开5432端口

vi /etc/sysconfig/iptables -A INPUT -m state --state NEW -m tcp -p tcp --dport 5432 -j ACCEPT

＃ 重启防火墙使新设置生效

service iptables restart

＃ 新增用户组

groupadd postgres

＃ 新增用户

useradd postgres -g postgres

＃ 新建数据库执行文件目录

mkdir -p /usr/local/pgsql

＃ 新建数据库数据文件目录

mkdir -p /db/pgsql/data

＃ 修改目录拥有者

chown -R postgres /usr/local/pgsql/.
chown -R postgres /db/pgsql/data
chown -R postgres /db/pgsql/data/.
＃ 编辑PATH搜索路径

vi /etc/profile
Append these 2 lines to the end of the file:

PATH=/usr/local/pgsql/bin:$PATH
export PATH
＃ 生效PATH搜索路径

source /etc/profile
＃ 安装编译源码所需的工具和库

yum -y install wget gcc readline-devel zlib-devel make
debian

 apt-get install zlibc libghc-zlib-dev libzlcore-dev gcc make git automake
＃ 进入源码压缩包下载目录

cd /usr/src
＃ 下载源码压缩包

wget http://ftp.postgresql.org/pub/source/v9.3.5/postgresql-9.3.5.tar.gz
＃ 解压缩源码包

tar zxvf ./postgresql-9.3.5.tar.gz
＃ 进入解压缩源码目录

cd ./postgresql-9.3.5
＃ 执行源码编译配置脚本

./configure
＃ 编译源码

make
＃ 安装

make install
＃ 变更登录用户

su - postgres
＃ 执行数据库初始化脚本

/usr/local/pgsql/bin/initdb --encoding=utf8 -D /db/pgsql/data
＃ 退出变更登录

exit
＃ 复制PostgreSQL执行脚本

cp /usr/src/postgresql-9.3.5/contrib/start-scripts/linux /etc/init.d/postgresql
＃ 增加执行权限

chmod +x /etc/init.d/postgresql
＃ 编辑PostgreSQL执行脚本，指定数据库文件目录

vi /etc/init.d/postgresql
PGDATA="/db/pgsql/data"
＃ 编辑配置文件，配置可访问数据库的网络地址 (注意别忘了去掉#listen_addresses=前面的#)

vi /db/pgsql/data/postgresql.conf
listen_addresses = '*'
＃ 启动PostgreSQL服务

service postgresql start
＃ 以postgres用户登录数据库，修改postgres用户的数据库密码

psql -U postgres
 postgres=# ALTER USER postgres PASSWORD '123456';
  postgres=# \q
退出pg登录

exit
＃ 编辑配置文件，设置密码md5验证

vi /db/pgsql/data/pg_hba.conf
# "local" is for Unix domain socket connections only
local all all md5
# IPv4 local connections:
#host all all 127.0.0.1/32 trust
host all all 0.0.0.0/0 md5
＃ 重启数据库服务

service postgresql restart
debian下需要安装 chkconfig工具

设置开机自动启动服务

chkconfig postgresql on
配置LD_LIBRARY_PATH vi /etc/profile

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/pgsql/lib
export LD_LIBRARY_PATH

C_INCLUDE_PATH=$C_INCLUDE_PATH:/usr/local/pgsql/include
export C_INCLUDE_PATH
source /etc/profile

2. need you install bakefile

you can look this:http://nway.com.cn/t9.html

3. need you install freeswitch

you can look this:http://nway.com.cn/t4.html
