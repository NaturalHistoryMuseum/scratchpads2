<?php
/**
 * @file
 * Linkit Plugin interface.
 *
 * Provides an interface and classes to implement Linkit plugins.
 */

/**
 * Linkit plugin interface.
 */
interface LinkitPluginInterface {
  /**
   * This method gets called when searching for items to link to in the
   * autocomplete field.
   */
  public function autocomplete_callback();
}

/**
 *
 * LinkitPlugin.
 */
abstract class LinkitPlugin implements LinkitPluginInterface {

  /**
   * Initialize this plugin with the plugin and the profile.
   *
   * @param array $plugin
   *   The plugin array.
   * @param object $profile
   *   The Linkit profile object.
   */
  function __construct($plugin, $profile) {
    $this->plugin = $plugin;
    $this->profile = $profile;
  }

  /**
   * Return a string representing this handler's name in the UI.
   */
  function ui_title() {
    if (!isset($this->plugin['ui_title'])) {
      return check_plain($this->plugin['module'] . ':' . $this->plugin['name']);
    }
    return check_plain($this->plugin['ui_title']);
  }

  /**
   * Return a string representing this handler's description in the UI.
   */
  function ui_description() {
    if (isset($this->plugin['ui_description'])) {
      return check_plain($this->plugin['ui_description']);
    }
  }

  /**
   * Set the search string.
   *
   * @param string $search_string.
   */
  function setSearchString($search_string) {
    $this->search_string = $search_string;
  }

  /**
   * Build the label that will be used in the search result for each row.
   */
  function buildLabel($label) {
    return check_plain($label);
  }

  /**
   * Build an URL based in the path and the options.
   */
  function buildPath($path, $options = array()) {
    return url($path, $options);
  }

  /**
   * Build the search row description.
   *
   * If there is a "result_description", run it thro token_replace.
   *
   * @param object $data
   *   An object that will be used in the token_place function
   *
   * @see token_replace()
   */
  function buildDescription($data) {
    if (isset($this->profile->data[$this->plugin['name']]['result_description'])) {
      $description =  token_replace(check_plain($this->profile->data[$this->plugin['name']]['result_description']), array(
        $this->plugin_name => $data,
      ), array('clear' => TRUE));
      return $description;
    }
  }

  /**
   * Returns a string to use as the search result group name.
   */
  function buildGroup($group_name) {
    return check_plain($group_name);
  }

  /**
   * Returns a string with CSS classes that will be added to the search result
   * row for this item.
   *
   * @return
   *   A string with CSS classes
   */
  function buildRowClass($row_classes) {
    if (is_array($row_classes)) {
      $row_classes = implode(' ', $row_classes);
    }
    return $row_classes;
  }

  /**
   * Generate a settings form for this handler.
   * Uses the standard Drupal FAPI.
   *
   * @return
   *   An array containing any custom form elements to be displayed in the
   *   profile editing form
   */
  function buildSettingsForm() {}

  /**
   * Determine if the handler is considered 'broken', meaning it's a
   * a placeholder used when a handler can't be found.
   */
  function broken() { }
}

/**
 * A special handler to take the place of missing or broken handlers.
 */
class LinkitPluginBroken extends LinkitPlugin {

  function ui_title() { return t('Broken/missing handler'); }

  function ui_description() {}
  function autocomplete_callback() {}

  /**
   * Determine if the handler is considered 'broken'.
   */
  function broken() { return TRUE; }
}