<?php

/**
 * @file
 * Export UI display customizations.
 */

/**
 * CTools export UI extending class. Slightly customized based on Context module.
 */
class arc2_store_export_ui extends ctools_export_ui {

  /**
   * Overrides ctools_export_ui::list_form().
   *
   * Simplifies the form similar to how the Context module does it.
   */
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    $form['top row']['submit'] = $form['bottom row']['submit'];
    $form['top row']['reset'] = $form['bottom row']['reset'];
    $form['bottom row']['#access'] = FALSE;
    return;
  }

  /**
   * Overrides ctools_export_ui::edit_save_form().
   *
   * Clear menu cache in case the SPARQL endpoint path was modified.
   */
  function edit_save_form($form_state) {
    parent::edit_save_form($form_state);
    if (!empty($form_state['plugin']['schema']) && $form_state['plugin']['schema'] == 'arc2_store_settings') {
      menu_rebuild();
    }
  }
}

/**
 * Define the preset add/edit form.
 */
function arc2_store_export_ui_form(&$form, &$form_state) {
  $item = &$form_state['item'];
  if (empty($item->settings)) {
    $item->settings = arc2_store_get_defaults();
  }

  // We build our form ourselves so we can implement a proper on the fly machine name.
  unset($form['info']);

  $form['label'] = array(
    '#title' => t('Store name'),
    '#type' => 'textfield',
    '#default_value' => $item->label,
    '#description' => t('The human-readable name of the ARC2 store.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#size' => 30,
  );

  $form['store_id'] = array(
    '#type' => 'machine_name',
    '#default_value' => $item->store_id,
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'arc2_store_exists',
      'source' => array('label'),
    ),
    '#disabled' => ('clone' != $form_state['form type'] && !empty($item->name)),
    '#description' => t('The machine readable name of the ARC2 store. This value can only contain letters, numbers, and underscores.'),
  );

  $form['#tree'] = TRUE;

  // Endpoint - read settings.
  $form['settings']['endpoint_enabled'] = array(
    '#title' => t('Enable SPARQL Endpoint'),
    '#type' => 'checkbox',
    '#default_value' => $item->settings['endpoint_enabled'],
    '#description' => t('Check this if you want to enable a SPARQL endpoint on this store.'),
  );
  $form['settings']['endpoint_path'] = array(
    '#title' => t('SPARQL endpoint path'),
    '#type' => 'textfield',
    '#default_value' => $item->settings['endpoint_path'],
    '#description' => t('Path where the SPARQL endpoint will be available to end users.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['endpoint_read_features'] = array(
    '#title' => t('SPARQL endpoint features'),
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array('select', 'construct', 'ask', 'describe', 'dump')),
    '#default_value' => $item->settings['endpoint_read_features'],
    '#description' => t('Operations allowed to be performed at the SPARQL endpoint. dump is an ARC2 specific command for streaming SPOG export.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['endpoint_max_limit'] = array(
    '#title' => t('SPARQL endpoint maximum results limit'),
    '#type' => 'textfield',
    '#default_value' => $item->settings['endpoint_max_limit'],
    '#description' => t('Maximum number of results returned by the SPARQL endpoint if no limit is specified in the query.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['endpoint_timeout'] = array(
    '#title' => t('SPARQL endpoint timeout'),
    '#type' => 'textfield',
    '#default_value' => $item->settings['endpoint_timeout'],
    '#description' => t('Timeout of the endpoint in seconds.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['endpoint_read_key'] = array(
    '#title' => t('SPARQL endpoint read API key'),
    '#type' => 'textfield',
    '#default_value' => $item->settings['endpoint_read_key'],
    '#description' => t('API key for read operations (leave empty if this is a public endpoint). Randomly generated API key example: ' . sha1(rand())),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Endpoint - write settings.
  $form['settings']['endpoint_write_enabled'] = array(
    '#title' => t('Enable write operations for this SPARQL Endpoint'),
    '#type' => 'checkbox',
    '#default_value' => $item->settings['endpoint_write_enabled'],
    '#description' => t('Check this only if you want to allow end users to write data via the SPARQL endpoint.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['endpoint_write_key'] = array(
    '#title' => t('SPARQL endpoint write API key (recommended)'),
    '#type' => 'textfield',
    '#default_value' => $item->settings['endpoint_write_key'],
    '#description' => t('API key for write operations.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_write_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['settings']['endpoint_write_features'] = array(
    '#title' => t('SPARQL endpoint write operations'),
    '#type' => 'checkboxes',
    '#options' => drupal_map_assoc(array('load', 'insert', 'delete')),
    '#default_value' => $item->settings['endpoint_write_features'],
    '#description' => t('Write operations allowed to be performed via the SPARQL endpoint.'),
    '#states' => array(
      'visible' => array(
        ':input[name="settings[endpoint_write_enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );
}

/**
 * Tests if the store name already exists.
 *
 * @name
 *   Machine name of the arc2_store to check.
 *
 * @return
 *   A boolean flagging whether the item exists.
 */
function arc2_store_exists($name) {
  $configs = ctools_export_crud_load_all('arc2_store_settings');
  return isset($configs[$name]);
}

/**
 * Returns default settings.
 */
function arc2_store_get_defaults() {
  return array(
    'endpoint_enabled' => TRUE,
    'endpoint_path' => '',
    'endpoint_read_features' => drupal_map_assoc(array('select', 'construct', 'ask', 'describe')),
    'endpoint_max_limit' => 500,
    'endpoint_timeout' => 60,
    'endpoint_read_key' => '',
    'endpoint_write_enabled' => FALSE,
    'endpoint_write_key' => sha1(rand()),
    'endpoint_write_features' => array(),
  );
}
