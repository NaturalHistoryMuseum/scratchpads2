<?php

class WSHelpers{

    /**
     * Build a response header
     *
     * @param $buffer
     *    data to be used in response header
     * @return
     *    response header
     */
    static function getResponseHeaders($buffer = '', $uniqueOrigin = FALSE){
        list($resource, $host, $origin, $strkey1, $strkey2, $data) = self::getRequestHeaders($buffer);
        if(!self::validOrigin($origin, $uniqueOrigin)){
            self::console('Refusing connection from origin %s. Allowed origin(s): %s', array($origin, implode(', ', $uniqueOrigin)));
            return FALSE;
        }

        // find numbers
        $pattern = '/[^\d]*/';
        $replacement = '';
        $numkey1 = preg_replace($pattern, $replacement, $strkey1);
        $numkey2 = preg_replace($pattern, $replacement, $strkey2);

        // find spaces
        $pattern = '/[^ ]*/';
        $replacement = '';
        $spaces1 = strlen(preg_replace($pattern, $replacement, $strkey1));
        $spaces2 = strlen(preg_replace($pattern, $replacement, $strkey2));

        if ($spaces1 == 0 || $spaces2 == 0 || $numkey1 % $spaces1 != 0 || $numkey2 % $spaces2 != 0) {
            WSHelpers::console('Handshake failed');
            return FALSE;
        }

        $ctx = hash_init('md5');
        hash_update($ctx, pack("N", $numkey1 / $spaces1));
        hash_update($ctx, pack("N", $numkey2 / $spaces2));
        hash_update($ctx, $data);
        $hash_data = hash_final($ctx, TRUE);

        return "HTTP/1.1 101 WebSocket Protocol Handshake\r\n" .
               "Upgrade: WebSocket\r\n" .
               "Connection: Upgrade\r\n" .
               "Sec-WebSocket-Origin: " . $origin . "\r\n" .
               "Sec-WebSocket-Location: ws://" . $host . $resource . "\r\n" .
               "\r\n" . $hash_data;
    }

    static function validOrigin($origin, $uniqueOrigin){
        if(is_array($uniqueOrigin)) {
            return in_array($origin, $uniqueOrigin);
        }
        return TRUE;
    }

    /**
     * Get the header values from the received request
     *
     * @param $req
     *    The header request
     * @return
     *    an array containing the following values:
     *    resource, host, origin, key1, key2, data
     */
    static function getRequestHeaders($req){
        $r = $h = $o = $key1 = $key2 = $data = null;
        if(preg_match("/GET (.*) HTTP/"   , $req, $match))              { $r=$match[1];    }
        if(preg_match("/Host: (.*)\r\n/"  , $req, $match))              { $h=$match[1];    }
        if(preg_match("/Origin: (.*)\r\n/", $req, $match))              { $o=$match[1];    }
        if(preg_match("/Sec-WebSocket-Key2: (.*)\r\n/", $req, $match))  { $key2=$match[1]; }
        if(preg_match("/Sec-WebSocket-Key1: (.*)\r\n/", $req, $match))  { $key1=$match[1]; }
        if(preg_match("/\r\n(.*?)\$/", $req, $match))                   { $data=$match[1]; }
        return array($r, $h, $o, $key1, $key2, $data);
    }
    
    
    /**
     * Verify if a class exists and if it extends a base class
     *
     * @param $className
     *    The class to verify
     * @param $parentName
     *    The base class that must be extended by $className
     */
    static function validateClass($className, $parentName){

        if (!class_exists((string)$className)){
            throw new Exception('Non existing class given to extend '.$parentName.' class: '.$className);
        }
        
        $class = new ReflectionClass($className);
        $parents = array($className);

        while ($parent = $class->getParentClass()) {
            $class = $parent;
            array_push($parents, $parent->getName());
        }
        if(!in_array($parentName, $parents)){
            throw new Exception($className.' does not extend '.$parentName);
        }
    }
    
    /**
     * Prepare a message for sending of websocket
     *
     * @param $msg
     *    The message to be prepared
     * @return
     *    The message wrapped up for sending
     */
    static function wrap($msg = ''){
        if(is_object($msg) || is_array($msg)){
            $msg = json_encode($msg);
        }
        return chr(0) . $msg . chr(255);
    }

    /**
     * Remove wrapper characters from received message
     *
     * @param $msg
     *    The message received over the socket
     * @return
     *    The unwrapped string
     */
    static function unwrap($msg = ''){
        $msg = substr($msg, 1, strlen($msg) - 2);
        if(json_decode($msg) !== null){
            $msg = json_decode($msg);
        }
        return $msg;
    }

    /**
     * Output debugging message
     *
     * @param $msg
     *    The message to ouput to terminal
     */
    static function console($msg = '', $vars = array()){
        vprintf($msg . "\n", $vars);
    }
}