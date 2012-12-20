<?php

/**
 * @file
 * Administration page callbacks for the fbconnect module.
 */

function fbconnect_api_keys_settings($form, &$form_state) {
  $form['fbconnect_appid'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Application ID'),
    '#default_value' => variable_get('fbconnect_appid', NULL),
    '#description' => t('Also called the <em>OAuth client_id</em> value on Facebook App settings pages. <a href="https://www.facebook.com/developers/createapp.php">Facebook Apps must first be created</a> before they can be added here.'),
  );

  $form['fbconnect_skey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Application Secret'),
    '#default_value' => variable_get('fbconnect_skey', NULL),
    '#description' => t('Also called the <em>OAuth client_secret</em> value on Facebook App settings pages.'),
  );

  $form['fbconnect_language_code'] = array(
    '#type' => 'textfield',
    '#title' => (module_exists('i18n')) ? t('Default language code') : t('Language code'),
    '#description' => t('Enter your country code here to get translated versions of Facebook connect. (e.g. en_US or de_DE)'),
    '#default_value' => variable_get('fbconnect_language_code', 'en_US'),
  );

  if (module_exists('i18n')) {
    $form['language_codes'] = array(
      '#type'          => 'fieldset',
      '#title'         => t('Automatically Switch Language Code'),
      '#description'   => t('Enable the fbconnect module to use a different language code for each of your site languages.'),
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );

    foreach (language_list() as $langcode => $language) {
      $form['language_codes']['fbconnect_language_code_' . $langcode] = array(
        '#type'          => 'select',
        '#title'         => t('@name (@native) Language code', array('@name' => $language->name, '@native' => $language->native)),
        '#default_value' => variable_get('fbconnect_language_code_' . $langcode, ''),
        '#options'       => array('' => t('- Use default -')) + _fbconnect_language_codes(),
      );
    }
  }

  $form['fbconnect_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#default_value' => variable_get('fbconnect_debug', FALSE),
  );
  $form['fbconnect_ssl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always use SSL on authentication'),
    '#description' => t('Enable this if you want to use SSL on authentication.'),
    '#default_value' => variable_get('fbconnect_ssl', FALSE),
  );

  $form['fbconnect_connect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Connect url'),
    '#description' => t('Copy this value into Facebook Applications on Connect settings tab'),
    '#default_value' => variable_get('fbconnect_connect_url', $GLOBALS['base_url'] . '/'),
  );

  $form['fbconnect_noroot'] = array(
    '#title' => t("Don't include settings in footer"),
    '#type' => 'checkbox',
    '#description' => t('Check if other Facebook modules are in use so fb-root code is not added twice.'),
    '#default_value' => variable_get('fbconnect_noroot', '')
  );

  //  locale
  //  connect_logo_url
  //  about_url
  //  base_domain
  //  tos_url

  return system_settings_form($form);
}

