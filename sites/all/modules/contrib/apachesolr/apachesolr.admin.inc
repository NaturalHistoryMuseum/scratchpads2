<?php

/**
 * @file
 *   Administrative pages for the Apache Solr framework.
 */

/**
 * Form to delete a search environment
 *
 * @param array $form
 * @param array $form_state
 * @param array $environment
 *
 * @return array output of confirm_form()
 */
function apachesolr_environment_delete_form(array $form, array &$form_state, array $environment) {
  $form['env_id'] = array(
    '#type' => 'value',
    '#value' => $environment['env_id'],
  );
  if (isset($environment['export_type']) && $environment['export_type'] == 3) {
    $verb = t('Revert');
  }
  else {
    $verb = t('Delete');
  }
  return confirm_form(
    $form,
    t('Are you sure you want to !verb search environment %name?', array('%name' => $environment['name'], '!verb' => strtolower($verb))),
    'admin/config/search/apachesolr',
    t('This action cannot be undone.'),
    $verb,
    t('Cancel')
  );
}

/**
 * Submit handler for the delete form
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_environment_delete_form_submit(array $form, array &$form_state) {
  if (apachesolr_environment_delete($form_state['values']['env_id'])) {
    drupal_set_message(t('The search environment was deleted'));
  }
  $form_state['redirect'] = 'admin/config/search/apachesolr/settings';
}

function apachesolr_environment_edit_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/search/apachesolr/settings/' . $form_state['values']['env_id'] . '/delete';

  // Regardlessly of the destination parameter we want to go to another page
  unset($_GET['destination']);
  drupal_static_reset('drupal_get_destination');
  drupal_get_destination();
}

/**
 * Settings page for a specific environment (or default one if not provided)
 *
 * @param array|bool $environment
 *
 * @return array Render array for a settings page
 */
function apachesolr_environment_settings_page(array $environment = array()) {
  if (empty($environment)) {
    $env_id = apachesolr_default_environment();
    $environment = apachesolr_environment_load($env_id);
  }
  $env_id = $environment['env_id'];

 // Initializes output with information about which environment's setting we are
  // editing, as it is otherwise not transparent to the end user.
  $output = array(
    'apachesolr_environment' => array(
      '#theme' => 'apachesolr_settings_title',
      '#env_id' => $env_id,
    ),
  );
  $output['form'] = drupal_get_form('apachesolr_environment_edit_form', $environment);
  return $output;
}

/**
 * Form to clone a certain environment
 *
 * @param array $form
 * @param array $form_state
 * @param array $environment
 *
 * @return array output of confirm_form()
 */
function apachesolr_environment_clone_form(array $form, array &$form_state, array $environment) {
  $form['env_id'] = array(
    '#type' => 'value',
    '#value' => $environment['env_id'],
  );
  return confirm_form(
    $form,
    t('Are you sure you want to clone search environment %name?', array('%name' => $environment['name'])),
    'admin/config/search/apachesolr',
    '',
    t('Clone'),
    t('Cancel')
  );
}

/**
 * Submit handler for the clone form
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_environment_clone_form_submit(array $form, array &$form_state) {
  if (apachesolr_environment_clone($form_state['values']['env_id'])) {
    drupal_set_message(t('The search environment was cloned'));
  }
  $form_state['redirect'] = 'admin/config/search/apachesolr/settings';
}

/**
 * Submit handler for the confirmation page of cloning an environment
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_environment_clone_submit(array $form, array &$form_state) {
  $form_state['redirect'] = 'admin/config/search/apachesolr/settings/' . $form_state['values']['env_id'] . '/clone';
}

/**
 * Form builder for adding/editing a Solr environment used as a menu callback.
 */
