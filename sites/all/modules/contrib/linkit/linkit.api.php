<?php

/**
 * @file
 * Hooks and alters provided by Linkit.
 */

/**
 * Defines one or more attributes to use with Linkit.
 *
 * All attributes is defined as form elements, and it used both in the Linkit
 * profile form and in the Linkit dashboard.
 *
 * See Drupal FAPI for more info.
 * http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html
 *
 * @param array $profile
 *   The profile settings the user calling this function has.
 * @return
 *   An associative array with form elements with the key being the HTML
 *   attribute name (my_attribute = <a my_attribute="value"></a>).
 */
function hook_linkit_attributes($profile) {
  $attributes['my_attribute'] = array(
    '#type' => 'textfield',
    '#title' => t('My attribute'),
    '#maxlength' => 255,
    '#size' => 40,
    '#default_value' => '',
    '#weight' => isset($profile->data['attributes']['my_attribute']['weight']) ? $profile->data['attributes']['my_attribute']['weight'] : 0,
  );

  return $attributes;
}

/**
 * Alter an attribute before it has been processed.
 *
 * This hook is useful for altering the attribute form array that will be used
 * in both the Linkit profile form and in the Linkit dashboard.
 *
 * @param $attributes
 *   An associative array with form elements defining attributes.
 *
 * @see hook_linkit_attributes()
 */
function hook_linkit_attributes_alter(&$attributes) {
  $attributes['rel']['#type'] = 'select';
  $attributes['rel']['#title'] = t('Rel select');
  $attributes['rel']['#options'] = array(
    '' => t('None'),
    'now-follow' => t('No follow'),
    'other-rel' => t('Other rel'),
  );
}

/**
 * Defines one or more plugins to use with Linkit.
 *
 * @return
 *   An associative array with the key being the machine name for the
 *   implementation and the values being an array with the following keys:
 *     - "title": The untranslated human readable name for the plugin.
 *     - "description": Short untranslated description for the plugin.
 *     - "file": (optional) A file that will be included before the
 *       autocomplete callback function is called.
 *     - "autocomplete callback": The function to call when the users search
 *       for something.
 *     - "path info callback": (optional) The function to call then the user
         have provided something that looks like an URL in the autocomplete
         textfield.
 */
function hook_linkit_plugins() {
  $plugins['myplugin'] = array(
    'title' => 'My plugin',
    'description' => 'My plugin implementation',
    'file' => drupal_get_path('module', 'mymodule') . '/mymodule.inc',
    'autocomplete callback' => 'mymodule_autocomplete_function',
    'path info callback' => 'mymodule_path_info_function'
  );
  return $plugins;
}

/**
 * "autocomplete callback" given in hook_linkit_plugins().
 *
 * This function is called when the user is typing in the autocomplete field
 * and the search is triggerd.
 *
 * Note: If there is no search results, just return an empty array.
 *
 * @param $string
 *   A string contains the text from the autocomplete field.
 * @param array $profile
 *   The profile settings the user calling this function has.
 * @return
 *   An array of search results. Each search result is an associative array
 *   that may contain the following key-value pairs:
 *     - "title": The untranslated title for the item.
 *     - "description": (optional) The untranslated description that will be
 *       shown under the title.
 *     - "path": The path for the item.
 *     - "group": (optional) The untranslated group name. This is used to group
 *       results into different groups, like Content, user, term and so on.
 */
function mymodule_autocomplete_function($string, $profile) {
}

/**
 * "path info callback" given in hook_linkit_plugins().
 *
 * Retrieve relevant information about a URL.
 *
 * Note: Do not return en empty result, return FALSE instead as the first
 * returned value will be used in this call stack.
 *
 * @param $path_info
 *   An associative array containing information about the URL requested.
 * @param array $profile
 *   The profile settings the user calling this function has.
 * @return
 *   An array of search results. Each search result is an associative array
 *   that may contain the following key-value pairs:
 *     - "title": The untranslated title for the item.
 *     - "description": (optional) The untranslated description that will be
 *       shown under the title.
 *     - "path": The path for the item.
 *     - "group": (optional) The untranslated group name. This is used to group
 *       results into different groups, like Content, user, term and so on.
 */
function mymodule_path_info_function($path_info, $profile) {
}


/**
 * If you have a custom scheme, you can define how the relative URL, used by
 * Linkit file plugin will look.
 *
 * public:// and private:// is implemented by Linkit core.
 *
 * @param string $scheme
 *   The scheme for the file.
 * @param string $target
 *   The target for the file.
 * @return
 *   A string with the relative or absolute URL to your file. We use relative
 *   for all Drupal internal paths.
 *
 * @see file_uri_scheme()
 * @see file_uri_target()
 * @see _linkit_file_get_url()
 */
function mymodule_linkit_get_url($scheme, $target) {
  switch ($scheme) {
    case 'mycustomscheme':
      // Get some fancy url to my custom scheme and return it as a string.
      break;
  }
}