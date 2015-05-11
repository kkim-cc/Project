<?php
session_start(); // Starting Session
$username="";
if(isset($_SESSION['login_user'])){
  $username = $_SESSION['login_user'];
}
?>

<!--Force IE6 into quirks mode with this comment tag-->
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<title>ASQL ETL Graph</title>
<link href="/etl/css/menu.css" rel="stylesheet" type="text/css"> 
</head>

<body>
<h2 align='center'> ASQL <br/> ETL <br/>Graph</h2>
<nav>
	<ul class="sonarmenu">
		<li><a href="/etl/php/index.php" target="main">Login</a></li>
<?php
if($username != ""){
?>
		<li><a href="/etl/php/file_upload.php" target="main">Upload CSV</a></li>
    <li><a href="/etl/php/load.php" target="main">Load Data</a></li>
		<li><a href="/etl/chart.php" target="main">Graphing</a></li>
		<li><a href="/etl/php/ViewLog.php" target="main">Logging</a></li>
<?php
}
?>
	</ul>
</nav>



</body>
</html>
