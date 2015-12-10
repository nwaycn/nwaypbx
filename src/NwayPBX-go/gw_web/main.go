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
package main

import (
	_"github.com/nwaycn/NwayPBX-go/gw_web/routers"
	"github.com/astaxie/beego"
	 
	"github.com/nwaycn/NwayPBX-go/gw_web/controllers/log"
	 
)
type MainController struct {
    beego.Controller
}

func main() {
	logger.SetConsole(true)
	logger.SetRollingDaily(".", "nwaypbx.log")
	logger.SetLevel(logger.INFO)
	beego.SessionOn = true
	//beego.()
    
	beego.Run()
}

