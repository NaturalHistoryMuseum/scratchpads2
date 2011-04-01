<?php

/**
 * @file
 *   Administrative pages for the Apache Solr framework.
 */

/**
 * Form builder for adding/editing a Solr server used as a menu callback.
 */
function apachesolr_server_delete_page($server) {
  $is_default = $server['server_id'] == variable_get('apachesolr_default_server', 'solr');
  if ($is_default) {
    return MENU_ACCESS_DENIED;
  }
  return drupal_get_form('apachesolr_server_delete_form', $server);
}

function apachesolr_server_delete_form($form, &$form_state, $server) {
  $form['server_id'] = array(
    '#type' => 'value',
    '#value' => $server['server_id'],
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete server %name?', array('%name' => $server['name'])),
    'admin/config/search/apachesolr',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function apachesolr_server_delete_form_submit($form, &$form_state) {
  if (apachesolr_server_delete($form_state['values']['server_id'])) {
    drupal_set_message(t('The server was deleted'));
  }
  $form_state['redirect'] = 'admin/config/search/apachesolr';
}

/**
 * Form builder for adding/editing a Solr server used as a menu callback.
 */
function apachesolr_server_edit_form($form, &$form_state, $server = NULL) {
  
  if (empty($server)) {
    $server = array();
  }
  $server += array('server_id' => '', 'name' => '', 'url' => '', 'service_class' => '');

  $form['original_server_id'] = array(
    '#type' => 'value',
    '#value' => $server['server_id'],
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr server URL'),
    '#default_value' => $server['url'],
    '#description' => t('URL that identifies the Solr server and request path to be used. E.g. http://localhost:8983/solr'),
    '#required' => TRUE,
  );
  $is_default = $server['server_id'] == variable_get('apachesolr_default_server', 'solr');
  $form['make_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make this Solr server the default'),
    '#default_value' => $is_default,
    '#disabled' => $is_default,
    '#description' => $is_default ? t('Select this option for a different server to make this one non-default.') : '',
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $server['name'],
    '#description' => t('Short description of your Solr server.'),
    '#required' => TRUE,
  );
  $form['server_id'] = array(
    '#type' => 'machine_name',
    '#title' => t('Server id'),
    '#machine_name' => array(
      'exists' => 'apachesolr_server_load',
    ),
    '#default_value' => $server['server_id'],
    '#description' => t('Unique, machine-readable identifier Solr server.'),
    '#required' => TRUE,
  );
  $form['service_class'] = array(
    '#type' => 'value',
    '#value' => $server['service_class'],
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#validate' => array('apachesolr_server_save_validate'),
    '#submit' => array('apachesolr_server_save_submit'),
    '#value' => t('Save'),
  );
  $form['test'] = array(
    '#type' => 'submit',
    '#validate' => array('apachesolr_server_save_validate'),
    '#submit' => array('apachesolr_server_test_submit'),
    '#value' => t('Test connection'),
  );
  if (!empty($server['server_id']) && !$is_default) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#submit' => array('apachesolr_server_delete_submit'),
      '#value' => t('Delete'),
    );
  }
  return $form;
}

function apachesolr_server_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/search/apachesolr/server/' . $form_state['values']['original_server_id'] . '/delete';
}

function apachesolr_server_test_submit($form, &$form_state) {
  $ping = apachesolr_server_status($form_state['values']['url'], $form_state['values']['service_class']);
  if ($ping) {
    drupal_set_message(t('Your site has contacted the Apache Solr server.'));
  }
  else {
    drupal_set_message(t('Your site was unable to contact the Apache Solr server.'), 'error');
  }
  $form_state['rebuild'] = TRUE;
}

function apachesolr_server_save_validate($form, &$form_state) {
  $parts = parse_url($form_state['values']['url']);
  foreach (array('scheme', 'host', 'path') as $key) {
    if (empty($parts[$key])) {
      form_set_error('url', t('The Solr server URL needs to include a !part', array('!part' => $key)));
    }
  }
  if (isset($parts['port'])) {
    // TODO: Port range should be 0-65535, but 0 crashes apachesolr
    if (!ctype_digit($parts['port']) || $parts['port'] < 1 || $parts['port'] > 65535) {
      form_set_error('port', t('The port has to be an integer between 1 and 65535.'));
    }
    else {
      // Normalize the path to have just a leading slash.
      $form_state['values']['url'] = trim($form_state['values']['url'], "/ \t\r\n\0\x0B");
    }
  }
}