function apachesolr_environment_edit_form(array $form, array &$form_state, array $environment = array()) {
  if (empty($environment)) {
    $environment = array();
  }
  $environment += array('env_id' => '', 'name' => '', 'url' => '', 'service_class' => '', 'conf' => array());

  $form['#environment'] = $environment;
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr server URL'),
    '#default_value' => $environment['url'],
    '#description' => t('Example: http://localhost:8983/solr'),
    '#required' => TRUE,
  );
  $is_default = $environment['env_id'] == apachesolr_default_environment();
  $form['make_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make this Solr search environment the default'),
    '#default_value' => $is_default,
    '#disabled' => $is_default,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $environment['name'],
    '#required' => TRUE,
  );
  $form['env_id'] = array(
    '#type' => 'machine_name',
    '#title' => t('Environment id'),
    '#machine_name' => array(
      'exists' => 'apachesolr_environment_load',
    ),
    '#default_value' => $environment['env_id'],
    '#disabled' => !empty($environment['env_id']), // Cannot change it once set.
    '#description' => t('Unique, machine-readable identifier for this Solr environment.'),
    '#required' => TRUE,
  );
  $form['service_class'] = array(
    '#type' => 'value',
    '#value' => $environment['service_class'],
  );
  $form['conf'] = array(
    '#tree' => TRUE,
  );
  $form['conf']['apachesolr_read_only'] = array(
    '#type' => 'radios',
    '#title' => t('Index write access'),
    '#default_value' => isset($environment['conf']['apachesolr_read_only']) ? $environment['conf']['apachesolr_read_only'] : APACHESOLR_READ_WRITE,
    '#options' => array(APACHESOLR_READ_WRITE => t('Read and write (normal)'), APACHESOLR_READ_ONLY => t('Read only')),
    '#description' => t('<em>Read only</em> stops this site from sending updates to this search environment. Useful for development sites.'),
  );
  $form['conf']['apachesolr_direct_commit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Commit changes after the index process has submitted them'),
    '#description' => t('Commits the updates to solr. Uses soft commits where possible'),
    '#default_value' => isset($environment['conf']['apachesolr_direct_commit']) ? $environment['conf']['apachesolr_direct_commit'] : false,
  );
  $form['conf']['apachesolr_soft_commit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Commit changes to memory.'),
    '#description' => t(' Reduces the load on your disk. Also known as Soft Commits, recommended for Solr 4.'),
    '#default_value' => isset($environment['conf']['apachesolr_soft_commit']) ? $environment['conf']['apachesolr_soft_commit'] : false,
  );
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#validate' => array('apachesolr_environment_edit_validate'),
    '#submit' => array('apachesolr_environment_edit_submit'),
    '#value' => t('Save'),
  );
  $form['actions']['save_edit'] = array(
    '#type' => 'submit',
    '#validate' => array('apachesolr_environment_edit_validate'),
    '#submit' => array('apachesolr_environment_edit_submit'),
    '#value' => t('Save and edit'),
  );
  $form['actions']['test'] = array(
    '#type' => 'submit',
    '#validate' => array('apachesolr_environment_edit_validate'),
    '#submit' => array('apachesolr_environment_edit_test_submit'),
    '#value' => t('Test connection'),
  );
  if (!empty($environment['env_id']) && !$is_default) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#submit' => array('apachesolr_environment_edit_delete_submit'),
      '#value' => t('Delete'),
    );
  }

  // Ensures destination is an internal URL, builds "cancel" link.
  if (isset($_GET['destination']) && !url_is_external($_GET['destination'])) {
    $destination = $_GET['destination'];
  }
  else {
    $destination = 'admin/config/search/apachesolr/settings';
  }
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $destination,
  );

  return $form;
}

/**
 * Submit handler for the test button in the environment edit page
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_environment_edit_test_submit(array $form, array &$form_state) {
  $ping = apachesolr_server_status($form_state['values']['url'], $form_state['values']['service_class']);
  if ($ping) {
    drupal_set_message(t('Your site has contacted the Apache Solr server.'));
  }
  else {
    drupal_set_message(t('Your site was unable to contact the Apache Solr server.'), 'error');
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate handler for the environment edit page
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_environment_edit_validate(array $form, array &$form_state) {
  $parts = parse_url($form_state['values']['url']);
  foreach (array('scheme', 'host', 'path') as $key) {
    if (empty($parts[$key])) {
      form_set_error('url', t('The Solr server URL needs to include a !part', array('!part' => $key)));
    }
  }
  if (isset($parts['port'])) {
    // parse_url() should always give an integer for port. Since drupal_http_request()
    // also uses parse_url(), we don't need to validate anything except the range.
    $pattern = empty($parts['user']) ? '@://[^:]+:([^/]+)@' : '#://[^@]+@[^:]+:([^/]+)#';
    preg_match($pattern, $form_state['values']['url'], $m);
    if (empty($m[1]) || !ctype_digit($m[1]) || $m[1] < 1 || $m[1] > 65535) {
      form_set_error('port', t('The port has to be an integer between 1 and 65535.'));
    }
    else {
      // Normalize the url by removing extra slashes and whitespace.
      $form_state['values']['url'] = trim($form_state['values']['url'], "/ \t\r\n\0\x0B");
    }
  }
}

/**
 * Submit handler for the environment  edit page
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_environment_edit_submit(array $form, array &$form_state) {
  apachesolr_environment_save($form_state['values']);
  if (!empty($form_state['values']['make_default'])) {
    apachesolr_set_default_environment($form_state['values']['env_id']);
  }
  cache_clear_all('apachesolr:environments', 'cache_apachesolr');
  drupal_set_message(t('The %name search environment has been saved.', array('%name' => $form_state['values']['name'])));
  if ($form_state['values']['op'] == t('Save')) {
    $form_state['redirect'] = 'admin/config/search/apachesolr/settings';
  }
  else {
    $form_state['redirect'] = current_path();
  }
  // Regardlessly of the destination parameter we want to go to another page
  unset($_GET['destination']);
  drupal_static_reset('drupal_get_destination');
  drupal_get_destination();
}

/**
 * Check to see if the facetapi module is installed, and if not put up
 * a message.
 *
 * Only call this function if the user is already in a position for this to
 * be useful.
 */
function apachesolr_check_facetapi() {
  if (!module_exists('facetapi')) {
    $filename = db_query_range("SELECT filename FROM {system} WHERE type = 'module' AND name = 'facetapi'", 0, 1)
      ->fetchField();
    if ($filename && file_exists($filename)) {
      drupal_set_message(t('If you <a href="@modules">enable the facetapi module</a>, Apache Solr Search will provide you with configurable facets.', array('@modules' => url('admin/modules'))));
    }
    else {
      drupal_set_message(t('If you install the facetapi module from !href, Apache Solr Search will provide you with configurable facets.', array('!href' => url('http://drupal.org/project/facetapi'))));
    }
  }
}

/**
 * Form builder for general settings used as a menu callback.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array Output of the system_settings_form()
 */