function fbconnect_appearance_settings($form, &$form_state) {
  $form['site'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for !site', array('!site' => variable_get('site_name', t('Your Site')))),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['site']['fbconnect_fast_reg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fast registration mode'),
    '#default_value' => variable_get('fbconnect_fast_reg', NULL),
    '#description' => t("Enable this mode will bypass Drupal's standard registration form."),
  );

  $form['site']['fbconnect_fast_reg_autoname'] = array(
    '#type' => 'checkbox',
    '#title' => t('Assign new random name on name conflict during Fast Registration'),
    '#default_value' => variable_get('fbconnect_fast_reg_autoname', NULL),
    '#description' => t("Creates a new random name for the user if the user's Facebook is already taken by another user on this site. The random name will look something like fbuser123."),
  );

  $form['site']['fbconnect_loginout_mode'] = array(
    '#type' => 'select',
    '#title' => t('Login/logout mode'),
    '#options' => array(
      'manual' => t('Manual'),
      'auto'   => t('Auto login/logout'),
    ),
    '#default_value' => variable_get('fbconnect_loginout_mode', FALSE),
    '#description' => t("Login to Drupal/logout from Facebook. Enable this if you don't want to logout automatically from Facebook."),
  );

  // TODO: rename to more clear name
  $form['site']['fbconnect_reg_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable linking accounts during registration'),
    '#default_value' => variable_get('fbconnect_reg_options', 0),
    '#description' => t('Disables the option to link local account to a FB account on registration'),
  );
  $form['site']['fbconnect_user_reg_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display FBConnect button on registration form'),
    '#default_value' => variable_get('fbconnect_user_reg_display', TRUE),
    '#description' => t('Add the FBConnect button to the "create new account" form. This will only appear if "fast registration mode" is not selected.'),
  );
  $form['site']['fbconnect_user_reg_location'] = array(
    '#type'=> 'select',
    '#title' => t('Location of FBConnect button on registration form'),
    '#default_value' => variable_get('fbconnect_user_reg_location', 'top'),
    '#options' => array(
      'top' => t('Top'),
      'bottom' => t('Bottom'),
    ),
  );

  $form['display'] = array(
    '#title' => t('Display Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (fbconnect_get_config()) {
    $button = array(
      'small_short'  => fbconnect_render_button(array('size' => 'small', 'onclick' => 'return false;'), 'register'),
      'medium_short' => fbconnect_render_button(array('size' => 'medium', 'onclick' => 'return false;'), 'register'),
      'large_short'  => fbconnect_render_button(array('size' => 'large', 'onclick' => 'return false;'), 'register'),
    );
    $form['display']['fbconnect_button'] = array(
      '#title' => t('Facebook Button Size'),
      '#type' => 'radios',
      '#default_value' => variable_get('fbconnect_button', 'medium_short'),
      '#options' => $button,
    );
    $form['display']['fbconnect_button_register_text'] = array(
      '#title' => t('Not Logged into Facebook Text'),
      '#type' => 'textfield',
      '#description' => t('The user is not logged into Facebook or this site so we have not info about their status.'),
      '#default_value' => variable_get('fbconnect_button_register_text', 'Connect'),
    );
    $form['display']['fbconnect_button_login_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Logged into Facebook and Registered but not Logged Into This Site'),
      '#description' => t('They are logged into Facebook and have registered but not logged into this site (manual mode only)'),
      '#states' => array(
        'visible' => array(
          ':input[name="fbconnect_loginout_mode"]' => array('value' => t('manual')),
        ),
      ),
      '#default_value' => variable_get('fbconnect_button_login_text', 'Login'),
    );
  }

  $form['display']['fbconnect_pic_allow'] = array(
    '#type' => 'select',
    '#title' => t('Facebook picture settings'),
    '#description' => t('Here you can deside whether you want to allow the use of the Facebook picture - or not.'),
    '#default_value' => variable_get('fbconnect_pic_allow', 'allow'),
    '#options' => array(
      'allow' => t('Let the user choose'),
      'disallow' => t('Do not allow the use of Facebook user picture at your site'),
    ),
  );

  $options_pic_size = array(
    'thumb' => t('Thumb (50px wide)'),
    'small' => t('Small (100px wide)'),
    'normal' => t('Normal (200px wide)'),
    'square' => t('Square (50px by 50px)'),
  );

  $form['display']['fbconnect_pic_size'] = array(
    '#type' => 'select',
    '#title' => t('Default Facebook picture size'),
    '#description' => t('This will set the default user picture size throughout the site. If your user decides to use his Facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size', 'square'),
    '#options' => $options_pic_size,
  );

  $form['display']['fbconnect_pic_size_nodes'] = array(
    '#type' => 'select',
    '#title' => t('Node Facebook picture size'),
    '#description' => t('This will set the picture size when viewing a node post. If your user decides to use his Facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size_nodes', 'square'),
    '#options' => $options_pic_size,
  );
  $form['display']['fbconnect_pic_size_comments'] = array(
    '#type' => 'select',
    '#title' => t('Comments Facebook picture size'),
    '#description' => t('This will set the picture size when viewing a comment post. If your user decides to use his Facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size_comments', 'square'),
    '#options' => $options_pic_size,
  );
  $form['display']['fbconnect_pic_size_profile'] = array(
    '#type' => 'select',
    '#title' => t('Profile Facebook picture size'),
    '#description' => t("This will set the picture size when viewing a user's profile page. If your user decides to use his Facebook picture, choose its size here."),
    '#default_value' => variable_get('fbconnect_pic_size_profile', 'small'),
    '#options' => $options_pic_size,
  );

  $form['additional'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['additional']['fbconnect_exclude_patterns'] = array(
    '#type' => 'textarea',
    '#title' => t('Disable FBConnect on specified pages'),
    '#description' => t('Enter here path patterns accepted by !drupal_match_path function. One per line.', array(
      '!drupal_match_path' => l(
        'drupal_match_path',
        'http://api.drupal.org/api/drupal/includes--path.inc/function/drupal_match_path/6',
        array('attributes' => array('target' => '_blank'))
      ),
    )),
    '#default_value' => variable_get('fbconnect_exclude_patterns', ''),
  );

  return system_settings_form($form);
}


