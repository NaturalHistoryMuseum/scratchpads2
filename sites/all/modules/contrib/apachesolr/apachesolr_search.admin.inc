<?php

/**
 * @file
 *   Administrative settings for searching.
 */

/**
 * Helper function for empty search configuration.
 */
function _apachesolr_search_browse_form($default_value) {
  $description = t('This is what is shown when the user enters an empty search, or removes all filters from an active search.') . ' ';
  if (!module_exists('facetapi')) {
    $description .= t('<strong>Facets will not be shown until you enable Facet API module.</strong>');
  }
  else {
    $description .= t('Remember to configure the facets on the <a href="!facetslink">search environment page</a> and assign blocks to regions on the <a href="!blocklink">block settings page</a>', array(
      '!facetslink' => url('admin/config/search/apachesolr/settings/'),
      '!blocklink' => url('admin/structure/block'),
    ));
  }
  return array(
    '#type' => 'radios',
    '#title' => t('Behavior on empty search'),
    '#options' => array(
      'none' => t("Show search box"),
      'browse'  => t("Show enabled facets' blocks under the search box"),
      'blocks'  => t("Show enabled facets' blocks in their configured regions"),
      'results' => t("Show enabled facets' blocks in their configured regions and first page of all available results"),
    ),
    '#default_value' => $default_value,
    '#description' => $description,
  );
}

/**
 * Menu callback for the overview page showing custom search pages and blocks.
 * @return array $build
 */
function apachesolr_search_page_list_all() {
  $build['pages'] = apachesolr_search_page_list_pages();
  $build['blocks'] = apachesolr_search_page_list_blocks();
  return $build;

}

/**
 * Listing of all the search pages
 * @return array $build
 */
