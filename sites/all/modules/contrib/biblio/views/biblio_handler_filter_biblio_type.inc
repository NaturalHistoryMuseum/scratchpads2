<?php
class biblio_handler_filter_biblio_type extends views_handler_filter_in_operator {

  function construct() {
    parent::construct();
    $this->definition['numeric'] = TRUE;
  }

  function get_value_options() {
    if (!isset($this->value_options)) {
      $result = db_query("SELECT name, tid
              FROM {biblio_types} t
              WHERE t.tid > 0 AND t.visible=1
              ORDER by name");
      $this->value_title = t('Publication type');
      $options = array();
      foreach ($result as $row) {
        $options[$row->tid] =  $row->name;
      }
      $this->value_options = $options;
    }
  }
}
