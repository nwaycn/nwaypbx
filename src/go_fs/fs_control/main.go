package main

import (
	_ "fs_control/database"
)

func main(){
	var dbinstance *NwayConnect =nil
	dbinstance = New()
	dbinstance.Init()
}