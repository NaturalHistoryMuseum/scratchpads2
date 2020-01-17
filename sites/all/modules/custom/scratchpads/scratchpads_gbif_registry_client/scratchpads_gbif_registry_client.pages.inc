<?php

module_load_include('inc', 'dwcarchiver', 'dwcarchiver.rebuild');

/**
 * Admin settings.
 */
function scratchpads_gbif_registry_client_admin_settings()
{
  $default_licence = CC_BY;
  $gbif_allowed_licences = array(CC_0, CC_BY, CC_BY_NC);
  $licence_options = [];
  foreach ($gbif_allowed_licences as $licence) {
    $licence_options[$licence] = creative_commons_get_licence_types($licence);
  };
  $cc_block_licence_type = variable_get('creative_commons_block_licence_type', CC_BY);
  if (in_array($cc_block_licence_type, $licence_options)) {
    $default_licence = $cc_block_licence_type;
  }
  $form = [];

  $uuid = variable_get('scratchpads_gbif_registry_client_uuid', false);
  if($uuid) {
    $api = variable_get('gbif_registry_web_service_url', 'http://api.gbif.org/v1/');
    $domain = str_replace('api', 'www', parse_url($api, PHP_URL_HOST));
    $url = "http://$domain/dataset/$uuid";
    $form['scratchpads_gbif_registry_client_uuid'] = [
      '#type' => 'markup',
      '#markup' => '<p>' . l(t('Registered on GBIF with UUID @uuid', ['@uuid' => $uuid]), $url) . '</p>'
    ];
  }

  $form += array(
    'scratchpads_gbif_registry_client_registration_enabled' => array(
      '#type' => 'checkbox',
      '#title' => t('Enable GBIF registration'),
      '#default_value' => variable_get('scratchpads_gbif_registry_client_registration_enabled', false),
      '#description' => t("Whether to register this scratchpad with GBIF"),
    ),
    'scratchpads_gbif_registry_client_registration_licence' => array(
      '#type' => 'select',
      '#options' => $licence_options,
      '#title' => t('GBIF licence'),
      '#default_value' => variable_get('scratchpads_gbif_registry_client_registration_licence', $default_licence),
      '#description' => t("The licence to use for the DwC-A archive"),
    )
  );

  $form['#submit'][] = 'scratchpads_gbif_registry_client_admin_settings_form_submit';

  // use the system_settings_form function so that we don't have to implement our own submit function
  return system_settings_form($form);
}

/**
 * Submit handler for the scratchpads_gbif_registry_client_admin_settings form.
 */
function scratchpads_gbif_registry_client_admin_settings_form_submit($form, &$form_state)
{
  variable_del('scratchpads_gbif_registry_client_last_update');

  if(module_exists('dwcarchiver')) {
    dwcarchiver_rebuild('gbif-dwca', false);
    menu_rebuild();
  }
}
