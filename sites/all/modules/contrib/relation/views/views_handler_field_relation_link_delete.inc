<?php

/**
 * @file
 * Definition of views_handler_field_relation_link_delete.
 */

/**
 * Field handler to present a link to delete a relation.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_relation_link_delete extends views_handler_field_relation_link {

  /**
   * Renders the link.
   */
  function render_link($relation, $values) {
    // Ensure user has access to delete this relation.
    if (!user_access('delete relations')) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "relation/$relation->rid/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('Delete');
    return $text;
  }
}
