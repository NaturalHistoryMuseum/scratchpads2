<?php

// $Id$

/**
 * @file
 * Contains the MessagesAlter class
 *
 * This class gives you the ability to use better
 * contains() & remove() methods
 */

/**
 * 
 */
class MessagesAlter {

  var $version = '6.x-1.3';
  var $messages;
  var $remove_used = FALSE;

  // PHP 5.x constructor
  function __construct(&$messages) {

    $this->MessagesAlter($messages);

  }
  
  // PHP 4.x constructor
  function MessagesAlter(&$messages) {
    
    $this->messages =& $messages;  
  
  }
  
  // gets all the available types
  function getTypes($type = 'all') {
    
    if ($type == 'all') {
      $types = array_keys($this->messages);
      return $types;
    }
    
    return isset($this->messages[$type]) ? array($type) : array();

  }

  /**
   * A wrapper around the internal Drupal
   * way to add status messages.
   */
  function add($message, $type='status') {
    drupal_set_message($message, $type);
  }
  
  /**
   * Matches a regex that you provide.
   *
   * @return ARRAY if the messages matches your regex
   * FALSE when no match
   */
  function match($regex, $type='all') {
    
    $matches = array();
    $has_match = FALSE;

    $types = $this->getTypes($type);
    foreach ($types as $t) {

      if ($match = $this->matchByType($t, $regex)) {
        $matches[$t] = $match;
        $has_match = TRUE;
      }
      
    }
    
    return $has_match ? $matches : FALSE;
    
  }
  
  /**
   * Does a regex by message type (status, warning, error)
   *
   * Pretty much just a wrapper of preg_match
   */
  function matchByType($type, $regex) {
    
    $matches = array();
    $has_match = FALSE;

    if (isset($this->messages[$type]) && is_array($this->messages[$type])) {
  
      foreach ($this->messages[$type] as $k => $v) {
        if ($cnt = preg_match($regex, $v, $m)) {

          /**
           * I'm returning everything because
           * I'm not exactly sure how you're going
           * to use this in your code
           */
          $matches[] = array(
            'type' => $type,
            'search' => $regex,
            'index' => $k,
            'message' => $v,
            'count' => $cnt,
            'matches' => $m,
          );
          $has_match = TRUE;
  
        }
  
      }
    
    }
    
    return $has_match ? $matches : FALSE;
    
  }

  /**
   * An easy way to search the messages for a string
   *
   * @param $search
   * A string to search for
   *
   * @param $type
   * The type of message you're searching
   *
   * @return ARRAY if the messages contains the string
   * FALSE when no string exists
   * 
   */
  function contains($search, $type='all') {

    $matches = array();
    $has_match = FALSE;

    $types = $this->getTypes($type);
    foreach ($types as $t) {

      if ($match = $this->containsByType($t, $search)) {
        $matches[$t] = $match;
        $has_match = TRUE;
      }

    }
    
    return $has_match ? $matches : FALSE;

  }
  
  function containsByType($type, $search) {
    
    $matches = array();
    $has_match = FALSE;
    
    if (isset($this->messages[$type]) && is_array($this->messages[$type])) {
      foreach ($this->messages[$type] as $k => $v) {
        if (strpos($this->messages[$type][$k], $search) !== FALSE) {
          $matches[] = array(
            'type' => $type,
            'search' => $search,
            'index' => $k,
            'message' => $this->messages[$type][$k],
          );
          $has_match = TRUE;
        }
      }
    }
    
    return $has_match ? $matches : FALSE;
    
  }
  
  /**
   * Allows you to do a inline search & replace.
   *
   * @return NULL
   */
  function strReplace($search, $replace, $type = 'all') {
    
    $types = $this->getTypes($type);
    foreach ($types as $t) {

      if (isset($this->messages[$t]) && is_array($this->messages[$t])) {
        foreach ($this->messages[$t] as $k => $v) {
          $this->messages[$t][$k] = str_replace($search, $replace, $v); 
        }
      }

    }
    
  }
  
  function remove($items) {

    // we can only remove something
    // if we have something
    if (is_array($items) && count($items)) {

      foreach ($items as $item_type) {
        foreach ($item_type as $item) {
          if (isset($this->messages[$item['type']][$item['index']])) {
            unset($this->messages[$item['type']][$item['index']]);
          }
        }
      }
      
      $this->remove_used = TRUE;
      
    }

  }

  function clean() {

    if ($this->remove_used) {

      // get rid of the empty status from people using the remove function
      foreach ($this->messages as $key => $val) {
        $count = count($this->messages[$key]);
        if ($count == 0) {
          unset($this->messages[$key]);
        }
        elseif ($count == 1 && !isset ($this->messages[$key][0])) {
          // the default theme_status_messages function
          // outputs the first index when there is
          // only one... so we need to make sure it gets what it wants
          // when this happens and our first index is missing
          $this->messages[$key][0] = array_pop($this->messages[$key]);
        }
      }

    }

  }

}