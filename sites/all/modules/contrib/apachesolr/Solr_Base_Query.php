<?php

class SolrBaseQuery implements DrupalSolrQueryInterface {

  /**
   * Extract all uses of one named field from a filter string e.g. 'bundle:book'
   */
  public function filter_extract(&$filterstring, $name) {
    $extracted = array();
    $name = preg_quote($name, '/');
    // Range queries.  The "TO" is case-sensitive.
    $patterns[] = '/(^| |-)'. $name .':([\[\{](\S+) TO (\S+)[\]\}])/';
    // Match quoted values.
    $patterns[] = '/(^| |-)'. $name .':"([^"]*)"/';
    // Match unquoted values.
    $patterns[] = '/(^| |-)'. $name .':([^ ]*)/';
    foreach ($patterns as $p) {
      if (preg_match_all($p, $filterstring, $matches, PREG_SET_ORDER)) {
        // Sort matches longest to shortest to avoid accidentally
        // removing a sub-string.
        usort($matches, array($this, 'filter_extract_cmp'));
        foreach ($matches as $match) {
          $filter = array();
          $filter['#query'] = $match[0];
          $filter['#exclude'] = ($match[1] == '-');
          $filter['#value'] = trim($match[2]);
          // Empty values cause Lucene parse errors, so skip them.
          if (strlen($filter['#value'])) {
            if (isset($match[3])) {
              // Extra data for range queries
              $filter['#start'] = $match[3];
              $filter['#end'] = $match[4];
            }
            $extracted[] = $filter;
          }
          // Update the local copy of $filters by removing the match.
          $filterstring = str_replace($match[0], '', $filterstring);
        }
      }
    }
    return $extracted;
  }

  public function filter_extract_cmp($a, $b) {
    if (strlen($a[0]) == strlen($b[0])) {
      return 0;
    }
    return (strlen($a[0]) > strlen($b[0])) ? -1 : 1;
  }

  /**
   * Takes an array $field and combines the #name and #value in a way
   * suitable for use in a Solr query.
   */
  public function make_filter(array $filter) {
    // If the field value has spaces, or : in it, wrap it in double quotes.
    // unless it is a range query.
    if (preg_match('/[ :]/', $filter['#value']) && !isset($filter['#start']) && !preg_match('/[\[\{]\S+ TO \S+[\]\}]/', $filter['#value'])) {
      $filter['#value'] = '"' . $filter['#value'] . '"';
    }
    $prefix = empty($filter['#exclude']) ? '' : '-';
    return $prefix . $filter['#name'] . ':' . $filter['#value'];
  }

  /**
   * Static shared by all instances, used to increment ID numbers.
   */
  protected static $idCount = 0;

  /**
   * Each query/subquery will have a unique ID
   */
  public $id;

  /**
   * The parameters that get sent to Solr.
   */
  public $params = array('start' => 0, 'rows' => 10);

  /**
   * A keyed array where the key is a position integer and the value
   * is an array with #name and #value properties.  Each value is a
   * used for filter queries, e.g. array('#name' => 'is_uid', '#value' => 0)
   * for anonymous content.
   */
  protected $fields = array();
  protected $fields_added = array();
  protected $fields_removed = array();

  /**
   * The complete filter string for a query.  Usually from $_GET['filters']
   * Contains name:value pairs for filter queries.  For example,
   * "bundle:book" for book nodes.
   */
  protected $filterstring;

  /**
   * A mapping of field names from the URL to real index field names.
   */
  protected $field_map = array();

  /**
   * An array of subqueries.
   */
  protected $subqueries = array();

  /**
   * The search keywords.
   */
  protected $keys;

  /**
   * The search base path.
   */
  protected $base_path;

  /**
   * Apache_Solr_Service object
   */
  protected $solr;

  protected $available_sorts;

  // Makes sure we always have a valid sort.
  protected $solrsort = array('#name' => 'score', '#direction' => 'desc');

