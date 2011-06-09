<?php  

// Usage: $master=new WebSocket("localhost",12345);

class WebSocket{
  var $master;
  var $sockets = array();
  var $users   = array();
  var $debug   = false;
  
  function __construct($address,$port){
    error_reporting(E_ALL);
    set_time_limit(0);
    ob_implicit_flush();

    $this->master=socket_create(AF_INET, SOCK_STREAM, SOL_TCP)     or die("socket_create() failed");
    socket_set_option($this->master, SOL_SOCKET, SO_REUSEADDR, 1)  or die("socket_option() failed");
    socket_bind($this->master, $address, $port)                    or die("socket_bind() failed");
    socket_listen($this->master,20)                                or die("socket_listen() failed");
    $this->sockets[] = $this->master;
    $this->say("Server Started : ".date('Y-m-d H:i:s'));
    $this->say("Listening on   : ".$address." port ".$port);
    $this->say("Master socket  : ".$this->master."\n");

    while(true){
      $changed = $this->sockets;
      socket_select($changed,$write=NULL,$except=NULL,NULL);
      foreach($changed as $socket){
        if($socket==$this->master){
          $client=socket_accept($this->master);
          if($client<0){ $this->log("socket_accept() failed"); continue; }
          else{ $this->connect($client); }
        }
        else{
          $bytes = @socket_recv($socket,$buffer,2048,0);
          if($bytes==0){ $this->disconnect($socket); }
          else{
            $user = $this->getuserbysocket($socket);
            if(!$user->handshake){ $this->dohandshake($user,$buffer); }
            else{ $this->process($user,$this->unwrap($buffer)); }
          }
        }
      }
    }
  }

  function process($user,$msg){
    /* Extend and modify this method to suit your needs */
    /* Basic usage is to echo incoming messages back to client */
    $this->send($user->socket,$msg);
  }

  function send($client,$msg){ 
    $this->say("> ".$msg);
    $msg = $this->wrap($msg);
    socket_write($client,$msg,strlen($msg));
    $this->say("! ".strlen($msg));
  } 

  function connect($socket){
    $user = new User();
    $user->id = uniqid();
    $user->socket = $socket;
    array_push($this->users,$user);
    array_push($this->sockets,$socket);
    $this->log($socket." CONNECTED!");
    $this->log(date("d/n/Y ")."at ".date("H:i:s T"));
  }

  function disconnect($socket){
    $found=null;
    $n=count($this->users);
    for($i=0;$i<$n;$i++){
      if($this->users[$i]->socket==$socket){ $found=$i; break; }
    }
    if(!is_null($found)){ array_splice($this->users,$found,1); }
    $index=array_search($socket,$this->sockets);
    socket_close($socket);
    $this->log($socket." DISCONNECTED!");
    if($index>=0){ array_splice($this->sockets,$index,1); }
  }

  function dohandshake($user,$buffer){
    $this->log("\nRequesting handshake...");
    $this->log($buffer);
    list($resource,$host,$origin,$key1,$key2,$l8b) = $this->getheaders($buffer);
    $this->log("Handshaking...");
    //$port = explode(":",$host);
    //$port = $port[1];
    //$this->log($origin."\r\n".$host);
    $upgrade  = "HTTP/1.1 101 WebSocket Protocol Handshake\r\n" .
                "Upgrade: WebSocket\r\n" .
                "Connection: Upgrade\r\n" .
                                //"WebSocket-Origin: " . $origin . "\r\n" .
                                //"WebSocket-Location: ws://" . $host . $resource . "\r\n" .
                "Sec-WebSocket-Origin: " . $origin . "\r\n" .
                    "Sec-WebSocket-Location: ws://" . $host . $resource . "\r\n" .
                    //"Sec-WebSocket-Protocol: icbmgame\r\n" . //Client doesn't send this
                "\r\n" .
                    $this->calcKey($key1,$key2,$l8b) . "\r\n";// .
                        //"\r\n";
    socket_write($user->socket,$upgrade.chr(0),strlen($upgrade.chr(0)));
    $user->handshake=true;
    $this->log($upgrade);
    $this->log("Done handshaking...");
    return true;
  }
  
  function calcKey($key1,$key2,$l8b){
        //Get the numbers
        preg_match_all('/([\d]+)/', $key1, $key1_num);
        preg_match_all('/([\d]+)/', $key2, $key2_num);
        //Number crunching [/bad pun]
        $this->log("Key1: " . $key1_num = implode($key1_num[0]) );
        $this->log("Key2: " . $key2_num = implode($key2_num[0]) );
        //Count spaces
        preg_match_all('/([ ]+)/', $key1, $key1_spc);
        preg_match_all('/([ ]+)/', $key2, $key2_spc);
        //How many spaces did it find?
        $this->log("Key1 Spaces: " . $key1_spc = strlen(implode($key1_spc[0])) );
        $this->log("Key2 Spaces: " . $key2_spc = strlen(implode($key2_spc[0])) );
        if($key1_spc==0|$key2_spc==0){ $this->log("Invalid key");return; }
        //Some math
        $key1_sec = pack("N",$key1_num / $key1_spc); //Get the 32bit secret key, minus the other thing
        $key2_sec = pack("N",$key2_num / $key2_spc);
        //This needs checking, I'm not completely sure it should be a binary string
        return md5($key1_sec.$key2_sec.$l8b,1); //The result, I think
  }
  
  function getheaders($req){
    $r=$h=$o=null;
    if(preg_match("/GET (.*) HTTP/"               ,$req,$match)){ $r=$match[1]; }
    if(preg_match("/Host: (.*)\r\n/"              ,$req,$match)){ $h=$match[1]; }
    if(preg_match("/Origin: (.*)\r\n/"            ,$req,$match)){ $o=$match[1]; }
    if(preg_match("/Sec-WebSocket-Key1: (.*)\r\n/",$req,$match)){ $this->log("Sec Key1: ".$sk1=$match[1]); }
    if(preg_match("/Sec-WebSocket-Key2: (.*)\r\n/",$req,$match)){ $this->log("Sec Key2: ".$sk2=$match[1]); }
    if($match=substr($req,-8))                                                                  { $this->log("Last 8 bytes: ".$l8b=$match); }
    return array($r,$h,$o,$sk1,$sk2,$l8b);
  }

  function getuserbysocket($socket){
    $found=null;
    foreach($this->users as $user){
      if($user->socket==$socket){ $found=$user; break; }
    }
    return $found;
  }

  function     say($msg=""){ echo $msg."\n"; }
  function     log($msg=""){ if($this->debug){ echo $msg."\n"; } }
  function    wrap($msg=""){ return chr(0).$msg.chr(255); }
  function  unwrap($msg=""){ return substr($msg,1,strlen($msg)-2); }

}

class User{
  var $id;
  var $socket;
  var $handshake;
}

?>
