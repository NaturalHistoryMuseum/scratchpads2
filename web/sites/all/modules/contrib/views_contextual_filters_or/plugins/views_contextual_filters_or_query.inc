<?php

/**
 * @file
 * Defines the alter query object.
 */

/**
 * Object used to create a SELECT query.
 */
class views_contextual_filters_or_query extends views_plugin_query_default {
  function option_definition() {
    $options = parent::option_definition();

    $options['contextual_filters_or'] = array(
      'default' => FALSE,
      'translatable' => FALSE,
      'bool' => TRUE,
    );

    return $options;
  }

  /**
   * Add settings for the ui.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['contextual_filters_or'] = array(
      '#title' => t('Contextual filters OR'),
      '#description' => t('Contextual filters applied to OR logic.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['contextual_filters_or']),
    );
  }

  /**
   * Add a simple WHERE clause to the query. The caller is responsible for
   * ensuring that all fields are fully qualified (TABLE.FIELD) and that
   * the table already exists in the query.
   *
   * TODO : Perhaps this function is not needed.
   */
  function add_where($group, $field, $value = NULL, $operator = NULL) {
    parent::add_where($group, $field, $value, $operator);

    // Check for a group.
    if ($group === 0 && isset($this->where[$group])) {
      $op = $this->options['contextual_filters_or'] ? 'OR' : 'AND';
      $this->set_where_group($op, $group);
    }
  }

  /**
   * Add a complex WHERE clause to the query.
   */
  function add_where_expression($group, $snippet, $args = array()) {
    parent::add_where_expression($group, $snippet, $args);

    // Check for a group.
    if ($group === 0 && isset($this->where[$group])) {
      $op = $this->options['contextual_filters_or'] ? 'OR' : 'AND';
      $this->set_where_group($op, $group);
    }
  }
}