function apachesolr_settings(array $form, array &$form_state) {
  $form = array();
  $rows = array();

  // Environment settings
  $id = apachesolr_default_environment();
  $environments = apachesolr_load_all_environments();
  $default_environment = apachesolr_default_environment();
  apachesolr_check_facetapi();

  // Reserve a row for the default one
  $rows[$default_environment] = array();

  foreach ($environments as $environment_id => $data) {
    // Define all the Operations
    $confs = array();
    $ops = array();
    // Whenever facetapi is enabled we also enable our operation link
    if (module_exists('facetapi')) {
      $confs['facets'] = array(
        'class' => 'operation',
        'data' => l(t('Facets'),
          'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/facets',
          array('query' => array('destination' => current_path()))
        ),
      );
    }
    // These are our result and bias settings
    if (module_exists('apachesolr_search')) {
      $confs['result_bias'] = array(
        'class' => 'operation',
        'data' => l(t('Bias'),
          'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/bias',
          array('query' => array('destination' => current_path()))
        ),
      );
    }
    $confs['index'] = array(
      'class' => 'operation',
      'data' => l(t('Index'),
        'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/index'
      ),
    );
    $ops['edit'] = array(
      'class' => 'operation',
      'data' => l(t('Edit'),
        'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/edit',
        array('query' => array('destination' => current_path()))
      ),
    );

    $ops['clone'] = array(
      'class' => 'operation',
      'data' => l(t('Clone'),
        'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/clone',
        array('query' => array('destination' => $_GET['q']))
      ),
    );
    $env_name = l($data['name'], 'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/edit', array('query' => array('destination' => $_GET['q'])));

    // Is this row our default environment?
    if ($environment_id == $default_environment) {
      $env_name = t('!environment <em>(Default)</em>', array('!environment' => $env_name));
      $env_class_row = 'default-environment';
    }
    else {
      $env_class_row = '';
    }
    // For every non-default we add a delete link
    // Allow to revert a search environment or to delete it
    $delete_value = '';
    if (!isset($data['in_code_only'])) {
      if ((isset($data['type']) && $data['type'] == 'Overridden')) {
        $delete_value = array(
          'class' => 'operation',
          'data' => l(t('Revert'), 'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/delete'),
        );
      }
      // don't allow the deletion of the default environment
      elseif ($environment_id != $default_environment) {
        $delete_value = array(
          'class' => 'operation',
          'data' => l(t('Delete'), 'admin/config/search/apachesolr/settings/' . $data['env_id'] . '/delete'),
        );
      }
    }
    $ops['delete'] = $delete_value;

    // When we are receiving a http POST (so the page does not show) we do not
    // want to check the statusses of any environment
    $class = '';
    if (empty($form_state['input'])) {
      $class = apachesolr_server_status($data['url'], $data['service_class']) ? 'ok' : 'error';
    }

    $headers = array(
      array('data' => t('Name'), 'colspan' => 2),
      t('URL'),
      array('data' => t('Configuration'), 'colspan' => count($confs)),
      array('data' => t('Operations'), 'colspan' => count($ops)),
    );

    $rows[$environment_id] = array('data' =>
      array(
        // Cells
        array(
          'class' => 'status-icon',
          'data' => '<div title="' . $class . '"><span class="element-invisible">' . $class . '</span></div>',
        ),
        array(
          'class' => $env_class_row,
          'data' => $env_name,
        ),
        check_plain($data['url']),
      ),
      'class' => array(drupal_html_class($class)),
    );
    // Add the links to the page
    $rows[$environment_id]['data'] = array_merge($rows[$environment_id]['data'], $confs);
    $rows[$environment_id]['data'] = array_merge($rows[$environment_id]['data'], $ops);
  }
  // Custom CSS for Admin ApacheSolr table.
  $form['#attached']['css'][] = drupal_get_path('module', 'apachesolr') . '/apachesolr.css';

  $form['apachesolr_host_settings']['actions'] = array(
    '#markup' => '<ul class="action-links">' . drupal_render($actions) . '</ul>',
  );
  $form['apachesolr_host_settings']['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => array_values($rows),
    '#attributes' => array('class' => array('admin-apachesolr')),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced configuration'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['advanced']['apachesolr_set_nodeapi_messages'] = array(
    '#type' => 'radios',
    '#title' => t('Extra help messages for administrators'),
    '#description' => t('Adds notices to a page whenever Drupal changed content that needs reindexing'),
    '#default_value' => variable_get('apachesolr_set_nodeapi_messages', 1),
    '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
  );

  // Number of Items to index
  $numbers = drupal_map_assoc(array(1, 5, 10, 20, 50, 100, 200));
  $default_cron_limit = variable_get('apachesolr_cron_limit', 50);

  // apachesolr_cron_limit may be overridden in settings.php. If its current
  // value is not among the default set of options, add it.
  if (!isset($numbers[$default_cron_limit])) {
    $numbers[$default_cron_limit] = $default_cron_limit;
  }
  $form['advanced']['apachesolr_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to index per cron run'),
    '#default_value' => $default_cron_limit,
    '#options' => $numbers,
    '#description' => t('Reduce the number of items to prevent timeouts and memory errors while indexing.', array('@cron' => url('admin/reports/status')))
  );

  $options = array('apachesolr:show_error' => t('Show error message'));
  $system_info = system_get_info('module');
  if (module_exists('search')) {
    foreach (search_get_info() as $module => $search_info) {
      // Don't allow apachesolr to return results on failure of apachesolr.
      if ($module == 'apachesolr_search') {
        continue;
      }
      $options[$module] = t('Show @name search results', array('@name' => $system_info[$module]['name']));
    }
  }

  $options['apachesolr:show_no_results'] = t('Show no results');
  $form['advanced']['apachesolr_failure'] = array(
    '#type' => 'select',
    '#title' => t('On failure'),
    '#options' => $options,
    '#default_value' => variable_get('apachesolr_failure', 'apachesolr:show_error'),
  );

  $form['advanced']['apachesolr_watchdog_successes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log successful Apache Solr Search actions'),
    '#description' => t('Watchdog will log successful adds and indexes.'),
    '#default_value' => variable_get('apachesolr_watchdog_successes', TRUE),
  );

  return system_settings_form($form);
}

