<?php
  include('login.php'); // Includes Login Script

  if(isset($_SESSION['login_user'])){
    header("location: profile.php");
  }
?>
<!DOCTYPE html>
<html>
  <head>
    <title>ASQL Project</title>
    <link href="../css/style.css" rel="stylesheet" type="text/css">
    <script>
    parent.menu.location = "/etl/php/menu.php";
    </script>
  </head>
  <body>
    <div id="main">
      <div id="login">
        <h2>Login Form</h2>
        <form action="" method="post">
          <label>UserName :</label>
          <input id="name" name="username" placeholder="username" type="text">
          <label>Password :</label>
          <input id="password" name="password" placeholder="**********" type="password">
          <input name="submit" type="submit" value=" Login ">
          <div id="error">
          <span><?php echo $error; ?></span>
          </div>
        </form>
      </div>
    </div>
  </body>
</html>