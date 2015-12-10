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
	"github.com/nwaycn/NwayPBX-go/gw_web/models"
)

type IndexController struct {
	beego.Controller
}

func (index *IndexController) Get() {
	sess := index.StartSession()
	username := sess.Get("username")
	fmt.Println(username)
	if username == nil || username == "" {
		index.TplNames = "index.tpl"
	} else {
		index.TplNames = "success.tpl"
	}

}

func (index *IndexController) Post() {
	sess := index.StartSession()
	var user models.User
	inputs := index.Input()

	user.Username = inputs.Get("username")
	user.Pwd = inputs.Get("pwd")
	//fmt.Println(user)
	//err := models.ValidateUser(user)
	err := models.ValidateUser2(user)
	if err == nil {
		sess.Set("username", user.Username)
		fmt.Println("to gateways username:", sess.Get("username"))
		index.Redirect("/gateways", 302)
		//index.TplNames = "gateways.tpl"
	} else {
		fmt.Println("validate user failed", err)
		index.Data["error"] = "用户登录错误"
		index.Redirect("/gateways", 302)
	}
}
