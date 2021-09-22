<?php

/**
 * @file styles.variables.inc
 * Variable defaults for Styles.
 */

/**
 * Define our constants.
 */

/**
 * This is the variable namespace, automatically prepended to module variables.
 */
define('STYLES_NAMESPACE', 'styles__');

/**
 * Styles constant for user styles in the database.
 */
define('STYLES_STORAGE_NORMAL', 1);

/**
 * Styles constant for user styles that override module-defined styles.
 */
define('STYLES_STORAGE_OVERRIDE', 2);

/**
 * Styles constant for module-defined styles in code.
 */
define('STYLES_STORAGE_DEFAULT', 4);

/**
 * Styles constant to represent an editable preset.
 */
define('STYLES_STORAGE_EDITABLE', STYLES_STORAGE_NORMAL | STYLES_STORAGE_OVERRIDE);

/**
 * Styles constant to represent any module-based preset.
 */
define('STYLES_STORAGE_MODULE', STYLES_STORAGE_OVERRIDE | STYLES_STORAGE_DEFAULT);

/**
 * Wrapper for variable_get() using the Styles variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending STYLES_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the styles_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see styles_variable_set()
 * @see styles_variable_del()
 * @see styles_variable_default()
 */
function styles_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing is still
  // desired.
  if (!isset($default)) {
    $default = styles_variable_default($name);
  }
  // Namespace all variables.
  $variable_name = STYLES_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() using the Styles variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending STYLES_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see styles_variable_get()
 * @see styles_variable_del()
 * @see styles_variable_default()
 */
function styles_variable_set($name, $value) {
  $variable_name = STYLES_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() using the Styles variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending STYLES_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see styles_variable_get()
 * @see styles_variable_set()
 * @see styles_variable_default()
 */
function styles_variable_del($name) {
  $variable_name = STYLES_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Styles namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the STYLES_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see styles_variable_get()
 * @see styles_variable_set()
 * @see styles_variable_del()
 */
function styles_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
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
 *  STYLES_NAMESPACE.
 */
function styles_variable_name($name) {
  return STYLES_NAMESPACE . $name;
}