function apachesolr_server_save_submit($form, &$form_state) {
  apachesolr_server_save($form_state['values'], $form_state['values']['original_server_id']);
  if (!empty($form_state['values']['make_default'])) {
    variable_set('apachesolr_default_server', $form_state['values']['server_id']);
  }  
  cache_clear_all('apachesolr:servers', 'cache_apachesolr');
  $form_state['redirect'] = 'admin/config/search/apachesolr';
}

/**
 * Form builder for general settings used as a menu callback.
 */
function apachesolr_settings($form, &$form_state) {
  $form = array();
  $collapse_host = TRUE;

  // Perform a check to ensure the server is there
  if (empty($_POST)) {
    module_load_install('apachesolr');
    foreach (apachesolr_requirements('runtime') as $requirement) {
      $status = $requirement['severity'] == REQUIREMENT_ERROR ? 'error' : 'status';
      if ($status == 'error') {
        drupal_set_message($requirement['title'] . ': ' . $requirement['value'], $status);
        $collapse_host = FALSE;
      }
    }
  }
  // Add a link to add more mlt blocks.
  $form['mlt_link'] = array(
    '#type' => 'item',
    '#description' => format_plural(count(apachesolr_mlt_list_blocks()), 'You currently have 1 block.', 'You currenly have @count blocks.'),
  );
  $form['apachesolr_host_settings'] = array(
    '#title' => t('Solr host settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => $collapse_host,
  );
  $id = variable_get('apachesolr_default_server', 'solr');
  $servers = apachesolr_load_all_servers();
  $rows = array();
  $headers = array(
    t('Default'),
    array('data' => t('Operations'), 'colspan' => 2),
    t('Name'),
    t('URL'),
  );
  foreach ($servers as $server_id => $data) {
    $is_default = $server_id == variable_get('apachesolr_default_server', 'solr');
    if ($is_default) {
      $type = 'selected';
      $delete_link = '';
    }
    else {
      $type = 'none';
      $delete_link = array('class' => 'operation', 'data' => l(t('delete'), 'admin/config/search/apachesolr/server/' . $data['server_id'] . '/delete'));
    }
    $rows[] = array('data' =>
      array(
        // Cells
        array('class' => 'icon'),
        array('class' => 'operation', 'data' => l(t('edit'), 'admin/config/search/apachesolr/server/' . $data['server_id'] . '/edit')),
        $delete_link,
        check_plain($data['name']),
        check_plain($data['url']),
      ),
      // Attributes for tr
      'class' => array(drupal_html_class('apachesolr-' . $type)),
    );
  }
  $actions[] = array(
    '#theme' => 'menu_local_action',
    '#link' => array('title' => t('Add new server'), 'href' => 'admin/config/search/apachesolr/server/add', 'options' => array()),
  );

  $form['apachesolr_host_settings']['actions'] = array(
    '#markup' => '<ul class="action-links">' . drupal_render($actions) . '</ul>',
  );
  $form['apachesolr_host_settings']['table'] = array(
    '#theme' => 'table',
    '#header' => $headers,
    '#rows' => $rows,
    '#attributes' => array('class' => array('admin-apachesolr')),
  );
  
  $numbers = drupal_map_assoc(array(1, 5, 10, 20, 50, 100, 200));
  $default_cron_limit = variable_get('apachesolr_cron_limit', 50);
  // apachesolr_cron_limit may be overridden in settings.php. If its current
  // value is not among the default set of options, add it.
  if (!isset($numbers[$default_cron_limit])) {
    $numbers[$default_cron_limit] = $default_cron_limit;
  }
  $form['apachesolr_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to index per cron run'),
    '#default_value' => $default_cron_limit,
    '#options' => $numbers,
    '#description' => t('The maximum number of items indexed in each pass of a <a href="@cron">cron maintenance task</a>. If necessary, reduce the number of items to prevent timeouts and memory errors while indexing.', array('@cron' => url('admin/reports/status')))
  );

  $form['apachesolr_rows'] = array(
    '#type' => 'textfield',
    '#title' => t('Results per page'),
    '#default_value' => variable_get('apachesolr_rows', 10),
    '#description' => t('The number of results that will be shown per page.'),
  );
  $form['apachesolr_facetstyle'] = array(
    '#type' => 'radios',
    '#title' => t('Style of facet links'),
    '#default_value' => variable_get('apachesolr_facetstyle', 'checkboxes'),
    '#options' => array('links' => t('Links only'), 'checkboxes' => t('Links with checkboxes')),
  );

  $options = array('apachesolr:show_error' => t('Show error message'));
  $system_info = system_get_info('module');
  foreach (search_get_info() as $module => $search_info) {
    // Don't allow apachesolr to return results on failure of apachesolr.
    if ($module == 'apachesolr_search') {
      continue;
    }
    $options[$module] = t('Show @name search results', array('@name' => $system_info[$module]['name']));
  }
  $options['apachesolr:show_no_results'] = t('Show no results');
  $form['apachesolr_failure'] = array(
    '#type' => 'select',
    '#title' => t('On failure'),
    '#options' => $options,
    '#default_value' => variable_get('apachesolr_failure', 'apachesolr:show_error'),
    '#description' => t('What to display if Apache Solr search is not available.'),
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
    '#default_value' => variable_get('apachesolr_set_nodeapi_messages', 1),
    '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
  );
  $form['advanced']['apachesolr_read_only'] = array(
    '#type' => 'radios',
    '#title' => t('Index write access'),
    '#default_value' => apachesolr_server_variable_get(apachesolr_default_server(), 'apachesolr_read_only', APACHESOLR_READ_WRITE),
    '#options' => array(APACHESOLR_READ_WRITE => t('Read and write (normal)'), APACHESOLR_READ_ONLY => t('Read only')),
    '#description' => t('<em>Read only</em> stops this site from sending updates to your search index. Useful for development sites.'),
  );
  return system_settings_form($form);
}

