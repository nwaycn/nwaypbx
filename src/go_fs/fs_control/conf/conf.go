package config
//单例模式，用于加快访问速度
import (
	//"go_fs/fs_control/conf"
	"go_fs/fs_control/log"
	"sync"
	
	//"fmt"
)

type DbConf struct{
	connString string
	
}

type FsConf struct{
	fsHost string
	fsPort int
	fsAuth string
	fsTimeout int
	
}

type FsOutboundConf struct{
	obPort int
}

type NwayConfig struct{
	dbconf DbConf
	fsconf FsConf
	fsoutboundconf FsOutboundConf
	
}

var _init_conf sync.Once
var _instanceconf_ *NwayConfig
var isLoad bool = false

func LoadConf() (*NwayConfig,bool){
	_init_conf.Do(func(){
		_instanceconf_ = new(NwayConfig)
		isLoad = _instanceconf_.Load()
		
	})
	return _instanceconf_ ,isLoad
}

func (this *NwayConfig)Load() bool{
	conf, result := SetConfig("conf.ini")
	if result == false {
		logger.Debug("load the config file failed")
		return false
	}

	
	this.dbconf.connString,result = conf.GetValue("database","connstring")
	if result == false{
		logger.Error("load database connect string failed");
		return false
	}
	this.fsconf.fsHost,result = conf.GetValue("freeswitch", "host")
	if result == false{
		logger.Error("load freeswitch host string failed");
		return false
	}
	this.fsconf.fsAuth,result = conf.GetValue("freeswitch","auth")
	if result == false{
		logger.Error("load freeswitch auth string failed");
		return false
	}
	this.fsconf.fsPort = conf.GetInt("freeswitch","port", 8021)
	 
	this.fsconf.fsTimeout =conf.GetInt("freeswitch", "timeout" , 500)
	 
	this.fsoutboundconf.obPort  =  conf.GetInt("freeswitch", "outboundport", 8040) 
	 
	return true
	
}
func (this *NwayConfig)GetConnStr()string{
	return this.dbconf.connString
}