package opfsgw
//不管freeswitch 如何都会重新rescan,失败不影响操作
import (
	"sync"
	"encoding/xml"
	"path/filepath"
	"path"
	"reflect"
	"fmt"
	"errors"
	_"fmt"
	_"encoding/xml"
    _ "io/ioutil"
    "go_fs/go_gw_restful/gw_web/controllers/conf"
	"go_fs/go_gw_restful/gw_web/controllers/log"
	"github.com/fiorix/go-eventsocket/eventsocket" 
	"os"
	"runtime"
)
   

type Fs_Gateway struct{
	//大写为public,小写为private
	gateway_name                  string 
	username                      string   
	realm                         string 
	from_user                     string 
	from_domain                   string 
	password                      string 
	extension                     string 
	proxy                         string 
	expire_seconds                string 
	register                      string 
	register_transport            string 
	retry_seconds                 string 
	caller_id_in_from             string 
	contact_params                string 
	ping                          string 
	filename                      string 
	register_proxy                string
	seq								int 
	//Gw_name						  string
}
func (f *Fs_Gateway)GetGateway()Fs_Gateway{
	return *f
}
func (f *Fs_Gateway)GetSeq()int{
	return f.seq
}
func (f *Fs_Gateway)GetRegisterProxy()string{
	return f.register_proxy
}
func (f *Fs_Gateway)SetRegisterProxy(value string){
	f.register_proxy = value
}
func (f *Fs_Gateway)GetFilename()string{
	return f.filename
}
func (f *Fs_Gateway)GetPing()string{
	return f.ping
}
func (f *Fs_Gateway)SetPing(value string){
	f.ping = value
}
func (f *Fs_Gateway)GetContactParams()string{
	return f.contact_params
}
func (f *Fs_Gateway)SetContactParams(value string){
	f.contact_params = value
}
func (f *Fs_Gateway)GetCallerIdInFrom()string{
	return f.caller_id_in_from
}
func (f *Fs_Gateway)SetCallerIdInFrom(value string){
	f.caller_id_in_from = value
}
func (f *Fs_Gateway)GetRetrySeconds()string{
	return f.retry_seconds
}
func (f *Fs_Gateway)SetRetrySeconds(value string){
	f.retry_seconds = value
}
func (f *Fs_Gateway)GetRegisterTransport()string{
	return f.register_transport
}
func (f *Fs_Gateway)SetRegisterTransport(value string){
	f.register_transport = value
}
func (f *Fs_Gateway)GetRegister()string{
	return f.register
}
func (f *Fs_Gateway)SetRegister(value string){
	f.register = value
}
func (f *Fs_Gateway)GetExpireSeconds()string{
	return f.expire_seconds
}
func (f *Fs_Gateway)SetExpireSeconds(value string){
	f.expire_seconds = value
}
func (f *Fs_Gateway)GetProxy()string{
	return f.proxy
}
func (f *Fs_Gateway)SetProxy(value string){
	f.proxy = value
}
func (f *Fs_Gateway)GetExtension()string{
	return f.extension
}
func (f *Fs_Gateway)SetExtension(value string ){
	f.extension = value
}
func (f *Fs_Gateway)GetPassword()string{
	return f.password
}
func (f *Fs_Gateway)SetPassword(value string) {
	f.password = value
}
func (f *Fs_Gateway)GetFromDomain()string{
	return f.from_domain
}
func (f *Fs_Gateway)SetFromDomain(value string){
	f.from_domain = value
}
func (f *Fs_Gateway)GetFromUser()string{
	return f.from_user
}
func (f *Fs_Gateway)SetFromUser(value string){
	f.from_user = value
}
func (f *Fs_Gateway)GetRealm()string{
	return f.realm
}
func (f *Fs_Gateway)SetRealm(value string){
	f.realm = value
}
func (f *Fs_Gateway)GetUsername()string{
	return f.username
}
func (f *Fs_Gateway)SetUsername(value string){
	f.username = value
}
func (f *Fs_Gateway)SetGatewayName(value string){
	f.gateway_name = value
}
func (f *Fs_Gateway)GetGatewayName() string{
	return f.gateway_name
}
func (f *Fs_Gateway)MySenquence(key int)(keyadd int){
		keyadd = key +1
		return keyadd
}

/////////////////////////////////////////////////////////////
//以下struct用于读取xml
/////////////////////////////////////////////////////////////
type Paramgw struct{
	XMLName  xml.Name `xml:"param"`
    Name     string   `xml:"name,attr"`
    Value    string   `xml:"value,attr"`
}
type Gw struct{
	XMLName xml.Name  `xml:"gateway"`
	Name    string    `xml:"name,attr"`
	Params  []Paramgw `xml:"param"`
}
//////////////////////////////////////////////////////////////

func (f *Fs_Gateway) GetXmlPath() (string,bool) {
	conf, result := config.SetConfig("conf.ini")
	if result == false {
		logger.Debug("load the config file failed")
		return "",false
	}
	var pathstring string 
	pathstring,result = conf.GetValue("freeswitch","externalpath")
	if result == false{
		logger.Error("load database connect string failed");
		return "",false
	}
	return pathstring, true
}