/**
 * Validation function for the apachesolr_settings() form.
 */
function apachesolr_settings_validate($form, &$form_state) {
  $intval = intval($form_state['values']['apachesolr_rows']);
  // TODO: does the max need to be variable?
  $max = 200;
  if (!is_numeric($form_state['values']['apachesolr_rows']) || $intval < 0 || $intval > $max) {
    form_set_error('apachesolr_rows', t('Results per page should be non-negative integer less than @max.', array('@max' => $max)));
  }
  else {
    // Set the integer value as the one we are saving to the variable.
    $form_state['values']['apachesolr_rows'] = $intval;
  }
}

/**
 * Gets information about the fields already in solr index.
 */
function apachesolr_index_page($server_id = NULL) {
  try {
    $solr = apachesolr_get_solr($server_id);
    // TODO: possibly clear this every page view if we are running multi-site.
    if (apachesolr_index_get_last_updated()) {
      $solr->clearCache();
    }
    $data = $solr->getLuke();
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    $data = new stdClass;
    $data->fields = array();
  }

  if (empty($server_id)) {
    $server_id = apachesolr_default_server();
  }

  $output = array();
  $status_header = '';
  if (isset($data->index->numDocs)) {
    $pending_docs = $delay_msg = $delete_msg = '';
    try {
      $stats_summary = $solr->getStatsSummary();
      $solr_msg = '<p>' . t('Using schema.xml version: <strong>@schema_version</strong>', $stats_summary);
      if ($stats_summary['@core_name']) {
        $solr_msg .= '<br />' . t('Solr core name: <strong>@core_name</strong>', $stats_summary);
      }
      $delay_msg = '<br />' . t('<em>The server has a @autocommit_time delay before updates are processed.</em>', $stats_summary) . "</p>\n";
      $delete_msg = '<p>' . t('Number of pending deletions: @deletes_total', $stats_summary) . "</p>\n";
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    }
    $status_header .= $solr_msg . $delay_msg;
    $pending_msg = $stats_summary['@pending_docs'] ? t('(@pending_docs sent but not yet processed)', $stats_summary) : '';
    $status_header .= '<p>' . t('Number of documents in index: @num !pending', array('@num' => $data->index->numDocs, '!pending' => $pending_msg)) . "</p>\n";
    $status_header .= $delete_msg;
  }
  $status_header .= '<p>' . l(t('View more details on the search index contents'), 'admin/reports/apachesolr') . "</p>\n";
  if (apachesolr_server_variable_get($server_id, 'apachesolr_read_only', APACHESOLR_READ_WRITE) == APACHESOLR_READ_WRITE) {
    $output['apachesolr_index_action_status_header'] = array('#markup' => $status_header);
    // Display the Delete Index form.
    $output['apachesolr_index_action_form'] = drupal_get_form('apachesolr_index_action_form');
  }
  else {
    drupal_set_message(t('The index is in read-only mode. Options for deleting and re-indexing are therefore not available. The index mode can be changed on the !settings_page', array('!settings_page' => l(t('settings page'), 'admin/config/search/apachesolr'))), 'warning');
  }

  return $output;
}

