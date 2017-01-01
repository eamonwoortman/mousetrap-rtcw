<?php


	if (empty($_GET)) return;


	require("dbclass.php");

	$table = "banlist";
	$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);

	$string = strip_tags($_GET['dataStr']);

	if (strlen($string) < 10)
		return;

	function hex2bin($h)
	{		
		$a = '';
		
		if (!is_string($h)) return null;

		$r='';
  		for ($a=0; $a<strlen($h); $a+=2) { 
			$r .= chr( hexdec( $h{$a} . $h{($a+1)} ) ); 
		}
	  
		return $r;
	}

	function isUser($guid){
		global $db;
		$queryStr = "SELECT * FROM `userdb` WHERE mtGuid='{$guid}' LIMIT 1";
		
		$result = $db->query($queryStr);
	
		if (!$result) die($db->getError());
	
		
		if ($result->getNumRows())
			return 1;
		else
			return 0;

	}

	function isBanned($guid){
		global $db, $table;
		$queryStr = "SELECT * FROM {$table} WHERE guid='{$guid}' LIMIT 1";
		
		$result = $db->query($queryStr);
	
		if (!$result) die($db->getError());
	
		$ipaddr = $_SERVER["REMOTE_ADDR"];
		
		if ($result->getNumRows())
			return 1;
		else
			return 0;

	}

	echo "debug 1 '$string'<BR>";
	$string = strrev($string);
	echo "debug 2 '$string'<BR>";
	$string = @hex2bin($string);
	echo "debug 3 '$string'<BR>";

	$succes = @list($guid, $name, $cheats ) = split('[|]', $string, 3);

	$count = count($succes);

	if ($count != 3) return;

	if (!isUser($guid)){
		echo "ERROR2";
		return;
	}

	if (isBanned($guid)){
		echo "ERROR3";
		return;
	}



	$queryStr ='INSERT INTO `'.$table.'` SET guid="'.$guid.'", name="'.$name.'", cheats="'.$cheats.'"';
			
	$result = $db->query($queryStr);
			
	if(!$result)
		die("ERROR"); 
	else
		echo "SUCCESS";

	//echo $guid.$name.$cheats;


?>