func (f * Fs_Gateway)Rescan()bool{
	conf, result := config.SetConfig("conf.ini")
	if result == false {
		logger.Debug("load the config file failed")
		return  false
	}
	
	var Fshost,Fsauth,Fsport string
	//var Fsport,Fstime int
	
	
	Fshost, result = conf.GetValue("freeswitch","fshost")
	if result == false{
		return false
	}
	Fsport, result = conf.GetValue("freeswitch","fsport" )
	if result == false{
		return false
	}
	Fsauth,result = conf.GetValue("freeswitch","fsauth")
	if result == false{
		return false
	}
	//Fstime, err := iniconf.Int("timeout",500)
	c, err := eventsocket.Dial(Fshost+":"+Fsport, Fsauth)
	if err != nil {
		fmt.Println(err)
		//log.Fatal(err)
		return false
	}
	defer c.Close()
	//sofia profile external rescan
	// sofia profile external rescan reloadxml
	//ev, err := c.Send(fmt.Sprintf("bgapi %s", dial_string))
	ev, err := c.Send("bgapi sofia profile external rescan reloadxml")
	if err != nil {
		fmt.Println(err)
		//log.Fatal(err)
		return false
	}
//	if len(ev.Body) > 0  {
//		fmt.Println(ev.Body)
//		return true
//	}
	fmt.Println(ev.Get("Reply-Text"))
	ev.PrettyPrint()
	return true
}
func (f* Fs_Gateway)GetAGateway(gw_name string) Fs_Gateway{
	//先按照gateway_name查询是否存在这个文件
	pathstring,result := f.GetXmlPath()
	var gateway Fs_Gateway
	//var err error
	if result == false {
		err := errors.New("FreeSWITCH external path not configuration")
		fmt.Println(err)
		return gateway
	}
	if len(pathstring) < 10{
		//意味着路径可能配得不对
		err := errors.New("FreeSWITCH external path configuration error ")
		fmt.Println(err)
		return gateway
	}
	//路径分隔符补足
	if pathstring[len(pathstring)-1]=='\\'  || pathstring[len(pathstring)-1]== '/' {
		fmt.Println(pathstring)	
	}else{
		if runtime.GOOS == "windows"{
			pathstring += "\\"
			}else{
				pathstring += "/"
			}
	}
	//查找文件是否存在
	pathstring +=  gw_name + ".xml"
	fmt.Println(pathstring)	
	if f.FileIsExist(pathstring) == true{
		gw,err := f.LoadXmlFile(pathstring)
		gateway = *gw
		fmt.Println(err)
		fmt.Println("get a gateway ",gw.GetGatewayName())		
	}
	return gateway
		
}
func (f *Fs_Gateway) FileIsExist(filename string) bool{
	_, err := os.Stat(filename) 
	return err == nil || os.IsExist(err) 
}

func (f *Fs_Gateway) SaveFile() (bool,error){
	//先按照gateway_name查询是否存在这个文件
	pathstring,result := f.GetXmlPath()
	if result == false {
		err := errors.New("FreeSWITCH external path not configuration")
		return false,err
	}
	if len(pathstring) < 10{
		//意味着路径可能配得不对
		err := errors.New("FreeSWITCH external path configuration error ")
		return false,err
	}
	//路径分隔符补足
	if pathstring[len(pathstring)-1]=='\\'  || pathstring[len(pathstring)-1]== '/' {
		fmt.Println(pathstring)	
	}else{
		if runtime.GOOS == "windows"{
			pathstring += "\\"
			}else{
				pathstring += "/"
			}
	}
	//查找文件是否存在
	pathstring +=  f.gateway_name + ".xml"
	if f.FileIsExist(pathstring) == true{
		//文件已存在,由于Create会自动清空文件，故而先用起来
		f.WriteXmlFile(pathstring)
	}else{
		//文件不存在，新生成一个文件
		f.WriteXmlFile(pathstring)
	}
	f.Rescan()
	return true,nil
}

func (f *Fs_Gateway)WriteXmlFile(filename string)(bool,error){
	file,err := os.Create(filename)
	
	if err != nil {
		return false,err
	}
	defer file.Close()
	file.WriteString("\xEF\xBB\xBF") //写入utf8bom
	file.WriteString("<include>\n     <gateway name=\"")
	file.WriteString(f.gateway_name)
	file.WriteString("\">")
	myvalue := reflect.ValueOf(f).Elem()
	//fmt.Println("numField %v" ,value.NumField())
	for i:=0; i< myvalue.NumField(); i++{
		typeField := myvalue.Type().Field(i)
		if typeField.Name != "gateway_name" && len(myvalue.Field(i).String())>1{
			
			file.WriteString("\n\t\t<param name=\"")
			//freeswitch保存时是以-号，而变量则需要_，故而转下
			switch typeField.Name{
				case "gateway_name":
					file.WriteString("gateway-name")
				case "from_user":
					file.WriteString("from-user")
				case "from_domain":
					file.WriteString("from-domain")
				case "expire_seconds":
					file.WriteString("expire-seconds")
				case "register_transport":
					file.WriteString("register-transport")
				case "retry_seconds":
					file.WriteString("retry-seconds")
				case "caller_id_in_from":
					file.WriteString("caller-id-in-from")
				case "contact-params":
					file.WriteString("contact-params")         
     			case "register-proxy":
 					file.WriteString("register-proxy")
 				default:
					file.WriteString(typeField.Name)
			}
			
			file.WriteString("\" value=\"")
			file.WriteString(myvalue.Field(i).String())
			file.WriteString("\"/>")
		}
	}
	file.WriteString("\n     </gateway>\n</include>")
	return true,nil
}

