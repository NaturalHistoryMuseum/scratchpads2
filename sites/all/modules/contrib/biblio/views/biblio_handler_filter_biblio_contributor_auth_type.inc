<?php
class biblio_handler_filter_biblio_contributor_auth_type extends views_handler_filter_in_operator {

  function construct() {
    parent::construct();
    $this->definition['numeric'] = TRUE;
  }

  function get_value_options() {
    if (!isset($this->value_options)) {
      $result = db_query("SELECT title, auth_type
              FROM {biblio_contributor_type_data}
              ORDER by auth_type");
      $this->value_title = t('Author type');
      $options = array();

      foreach ($result as $row) {
        $options[$row->auth_type] =  $row->title;
      }
      $this->value_options = $options;
    }
  }
}
