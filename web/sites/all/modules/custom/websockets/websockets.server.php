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
  protected $maxIdle = 300;

  // The database details for connecting to Drupal
  public $database_connection;
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
    // If the message starts with "DATABASE_PATH", then we should try to set
    // the database path for this user
    if(substr($msg, 0, 14) == 'DATABASE_PATH:'){
      $filepath = trim(array_pop(explode(":", $msg)));
      if(file_exists($filepath)){
        $user->database_connection = unserialize(file_get_contents($filepath));
      }
    }elseif(substr($msg, 0, 12) == 'DRUPAL_USER:'){
      $user->drupal_user = unserialize(trim(array_pop(explode(":", $msg))));
    }else{
      $this->sendToAllUsersButSameSite($user, $msg);
    }
  }

  // Send to only users on the same site
  function sendToAllUsersSameSite($user, $msg){
    foreach($this->users as $other_user){
      if($user->database_connection == $other_user->database_connection){
        $this->sendToUser($other_user, $msg);
      }
    }
  }

  // Send to only users on the same site
  function sendToAllUsersButSameSite($user, $msg){
    foreach($this->users as $other_user){
      if($user != $other_user){
        if($user->database_connection == $other_user->database_connection){
          $this->sendToUser($other_user, $msg);
        }
      }
    }
  }
}
$server = new DrupalServer();
$server->run();