<?php

/**
 * @file
 * This file holds the functions for the main openlayers Admin settings.
 *
 * @ingroup openlayers
 */

/**
 * Menu callback; displays the openlayers module settings page.
 *
 * @see system_settings_form()
 */
function openlayers_ui_admin_settings() {
  $source_description = '<p>'
    . t('By default the your site will point to a hosted version of
    the OpenLayers library, at <code>@ol_api_url.</code>',
    array('@ol_api_url' => 'http://openlayers.org/api/2.9/OpenLayers.js'))
    . t('Alternately, you can <a href="!ol_url">download OpenLayers</a>,
    add it to your Drupal installation, and use a relative path like
    <code>@suggested_path</code>',
      array(
        '!ol_url' => 'http://openlayers.org/',
        '@suggested_path' => drupal_get_path('module', 'openlayers') . '/OpenLayers-2.7/OpenLayers.js',
      )
    )
    . '</li></ul>';

  // Define Form
  $form['openlayers_source'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenLayers Source'),
    '#description' => $source_description,
    '#default_value' => variable_get('openlayers_source', 'http://openlayers.org/api/2.9/OpenLayers.js'),
  );

  $form['openlayers_default_preset'] = array(
    '#type' => 'select',
    '#title' => t('OpenLayers Default Preset'),
    '#description' => t('This is the default preset that will be used
      in cases where one is not defined.  It will also be used as the default when creating new presets.'),
    '#options' => openlayers_preset_options(),
    '#default_value' => variable_get('openlayers_default_preset', 'default'),
  );

  // Make a system setting form and return
  return system_settings_form($form);
}
