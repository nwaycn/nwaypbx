package main

import (
	 "go_fs/fs_control/database"
	 "go_fs/fs_control/log"
)

func main(){
	//initlize the logger file
	logger.SetConsole(true)
	logger.SetRollingDaily(".", "nwaypbx.log")
	logger.SetLevel(logger.INFO)
	//initlize the database
	var dbinstance *database.NwayConnect =nil
	dbinstance = database.NewDb()
	bres := dbinstance.Init()
	if bres==true {
		logger.Info("init db successed!")
	}else{
		logger.Info("init db failed!")
	}
	
}