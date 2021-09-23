<?php

/**
 * @file
 * Defines the alter query object.
 */

/**
 * Object used to create a SELECT query.
 */
class views_contextual_filters_or_search_api_query extends SearchApiViewsQuery {
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
   * Builds the necessary info to execute the query.
   */
  public function build(&$view) {
    if (!empty($this->errors)) {
      return;
    }

    $this->view = $view;

    // Setup the nested filter structure for this query.
    if (!empty($this->where)) {
      // If the different groups are combined with the OR operator, we have to
      // add a new OR filter to the query to which the filters for the groups
      // will be added.
      if ($this->group_operator === 'OR') {
        $base = $this->query->createFilter('OR');
        $this->query->filter($base);
      }
      else {
        $base = $this->query;
      }
      // Add a nested filter for each filter group, with its set conjunction.
      foreach ($this->where as $group_id => $group) {
        if (!empty($group['conditions']) || !empty($group['filters'])) {
          $group += array('type' => 'AND');

          if ($group_id === '') {
            $filter = $this->options['contextual_filters_or'] ? $this->query->createFilter('OR') : $this->query;
          }
          else {
            $filter = $this->query->createFilter($group['type']);
          }

          if (!empty($group['conditions'])) {
            foreach ($group['conditions'] as $condition) {
              list($field, $value, $operator) = $condition;
              $filter->condition($field, $value, $operator);
            }
          }
          if (!empty($group['filters'])) {
            foreach ($group['filters'] as $nested_filter) {
              $filter->filter($nested_filter);
            }
          }
          // If no group was given, the filters were already set on the query.
          if ($this->options['contextual_filters_or'] || $group_id !== '') {
            $base->filter($filter);
          }
        }
      }
    }

    // Initialize the pager and let it modify the query to add limits.
    $view->init_pager();
    $this->pager->query();

    // Set the search ID, if it was not already set.
    if ($this->query->getOption('search id') == get_class($this->query)) {
      $this->query->setOption('search id', 'search_api_views:' . $view->name . ':' . $view->current_display);
    }

    // Add the "search_api_bypass_access" option to the query, if desired.
    if (!empty($this->options['search_api_bypass_access'])) {
      $this->query->setOption('search_api_bypass_access', TRUE);
    }

    // If the View and the Panel conspire to provide an overridden path then
    // pass that through as the base path.
    if (!empty($this->view->override_path) && strpos(current_path(), $this->view->override_path) !== 0) {
      $this->query->setOption('search_api_base_path', $this->view->override_path);
    }
  }
}
