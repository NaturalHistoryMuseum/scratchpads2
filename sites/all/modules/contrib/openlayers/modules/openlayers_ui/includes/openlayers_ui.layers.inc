<?php

/**
 * @file
 * This file holds the functions handling layers in the
 * Openlayers UI.
 *
 * @ingroup openlayers
 */

/**
 * Layer add/edit form.
 */
function openlayers_ui_layers_types($form_state, $layer = NULL, $edit = FALSE) {
  $form = array();
  $form['layer_types'] = openlayers_ui_get_layer_type_options('map');
  return $form;
}


/**
 * Layer add/edit form.
 */
function openlayers_ui_layers_form($form_state, $layer = NULL) {
  $form = array();

  $form['layer'] = openlayers_ui_get_layer_form($layer);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for layers.
 */
function openlayers_ui_layers_form_submit(&$form, &$form_state) {
  // Save layer
  $layer_class = $form_state['values']['data']['layer_type'];
  $layer = openlayers_layer_type_load($layer_class);

  $layer->name = $form_state['values']['name'];
  $layer->title = $form_state['values']['title'];
  $layer->description = $form_state['values']['description'];
  $layer->data = $form_state['values']['data'];

  if (isset($layer->data['resolutions'])) {
    // resolutions are stored as strings because floats are not handled
    // well by formapi
    $layer->data['resolutions'] =
      array_map("floatval", array_keys($layer->data['resolutions']));
    rsort($layer->data['resolutions']);
  }

  $success = $layer->save();

  // Redirect to edit page
  if ($success) {
    drupal_set_message(t('Layer saved.'));
    $form_state['redirect'] = 'admin/structure/openlayers/layers/' . $layer->name . '/edit';
  }
  else {
    form_set_error('openlayers', t('Error trying to save layer.'));
  }
}

/**
 * Layer-wide-settings form
 */
function openlayers_ui_layers_settings($form_state, $layer = NULL) {
  $form = array();
  $layer_types = openlayers_layer_types();

  foreach ($layer_types as $name => $type_def) {
    $type = openlayers_layer_type_load($name);
    if (method_exists($type, 'settings_form')) {
      $form[$name] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#title' => $type_def['title']);
      $form[$name]['settings'] = $type->settings_form();
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for openlayers_ui_layers_settings
 */
function openlayers_ui_layers_settings_submit(&$form, &$form_state) {
  $layer_types = openlayers_layer_types();
  foreach ($form_state['values'] as $key => $value) {
    if (in_array($key, array_keys($layer_types))) {
      foreach ($value['settings'] as $k => $v) {
        variable_set($k, $v);
      }
    }
  }
}

/**
 * Get an actual layer form
 */
function openlayers_ui_get_layer_form($args, $layer = FALSE) {
  $form = array();

  // TODO: this shouldn't be necessary. this is ugly code.
  $type = $args['build_info']['args'][0];

  // @TODO: clarify use of data, and of layer_type in overriding it
  $form['data'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => '',
    '#description' => '',
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($type->name) ? $type->name : '',
    '#title' => t('Layer Name'),
    '#description' => t('This is the unique name of the layer. It must contain only alphanumeric characters and underscores.'),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Layer Title'),
    '#default_value' => isset($type->title) ? $type->title : '',
    '#description' => t('The friendly name of your layer, which will appear in the administration interface.'),
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Layer Description'),
    '#default_value' => isset($type->description) ? $type->description : '',
  );

  // Arrays merged in order so that options_form can override defaults
  $form['data'] = $type->options_form() + $form['data'];

  return $form;
}

/**
 * Get layer type options.
 */
function openlayers_ui_get_layer_type_options($type) {
  $form = array();
  $form['layer_type_help'] = array(
    '#type' => 'item',
    '#markup' => t('Select a layer type to create a new layer.')
  );
  foreach (openlayers_layer_types() as $key => $plugin) {
    $class = ctools_plugin_get_class($plugin, 'layer_type');
    if ($class) {
      // Build form.
      $form[$key] = array(
        '#type' => 'item',
        '#title' => l($plugin['title'], "admin/structure/openlayers/layers/add/{$key}"),
        '#value' =>  $plugin['description'],
      );
    }
  }
  return $form;
}

/**
 * Import a layer from cut & paste
 */
function openlayers_ui_layers_import_form($form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Layers name'),
    '#description' => t('Enter the name to use for this layer if it is different from the source layer. Leave blank to use the name of the layer.'),
  );

  $form['layer'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste layer code here'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#submit' =>   array('openlayers_ui_layers_import_submit'),
    '#validate' => array('openlayers_ui_layers_import_validate'),
  );
  return $form;
}

/**
 * Validate handler to import a layer
 */
function openlayers_ui_layers_import_validate($form, &$form_state) {

  $items = '';
  ob_start();
  eval($form_state['values']['layer']);
  ob_end_clean();

  if (!is_array($items)) {
    return form_error($form['layer'], t('Unable to interpret layer code.'));
  }

  // View name must be alphanumeric or underscores, no other punctuation.
  if (!empty($form_state['values']['name']) &&
    preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['name'])) {
      form_error($form['name'],
        t('Layer name must be alphanumeric or underscores only.'));
  }

  $layer = current($items);
  $layer_type_object = openlayers_layer_type_load($layer->data['layer_type']);

  if ($form_state['values']['name']) {
    $layer->name = $form_state['values']['name'];
  }

  if ($exists = openlayers_layer_load($layer->name)) {
    drupal_set_message(t('An existing layer of this name already exists'), 'error');
    $form_state['redirect'] = 'admin/structure/openlayers/layers';
  }
  else {
    drupal_set_title(t('Add Layer'));
    $layer_object = new $layer->data['layer_type']($layer);
    $output = drupal_get_form('openlayers_ui_layers_form',
      $layer_object);
    print theme('page', $output);
    exit;
  }
}
