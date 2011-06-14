#!/usr/bin/php -q
<?php
require_once dirname(__FILE__) . '/websocketserver/drupal.server.php';

/**
 * DrupalUser class extends the standard WebSockets Base user class.
 * 
 * The additional information that is stored per user is the database 
 * connection details to enable talk to the Drupal site.
 * @author simor
 */
class DrupalUser extends WSBaseUser{

  // Allow only 10 seconds of idle time.
  protected $maxIdle = 10;

  // The database details for connecting to Drupal
  protected $database_connection;
}

/**
 * Here we override the standard WebSockets configuration.
 * @author simor
 */
class DrupalConfig extends WSBaseConfig{

  public $address = '0';

  // The following variable must be changed if you would like to change the port
  // that the server listens on.
  public $port = 8080;

  // The Class of User to use
  public $userClass = 'DrupalUser';

  // Disable debugging.
  public $debug = FALSE;
}

class DrupalServer extends WebSocketServer{

  // Config class (as defined above).
  protected $configClass = 'DrupalConfig';

  // Here is where the hard work is done.  We need to connect to Drupal, 
  // process the message, and then check if there are any messages to return.
  function process($user, $msg){
    $this->sendToAll($msg);
  }
}
$server = new DrupalServer();
$server->run();