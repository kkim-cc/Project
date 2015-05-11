<!DOCTYPE html>
<html>
<head>
<link href="../css/style.css" rel="stylesheet" type="text/css">
<script>
function validateForm() {
  var x = document.forms["load"];
  if (x != null) {
    var r = confirm("Do you want replace database if exist ?");
    if (r == true) {
      return true;
    }
    return false;
  }
  return true;
}
</script>

</head>
<body>
<?php
session_start(); // Starting Session
$lerror = "";
$lsuccess = "";
$terror = "";
$tsuccess = "";
$fileinfo = "";
if(isset($_SESSION['load_success'])){
  $lsuccess = $_SESSION['load_success'];
}
if(isset($_SESSION['load_error'])){
  $lerror = $_SESSION['load_error'];
}
if(isset($_SESSION['trans_success'])){
  $tsuccess = $_SESSION['trans_success'];
}
if(isset($_SESSION['trans_error'])){
  $terror = $_SESSION['trans_error'];
}
?>
<span><?php echo $fileinfo; ?></span>
<div id="upload">
  <?php if(!isset($_SESSION['load_success']) && isset($_SESSION['upload_csv'])){ ?>
    <form id="load" name="load" action="load_proc.php" method="post" onsubmit="return validateForm();"enctype="multipart/form-data">
    <h2>Load CSV File</h2><br/><br/>
    Insert the information in File (<?php echo $_SESSION['upload_csv'] ?>) to mysql table: 
    <input name="submit" type="submit" value=" Load "/>
  <?php } ?>
    <br/><br/>
  <?php 
    if(isset($_SESSION['load_success']) && isset($_SESSION['upload_csv']) &&
       !isset($_SESSION['trans_success'])){ 
  ?>
    <form name="trans" action="trans.php" method="post" enctype="multipart/form-data">
    <h2>Transform the data in table</h2><br/><br/>
    Transform the information in the mysql table: 
    <input name="submit" type="submit" value=" Translate "/>
  <?php } ?>
  <br/><br/>
  <div id="error"><span><?php echo $lerror . "<br>" . $terror; ?></span></div>
  <div id="success"><span><?php echo $lsuccess . "<br>" . $tsuccess;?></span></div>
</form>
</div>
</body>
</html> 