/**
 * Gets information about the fields already in solr index.
 *
 * @param array $environment
 *   The environment for which we need to ask the status from
 *
 * @return array page render array
 */
function apachesolr_status_page($environment = array()) {
  if (empty($environment)) {
    $env_id = apachesolr_default_environment();
    $environment = apachesolr_environment_load($env_id);
  }
  else {
    $env_id = $environment['env_id'];
  }

  // Check for availability
  if (!apachesolr_server_status($environment['url'], $environment['service_class'])) {
    drupal_set_message(t('The server seems to be unavailable. Please verify the server settings at the <a href="!settings_page">settings page</a>', array('!settings_page' => url("admin/config/search/apachesolr/settings/{$environment['env_id']}/edit", array('query' =>  drupal_get_destination())))), 'warning');
    return '';
  }

  try {
    $solr = apachesolr_get_solr($environment["env_id"]);
    $solr->clearCache();
    $data = $solr->getLuke();
  }
  catch (Exception $e) {
    apachesolr_log_exception($environment['env_id'], $e);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    $data = new stdClass;
    $data->fields = array();
  }

  $messages = array();
  if (isset($data->index->numDocs)) {
    try {
      // Collect the stats
      $stats_summary = $solr->getStatsSummary();
      module_load_include('inc', 'apachesolr', 'apachesolr.index');
      $status = apachesolr_index_status($environment["env_id"]);
      // We need a schema version greater than beta3. This is mostly to catch
      // people using the Drupal 6 schema.
      if (preg_match('/^drupal-[13]/', $stats_summary['@schema_version'])) {
        $minimum = 'drupal-3.0-beta4';
        if (version_compare($stats_summary['@schema_version'], $minimum, '<')) {
          drupal_set_message(t('Your schema.xml version is too old. You must update it to at least %minimum and re-index your content.', array('%minimum' => $minimum)), 'error');
        }
      }
      $pending_msg = $stats_summary['@pending_docs'] ? t('(@pending_docs sent but not yet processed)', $stats_summary) : '';
      $index_msg = $stats_summary['@index_size'] ? t('(@index_size on disk)', $stats_summary) : '';
      $indexed_message = t('@num Items !pending !index_msg', array(
        '@num' => $data->index->numDocs,
        '!pending' => $pending_msg,
        '!index_msg' => $index_msg,
      ));
      $messages[] = array(t('Indexed'), $indexed_message);

      $remaining_message = t('@items (@percentage% has been sent to the server)', array(
          '@items' => format_plural($status['remaining'], t('1 item'), t('@count items')),
          '@percentage' => ((int)min(100, 100 * ($status['total'] - $status['remaining']) / max(1, $status['total']))),
        )
      );
      $messages[] = array(t('Remaining'), $remaining_message);

      $messages[] = array(t('Schema'), t('@schema_version', $stats_summary));
      if (!empty($stats_summary['@core_name'])) {
        $messages[] = array(t('Solr Core Name'), t('@core_name', $stats_summary));
      }
      $messages[] = array(t('Delay'), t('@autocommit_time before updates are processed.', $stats_summary));
      $messages[] = array(t('Pending Deletions'), t('@deletes_total', $stats_summary));
    }
    catch (Exception $e) {
      apachesolr_log_exception($environment['env_id'], $e);
    }
  }
  if (empty($messages)) {
    $messages[] = array(t('Error'), t('No data was returned from the server. Check your log messages.'));
  }
  // Initializes output with information about which server's setting we are
  // editing, as it is otherwise not transparent to the end user.
  $output['apachesolr_index_action_status'] = array(
    '#prefix' => '<h3>' . t('@environment: Search Index Content', array('@environment' => $environment['name'])) . '</h3>',
    '#theme' => 'table',
    '#header' => array(t('Type'), t('Value')),
    '#rows' => $messages,
  );

  $output['viewmore'] = array(
    '#markup' => l(t('View more details on the search index contents'), 'admin/reports/apachesolr'),
  );

  $write_status = apachesolr_environment_variable_get($env_id, 'apachesolr_read_only', APACHESOLR_READ_WRITE);
  if ($write_status == APACHESOLR_READ_WRITE) {
     $output['index_action_form'] = drupal_get_form('apachesolr_index_action_form', $env_id);
     $output['index_config_form'] = drupal_get_form('apachesolr_index_config_form', $env_id);
  }
  else {
    drupal_set_message(t('Options for deleting and re-indexing are not available because the index is read-only. This can be changed on the <a href="!settings_page">settings page</a>', array('!settings_page' => url('admin/config/search/apachesolr/settings/' . $env_id . '/edit', array('query' =>  drupal_get_destination())))), 'warning');
  }

  return $output;
}

