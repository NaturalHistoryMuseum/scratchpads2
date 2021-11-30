<?php
/**
 * @file
 * Context integration for Apachesolr.
 */

/**
 * Expose Apachesolr pages as a context condition.
 */
class apachesolr_context_page_condition extends context_condition {
  /**
   * Override of condition_values().
   */
  function condition_values() {
    // Retrieve all Apachesolr pages to return a list of conditions.
    $values = array();
    foreach (apachesolr_search_load_all_search_pages() as $page) {
      $values[$page['page_id']] = check_plain($page['label']);
    }
    return $values;
  }

  /**
   * Override of execute().
   *
   * @param Array $search_page The loaded search page.
   *   Loaded in apachesolr_search_custom_page().
   */
  function execute($search_page) {
    foreach ($this->get_contexts($search_page['page_id']) as $context) {
      // Set contexts for this Apachesolr page.
      $this->condition_met($context, $search_page['page_id']);
    }
  }
}
