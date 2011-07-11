<?php

/**
 * @file
 *
 * Provides the HTML and XML Classes for Feeds XPath Parser.
 */

/**
 * Base class for the HTML and XML parsers.
 */
class FeedsXPathParserBase extends FeedsParser {

  protected $modified_queries = array();
  protected $rawXML = array();
  protected $namespaces = array();

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $this->source_config = $source->getConfigFor($this);

    if (empty($this->source_config)) {
      $this->source_config = $this->config;
    }

    $this->parserResult = new FeedsParserResult();

    $mappings = $source->importer->processor->config['mappings'];
    $this->mappings = $this->filterMappings($mappings);
    $this->rawXML = array_keys(array_filter($this->source_config['rawXML']));
    // Set link.
    $fetcher_config = $source->getConfigFor($source->importer->fetcher);
    $this->parserResult->link = $fetcher_config['source'];
  }

  /**
   * Starts the parsing. First runs the context query, then queries each
   * item from the context with the field queries.
   *
   * @param $dom
   *   A DOMDocument object.
   *
   * @return array
   *   Returns a structured array suitable for use in FeedsParserResult().
   */
  protected function query() {

    $this->xpath = new FeedsXPathParserXPath($this->doc);
    $config = array();
    $config['debug'] = array_keys(array_filter($this->source_config['exp']['debug']));
    $config['errors'] = $this->source_config['exp']['errors'];

    $this->xpath->setConfig($config);
    $allNodes = $this->xpath->namespacedQuery($this->source_config['context'], NULL, 'context');

    foreach ($allNodes as $node) {
      $parsedItem = $variables = array();
      foreach ($this->source_config['sources'] as $source => $query) {
        // Variable substitution.
        $query = strtr($query, $variables);
        // Parse the item.
        $result = $this->parseSourceElement($query, $node, $source);
        if (isset($result)) {
          if (!is_array($result)) {
            $variables['$' . $this->mappings[$source]] = $result;
          }
          else {
            $variables['$' . $this->mappings[$source]] = '';
          }
          $parsedItem[$source] = $result;
        }
      }
      $this->parserResult->items[] = $parsedItem;
    }
  }

  /**
   * Parses one item from the context array.
   *
   * @param $item
   *   A  SimpleXMLElement from the context array.
   *
   * @param $query
   *   An XPath query.
   *
   * @param $source
   *   The name of the source for this query.
   *
   * @return array
   *   An array containing the results of the query.
   */
  protected function parseSourceElement($query, $context, $source) {

    if (empty($query)) {
      return;
    }

    $nodeList = $this->xpath->namespacedQuery($query, $context, $source);
    /**
     * Iterate through the results of the XPath query.  If this source is
     * configured to return raw xml, make it so.
     */
    if ($nodeList instanceof DOMNodeList) {
      $results = array();
      if (in_array($source, $this->rawXML)) {
        foreach ($nodeList as $node) {
          $results[] = $this->doc->savexML($node);
        }
      }
      else {
        foreach ($nodeList as $node) {
          $results[] = $node->nodeValue;
        }
      }
      if (count($results) === 1) {
        return $results[0];
      }
      if (count($results) === 0) {
        return;
      }
      return $results;
    }
    else {
      return $nodeList;
    }
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = array();

    $mappings_ = feeds_importer($this->id)->processor->config['mappings'];

    if (empty($source_config)) {
      $source_config = $this->config;
    }

    // Add extensions that might get importerd.
    $fetcher = feeds_importer($this->id)->fetcher;
    if (isset($fetcher->config['allowed_extensions'])) {
      if (strpos($fetcher->config['allowed_extensions'], 'html') === FALSE) {
        $fetcher->config['allowed_extensions'] .= ' html htm';
      }
    }

    $uniques = $mappings = array();
    foreach ($mappings_ as $mapping) {
      if (strpos($mapping['source'], 'xpathparser:') === 0) {
        $mappings[$mapping['source']] = $mapping['target'];
        if ($mapping['unique']) {
          $uniques[] = $mapping['target'];
        }
      }
    }
    $form['xpath'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('XPath Parser Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if (empty($mappings)) {
      $form['xpath']['error_message']['#markup'] = '<div class="help">' . t('FeedsXPathParser: No mappings are defined.') . '</div><br>';
      return $form;
    }
    $form['xpath']['context'] = array(
      '#type' => 'textfield',
      '#title' => t('Context'),
      '#required' => TRUE,
      '#description' => t('This is the base query, all other queries will run in this context.'),
      '#default_value' => isset($source_config['context']) ? $source_config['context'] : '',
      '#maxlength' => 1024,
      '#size' => 80,
    );
    $form['xpath']['sources'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
    );
    if (!empty($uniques)) {
      $items = array(
        format_plural(count($uniques),
          t('Field <strong>!column</strong> is mandatory and considered unique: only one item per !column value will be created.',
            array('!column' => implode(', ', $uniques))),
          t('Fields <strong>!columns</strong> are mandatory and values in these columns are considered unique: only one entry per value in one of these columns will be created.',
            array('!columns' => implode(', ', $uniques)))),
      );
      $form['xpath']['sources']['help']['#markup'] = '<div class="help">' . theme('item_list', array('items' => $items)) . '</div>';
    }
    $variables = array();
    foreach ($mappings as $source => $target) {
      $form['xpath']['sources'][$source] = array(
        '#type' => 'textfield',
        '#title' => check_plain($target),
        '#description' => t('The XPath query to run.'),
        '#default_value' => isset($source_config['sources'][$source]) ? $source_config['sources'][$source] : '',
        '#maxlength' => 1024,
        '#size' => 80,
      );
      if (!empty($variables)) {
        $form['xpath']['sources'][$source]['#description'] .= '<br>' . t('The variables '. implode(', ', $variables). ' are available for replacement.');
      }
      $variables[] = '$' . $target;
    }
    $form['xpath']['rawXML'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select the queries you would like to return raw XML or HTML'),
      '#options' => $mappings,
      '#default_value' => isset($source_config['rawXML']) ? $source_config['rawXML'] : array(),
    );
    $form['xpath']['exp'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
      '#title' => t('Debug Options'),
    );
    $form['xpath']['exp']['errors'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show error messages.'),
      '#default_value' => isset($source_config['exp']['errors']) ? $source_config['exp']['errors'] : FALSE,
    );
    if (extension_loaded('tidy')) {
      $form['xpath']['exp']['tidy'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use Tidy'),
        '#description' => t('The Tidy PHP extension has been detected.
                              Select this to clean the markup before parsing.'),
        '#default_value' => isset($source_config['exp']['tidy']) ? $source_config['exp']['tidy'] : FALSE,
      );
      $form['xpath']['exp']['tidy_encoding'] = array(
        '#type' => 'textfield',
        '#title' => t('Tidy encoding'),
        '#description' => t('Set the encoding for tidy. See the !phpdocs for possible values.', array('!phpdocs' => l('PHP docs', 'http://www.php.net/manual/en/tidy.parsestring.php/'))),
        '#default_value' => isset($source_config['exp']['tidy_encoding']) ? $source_config['exp']['tidy_encoding'] : 'UTF8',
        '#states' => array(
          'visible' => array(
            ':input[name$="[tidy]"]' => array(
              'checked' => TRUE,
            ),
          ),
        ),
      );
    }
    $form['xpath']['exp']['debug'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Debug query'),
      '#options' => array_merge(array('context' => 'context'), $mappings),
      '#default_value' => isset($source_config['exp']['debug']) ? $source_config['exp']['debug'] : array(),
    );
    return $form;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = $this->sourceForm($this->config);
    $form['xpath']['context']['#required'] = FALSE;
    $form['xpath']['#collapsed'] = FALSE;
    return $form;
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array();
  }

  /**
   * Define defaults.
   */
  public function configDefaults() {
    return array(
      'sources' => array(),
      'rawXML' => array(),
      'context' => '',
      'exp' => array(
        'errors' => FALSE,
        'tidy' => FALSE,
        'debug' => array(),
        'tidy_encoding' => 'UTF8',
      ),
    );
  }

  /**
   * Override parent::sourceFormValidate().
   *
   * If the values of this source are the same as the base config we set them to
   * blank to that the values will be inherited from the importer defaults.
   *
   * @param &$values
   *   The values from the form to validate, passed by reference.
   */
  public function sourceFormValidate(&$values) {
    $values = $values['xpath'];
    asort($values);
    asort($this->config);
    if ($values === $this->config) {
      $values = array();
      return;
    }
    $this->configFormValidate($values);
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function configFormValidate(&$values) {
    $config_form = FALSE;
    if (isset($values['xpath'])) {
      $values = $values['xpath'];
      $config_form = TRUE;
    }
    $class = get_class($this);
    $xml = new SimpleXMLElement('<?xml version="1.0" encoding="UTF-8"?>' . "\n<items></items>");
    $use_errors = libxml_use_internal_errors(TRUE);

    $values['context'] = trim($values['context']);
    if (!empty($values['context'])) {
      $result = $xml->xpath($values['context']);
    }
    $error = libxml_get_last_error();

    // Error code 1219 is undefined namespace prefix.
    // Our sample doc doesn't have any namespaces let alone the one they're
    // trying to use. Besides, if someone is trying to use a namespace in an
    // XPath query, they're probably right.
    if ($error && $error->code != 1219) {
      $element = 'feeds]['.$class.'][xpath][context';
      if ($config_form) {
        $element = 'xpath][context';
      }
      form_set_error($element, t('There was an error with the XPath selector: ') . $error->message);
      libxml_clear_errors();
    }
    foreach ($values['sources'] as $key => &$query) {
      $query = trim($query);
      if (!empty($query)) {
        $result = $xml->xpath($query);
        $error = libxml_get_last_error();
        if ($error && $error->code != 1219) {
          $element = 'feeds]['.$class.'][xpath][sources][' . $key;
          if ($config_form) {
            $element = 'xpath][sources][' . $key;
          }
          form_set_error($element, t('There was an error with the XPath selector: ') . $error->message);
          libxml_clear_errors();
        }
      }
    }
    libxml_use_internal_errors($use_errors);
  }

  /**
  * Override parent::getMappingSources().
  */
  public function getMappingSources() {
    $mappings = $this->filterMappings(feeds_importer($this->id)->processor->config['mappings']);
    $next = 0;
    if (!empty($mappings)) {
      $last_mapping = end(array_keys($mappings));
      $next = explode(':', $last_mapping);
      $next = $next[1] + 1;
    }
    return array(
      'xpathparser:' . $next => array(
        'name' => t('XPath Expression'),
        'description' => t('Allows you to configure an XPath expression that will populate this field.'),
      ),
    ) + parent::getMappingSources();
  }

  /**
   * Filters mappings, returning the ones that belong to us.
   */
  protected function filterMappings($mappings) {
    $our_mappings = array();
    foreach ($mappings as $mapping) {
      if (strpos($mapping['source'], 'xpathparser:') === 0) {
        $our_mappings[$mapping['source']] = $mapping['target'];
      }
    }
    return $our_mappings;
  }
}

class FeedsXPathParserHTML extends FeedsXPathParserBase {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    parent::parse($source, $fetcher_result);

    $raw = $fetcher_result->getRaw();

    if (empty($raw)) {
      drupal_set_message(t('Feeds XPath parser: The HTML document is empty.'), 'warning');
      return;
    }
    if (isset($this->source_config['exp']['tidy']) && $this->source_config['exp']['tidy']) {
      $config = array(
        'merge-divs'       => FALSE,
        'merge-spans'      => FALSE,
        'join-styles'      => FALSE,
        'drop-empty-paras' => FALSE,
        'wrap'             => 0,
        'tidy-mark'        => FALSE,
        'escape-cdata'     => TRUE,
        'word-2000'        => TRUE,
      );
      // Default tidy encoding is UTF8.
      $encoding = $this->source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($raw), $config, $encoding);
    }
    $this->doc = new DOMDocument();
    if ($this->source_config['exp']['errors']) {
      $success = $this->doc->loadHTML($raw);
    }
    else {
      $success = @$this->doc->loadHTML($raw);
    }
    unset($raw);
    if (!$success) {
      throw new Exception(t('There was an error parsing the HTML document.'));
    }
    // Fetch the namespaces defined in the XML document.
    $this->query();
    unset($this->doc);
    return $this->parserResult;
  }
}

