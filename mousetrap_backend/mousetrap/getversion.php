<?php

	if (empty($_GET)){
	 return;
	}
	if (!isset($_GET['mtguid'])){
		 return;
	}

	$c_fileguid = "F86EEB8A8EEE2A2F8F10570FAED80BDD";


	$string = strip_tags($_GET['mtguid']);


	if ($string == $c_fileguid)
		echo "SUCCESS";
	else
		echo "ERROR";
	//echo "lol";


?>