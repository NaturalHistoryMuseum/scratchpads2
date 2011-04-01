<?php
/**
 * Filter handler for contributors
 */
class biblio_handler_filter_contributor extends views_handler_filter_many_to_one {
  function get_value_options() {
    $result = db_query("SELECT lastname, firstname, initials, cid
              	  FROM {biblio_contributor_data} cd
                  ORDER by lastname, firstname");
    $this->value_options = array();
    foreach ($result as $row) {
      $this->value_options[$row->cid] =  "$row->lastname, $row->firstname $row->initials";
    }
  }
}
