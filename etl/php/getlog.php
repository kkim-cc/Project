<?php
session_start(); // Starting Session
$error=''; // Variable To Store Error Message
// Define $username and $password
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
// full time
$table = "<table>
<tr><th>&nbsp;Date&nbsp;</th><th>&nbsp;User ID&nbsp;</th><th>&nbsp;Log Data&nbsp;</th></tr>";

// search full-time
$sql = "SELECT userName, logDate, logData FROM user_log order by logDate DESC;";
if($result = $conn->query($sql)){

  if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
      $table .= "<tr><td>&nbsp;" . $row['logDate'] . "&nbsp;</td><td>&nbsp;" 
        . $row['userName'] . "&nbsp;</td><td>&nbsp;"
        . $row['logData'] . "&nbsp;</td></tr>";
    }
  } 
}
$table .= "</table>";
$conn->close();
?>