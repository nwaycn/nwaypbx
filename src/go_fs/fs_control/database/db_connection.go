package database

import (
	"database/sql"
	"fmt"
	 
	"github.com/astaxie/beego/config"
	_ "github.com/lib/pq"
	"log"
	_ "net"
	_ "net/http"
	_ "runtime"
	_ "time"
	"sync"
 
)

type NwayConnect struct{
	NwayDb *sql.DB
	
}

var _init_db sync.Once
var _instance_ *NwayConnect

func New() *NwayConnect{
	_init_db.Do(func(){
		_instance_ = new(NwayConnect)
		
	})
	return _instance_
}

func (this *NwayConnect) Init() bool{
//	result := true
	var err error
	iniconf, errc := config.NewConfig("ini", "restconf.conf")
	//var result int64
	if errc != nil {
		//log.Fatal(errc)
		fmt.Println("config file fatal")
		return false
	}
	dbstring := iniconf.String("database")
	
	this.NwayDb, err = sql.Open("postgres", dbstring)
	this.NwayDb.SetMaxIdleConns(4)
	this.NwayDb.SetMaxOpenConns(10)
	if err != nil {
		log.Fatalf("Got error when connect database, the error is '%v'", err)
		fmt.Println(dbstring)
		return false
	}
	return true
}

func (this *NwayConnect)GetConn() *sql.DB{
	//保证所有的连接都是有效的
	strsql := "select now();"
	rows, err := this.NwayDb.Query(strsql)
	if err != nil {
		 
		log.Fatal("failed to check_user")
		log.Fatal(err)
		return nil
	}
	rows.Close()
	return this.NwayDb
}

