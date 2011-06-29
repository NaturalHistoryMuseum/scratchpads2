<?php
/**
 * Configurations for Web Socket Server.
 *
 */
class WSBaseConfig {
    
    /**
     * Server address
     */
    public $address = 'localhost';

    /**
     * Server port
     */
    public $port = 12345;
    
    /**
     * Maximum number of users connected at the same time
     * NOTE: if a user tries to connect, the server first verifies if any connected clients
     * have been idle too long (this period is set in the user object) to free as many connections
     * as possible. If after that there are still no available sockets, the new connection will
     * not be accepted.
     */
    public $maxUsers = 1000;

    /**
     * Debug (true or false)
     * Sets the error reporting level.
     */
    public $debug = true;
    
    /**
     * Name of class to use when creating a new user
     * This may be changed, but be aware that a class must extend
     * the WSBaseUser class
     */
    public $userClass = 'WSBaseUser';
    
    /**
     * If this value is an array, only connections having an origin that's present
     * in the array are allowed. Any other connections will be refused.
     * To allow all origins, set this value to FALSE
     * If you're unable to connect when using an array, please check the debug message
     * sent to the terminal. This will state the origin that tried to connect, and a
     * list of allowed origins.
     */
     public $uniqueOrigin = FALSE;
}