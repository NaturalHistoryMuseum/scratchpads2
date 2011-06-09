#!/php -q
<?php  
// Run from command prompt > php -q chatbot.demo.php
include "websocket.class.php";

// Extended basic WebSocket as ChatBot
class ChatBot extends WebSocket{
  function process($user,$msg){
    $this->say("< ".$msg);
    switch($msg){
      case "hello" : $this->send($user->socket,"hello human");                       break;
      case "hi"    : $this->send($user->socket,"zup human");                         break;
      case "name"  : $this->send($user->socket,"my name is Multivac, silly I know"); break;
      case "age"   : $this->send($user->socket,"I am older than time itself");       break;
      case "date"  : $this->send($user->socket,"today is ".date("Y.m.d"));           break;
      case "time"  : $this->send($user->socket,"server time is ".date("H:i:s"));     break;
      case "thanks": $this->send($user->socket,"you're welcome");                    break;
      case "bye"   : $this->send($user->socket,"bye");                               break;
      default      : $this->send($user->socket,$msg." not understood");              break;
    }
  }
}

$master = new ChatBot("localhost",12345);