function fbconnect_fbapp_settings($form, &$form_state) {
  if (!facebook_client()) {
    drupal_set_message(t('Ensure that you entered valid api keys.'), 'error');
    drupal_goto('admin/config/people/fbconnect');
  }
  $fbuid = fbconnect_get_fbuid();
  if (!fbconnect_get_fbuid()) {
    $button_attr = array('text' => t('Connect with your Facebook account'));
    $form['fbconnect'] = array(
      '#prefix' => '<div>' . t('You are not connected to the Facebook account that this application is registered to') . '</div>',
      '#markup' => fbconnect_render_button($button_attr),
    );
    return $form;
  }
  $form['fbconnect_base_domaine'] = array(
    '#type' => 'textfield',
    '#title' => t('Base Domain Url'),
    '#description' => t('Use this to enable your Facebook Connect implementation to span multiple subdomains (e.g., using example.com would enable  www.example.com, foo.example.com or bar.example.com,).'),
    '#default_value' => variable_get('fbconnect_base_domaine', NULL),
  );

  $form['fbconnect_connect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Connect Url'),
    '#description' => t("Your site's main URL."),
    '#default_value' => variable_get('fbconnect_connect_url', $GLOBALS['base_url'] . '/'),
  );

  $form['fbconnect_uninstall_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Post-Remove Callback Url'),
    '#description' => t('Facebook pings this URL when a user removes your application.'),
    '#default_value' => variable_get(
      'fbconnect_uninstall_url',
      url('fbconnect/post-remove', array('absolute' => TRUE))
    ),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'fbconnect_fbapp_settings_submit';
  return $form;
}

function fbconnect_fbapp_settings_submit($form, &$form_state) {
  $properties = array(
    'connect_url'   => $form_state['values']['fbconnect_connect_url'],
    'base_domain'   => $form_state['values']['fbconnect_base_domaine'],
    'uninstall_url' => $form_state['values']['fbconnect_uninstall_url'],
  );

  if (!facebook_client()) {
    drupal_set_message(t('Unable to get a Facebook client, check your api key'), 'error');
    return FALSE;
  }
  if (!fbconnect_get_fbuid()) {
    drupal_set_message(t('Unable to autoconfigure your application settings, make sure you are logged into Facebook'), 'warning');
    return FALSE;
  }

  try {
    $res = facebook_client()->api(array(
      'method' => 'admin.setAppProperties',
      'properties' => array_filter($properties),
    ));
  }
  catch (Exception $e) {
    drupal_set_message(t('Exception thrown while using admin_setAppProperties : @code', array('@code' => $e->getMessage())), 'error');
  }
  if (!$res) {
    drupal_set_message(t('Checking your Facebook session, you are not allowed to change this application settings'), 'error');
  }
  else {
    drupal_set_message(t('Your Facebook app settings has been updated'));
  }
}

/**
 * Returns a list of valid Facebook language codes.
 *
 * see: http://wiki.developers.facebook.com/index.php/Facebook_Locales
 */
