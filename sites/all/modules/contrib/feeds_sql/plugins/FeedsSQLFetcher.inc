<?php

/**
 * @file
 * Fetches data from an SQL database.
 */

/**
 * Fetches data via pdo connection.
 */
class FeedsSQLFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $results = array();
    $source_config = $source->getConfigFor($this);
    $source_config['query'] = trim($source_config['query']);
    $query = token_replace($source_config['query']);
    // Make sure there is only one query
    if ($end = strpos($query, ';')) {
      $query = substr($query, 0, $end + 1);
    }
    // Verify the query is a SELECT statement
    $select = strtoupper(substr($query, 0, 6));
    if ($select != 'SELECT') {
      drupal_set_message(t('SQL query has to be of the form "SELECT field1, field2 FROM table WHERE conditions"'), 'error');
    }
    else {
      // Run the query on the selected database
      db_set_active($source_config['database']);
      $result = @db_query($query);
      foreach ($result as $record) {
        $results[] = $record;
      }
      // Switch back to the default database
      db_set_active();
    }
    return new FeedsFetcherResult($results);
  }
  
  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'databases' => array('default'),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    global $databases;
    $form = array();
    $form['databases'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Database'),
      '#description' => t('Select the databases from which to allow fetching the data.'),
      '#options' => array_combine(array_keys($databases), array_keys($databases)),
      '#default_value' => $this->config['databases'],
      '#required' => TRUE,
    );
    return $form;
  }
  
  /**
   * Override parent::sourceDefaults().
   */
  function sourceDefaults() {
    return array(
      'query' => '',
      'database' => 'default',
    );
  }
  
  /**
   * Override parent::sourceForm().
   */
  public function sourceForm($source_config) {
    $config = $this->getConfig();
    $databases = array();
    foreach ($config['databases'] as $database) {
      if ($database) {
        $databases[$database] = $database;
      }
    }
    $form = array();
    $form['query'] = array(
      '#type' => 'textarea',
      '#title' => t('SQL query'),
      '#description' => t('Enter the SQL query which will fetch the data to be imported.'),
      '#default_value' => isset($source_config['query']) ? $source_config['query'] : '',
      '#required' => TRUE,
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
      '#options' => $databases,
      '#default_value' => isset($source_config['database']) ? $source_config['database'] : 'default',
      '#required' => TRUE,
    );
    return $form;
  }

}