function apachesolr_index_report($server_id = NULL) {
  if (empty($server_id)) {
    $server_id = apachesolr_default_server();
  }
  try {
    $solr = apachesolr_get_solr($server_id);
    // TODO: possibly clear this every page view if we are running multi-site.
    if (apachesolr_index_get_last_updated()) {
      $solr->clearCache();
    }
    $data = $solr->getLuke();
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    $data->fields = array();
  }

  $output = '<p>' . t('Number of documents in index: @num !pending', array('@num' => $data->index->numDocs, '!pending' => '')) . "</p>\n";

  $limit = variable_get('apachesolr_luke_limit', 20000);
  if (isset($data->index->numDocs) && $data->index->numDocs > $limit) {
    $output .= '<p>' . t('You have more than @limit documents, so term frequencies are being omitted for performance reasons.', array('@limit' => $limit)) . "</p>\n";
    $not_found = t('<em>Omitted</em>');
  }
  elseif (isset($data->index->numDocs)) {
    $not_found = t('Not indexed');
    try {
      $solr = apachesolr_get_solr($server_id);
      // Note: we use 2 since 1 fails on Ubuntu Hardy.
      $data = $solr->getLuke(2);
      $output .= '<p>' . t('Number of terms in index: @num', array('@num' => $data->index->numTerms)) . "</p>\n";
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
      $data->fields = array();
    }
  }

  $fields = (array)$data->fields;
  if ($fields) {
    $output .= '<p>' . t('Number of fields in index: @num', array('@num' => count($fields))) . "</p>\n";
    $rows = array();
    foreach ($fields as $name => $field) {
      // TODO: try to map the name to something more meaningful.
      $rows[$name] = array($name, $field->type, isset($field->distinct) ? $field->distinct : $not_found);
    }
    ksort($rows);
    // Display the table of Field names, Index Types, and term counts.
    $output .= theme('table', array(t('Field name'), t('Index type'), t('Distinct terms')), $rows);
  }
  else {
    $output .= '<p>' . t('No data on indexed fields.') . "</p>\n";
  }
  return $output;
}

/**
 * Indicates what order the specified facets should be listed in.  This function is used in a usort
 * invocation.
 * @param $a
 *   The first facet.
 * @param $b
 *   The second facet.
 * @return
 *   A signed integer that indicates which of the specified facets should come first.
 */
function apachesolr_sort_facets($a, $b) {
  return strcasecmp($a['info'], $b['info']);
}

/**
 * This is the submit handler for the active facets form.
 *
 * The form values for each module are array filtereed to remove non-enabled items and
 * stored in the variable table with the name 'apachesolr_enabled_facets'.
 *
 * @see apachesolr_enabled_facets_form()
 */
function apachesolr_enabled_facets_form_submit($form, &$form_state) {
  $enabled = array();
  foreach ($form_state['values']['apachesolr_enabled_facets'] as $module => $facets) {
    $enabled[$module] = array_filter($facets);
  }
  apachesolr_save_enabled_facets($form['#server_id'], $enabled);
  // This cache being stale can prevent new facet filters from working.
  apachesolr_clear_cache($form['#server_id']);
  $msg = t('The Apache Solr filters settings were changed.  To arrange the blocks for your enabled filters, visit the <a href="@url">blocks administration page</a>.', array('@url' => url('admin/structure/block')));
  drupal_set_message($msg, 'warning');
}

/**
 * Creates the form that allows the user to select which facets will be enabled.
 *
 * Only enabled facets are sent to solr.  Fewer enabled facets can reduce the
 * load on the search server.  Blocks are only offered for enabled facets, so
 * this also reduces the clutter on the blocks admin page.
 */
