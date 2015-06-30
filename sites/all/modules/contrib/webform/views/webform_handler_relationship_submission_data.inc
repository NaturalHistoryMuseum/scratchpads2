<?php
/**
 * @file
 * Views' relationship handlers.
 */

class webform_handler_relationship_submission_data extends views_handler_relationship  {

  function option_definition() {
    $options = parent::option_definition();
    $options['webform_nid'] = array('default' => NULL);
    $options['webform_cid'] = array('default' => NULL);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    form_load_include($form_state, 'inc', 'webform', 'views/webform.views');

    $nid = (int) $this->options['webform_nid'];
    $cid = (int) $this->options['webform_cid'];

    // Helper function provides webform_nid and webform_cid options.
    _webform_views_options_form($form, $form_state, $nid, $cid);
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    _webform_views_options_validate($form, $form_state);
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
    _webform_views_options_submit($form, $form_state);
  }

  /**
   * Called to implement a relationship in a query.
   *
   * It respects the given component ids, provided via options form.
   */
  function query() {
    $this->definition['extra'][] = array(
      'table' => NULL,
      'field' => "%alias.nid",
      'value' => $this->options['webform_nid'],
    );
    $this->definition['extra'][] = array(
      'table' => NULL,
      'field' => "%alias.cid",
      'value' => $this->options['webform_cid'],
    );

    // The rest of building the join is performed by the parent.
    parent::query();
  }
}