class FeedsXPathParserXML extends FeedsXPathParserBase {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    parent::parse($source, $fetcher_result);
    $raw = $fetcher_result->getRaw();

    if (empty($raw)) {
      drupal_set_message(t('Feeds XPath parser: The XML document is empty.'), 'warning');
      return;
    }
    if (isset($this->source_config['exp']['tidy']) && $this->source_config['exp']['tidy']) {
      $config = array(
        'input-xml' => TRUE,
        'wrap'      => 0,
        'tidy-mark' => FALSE,
      );
      // Default tidy encoding is UTF8.
      $encoding = $this->source_config['exp']['tidy_encoding'];
      $raw = tidy_repair_string(trim($raw), $config, $encoding);
    }
    $this->doc = new DOMDocument();
    $this->doc->loadXML($raw);
    unset($raw);
    $this->query();
    unset($this->doc);
    return $this->parserResult;
  }
}

/**
 * Pseudo-parser of XPath queries.  When an XML document has a default
 * namespace this gets called so that adding the __default__ namepace where
 * appropriate. Aren't we nice?
 *
 * @todo
 *   Cleanup.
 * @param $query
 *   An XPath query string.
 * @return string
 *   An XPath query string with the __default__ namespace added.
 */
class FeedsXPathQueryParser {
  function __construct($query) {
    $this->query = preg_replace('/\s+\(\s*/', '(', $query);

    $this->word_boundaries = array(
      '[', ']', '=', '(', ')', '.', '<', '>', '*', '!', '|', '/', ',', ' ', ':',
    );
    $this->in_quotes = FALSE;
    $this->quote_char = '';
    $this->word = '';
    $this->output = '';
    $this->prev_boundary = '';
    $this->axis = '';
    $this->skip_next_word = FALSE;
    $this->start();
  }

