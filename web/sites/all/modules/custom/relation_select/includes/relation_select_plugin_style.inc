<?php

/**
 * @file
 * Plugin include file for export style plugin.
 */
/**
 * Generalized style plugin for export plugins.
 *
 * @ingroup views_style_plugins
 */
class relation_select_plugin_style extends views_plugin_style {

  function sanitize_columns($fields = NULL) {
    $sanitized = array();
    if ($fields === NULL) {
      $fields = $this->display->handler->get_option('fields');
    }
    // Preconfigure the sanitized array so that the order is retained.
    foreach ($fields as $field => $info) {
      // Set to itself so that if it isn't touched, it gets column
      // status automatically.
      $sanitized[$field] = $field;
    }
    return $sanitized;
  }

  function option_definition() {
    $options = parent::option_definition();
    // Register the 'lookup_on_change' option so it will be exported with views.
    $options['lookup_on_change'] = array('default' => array());
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $filters = $this->display->handler->get_handlers('filter');
    $lookup_options = array();
    foreach ($filters as $name => $filter) {
      if ($filter->options['exposed']) {
        $lookup_options[$name] = $filter->options['expose']['label'];
      }
    }
    if (count($lookup_options)) {
      $form['lookup_on_change'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Lookup on change'),
        '#default_value' => $this->options['lookup_on_change'],
        '#options' => $lookup_options,
        '#description' => t('A lookup will happen as soon as the exposed filters are changed.')
      );
    }
  }

  function options_validate(&$form, &$form_state) {
    // Remove empty array values so they don't waste memory.
    if (isset($form_state['values']['style_options']['lookup_on_change'])) {
      $form_state['values']['style_options']['lookup_on_change'] = array_filter($form_state['values']['style_options']['lookup_on_change']);
    }
    parent::options_validate($form, $form_state);
  }

  function query() {
    if ($this->view->query->definition['handler'] == 'efq_views_plugin_query') {
      $this->view->efq = TRUE;
    }
    else {
      // We always want the query to be distinct - this also adds the correct base field
      $this->view->query->set_distinct();
      $this->view->efq = FALSE;
    }
  }

  function pre_render($result) {
    // We don't want to output any exposed filters - the form for this is already embeded
    unset($this->view->exposed_widgets);
    return parent::pre_render($result);
  }

  function render() {
    // Add a close button
    $this->view->attachment_before .= theme('relation_select_attachment_before', array(
      'view' => $this->view
    ));
    return parent::render();
  }
}