function apachesolr_enabled_facets_form($form, &$form_state, $server_id = NULL) {
  if (empty($server_id)) {
    $server_id = apachesolr_default_server();
  }
  $facets = array();
  $module_facets = array();
  $module_list = array();
  foreach (module_implements('apachesolr_facets') as $module) {
    $return = module_invoke($module, 'apachesolr_facets');
    if (!empty($return)) {
      $module_facets[$module] = $return;
      uasort($module_facets[$module], 'apachesolr_sort_facets');
      $module_list[$module] = $module;
    }
  }

  $enabled_facets = apachesolr_get_enabled_facets($server_id);
  $form = array();
  $form['#server_id'] = $server_id;
  $form['apachesolr_enabled_facets']['help'] = array(
    '#type' => 'item',
    '#markup' => t('You can use this screen to select which search filter blocks should be created by enabling the corresponding filters on this page. For performance reasons, you should only enable filters that you intend to have available to users on the search page.  After selecting which filter blocks to create, you will be sent to the blocks page where you can choose which of those blocks should be enabled when your users search by placing each block in a region.'),
  );
  if ($module_list) {
    $result = db_select('system')
      ->fields('system', array('name', 'info'))
      ->condition('name', $module_list, 'IN')
      ->condition('type', 'module')
      ->execute();
    foreach ($result as $item) {
      $module_list[$item->name] = unserialize($item->info);
    }
  }
  foreach ($module_facets as $module => $facets) {
    $form['apachesolr_enabled_facets'][$module] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($module_list[$module]['name']),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    // We must use module + delta as the keys since that combination is
    // guaranteed to be unique.  A single module could, for example, have
    // two different blocks that expose different faceting on the same
    // field in the index.
    foreach ($facets as $delta => $data) {
      $form['apachesolr_enabled_facets'][$module][$delta] = array(
        '#type' => 'checkbox',
        '#title' => $data['info'],
        '#return_value' => $data['facet_field'],
        '#default_value' => isset($enabled_facets[$module][$delta]) ? $data['facet_field'] : 0,
      );
    }
  }

  $has_facets = (bool)$module_facets;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => $has_facets,
  );
  $form['no-facets-message'] = array(
    '#markup' => t('<em>No filters are available from your currently enabled modules</em>'),
    '#access' => !$has_facets,
  );

  $form['#tree'] = TRUE;

  return $form;
}

/**
 * Create a form for deleting the contents of the Solr index.
 */
function apachesolr_index_action_form() {
  $form = array();

  $form['action'] = array(
    '#type' => 'fieldset',
    '#title' => t('Index Actions'),
  );
  // Jump through some forms hoops to get a description under each radio button.
  $actions = array(
    'remaining' => array(
      'title' => t('Index queued content'),
      'description' => t('Any content that is queued for indexing will be submitted to Solr immediately. Depending on amount of content on the site, it may take a long time to complete, and may place an increased load on your server.'),
    ),
    'reindex' => array(
      'title' => t('Queue content for reindexing'),
      'description' => t('All content on the site will be queued for indexing. The documents currently in the Solr index will remain searchable. The content will be gradually resubmitted to Solr during cron runs.'),
    ),
    'delete' => array(
      'title' => t('Delete the index'),
      'description' => t('All documents in the Solr index will be deleted. This is rarely necessary unless your index is corrupt or you have installed a new schema.xml. After doing this your content will need to be resubmitted for indexing.'),
    ),
  );
  foreach ($actions as $key => $action) {
    // Generate the parents as the autogenerator does, so we will have a
    // unique id for each radio button.
    $form['action'][$key] = array(
      '#type' => 'radio',
      '#title' => $action['title'],
      '#default_value' => 'remaining',
      '#return_value' => $key,
      '#parents' => array('action'),
      '#description' => $action['description'],
      '#id' => drupal_clean_css_identifier('edit-' . implode('-', array('action', $key))),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Begin'),
    '#submit' => array('apachesolr_index_action_form_submit'),
  );

  return $form;
}

/**
 * Submit function for the index action form.
 */
function apachesolr_index_action_form_submit($form, &$form_state) {
  switch ($form_state['values']['action']) {
    case 'remaining':
      apachesolr_batch_index_remaining();
      break;

    case 'reindex':
      $form_state['redirect'] = 'admin/config/search/apachesolr/index/confirm/clear';
      break;

    case 'delete':
      $form_state['redirect'] = 'admin/config/search/apachesolr/index/confirm/delete';
      break;
  }
}

/**
 * Confirmation form for "Re-index all content" button
 * @see apachesolr_clear_index_submit()
 */
function apachesolr_clear_index_confirm() {
  $form = array();
  return confirm_form($form, t('Are you sure you want to queue content for reindexing?'), 'admin/config/search/apachesolr/index', t('All content on the site will be queued for indexing. The documents currently in the Solr index will remain searchable. The content will be gradually resubmitted to Solr during cron runs.'), t('Reindex'), t('Cancel'));
}

/**
 * Submit function for the "Re-index all content" confirmation form.
 *
 * @see apachesolr_clear_index_confirm()
 */
function apachesolr_clear_index_confirm_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/search/apachesolr/index';
  apachesolr_rebuild_index_table();
  drupal_set_message(t('All the content on your site is queued for indexing. You can wait for it to be indexed during cron runs, or you can manually reindex it.'), 'warning');
}

/**
 * Confirmation form for "Delete the index" button
 * @see apachesolr_delete_index_submit()
 */
function apachesolr_delete_index_confirm() {
  $form = array();
  return confirm_form($form, t('Are you sure you want to delete the search index?'), 'admin/config/search/apachesolr/index', t("All documents in the Solr index will be deleted. This is rarely necessary unless your index is corrupt or you have installed a new schema.xml. After doing this your content will need to be queued for indexing."), t('Delete'), t('Cancel'));
}

