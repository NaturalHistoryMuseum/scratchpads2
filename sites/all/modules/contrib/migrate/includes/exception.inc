<?php

/**
 * @file
 * Custom exception class for the migrate module.
 */

class MigrateException extends Exception {
  protected $level;
  public function getLevel() {
    return $this->level;
  }

  public function __construct($message, $level = Migration::MESSAGE_ERROR) {
    $this->level = $level;
    parent::__construct($message);
  }
}
