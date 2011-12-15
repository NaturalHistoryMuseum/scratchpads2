<?php

/**
 * @file file_styles/includes/file_styles.variables.inc
 * Variable defaults for File Styles.
 */

/**
 * Define our constants.
 */

/**
 * This is the variable namespace, automatically prepended to module variables.
 */
define('FILE_STYLES_NAMESPACE', 'file_styles__');

/**
 * Wrapper for variable_get() using the File Styles variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending FILE_STYLES_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the file_styles_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see file_styles_variable_set()
 * @see file_styles_variable_del()
 * @see file_styles_variable_default()
 */
function file_styles_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing is still
  // desired.
  if (!isset($default)) {
    $default = file_styles_variable_default($name);
  }
  // Namespace all variables.
  $variable_name = FILE_STYLES_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() using the File Styles variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending FILE_STYLES_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see file_styles_variable_get()
 * @see file_styles_variable_del()
 * @see file_styles_variable_default()
 */
function file_styles_variable_set($name, $value) {
  $variable_name = FILE_STYLES_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() using the File Styles variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending FILE_STYLES_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see file_styles_variable_get()
 * @see file_styles_variable_set()
 * @see file_styles_variable_default()
 */
function file_styles_variable_del($name) {
  $variable_name = FILE_STYLES_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the File Styles namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the FILE_STYLES_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see file_styles_variable_get()
 * @see file_styles_variable_set()
 * @see file_styles_variable_del()
 */
function file_styles_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
      'image_style_preview_image' => variable_get('image_style_preview_image', drupal_get_path('module', 'image') . '/sample.png'),
      'preview_image_directory' => 'file-styles',
      'preview_image' => '',
    );
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  FILE_STYLES_NAMESPACE.
 */
function file_styles_variable_name($name) {
  return FILE_STYLES_NAMESPACE . $name;
}
