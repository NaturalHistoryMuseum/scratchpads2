<?php

/**
 * This is an edited version of the PHP Websockets app.  This also supports the
 * fallback to classic HTTP POST.
 * 
 * http://code.google.com/p/phpwebsocket/
 */
// Usage: $master=new WebSocket("localhost",12345);
class WebSocket{

  var $master;

  var $sockets = array();

  var $users = array();

  var $debug = false;

  var $messages = array();

  function __construct($address, $port){
    // Set the time limit to 0 to ensure the process/server persists for as
    // long as required.
    set_time_limit(0);
    ob_implicit_flush();
    // Create the socket.
    $this->master = socket_create(AF_INET, SOCK_STREAM, SOL_TCP) or die("socket_create() failed");
    socket_set_option($this->master, SOL_SOCKET, SO_REUSEADDR, 1) or die("socket_option() failed");
    socket_bind($this->master, $address, $port) or die("socket_bind() failed");
    socket_listen($this->master, 20) or die("socket_listen() failed");
    $this->sockets[] = $this->master;
    while(true){
      // Receive
      $changed = $this->sockets;
      socket_select($changed, $write = NULL, $except = NULL, 1);
      foreach($changed as $socket){
        if($socket == $this->master){
          $client = socket_accept($this->master);
          if($client < 0){
            continue;
          }else{
            $this->connect($client);
          }
        }else{
          $bytes = @socket_recv($socket, $buffer, 2048, 0);
          if($bytes == 0){
            $this->disconnect($socket);
          }else{
            $user = $this->getuserbysocket($socket);
            if(!$user->handshake){
              $this->dohandshake($user, $buffer);
            }
            if($user->handshake){
              $this->process($user, $this->unwrap($buffer));
            }elseif($user->legacy){
              $this->process_legacy($user, $buffer);
            }
          }
        }
      }
      // Push
      // This is the main change from the original code.  Here we check for 
      // messages we would like to push from Drupal, and push them.
      foreach($this->users as $user){
        if($user->handshake && $user->database){
          $this->send($user->socket, 'Ping');
        }
      }
    }
  }

  function process_legacy($user, $msg){
    // Check whether or not this is a PULL or PUSH first.
    if(substr($msg, 0, 3) == 'GET'){
      $callback = '';
      if(strpos($msg, 'callback=')){
        $callback_parts = explode('callback=', $msg);
        $callback_parts = preg_split('/[&\ ]+/', $callback_parts[1]);
        $callback = $callback_parts[0];
      }
      $this->send_legacy($user, $callback);
    }else{
      // We need to process, and call the standard WebSockets process method.
      // First we get split the string on the first empty line.  Everything 
      // after that line is what has been sent.
      $lines = preg_split('/[\n\r]+/', $msg);
      $empty_reached = FALSE;
      $msg = array();
      foreach($lines as $line){
        if($empty_reached){
          $msg[] = $line;
        }
        if(!trim($line)){
          $empty_reached = TRUE;
        }
      }
      $msg = implode("\n", $msg);
      $this->send($user, $msg);
    }
  }

  function send_legacy($user, $callback){
    foreach($this->messages as $message){
      // Check that the time is valid on the message
      $msg = json_encode(array(
        'data' => 'Stuff'
      ));
      $line_end = "\r\n";
      $headers = 'HTTP/1.1 200 OK' . $line_end;
      $headers .= 'Date: ' . date('r') . $line_end;
      $headers .= 'Server: PHPWebSockets' . $line_end;
      $headers .= 'Set-Cookie: PHPWebSocketsSESSID=123; path=/' . $line_end;
      $headers .= 'Expires: Sun, 19 Nov 1978 05:00:00 GMT' . $line_end;
      $headers .= 'Cache-Control: no-cache, must-revalidate, post-check=0, pre-check=0' . $line_end;
      $headers .= 'Vary: Accept-Encoding' . $line_end;
      $headers .= 'Connection: close' . $line_end;
      $headers .= 'Content-Type: application/json; charset=utf-8' . $line_end . $line_end;
      socket_write($user->socket, "$headers$callback($msg)");
    }
    socket_close($user->socket);
  }

  function process($user, $msg){
    /* Extend and modify this method to suit your needs */
    /* Basic usage is to echo incoming messages back to client */
    $this->send($user->socket, $msg);
  }

  function send($client, $msg){
    // Save the message for legacy clients
    $message = new Message();
    $message->msg = $msg;
    $message->timestamp = time();
    $this->messages[] = $message;
    $msg = $this->wrap($msg);
    socket_write($client, $msg, strlen($msg));
  }

