<?php

/**
 * @file
 * Provide a field handler to present a delete link for taxonomy term revision.
 */

/**
 * Field handler to present a link to delete a taxonomy term to a revision
 */
class views_handler_field_taxonomy_term_revision_link_delete extends views_handler_field {
  /**
   * (non-PHPdoc)
   * @see views_handler_field::construct()
   */
  function construct() {
    parent::construct();
    $this->additional_fields['tid'] = 'tid';
    $this->additional_fields['revision_id'] = 'revision_id';
  }

  /**
   * Returns the user access to the field.
   *
   * @return bool
   *   Return TRUE if user has access to this field handler.
   */
  function access() {
    return user_access('delete taxonomy term revisions') || user_access('administer taxonomy');
  }

  /**
   * (non-PHPdoc)
   * @see views_handler_field::render()
   */
  function render($values) {
    if (!$this->access()) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "taxonomy/term/" . $values->{$this->aliases['tid']} . "/revisions/" . $values->{$this->aliases['revision_id']} . "/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    return $text;
  }
}
