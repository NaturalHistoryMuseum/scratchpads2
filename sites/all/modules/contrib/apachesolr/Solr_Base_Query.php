<?php
/**
 * This class allows you to make operations on a query that will be sent to
 * Apache Solr. methods such as adding and removing sorts, remove and replace
 * parameters, adding and removing filters, getters and setters for various
 * parameters and more
 * @file
 *   Class that defines the base query for the Apache Solr Drupal module.
 */

class SolrFilterSubQuery {

  /**
   * Static shared by all instances, used to increment ID numbers.
   */
  protected static $idCount = 0;

  /**
   * Each query/subquery will have a unique ID.
   */
  public $id;
  public $operator;

  /**
   * A keyed array where the key is a position integer and the value
   * is an array with #name and #value properties.  Each value is a
   * used for filter queries, e.g. array('#name' => 'is_uid', '#value' => 0)
   * for anonymous content.
   */
  protected $fields = array();

  /**
   * An array of subqueries.
   */
  protected $subqueries = array();

  function __construct($operator = 'OR') {
    $this->operator = $operator;
    $this->id = ++SolrFilterSubQuery::$idCount;
  }

  function __clone() {
    $this->id = ++SolrFilterSubQuery::$idCount;
  }

  public function getFilters($name = NULL) {
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

  public function hasFilter($name, $value, $exclude = FALSE) {
    foreach ($this->fields as $pos => $values) {
      if ($values['#name'] == $name && $values['#value'] == $value && $values['#exclude'] == $exclude) {
        return TRUE;
      }
    }
    return FALSE;
  }

  public function addFilter($name, $value, $exclude = FALSE, $local = '') {
    // @todo - escape the value if it has spaces in it and is not a range query or parenthesized.
    $filter = array(
      '#exclude' => (bool) $exclude,
      '#name' => trim($name),
      '#value' => trim($value),
      '#local' => trim($local),
    );
    $this->fields[] = $filter;
    return $this;
  }

  public function removeFilter($name, $value = NULL, $exclude = FALSE) {
    // Remove from the public list of filters.
    $this->unsetFilter($this->fields, $name, $value, $exclude);
    return $this;
  }

  protected function unsetFilter(&$fields, $name, $value, $exclude) {
    if (!isset($value)) {
      foreach ($fields as $pos => $values) {
        if ($values['#name'] == $name) {
          unset($fields[$pos]);
        }
      }
    }
    else {
      foreach ($fields as $pos => $values) {
        if ($values['#name'] == $name && $values['#value'] == $value && $values['#exclude'] == $exclude) {
          unset($fields[$pos]);
        }
      }
    }
  }

  public function getFilterSubQueries() {
    return $this->subqueries;
  }

  public function addFilterSubQuery(SolrFilterSubQuery $query) {
    $this->subqueries[$query->id] = $query;
    return $this;
  }

  public function removeFilterSubQuery(SolrFilterSubQuery $query) {
    unset($this->subqueries[$query->id]);
    return $this;
  }

  public function removeFilterSubQueries() {
    $this->subqueries = array();
    return $this;
  }

  public function makeFilterQuery(array $filter) {
    $prefix = empty($filter['#exclude']) ? '' : '-';
    if ($filter['#local']) {
      $prefix = '{!' . $filter['#local'] . '}' . $prefix;
    }
    // If the field value contains a colon or a space, wrap it in double quotes,
    // unless it is a range query or is already wrapped in double quotes or
    // parentheses.
    if (preg_match('/[ :]/', $filter['#value']) && !preg_match('/^[\[\{]\S+ TO \S+[\]\}]$/', $filter['#value']) && !preg_match('/^["\(].*["\)]$/', $filter['#value'])) {
      $filter['#value'] = '"' . $filter['#value'] . '"';
    }
    return $prefix . $filter['#name'] . ':' . $filter['#value'];
  }

  /**
   * Builds a set of filter queries from $this->fields and all subqueries.
   *
   * Returns an array of strings that can be combined into
   * a URL query parameter or passed to Solr as fq paramters.
   */
  protected function rebuildFq() {
    $fq = array();
    foreach ($this->fields as $pos => $field) {
      $fq[] = $this->makeFilterQuery($field);
    }
    foreach ($this->subqueries as $subquery) {
      $subfq = $subquery->rebuildFq();
      if ($subfq) {
        $operator = $subquery->operator;
        $fq[] = "(" . implode(" $operator ", $subfq) . ")";
      }
    }
    return $fq;
  }

}

class SolrBaseQuery extends SolrFilterSubQuery implements DrupalSolrQueryInterface {

  /**
   * The parameters that get sent to Solr.
   */
  protected $params = array('start' => 0, 'rows' => 10, 'fq' => array());

  /**
   * The search base path.
   */
  protected $base_path;
  protected $field_map = array();

  /**
   * DrupalApacheSolrService object
   */
  protected $solr;
  // The array keys must always be real Solr index fields.
  protected $available_sorts;

