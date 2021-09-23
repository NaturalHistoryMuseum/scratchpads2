<?php

/**
 * @file
 * Definition of MediaBrowserPluginInterface.
 */

/**
 * Defines a Media browser plugin.
 *
 * Extends the MediaBrowserPluginInterface with methods expected by all
 * Media browser classes.
 */
interface MediaBrowserPluginInterface {
  /**
   * Set up the plugin class.
   *
   * @param $info
   *   An array of plugin info from hook_media_browser_plugin_info()
   *   implementations.
   * @param $params
   *   An array of parameters which came in is $_GET['params']. The expected
   *   parameters are still being defined.
   *   - 'types': array of media types to support
   *   - 'multiselect': boolean; TRUE enables multiselect
   */
  public function __construct($info, $params);

  /**
   * Check if a user can access this plugin.
   *
   * @param object $account
   *   An optional user account object from user_load(). Defaults to the current
   *   global user.
   *
   * @return boolean
   *   TRUE if the user can access this plugin, or FALSE otherwise.
   */
  public function access($account = NULL);

  // The view() method is an abstract function so it is defined in MediaBrowserPlugin.
  // public function view();
}
