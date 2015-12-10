{{template "includes/header.tpl" }}
<div class="container">
	<div class="w20 align-left">
	
	   <h1>login</h1> <br />
	   <form action="/" method="post">
	       用户名:<input type="text" name="username" class="form-control" required autofocus><br>
	       密码:<input type="password" name="pwd" class="form-control" placeholder="Password" required><br>
	       <input type="submit" value="登录" class="btn btn-lg btn-primary btn-block"><br>
	       
	   </form>
	</div>
</div>
{{template "includes/footer.tpl" }}