/**
 * Get the report, eg.: some statistics and useful data from the Apache Solr index
 *
 * @param array $environment
 *
 * @return array page render array
 */
function apachesolr_index_report(array $environment = array()) {
  if (empty($environment)) {
    $env_id = apachesolr_default_environment();
    drupal_goto('admin/reports/apachesolr/' . $env_id);
  }
  $environments = apachesolr_load_all_environments();
  $environments_list = array();
  foreach ($environments as $env) {
    $var_status = array('!name' =>$env['name']);
    $environments_list[] = l(t('Statistics for !name', $var_status), 'admin/reports/apachesolr/' . $env['env_id']);
  }
  $output['environments_list'] = array(
    '#theme' => 'item_list',
    '#items' => $environments_list,
  );

  try {
    $solr = apachesolr_get_solr($environment['env_id']);
    $solr->clearCache();
    $data = $solr->getLuke();
  }
  catch (Exception $e) {
    apachesolr_log_exception($environment['env_id'], $e);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    return $output;
  }

  $messages = array();
  $messages[] = array(t('Number of documents in index'), $data->index->numDocs);

  $limit = variable_get('apachesolr_luke_limit', 20000);
  if (isset($data->index->numDocs) && $data->index->numDocs > $limit) {
    $messages[] = array(t('Limit'), t('You have more than @limit documents, so term frequencies are being omitted for performance reasons.', array('@limit' => $limit)));
    $not_found = t('<em>Omitted</em>');
  }
  elseif (isset($data->index->numDocs)) {
    $not_found = t('Not indexed');
    try {
      $solr = apachesolr_get_solr($environment['env_id']);
      // Note: we use 2 since 1 fails on Ubuntu Hardy.
      $data = $solr->getLuke(2);
      if (isset($data->index->numTerms)) {
        $messages[] = array(t('# of terms in index'), $data->index->numTerms);
      }
    }
    catch (Exception $e) {
      apachesolr_log_exception($environment['env_id'], $e);
      $data->fields = array();
    }
  }
  // Initializes output with information about which server's setting we are
  // editing, as it is otherwise not transparent to the end user.
  $fields = (array)$data->fields;
  if ($fields) {
    $messages[] = array(t('# of fields in index'), count($fields));
  }

  // Output the messages we have for this page
  $output['apachesolr_index_report'] = array(
    '#theme' => 'table',
    '#header' => array('type', 'value'),
    '#rows' => $messages,
  );

  if ($fields) {
    // Initializes table header.
    $header = array(
      'name' => t('Field name'),
      'type' => t('Index type'),
      'terms' => t('Distinct terms'),
    );

    // Builds table rows.
    $rows = array();
    foreach ($fields as $name => $field) {
      // TODO: try to map the name to something more meaningful.
      $rows[$name] = array(
        'name' => $name,
        'type' => $field->type,
        'terms' => isset($field->distinct) ? $field->distinct : $not_found
      );
    }
    ksort($rows);
    // Output the fields we found for this environment
    $output['field_table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  else {
    $output['field_table'] = array('#markup' => t('No data on indexed fields.'));
  }
  return $output;
}

/**
 * Page callback to show available conf files.
 *
 * @param array $environment
 *
 * @return string
 *   A non-render array but plain theme output for the config files overview. Could be done better probably
 */
function apachesolr_config_files_overview(array $environment = array()) {
  if (empty($environment)) {
    $env_id = apachesolr_default_environment();
  }
  else {
    $env_id = $environment['env_id'];
  }

  $xml = NULL;
  try {
    $solr = apachesolr_get_solr($env_id);
    $response = $solr->makeServletRequest('admin/file', array('wt' => 'xml'));
    $xml = simplexml_load_string($response->data);
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
  }

  if ($xml) {
    // Retrieve our items from the xml using xpath
    $items = $xml->xpath('//lst[@name="files"]/lst');

    // Add all the data of the file in a files array
    $files = array();
    foreach ($items as $item_id => $item) {
      // Do not list directories. Always a bool
      if (isset($item->bool)) {
        continue;
      }
      // Get data from the files.
      $name = ((string)$item->attributes()) ? (string)$item->attributes() : t('No name found');
      $files[$item_id]['name'] = l($name, 'admin/reports/apachesolr/' . $env_id . '/conf/' . $name, array('query' => array('no_fast_404' => 1)));

      // Retrieve the date attribute
      if (isset($item->date)) {
        $modified = ((string)$item->date->attributes() == 'modified') ? (string) $item->date : t('No date found');
        $files[$item_id]['modified'] = format_date(strtotime($modified));
      }

      // Retrieve the size attribute
      if (isset($item->long)) {
        $size = ((string)$item->long->attributes() == 'size') ? (string) $item->long : t('No size found');
        $files[$item_id]['size'] = t('Size (bytes): @bytes', array('@bytes' => $size));
      }
    }
    // Sort our files alphabetically
    ksort($files);

    // Initializes table header.
    $header = array(
      'name' => t('File name'),
      'date' => t('Modified'),
      'size' => t('Size'),
    );

    // Display the table of field names, index types, and term counts.
    $variables = array(
      'header' => $header,
      'rows' => $files,
    );
    $output = theme('table', $variables);
  }
  else {
    $output = '<p>' . t('No data about any file found.') . "</p>\n";
  }
  return $output;
}

/**
 * Page callback to show one conf file.
 *
 * @param string $name
 * @param array $environment
 *
 * @return string
 *   the requested config file
 */
function apachesolr_config_file($name, array $environment = array()) {
  if (empty($environment)) {
    $env_id = apachesolr_default_environment();
  }
  else {
    $env_id = $environment['env_id'];
  }

  $output = '';
  try {
    $solr = apachesolr_get_solr($env_id);
    $response = $solr->makeServletRequest('admin/file', array('file' => $name));
    $raw_file = $response->data;
    $output = '<pre>' . check_plain($raw_file) . '</pre>';
    drupal_set_title(check_plain($name));
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
  }
  return $output;
}

/**
 * Form builder for the Apachesolr Indexer actions form.
 *
 * @param array $form
 * @param array $form_state
 * @param string $env_id
 *   The machine name of the environment.
 * @see apachesolr_index_action_form_delete_submit().
 *
 * @return array $form
 */
function apachesolr_index_action_form(array $form, array $form_state, $env_id) {
  $form = array();
  $form['action'] = array(
    '#type' => 'fieldset',
    '#title' => t('Actions'),
    '#collapsible' => TRUE,
  );

  $form['action']['env_id'] = array(
    '#type' => 'value',
    '#value' => $env_id,
  );

  $form['action']['cron'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Index queued content (!amount)', array('!amount' => variable_get('apachesolr_cron_limit', 50))),
    '#submit' => array('apachesolr_index_action_form_cron_submit'),
  );
  $form['action']['cron_description'] = array(
    '#prefix' => '<span>',
    '#suffix' => '</span></div>',
    '#markup' => t('Indexes just as many items as 1 cron run would do.'),
  );

  $form['action']['remaining'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Index all queued content'),
    '#submit' => array('apachesolr_index_action_form_remaining_submit'),
  );
  $form['action']['remaining_description'] = array(
    '#prefix' => '<span>',
    '#suffix' => '</span></div>',
    '#markup' => t('Could take time and could put an increased load on your server.'),
  );

  $form['action']['reset'] = array(
    '#prefix' => '<div>',
    '#suffix' => '</div>',
    '#type' => 'submit',
    '#value' => t('Queue all content for reindexing'),
    '#submit' => array('apachesolr_index_action_form_reset_submit'),
  );
  $form['action']['delete'] = array(
    '#prefix' => '<div>',
    '#type' => 'submit',
    '#value' => t('Delete the Search & Solr index'),
    '#submit' => array('apachesolr_index_action_form_delete_submit'),
  );
  $form['action']['delete_description'] = array(
    '#prefix' => '<span>',
    '#suffix' => '</span></div>',
    '#markup' => t('Useful with a corrupt index or a new schema.xml.'),
  );

  return $form;
}

/**
 * Submit handler for the Indexer actions form, delete button.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_remaining_submit(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $env_id = $form_state['values']['env_id'];
  $form_state['redirect'] = array('admin/config/search/apachesolr/settings/' . $env_id . '/index/remaining', array('query' => $destination));
}

/**
 * Submit handler for the Indexer actions form, delete button.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_delete_submit(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $env_id = $form_state['values']['env_id'];
  $form_state['redirect'] = array('admin/config/search/apachesolr/settings/' . $env_id . '/index/delete', array('query' => $destination));
}

/**
 * Submit handler for the Indexer actions form, delete button.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_reset_submit(array $form, array &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $env_id = $form_state['values']['env_id'];
  $form_state['redirect'] = array('admin/config/search/apachesolr/settings/' . $env_id . '/index/reset', array('query' => $destination));
}

/**
 * Submit handler for the deletion form.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_cron_submit(array $form, array &$form_state) {
  if (!empty($form_state['build_info']['args'][0])) {
    $env_id = $form_state['build_info']['args'][0];
    $form_state['redirect'] = 'admin/config/search/apachesolr/settings/' . $env_id . '/index';
  }
  else {
    $env_id = apachesolr_default_environment();
    $form_state['redirect'] = 'admin/config/search/apachesolr';
  }
  apachesolr_cron($env_id);
  drupal_set_message(t('Apachesolr cron successfully executed'));
}

/**
 * Form builder for to reindex the remaining items left in the queue.
 *
 * @see apachesolr_index_action_form_delete_confirm_submit().
 *
 * @param array $form
 * @param array $form_state
 * @param array $environment
 *
 * @return mixed
 */
function apachesolr_index_action_form_remaining_confirm(array $form, array &$form_state, array $environment) {
  return confirm_form($form,
    t('Are you sure you want index all remaining content?'),
    'admin/config/search/apachesolr/settings/' . $environment['env_id'] . '/index',
    NULL,
    t('Index all remaining')
  );
}

/**
 * Submit handler for the deletion form.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_remaining_confirm_submit(array $form, array &$form_state) {
  if (!empty($form_state['build_info']['args'][0]['env_id'])) {
    $env_id = $form_state['build_info']['args'][0]['env_id'];
    $form_state['redirect'] = 'admin/config/search/apachesolr/settings/' . $env_id . '/index';
  }
  else {
    $env_id = apachesolr_default_environment();
    $form_state['redirect'] = 'admin/config/search/apachesolr';
  }
  apachesolr_index_batch_index_remaining($env_id);
}

/**
 * Form builder for the index re-enqueue form.
 *
 * @see apachesolr_index_action_form_reset_confirm_submit().
 *
 * @param array $form
 * @param array $form_state
 * @param array $environment
 *
 * @return mixed
 */
function apachesolr_index_action_form_reset_confirm(array $form, array &$form_state, array $environment) {
  return confirm_form($form,
    t('Are you sure you want to queue content for reindexing?'),
    'admin/config/search/apachesolr/settings/' . $environment['env_id'] . '/index',
    t('All content on the site will be queued for indexing. The documents currently in the Solr index will remain searchable.'),
    t('Queue all content')
  );
}

/**
 * Submit handler for the deletion form.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_reset_confirm_submit(array $form, array &$form_state) {
  if (!empty($form_state['build_info']['args'][0]['env_id'])) {
    $env_id = $form_state['build_info']['args'][0]['env_id'];
    $form_state['redirect'] = 'admin/config/search/apachesolr/settings/' . $env_id . '/index';
  }
  else {
    $env_id = apachesolr_default_environment();
    $form_state['redirect'] = 'admin/config/search/apachesolr';
  }
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  apachesolr_index_mark_for_reindex($env_id);
  drupal_set_message(t('All the content on your site is queued for indexing. You can wait for it to be indexed during cron runs, or you can manually reindex it.'));
}

/**
 * Form builder for the index delete/clear form.
 *
 * @see apachesolr_index_action_form_delete_confirm_submit().

 * @param array $form
 * @param array $form_state
 * @param array $environment
 *
 * @return array output of confirm_form()
 */
function apachesolr_index_action_form_delete_confirm(array $form, array &$form_state, array $environment) {
  return confirm_form($form,
    t('Are you sure you want to clear your index?'),
    'admin/config/search/apachesolr/settings/' . $environment['env_id'] . '/index',
    t('This will remove all data from your index and all search results will be incomplete until your site is reindexed.'),
    t('Delete index')
  );
}

/**
 * Submit handler for the deletion form.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_action_form_delete_confirm_submit(array $form, array &$form_state) {
  if (!empty($form_state['build_info']['args'][0]['env_id'])) {
    $env_id = $form_state['build_info']['args'][0]['env_id'];
    $form_state['redirect'] = 'admin/config/search/apachesolr/settings/' . $env_id . '/index';
  }
  else {
    $env_id = apachesolr_default_environment();
    $form_state['redirect'] = 'admin/config/search/apachesolr';
  }
  // Rebuild our tracking table.
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  if (apachesolr_index_delete_index($env_id)) {
    drupal_set_message(t('The index has been deleted.'));
  }
  else {
    drupal_set_message(t('An error occured while trying to delete the index.'), 'error');
  }
}

/**
 * Submit a batch job to index the remaining, non-indexed content.
 *
 * @param string $env_id
 *   The environment ID where it needs to index the remaining items for
 */
function apachesolr_index_batch_index_remaining($env_id, $total_limit = null) {
  $batch = array(
    'operations' => array(
      array(
        'apachesolr_index_batch_index_entities',
        array(
          $env_id,
          $total_limit,
        ),
      ),
    ),
    'finished' => 'apachesolr_index_batch_index_finished',
    'title' => t('Indexing'),
    'init_message' => t('Preparing to submit content to Solr for indexing...'),
    'progress_message' => t('Submitting content to Solr...'),
    'error_message' => t('Solr indexing has encountered an error.'),
    'file' => drupal_get_path('module', 'apachesolr') . '/apachesolr.admin.inc',
  );
  batch_set($batch);
}


/**
 * Batch Operation Callback
 *
 * @param string $env_id
 *   The machine name of the environment.
 * @param $total_limit
 *   The total number of items to index across all batches
 * @param array $context
 *
 * @return false
 *   return false when an exception was caught
 *
 * @throws Exception
 *   When solr gives an error, throw an exception that solr is not available
 */
function apachesolr_index_batch_index_entities($env_id, $total_limit = NULL, &$context) {
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  if (empty($context['sandbox'])) {
    try {
      // Get the $solr object
      $solr = apachesolr_get_solr($env_id);
      // If there is no server available, don't continue.
      if (!$solr->ping()) {
        throw new Exception(t('No Solr instance available during indexing.'));
      }
    }
    catch (Exception $e) {
      apachesolr_log_exception($env_id, $e);
      return FALSE;
    }

    $status = apachesolr_index_status($env_id);
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['submitted'] = 0;

    // How many items do we want to index? All or a limited set of items
    if (empty($total_limit)) {
      $context['sandbox']['max'] = $status['remaining'];
    }
    else {
      $context['sandbox']['max'] = $total_limit;
    }
  }

  // We can safely process the apachesolr_cron_limit nodes at a time without a
  // timeout or out of memory error.
  $limit = variable_get('apachesolr_cron_limit', 50);

  // Reduce the limit for our final batch if we would be processing more than had been requested
  if ($limit + $context['sandbox']['progress'] > $context['sandbox']['max']) {
    $limit = $context['sandbox']['max'] - $context['sandbox']['progress'];
  }

  if ($context['sandbox']['max'] >= $context['sandbox']['progress'] + $limit) {
    $context['sandbox']['progress'] += $limit;
  }
  else {
    $context['sandbox']['progress'] = $context['sandbox']['max'];
  }
  $context['sandbox']['submitted'] += apachesolr_index_entities($env_id, $limit);

  $arguments = array(
    '@current' => $context['sandbox']['progress'],
    '@total' => $context['sandbox']['max'],
    '@submitted' => $context['sandbox']['submitted'],
    );
  $context['message'] = t('Inspected @current of @total entities. Submitted @submitted documents to Solr', $arguments);

  // Inform the batch engine that we are not finished, and provide an
  // estimation of the completion level we reached.
  $context['finished'] = empty($context['sandbox']['max']) ? 1 : $context['sandbox']['progress'] / $context['sandbox']['max'];

  // Put the total into the results section when we're finished so we can
  // show it to the admin.
  if ($context['finished']) {
    $context['results']['count'] = $context['sandbox']['progress'];
    $context['results']['submitted'] = $context['sandbox']['submitted'];
    $context['results']['env_id'] = $env_id;
  }
}

/**
 * Batch 'finished' callback
 *
 * @param bool $success
 *   Whether the batch ended with success or not
 * @param array $results
 * @param array $operations
 */
function apachesolr_index_batch_index_finished($success, array $results, array $operations) {
  $message = '';
  // $results['count'] will not be set if Solr is unavailable.
  if (isset($results['count'])) {
    $message .= format_plural($results['count'], '1 item processed successfully. ', '@count items successfully processed. ');
  }
  if (isset($results['submitted'])) {
    $message .= format_plural($results['submitted'], '1 document successfully sent to Solr.', '@count documents successfully sent to Solr.');
  }
  if ($success) {
    // Directly process those documents if direct commit was enabled.
    $direct_commit = apachesolr_environment_variable_get($results['env_id'], 'apachesolr_direct_commit', FALSE);
    if ($direct_commit) {
      // Get the $solr object
      $solr = apachesolr_get_solr($results['env_id']);
      $solr->commit();
    }
    $type = 'status';
  }
  else {
    // An error occurred. $operations contains the unprocessed operations.
    $error_operation = reset($operations);
    $message .= ' ' . t('An error occurred while processing @num with arguments: @args', array('@num' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE)));
    $type = 'error';
  }
  drupal_set_message($message, $type);
}

