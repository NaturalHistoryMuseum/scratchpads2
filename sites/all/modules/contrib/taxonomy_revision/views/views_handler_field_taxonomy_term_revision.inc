<?php

/**
 * @file
 * Contains the basic 'taxonomy_revision' field handler.
 */

/**
 * Field handler for the revision_id column.
 */
class views_handler_field_taxonomy_term_revision extends views_handler_field_taxonomy {
  /**
   * (non-PHPdoc)
   * @see views_handler_field_taxonomy::init()
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_taxonomy_revision'])) {
      $this->additional_fields['tid'] = 'tid';
      $this->additional_fields['revision_id'] = 'revision_id';
    }
  }

  /**
   * (non-PHPdoc)
   * @see views_handler_field_taxonomy::option_definition()
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_taxonomy_revision'] = array('default' => FALSE);
    return $options;
  }

  /**
   * (non-PHPdoc)
   * @see views_handler_field_taxonomy::options_form()
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy_revision'] = array(
      '#title' => t('Link this field to its taxonomy term revision'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy_revision']),
    );
  }

  /**
   * (non-PHPdoc)
   * @see views_handler_field_taxonomy::render_link()
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_taxonomy_revision']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "taxonomy/term/" . $values->{$this->aliases['tid']} . '/revisions/' . $values->{$this->aliases['revision_id']} . '/view';
    }
    else {
      return parent::render_link($data, $values);
    }
    return $data;
  }
}
