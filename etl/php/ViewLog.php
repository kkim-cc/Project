<?php
  $table = "";
  include('getlog.php'); // Includes Login Script
?>
<!DOCTYPE html>
<html>
  <head>
    <title>SQ2 EMS</title>
    <link href="../css/style.css" rel="stylesheet" type="text/css">
  </head>
  <body>
    
    <form action="" method="post" id="audit" name="audit" >
      <h2>View Log Data</h2>
      <span><?php echo $table; ?></span>
      <div id="error">
      <span><?php echo $error; ?></span>
      </div>
    </form>
</body>
</html>