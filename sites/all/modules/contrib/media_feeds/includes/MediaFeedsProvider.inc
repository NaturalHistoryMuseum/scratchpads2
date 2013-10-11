<?php

/**
 * @file
 * The MediaFeedsProvider base class.
 */

/**
 * Base class for different Media Feeds providers.
 */
abstract class MediaFeedsProvider {

  /**
   * The FeedsSource.
   */
  protected $source;

  /**
   * The target entity.
   */
  protected $entity;

  /**
   * The target name.
   */
  protected $target;

  /**
   * The values to save.
   */
  protected $value;

  /**
   * An associative array of configuration options.
   */
  protected $config;

  /**
   * Constructor of MediaFeedsProvider.
   *
   * @param $value
   *   The value to save.
   *
   * @param $config
   *   An associative array of configuration options.
   */
  public function __construct($value, $config = array()) {
    // Explicitly support FeedsEnclosure.
    if ($value instanceof FeedsEnclosure) {
      $value = $value->getValue();
    }

    $this->value = $value;
    $this->config = $config;
  }

  /**
   * Validate the source value.
   */
  public function validate() { }

  /**
   * Create a file object from the source value.
   *
   * @return
   *   File object on success, FALSE on failure.
   */
  public abstract function getFileObject();

  /**
   * Save the file in the database.
   *
   * @return
   *   The file object as it has been saved or FALSE on failure.
   */
  public abstract function save();
}