/**
 * Submit function for the "Delete the index" confirmation form.
 *
 * @see apachesolr_delete_index_confirm()
 */
function apachesolr_delete_index_confirm_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/search/apachesolr/index';
  try {
    apachesolr_delete_index();
    drupal_set_message(t('The Apache Solr content index has been erased. All the content on your site is queued for indexing. You can wait for it to be indexed during cron runs, or you can manually reindex it.'), 'warning');
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
  }
}

/**
 * Utility function to delete the index and reset all index counters.
 *
 * @param $type
 *   a single content type to be deleted from the index.
 *
 * @throws Exception
 */
function apachesolr_delete_index($type = NULL) {
  // Instantiate a new Solr object.
  $solr = apachesolr_get_solr();
  if ($type) {
    $query = 'bundle:' . $type;
  }
  else {
    $query = '*:*';
  }
  // Allow other modules to modify the delete query.
  // For example, use the site hash so that you only delete this site's
  // content:  $query = 'hash:' . apachesolr_site_hash()
  drupal_alter('apachesolr_delete_index', $query);
  $solr->deleteByQuery($query);
  $solr->commit();
  // Rebuild our node-tracking table.
  apachesolr_rebuild_index_table();
  apachesolr_index_set_last_updated(REQUEST_TIME);
}

/**
 * MoreLikeThis administration and utility functions.
 */

function apachesolr_mlt_add_block_form() {
  $form = apachesolr_mlt_block_form();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => '5',
  );
  return $form;
}

