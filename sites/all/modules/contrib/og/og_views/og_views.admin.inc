<?php


/**
 * @file
 * Admin settings for the Group views module.
 */

function og_views_settings() {
  $form = array();

  $form['og_views_global_view'] = array(
    '#type' => 'select',
    '#title' => t('Group views default'),
    '#options' => og_views_field_allowed_values(FALSE),
    '#default_value' => variable_get('og_views_global_view', 'og_nodes'),
  );

  return system_settings_form($form);
}