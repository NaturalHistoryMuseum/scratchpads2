#!/php -q
<?php
// We define a variable for the websockets port, as it needs to be the same on
// all sites in a multisite setup, but can be changed if required (You may
// already have services listening on the default port 8080).
// Note, this is the default WebSockets port to be used by simple services.
// More complex services that will react to posted content will create their
// own sockets/servers
DEFINE('WEBSOCKETS_PORT', 8080);
include "websocket.class.php";

// Extended basic WebSocket as ChatBot
class DrupalSocket extends WebSocket{

  function process($user, $msg){
    foreach($this->users as $user){
      $this->send($user->socket, "Received: $msg");
      echo "Received: $msg";
    }
  }
}
$master = new DrupalSocket(0, WEBSOCKETS_PORT);