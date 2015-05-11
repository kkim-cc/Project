<?php
session_start(); // Starting Session
ini_set('display_errors', 'On');
error_reporting(E_ALL);
$error=''; // Variable To Store Error Message
$servername = "localhost";
$username = "root";
$password = "Password1";
$dbname = "azure";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$start_time = date('Y-m-d H:i:s');
// 1. Extract Transform Data.
$sql = "CALL transform_units('" . $_SESSION['login_user'] . "');";
if($conn->query($sql)){
  $_SESSION['trans_success'] = "Transform Data is done.";
  $_SESSION['trans_error'] = "";
  
  // 2. logging to 
$end_time = date('Y-m-d H:i:s');
$sql = "insert into user_log (userName, logDate, logData) values ('" 
    . $_SESSION['login_user']. "', '" 
    . $end_time . "', 'Create table and load data from file ("
    . $_SESSION['upload_csv'] .") start time:" . $start_time . "');";
$conn->query($sql);

}
else{
  $_SESSION['trans_error'] = $conn->error;
}

$conn->close();

header("location: load.php");
?> 