function apachesolr_mlt_add_block_form_submit($form, &$form_state) {
  apachesolr_mlt_save_block($form_state['values']);
  drupal_set_message(t('New content recommendation block created. Drag it into a region to enable it'));
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Page callback to show available conf files.
 */
function apachesolr_config_files_overview() {
  $output = array();
  $xml = NULL;
  try {
    $solr = apachesolr_get_solr();
    $response = $solr->makeServletRequest('admin/file');
    $xml = simplexml_load_string($response->data);
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
  }
  if ($xml) {
    $files = $xml->xpath('//lst[@name="files"]/lst');
    $items = array();
    foreach ($files as $file) {
      $atr = $file->attributes();
      $name = (string) $atr[0];
      $str = l($name, 'admin/reports/apachesolr/conf/' . $name);
      $str .= '<br />' . format_date(strtotime((string) $file->date));
      $str .= '<br />' . t('Size (bytes): @bytes', array('@bytes' => (string) $file->long));
      $items[$name] = $str;
    }
    ksort($items);
    $output = theme('item_list', array('items' => array_values($items)));
  }
  return $output;
}

/**
 * Page callback to show one conf file.
 */
function apachesolr_config_file($name) {
  $output = '';
  try {
    $solr = apachesolr_get_solr();
    $response = $solr->makeServletRequest('admin/file', array('file' => $name));
    $raw_file = $response->data;
    $output = '<pre>' . check_plain($raw_file) . '</pre>';
    drupal_set_title(check_plain($name));
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
  }
  return $output;
}

/**
 * Form to edit moreLikeThis block settings.
 *
 * @param int $delta If editing, the id of the block to edit.
 *
 * @return array The form used for editing.
 * TODO:
 *   Add term boost settings.
 *   Enable the user to specify a query, rather then forcing suggestions based
 *     on the node id.
 *
 */
function apachesolr_mlt_block_form($delta = NULL) {
  if (isset($delta)) {
    $block = apachesolr_mlt_load_block($delta);
    if (!$block) {
      return array();
    }
  }
  else {
    $block = apachesolr_mlt_block_defaults();
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Block name'),
    '#description' => t('The block name displayed to site users.'),
    '#required' => TRUE,
    '#default_value' => $block['name'],
    '#weight' => '-2',
  );
  $form['num_results'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of related items to display'),
    '#default_value' => $block['num_results'],
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)),
    '#weight' => -1,
    );
  $form['mlt_fl'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields for finding related content'),
    '#description' => t('Choose the fields to be used in calculating similarity. The default combination of %taxonomy_names and %title will provide relevant results for typical sites.', array("%taxonomy_names" => apachesolr_field_name_map("taxonomy_names"), "%title" => apachesolr_field_name_map("label"))),
    '#options' => apachesolr_mlt_get_fields(),
    '#required' => TRUE,
    '#default_value' => $block['mlt_fl'],
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced configuration'),
    '#weight' => '1',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7));
  $form['advanced']['mlt_mintf'] = array(
    '#type' => 'select',
    '#title' => t('Minimum term frequency'),
    '#description' => t('A word must appear this many times in any given document before the document is considered relevant for comparison.'),
    '#default_value' => $block['mlt_mintf'],
    '#options' => $options,
  );
  $form['advanced']['mlt_mindf'] = array(
    '#type' => 'select',
    '#title' => t('Minimum document frequency'),
    '#description' => t('A word must occur in at least this many documents before it will be used for similarity comparison.'),
    '#default_value' => $block['mlt_mindf'],
    '#options' => $options,
  );
  $form['advanced']['mlt_minwl'] = array(
    '#type' => 'select',
    '#title' => t('Minimum word length'),
    '#description' => 'You can use this to eliminate short words such as "the" and "it" from similarity comparisons. Words must be at least this number of characters or they will be ignored.',
    '#default_value' => $block['mlt_minwl'],
    '#options' => $options,
  );
  $form['advanced']['mlt_maxwl'] = array(
    '#type' => 'select',
    '#title' => t('Maximum word length'),
    '#description' => t('You can use this to eliminate very long words from similarity comparisons. Words of more than this number of characters will be ignored.'),
    '#default_value' => $block['mlt_maxwl'],
    '#options' => drupal_map_assoc(array(8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)),
  );
  $form['advanced']['mlt_maxqt'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of query terms'),
    '#description' => t('The maximum number of query terms that will be included in any query. Lower numbers will result in fewer recommendations but will get results faster. If a content recommendation is not returning any recommendations, you can either check more "Comparison fields" checkboxes or increase the maximum number of query terms here.'),
    '#options' => drupal_map_assoc(array(3, 5, 7, 10, 12, 15, 20, 25, 30, 35, 40)),
    '#default_value' => $block['mlt_maxqt'],
  );

  $form['restrictions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#weight' => '1',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $type_options = array();
  foreach (node_type_get_types() as $key => $type) {
    $type_options[$key] = $type->name;
  }

  $form['restrictions']['mlt_type_filters'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content Types'),
    '#default_value' => is_array($block['mlt_type_filters']) ? $block['mlt_type_filters'] : array_keys($type_options),
    '#options' => $type_options,
    '#description' => t('Select the content types that similarity suggestions should be restricted to. Multiple types are joined with an OR query, so selecting more types results in more recommendations.'),
    '#weight' => '-2',
  );

  $form['restrictions']['mlt_custom_filters'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional Query'),
    '#description' => t("An additional query, in Lucene syntax, which will further filter the similarity suggestions. Ex. 'label:strategy' will filter related content further to only those with strategy in the title. Likewise 'content:HDTV^3' will boost the term HDTV by a factor of 3. Here are some more examples:") .
                        '<ul>
                            <li>ss_language:fr</li>
                            <li>tid:5 OR tid 7</li>
                            <li>ds_created:[2009-05-01T23:59:59Z TO 2009-07-28T12:30:00Z]</li>
                            <li>-is_uid:0 AND -is_uid:1</li>
                        </ul>',
    '#required' => FALSE,
    '#default_value' => $block['mlt_custom_filters'],
    '#weight' => '-1',
  );

  return $form;
}

/**
 * Merge supplied settings with the standard defaults..
 */
function apachesolr_mlt_block_defaults($block = array()) {
  return $block + array(
    'name' => '',
    'num_results' => '5',
    'mlt_fl' => array(
      'label' => 'label',
      'taxonomy_names' => 'taxonomy_names',
    ),
    'mlt_mintf' => '1',
    'mlt_mindf' => '1',
    'mlt_minwl' => '3',
    'mlt_maxwl' => '15',
    'mlt_maxqt' => '20',
    'mlt_type_filters' => array_keys(node_type_get_types()),
    'mlt_custom_filters' => '',
  );
}

/**
 * Constructs a list of field names used on the settings form.
 *
 * @return array An array containing a the fields in the solr instance.
 */
function apachesolr_mlt_get_fields() {
  $solr = apachesolr_get_solr();
  $fields = $solr->getFields();
  $rows = array();
  foreach ($fields as $field_name => $field) {
    if ($field->schema{4} == 'V') {
      $rows[$field_name] = apachesolr_field_name_map($field_name);
    }
  }
  ksort($rows);
  return $rows;
}

/**
 * A helper function to save MLT block data.
 *
 * If passed a block delta, the function will update block settings. If it is
 * not passed a block delta, the function will create a new block.
 *
 * @param array $block_settings An array containing the settings required to form
 * a moreLikeThis request.
 *
 * @param int $delta The id of the block you wish to update.
 */
