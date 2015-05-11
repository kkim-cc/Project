<?php
session_start(); // Starting Session
$error=''; // Variable To Store Error Message
$target_dir = "/uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;

//
// define ('SITE_ROOT', realpath(dirname(__FILE__)));
define("SITE_ROOT", realpath(dirname(__FILE__) . '/..'));

$extFileType = pathinfo($target_file,PATHINFO_EXTENSION);

// Check if file already exists
if (file_exists($target_file)) {
    $error= "Sorry, file already exists.";
    $uploadOk = 0;
}
// Check file size
if ($_FILES["fileToUpload"]["size"] > 2048000) {
    $error= "Sorry, your file is too large.";
    $uploadOk = 0;
}
// Allow certain file formats
if($extFileType != "csv" ) {
    $error= "Sorry, only csv file are allowed.";
    $uploadOk = 0;
}
//echo "name:". $_FILES["fileToUpload"]["name"] . "<br>";
//echo "tmp_name:". $_FILES["fileToUpload"]["tmp_name"] . "<br>";
//echo "target_file:". SITE_ROOT.$target_file . "<br/>";

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    $error= $error."<br/>Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], SITE_ROOT.$target_file)) {
        $_SESSION['upload_success'] = "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
        $_SESSION['upload_csv'] = basename( $_FILES["fileToUpload"]["name"]);
        $_SESSION['upload_error'] = "";
    } else {
        $error= "Sorry, there was an error uploading your file.";
    }
}
if($error != "")
{
  $_SESSION['upload_error'] = $error;
  $_SESSION['upload_success'] = "";
}
header("location: file_upload.php");
?> 