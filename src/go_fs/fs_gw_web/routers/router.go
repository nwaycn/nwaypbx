package routers

import (
	"github.com/nwaycn/fs_gw_web/controllers"
	"github.com/astaxie/beego"
)

func init() {
    beego.Router("/", &controllers.MainController{})
}
