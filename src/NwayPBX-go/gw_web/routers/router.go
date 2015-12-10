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
package routers

import (
	"github.com/nwaycn/NwayPBX-go/gw_web/controllers"
	"github.com/astaxie/beego"
)

func init() {
    beego.Router("/", &controllers.IndexController{})
    
	beego.Router("/gateways",&controllers.GatewaysController{})
	beego.Router("/gateway",&controllers.GatewayController{})
	beego.Router("/gateway/save", &controllers.GatewayController{}, "post:Save")
	beego.Router("/gateway/save", &controllers.GatewayController{}, "get:Save")
	beego.Router("/gateway/remove", &controllers.GatewayController{}, "post:NwayRemove")
	beego.Router("/gateway/remove", &controllers.GatewayController{}, "get:NwayRemove")
}
