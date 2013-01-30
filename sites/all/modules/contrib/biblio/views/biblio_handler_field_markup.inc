<?php
class biblio_handler_field_markup extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields = array();
    $this->additional_fields['biblio_formats'] = 'biblio_formats';

  }

  function render($values) {
    $value = $this->get_value($values);
    $formats = $this->get_value($values, 'biblio_formats');
    $format = filter_default_format();
    if (!empty($formats) ) {
      $formats = unserialize($formats);
      $format  = isset($formats[$this->field]) ? $formats[$this->field] : $format;
    }
    if ($value) {
      $value = str_replace('<!--break-->', '', $value);
      return check_markup($value, $format, '');
    }
  }



}