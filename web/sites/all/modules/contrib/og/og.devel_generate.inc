<?php

/**
 * @file
 * Devel generate integration with Organic groups module.
 */

function og_devel_generate($object, $field, $instance, $bundle) {
  if (field_behaviors_widget('multiple values', $instance) == FIELD_BEHAVIOR_CUSTOM) {
    return og_devel_generate_multiple('_og_devel_generate', $object, $field, $instance, $bundle);
  }
  else {
    return _og_devel_generate($object, $field, $instance, $bundle);
  }
}

function _og_devel_generate($object, $field, $instance, $bundle) {
  $object_field = array();
  if ($allowed_values = og_get_all_group()) {
    $keys = array_keys($allowed_values);
    $object_field['gid'] = $keys[mt_rand(0, count($allowed_values) - 1)];
  }
  return $object_field;
}

/**
 * Return multiple values for the Organic groups audience field.
 *
 * Since devel_generate_multiple() doesn't check the returned random value is
 * unique, we have our own custom code, that makes sure the group ID is valid,
 * so a field will not reference the same group ID twice.
 */
function og_devel_generate_multiple($function, $object, $field, $instance, $bundle) {
  $object_field = array();
  if (function_exists($function)) {
    switch ($field['cardinality']) {
      case FIELD_CARDINALITY_UNLIMITED:
        $max = rand(0, 3); //just an arbitrary number for 'unlimited'
        break;
      default:
        $max = $field['cardinality'] - 1;
        break;
    }
    // Hold an array with the already returned values.
    $used_values = array();
    for ($i = 0; $i <= $max; $i++) {
      $result = $function($object, $field, $instance, $bundle);
      if (!empty($result)) {
        // Make sure the value is unique.
        if (empty($used_values[$result['gid']])) {
          $object_field[$i] = $result;
        }
        $used_values[$result['gid']] = TRUE;
      }
    }
  }
  return $object_field;
}
