<?php

class WSBaseUser{

    public $id;
    public $socket;
    public $handshake;
    public $idle;

    /**
     * Maximum number of seconds a client may be idle before being disconnected
     */
    protected $maxIdle = 60;//seconds

    /**
     * Setup new user
     *
     * @param $id
     *    Unique id
     * @param $socket
     *    This users socket
     */
    public function __construct($id, $socket){
        $this->id = $id;
        $this->socket = $socket;
        $this->resetIdle();
    }
    
    /**
     * Check if client has been idle too long
     *
     * @return
     *    if client has been idle too long (boolean)
     */
    public function isIdle(){
        return time() - $this->idle > $this->maxIdle;
    }
    
    private final function resetIdle(){
        $this->idle = time();
    }
    
    /**
     * Notify that a message was reveived for this user
     */
    public function messageReceived($msg){
        $this->resetIdle();
    }
    /**
     * Find the owner of a socket
     * 
     * @param
     *    The socket for which we want to find the owner
     * @return
     *    The owner of the socket
     */
    static function getUserBySocket($socket, $users){
        $found = null;
        foreach($users as $user){
            if($user->socket == $socket){
                $found = $user;
                break;
            }
        }
        return $found;
    }
}
