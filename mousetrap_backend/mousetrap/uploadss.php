<?php
require("dbclass.php");
// This function makes usage of
// $_GET, $_POST, etc... variables
// completly safe in SQL queries

global $postArray;
$postArray[0] = "sendSS";
$postArray[1] = "title";
$postArray[2] = "macAddr";

$msg = '';

function sql_safe($s)
{
	if (get_magic_quotes_gpc())
		$s = stripslashes($s);

	return mysql_real_escape_string($s);
}


function writeLog($somecontent){
	
	
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


}
//writeLog("test");

function stripPosts($string){

	$new_string = strip_tags($string);
	
	//$new_string = substr($new_string, 0, 25);
	
	$new_string = ereg_replace("[^A-Za-z0-9]", "", $new_string);
	
	return $new_string;
}

function checkPostData(){
global $postArray;

	
	for ($i = 0; $i < count($postArray);$i++){
		$var = $postArray[$i];

		if (!isset($_POST[$var]))
			return false;
		
			$postArray[$i] = strip_tags($_POST[$var]);
			writeLog("[".$var."] '".$_POST[$var]."'");
	}

	if(!isset($_FILES['photo']))
		return false;
	
		
	return true;	
}

function checkMacAddr($macToCompare, $dbp, $table){
	        
	$queryStr = "SELECT * FROM {$table} WHERE macAddr='{$macToCompare}' LIMIT 1";
		
	$result = $dbp->query($queryStr);
			
			
	if (!$result) return false;
	//if(!$result)die($dbp->getError()); 
			
	if ($result->getNumRows() == 0)	return false;
				
	return true;
				
}	   



// If user pressed submit in one of the forms
if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
	$msg = '';
	
	if(!checkPostData()){ 
		echo "Incorrect post messages";
		exit(0);
	}

	$table = 'ae_gallery';
	$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);
	
	
	if (!checkMacAddr($postArray[2],$db, "userdb")) {
		echo "Error: 1";	
		exit(0);
	}
	
	$macAddr=$postArray[2];
	
	$ipaddr = $_SERVER["REMOTE_ADDR"];
	
	//$db->checkMacAddr($
	// cleaning title field
	if(isset($_POST['title']))
		$title = trim(sql_safe($_POST['title']));
	else
		$title = '(empty title)';// use (empty title) string
	
	$title = stripPosts($title);
	

	//print $title."\n";
	
	$queryStr = "SELECT * FROM `userdb` WHERE macAddr='{$_POST['macAddr']}' LIMIT 1";
		
	
	$result = $db->query($queryStr);
	
	if (!$result) die($db->getError());
	
	//results

	if ($result->getNumRows() != 0){
		
		$row = $result->getRow();
		
		//foreach ($row as $str)	
	//	echo "'" . $row[1] . "' - '" . $postArray[1] . "'";
//echo $row[1] . "-" . $postArray[1];
//exit(0);
		$postArray[1] = stripPosts($postArray[1]);
		if(!strstr($row[1], $postArray[1]))
			$newNames = $row[1] . "~" . $postArray[1];
		else
			$newNames = $row[1];
		
		
		$queryStr ='UPDATE `userdb` 
		SET unames="'.$newNames.'", ipaddr="'.$ipaddr.'", macAddr="'.$postArray[2].'"';
		
		$result = $db->query($queryStr);
			
		if(!$result)die($db->getError()); 
	
		
	}
		
	if (isset($_FILES['photo']))
	{
		@list(, , $imtype, ) = getimagesize($_FILES['photo']['tmp_name']);
		// Get image type.
		// We use @ to omit errors
		
		if ($imtype == 3) // cheking image type
			$ext="png"; // to use it later in HTTP headers
		elseif ($imtype == 2)
			$ext="jpeg";
		elseif ($imtype == 1)
			$ext="gif";
		else
			$msg = 'Error: unknown file format\n';
		
	

		if (!strlen($msg)) // If there was no error
		{
			$data = file_get_contents($_FILES['photo']['tmp_name']);
			$data = mysql_real_escape_string($data);
			// Preparing data to be used in MySQL query
			$ipaddr = $_SERVER["REMOTE_ADDR"];
			
			$queryStr = "INSERT INTO {$table}
			SET ext='$ext', title='$title',
			data='$data', ipaddr='$ipaddr', macAddr='$macAddr'";
			
			$query = $db->query($queryStr);
			
			if($query)
				$msg .= 'Success: image uploaded';
			else
				$msg = 'Failed to upload, query error';
		}
	}
	//- UPLOAD SCREENSHOT

	echo $msg;
}
?>