  function start() {
    for ($i=0; $i < strlen($this->query); $i++) {
      $this->i = $i;
      $c = $this->query[$i];

      if ($c == '"' || $c == "'") {
        $this->handle_quote($c);
        continue;
      }
      if ($this->in_quotes) {
        $this->word .= $c;
        continue;
      }

      if (in_array($c, $this->word_boundaries)) {
        $this->handle_word_boundary($c);
      }
      else {
        $this->word .= $c;
      }
    }
    $this->handle_word();
  }

  function handle_quote($c) {
    if ($this->in_quotes && $c == $this->quote_char) {
      $this->in_quotes = FALSE;
      $this->word .= $c;
      $this->output .= $this->word;
      $this->word = '';
    }
    elseif (!$this->in_quotes) {
      $this->in_quotes = TRUE;
      $this->handle_word();
      $this->word = $c;
      $this->quote_char = $c;
    }
    else {
      $this->word .= $c;
    }
  }

  function handle_word_boundary($c) {
    if (in_array($this->word, array('div', 'or', 'and', 'mod')) &&
        $this->prev_boundary == ' ' && $c == ' ') {
      $this->output .= $this->word;
    }
    else {
      $this->handle_word($c);
    }
    $this->output .= $c;
    $this->word = '';
    $this->prev_boundary = $c;
  }

