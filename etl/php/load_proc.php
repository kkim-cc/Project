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

// 1. drop table if exist.
$sql = "DROP TABLE IF EXISTS " . $_SESSION['login_user'];
$conn->query($sql);

// 2. create table.
$sql = "CREATE TABLE IF NOT EXISTS " . $_SESSION['login_user'] 
    . " (wStateCode 		INT,
        wYear			INT, 
        wMonth			INT,
        wPCP			DECIMAL(5,2),
        wCDD			INT,
        wHDD			INT,
        wTMIN			DECIMAL(5,2),
        wTMAX			DECIMAL(5,2),
        wTAVG			DECIMAL(5,2),
        PRIMARY KEY(wStateCode, wYear, wMonth),
        FOREIGN KEY(wStateCode) REFERENCES StateCode(scode)
        );";
if($conn->query($sql)){}
else{
  $_SESSION['load_error'] = $conn->error;
  echo $conn->error;
  $conn->close();
  //header("location: load.php");
  return;
}
  

// 3. insert from file.
$sql = "LOAD DATA LOCAL INFILE 'C:\\\\inetpub\\\\wwwroot\\\\etl\\\\uploads\\\\" .
      $_SESSION['upload_csv'] . "' 
      INTO TABLE " . 
      $_SESSION['login_user'] . " 
      FIELDS TERMINATED BY ',' 
      LINES TERMINATED BY '\n' 
      IGNORE 1 ROWS 
        (@StateCode, @Division, @YearMonth, @PCP, @TAVG, @PDSI, @PHDI, @ZNDX, @PMDI,
         @CDD, @HDD, @SP01, @SP02, @SP03, @SP06, @SP09, @SP12, @SP24, @TMIN, @TMAX) 
      SET wStateCode=@StateCode,wYear=FLOOR(@YearMonth/100), wMonth=@YearMonth%100, 
      wPCP=@PCP, wCDD=@CDD, wHDD=@HDD, wTMIN=@TMIN, wTMAX=@TMAX, wTAVG=@TAVG;";
if($conn->query($sql)){}
else{
  $_SESSION['load_error'] = $conn->error;
  echo $conn->error . "<br/>";
  echo $sql;
  $conn->close();
  header("location: load.php");
  return;
}

// 4. logging to 
$end_time = date('Y-m-d H:i:s');
$sql = "insert into user_log (userName, logDate, logData) values ('" 
    . $_SESSION['login_user']. "', '" 
    . $end_time . "', 'Create table and load data from file ("
    . $_SESSION['upload_csv'] .") start time:" . $start_time . "');";
$conn->query($sql);

$conn->close();

$_SESSION['load_success'] = 
    "data(" . $_SESSION['upload_csv'] . ") is populated to table(" 
    . $_SESSION['login_user'] . ").";
$_SESSION['load_error'] = "";

header("location: load.php");
?> 