/**
 * Form builder for the bundle configuration form.
 *
 * @see apachesolr_index_config_form_submit().
 *
 * @param array $form
 * @param array $form_state
 * @param string $env_id
 *   The machine name of the environment.
 *
 * @return array $form
 */
function apachesolr_index_config_form(array $form, array $form_state, $env_id) {
  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Configuration'),
    '#collapsible' => TRUE,
  );

  $form['config']['bundles'] = array(
    '#type' => 'markup',
    '#markup' => t('Select the entity types and bundles that should be indexed.'),
  );

  // For future extensibility, when we have multiple cores.
  $form['config']['env_id'] = array(
    '#type' => 'value',
    '#value' => $env_id,
  );

  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (!empty($entity_info['apachesolr']['indexable'])) {
      $options = array();
      foreach ($entity_info['bundles'] as $key => $info) {
        $options[$key] = $info['label'];
      }
      asort($options);

      $form['config']['entities']['#tree'] = TRUE;
      $form['config']['entities'][$entity_type] = array(
        '#type' => 'checkboxes',
        '#title' => check_plain($entity_info['label']),
        '#options' => $options,
        '#default_value' => apachesolr_get_index_bundles($env_id, $entity_type),
      );
    }
  }

  $form['config']['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Submit handler for the bundle configuration form.
 *
 * @param array $form
 * @param array $form_state
 */
function apachesolr_index_config_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  $form_values = $form_state['values'];
  $env_id = $form_values['env_id'];

  foreach ($form_values['entities'] as $entity_type => $bundles) {
    $existing_bundles = apachesolr_get_index_bundles($env_id, $entity_type);
    $all_bundles = array_keys($bundles);
    $new_bundles = array_values(array_filter($bundles));
    apachesolr_index_set_bundles($env_id, $entity_type, $new_bundles);

    // Remove all excluded bundles - this happens on form submit
    // even if there is no change so the admin can remove
    // bundles if there was an error.
    $excluded_bundles = array_diff($all_bundles, $new_bundles);
    if (apachesolr_index_delete_bundles($env_id, $entity_type, $excluded_bundles)) {
      $callback = apachesolr_entity_get_callback($entity_type, 'bundles changed callback');
      if (!empty($callback)) {
        call_user_func($callback, $env_id, $existing_bundles, $new_bundles);
      }
    }
    else {
      drupal_set_message(t('Search is temporarily unavailable. If the problem persists, please contact the site administrator.'), 'error');
    }
  }

  // Clear the entity cache, since we will be changing its data.
  entity_info_cache_clear();
  cache_clear_all('apachesolr:environments', 'cache_apachesolr');
  drupal_set_message(t('Your settings have been saved.'));
}

/**
 * Page callback for node/%node/devel/apachesolr.
 *
 * @param object $node
 * @return string debugging information
 */
function apachesolr_devel($node) {
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  $item = new stdClass();
  $item->entity_type = 'node';
  $item->entity_id = $node->nid;
  $output = '';
  foreach (apachesolr_load_all_environments() as $env_id => $environment) {
    $documents = apachesolr_index_entity_to_documents($item, $env_id);
    $output .= '<h1>' . t('Environment %name (%env_id)', array('%name' => $environment['name'], '%env_id' => $env_id)). '</h1>';
    foreach ($documents as $document) {
      $debug_data = array();
      foreach ($document as $key => $value) {
        $debug_data[$key] = $value;
      }
      $output .= kdevel_print_object($debug_data);
    }
  }
  return $output;
}
