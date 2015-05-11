<!DOCTYPE html>
<html>
<head>
<link href="../css/style.css" rel="stylesheet" type="text/css">
</head>
<body>
<?php
session_start(); // Starting Session
$error = "";
$success = "";
$fileinfo = "";
if(isset($_SESSION['upload_csv'])){
  $fileinfo = "Current uploaded CSV File:" . $_SESSION['upload_csv'];
}
if(isset($_SESSION['upload_success'])){
  $success = $_SESSION['upload_success'];
}
if(isset($_SESSION['upload_error'])){
  $error = $_SESSION['upload_error'];
}

?>

<div id="upload">
<form action="upload.php" method="post" enctype="multipart/form-data">
    <span><?php echo $fileinfo; ?></span>
    <br/><br/>
    Select CSV to upload:
    <input type="file" name="fileToUpload" id="fileToUpload">
    <br/><br/>
    <input type="submit" value="Upload CSV" name="submit">
    
</form>
</div>
<br/><br/>
<div id="error"><span><?php echo $error; ?></span></div>
<div id="success"><span><?php echo $success; ?></span></div>
</body>
</html> 