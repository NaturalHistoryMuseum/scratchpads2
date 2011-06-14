<?php
require_once dirname(__FILE__).'/user.class.php';
require_once dirname(__FILE__).'/helpers.class.php';
require_once dirname(__FILE__).'/config.class.php';

/**
 * Web socket server class
 */
abstract class WebSocketServer
{
    /** Server socket */
    protected $master = NULL;

    /** Client sockets */
    protected $sockets = array();
    
    /** Client objects (see user.class.php) */
    protected $users = array();

    /** Available hooks */
    protected $hooks = array('preSend', 'postSend', 'preConnect', 'postConnect', 'preDisconnect', 'postDisconnect', 'preShutdown', 'postShutdown');

    /** Error collector */
    protected $errors = array();
    
    /** Server  configuration */
    protected $config = NULL;

    /** Config class containing server settings */
    protected $configClass = 'WSBaseConfig';

    /**
     * process() must be implemented by extending class
     *   The process() method is called when a client sends
     *   a message. The user object and message are passed.
     *   A typical implementation of the process() method
     *   is to use one of the send() methods to update clients.
     *
     * @param $user
     *    Which user sent message (user object)
     * @param $msg
     *    The message sent by the user
     */
    protected abstract function process($user, $msg);

    /**
     * Constructor: set up a server socket
     *   This doesn't take any parameters as it takes all its
     *   configurations from the appropriate config class.
     */
    public function __construct() {
        $configClass = (string)$this->configClass;        
        WSHelpers::validateClass($configClass, 'WSBaseConfig');
        $this->config = new $configClass();
        
        WSHelpers::validateClass($this->config->userClass, 'WSBaseUser');
        
        try {
            if(!is_numeric($this->config->port)){
                throw new Exception('Port must be numeric');
            }
            $this->master = $this->websocket($this->config->address, $this->config->port);
            $this->sockets = array($this->master);
        }
        catch(Exception $e){
            array_push($this->errors, $e);
        }
        
        if($this->config->debug){
            error_reporting(E_ALL);
        }
        else{
            error_reporting(E_ERROR);
        }
        // don't  time out
        set_time_limit(0);
        ob_implicit_flush();
    }
    
    /**
     * Catching unimplemented hooks
     */
    public function __call($name, $arguments){
        if(!in_array($name, $this->hooks)){
            throw new Exception('Method does not exist: '.$name);
        }
    }
    
    /**
     * Start accepting connections
     */
    public final function run(){
        while(true){
            $changed = $this->sockets;
            socket_select($changed, $write = NULL, $except = NULL, NULL);
            // $changed: the sockets that changed status
            foreach($changed as $socket){
                if($socket == $this->master){ // newly added
                    $client = socket_accept($this->master);
                    if($client < 0){
                        WSHelpers::console('socket_accept() failed');
                        continue;
                    }
                    else{
                        $this->disconnectIdleClients();
                        if(count($this->users) < $this->config->maxUsers){
                            $this->connect($client);
                        }
                        else{
                            socket_close($client);
                            WSHelpers::console('Refused connection because of max nr. of clients (%s) reached', $this->config->maxUsers);
                        }
                    }
                }

                else {
                    $bytes = @socket_recv($socket, $buffer, 2048, 0);
                    if($bytes == 0){
                        WSHelpers::console('Socket was disconnected');
                        $this->disconnect($socket);
                    }
                    else{
                        $userClass = $this->config->userClass;
                        $user = $userClass::getUserBySocket($socket, $this->users);
                        // have we shaken hands?
                        if(!$user->handshake){
                            if(!$this->doHandshake($user, $buffer)){
                                $this->disconnect($socket);
                            }
                        }
                        else{
                            $msg = WSHelpers::unwrap($buffer);
                            $user->messageReceived($msg);
                            $this->process($user, $msg);
                        }
                    }
                }
            }

        }
    }
    
    /**
     * Overwrite this method to use a different config class
     *    If your class doesn't extend WSBaseConfig, please make sure
     *    it includes the same parameters.
     *
     * @return
     *    The name of the configuration class to use.
     */
    protected function configClass(){
        return 'WSBaseConfig';
    }
    
