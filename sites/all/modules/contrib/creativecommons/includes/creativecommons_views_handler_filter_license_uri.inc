<?php

/**
 * @file
 * Filter by license type.
 *
 */

class creativecommons_views_handler_filter_license_uri extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('License Types');
      $available_license_types = creativecommons_get_available_license_types();
      $license_array_keys = creativecommons_get_licenses(array('jurisdiction' => variable_get('creativecommons_default_jurisdiction', '')), TRUE);
        
      foreach ($license_array_keys as $key => $info) {
        if (is_array($info)) {
          foreach ($info as $key => $value) {
            if (in_array($key, $available_license_types)) {
              $options[$key] = $value;
            }
          }
        } 
        else {
          if (in_array($key, $available_license_types)) {
            $options[$key] = $info;
          }
        }
      }
      $this->value_options = $options;
    }
  }
  function query() {

    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();

    if ($this->operator == 'not in') {
      $not = ' NOT ';
    } 
    foreach ($this->value as $value) {
      if (strlen($where)) {
        if ($this->operator == 'not in') {
          $or = ' AND ';
        } 
        else {
          $or = ' OR ';
        }
      }
      if (strlen($value)) {
        $like = '\'http://creativecommons.org/%/' . $value . '/%\' ';
      }
      else {
        $like = '\'\'';
      }
      $where = " $this->table_alias.$this->real_field $not" . " LIKE " . $like . $or . $where;     
    }
    $this->query->add_where($this->options['group'], $where);
  }

}
