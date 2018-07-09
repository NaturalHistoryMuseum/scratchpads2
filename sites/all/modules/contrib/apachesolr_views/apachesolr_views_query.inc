<?php

/**
 * @file
 * Views query plugin for Apache Solr Views.
 * Gets its data not from the database, but from a Solr server.
 */

class apachesolr_views_query extends views_plugin_query {

  /**
   * Array of parameters for Solr query.
   */
  protected $params;
  protected $query_params;
  public $orderby = array();

  /**
   * Store results of apachesolr search.
   */
  protected $apachesolr_results;

  /**
   * Array of where conditions.
   *
   * Neede for grouppin of query conditions.
   */
  protected $where = array();

  /**
   * The default group operator.
   *
   * @var string
   */
  protected $group_operator = 'AND';

  /**
   * Builds what is necessary prior to executing the query.
   */
  public function build(&$view) {
    $view->init_pager();

    // Let the pager modify the query to add limits.
    $this->pager->query();

    // Add fields to the query so they will be shown in solr document.
    $this->params['fl'] = array_keys($view->field);

    $params = array();
    if (isset($this->params['q'])) {
      $params['q'] = $this->params['q'];
    }

    $params['rows'] = $this->pager->options['items_per_page'];
    $params['start'] = $this->pager->current_page * $this->pager->options['items_per_page'];

    // If we display all items without pager.
    if ($params['rows'] == 0) {
      $params['rows'] = 100000;
    }

    // Add fields.
    $params['fl'] = array('id', 'entity_id');
    if (isset($this->params['fl'])) {
      $params['fl'] = array_merge($params['fl'], $this->params['fl']);
    }
    $params['fl'] = implode(',', $params['fl']);

    $where = $this->where;
    // Remove any empty conditions (exposed filters), they will cause an error.
    foreach ($where as &$where_condition) {
      foreach ($where_condition['conditions'] as $index => $condition) {
        if ($condition['value'] == '') {
          unset($where_condition['conditions'][$index]);
        }
      }
    }
    // Add conditions to filter parameter.
    $conditions = array('conditions' => $where, 'type' => $this->group_operator);
    $conditions_string = $this->build_where_string($conditions);

    if (!empty($conditions_string)) {
      $params['fq'] = $conditions_string;
    }

    // Set query type if it is present.
    if (isset($this->params['defType'])) {
      $params['defType'] = $this->params['defType'];
    }

    $this->query_params = $params;

    // Export parameters for preview.
    $view->build_info['query'] = var_export($params, TRUE);
  }

  /**
   * Let modules modify the query just prior to finalizing it.
   */
  public function alter(&$view) {
    foreach (module_implements('views_query_alter') as $module) {
      $function = $module . '_views_query_alter';
      $function($view, $this);
    }
  }

  /**
   * Executes the query.
   *
   * Assigns the resulting values to the view object.
   * Values to set: $view->result, $view->total_rows, $view->execute_time.
   */
  public function execute(&$view) {
    try {
      $start = microtime(TRUE);

      // Execute the search.


      // Load search query.
      // Get the Apache Solr "environment id".
      if (strpos($view->base_table, 'apachesolr__') === 0) {
        $env_id = substr($view->base_table, 12);
      }
      else {
        $env_id = apachesolr_default_environment();
      }

      $solr = apachesolr_get_solr($env_id);
      $context = array(
        'search_type' => 'apachesolr_views_query',
        'view_name' => $view->name,
        'current_display' => $view->current_display,
      );

      $query = new ApachesolrViewsSolrBaseQuery('apachesolr', $solr, $this->query_params, '', current_path(), $context, $view);

      // Add sorting. The setSolrsort method can't be used, because it doesn't support multiple sorting criteria.
      $query->replaceParam('sort', $this->orderby);

      $query->page = $this->pager->current_page;

      // Boost parameters if apachesolr_search module is available.
      apachesolr_search_add_boost_params($query);

      // Execute search.
      list($final_query, $response) = apachesolr_do_query($query);
      apachesolr_has_searched($solr->getId(), TRUE);

      // Store results.
      $view->result = $response->response->docs;

      // Store apachesolr cached response.
      $this->apachesolr_response = $response;

      // Store the results.
      $this->pager->total_items = $view->total_rows = $this->apachesolr_response->response->numFound;
      $this->pager->update_page_info();
    }
    catch (Exception $e) {
      $view->result = array();
      $view->total_rows = 0;
      if (!empty($view->live_preview)) {
        drupal_set_message($e->getMessage(), 'error');
      }
      else {
        vpr('Exception in @human_name[@view_name]: @message', array('@human_name' => $view->human_name, '@view_name' => $view->name, '@message' => $e->getMessage()));
      }
    }

    $view->execute_time = microtime(TRUE) - $start;
  }

  public function add_filter($type, $value, $exclude = FALSE) {
    $exclude_string = ($exclude) ? '-' : '';
    $this->params['filters'][] = $exclude_string . $type . ':(' . $value . ')';
  }

  public function add_filter_string($string) {
    $this->params['q.alt'][] = $string;
  }

  public function add_sort($field, $order) {
    $this->orderby[] = "$field $order";
  }

  public function add_parameter($key, $value) {
    $this->params[$key] = $value;
  }

  public function add_field($table_alias, $field, $alias = '', $params = array()) {
    // Make sure an alias is assigned.
    $alias = $alias ? $alias : $field;
    return $alias;
  }

  public function get_params() {
    return $this->params;
  }

  /**
   *  Build filter string from where array.
   */
  function build_where_string($where) {
    if (!isset($where['conditions'])) {
      return $where['field'] . ':(' . $where['value'] . ')';
    }
    else {
      $condition_strings = array();
      foreach ($where['conditions'] as $condition) {
        $condition_strings[] = $this->build_where_string($condition);
      }
      $condition_strings = array_filter($condition_strings);
      $condition_string = implode(' ' . $where['type'] . ' ', $condition_strings);
      // Respect grouping by wrapping multiple conditions with parenthesis.
      if (count($condition_strings) > 1) {
        $condition_string = '(' . $condition_string . ')';
      }
      return $condition_string;
    }
  }

  /**
   * Support for groupping.
   *
   * @see views_plugin_query_default::add_where().
   */
  function add_where($group, $field, $value = NULL, $operator = NULL) {
    // Ensure all variants of 0 are actually 0. Thus '', 0 and NULL are all
    // the default group.
    if (empty($group)) {
      $group = 0;
    }

    // Check for a group.
    if (!isset($this->where[$group])) {
      $this->set_where_group('AND', $group);
    }

    $this->where[$group]['conditions'][] = array(
      'field' => $field,
      'value' => $value,
      'operator' => $operator,
    );
  }

  /**
   * Support for groupping.
   *
   * @see views_plugin_query_default::set_where_group().
   */
  function set_where_group($type = 'AND', $group = NULL, $where = 'where') {
    // Set an alias.
    $groups = &$this->$where;

    if (!isset($group)) {
      $group = empty($groups) ? 1 : max(array_keys($groups)) + 1;
    }

    // Create an empty group
    if (empty($groups[$group])) {
      $groups[$group] = array('conditions' => array(), 'args' => array());
    }

    $groups[$group]['type'] = strtoupper($type);
    return $group;
  }

  /**
   * Implement ensure_table, do nothing.
   */
  function ensure_table($table, $relationship = NULL, $join = NULL) {
  }
}
