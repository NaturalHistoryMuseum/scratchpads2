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
  public $exclude;

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

  function __construct($operator = 'OR', $exclude = FALSE) {
    $this->operator = $operator;
    $this->exclude = $exclude;
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
   * Make sure our query matches the pattern name:value or name:"value"
   * Make sure that if we are ranges we use name:[ AND ]
   * allowed inputs :
   * a. bundle:article
   * b. date:[1970-12-31T23:59:59Z TO NOW]
   * Split the text in 4 different parts
   * 1. name, eg.: bundle or date
   * 2. The first opening bracket (or nothing), eg.: [
   * 3. The value of the field, eg. article or 1970-12-31T23:59:59Z TO NOW
   * 4. The last closing bracket, eg.: ]
   * @param string $filter
   *   The filter to validate
   * @return boolean
   */
  public static function validFilterValue($filter) {
    $name = NULL;
    $value = NULL;
    $matches = array();
    $datefields = array();
    $datefield_match = array();

    if (preg_match('/(?P<name>[^:]+):(?P<value>.+)?$/', $filter, $matches)) {
      foreach ($matches as $match_id => $match) {
        switch($match_id) {
          case 'name' :
            $name = $match;
            break;
          case 'value' :
            $value = $match;
            break;
        }
      }

      // For the name we allow any character that fits between the A-Z0-9 range and
      // any alternative for this in other languages. No special characters allowed.
      // Negative filters may have a leading "-".
      if (!preg_match('/^-?[a-zA-Z0-9_\x7f-\xff]+$/', $name)) {
        return FALSE;
      }

      // For the value we allow anything that is UTF8
      if (!drupal_validate_utf8($value)) {
        return FALSE;
      }

      // Check our bracket count. If it does not match it is also not valid
      $valid_brackets = TRUE;
      $brackets['opening']['{'] = substr_count($value, '{');
      $brackets['closing']['}'] = substr_count($value, '}');

      $valid_brackets = $valid_brackets && ($brackets['opening']['{'] == $brackets['closing']['}']);
      $brackets['opening']['['] = substr_count($value, '[');
      $brackets['closing'][']'] = substr_count($value, ']');
      $valid_brackets = $valid_brackets && ($brackets['opening']['['] == $brackets['closing'][']']);
      $brackets['opening']['('] = substr_count($value, '(');
      $brackets['closing'][')'] = substr_count($value, ')');
      $valid_brackets = $valid_brackets && ($brackets['opening']['('] == $brackets['closing'][')']);

      if (!$valid_brackets) {
        return FALSE;
      }

      // Check the date field inputs
      if (preg_match('/\[(.+) TO (.+)\]$/', $value, $datefields)) {
        // Only Allow a value in the form of
        // http://lucene.apache.org/solr/api/org/apache/solr/schema/DateField.html
        // http://lucene.apache.org/solr/api/org/apache/solr/util/DateMathParser.html
        // http://wiki.apache.org/solr/SolrQuerySyntax
        // 1976-03-06T23:59:59.999Z (valid)
        // * (valid)
        // 1995-12-31T23:59:59.999Z (valid)
        // 2007-03-06T00:00:00Z (valid)
        // NOW-1YEAR/DAY (valid)
        // NOW/DAY+1DAY (valid)
        // 1976-03-06T23:59:59.999Z (valid)
        // 1976-03-06T23:59:59.999Z+1YEAR (valid)
        // 1976-03-06T23:59:59.999Z/YEAR (valid)
        // 1976-03-06T23:59:59.999Z (valid)
        // 1976-03-06T23::59::59.999Z (invalid)
        if (!empty($datefields[1]) && !empty($datefields[2])) {
          // Do not check to full value, only the splitted ones
          unset($datefields[0]);
          // Check if both matches are valid datefields
          foreach ($datefields as $datefield) {
            if (!preg_match('/(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:[\d\.]{2,6}Z(\S)*)|(^([A-Z\*]+)(\A-Z0-9\+\-\/)*)/', $datefield, $datefield_match)) {
              return FALSE;
            }
          }
        }
      }
    }
    return TRUE;
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
        $prefix = $subquery->exclude ? '-' : '';
        $fq[] = "$prefix(" . implode(" $operator ", $subfq) . ")";
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
   * The query name is used to construct a searcher string. Mostly the
   * environment id
   */
  protected $name;
  protected $context = array();
  // Makes sure we always have a valid sort.
  protected $solrsort = array('#name' => 'score', '#direction' => 'desc');
  // A flag to allow the search to be aborted.
  public $abort_search = FALSE;

  // A flag to check if need to retrieve another page of the result set
  public $page = 0;

  /**
   * @param $name
   *   The search name, used for finding the correct blocks and other config.
   *   Typically "apachesolr".
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
  function __construct($name, $solr, array $params = array(), $sortstring = '', $base_path = '', $context = array()) {
    parent::__construct();
    $this->name = $name;
    $this->solr = $solr;
    $this->addContext((array) $context);
    $this->addParams((array) $params);
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

  /**
   * Get context values.
   */
  public function getContext() {
    return $this->context;
  }

  /**
   * Set context value.
   */
  public function addContext(array $context) {
    foreach ($context as $k => $v) {
      $this->context[$k] = $v;
    }
    // The env_id must match that of the actual $solr object
    $this->context['env_id'] = $this->solr->getId();
    return $this->context;
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
    'mm' => TRUE,
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
    $string = trim($string);
    $local = '';
    $exclude = FALSE;
    $name = NULL;
    $value = NULL;
    $matches = array();

    // Check if we are dealing with an exclude
    if (preg_match('/^-(.*)/', $string, $matches)) {
      $exclude = TRUE;
      $string = $matches[1];
    }

    // If {!something} is found as first character then this is a local value
    if (preg_match('/\{!([^}]+)\}(.*)/', $string, $matches)) {
      $local = $matches[1];
      $string = $matches[2];
    }

    // Anything that has a name and value
    // check if we have a : in the string
    if (strstr($string, ':')) {
      list($name, $value) = explode(":", $string, 2);
    }
    else {
      $value = $string;
    }
    $this->addFilter($name, $value, $exclude, $local);
    return $this;
  }

  public function addParam($name, $value) {
    if (isset($this->single_value_params[$name])) {
      if (is_array($value)) {
        $value = end($value);
      }
      $this->params[$name] = $this->normalizeParamValue($value);
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

    if (!is_array($value)) {
      // Convert to array for array_map.
      $param_values = array($value);
    }
    else {
      // Convert to a numerically keyed array.
      $param_values = array_values($value);
    }
    $this->params[$name] = array_merge($this->params[$name], array_map(array($this, 'normalizeParamValue'), $param_values));

    return $this;
  }

  protected function normalizeParamValue($value) {
    // Convert boolean to string.
    if (is_bool($value)) {
      return $value ? 'true' : 'false';
    }
    // Convert to trimmed string.
    return trim($value);
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

  /**
   * Handles aliases for field to make nicer URLs.
   *
   * @param $field_map
   *   An array keyed with real Solr index field names with the alias as value.
   *
   * @return DrupalSolrQueryInterface
   *   The called object.
   */
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
      $fields = array_keys($this->available_sorts);
      // Loop through available sorts and escape them, to allow for function sorts like geodist() in the preg_match() below
      foreach ($fields as $key => $field) {
        $fields[$key] = preg_quote($field);
      }
      // Implode the escaped available sorts together, then preg_match() against the sort string
      $fields = implode('|', $fields);
      if (preg_match('/^(?:(' . $fields . ') (asc|desc),?)+$/', $sortstring, $matches)) {
        // We only use the last match.
        $this->solrsort['#name'] = $matches[1];
        $this->solrsort['#direction'] = $matches[2];
        $this->params['sort'] = array($sortstring);
      }
      else {
        return FALSE;
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
    elseif ($this->getParam('q')) {
      return $this->base_path . '/' . $this->getParam('q');
    }
    else {
      // Return with empty query (the slash). The path for a facet
      // becomes $this->base_path . '//facetinfo';
      // We do this so we can have a consistent way of retrieving the query +
      // additional parameters
      return $this->base_path . '/';
    }
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
