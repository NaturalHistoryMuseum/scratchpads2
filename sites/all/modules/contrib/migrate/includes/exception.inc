<?php

/**
 * @file
 * Custom exception class for the migrate module.
 */

class MigrateException extends Exception {

  /**
   * The level of the error being reported (a Migration::MESSAGE_* constant)
   *
   * @var int
   */
  protected $level;

  public function getLevel() {
    return $this->level;
  }

  /**
   * The status to record in the map table for the current item (a
   * MigrateMap::STATUS_* constant)
   *
   * @var int
   */
  protected $status;

  public function getStatus() {
    return $this->status;
  }

  public function __construct($message, $level = Migration::MESSAGE_ERROR,
                              $status = MigrateMap::STATUS_FAILED) {
    $this->level = $level;
    $this->status = $status;
    parent::__construct($message);
  }
}
