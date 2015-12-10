{{template "includes/header.tpl" }}
  
	<div class="table-responsive w90" >
		<h2>{{.operate}}网关操作 </h2>
		 
		<form action="/gateway/save" method="post"> 
		    <table id="main" class="table table-striped table-bordered table-hover">
			    <tr> 
					<td>网关名称</td>  
					<td><input type="text" name="gateway_name" value="{{.gateway_name}}"  class="form-control" required autofocus > </td>  
					<td>*必填</td> 
				</tr>
				    
				
				<tr> 
					<td>验证用户名</td>  
					<td><input type="text" name="username" value="{{.username}}"  class="form-control"     > </td>  
					<td>远程对接的用户名</td> 
				</tr>
				
				<tr> 
					<td>远端IP</td>  
					<td><input type="text" name="realm" value="{{.realm}}"  class="form-control" required autofocus > </td>  
					<td>*必填</td>
				</tr>
				<tr> 
					<td>from-user</td>  
					<td><input type="text" name="from_user" value="{{.from_user}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				<tr> 
					<td>from-domain</td>  
					<td><input type="text" name="from_domain" value="{{.from_domain}}"  class="form-control"   > </td>  
					<td></td>
				</tr>
			           
		
				<tr> 
					<td>验证密码</td>  
					<td><input type="text" name="password" value="{{.password}}"  class="form-control"   > </td>  
					<td></td>
				</tr>
				<tr> 
					<td>extension</td>  
					<td><input type="text" name="extension" value="{{.extension}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				<tr> 
					<td>代理IP</td>  
					<td><input type="text" name="proxy" value="{{.proxy}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				<tr> 
					<td>expire-seconds</td>  
					<td><input type="text" name="expire_seconds" value="{{.expire_seconds}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				<tr> 
					<td>register</td>  
					<td><input type="text" name="register" value="{{.register}}"  class="form-control" required autofocus > </td>  
					<td>*必填：true,false</td>
				</tr>
	  
				
				<tr> 
					<td>register_transport</td>  
					<td><input type="text" name="register_transport" value="{{.register_transport}}"  class="form-control"   > </td>  
					<td></td>
				</tr>
				
				<tr> 
					<td>retry_seconds</td>  
					<td><input type="text" name="retry_seconds" value="{{.retry_seconds}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				
				<tr> 
					<td>caller_id_in_from</td>  
					<td><input type="text" name="caller_id_in_from" value="{{.caller_id_in_from}}"  class="form-control"  > </td>  
					<td>true,false</td>
				</tr>
				
				<tr> 
					<td>contact_params</td>  
					<td><input type="text" name="contact_params" value="{{.contact_params}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				
				<tr> 
					<td>ping</td>  
					<td><input type="text" name="ping" value="{{.ping}}"  class="form-control"   > </td>  
					<td>数值</td>
				</tr>
				<tr> 
					<td>register_proxy</td>  
					<td><input type="text" name="register_proxy" value="{{.register_proxy}}"  class="form-control"  > </td>  
					<td></td>
				</tr>
				<tr> 
					<td colspan="3"> <input type="submit" value="提交" class="btn btn-lg btn-primary btn-block"><br></td>  
					   
				</tr>
			</table>
		</form> 
		 
	</div>

{{template "includes/footer.tpl" }}