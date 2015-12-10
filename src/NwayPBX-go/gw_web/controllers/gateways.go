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

package controllers

import (
	"fmt"
	"github.com/astaxie/beego"
	"github.com/nwaycn/NwayPBX-go/gw_web/gw_op"
	"github.com/nwaycn/NwayPBX-go/gw_web/models"
)

type GatewaysController struct {
	beego.Controller
}

func (this *GatewaysController) Get() {
	sess := this.StartSession()
	username := sess.Get("username")
	fmt.Println(username)
	if username == nil || username == "" {
		this.TplNames = "index.tpl"
	} else {
		//开始处理gateways
		var fsGateway opfsgw.Fs_Gateway
		nwaygws, result := fsGateway.GetAllGw()
		fmt.Println(nwaygws)
		if result {
			this.Data["gateways"] = nwaygws
		}
		this.TplNames = "gateways.tpl"
	}
}

func (index *GatewaysController) Post() {
	sess := index.StartSession()
	var user models.User
	inputs := index.Input()

	user.Username = inputs.Get("username")
	user.Pwd = inputs.Get("pwd")
	//fmt.Println(user)
	err := models.ValidateUser(user)
	if err == nil {
		sess.Set("username", user.Username)
		fmt.Println("username:", sess.Get("username"))
		index.Redirect("/gateways", 302)
		//index.TplNames = "gateways.tpl"
	} else {
		fmt.Println("validate user failed", err)
		index.TplNames = "gateways.tpl"
	}
}
