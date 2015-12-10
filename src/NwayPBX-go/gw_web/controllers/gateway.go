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
	 
)

type GatewayController struct {
	beego.Controller
}

func (this *GatewayController) Get() {
	gateway_name := this.Input().Get("gatewayname")

	//this.Input().Get("gateway_name")   //this.GetString("gateway_name")
	sess := this.StartSession()
	username := sess.Get("username")
	if username == nil || username == "" {
		this.TplNames = "index.tpl"
	} else {

		if len(gateway_name) > 0 {
			this.Data["operate"] = "编辑"
			var fsGateway opfsgw.Fs_Gateway
			gateway := fsGateway.GetAGateway(gateway_name)
			//gateway.Gw_name = gateway.GetGatewayName()
			fmt.Println("the name:   ---", gateway.GetGatewayName())
			//this.Data["gateway"]= gateway
			this.Data["gateway_name"] = gateway.GetGatewayName()
			this.Data["username"] = gateway.GetUsername()
			this.Data["realm"] = gateway.GetRealm()
			this.Data["from_user"] = gateway.GetFromUser()
			this.Data["from_domain"] = gateway.GetFromDomain()
			this.Data["password"] = gateway.GetPassword()
			this.Data["extension"] = gateway.GetExtension()
			this.Data["proxy"] = gateway.GetProxy()
			this.Data["expire_seconds"] = gateway.GetExpireSeconds()
			this.Data["register"] = gateway.GetRegister()
			this.Data["register_transport"] = gateway.GetRegisterTransport()
			this.Data["retry_seconds"] = gateway.GetRetrySeconds()
			this.Data["caller_id_in_from"] = gateway.GetCallerIdInFrom()
			this.Data["contact_params"] = gateway.GetContactParams()
			this.Data["ping"] = gateway.GetPing()

			this.Data["register_proxy"] = gateway.GetRegisterProxy()

		} else {
			this.Data["operate"] = "新建"
		}

		this.TplNames = "gateway.tpl"
	}
}

// @Title Post
// @Description create Doctors
// @Param   body        body    models.Doctors  true        "body for Doctors content"
// @Success 200 {int} models.Doctors.Id
// @Failure 403 body is empty

func (this *GatewayController) Post() {

}

func (this *GatewayController) NwayRemove() {

	//fmt.Println("this is the gateway remove controller")
	//fmt.Println(this.Ctx.Input.Params[":gatewayname"])
	sess := this.StartSession()
	username := sess.Get("username")
	if username == nil || username == "" {
		this.TplNames = "index.tpl"
	} else {
		inputs := this.Input()
		fmt.Println(inputs)
		gateway_name := inputs.Get("gatewayname")
		var fsGateway opfsgw.Fs_Gateway
		if len(gateway_name) > 0 {
			bres, err := fsGateway.EarseGateway(gateway_name)
			if bres == false {
				fmt.Println(err)
				this.Data["json"] = err.Error()
			}
			this.Data["json"] = "ok"
		}
	}
	this.ServeJson()

}

func (this *GatewayController) Save() {
	sess := this.StartSession()
	username := sess.Get("username")
	if username == nil || username == "" {
		this.TplNames = "index.tpl"
	} else {
		inputs := this.Input()
		var fsGateway opfsgw.Fs_Gateway
		fsGateway.SetCallerIdInFrom(inputs.Get("caller_id_in_from"))
		fsGateway.SetContactParams(inputs.Get("contact_params"))
		fsGateway.SetExpireSeconds(inputs.Get("expire_seconds"))
		fsGateway.SetExtension(inputs.Get("extension"))
		fsGateway.SetFromDomain(inputs.Get("from_domain"))
		fsGateway.SetFromUser(inputs.Get("from_user"))
		fsGateway.SetGatewayName(inputs.Get("gateway_name"))
		fsGateway.SetPassword(inputs.Get("password"))
		fsGateway.SetPing(inputs.Get("ping"))
		fsGateway.SetProxy(inputs.Get("proxy"))
		fsGateway.SetRealm(inputs.Get("realm"))
		fsGateway.SetRegister(inputs.Get("register"))
		fsGateway.SetRegisterProxy(inputs.Get("register_proxy"))
		fsGateway.SetRegisterTransport(inputs.Get("register_transport"))
		fsGateway.SetRetrySeconds(inputs.Get("retry_seconds"))
		fsGateway.SetUsername(inputs.Get("username"))
		if len(fsGateway.GetGatewayName()) > 1 {
			bres, err := fsGateway.SaveFile()
			if bres == false {
				fmt.Println(err)
			}
		}
	}
	this.Redirect("/gateways", 302)

}
