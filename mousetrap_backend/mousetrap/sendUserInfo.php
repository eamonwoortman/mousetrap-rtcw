<?php
require("dbclass.php");


function sql_safe($s)
{
	if (get_magic_quotes_gpc())
		$s = stripslashes($s);

	return mysql_real_escape_string($s);
}
//global $postArray;
$postArray[0] = "sendData";
$postArray[1] = "playerName";
$postArray[2] = "macAddr";
$postArray[3] = "mtGuid";
$postArray[4] = "cheat";
$postArray[5] = "cvarViolation"; 
	
global $test;

function stripPosts($string){

	$new_string = strip_tags($string);
	
	$new_string = substr($new_string, 0, 25);
	
	$new_string = ereg_replace("[^A-Za-z0-9]", "", $new_string);
	
	return $new_string;
}

function writeLog($somecontent){
	/*

	
$filename = getcwd() . '/test.txt';
//$somecontent = "Add this to the file\n";
$somecontent .= "\n";
// Let's make sure the file exists and is writable first.


    // In our example we're opening $filename in append mode.
    // The file pointer is at the bottom of the file hence
    // that's where $somecontent will go when we fwrite() it.
    if (!$handle = fopen($filename, 'a')) {
         // echo "Cannot open file ($filename)";
         exit;
    }

    // Write $somecontent to our opened file.
    if (fwrite($handle, $somecontent) === FALSE) {
      //  echo "Cannot write to file ($filename)";
        exit;
    }

    //echo "Success, wrote ($somecontent) to file ($filename)";

    fclose($handle);
*/

}
function checkPostData(){
global $postArray;

	for ($i = 0; $i < count($postArray);$i++){
		$var = $postArray[$i];

		//print_r($_POST);

		if (!isset($_POST[$var]))
			return false;
		
			$postArray[$i] = $_POST[$var];
			
			writeLog("[" . $i . "] '" . $_POST[$var] . "'" );
			//writeLog("[".$var."] '".$_POST[$var]."'");
		}

	return true;	
}

$table = "userdb";

$msg = '';


function processPostData (){

	$table = "userdb";

	$mtGuid = "theMTGuid";
	global $db_host, $db_user, $db_pwd, $db_name;
	$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);
	global $table;
	global $postArray;
	
	$queryStr = "SELECT * FROM {$table} WHERE macAddr='{$_POST['macAddr']}' LIMIT 1";
		
	$result = $db->query($queryStr);
	
	if (!$result) die($db->getError());
	
	$ipaddr = $_SERVER["REMOTE_ADDR"];
		
	if ($result->getNumRows() == 0){
			

			
		if ($postArray[3] != $mtGuid)
			die("wrong mt guid");

			//writeLog("deb1 -" .$postArray[1]."-");
		$queryStr ='INSERT INTO `'.$table.'` SET unames="'.stripPosts($postArray[1]).'", ipaddr="'.$ipaddr.'", macAddr="'.$postArray[2].'", mtGuid="'.$postArray[3].'", cheat="'.$postArray[4].'", cvarViolation="'.$postArray[5].'"';
			
			$result = $db->query($queryStr);
			
			if(!$result)die($db->getError()); 
				//return "could not create user";
				

	}else{
		$row = $result->getRow();
		$id = $row[0];
		
		if ($postArray[3] != $mtGuid)
			die("wrong mt guid");
			
		if(!strstr($row[1], $postArray[1]))
			$newNames = $row[1] . "~" . stripPosts($postArray[1]);
		else
			$newNames = $row[1];
		
		writeLog("deb2 -" .$newNames."-");
			//		echo "$postArray[1] $row[1]";
		$queryStr ="UPDATE {$table} 
		SET unames='$newNames', ipaddr='$ipaddr', macAddr='$postArray[2]' , cheat='$postArray[4]', cvarViolation='$postArray[5]'
		";
				//$queryStr = "UPDATE `userdb` SET `unames` = 'Sjohn' WHERE `ID` = '$id' LIMIT 1";
		
		$result = $db->query($queryStr);
			
		if(!$result)die($db->getError()); 
			
	}
		
	return 'success';

}
// If user pressed submit in one of the forms
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{



	if(!checkPostData()){ 
		echo "Incorrect post messages";
		exit(0);
	}


	$msg = processPostData();
	
	echo $msg;
		
	exit(0);		
	

}
?>