  /**
   * @param $solr
   *   An instantiated Apache_Solr_Service Object.
   *   Can be instantiated from apachesolr_get_solr().
   *
   * @param $keys
   *   The string that a user would type into the search box. Suitable input
   *   may come from search_get_keys().
   *
   * @param $filterstring
   *   Key and value pairs that are applied as filter queries.
   *
   * @param $sortstring
   *   Visible string telling solr how to sort - added to GET query params.
   *
   * @param $base_path
   *   The search base path (without the keywords) for this query, without trailing slash.
   */
  function __construct($solr, $keys, $filterstring, $sortstring, $base_path) {
    $this->solr = $solr;
    $this->keys = trim($keys);
    $this->filterstring = trim($filterstring);
    $this->parse_filters();
    $this->available_sorts = $this->default_sorts();
    $this->sortstring = trim($sortstring);
    $this->parse_sortstring();
    $this->base_path = $base_path;
    $this->id = ++self::$idCount;
  }

  function __clone() {
    $this->id = ++self::$idCount;
  }

  public function get_filters($name = NULL) {
    if (empty($name)) {
      return $this->fields;
    }
    reset($this->fields);
    $matches = array();
    foreach ($this->fields as $filter) {
      if ($filter['#name'] == $name) {
        $matches[] = $filter;
      }
    }
    return $matches;
  }

  public function has_filter($name, $value) {
    foreach ($this->fields as $pos => $values) {
      if (isset($values['#name']) && isset($values['#value']) && $values['#name'] == $name && $values['#value'] == $value) {
        return TRUE;
      }
    }
    return FALSE;
  }

  public function add_filter($name, $value, $exclude = FALSE, $callbacks = array()) {
    $filter = array('#exclude' => $exclude, '#name' => $name, '#value' => trim($value), '#callbacks' => $callbacks);
    // Record the addition.
    $this->fields_added[] = $filter;
    // Add to the public list of filters.
    $this->fields[] = $filter;
    // Remove from the record of removed filters.
    $this->unset_filter($this->fields_removed, $name, $value);
    return $this;
  }

  public function remove_filter($name, $value = NULL) {
    // We can only remove named fields.
    if (empty($name)) {
      return;
    }
    // Record the removal.
    $this->fields_removed[$name][] = $value;
    // Remove from the public list of filters.
    $this->unset_filter($this->fields, $name, $value);
    // Remove from the record of added filters.
    $this->unset_filter($this->fields_added, $name, $value);
    return $this;
  }

  protected function unset_filter(&$fields, $name, $value) {
    if (!isset($value)) {
      foreach ($fields as $pos => $values) {
        if ($values['#name'] == $name) {
          unset($fields[$pos]);
        }
      }
    }
    else {
      foreach ($fields as $pos => $values) {
        if ($values['#name'] == $name && $values['#value'] == $value) {
          unset($fields[$pos]);
        }
      }
    }
  }

  /**
   * Handle aliases for field to make nicer URLs
   *
   * @param $field_map
   *   An array keyed with real Solr index field names, with value being the alias.
   *
   * @return SolrBaseQuery
   *   The called object.
   */
  public function add_field_aliases($field_map) {
    $this->field_map = array_merge($this->field_map, $field_map);
    // We have to re-parse the filters.
    $this->parse_filters();
    return $this;
  }

  public function get_field_aliases() {
    return $this->field_map;
  }

  public function clear_field_aliases() {
    $this->field_map = array();
    // We have to re-parse the filters.
    $this->parse_filters();
    return $this;
  }

  public function get_keys() {
    return $this->keys;
  }

  public function set_keys($keys) {
    $this->keys = $keys;
    return $this;
  }

  public function remove_keys() {
    $this->keys = '';
    return $this;
  }

  public function add_subquery(DrupalSolrQueryInterface $query, $fq_operator = 'OR', $q_operator = 'AND') {
    $this->subqueries[$query->id] = array('#query' => $query, '#fq_operator' => $fq_operator, '#q_operator' => $q_operator);
    return $this;
  }

  public function remove_subquery(DrupalSolrQueryInterface $query) {
    unset($this->subqueries[$query->id]);
    return $this;
  }

