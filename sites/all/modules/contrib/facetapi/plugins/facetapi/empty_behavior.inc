<?php

/**
 * @file
 * Base class for empty behaviors.
 */

/**
 * Abstract class implemented by all empty behavior plugins.
 */
abstract class FacetapiEmptyBehavior {

  /**
   * An array of facet settings.
   *
   * @var array
   */
  protected $settings;

  /**
   * Initializes settings.
   */
  public function __construct(stdClass $settings) {
    $this->settings = $settings->settings;
    $this->settings += $this->getDefaultSettings();
  }

  /**
   * Executes the abstract class behavior.
   *
   * @return
   *   The element's render array.
   */
  abstract public function execute();

  /**
   * Allows for backend specific overrides to the settings form.
   */
  public function settingsForm(&$form, &$form_state) {
    // Nothing to do...
  }

  /**
   * Returns an array of default settings.
   */
  public function getDefaultSettings() {
    return array();
  }
}

/**
 * Plugin that returns an empty array, meaning
 */
class FacetapiEmptyBehaviorNone extends FacetapiEmptyBehavior {

  /**
   * Returns an empty array.
   */
  public function execute() {
    return array();
  }
}

