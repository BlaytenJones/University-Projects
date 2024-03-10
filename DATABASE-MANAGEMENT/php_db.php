<?php
// A class that contains useful php functions that interact with mysql

class php_db 
{
   // the instance variables
   private $conn;
   private $host;
   private $user;
   private $password;
   private $dbaseName;
   private $debug;
   private $status_fatal;
     
   function __construct($username,$pass,$dbname) 
   {
      $this->conn = false;
      $this->host = 'turing';               //hostname
      $this->user = $username;           
      $this->password = $pass  ;  
      $this->dbaseName = $dbname;      
      $this->port = '3306';
      $this->debug = true;
      $this->connect();
   }
     
   function __destruct() 
   {
      $this->disconnect();
   }
    
   function connect() 
   {
      if (!$this->conn) 
      {
         try 
         {
            $this->conn = new PDO('mysql:host='.$this->host.';dbname='.$this->dbaseName.'', 
                                  $this->user, $this->password, 
				   array(PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES utf8'));  
         }
         catch (Exception $e) 
         {
            die('Error : ' . $e->getMessage());
         }
     
         if (!$this->conn) 
         {
            $this->status_fatal = true;
            echo 'Connection BD failed';
            die();
         } 
         else 
         {
            $this->status_fatal = false;
         }
      }
      return $this->conn;
   }
     
   function disconnect() 
   {
      if ($this->conn) 
      {
         $this->conn = null;
      }
   }
    
   function query($query) 
   {
   $result = $this->conn->prepare($query);
   $ret = $result->execute();

      if (!$ret) 
      {
         echo 'PDO::errorInfo():';
         echo '<br />';
         echo "error SQL: $query";
         die();
      }
      $result->setFetchMode(PDO::FETCH_ASSOC);
      $reponse = $result->fetchAll();
    
      return $reponse;
   }
    
   function execute($query) 
   {
      if (!$response = $this->conn->exec($query)) 
      {
         echo 'PDO::errorInfo():';
         echo '<br />';
         echo 'error SQL: '.$query;
         die();
      }
      return $response;
   }

   function initDatabase()
   {
      try 
      {
    
		 
      }     
      catch (Exception $e) 
      {
         die('Error : ' . $e->getMessage());
      }
   }

   function insert($table, $values)

   { 
     $result = $this->query("INSERT INTO " . $table . " " . " VALUES (" . $values . ")");
   }

   function nextId($table) 
   {
      $result = $this->query("select IFNULL(max(ID), 0) as max_id from " . $table);
      return $result[0]["max_id"] + 1;
   }

   function printTable($arrValues)
   {
      try
      {
		 echo '<table>';

         // Output header row from keys.
         echo '<tr>';
         foreach($arrValues[0] as $key => $field) 
            echo '<th>' . $key . '</th>';
         echo '</tr>';

         // Output data rows from keys.
         foreach ($arrValues as $row) 
         {
            echo '<tr>';
            foreach($row as $key => $field) 
               echo '<td>' . $field . '</td>';
            echo '</tr>';
         }
         echo '</table>';
      }
      catch (Exception $e) 
      {
         die('Error : ' . $e->getMessage());
      }
   }
}
