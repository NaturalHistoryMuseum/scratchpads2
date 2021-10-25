<?php

/**
 * @file
 * Link admin settings.
 */

/**
 * Form constructor for the settings form.
 *
 * @ingroup forms
 */
function link_admin_settings() {
  $form['link_default_protocol'] = array(
    '#type' => 'select',
    '#title' => t('Protocol'),
    '#options' => array(
      LINK_HTTP_PROTOCOL => strtoupper(LINK_HTTP_PROTOCOL),
      LINK_HTTPS_PROTOCOL => strtoupper(LINK_HTTPS_PROTOCOL),
    ),
    '#default_value' => variable_get('link_default_protocol', LINK_HTTP_PROTOCOL),
    '#description' => t('Select the default protocol to use for each link.'),
  );

  return system_settings_form($form);
}
