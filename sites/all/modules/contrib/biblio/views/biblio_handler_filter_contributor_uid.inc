<?php
/**
 * Filter handler for contributors
 */
class biblio_handler_filter_contributor_uid extends views_handler_filter_many_to_one {
  function get_value_options() {
    $result = db_query("SELECT u.name, lastname, firstname, initials, cid, drupal_uid
              FROM {biblio_contributor_data} cd
              INNER JOIN {users} u on u.uid = cd.drupal_uid
              WHERE cd.drupal_uid > 0
              ORDER by lastname, firstname");
    $this->value_options = array();
    foreach ($result as $row) {
      $this->value_options[$row->drupal_uid] =  "$row->lastname, $row->firstname $row->initials ($row->name)";
    }
  }
}