  /**
   * The query name is used to construct a searcher string. Typically something like 'apachesolr'
   */
  protected $name;
  // Makes sure we always have a valid sort.
  protected $solrsort = array('#name' => 'score', '#direction' => 'desc');
  // A flag to allow the search to be aborted.
  public $abort_search = FALSE;

  /**
   * @param $name
   *   A name (namespce) for this query.  Typically 'apachesolr'.
   *
   * @param $solr
   *   An instantiated DrupalApacheSolrService Object.
   *   Can be instantiated from apachesolr_get_solr().
   *
   * @param $params
   *   Array of params to initialize the object (typically 'q' and 'fq').
   *
   * @param $sortstring
   *   Visible string telling solr how to sort - added to GET query params.
   *
   * @param $base_path
   *   The search base path (without the keywords) for this query, without trailing slash.
   */
  function __construct($name, $solr, array $params = array(), $sortstring = '', $base_path = '') {
    parent::__construct();
    $this->name = $name;
    $this->solr = $solr;
    $this->addParams($params);
    $this->available_sorts = $this->defaultSorts();
    $this->sortstring = trim($sortstring);
    $this->parseSortString();
    $this->base_path = $base_path;
  }

  protected function defaultSorts() {
    return array(
      'score' => array('title' => t('Relevancy'), 'default' => 'desc'),
      'sort_label' => array('title' => t('Title'), 'default' => 'asc'),
      'bundle' => array('title' => t('Type'), 'default' => 'asc'),
      'sort_name' => array('title' => t('Author'), 'default' => 'asc'),
      'ds_created' => array('title' => t('Date'), 'default' => 'desc'),
    );
  }

  /**
   * Get query name.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Get query searcher name (for facetapi, views, pages, etc).
   */
  public function getSearcher() {
    return $this->name . '@' . $this->solr->getId();
  }

  protected $single_value_params = array(
    'q' => TRUE, // http://wiki.apache.org/solr/SearchHandler#q
    'q.op' => TRUE, // http://wiki.apache.org/solr/SearchHandler#q.op
    'q.alt' => TRUE, // http://wiki.apache.org/solr/SearchHandler#q
    'df' => TRUE,
    'qt' => TRUE,
    'defType' => TRUE,
    'timeAllowed' => TRUE,
    'omitHeader' => TRUE,
    'debugQuery' => TRUE,
    'start' => TRUE,
    'rows' => TRUE,
    'stats' => TRUE,
    'facet' => TRUE,
    'facet.prefix' => TRUE,
    'facet.limit' => TRUE,
    'facet.offset' => TRUE,
    'facet.mincount' => TRUE,
    'facet.missing' => TRUE,
    'facet.method' => TRUE,
    'facet.enum.cache.minDf' => TRUE,
    'facet.date.start' => TRUE,
    'facet.date.end' => TRUE,
    'facet.date.gap' => TRUE,
    'facet.date.hardend' => TRUE,
    'facet.date.other' => TRUE,
    'facet.date.include' => TRUE,
    'hl' => TRUE,
    'hl.snippets' => TRUE,
    'hl.fragsize' => TRUE,
    'hl.mergeContiguous' => TRUE,
    'hl.requireFieldMatch' => TRUE,
    'hl.maxAnalyzedChars' => TRUE,
    'hl.alternateField' => TRUE,
    'hl.maxAlternateFieldLength' => TRUE,
    'hl.formatter' => TRUE,
    'hl.simple.pre/hl.simple.post' => TRUE,
    'hl.fragmenter' => TRUE,
    'hl.fragListBuilder' => TRUE,
    'hl.fragmentsBuilder' => TRUE,
    'hl.useFastVectorHighlighter' => TRUE,
    'hl.usePhraseHighlighter' => TRUE,
    'hl.highlightMultiTerm' => TRUE,
    'hl.regex.slop' => TRUE,
    'hl.regex.pattern' => TRUE,
    'hl.regex.maxAnalyzedChars' => TRUE,
    'spellcheck' => TRUE,
  );

  public function getParam($name) {
    if ($name == 'fq') {
      return $this->rebuildFq();
    }
    $empty = isset($this->single_value_params[$name]) ? NULL : array();
    return isset($this->params[$name]) ? $this->params[$name] : $empty;
  }

  public function getParams() {
    $params = $this->params;
    $params['fq'] = $this->rebuildFq();
    return $params;
  }

  public function getSolrParams() {
    $params = $this->getParams();
    // For certain fields Solr prefers a comma separated list.
    foreach (array('fl', 'hl.fl', 'sort', 'mlt.fl') as $name) {
      if (isset($params[$name])) {
        $params[$name] = implode(',', $params[$name]);
      }
    }
    return $params;
  }