  function connect($socket){
    $user = new User();
    $user->id = uniqid();
    $user->socket = $socket;
    array_push($this->users, $user);
    array_push($this->sockets, $socket);
  }

  function disconnect($socket){
    $found = null;
    $n = count($this->users);
    for($i = 0; $i < $n; $i++){
      if($this->users[$i]->socket == $socket){
        $found = $i;
        break;
      }
    }
    if(!is_null($found)){
      array_splice($this->users, $found, 1);
    }
    $index = array_search($socket, $this->sockets);
    socket_close($socket);
    if($index >= 0){
      array_splice($this->sockets, $index, 1);
    }
  }

  function dohandshake($user, $buffer){
    echo "BUFFER is:\n$buffer\nEND OF BUFFER\n";
    echo "USERS:\n" . print_r($this->users, 1) . "\nEND OF USERS\n";
    list($resource, $host, $origin, $key1, $key2, $l8b) = $this->getheaders($buffer);
    if(is_null($key1) || is_null($key2)){
      // We're likely dealing with a legacy client here.  We don't need to
      // actually do anything!
      $user->legacy = TRUE;
      return true;
    }else{
      $upgrade = "HTTP/1.1 101 WebSocket Protocol Handshake\r\n" . "Upgrade: WebSocket\r\n" . "Connection: Upgrade\r\n" . //"WebSocket-Origin: " . $origin . "\r\n" .
//"WebSocket-Location: ws://" . $host . $resource . "\r\n" .
      "Sec-WebSocket-Origin: " . $origin . "\r\n" . "Sec-WebSocket-Location: ws://" . $host . $resource . "\r\n" . //"Sec-WebSocket-Protocol: icbmgame\r\n" . //Client doesn't send this
"\r\n" . $this->calcKey($key1, $key2, $l8b) . "\r\n"; // .
      //"\r\n";
      socket_write($user->socket, $upgrade . chr(0), strlen($upgrade . chr(0)));
      $user->handshake = true;
      return true;
    }
  }

  function calcKey($key1, $key2, $l8b){
    //Get the numbers
    preg_match_all('/([\d]+)/', $key1, $key1_num);
    preg_match_all('/([\d]+)/', $key2, $key2_num);
    //Number crunching [/bad pun]
    $key1_num = implode($key1_num[0]);
    $key2_num = implode($key2_num[0]);
    //Count spaces
    preg_match_all('/([ ]+)/', $key1, $key1_spc);
    preg_match_all('/([ ]+)/', $key2, $key2_spc);
    //How many spaces did it find?
    $key1_spc = strlen(implode($key1_spc[0]));
    $key2_spc = strlen(implode($key2_spc[0]));
    if($key1_spc == 0 | $key2_spc == 0){return;}
    //Some math
    $key1_sec = pack("N", $key1_num / $key1_spc); //Get the 32bit secret key, minus the other thing
    $key2_sec = pack("N", $key2_num / $key2_spc);
    //This needs checking, I'm not completely sure it should be a binary string
    return md5($key1_sec . $key2_sec . $l8b, 1); //The result, I think
  }

  function getheaders($req){
    $sk1 = $sk2 = $r = $h = $o = null;
    if(preg_match("/GET (.*) HTTP/", $req, $match)){
      $r = $match[1];
    }
    if(preg_match("/Host: (.*)\r\n/", $req, $match)){
      $h = $match[1];
    }
    if(preg_match("/Origin: (.*)\r\n/", $req, $match)){
      $o = $match[1];
    }
    if(preg_match("/Sec-WebSocket-Key1: (.*)\r\n/", $req, $match)){
      $sk1 = $match[1];
    }
    if(preg_match("/Sec-WebSocket-Key2: (.*)\r\n/", $req, $match)){
      $sk2 = $match[1];
    }
    if($match = substr($req, -8)){
      $l8b = $match;
    }
    return array(
      $r,
      $h,
      $o,
      $sk1,
      $sk2,
      $l8b
    );
  }

  function getuserbysocket($socket){
    $found = null;
    foreach($this->users as $user){
      if($user->socket == $socket){
        $found = $user;
        break;
      }
    }
    return $found;
  }

  function wrap($msg = ""){
    return chr(0) . $msg . chr(255);
  }

  function unwrap($msg = ""){
    return substr($msg, 1, strlen($msg) - 2);
  }
}

class User{

  var $id;

  var $socket;

  var $handshake;

  var $phpsession;

  var $database;
}

class Message{

  var $msg;

  var $timestamp;
}