  function handle_word($c='') {
    if ($this->word == '') {
      return;
    }
    if ($c == ':' && $this->query[$this->i + 1] == ':') {
      $this->axis = $this->word;
    }
    if ($c == ':' && $this->query[$this->i - 1] != ':'  &&
        $this->query[$this->i + 1] != ':') {
      $this->output .= $this->word;
      $this->skip_next_word = TRUE;
      return;
    }
    if ($this->skip_next_word) {
      $this->skip_next_word = FALSE;
      $this->output .= $this->word;
      return;
    }
    if (is_numeric($this->word) ||
        $this->axis == 'attribute' ||
        strpos($this->word, '@') === 0 ||
        $c == '(' ||
        $c == ':') {
      $this->output .= $this->word;
      return;
    }
    $this->output .= '__default__:' . $this->word;
  }

  function getQuery() {
    return $this->output;
  }
}

class FeedsXPathParserXPath extends DOMXPath {
  protected $config = array();
  protected $modifiedQueries = array();

  public function __construct(DOMDocument $doc) {
    $this->namespaces = simplexml_import_dom($doc)->getNamespaces(TRUE);
    $this->doc = $doc;
    parent::__construct($doc);
  }

  public function setConfig(array $config) {
    $this->config = $config;
  }

  protected function debug($data, $source) {
    $o = '<ul>';
    if ($data instanceof DOMNodeList) {
      foreach ($data as $node) {
        $o .= '<li>';
        $o .= check_plain($this->doc->saveXML($node));
        $o .= '</li>';
      }
    }
    else {
      $o .= '<li>' . check_plain($data) . '</li>';
    }
    $o .= '</ul>';
    drupal_set_message($source . ':' . $o);
  }
  /**
   * Executes an XPath query with namespace support.
   *
   * @param $xpath
   *   The DOMXPath object.
   *
   * @param $query
   *   An XPath query.
   *
   * @return array
   *   An array containing the results of the query.
   */
  public function namespacedQuery($query, $context, $source) {
    $this->debug($this->query('namespace'), 'test');
    $this->addDefaultNamespace($query);
    $results = $this->_query($query, $context);
    if (in_array($source, $this->config['debug'])) {
      $this->debug($results, $source);
    }

    if (is_object($this->error) && $this->config['errors']) {

      if ($this->error->level == LIBXML_ERR_ERROR) {
       drupal_set_message(
        t("There was an error during the XPath query: %query.<br>
          Libxml returned the message: %message, with the error code: %code.",
          array('%query'   => $query,
                '%message' => trim($this->error->message),
                '%code'    => $this->error->code)),
        'error',
        FALSE);
      }
      elseif ($this->error->level == LIBXML_ERR_WARNING) {
       drupal_set_message(
        t("There was an error during the XPath query: %query.<br>
          Libxml returned the message: %message, with the error code: %code.",
          array('%query'   => $query,
                '%message' => trim($this->error->message),
                '%code'    => $this->error->code)),
        'warning',
        FALSE);
      }
    }
    // Some versions of libxml return an empty array, some false on a query
    // that returns no results. We always return an array to make like easier.
    if ($results === FALSE) {
      return array();
    }
    return $results;
  }