  public function remove_subqueries() {
    $this->subqueries = array();
    return $this;
  }

  protected function parse_sortstring() {
    // Substitute any field aliases with real field names.
    $sortstring = strtr($this->sortstring, array_flip($this->field_map));
    // Score is a special case - it's the default sort for Solr.
    if ('' == $sortstring) {
      $this->set_solrsort('score', 'desc');
    }
    else {
      // Validate and set sort parameter
      $fields = implode('|', array_keys($this->available_sorts));
      if (preg_match('/^(?:('. $fields .') (asc|desc),?)+$/', $sortstring, $matches)) {
        // We only use the last match.
        $this->set_solrsort($matches[1], $matches[2]);
      }
    }
  }

  /**
   * Returns a default list of sorts.
   */
  protected function default_sorts() {
    // The array keys must always be real Solr index fields.
    return array(
      'score' => array('title' => t('Relevancy'), 'default' => 'desc'),
      'sort_label' => array('title' => t('Title'), 'default' => 'asc'),
      'bundle' => array('title' => t('Type'), 'default' => 'asc'),
      'sort_name' => array('title' => t('Author'), 'default' => 'asc'),
      'ds_created' => array('title' => t('Date'), 'default' => 'desc'),
    );
  }

  public function get_available_sorts() {
    return $this->available_sorts;
  }

  public function set_available_sort($name, $sort) {
    // We expect non-aliased sorts to be added.
    $this->available_sorts[$name] = $sort;
    // Re-parse the sortstring.
    $this->parse_sortstring();
    return $this;
  }

  public function remove_available_sort($name) {
    unset($this->available_sorts[$name]);
    // Re-parse the sortstring.
    $this->parse_sortstring();
    return $this;
  }

  public function get_solrsort() {
    return $this->solrsort;
  }

  public function set_solrsort($name, $direction) {
    if (isset($this->available_sorts[$name])) {
      $this->solrsort = array('#name' => $name, '#direction' => $direction);
    }
    return $this;
  }

  /**
   * Return the search path (including the search keywords).
   *
   * @param string $new_keywords
   *   Optional. When set, this string overrides the query's current keywords.
   */
  public function get_path($new_keywords = NULL) {
    if (isset($new_keywords)) {
      return $this->base_path . '/' . $new_keywords;
    }
    return $this->base_path . '/' . $this->get_query_basic();
  }

  public function get_url_queryvalues() {
    $queryvalues = array();
    $filters = array();
    foreach ($this->fields as $pos => $field) {
      // Look for a field alias.
      if (isset($this->field_map[$field['#name']])) {
        $field['#name'] = $this->field_map[$field['#name']];
      }
      $filters[] = $this->make_filter($field);
    }
    if ($filters) {
      $queryvalues['filters'] = implode(' ', $filters);
    }
    $solrsort = $this->solrsort;
    if ($solrsort && ($solrsort['#name'] != 'score' || $solrsort['#direction'] != 'desc')) {
      if (isset($this->field_map[$solrsort['#name']])) {
        $solrsort['#name'] = $this->field_map[$solrsort['#name']];
      }
      $queryvalues['solrsort'] = $solrsort['#name'] .' '. $solrsort['#direction'];
    }
    return $queryvalues;
  }

  public function get_query_basic() {
    return $this->rebuild_query();
  }

  public function get_fq() {
    return $this->rebuild_fq();
  }

