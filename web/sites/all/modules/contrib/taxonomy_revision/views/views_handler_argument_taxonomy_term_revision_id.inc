<?php

/**
 * @file
 * Provide taxonomy term revision_id argument handler.
 */

/**
 * Argument handler to accept a taxonomy term revision id.
 */
class views_handler_argument_taxonomy_term_revision_id extends views_handler_argument_numeric {
  /**
   * Override the behavior of views_handler_argument_numeric::title().
   * @return string
   *   Get the title of the revision.
   */
  function title_query() {
    $query = db_select('taxonomy_term_data_revision', 'tdt')
      ->condition('tdt.tid', $this->value, 'IN')
      ->fields('tdt', array('tid', 'name'));

    $result = $query->execute();

    return $result->fetchAllKeyed();
  }
}
