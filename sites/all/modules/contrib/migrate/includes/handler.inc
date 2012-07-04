<?php

/**
 * @file
 * Defines the base class for destination handlers.
 */

/**
 * Abstract base class for destination handlers. Handler objects are expected
 * to implement appropriate methods (e.g., prepare, complete, or fields).
 */
abstract class MigrateHandler {
  /**
   * List of other handler classes which should be invoked before the current one.
   *
   * @var array
   */
  protected $dependencies = array();
  public function getDependencies() {
    return $this->dependencies;
  }

  /**
   * List of "types" handled by this handler. Depending on the kind of handler,
   * these may be destination types, field types, etc.
   *
   * @var array
   */
  protected $typesHandled = array();
  public function getTypesHandled() {
    return $this->typesHandled;
  }

  /**
   * Register a list of types handled by this class
   *
   * @param array $types
   */
  protected function registerTypes(array $types) {
    // Make the type names the keys
    foreach ($types as $type) {
      $type = drupal_strtolower($type);
      $this->typesHandled[$type] = $type;
    }
  }

  /**
   * Does this handler handle the given type?
   *
   * @param boolean $type
   */
  public function handlesType($type) {
    return isset($this->typesHandled[strtolower($type)]);
  }

  abstract public function __construct();
}