function apachesolr_search_page_list_pages() {
  $build = array();
  $rows = array();
  $rows['core_search'] = array();

  // Build the sortable table header.
  $header = array(
    'label' => array('data' => t('Name'), 'field' => 's.label'),
    'path' => array('data' => t('Path'), 'field' => 's.search_path'),
    'environment' => array('data' => t('Search environment')),
    'operations' => array('data' => t('Operations')),
  );

  $search_pages = apachesolr_search_load_all_search_pages();
  $default_search_page = apachesolr_search_default_search_page();
  foreach ($search_pages as $search_page) {
    $row = array();

    // Add the label
    $label = check_plain($search_page['label']);
    // Is this row our default environment?
    if ($search_page['page_id'] == $default_search_page) {
      $label = t('!search_page <em>(Default)</em>', array('!search_page' => $label));
    }

    $row[] = $label;
    // Add the link
    $row[] = array(
      'data' => array(
        '#type' => 'link',
        '#title' => $search_page['search_path'],
        '#href' => $search_page['search_path'],
      ),
    );

    // Add the search environment
    $environment = apachesolr_environment_load($search_page['env_id']);
    $row[] = $environment ? check_plain($environment['name']) : check_plain(t('<Disabled>'));
    // Operations
    $row[] = array('data' => l(t('Edit'), 'admin/config/search/apachesolr/search-pages/' . $search_page['page_id'] . '/edit'));
    $row[] = array('data' => l(t('Clone'), 'admin/config/search/apachesolr/search-pages/' . $search_page['page_id'] . '/clone'));

    // Allow to revert a search page or to delete it
    if (!isset($search_page['settings']['apachesolr_search_not_removable']) && !isset($search_page['in_code_only'])) {
      if ((isset($search_page['type']) && $search_page['type'] == 'Overridden')) {
        $row[] = array('data' => l(t('Revert'), 'admin/config/search/apachesolr/search-pages/' . $search_page['page_id'] . '/delete'));
      } else {
        $row[] = array('data' => l(t('Delete'), 'admin/config/search/apachesolr/search-pages/' . $search_page['page_id'] . '/delete'));
      }
    }
    else {
      $row[] = '';
    }
    $rows[$search_page['page_id']] = $row;
  }

  // Automatically enlarge our header with the operations size
  $header['operations']['colspan'] = count(reset($rows)) - 3;

  $build['list'] = array(
    '#prefix' => '<h3>Pages</h3>',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array_values($rows),
    '#empty' => t('No available search pages.'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 20,
    '#weight' => 10,
  );

  return $build;
}

/**
 * Listing of all the search blocks
 * @return array $build
 */
function apachesolr_search_page_list_blocks() {
  $build = array();
  $rows = array();

  // Build the sortable table header.
  $header = array(
    'label' => array('data' => t('Name'), 'field' => 's.label'),
    'environment' => array('data' => t('Search environment')),
    'operations' => array('data' => t('Operations')),
  );

  $search_blocks = variable_get('apachesolr_search_mlt_blocks', array());
  foreach ($search_blocks as $search_block_id => $search_block) {
    $row = array();

    // Add the label
    $label = check_plain($search_block['name']);
    $row[] = $label;

    // Add the search environment
    $environment = apachesolr_environment_load($search_block['mlt_env_id']);
    $row[] = $environment ? check_plain($environment['name']) : check_plain(t('<Disabled>'));
    // Operations
    if (module_exists('block')) {
      $row[] = array('data' => l(t('Configure'), 'admin/structure/block/manage/apachesolr_search/' . $search_block_id . '/configure', array('query' => array('destination' => current_path()))));
    }
    $row[] = array('data' => l(t('Delete'), 'admin/config/search/apachesolr/search-pages/block/' . $search_block_id . '/delete'));
    $rows[$search_block_id] = $row;
  }

  // Automatically enlarge our header with the operations size
  $header['operations']['colspan'] = count(reset($rows)) - 2;

  $build['list'] = array(
    '#prefix' => '<h3>Blocks "More Like This"</h3>',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array_values($rows),
    '#empty' => t('No available search blocks.'),
  );
  $build['pager'] = array(
    '#theme' => 'pager',
    '#quantity' => 20,
    '#weight' => 10,
  );

  return $build;
}

/**
 * Menu callback/form-builder for the form to create or edit a search page.
 */
function apachesolr_search_page_settings_form($form, &$form_state, $search_page = NULL) {
  $environments = apachesolr_load_all_environments();
  $options = array('' => t('<Disabled>'));
  foreach ($environments as $id => $environment) {
    $options[$id] = $environment['name'];
  }
  // Validate the env_id.
  if (!empty($search_page['env_id']) && !apachesolr_environment_load($search_page['env_id'])) {
    $search_page['env_id'] = '';
  }

  // Initializes form with common settings.
  $form['search_page'] = array(
      '#type' => 'value',
      '#value' => $search_page,
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#required' => TRUE,
    '#size' => 30,
    '#maxlength' => 32,
    '#default_value' => !empty($search_page['label']) ? $search_page['label'] : '',
    '#description' => t('The human-readable name of the search page configuration.'),
  );

  $form['page_id'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'apachesolr_search_page_exists',
      'source' => array('label'),
    ),
    '#default_value' => !empty($search_page['page_id']) ? $search_page['page_id'] : '',
    '#disabled' => !empty($search_page),
    '#description' => t('A unique machine-readable identifier for the search page configuration. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Description'),
    '#default_value' => !empty($search_page['description'])  ? TRUE : FALSE
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Provide description'),
    '#title_display' => 'invisible',
    '#size' => 64,
    '#default_value' => !empty($search_page['description']) ? $search_page['description'] : '',
    '#dependency' => array(
      'edit-description-enable' => array(1),
    ),
  );

  $is_default = FALSE;
  if (!empty($search_page)) {
    $is_default = $search_page['page_id'] == apachesolr_search_default_search_page();
  }
  $form['make_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make this Solr Search Page the default'),
    '#description' => t('Useful for eg. making facets to link to this page when they are shown on non-search pages'),
    '#default_value' => $is_default,
    '#disabled' => $is_default,
  );

  $form['info'] = array(
    '#title' => t('Search Page Information'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#prefix' => '<div id="dynamic-search-page">',
    '#suffix' => '</div>',
  );

  $core_search = FALSE;
  if (!empty($search_page['page_id']) && ($search_page['page_id'] == 'core_search')) {
    $core_search = TRUE;
  }
  if ($core_search) {
    $description = t('This page always uses the current default search environment');
  }
  else {
    $description = t('The environment that is used by this search page. If no environment is selected, this page will be disabled.');
  }

  $form['info']['env_id'] = array(
    '#title' => t('Search environment'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => !empty($search_page['env_id']) ? $search_page['env_id'] : '',
    '#disabled' => $core_search,
    '#description' => $description,
  );

  $form['info']['page_title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#description' => t('You can use %value to put the value of % in the path into the title and %terms to put the search terms into the title. Please note if you have several active search modules, this refers to the title of the tab instead of that of the page.'),
    '#default_value' => !empty($search_page['page_title']) ? $search_page['page_title'] : '',
  );

  $search_types = apachesolr_search_load_all_search_types();
  $options = array();
  foreach ($search_types as $id => $search_type) {
    $options[$id] = $search_type['name'];
  }

  $form['info']['search_type'] = array(
    '#title' => t('Search Type'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => !empty($search_page['settings']['apachesolr_search_search_type']) ? $search_page['settings']['apachesolr_search_search_type'] : '',
    '#access' => !$core_search,
    '#description' => t('Use this only when filtering on a value from the search path.
      For example, select Taxonomy Term to filter on a term ID (search/taxonomy/%).'),
    '#ajax' => array(
      'callback' => 'apachesolr_search_ajax_search_page_default',
      'wrapper' => 'dynamic-search-page',
      'method' => 'replace',
    ),
  );

  // Token element validate is added to validate the specific
  // tokens that are allowed
  $form['info']['search_path'] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#maxlength' => 255,
    '#description' => t('For example: search/my-search-page. Search keywords will appear at the end of the path.'),
    '#default_value' => !empty($search_page['search_path']) ? $search_page['search_path'] : '',
  );
  if (!$core_search) {
    $form['info']['search_path']['#description'] .= ' ' . t('You can use one % to make the search page dynamic.');
  }

  $form['info']['custom_filter_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Custom Filter'),
    '#default_value' => !empty($search_page['settings']['apachesolr_search_custom_enable'])  ? TRUE : FALSE
  );

  if (!$core_search) {
    $additional_help_text = t('E.g. "bundle:blog, is_uid:(1 OR 2 OR %). % will be replaced by the value of % in the path"');
  }
  else {
    $additional_help_text = t('E.g. "bundle:blog, is_uid:(1 OR 2)"');
  }
  $t_args = array('!additional_help_text' => $additional_help_text);
  $form['info']['filters'] = array(
    '#title' => t('Custom filters'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#maxlength' => 255,
    '#description' => t('A comma-separated list of lucene filter queries to apply by default.') . ' ' . $additional_help_text,
    '#default_value' => !empty($search_page['settings']['fq'])  ? implode(', ', $search_page['settings']['fq']) : '',
    '#dependency' => array(
      'edit-custom-filter-enable' => array(1),
      'edit-search-type' => array('custom'),
    ),
  );

  $form['advanced'] = array(
    '#title' => t('Advanced Search Page Options'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  // Results per page per search page
  $default_value = isset($search_page['settings']['apachesolr_search_per_page']) ? $search_page['settings']['apachesolr_search_per_page'] : '10';
  $form['advanced']['apachesolr_search_per_page'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#title' => t('Results per page'),
    '#description' => t('How many items will be displayed on one page of the search result.'),
    '#default_value' => $default_value,
  );

  // Enable/disable spellcheck on pages
  $default_value = isset($search_page['settings']['apachesolr_search_spellcheck']) ? $search_page['settings']['apachesolr_search_spellcheck'] : TRUE;
  $form['advanced']['apachesolr_search_spellcheck'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable spell check'),
    '#description' => t('Display "Did you mean … ?" above search results.'),
    '#default_value' => $default_value,
  );

  // Enable/disable search form on search page (replaced by a block perhaps)
  $default_value = isset($search_page['settings']['apachesolr_search_search_box']) ? $search_page['settings']['apachesolr_search_search_box'] : TRUE;
  $form['advanced']['apachesolr_search_search_box'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable the search box on the page'),
    '#description' => t('Display a search box on the page.'),
    '#default_value' => $default_value,
  );

  // Enable/disable search form on search page (replaced by a block perhaps)
  $default_value = isset($search_page['settings']['apachesolr_search_allow_user_input']) ? $search_page['settings']['apachesolr_search_allow_user_input'] : FALSE;
  $form['advanced']['apachesolr_search_allow_user_input'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user input using the URL'),
    '#description' => t('Allow users to use the URL for manual facetting via fq[] params (e.g. http://example.com/search/site/test?fq[]=uid:1&fq[]=tid:99). This will only work in combination with a keyword search. The recommended value is unchecked'),
    '#default_value' => $default_value,
  );

  // Use the main search page setting as the default for new pages.
  $default_value = isset($search_page['settings']['apachesolr_search_browse']) ? $search_page['settings']['apachesolr_search_browse'] : 'browse';
  $form['advanced']['apachesolr_search_browse'] = _apachesolr_search_browse_form($default_value);

  // Button for the corresponding actions
  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#redirect' => 'admin/config/search/apachesolr/search-pages',
    '#value' => t('Save'),
  );
  $form['actions']['submit_edit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and edit'),
  );

  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/search/apachesolr/search-pages',
  );

  $form['#submit'][] = 'apachesolr_search_page_settings_form_submit';

  return $form;
}

/**
 * Callback element needs only select the portion of the form to be updated.
 * Since #ajax['callback'] return can be HTML or a renderable array (or an
 * array of commands), we can just return a piece of the form.
 */
function apachesolr_search_ajax_search_page_default($form, $form_state, $search_page = NULL) {

  $search_page = $form_state['values']['search_page'];
  $search_types = apachesolr_search_load_all_search_types();

  // Helping with sensible defaults for the search path
  $default_search_path = '';
  if (!empty($form_state['values']['search_type']) && $form_state['values']['search_type'] != 'custom') {
    $default_search_path = $search_types[$form_state['values']['search_type']]['default menu'];
    $form['info']['search_path']['#value'] = $default_search_path;
  }

  // Helping with sensible defaults for the search title
  $default_search_title = '';

  if (empty($form_state['values']['page_title']) && $form_state['values']['search_type'] != 'custom') {
    $default_search_title_callback = $search_types[$form_state['values']['search_type']]['title callback'];
    $default_search_title = $default_search_title_callback();
    $form['info']['page_title']['#value'] = $default_search_title;
  }
  return $form['info'];
}

function apachesolr_search_page_settings_form_validate($form, &$form_state) {
  // Performs basic validation of the menu path.
  if (url_is_external($form_state['values']['search_path'])) {
    form_set_error('search_path', t('Path must be local.'));
  }
  $form_state['values']['search_path'] = trim($form_state['values']['search_path'], '/');
  if (empty($form_state['values']['search_path'])) {
    form_set_error('search_path', t('Path required.'));
  }
  if (!is_numeric($form_state['values']['advanced']['apachesolr_search_per_page'])) {
    form_set_error('advanced][apachesolr_search_per_page', t('The amount of search results must be an integer.'));
  }
  $form_state['values']['advanced']['apachesolr_search_per_page'] = (int) $form_state['values']['advanced']['apachesolr_search_per_page'];
  if (empty($form_state['values']['advanced']['apachesolr_search_per_page'])) {
    form_set_error('advanced][apachesolr_search_per_page', t('The amount of search results cannot be empty.'));
  }
  if ($form_state['values']['page_id'] == 'core_search') {
    if (!preg_match('@^search/[^/%]+$@', $form_state['values']['search_path'])) {
      form_set_error('search_path', t('The core Search page path must start with search/ and only have one /'));
    }
  }
  elseif (count(explode('%', $form_state['values']['search_path'])) > 2) {
    form_set_error('search_path', t('Only one % placeholder is allowed.'));
  }
  // Make sure we cannot get duplicate search page paths
  foreach (apachesolr_search_load_all_search_pages() as $search_page) {
    if ($form_state['values']['search_path'] == $search_page['search_path'] && $form_state['values']['page_id'] != $search_page['page_id']) {
      form_set_error('search_path', t('The search path must be unique. This path is already in use by another search page.'));
    }
  }
}

/**
 * Processes apachesolr_search_page_settings_form form submissions.
 */
function apachesolr_search_page_settings_form_submit($form, &$form_state) {
  $settings = array();
  $settings['fq'] = array();
  if ($form_state['values']['filters']) {
    foreach (explode(',', $form_state['values']['filters']) as $string) {
      $string = trim($string);
      // Minimal validation.  ':' must exist and can't be the 1st char..
      if (strpos($string, ':')) {
        $settings['fq'][] = $string;
      }
    }
  }
  $settings['apachesolr_search_custom_enable'] = $form_state['values']['custom_filter_enable'];
  $settings['apachesolr_search_search_type'] = $form_state['values']['search_type'];
  // Add all advanced settings.
  $settings += $form_state['values']['advanced'];

  // Set the default search page settings
  if (!empty($form_state['values']['make_default']) && isset($form_state['values']['page_id'])) {
    variable_set('apachesolr_search_default_search_page', $form_state['values']['page_id']);
  }

  $search_page = array();
  $search_page['page_id'] = $form_state['values']['page_id'];
  $search_page['label'] = $form_state['values']['label'];
  $search_page['description'] = $form_state['values']['description'];
  $search_page['env_id'] = $form_state['values']['env_id'];
  $search_page['search_path'] = $form_state['values']['search_path'];
  $search_page['page_title'] = $form_state['values']['page_title'];
  $search_page['settings'] = $settings;
  apachesolr_search_page_save($search_page);

  // Menu rebuild needed to pick up search path.
  variable_set('menu_rebuild_needed', TRUE);

  // Saves our values in the database, sets redirect path on success.
  drupal_set_message(t('The configuration options have been saved for %page.', array('%page' => $form_state['values']['label'])));
  if (isset($form_state['clicked_button']['#redirect'])) {
    $form_state['redirect'] = $form_state['clicked_button']['#redirect'];
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
 * Deletes a single search page configuration.
 */
function apachesolr_search_delete_search_page_confirm($form, &$form_state, $search_page) {

  // Sets values required for deletion.
  $form['page_id'] = array('#type' => 'value', '#value' => $search_page['page_id']);
  $form['label'] = array('#type' => 'value', '#value' => $search_page['label']);

  if (isset($search_page['export_type']) && $search_page['export_type'] == '3') {
    $verb = t('Revert');
  }
  else {
    $verb = t('Delete');
  }

  // Sets the message, or the title of the page.
  $message = t(
    'Are you sure you want to !verb the %label search page configuration?',
    array('%label' => $form['label']['#value'], '!verb' => strtolower($verb))
  );


  // Builds caption.
  $caption = '<p>';
  $caption .= t(
    'The %label search page configuration will be deleted.',
    array('%label' => $form['label']['#value'])
  );
  $caption .= '</p>';
  $caption .= '<p><strong>' . t('This action cannot be undone.') . '</strong></p>';

  // Finalizes and returns the confirmation form.
  $return_path = 'admin/config/search/apachesolr/search-pages';
  $button_text = $verb;
  if (!isset($search_page['settings']['apachesolr_search_not_removable'])) {
    return confirm_form($form, filter_xss($message), $return_path, filter_xss($caption), check_plain($button_text));
  }
  else {
    // Maybe this should be solved somehow else
    drupal_access_denied();
  }
}

/**
 * Process content type delete confirm submissions.
 */
function apachesolr_search_delete_search_page_confirm_submit($form, &$form_state) {
  // Deletes the index configuration settings.
  // @todo Invoke a hook that allows backends and indexers to delete their stuff.
  db_delete('apachesolr_search_page')
    ->condition('page_id', $form_state['values']['page_id'])
    ->execute();

  // Sets message, logs action.
  drupal_set_message(t(
    'The %label search page configuration has been deleted.',
    array('%label' => $form_state['values']['label'])
  ));
  watchdog('Apache Solr', 'Deleted search page configuration "@page_id".', array('@page_id' => $form_state['values']['page_id']), WATCHDOG_NOTICE);

  // Rebuilds the menu cache.
  menu_rebuild();

  // Returns back to search page list page.
  $form_state['redirect'] = 'admin/config/search/apachesolr/search-pages';
}

/**
 * Clones a single search page configuration
 * @param $search_page
 *   The search page that needs to be cloned
 */
function apachesolr_search_clone_search_page_confirm($form, &$form_state, $search_page) {
  $form['page_id'] = array(
    '#type' => 'value',
    '#value' => $search_page['page_id'],
  );
  return confirm_form(
    $form,
    t('Are you sure you want to clone search page %name?', array('%name' => $search_page['label'])),
    'admin/config/search/apachesolr',
    '',
    t('Clone'),
    t('Cancel')
  );
}

/**
 * Submits the confirmations of the cloning of a search page
 */
function apachesolr_search_clone_search_page_confirm_submit($form, &$form_state) {
  if (apachesolr_search_page_clone($form_state['values']['page_id'])) {
    drupal_set_message(t('The search page was cloned'));
  }
  $form_state['redirect'] = 'admin/config/search/apachesolr/search-pages';
}

/**
 * Menu callback - the settings form.
 */
function apachesolr_search_get_fields($environment = NULL) {
  if (empty($environment)) {
    $env_id = apachesolr_default_environment();
    $environment = apachesolr_environment_load($env_id);
  }
  $env_id = $environment['env_id'];

  // Try to fetch the schema fields.
  try {
    $solr = apachesolr_get_solr($env_id);
    $fields = $solr->getFields();
    return $fields;
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
    drupal_set_message(nl2br(check_plain($e->getMessage())), 'warning');
    drupal_set_message(t('Cannot get information about the fields in the index.'), 'warning');
  }
}

/**
 * Menu callback - Bias settings form.
 */
function apachesolr_bias_settings_page($environment = NULL) {
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

  // Adds content bias and type boost forms.
  $fields = apachesolr_search_get_fields($environment);
  $form = array();
  $form = drupal_get_form('apachesolr_search_bias_form', $env_id, $fields);
  $output['bias_forms'] = $form;
  return $output;
}

function apachesolr_search_bias_form($form, &$form_state, $env_id, $fields) {
  $form['#env_id'] = $env_id;
  $form['bias_tabs'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#submit' => array('apachesolr_search_bias_form_submit'),
  );
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('apachesolr_search_bias_form_reset'),
  );
  $form += apachesolr_search_result_bias_form($env_id);
  $form += apachesolr_search_type_boost_form($env_id);
  $form += apachesolr_search_field_bias_form($fields, $env_id);
  return $form;
}

function apachesolr_search_bias_form_submit(&$form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    // There is no need to set default variable values.
    if (!isset($form[$key]['#default_value']) || $form[$key]['#default_value'] != $value) {
      switch ($key) {
        case 'apachesolr_search_sticky_boost' :
        case 'apachesolr_search_promote_boost' :
        case 'apachesolr_search_date_boost' :
        case 'apachesolr_search_comment_boost' :
        case 'apachesolr_search_changed_boost' :
        case 'apachesolr_search_type_boosts' :
        case 'field_bias' :
          apachesolr_environment_variable_set($form['#env_id'], $key, $value);
      }
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

function apachesolr_search_bias_form_reset($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);

  foreach ($form_state['values'] as $key => $value) {
    apachesolr_environment_variable_del($form['#env_id'], $key);
  }
  drupal_set_message(t('The configuration options have been reset to their default values.'));
}

/**
 * Form builder function to set date, comment, etc biases.
 */
function apachesolr_search_result_bias_form($env_id) {

  $date_settings = apachesolr_environment_variable_get($env_id, 'apachesolr_search_date_boost', '0:0');
  $comment_settings = apachesolr_environment_variable_get($env_id, 'apachesolr_search_comment_boost', '0:0');
  $changed_settings = apachesolr_environment_variable_get($env_id, 'apachesolr_search_changed_boost', '0:0');
  $sticky_boost = apachesolr_environment_variable_get($env_id, 'apachesolr_search_sticky_boost', '0');
  $promote_boost = apachesolr_environment_variable_get($env_id, 'apachesolr_search_promote_boost', '0');

  $options = array(
    '10:2000.0' => '10',
    '8:1000.0' => '9',
    '8:700.0' => '8',
    '8:500.0' => '7',
    '4:300.0' => '6',
    '4:200.0' => '5',
    '4:150.0' => '4',
    '2:150.0' => '3',
    '2:100.0' => '2',
    '1:100.0' => '1',
    '0:0' => t('Ignore'),
  );

  $weights = drupal_map_assoc(array('21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'));
  $weights['0'] = t('Ignore');

  $form = array();
  $form['result_bias'] = array(
    '#type' => 'fieldset',
    '#title' => t('Result biasing'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Give bias to certain properties when ordering the search results. Any value except <em>Ignore</em> will increase the score of the given type in search results. Choose <em>Ignore</em> to ignore any given property.'),
    '#group' => 'bias_tabs',
  );
  $form['result_bias']['apachesolr_search_sticky_boost'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#title' => t("Sticky at top of lists"),
    '#default_value' => $sticky_boost,
    '#description' => t("Select additional bias to give to nodes that are set to be 'Sticky at top of lists'."),
  );
  $form['result_bias']['apachesolr_search_promote_boost'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#title' => t("Promoted to home page"),
    '#default_value' => $promote_boost,
    '#description' => t("Select additional bias to give to nodes that are set to be 'Promoted to home page'."),
  );
  $form['result_bias']['apachesolr_search_date_boost'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t("More recently created"),
    '#default_value' => $date_settings,
    '#description' => t('This setting will change the result scoring so that nodes created more recently may appear before those with higher keyword matching.'),
  );
  $form['result_bias']['apachesolr_search_comment_boost'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t("More comments"),
    '#default_value' => $comment_settings,
    '#description' => t('This setting will change the result scoring so that nodes with more comments may appear before those with higher keyword matching.'),
  );
  $form['result_bias']['apachesolr_search_changed_boost'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t("More recent comments"),
    '#default_value' => $changed_settings,
    '#description' => t('This setting will change the result scoring so that nodes with the most recent comments (or most recent updates to the node itself) may appear before those with higher keyword matching.'),
  );
  return $form;
}

/**
 * Form builder function to set query field weights.
 */
function apachesolr_search_field_bias_form($fields, $env_id) {
  $form = array();
  // get the current weights
  $defaults = array(
    'content' => '1.0',
    'ts_comments' => '0.5',
    'tos_content_extra' => '0.1',
    'label' => '5.0',
    'tos_name' => '3.0',
    'taxonomy_names' => '2.0',
    'tags_h1' => '5.0',
    'tags_h2_h3' => '3.0',
    'tags_h4_h5_h6' => '2.0',
    'tags_inline' => '1.0',
    'tags_a' => '0',
  );
  $qf = apachesolr_environment_variable_get($env_id, 'field_bias', $defaults);
  $weights = drupal_map_assoc(array('21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'));
  $weights['0'] = t('Omit');
  if (!$qf) {
    $qf = $defaults;
  }
  if ($fields) {
    $form['field_bias'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field biases'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#description' => t('Specify here which fields are more important when searching. Give a field a greater numeric value to make it more important. If you omit a field, it will not be searched.'),
      '#group' => 'bias_tabs',
    );
    foreach ($fields as $field_name => $field) {
      // Only indexed feids are searchable.
      if (strpos($field->schema, 'I') !== FALSE) {
        // By default we only show text fields.  Use hook_form_alter to change.
        // We use filter_xss to make sure links are allowed
        $form['field_bias'][$field_name] = array(
          '#access' => ($field->type == 'text' || $field->type == 'text_und'),
          '#type' => 'select',
          '#options' => $weights,
          '#title' => filter_xss(apachesolr_field_name_map($field_name)),
          '#default_value' => isset($qf[$field_name]) ? $qf[$field_name] : '0',
        );
      }
    }

    // Make sure all the default fields are included, even if they have
    // no indexed content.
    foreach ($defaults as $field_name => $weight) {
      $form['field_bias'][$field_name] = array(
        '#type' => 'select',
        '#options' => $weights,
        '#title' => check_plain(apachesolr_field_name_map($field_name)),
        '#default_value' => isset($qf[$field_name]) ? $qf[$field_name] : $defaults[$field_name],
      );
    }

    ksort($form['field_bias']);
  }
  return $form;
}

/**
 * Form builder function to set query type weights.
 */
function apachesolr_search_type_boost_form($env_id) {

  $form['type_boost'] = array(
    '#type' => 'fieldset',
    '#title' => t('Type biasing'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'bias_tabs',
  );
  $form['type_boost']['apachesolr_search_type_boosts'] = array(
    '#type' => 'item',
    '#description' => t("Specify here which node types should get a higher relevancy score in searches. Any value except <em>Ignore</em> will increase the score of the given type in search results."),
    '#tree' => TRUE,
  );

  $weights = drupal_map_assoc(array('21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'));
  $weights['0'] = t('Ignore');

  // Get the current boost values.
  $type_boosts = apachesolr_environment_variable_get($env_id, 'apachesolr_search_type_boosts', array());
  $names = array();
  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (!empty($entity_info['apachesolr']['indexable'])) {
      foreach ($entity_info['bundles'] as $key => $info) {
        $names[$key] = $info['label'];
      }
    }
  }
  asort($names);

  foreach ($names as $type => $name) {
    $form['type_boost']['apachesolr_search_type_boosts'][$type] = array(
      '#type' => 'select',
      '#title' => t('%type type content bias', array('%type' => $name)),
      '#options' => $weights,
      '#default_value' => isset($type_boosts[$type]) ? $type_boosts[$type] : 0,
    );
  }

  return $form;
}

/**
 * MoreLikeThis administration and utility functions.
 */
function apachesolr_search_mlt_add_block_form() {
  $form = apachesolr_search_mlt_block_form();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => '5',
  );
  return $form;
}

function apachesolr_search_mlt_add_block_form_submit($form, &$form_state) {
  apachesolr_search_mlt_save_block($form_state['values']);
  $block_message = t('New More like this block created. <a href="!configure">Configure</a> this block in the Block administration', array('!configure' => url('admin/structure/block')));
  drupal_set_message($block_message);
  $form_state['redirect'] = 'admin/config/search/apachesolr/search-pages';
}

/**
 * Merge supplied settings with the standard defaults..
 */
function apachesolr_search_mlt_block_defaults($block = array()) {
  return $block + array(
    'name' => '',
    'num_results' => '5',
    'mlt_fl' => array(
      'label' => 'label',
      'taxonomy_names' => 'taxonomy_names',
    ),
    'mlt_env_id' => 'solr',
    'mlt_mintf' => '1',
    'mlt_mindf' => '1',
    'mlt_minwl' => '3',
    'mlt_maxwl' => '15',
    'mlt_maxqt' => '20',
    'mlt_type_filters' => array(),
    'mlt_custom_filters' => '',
  );
}

/**
 * Constructs a list of field names used on the settings form.
 * TODO: This always assumes the default environment!
 *
 * @return array An array containing a the fields in the solr instance.
 */
function apachesolr_search_mlt_get_fields() {
  $rows = array();

  $env_id = apachesolr_default_environment();
  try {
    $solr = apachesolr_get_solr($env_id);
    $fields = $solr->getFields();
    foreach ($fields as $field_name => $field) {
      // 'V' appears in different positions in different Solr versions.
      if (strpos($field->schema, 'V') !== FALSE) {
        $rows[$field_name] = apachesolr_field_name_map($field_name);
      }
    }
    ksort($rows);
  }
  catch (Exception $e) {
    apachesolr_log_exception($env_id, $e);
  }

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
function apachesolr_search_mlt_save_block($block_settings = array(), $delta = NULL) {
  $blocks = variable_get('apachesolr_search_mlt_blocks', array());
  if (is_null($delta)) {
    $count = 0;
    ksort($blocks);
    // Construct a new array key.
    if (end($blocks)) {
      list(, $count) = explode('-', key($blocks));
    }
    $delta = sprintf('mlt-%03d', 1 + $count);
  }
  $defaults = apachesolr_search_mlt_block_defaults();
  // Remove stray form values.
  $blocks[$delta] = array_intersect_key($block_settings, $defaults) + $defaults;
  // Eliminate non-selected fields.
  $blocks[$delta]['mlt_fl'] = array_filter($blocks[$delta]['mlt_fl']);
  $blocks[$delta]['delta'] = $delta;
  $blocks[$delta]['mlt_type_filters'] = array_filter($blocks[$delta]['mlt_type_filters']);
  $blocks[$delta]['mlt_custom_filters'] = trim($blocks[$delta]['mlt_custom_filters']);
  variable_set('apachesolr_search_mlt_blocks', $blocks);
}

function apachesolr_search_mlt_delete_block_form($form, &$form_state, $block) {
  if ($block) {
    // Backwards compatibility for the block deltas
    if (isset($block['delta'])) {
      $delta = $block['delta'];
    }
    else {
      $delta = arg(6);
    }
    // Add our delta to the delete form
    $form['delta'] = array(
      '#type' => 'value',
      '#value' => $delta,
    );
    $question = t('Are you sure you want to delete the "More Like this" block %name?', array('%name' => $block['name']));
    $path = 'admin/structure/block';
    $description = t('The block will be deleted. This action cannot be undone.');
    $yes = t('Delete');
    $no = t('Cancel');
    return confirm_form($form, filter_xss($question), $path, $description, $yes, $no);
  }
}

function apachesolr_search_mlt_delete_block_form_submit($form, &$form_state) {
  $blocks = apachesolr_search_load_all_mlt_blocks();

  unset($blocks[$form_state['values']['delta']]);
  variable_set('apachesolr_search_mlt_blocks', $blocks);
  drupal_set_message(t('The block has been deleted.'));
  $form_state['redirect'] = 'admin/config/search/apachesolr/search-pages';
}

/**
 * Form to edit moreLikeThis block settings.
 *
 * @param int $delta If editing, the id of the block to edit.
 *
 * @return array The form used for editing.
 * @todo Add term boost settings.
 * @todo Enable the user to specify a query, rather then forcing suggestions
 *  based on the node id.
 */
function apachesolr_search_mlt_block_form($block_id = NULL) {
  if (!empty($block_id)) {
    $block = apachesolr_search_mlt_block_load($block_id);
    if (!$block) {
      return array();
    }
  }
  else {
    $block = apachesolr_search_mlt_block_defaults();
  }

  $form['delta'] = array(
    '#type' => 'value',
    '#default_value' => isset($block['delta']) ? $block['delta'] : '',
    '#weight' => '-2',
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Block name'),
    '#description' => t('The block name displayed to site users.'),
    '#required' => TRUE,
    '#default_value' => isset($block['name']) ? $block['name'] : '',
    '#weight' => '-2',
  );

  $environments = apachesolr_load_all_environments();
  $options = array('' => t('<Disabled>'));
  foreach ($environments as $id => $environment) {
    $options[$id] = $environment['name'];
  }
  $form['mlt_env_id'] = array(
    '#title' => t('Search environment'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($block['mlt_env_id']) ? $block['mlt_env_id'] : apachesolr_default_environment(),
  );

  $form['num_results'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of related items to display'),
    '#default_value' => isset($block['num_results']) ? $block['num_results'] : '',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)),
    '#weight' => -1,

    );
  $form['mlt_fl'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields for finding related content'),
    '#description' => t('Choose the fields to be used in calculating similarity. The default combination of %taxonomy_names and %title will provide relevant results for typical sites.', array("%taxonomy_names" => apachesolr_field_name_map("taxonomy_names"), "%title" => apachesolr_field_name_map("label"))),
    '#options' => apachesolr_search_mlt_get_fields(),
    '#required' => TRUE,
    '#default_value' => isset($block['mlt_fl']) ? $block['mlt_fl'] : '',
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
    '#default_value' => isset($block['mlt_mintf']) ? $block['mlt_mintf'] : '',
    '#options' => $options,
  );
  $form['advanced']['mlt_mindf'] = array(
    '#type' => 'select',
    '#title' => t('Minimum document frequency'),
    '#description' => t('A word must occur in at least this many documents before it will be used for similarity comparison.'),
    '#default_value' => isset($block['mlt_mindf']) ? $block['mlt_mindf'] : '',
    '#options' => $options,
  );
  $form['advanced']['mlt_minwl'] = array(
    '#type' => 'select',
    '#title' => t('Minimum word length'),
    '#description' => 'You can use this to eliminate short words such as "the" and "it" from similarity comparisons. Words must be at least this number of characters or they will be ignored.',
    '#default_value' => isset($block['mlt_minwl']) ? $block['mlt_minwl'] : '',
    '#options' => $options,
  );
  $form['advanced']['mlt_maxwl'] = array(
    '#type' => 'select',
    '#title' => t('Maximum word length'),
    '#description' => t('You can use this to eliminate very long words from similarity comparisons. Words of more than this number of characters will be ignored.'),
    '#default_value' => isset($block['mlt_maxwl']) ? $block['mlt_maxwl'] : '',
    '#options' => drupal_map_assoc(array(8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)),
  );
  $form['advanced']['mlt_maxqt'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of query terms'),
    '#description' => t('The maximum number of query terms that will be included in any query. Lower numbers will result in fewer recommendations but will get results faster. If a content recommendation is not returning any recommendations, you can either check more "Comparison fields" checkboxes or increase the maximum number of query terms here.'),
    '#options' => drupal_map_assoc(array(3, 5, 7, 10, 12, 15, 20, 25, 30, 35, 40)),
    '#default_value' => isset($block['mlt_maxqt']) ? $block['mlt_maxqt'] : '',
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
    '#default_value' => is_array($block['mlt_type_filters']) ? $block['mlt_type_filters'] : array(),
    '#options' => $type_options,
    '#description' => t('Select the content types that similarity suggestions should be restricted to. Multiple types are joined with an OR query, so selecting more types results in more recommendations. If none are selected, no filter will be applied.'),
    '#weight' => '-2',
  );

  $form['restrictions']['mlt_custom_filters'] = array(
    '#type' => 'textfield',
    '#title' => t('Additional Query'),
    '#description' => t("A query, in Lucene syntax, which will further filter the similarity suggestions. For example, 'label:strategy' will filter related content further to only those with strategy in the title. Here are some more examples:") .
                        '<ul>
                            <li>ss_language:fr</li>
                            <li>tid:(5 OR 7)</li>
                            <li>ds_created:[2009-05-01T23:59:59Z TO 2009-07-28T12:30:00Z]</li>
                            <li>-is_uid:0, -is_uid:1</li>
                        </ul>',
    '#required' => FALSE,
    '#default_value' => isset($block['mlt_custom_filters']) ? $block['mlt_custom_filters'] : '',
    '#weight' => '-1',
  );

  return $form;
}
