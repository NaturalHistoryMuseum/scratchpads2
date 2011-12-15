<?php
/**
 * @file
 */


/**
 * Extends Content Handler Filter Many To One for CCK Select Other
 */
class cck_select_other_handler_filter extends views_handler_filter_field_list {

  function get_value_options() {
    // This is a gigantic hack because views and views documentation suck.
    $n = preg_match_all("/(\S+):(\S+)/", $this->definition['help'], $matches);
    $entity_type = isset($matches[1]) ? $matches[1] : '';
    $bundle = isset($matches[2]) ? $matches[2] : '';

    $instance = field_read_instance($entity_type, $this->definition['field_name'], $bundle);
    $this->value_options = cck_select_other_options($instance);
  }

  function query() {
    if (isset($this->value[0]) && $this->value[0] == 'other') {
    }
    else {
      parent::query();
    }
  }
}