func (f *Fs_Gateway)LoadXmlFile(filename string)(*Fs_Gateway,error){
	var aFs_gateway Fs_Gateway
	xmlfile,err := os.Open(filename)
	if err != nil {
    	logger.Error("Error opening file: ", err)
    	return nil,err
	}

	defer xmlfile.Close()
	decoder := xml.NewDecoder(xmlfile)
	for {
	    t, _ := decoder.Token()
	    if t == nil {
	        break
	    }
	    switch se := t.(type) {
	    case xml.StartElement:
	       if se.Name.Local == "gateway"{
				fmt.Println(se.Attr)
				var d Gw
				decoder.DecodeElement(&d,&se)
				fmt.Println(d.Name)
				aFs_gateway.gateway_name = d.Name
				for _,k := range d.Params{
					//fmt.Println(v)
					fmt.Println(k.Name)
					fmt.Println(k.Value)
					switch k.Name {
						case "realm":
						   aFs_gateway.realm = k.Value
						case "proxy":
							aFs_gateway.proxy = k.Value
						case "register":
							aFs_gateway.register = k.Value
						case "username":
							aFs_gateway.username = k.Value  			
						case "from-user":
							aFs_gateway.from_user = k.Value		
						case "from-domain":
							aFs_gateway.from_domain = k.Value
						case "password":
							aFs_gateway.password = k.Value
						case "extension":
							aFs_gateway.extension = k.Value
						case "register-proxy":
							aFs_gateway.register_proxy = k.Value
						case "expire-seconds":
							aFs_gateway.expire_seconds = k.Value
						case "register-transport":
							aFs_gateway.register_transport = k.Value
						case "retry-seconds":
							aFs_gateway.retry_seconds = k.Value
						case "caller-id-in-from":
							aFs_gateway.caller_id_in_from = k.Value
						case "contact-params":
							aFs_gateway.contact_params = k.Value
						case "ping":

					}
				}
			}
		}
	}
	return &aFs_gateway,nil
}

func (f *Fs_Gateway)EarseGateway(gateway string)(bool,error){
	if len(gateway) <1 {
		return false,errors.New("网关名称为空，无法删除")
	}
	//先按照gateway_name查询是否存在这个文件
	pathstring,result := f.GetXmlPath()
	if result == false {
		err := errors.New("FreeSWITCH external path not configuration")
		return false,err
	}
	if len(pathstring) < 10{
		//意味着路径可能配得不对
		err := errors.New("FreeSWITCH external path configuration error ")
		return false,err
	}
	//路径分隔符补足
	if pathstring[len(pathstring)-1]=='\\'  || pathstring[len(pathstring)-1]== '/' {
		fmt.Println(pathstring)	
	}else{
		if runtime.GOOS == "windows"{
			pathstring += "\\"
			}else{
				pathstring += "/"
			}
	}
	//查找文件是否存在
	pathstring +=  gateway + ".xml"
	if f.FileIsExist(pathstring) == true{
		err :=os.Remove(pathstring)
		if err !=nil{
			return false,err
		}
	}
	f.Rescan()
	return true,nil
}

func (f* Fs_Gateway)GetAllGw()([]Fs_Gateway,bool){
	//先按照gateway_name查询是否存在这个文件
	pathstring,result := f.GetXmlPath()
	if result == false {
		 
		logger.Error("FreeSWITCH external path not configuration")
		return nil,false
	}
	if len(pathstring) < 10{
		//意味着路径可能配得不对
		 
		logger.Error("FreeSWITCH external path configuration error ")
		return nil,false
	}
	
	//var myGateways []Fs_Gateway
	myGateways := make([]Fs_Gateway, 0 )
	var i int = 1
	var mylock sync.Mutex
	err := filepath.Walk(pathstring, func(pathstring string, fi os.FileInfo, err error) error {
		        mylock.Lock()
				defer mylock.Unlock()
                if ( fi == nil ) {return err}
                if fi.IsDir() {return nil}
                //println(pathstring)
				fmt.Println(1)
				fileSuffix := path.Ext(pathstring)
				if fileSuffix != ".xml" {return nil}
				mygw,err := f.LoadXmlFile(pathstring)
				
				if err ==nil && len(mygw.gateway_name) >1 {
					//myGateways[i]=*mygw
					mygw.seq = i 
					myGateways = append(myGateways, *mygw)
					i = i+1
				}
				 
                return nil
        })
    if err != nil {
            fmt.Printf("filepath.Walk() returned %v\n", err)
    }
	return myGateways,true
}