  /**
   * Build additional breadcrumb elements relative to $base.
   */
  public function get_breadcrumb($base = NULL) {
    $breadcrumb = array();

    $progressive_crumb = array();
    if (!isset($base)) {
      $base = $this->get_path();
    }

    $search_keys = $this->get_query_basic();
    if ($search_keys) {
      $breadcrumb[] = l($search_keys, $base);
    }

    foreach ($this->fields as $field) {
      $name = $field['#name'];
      // Look for a field alias.
      if (isset($this->field_map[$name])) {
        $field['#name'] = $this->field_map[$name];
      }
      $progressive_crumb[] = $this->make_filter($field);
      $options = array('query' => array('filters' => implode(' ', $progressive_crumb)));
      $breadcrumb_name = 'apachesolr_breadcrumb_' . $name;
      // Modules utilize this alter to consolidate several fields into one
      // theme function. This is how CCK breadcrumbs are handled.
      drupal_alter('apachesolr_theme_breadcrumb', $breadcrumb_name);
      if ($themed = theme($breadcrumb_name, array('field' => $field))) {
        $breadcrumb[] = l($themed, $base, $options);
      }
      else {
        $breadcrumb[] = l($field['#value'], $base, $options);
      }
    }

    if (!empty($breadcrumb)) {
      // The last breadcrumb is the current page, so it shouldn't be a link.
      $last = count($breadcrumb) - 1;
      $breadcrumb[$last] = strip_tags($breadcrumb[$last]);
    }

    return $breadcrumb;
  }

  /**
   * Parse the filter string in $this->filters into $this->fields.
   *
   * Builds an array of field name/value pairs.
   */
  protected function parse_filters() {
    $this->fields = array();
    $parsed_fields = array();
    $filterstring = $this->filterstring;

    // Gets information about the fields already in solr index.
    $index_fields = $this->solr->getFields();
    foreach ((array) $index_fields as $name => $data) {
      // Look for a field alias.
      $alias = isset($this->field_map[$name]) ? $this->field_map[$name] : $name;
      // Get the values for $name
      $extracted = $this->filter_extract($filterstring, $alias);
      if (count($extracted)) {
        // A trailing space is required since we match all individual
        // filter terms using a trailing space.
        $filter_pos_string = $this->filterstring . ' ';
        foreach ($extracted as $filter) {
          // The trailing space on $filter['#query'] avoids incorrect
          //  matches to a substring. See http://drupal.org/node/891962
          $pos = strpos($filter_pos_string, $filter['#query'] . ' ');
          // $solr_keys and $solr_crumbs are keyed on $pos so that query order
          // is maintained. This is important for breadcrumbs.
          $filter['#name'] = $name;
          $parsed_fields[$pos] = $filter;
        }
      }
    }
    // Even though the array has the right keys they are likely in the wrong
    // order. ksort() sorts the array by key while maintaining the key.
    ksort($parsed_fields);
    foreach ($this->fields_removed as $name => $values) {
      foreach ($values as $val) {
        $this->unset_filter($parsed_fields, $name, $val);
      }
    }
    $this->fields = array_merge(array_values($parsed_fields), $this->fields_added);
  }

  /**
   * Builds a set of filter queries from $this->fields and all subqueries.
   *
   * Returns an array of strings that can be combined into
   * a URL query parameter or passed to Solr as fq paramters.
   */
  protected function rebuild_fq($aliases = FALSE) {
    $fq = array();
    $fields = array();
    foreach ($this->fields as $pos => $field) {
      // Look for a field alias.
      if ($aliases && isset($this->field_map[$field['#name']])) {
        $field['#name'] = $this->field_map[$field['#name']];
      }
      $fq[$field['#name']][] = $this->make_filter($field);
    }
    foreach ($this->subqueries as $id => $data) {
      $subfq = $data['#query']->rebuild_fq($aliases);
      if ($subfq) {
        $operator = $data['#fq_operator'];
        $subqueries = array();
        foreach ($subfq as $key => $values) {
          foreach ($values as $value) {
            $subqueries[] = $value;
          }
        }
        $fq['subqueries'][$id] =  " (" . implode(" $operator " , $subqueries) . ")";
      }
    }
    return $fq;
  }

  protected function rebuild_query() {
    $query = $this->keys;
    foreach ($this->subqueries as $id => $data) {
      $operator = $data['#q_operator'];
      $subquery = $data['#query']->get_query_basic();
      if ($subquery) {
        $query .= " {$operator} ({$subquery})";
      }
    }
    return $query;
  }

  function search($keys = NULL) {
    if (!isset($keys)) {
      $keys = $this->rebuild_query();
    }
    return $this->solr->search($keys, $this->params);
  }

  function solr($method) {
    return $this->solr->$method();
  }
}