  /**
   * Normalizes XPath queries, adding the default namespace.
   *
   * @param $query
   *   An XPath query string
   */
  protected function addDefaultNamespace(&$query) {
    foreach ($this->namespaces as $prefix => $namespace) {
      if ($prefix === '') {
        $this->registerNamespace('__default__', $namespace);

        // Replace all the elements without prefix by the default prefix.
        if (!isset($this->modifiedQueries[$query])) {
          $parser = new FeedsXPathQueryParser($query);
          $modQuery = $parser->getQuery();
          $this->modifiedQueries[$query] = $modQuery;
          $query = $modQuery;
        }
        else {
          $query = $this->modifiedQueries[$query];
        }
      }
      else {
        $this->registerNamespace($prefix, $namespace);
      }
    }
  }

  /**
   * Here we set libxml_use_internal_errors to TRUE because depending on the
   * libxml version, $xml->xpath() might return FALSE or an empty array() when
   * a query doesn't match.
   */
  protected function _query($query, $context = NULL) {
    $use_errors = libxml_use_internal_errors(TRUE);

    // Perfom XPath query.
    if ($context) {
      $results = $this->evaluate($query, $context);
    }
    else {
      $results = $this->query($query);
    }
    $this->error = libxml_get_last_error();
    libxml_clear_errors();
    libxml_use_internal_errors($use_errors);
    return $results;
  }
}
