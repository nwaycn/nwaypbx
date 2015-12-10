/*************************************************************************
based for NwayPBX-go
Copyright (C) 2015-, Li hao <lihao@nway.com.cn>
License： GPL
author: Li hao
email: lihao@nway.com.cn
The Initial Developer of the Original Code is
Li hao<lihao@nway.com.cn>
Portions created by the Initial Developer are Copyright (C)
the Initial Developer. All Rights Reserved.
Contributor(s):
**************************************************************************/

package models
 
import (
	_"fmt"
	"errors"
    _"database/sql"
	 _"github.com/lib/pq"
	 
	"github.com/nwaycn/NwayPBX-go/gw_web/controllers/conf"
	"github.com/nwaycn/NwayPBX-go/gw_web/controllers/log"
   
)
 
type User struct {
    
    Username string
    Pwd      string
}
 
/*func getLink() beedb.Model {
    db, err := sql.Open("mysql", "root:root@tcp(192.168.1.81:3306)/test_my?charset=utf8")
    if err != nil {
        panic(err)
    }
    orm := beedb.New(db)
    return orm
}
 */
var DBSTRING string
func GetDbString()(string,bool){
	conf, result := config.SetConfig("conf.ini")
	if result == false {
		logger.Debug("load the config file failed")
		return "",false
	}
	
	
	var connstring string 
	connstring,result = conf.GetValue("postgres","connstring")
	if result == false{
		logger.Error("load database connect string failed");
		return "",false
	}
	DBSTRING = connstring 
	return connstring, true
}

func SaveUser(user User) error {
   /* orm := getLink()
    fmt.Println(user)
    err := orm.Save(&user)*/
	var err error
    return err
}

func ValidateUser2(user User) error{
	//从配置文件中进行用户验证
	conf, result := config.SetConfig("conf.ini")
	if result == false {
		logger.Debug("load the config file failed\n")
		err := errors.New("load the config file failed")
		return err
	}
	var username,password string
	username, result = conf.GetValue("user","username")
	if result == false{
		err := errors.New("管理用户没找到\n")
		logger.Error(err)
		return err
	}
	password, result = conf.GetValue("user","password")
	if result == false{
		err := errors.New("管理用户的密码为空\n")
		logger.Error(err)
		return err
	}
	if username == user.Username && password == user.Pwd{
		return nil
	}
	err := errors.New("用户验证失败\n")
		logger.Error(err)
		return err
}

func ValidateUser(user User) error {
 /* 
    //以下为使用postgresql为数据库进行用户验证的部分   
	connstring , result :=GetDbString()
	if result == false{ return errors.New("数据库连接配置出错!")}
	fmt.Println(connstring)
	DB, err := sql.Open("postgres", connstring)
	
	if err != nil {
		fmt.Println("connect to db failed %v\n",err)
		return err
		}
	defer DB.Close()
	sqlstring := "SELECT man_name FROM gateway_manager where man_name='" + user.Username + "' and man_password='" + user.Pwd + "';"
	fmt.Println(sqlstring)
	rows, serr := DB.Query(sqlstring)
	
	if serr != nil { 
		fmt.Println("query db failed ",serr)
		return serr
	}
	
	defer rows.Close()
	var myuser string
	for rows.Next() {
		
		fmt.Println("to scan a data")
		serr = rows.Scan(&myuser)
		if serr != nil { return serr}
		if len(myuser) >0 {
			fmt.Println("found a valid user", myuser)
			return nil
		}
		
	}
	*/
    return errors.New("没有找到用户或密码错误")
}