function apachesolr_mlt_save_block($block_settings = array(), $delta = NULL) {
  $blocks = variable_get('apachesolr_mlt_blocks', array());
  if (is_null($delta)) {
    $count = 0;
    ksort($blocks);
    // Construct a new array key.
    if (end($blocks)) {
      list(, $count) = explode('-', key($blocks));
    }
    $delta = sprintf('mlt-%03d', 1 + $count);
  }
  $defaults = apachesolr_mlt_block_defaults();
  // Remove stray form values.
  $blocks[$delta] = array_intersect_key($block_settings, $defaults) + $defaults;
  // Eliminate non-selected fields.
  $blocks[$delta]['mlt_fl'] = array_filter($blocks[$delta]['mlt_fl']);
  $blocks[$delta]['mlt_type_filters'] = array_filter($blocks[$delta]['mlt_type_filters']);
  $blocks[$delta]['mlt_custom_filters'] = trim($blocks[$delta]['mlt_custom_filters']);
  variable_set('apachesolr_mlt_blocks', $blocks);
}

function apachesolr_mlt_delete_block_form($form_state, $delta) {
  if ($block = apachesolr_mlt_load_block($delta)) {
    $form['delta'] = array(
      '#type' => 'value',
      '#value' => $delta
    );

    return confirm_form($form,
      t('Are you sure you want to delete the Apache Solr content recommendation block %name?', array('%name' => $block['name'])),
      'admin/structure/block',
      t('The block will be deleted. This action cannot be undone.'),
      t('Delete'), t('Cancel'));
  }
}

function apachesolr_mlt_delete_block_form_submit($form, &$form_state) {
  $blocks = variable_get('apachesolr_mlt_blocks', array());
  unset($blocks[$form_state['values']['delta']]);
  variable_set('apachesolr_mlt_blocks', $blocks);
  drupal_set_message(t('The block has been deleted.'));
  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Batch reindex functions.
 */

/**
 * Submit a batch job to index the remaining, unindexed content.
 */
function apachesolr_batch_index_remaining() {
  $batch = array(
    'operations' => array(
      array('apachesolr_batch_index_nodes', array()),
    ),
    'finished' => 'apachesolr_batch_index_finished',
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
 */
function apachesolr_batch_index_nodes(&$context) {
  if (empty($context['sandbox'])) {
    try {
      // Get the $solr object
      $solr = apachesolr_get_solr();
      // If there is no server available, don't continue.
      if (!$solr->ping()) {
        throw new Exception(t('No Solr instance available during indexing.'));
      }
    }
    catch (Exception $e) {
      watchdog('Apache Solr', $e->getMessage(), NULL, WATCHDOG_ERROR);
      return FALSE;
    }

    $status = apachesolr_index_status('apachesolr_search');
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $status['remaining'];
  }

  // We can safely process the apachesolr_cron_limit nodes at a time without a
  // timeout or out of memory error.
  $limit = variable_get('apachesolr_cron_limit', 50);

  // With each pass through the callback, retrieve the next group of nids.
  $rows = apachesolr_get_nodes_to_index('apachesolr_search', $limit);
  $pos = apachesolr_index_nodes($rows, 'apachesolr_search');

  $context['sandbox']['progress'] += $pos['nodes_processed'];
  $context['message'] = t('Indexed @current of @total nodes', array('@current' => $context['sandbox']['progress'], '@total' => $context['sandbox']['max']));

  // Inform the batch engine that we are not finished, and provide an
  // estimation of the completion level we reached.
  $context['finished'] = empty($context['sandbox']['max']) ? 1 : $context['sandbox']['progress'] / $context['sandbox']['max'];

  // Put the total into the results section when we're finished so we can
  // show it to the admin.
  if ($context['finished']) {
    $context['results']['count'] = $context['sandbox']['progress'];
  }
}

/**
 * Batch 'finished' callback
 */
function apachesolr_batch_index_finished($success, $results, $operations) {
  $message = '';
  // $results['count'] will not be set if Solr is unavailable.
  if (isset($results['count'])) {
    $message .= format_plural($results['count'], '1 item processed successfully.', '@count items successfully processed.');
  }
  if ($success) {
    $type = 'status';
  }
  else {
    // An error occurred. $operations contains the unprocessed operations.
    $error_operation = reset($operations);
    $message .= ' ' . t('An error occurred while processing @num with arguments :', array('@num' => $error_operation[0])) . print_r($error_operation[0], TRUE);
    $type = 'error';
  }
  drupal_set_message($message, $type);
}
