<?php
session_start(); // Starting Session
ini_set('display_errors', 'On');
error_reporting(E_ALL);

$servername = "localhost";
$username = "root";
$password = "Password1";
$dbname = "azure";

$code = "103";
if(isset($_GET['code'])){
  $code = $_GET['code'];
}

$mysqli = new mysqli($servername, $username, $password, $dbname);
$myArray = array();

////////////////*************** change state code!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

$sql="select CONCAT(wYear, '-', right(concat('0',wMonth),2)) as category, wPCP as PCP, wCDD as CDD, 
	     wHDD as HDD, wTMIN as TMIN, wTMAX as TMAX, wTAVG as TAVG 
       from " . $_SESSION['login_user'] . " where wStateCode=" . $code . 
       " order by category;";
       
//echo $sql;

if ($result = $mysqli->query($sql)) {

    while($row = $result->fetch_array(MYSQL_ASSOC)) {
            $myArray[] = $row;
    }
    echo json_encode($myArray);
    $result->close();
}
$mysqli->close();
?>