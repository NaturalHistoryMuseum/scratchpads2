<?php
// $Id$

/**
 * Implementation of hook_field_views_data
 */
function geofield_field_views_data_alter(&$result, $field, $module) {
  if ($module == 'geofield') {
    foreach ($result as &$r) {
      foreach ($r as &$f) {
        if (isset($f['field']) && is_array($f['field'])) {
          $f['field']['handler'] = 'geofield_handler_field_point';
        }
      }
    }
  }
}
