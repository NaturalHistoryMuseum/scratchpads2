Extendible Web Sockets Server
========================

_Feautures_:

- Extendible server class
- Extendible configuration class
- Extendible user class
- Configurable maximum number of connections
- Hooks to catch / modify most events
- Extremely easy implementation

Implementation
---------------------
1. Copy all files to a directory of your choice
2. In your php file, include the server.php file
3. Extend WebSocketServer and create the process() method
4. Instantiate a new object and call the run() method

Optional:

1. Extend the configuration class and add custom configs (see config.class.php for available configurations). If you wish to use a custom config class (which you probably will in a production environment) you need to tell your server about it by adding the protected $configClass property to the server class you created in step 3, having the name of your class as its value.
2. Extend the user class to add your own methods which will become available in your hook implementations.
3. Implement hooks (see below)

Implementation example:
------------------------------------
    require_once dirname(__FILE__).'/websocketserver/server.php';

    class ChatUser extends WSBaseUser {
        protected $maxIdle = 10;
    }

    class ChatConfig extends WSBaseConfig {
        public $address = 'localhost';
        public $post = 12345;
        public $userClass = 'ChatUser';
    }

    class ChatServer extends WebSocketServer {
    
        protected $configClass = 'ChatConfig';

        function process($user, $msg){
            $this->sentToAllBut($user, $msg);
        }
    }

    $server = new ChatServer();
    $server->run();


Hooks
---------
Available hooks that may be implemented:

1. preSend

        function preSend(&$user, &$msg)
2. postSend

        function postSend(&$user, &$msg)
3. preConnect

        function preConnect(&$user, $socket)
4. postConnect

        function postConnect(&$user, $socket)
5. preDisconnect

          function preDisconnect(&$user, $socket)
6. postDisconnect

          function postDisconnect()
7. preShutdown:

        function preShutdown(&$doShutdown)
8. postShutdown

        function postShutdown()

Implementing these hooks is done by creating a method with that name in the class that extends WebSocketServer. Some parameters are passed by reference, meaning that hooks don't only provide information from the server, but also allows influencing its behaviour. In cases where a user is passed to a hook, the actual user object is available, so any methods that were added to the use class by extending it, are available in these hooks.

Send methods
--------------------
In the following, $user is the complete user object, and $msg is the message (string, array or object) to send to the client.

1. Send to one specific user

        sendToUser($user, $msg)
2. Send to all users

        sendToAll($msg)
3. Send to all users, except one

        sendToAllBut($user, $msg)

Message formats:
----------------
The server adds the opening and closing characters for correctly sending it over a socket connection, and removes them when a message is received. If you send an object or array as message, the server will encode it to json. If the server receives a message in json format, it will be converted to an object or array for internal use. _All hooks and the process method can use objects and arrays as messages, the server will do the conversion where appropriate._


Unique origins
--------------
The server can be configured to allow connections from any origin, or you may supply a list of allowed origins. Please see config.class.php for more details.


Thanks!
-------

A big thank you goes to georgenava, whose example code was great for learning how to do a basic implementation of websockets in PHP. The run() method in my implementation and some other parts are taken almost literally from his code at http://code.google.com/p/phpwebsocket/ and https://github.com/GeorgeNava/phpwebsocket