{{template "includes/header.tpl" }}
  


	<div class="table-responsive w90" >
       <a href="/gateway" class="btn btn-success"><span class="glyphicon glyphicon-plus"></span>新建</a>
	  <table id="main" class="table table-striped table-bordered table-hover">
			<thead>
				<tr class="row0">
				   <th> 序号</th>   <th  >名称</th>      <th>存储文件</th>   <th>远端IP</th> <th>代理IP</th>  <th>注册否</th>  <th>操作</th>
				</tr>
		    </thead>
			<tbody>
				 
			    {{range .gateways}}
				<tr class="row0">
				    <td>{{.GetSeq}}</td>    <td>  {{.GetGatewayName}}</td>  <td>{{.GetFilename}}</td> <td> {{.GetRealm}}</td><td> {{.GetProxy}}</td>           
					<td> {{.GetRegister}}</td>
					 <td>
					<input type="hidden" id="gatewayname" value="{{.GetGatewayName}}"> 
					 <button onclick="del({{.GetGatewayName}},'{{.GetGatewayName}}')" class="btn btn-danger"><span class="glyphicon glyphicon-trash"></span>刪除</button>
					<!--<a href="/gateway/remove?gatewayname={{.GetGatewayName}}" class="btn btn-success"><span class="glyphicon glyphicon-trash"></span>刪除</a> 
					<a href="/gateway" class="btn btn-success"><span class="glyphicon glyphicon-plus"></span>新建</a> -->
					<a href="/gateway?gatewayname={{.GetGatewayName}}" class="btn btn-primary"><span class="glyphicon glyphicon-edit"></span>编辑</button>
					</td>
				  
			    </tr>
				 
				{{end}}
			</tbody>
		
		
		</table>
	</div>
	<script src="http://cdn.bootcss.com/jquery/1.11.1/jquery.min.js"></script>
    <script src="http://cdn.bootcss.com/bootstrap/3.3.4/js/bootstrap.min.js"></script>
    <script src="http://cdn.bootcss.com/jquery-cookie/1.4.1/jquery.cookie.min.js"></script>
	
<script>
function del(id,subject){
var msg="确定要刪除网关\t   " +subject+"  \t吗？\n注意：删除后将无法恢复！"
if (confirm(msg)){
		//$.post("/gateway/Remove",{gatewayname:id);
		
		$.ajax({
			type:"post",
			data:{gatewayname:id},
			url:"/gateway/remove",
			dataType:"json",
			success:function(json){
				if (json="ok"){
					//$.get("/gateways")
					window.location.reload();
				}else{
					alert(json)
				}
				
			}
		})
		
	}
}
</script>

<script>
var xsrf=$.cookie("_xsrf")
$("#update").click(function(){
if ($("#bkname").val()=="" || $("#Subtitle").val()=="" || $("#description").val()=="" || $("#keywords").val()=="") { alert("抱歉，有项目为空！")}
else {
$.post("update",{bkname:$("#bkname").val(),
subtitle:$("#Subtitle").val(),
description:$("#description").val(),
keywords:$("#keywords").val(),
_xsrf:xsrf},function(date){
if (date=="0"){location.reload();}
else {alert("设定更新失败，请确认后端服务是否正常！")}
})
}
})
$("#userupdate").click(function(){
if ($("#newuser").val()=="" || $("#oldpwd").val()=="" || $("#pwd1").val()=="" || $("#pwd2").val()=="") { alert("抱歉，有项目为空！")}
else if($("#pwd1").val()==$("#pwd2").val()){
$.post("userupdate",{username:$("#newuser").val(),
oldpwd:$("#oldpwd").val(),
pwd1:$("#pwd1").val(),
pwd2:$("#pwd2").val(),
_xsrf:xsrf},function(date){
if (date=="0"){window.location.href="/logout"}
else {alert(data)}
})} else{alert("两次密码输入不一致！")}
})
</script>

{{template "includes/footer.tpl" }}
