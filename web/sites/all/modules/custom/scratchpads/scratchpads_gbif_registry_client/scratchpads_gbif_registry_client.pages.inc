<?php

/**
 * @file
 */

module_load_include('inc', 'dwcarchiver', 'dwcarchiver.rebuild');

/**
 * Admin settings.
 */
function scratchpads_gbif_registry_client_admin_settings() {
  $default_licence = CC_BY;
  $gbif_allowed_licences = [CC_0, CC_BY, CC_BY_NC];
  $licence_options = [];
  foreach ($gbif_allowed_licences as $licence) {
    $licence_options[$licence] = creative_commons_get_licence_types($licence);
  };
  $cc_block_licence_type = variable_get('creative_commons_block_licence_type', CC_BY);
  if (in_array($cc_block_licence_type, $licence_options)) {
    $default_licence = $cc_block_licence_type;
  }
  $form = [];

  $uuid = variable_get('scratchpads_gbif_registry_client_uuid', FALSE);
  if ($uuid) {
    $api = variable_get('gbif_registry_web_service_url', 'http://api.gbif.org/v1/');
    $domain = str_replace('api', 'www', parse_url($api, PHP_URL_HOST));
    $url = "http://$domain/dataset/$uuid";
    $form['scratchpads_gbif_registry_client_uuid'] = [
      '#type' => 'markup',
      '#markup' => '<p>' . l(t('Registered on GBIF with UUID @uuid', ['@uuid' => $uuid]), $url) . '</p>',
    ];
  }

  $nonBio = !variable_get('biological_vids');

  $default_description = implode(' | ', array_filter([
    variable_get('site_slogan', ''),
    strip_tags(variable_get('front_page_welcome_message', ['value' => ''])['value']),
  ]));

  $form += [
    'scratchpads_gbif_registry_client_registration_enabled' => [
      '#type' => 'checkbox',
      '#title' => t('Enable GBIF registration'),
      '#default_value' => variable_get('scratchpads_gbif_registry_client_registration_enabled', FALSE),
      '#description' => t("Whether to register this scratchpad with GBIF."),
      '#disabled' => $nonBio,
    ],
    'scratchpads_gbif_registry_client_description' => [
      '#type' => 'textarea',
      '#title' => t('Dataset description'),
      '#default_value' => variable_get('scratchpads_gbif_registry_client_description', $default_description),
      '#description' => t('A summary of the dataset. This will appear on the GBIF web page for your dataset and in GBIF search results.'),
    ],
    'scratchpads_gbif_registry_client_registration_licence' => [
      '#type' => 'select',
      '#options' => $licence_options,
      '#title' => t('GBIF licence'),
      '#default_value' => variable_get('scratchpads_gbif_registry_client_registration_licence', $default_licence),
      '#description' => t("The licence to use for the DwC-A archive"),
    ],
  ];

  if ($nonBio) {
    $link = url('admin/structure/taxonomy/add');
    $text = t(
      'There are no biological taxonomies on this scratchpad, please <a href="@url">create one</a> before registering with GBIF.',
      [
        "@url" => $link,
      ]
    );
    $form['scratchpads_gbif_registry_client_registration_enabled']['#description'] .= "<p><b>" . $text . "</b></p>";
  }

  $form['#submit'][] = 'scratchpads_gbif_registry_client_admin_settings_form_submit';
  $form['#validate'][] = 'scratchpads_gbif_registry_client_admin_settings_form_validate';

  // Use the system_settings_form function so that we don't have to implement our own submit function.
  return system_settings_form($form);
}

/**
 * Validation handler for settings form.
 */
function scratchpads_gbif_registry_client_admin_settings_form_validate($form, &$form_state) {
  $enabled = $form_state['values']['scratchpads_gbif_registry_client_registration_enabled'];
  $description = $form_state['values']['scratchpads_gbif_registry_client_description'];

  if ($enabled && !$description) {
    form_set_error('scratchpads_gbif_registry_client_description', t('You must proide a description to submit your dataset to GBIF.'));
  }
}

/**
 * Submit handler for the scratchpads_gbif_registry_client_admin_settings form.
 */
function scratchpads_gbif_registry_client_admin_settings_form_submit($form, &$form_state) {
  variable_del('scratchpads_gbif_registry_client_last_update');

  $new_desc = $form_state['values']['scratchpads_gbif_registry_client_description'];

  if($form['scratchpads_gbif_registry_client_description']['#default_value'] !== $new_desc) {
    $uuid = variable_get('scratchpads_gbif_registry_client_uuid', false);
    if($uuid) {
      // GBIFRegistry::singleton();
      // $dataset = new GBIF\Dataset($uuid);
      // $dataset->setDescription($new_desc);
      // $dataset->save();
    }
  }
  if(module_exists('dwcarchiver')) {
    dwcarchiver_rebuild('gbif-dwca', false);
    menu_rebuild();
  }
}
