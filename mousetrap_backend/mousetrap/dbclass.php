<?php


$db_host = 'localhost'; // don't forget to change 
$db_user = 'wwwimage_MTUser'; 
$db_pwd = 'm0uS3tr4p';
$db_name = 'wwwimage_MTDB';



class dbClass
{
        private $conn;
        private $error = 'No Error';

		
        public function __construct($hostname = 'localhost', $username = null, $password = null, $database = null)
        {
                $this->conn = mysql_connect($hostname, $username, $password);
                if (!$this->conn) $this->error = 'Could not connect to database.';
                mysql_select_db($database);
                return $this->conn;
        }

        public function query($sql)
        {
                if (!$this->conn)
                {
                        $this->error = 'Not connected to database.';
                        return false;
                }

                $res = @mysql_query($sql, $this->conn);
                if (!$res)
                {
                        $this->error = mysql_error($this->conn);
                        return false;
                }

                return new DB_Result($res, $this->conn);
        }

		
        public function getError()
        {
                return $this->error;
        }
}

class DB_Result
{
        var $conn;
        var $res;
        var $numRows;
        var $numAffected;

        public function __construct(&$res, &$conn)
        {
                $this->res = $res;
                $this->conn = $conn;
        }

        function getNext()
        {
                $res = @mysql_fetch_assoc($this->res);
                return $res;
        }

        function getNumRows()
        {
                $res = ($this->numRows ? $this->numRows : @mysql_num_rows($this->res));
                $this->numRows = $res;
                return $res;
        }

        public function getRow()
        {
	    
	        if ($this->getNumRows() == 0)
	        	return false;
	        	
	        $res = @mysql_fetch_row($this->res);    
	        return $res;
	       
	        
        }
        
        function getNumAffected()
        {
                $res = ($this->numAffected ? $this->numAffected : @mysql_affected_rows($this->conn));
                $this->numAffected = $res;
                return $res;
        }
                	
}

function getPlayerInfoByMac($macAddr){
	global $db_host, $db_user, $db_pwd, $db_name;
	
	$table = "userdb";
	
	$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);

	$queryStr = "SELECT * FROM {$table} WHERE macAddr='{$macAddr}' LIMIT 1";
	               
	$result = $db->query($queryStr);
	
	if (!$result) die($db->getError());
	
	
	if ($result->getNumRows() == 0) return '';

	return $result->getRow();
}
/**
 * EXAMPLES:

$db_host = 'localhost'; // don't forget to change 
$db_user = 'trainee'; 
$db_pwd = 'abc123';
$db_name = 'test';
$table = "userdb";
$macAddr = "23-01-ab-34-14";
$db = new dbClass($db_host, $db_user, $db_pwd, $db_name);


 	$queryStr = "SELECT * FROM {$table} WHERE macAddr='{$macAddr}' LIMIT 1";
	               
$result = $db->query($queryStr);

if (!$result) die($db->getError());


if ($result->getNumRows() == 0) die('No Results');
while ($row = $result->getNext())
{
        //echo "{$row['username']}<br />\n";
        print_r($row);
}*/
?>