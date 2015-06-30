<?php

/**
 * @file
 * Views handler to display a results link for Webform submissions.
 */

/**
 * Field handler to present a link node edit.
 */
class webform_handler_field_node_link_results extends views_handler_field_node_link {

  function option_definition() {
    $options = parent::option_definition();
    $options['subpath'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['subpath'] = array(
      '#type' => 'textfield',
      '#title' => t('Subpath'),
      '#default_value' => $this->options['subpath'],
      '#field_prefix' => 'node/NID/webform-results/',
    );
  }

  /**
   * Renders the link.
   */
  function render_link($node, $values) {
    // Ensure user has access to edit this node.
    if (!webform_results_access($node)) {
      return;
    }

    // For clear, ensure user has access to clear all the submissions.
    if (stripos($this->options['subpath'], 'clear') === 0 && !user_access('delete all webform submissions')) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "node/$node->nid/webform-results" .
                                      (strlen($this->options['subpath']) ? '/' . $this->options['subpath'] : '');

    $text = !empty($this->options['text']) ? $this->options['text'] : t('results');
    return $text;
  }

}
