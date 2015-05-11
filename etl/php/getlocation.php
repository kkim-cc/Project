<?php
session_start(); // Starting Session
$companydata = "";

function getLocation()
{
  $servername = "localhost";
  $username = "root";
  $password = "Password1";
  $dbname = "azure";
  $companydata = "";
  $setdefault = 1;
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }

  $sql = "select s.scode, s.sname from ". $_SESSION['login_user'] ." as d
      join statecode as s on d.wStateCode = s.scode
      group by s.sname;";
  $result = $conn->query($sql);

  if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
      //"<option value=''>A</option>"
      if($setdefault){
        $companydata = $companydata . "<option value='" . $row['scode'] . "' selected>";
        $setdefault = 0;
      }
      else {
        $companydata = $companydata . "<option value='" . $row['scode'] . "'>";
      }
      $companydata = $companydata . $row['sname'] . "</option>";
    }
  } 
  $conn->close();
  return $companydata;
}

$companydata = getLocation();

?> 
