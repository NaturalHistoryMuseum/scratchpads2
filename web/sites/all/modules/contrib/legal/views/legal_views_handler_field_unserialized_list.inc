<?php

/**
 * @file
 * Views fields displaying additional checkboxes.
 */

/**
 * Implementing serialized fields.
 */
class legal_views_handler_field_unserialized_list extends views_handler_field {

  function render($values) {
    $extras = unserialize($values->{$this->field_alias});

    return theme('item_list', $extras);
  }
}