    /**
     * Shutdown server
     */
    protected final function shutdown(){
        WSHelpers::console("\nShutting down...");
        
        $this->preShutdown($doShutdown = TRUE);
        if($doShutdown){
            foreach($this->sockets as $socket){
                $this->disconnect($socket);
            }
            socket_close($this->master);
        
            $this->postShutdown();
            WSHelpers::console("\nServer shut down.");
            exit();
        }
	}

    /**
     * Send a message to a client
     *
     * @param $user
     *    The object user to receive the message
     * @param $message
     *    The message to send
     */
    protected final function sendToUser($user, $msg){
          $this->preSend($user, $msg);
          
          $msg = WSHelpers::wrap($msg);
          socket_write($user->socket, $msg, strlen($msg));
          
          $this->postSend($user, $msg);
    }

    /**
     * Send to all users, including sender
     *
     * @param $message
     *    The message to send
     */
    protected final function sendToAll($msg){
        foreach($this->users as $user){
            $this->sendToUser($user, $msg);
        }
    }

    /**
     * Send to all users, exclusing $user. Useful for updating all users
     * with action of one of the users
     *
     * @param $user
     *    The object user to receive the message
     * @param $message
     *    The message to send
     */
    protected final function sentToAllBut($user, $msg){
        foreach($this->users as $u){
            if($user->id == $u->id) continue;
            $this->sendToUser($u, $msg);
        }
    }
    
    /**
     * Check for idle clients
     */
    private final function disconnectIdleClients(){
        foreach($this->users as $user){
            if($user->isIdle()){
                WSHelpers::console('Disconnecting idle client %s', $user->id);
                $this->disconnect($user->socket);
            }
        }
    }

    /**
     * Create a web socket
     *
     * @param $address
     *    The address of the web socket server
     * @param $port
     *    The port of the web socket server
     * @return
     *    The created socket
     */
    private final function websocket($address, $port){
        $masterSocket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
        socket_set_option($masterSocket, SOL_SOCKET, SO_REUSEADDR, 1);
        socket_bind($masterSocket, $address, $port);
        socket_listen($masterSocket, 20);

        WSHelpers::console("Server Started : %s\n"
                          ."Master socket  : %s\n"
                          ."Listening on   : %s port %s\n\n"
                          , array(@date('Y-m-d H:i:s'), $masterSocket, $address, $port));

        return $masterSocket;
    }

    /**
     * Add a new user
     *
     * @param $socket
     *    The created socket for the new user
     */
    private final function connect($socket){
        $userClass = $this->config->userClass;
        $user = new $userClass(uniqid(), $socket);
        
        $this->preConnect($user, $socket);

        array_push($this->users, $user);
        array_push($this->sockets, $socket);
        WSHelpers::console('USER %s CONNECTED!', $user->id);
          
        $this->postConnect($user, $socket);
    }

    /**
     * Remove a user if it disconnects
     * 
     * @param $socket
     *    The socket of the user to disconnect
     */
    private final function disconnect($socket){
          $found = null;
          $n = count($this->users);
          
          // remove the disconnected user from the users array
          for($i=0; $i < $n; $i++){
              if($this->users[$i]->socket == $socket){
                  $found = $this->users[$i];
                  break;
              }
          }
          
          $this->preDisconnect($found, $socket);

          WSHelpers::console('USER %s DISCONNECTED!', $found->id);

          if(!is_null($found)){
              array_splice($this->users, $i, 1);
          }
          
          // close the socket
          $index = array_search($socket, $this->sockets);
          socket_close($socket);
          
          // remove the disconnected socket from the sockets array
          if($index >= 0){
              array_splice($this->sockets, $index, 1);
          }

          $this->postDisconnect();
    }

    /**
     * Shake hands with connecting user
     *
     * @param $user
     *    The user object of the new user
     * @param $buffer
     *    The received headers
     * @return
     *    TRUE if successfull handshake, FALSE if not
     */
    private final function doHandshake($user, $buffer){
        WSHelpers::console("\nRequesting handshake for user %s", $user->id);
        if(!$upgrade = WSHelpers::getResponseHeaders($buffer, $this->config->uniqueOrigin)){
            return FALSE;
        }
        socket_write($user->socket, $upgrade . chr(0), strlen($upgrade . chr(0)));
        $user->handshake = TRUE;

        WSHelpers::console('Done handshaking for user %s', $user->id);
        return TRUE;
    }

}