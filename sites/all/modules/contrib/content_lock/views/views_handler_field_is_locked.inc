<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

/**
 * @file
 * Handler to identify if node is locked or not
 */

class views_handler_field_is_locked extends views_handler_field_boolean {
  function construct() {
    parent::construct();
    // Add the field
    // Since we don't have a specific boolean field, we use the timestamp
    $this->additional_fields['timestamp'] = 'timestamp';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $value = $values->content_lock_timestamp ? TRUE : FALSE;
    if (!empty($this->options['not'])) {
      $value = !$value;
    }

    switch ($this->options['type']) {
      case 'yes-no':
      default:
        return $value ? t('Yes') : t('No');
      case 'true-false':
        return $value ? t('True') : t('False');
      case 'on-off':
        return $value ? t('On') : t('Off');
    }
  }
}
