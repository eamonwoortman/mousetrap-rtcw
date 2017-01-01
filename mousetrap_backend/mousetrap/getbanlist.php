<?php
require("dbclass.php");

$table = 'banlist';
// use the same name as SQL table

//$password = 'aaaaaa';

/*
$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);

$result = $db->query("SELECT * FROM `banlist`");
	
$numrows = $result->getNumRows();
if ($numrows == 0)
	die('no banlist');

$num = 0;	
*/

$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);

$result = $db->query("SELECT * FROM `banlist`");
	
$numrows = $result->getNumRows();
if ($numrows == 0) {
	die();
}

$num = 0;	

while($banEntry = $result->getRow()){

	$num++;

	//if ($num != $numrows)
		echo $banEntry[2] . "\n";
	//else
	//	echo $banEntry[2];


}
/*
SELECT * 
FROM `banlist` 
LIMIT 0 , 30
*/





?>