function _fbconnect_language_codes() {
  return array(
    'af_ZA' => 'Afrikaans',
    'ar_AR' => 'Arabic',
    'ay_BO' => 'Aymara',
    'az_AZ' => 'Azeri',
    'be_BY' => 'Belarusian',
    'bg_BG' => 'Bulgarian',
    'bn_IN' => 'Bengali',
    'bs_BA' => 'Bosnian',
    'ca_ES' => 'Catalan',
    'ck_US' => 'Cherokee',
    'cs_CZ' => 'Czech',
    'cy_GB' => 'Welsh',
    'da_DK' => 'Danish',
    'de_DE' => 'German',
    'el_GR' => 'Greek',
    'en_GB' => 'English (UK)',
    'en_PI' => 'English (Pirate)',
    'en_UD' => 'English (Upside Down)',
    'en_US' => 'English (US)',
    'eo_EO' => 'Esperanto',
    'es_CL' => 'Spanish (Chile)',
    'es_CO' => 'Spanish (Colombia)',
    'es_ES' => 'Spanish (Spain)',
    'es_LA' => 'Spanish',
    'es_MX' => 'Spanish (Mexico)',
    'es_VE' => 'Spanish (Venezuela)',
    'et_EE' => 'Estonian',
    'eu_ES' => 'Basque',
    'fa_IR' => 'Persian',
    'fb_FI' => 'Finnish (test)',
    'fb_LT' => 'Leet Speak',
    'fi_FI' => 'Finnish',
    'fo_FO' => 'Faroese',
    'fr_CA' => 'French (Canada)',
    'fr_FR' => 'French (France)',
    'ga_IE' => 'Irish',
    'gl_ES' => 'Galician',
    'gn_PY' => 'Guaraní',
    'gu_IN' => 'Gujarati',
    'he_IL' => 'Hebrew',
    'hi_IN' => 'Hindi',
    'hr_HR' => 'Croatian',
    'hu_HU' => 'Hungarian',
    'hy_AM' => 'Armenian',
    'id_ID' => 'Indonesian',
    'is_IS' => 'Icelandic',
    'it_IT' => 'Italian',
    'ja_JP' => 'Japanese',
    'jv_ID' => 'Javanese',
    'ka_GE' => 'Georgian',
    'kk_KZ' => 'Kazakh',
    'km_KH' => 'Khmer',
    'kn_IN' => 'Kannada',
    'ko_KR' => 'Korean',
    'ku_TR' => 'Kurdish',
    'la_VA' => 'Latin',
    'li_NL' => 'Limburgish',
    'lt_LT' => 'Lithuanian',
    'lv_LV' => 'Latvian',
    'mg_MG' => 'Malagasy',
    'mk_MK' => 'Macedonian',
    'ml_IN' => 'Malayalam',
    'mn_MN' => 'Mongolian',
    'mr_IN' => 'Marathi',
    'ms_MY' => 'Malay',
    'mt_MT' => 'Maltese',
    'nb_NO' => 'Norwegian (bokmal)',
    'ne_NP' => 'Nepali',
    'nl_BE' => 'Dutch (België)',
    'nl_NL' => 'Dutch',
    'nn_NO' => 'Norwegian (nynorsk)',
    'pa_IN' => 'Punjabi',
    'pl_PL' => 'Polish',
    'ps_AF' => 'Pashto',
    'pt_BR' => 'Portuguese (Brazil)',
    'pt_PT' => 'Portuguese (Portugal)',
    'qu_PE' => 'Quechua',
    'rm_CH' => 'Romansh',
    'ro_RO' => 'Romanian',
    'ru_RU' => 'Russian',
    'sa_IN' => 'Sanskrit',
    'se_NO' => 'Northern Sámi',
    'sk_SK' => 'Slovak',
    'sl_SI' => 'Slovenian',
    'so_SO' => 'Somali',
    'sq_AL' => 'Albanian',
    'sr_RS' => 'Serbian',
    'sv_SE' => 'Swedish',
    'sw_KE' => 'Swahili',
    'sy_SY' => 'Syriac',
    'ta_IN' => 'Tamil',
    'te_IN' => 'Telugu',
    'tg_TJ' => 'Tajik',
    'th_TH' => 'Thai',
    'tl_PH' => 'Filipino',
    'tl_ST' => 'Klingon',
    'tr_TR' => 'Turkish',
    'tt_RU' => 'Tatar',
    'uk_UA' => 'Ukrainian',
    'ur_PK' => 'Urdu',
    'uz_UZ' => 'Uzbek',
    'vi_VN' => 'Vietnamese',
    'xh_ZA' => 'Xhosa',
    'yi_DE' => 'Yiddish',
    'zh_CN' => 'Simplified Chinese (China)',
    'zh_HK' => 'Traditional Chinese (Hong Kong)',
    'zh_TW' => 'Traditional Chinese (Taiwan)',
    'zu_ZA' => 'Zulu',
  );
}
