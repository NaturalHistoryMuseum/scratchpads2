<?php

/**
 * @file
 *  This is the GlobalRedirect admin include which provides an interface to global redirect to change some of the default settings
 */

/**
 *  Function to generate the form setting array
 */
function globalredirect_settings() {
  $settings = _globalredirect_get_settings();

  $form['settings'] = array(
    '#tree' => TRUE,
  );
  $form['settings']['deslash'] = array(
    '#type' => 'checkbox',
    '#title' => t('Deslash'),
    '#description' => t('If enabled, this option will remove the trailing slash from requests. This stops requests such as <code>example.com/node/1/</code> failing to match the corresponding alias and can cause duplicate content. On the other hand, if you require certain requests to have a trailing slash, this feature can cause problems so may need to be disabled.'),
    '#default_value' => $settings['deslash'],
  );

  $form['settings']['nonclean_to_clean'] = array(
    '#type' => 'checkbox',
    '#title' => t('Non-clean to Clean'),
    '#description' => t('If enabled, this option will redirect from non-clean to clean URL (if Clean URL\'s are enabled). This will stop, for example, node 1  existing on both <code>example.com/node/1</code> AND <code>example.com?q=node/1</code>.'),
    '#default_value' => $settings['nonclean_to_clean'],
  );

  $form['settings']['trailing_zero'] = array(
    '#type' => 'radios',
    '#title' => t('Remove Trailing Zero Argument'),
    '#description' => t('If enabled, any instance of "/0" will be trimmed from the right of the URL. This stops duplicate pages such as "taxonomy/term/1" and "taxonomy/term/1/0" where 0 is the default depth. There is an option of limiting this feature to taxonomy term pages ONLY or allowing it to effect any page. <strong>By default this feature is disabled to avoid any unexpected behavior. Also of note, the trailing /0 "depth modifier" was removed from Drupal 7.</strong>'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled for all pages'),
      2 => t('Enabled for taxonomy term pages only'),
    ),
    '#default_value' => $settings['trailing_zero'],
  );

  $form['settings']['menu_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Menu Access Checking'),
    '#description' => t('If enabled, the module will check the user has access to the page before redirecting. This helps to stop redirection on protected pages and avoids giving away <em>secret</em> URL\'s. <strong>By default this feature is disabled to avoid any unexpected behavior</strong>'),
    '#default_value' => $settings['menu_check'],
  );

  $form['settings']['case_sensitive_urls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Case Sensitive URL Checking'),
    '#description' => t('If enabled, the module will compare the current URL to the alias stored in the system. If there are any differences in case then the user will be redirected to the correct URL.'),
    '#default_value' => $settings['case_sensitive_urls'],
  );


  $form['settings']['language_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Language Path Checking'),
    '#description' => t('If enabled, the module will check that the page being viewed matches the language in the URL or the system default. For example, viewing a French node while the site is in English will cause a redirect to the English node.'),
    '#default_value' => $settings['language_redirect'],
  );


  $form['settings']['canonical'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Canonical Link'),
    '#description' => t('If enabled, will add a <a href="!canonical">canonical link</a> to each page.', array('!canonical' => 'http://googlewebmastercentral.blogspot.com/2009/02/specify-your-canonical.html')),
    '#default_value' => $settings['canonical'],
  );


  $form['settings']['content_location_header'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set Content Location Header'),
    '#description' => t('If enabled, will add a <a href="!canonical">Content-Location</a> header.', array('!canonical' => 'http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.14')),
    '#default_value' => $settings['content_location_header'],
  );


  $form['settings']['term_path_handler'] = array(
    '#type' => 'checkbox',
    '#title' => t('Taxonomy Term Path Handler'),
    '#description' => t('If enabled, any request to a taxonomy/term/[tid] page will check that the correct path is being used for the term\'s vocabulary.'),
    '#default_value' => $settings['term_path_handler'],
  );

  $form['settings']['frontpage_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Frontpage Redirect Handler'),
    '#description' => t('If enabled, any request to the frontpage path will redirect to the site root.<br />
                         Whatever you set as the path of the front page on the !link settings page will redirect to the site root (e.g. "node" or "node/1" and also its alias (e.g. in case you have set "node/1" as your home page but that page also has an alias "home")).', array(
      '!link' => l(t('Site Information'), 'admin/settings/site-information'),
    )),
    '#default_value' => $settings['frontpage_redirect'],
  );

  $form['settings']['ignore_admin_path'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore Admin Path'),
    '#description' => t('If enabled, any request to the admin section of the site will be ignored by Global Redirect.<br />
                         This is useful if you are experiencing problems with Global Redirect and want to protect the admin section of your website. NOTE: This may not be desirable if you are using path aliases for certain admin URLs.'),
    '#default_value' => $settings['ignore_admin_path'],
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('globalredirect_settings_submit_save'),
    '#value' => t('Save Configuration'),
  );

  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#submit' => array('globalredirect_settings_submit_reset'),
    '#value' => t('Reset to defaults'),
  );

  return $form;
}


/**
 * Save submit handler for the globalredirect_settings form.
 * Compares the submitted settings to the defaults and unsets any that are equal. This was we only store overrides.
 */
function globalredirect_settings_submit_save($form_id, &$form_state) {
  // Grab the defaults
  $defaults = _globalredirect_get_settings(TRUE);

  // Copy out the settings
  $settings = $form_state['values']['settings'];

  // Compare each setting to the default. If equal, remove. If not, cast to an int (FormAPI converts keys to string).
  foreach ($settings as $key => $value) {
    if ($value == $defaults[$key]) {
      unset($settings[$key]);
    }
    else {
      $settings[$key] = (int)$value;
    }
  }

  // If we've ended up with an empty settings array, delete the settings variable...
  if (empty($settings)) {
    variable_del('globalredirect_settings');
  }
  // ... otherwise store the settings
  else {
    variable_set('globalredirect_settings', $settings);
  }
  drupal_set_message(t('Globalredirect settings have been saved.'));
}


/**
 * Reset submit handler for the globalredirect_settings form.
 * This simply deletes any overridden settings which forces the system to fallback to defaults
 */
function globalredirect_settings_submit_reset($form_id, &$form_state) {
  variable_del('globalredirect_settings');
  drupal_set_message(t('Globalredirect settings have been reset to default.'));
}
