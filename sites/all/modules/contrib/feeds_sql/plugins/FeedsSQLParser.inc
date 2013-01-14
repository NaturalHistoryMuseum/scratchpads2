<?php

/**
 * Parses data from an SQL database.
 */
class FeedsSQLParser extends FeedsParser {

  /**
   * Implements FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $source_config = $source->getConfigFor($this);
    // Construct the standard form of the parsed feed
    $result = new FeedsParserResult();
    $result->title = '';
    $result->description = '';
    $result->link = '';
    // Iterate through the fetcher results
    foreach ($fetcher_result->getRaw() as $index => $row) {
      $result->items[$index] = (array) $row;
    }
    // Create a result object and return it.
    return $result;
  }

  /**
   * Override parent::getMappingSources().
   */
  public function getMappingSources() {
    $config = $this->getConfig();
    $sources = array();
    foreach ($config['mapping'] as $field) {
      $sources[$field] = array(
        'name' => $field,
        'description' => $field,
      );
    }
    if (!empty($sources)) {
      return $sources;
    }
    else {
      return parent::getMappingSources();
    }
  }

  /**
   * Override parent::getSourceElement() to use only lower keys.
   */
  public function getSourceElement(FeedsSource $source, FeedsParserResult $result, $element_key) {
    return parent::getSourceElement($source, $result, drupal_strtolower($element_key));
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return array();
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    return $form;
  }

  /**
   * Define default configuration.
   */
  public function configDefaults() {
    return array(
      'query' => '',
      'database' => 'default',
      'mapping' => array(),
      'results' => '',
    );
  }

  /**
   * Build configuration form.
   */
  public function configForm(&$form_state) {
    global $databases;
    $form = array();
    $form['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Use this area to pre-fetch your intended query. This will allow you to test the query itself and to populate the mapping array.'),
    );
    $form['query'] = array(
      '#type' => 'textarea',
      '#title' => t('SQL query'),
      '#description' => t('Enter the SQL query which will fetch the data to be imported.'),
      '#default_value' => isset($this->config['query']) ? $this->config['query'] : '',
    );
    if (module_exists('token')) {
      $form['token_help'] = array(
        '#title' => t('Replacement patterns'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['token_help']['help'] = array(
        '#theme' => 'token_tree',
        '#token_types' => array(),
      );
    }
    $form['database'] = array(
      '#type' => 'select',
      '#title' => t('Database'),
      '#description' => t('Select the database from which to fetch the data.'),
      '#options' => array_combine(array_keys($databases), array_keys($databases)),
      '#default_value' => isset($this->config['database']) ? $this->config['database'] : 'default',
      '#required' => TRUE,
    );
    $form['mapping'] = array(
      '#type' => 'hidden',
      '#value' => $this->config['mapping'],
    );
    if (!empty($this->config['results'])) {
      $form['results'] = array(
        '#type' => 'markup',
        '#markup' => $this->config['results'],
      );
      unset($this->config['results']);
      $this->save();
    }
    return $form;
  }
  
  /**
   * Validate entered query to make sure it works.
   */
  public function configFormValidate(&$config) {
    $results = array();
    $config['query'] = trim($config['query']);
    $query = token_replace($config['query']);
    // Make sure there is only one query
    if ($end = strpos($query, ';')) {
      $query = substr($query, 0, $end + 1);
    }
    // Verify the query is a SELECT statement
    $select = strtoupper(substr($query, 0, 6));
    if ($select != 'SELECT') {
      form_set_error('query', t('SQL query has to be of the form "SELECT field1, field2 FROM table WHERE conditions"'));
      return;
    }
    try {
      // Switch to the selected database
      db_set_active($config['database']);
      // Test the query
      $count = 0;
      $result = @db_query($query);
      foreach ($result as $record) {
        $count++;
        // Store only up to 10 results
        if ($count <= 10) {
          $results[] = (array) $record;
        }
      }
      // Switch back to the default database
      db_set_active();
    }
    catch (Exception $error) {
      form_set_error('query', $error->getMessage());
      db_set_active();
    }
    // Create mapping sources
    $mapping = array();
    if (is_array($results) && !empty($results[0])) {
      foreach ($results[0] as $field => $value) {
        $mapping[$field] = $field;
      }
    }
    // Display some of the results
    $config['results'] = theme('form_element_label', array(
      'element' => array(
        '#title' => t('Test results'),
        '#title_display' => 'before',
      ),
    )) . theme('item_list', array(
      'items' => array(
        t('Total number of records: !records', array('!records' => $count)),
        t('Total number of columns: !columns', array('!columns' => count($mapping))),
      )
    )) . theme('table', array(
      'header' => $mapping,
      'rows' => $results,
    )) . t('Verify that the results from the query are what you expect. Note that this is limited to 10 results.') . '<br />';
    ksort($mapping);
    $config['mapping'] = $mapping;
    $this->addConfig($config);
  }

}
