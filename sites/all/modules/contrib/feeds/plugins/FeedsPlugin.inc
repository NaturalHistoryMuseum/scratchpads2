<?php

/**
 * @file
 * Definition of FeedsPlugin class.
 */

/**
 * Implement source interface for all plugins.
 *
 * Note how this class does not attempt to store source information locally.
 * Doing this would break the model where source information is represented by
 * an object that is being passed into a Feed object and its plugins.
 */
abstract class FeedsPlugin extends FeedsConfigurable implements FeedsSourceInterface {

  /**
   * Constructor.
   *
   * Initialize class variables.
   */
  protected function __construct($id) {
    parent::__construct($id);
    $this->source_config = $this->sourceDefaults();
  }

  /**
   * Save changes to the configuration of this object.
   * Delegate saving to parent (= Feed) which will collect
   * information from this object by way of getConfig() and store it.
   */
  public function save() {
    feeds_importer($this->id)->save();
  }

  /**
   * Returns TRUE if $this->sourceForm() returns a form.
   */
  public function hasSourceConfig() {
    $form = $this->sourceForm(array());
    return !empty($form);
  }

  /**
   * Implementation of FeedsSourceInterface::sourceDefaults().
   */
  public function sourceDefaults() {
    $values = array_flip(array_keys($this->sourceForm(array())));
    foreach ($values as $k => $v) {
      $values[$k] = '';
    }
    return $values;
  }

  /**
   * Callback methods, exposes source form.
   */
  public function sourceForm($source_config) {
    return array();
  }

  /**
   * Validation handler for sourceForm.
   */
  public function sourceFormValidate(&$source_config) {}

  /**
   * A source is being saved.
   */
  public function sourceSave(FeedsSource $source) {}

  /**
   * A source is being deleted.
   */
  public function sourceDelete(FeedsSource $source) {}

  /**
   * Loads on-behalf implementations from mappers/ directory.
   *
   * FeedsProcessor::map() does not load from mappers/ as only node and user
   * processor ship with on-behalf implementations.
   *
   * @see FeedsNodeProcessor::map()
   * @see FeedsUserProcessor::map()
   */
  protected static function loadMappers() {
    static $loaded = FALSE;
    if (!$loaded) {
      $path = drupal_get_path('module', 'feeds') .'/mappers';
      $files = drupal_system_listing('.*\.inc$', $path, 'name', 0);
      foreach ($files as $file) {
        if (strstr($file->filename, '/mappers/')) {
          require_once("./$file->filename");
        }
      }
      // Rebuild cache.
      module_implements('', FALSE, TRUE);
    }
    $loaded = TRUE;
  }
}

/**
 * Used when a plugin is missing.
 */
class FeedsMissingPlugin extends FeedsPlugin {
  public function menuItem() {
    return array();
  }
}