  protected function addFq($string, $index = NULL) {
    // Gets information about the fields already in solr index.
    $string = trim($string);
    $local = '';
    $exclude = FALSE;
    if (preg_match('/\({!([^}]+)\}(.*)/', $string, $matches)) {
      $local = $matches[1];
      $string = $matches[2];
    }
    if (preg_match('/(-|)(\(\S+\))/', $string, $matches)) {
      // Something complicated
      $exclude = !empty($matches[1]);
      $this->addFilter('', $matches[2], $exclude, $local);
    }
    elseif (preg_match('/(-|)([^:]+):([\("\[].+[\)"\]])/', $string, $matches)) {
      // Something with a complicated right-hand-side.
      // Ex.: bundle:(article OR page)
      // Ex.: title:"double words"
      // Ex.: field_date:[1970-12-31T23:59:59Z TO NOW]
      $exclude = !empty($matches[1]);
      $this->addFilter($matches[2], $matches[3], $exclude, $local);
    }
    elseif (preg_match('/(-|)([^:]+):(\S+)/', $string, $matches)) {
      //$index_fields = (array) $this->solr->getFields();
      $exclude = !empty($matches[1]);
      $this->addFilter($matches[2], $matches[3], $exclude, $local);
    }
    return $this;
  }

  public function addParam($name, $value) {
    if (isset($this->single_value_params[$name])) {
      if (is_array($value)) {
        $value = end($value);
      }
      $this->params[$name] = trim($value);
      return $this;
    }
    // We never actually populate $this->params['fq'].  Instead
    // we manage everything via the filter methods.
    if ($name == 'fq') {
      if (is_array($value)) {
        array_walk_recursive($value, array($this, 'addFq'));
        return $this;
      }
      else {
        return $this->addFq($value);
      }
    }

    if (!isset($this->params[$name])) {
      $this->params[$name] = array();
    }

    if (is_array($value)) {
      $this->params[$name] = array_merge($this->params[$name], array_values($value));
    }
    else {
      $this->params[$name][] = $value;
    }
    return $this;
  }

  public function addParams(Array $params) {
    foreach ($params as $name => $value) {
      $this->addParam($name, $value);
    }
    return $this;
  }

  public function removeParam($name) {
    unset($this->params[$name]);
    if ($name == 'fq') {
      $this->fields = array();
      $this->subqueries = array();
    }
    return $this;
  }

  public function replaceParam($name, $value) {
    $this->removeParam($name);
    return $this->addParam($name, $value);
  }

  public function addFieldAliases($field_map) {
    $this->field_map = array_merge($this->field_map, $field_map);
    // We have to re-parse the filters.
    $this->parseSortString();
    return $this;
  }

  public function getFieldAliases() {
    return $this->field_map;
  }

  public function clearFieldAliases() {
    $this->field_map = array();
    // We have to re-parse the filters.
    $this->parseSortString();
    return $this;
  }

  protected function parseSortString() {
    // Substitute any field aliases with real field names.
    $sortstring = strtr($this->sortstring, $this->field_map);
    // Score is a special case - it's the default sort for Solr.
    if ('' == $sortstring || 'score desc' == $sortstring) {
      $this->solrsort['#name'] = 'score';
      $this->solrsort['#direction'] = 'desc';
      unset($this->params['sort']);
    }
    else {
      // Validate and set sort parameter
      $fields = implode('|', array_keys($this->available_sorts));
      if (preg_match('/^(?:(' . $fields . ') (asc|desc),?)+$/', $sortstring, $matches)) {
        // We only use the last match.
        $this->solrsort['#name'] = $matches[1];
        $this->solrsort['#direction'] = $matches[2];
        $this->params['sort'] = array($sortstring);
      }
    }
  }

  public function getAvailableSorts() {
    return $this->available_sorts;
  }

  public function setAvailableSort($name, $sort) {
    // We expect non-aliased sorts to be added.
    $this->available_sorts[$name] = $sort;
    // Re-parse the sortstring.
    $this->parseSortString();
    return $this;
  }

  public function setAvailableSorts($sorts) {
    // We expect a complete array of valid sorts.
    $this->available_sorts = $sorts;
    $this->parseSortString();
    return $this;
  }

  public function removeAvailableSort($name) {
    unset($this->available_sorts[$name]);
    // Re-parse the sortstring.
    $this->parseSortString();
    return $this;
  }

  public function getSolrsort() {
    return $this->solrsort;
  }

  public function setSolrsort($name, $direction) {
    $this->sortstring = trim($name) . ' ' . trim($direction);
    $this->parseSortString();
    return $this;
  }

  public function getPath($new_keywords = NULL) {
    if (isset($new_keywords)) {
      return $this->base_path . '/' . $new_keywords;
    }
    return $this->base_path . '/' . $this->getParam('q');
  }

  public function getSolrsortUrlQuery() {
    $queryvalues = array();
    $solrsort = $this->solrsort;
    if ($solrsort && ($solrsort['#name'] != 'score')) {
      if (isset($this->field_map[$solrsort['#name']])) {
        $solrsort['#name'] = $this->field_map[$solrsort['#name']];
      }
      $queryvalues['solrsort'] = $solrsort['#name'] . ' ' . $solrsort['#direction'];
    }
    else {
      // Return to default relevancy sort.
      unset($queryvalues['solrsort']);
    }
    return $queryvalues;
  }

  public function search($keys = NULL) {
    if ($this->abort_search) {
      return NULL;
    }
    return $this->solr->search($keys, $this->getSolrParams());
  }

  public function solr($method) {
    return $this->solr->$method();
  }

}
