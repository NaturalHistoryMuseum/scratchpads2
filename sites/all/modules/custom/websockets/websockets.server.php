#!/php -q
<?php
require_once dirname(__FILE__) . '/websocketserver/server.php';

class DrupalUser extends WSBaseUser{

  protected $maxIdle = 100;
}

class DrupalConfig extends WSBaseConfig{

  public $address = '0';

  // We define a variable for the websockets port, as it needs to be the same on
  // all sites in a multisite setup, but can be changed if required (You may
  // already have services listening on the default port 8080).
  // Note, this is the default WebSockets port to be used by simple services.
  // More complex services that will react to posted content will create their
  // own sockets/servers
  public $port = 8080;

  public $userClass = 'DrupalUser';
}

class DrupalServer extends WebSocketServer{

  protected $configClass = 'DrupalConfig';

  function process($user, $msg){
    $this->sentToAllBut($user, $msg);
  }
}
$server = new DrupalServer();
$server->run();