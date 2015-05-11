<?php
  include('session.php');
?>
<!DOCTYPE html>
<html>
  <head>
    <title>ASQL ETL Graph</title>
    <link href="../css/style.css" rel="stylesheet" type="text/css">
    <script>
    parent.menu.location = "/etl/php/menu.php";
    </script>
  </head>
  <body>
    <div id="profile">
      <b id="welcome">Welcome : <i><?php echo $login_session; ?></i></b>
      <b id="logout"><a href="logout.php">Log Out</a></b>
    </div>
  </body>
</html>