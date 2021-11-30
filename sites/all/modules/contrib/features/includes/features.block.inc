<?php

/**
 * @file
 * Features integration for 'block' module.
 *
 * @todo This seems to be obsolete and not doing anything. See #3076925.
 */

/**
 * Implements hook_features_api().
 */
function block_features_api() {
  return array();
}

/**
 * Implements hook_features_export().
 */
function block_features_export($data, &$export) {
  $pipe = array();
  foreach ($data as $bid) {
    $split = explode('-', $bid);
    $module = array_shift($split);
    $delta = implode('-', $split);

    $export['dependencies'][$module] = $module;

    switch ($module) {
      case 'views':
        if (strlen($delta) == 32) {
          $hashes = variable_get('views_block_hashes', array());
          if (!empty($hashes[$delta])) {
            $delta = $hashes[$delta];
          }
        }

        $delta_split = explode('-', $delta);
        $view_name = $delta_split[0];
        if (!empty($view_name)) {
          $pipe['views'][] = $view_name;
        }
        break;
    }
  }
  return $pipe;
}
