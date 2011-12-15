<?php

/**
 * @file
 * Determination of user's Country based on IP address.
 *
 * This module uses the IP Address that a user is connected from to deduce
 * the Country where the user is located.  This method is not foolproof,
 * because a user may connect through an anonymizing proxy, or may be in
 * an unusual case, such as getting service from a neighboring country,
 * or using an IP block leased from a company in another country.
 * Additionaly, users accessing a server on a local network may be using
 * an IP that is not assigned to any country (e.g. 192.168.x.x).
 *
 * Country determination occurs upon user login.  If a country can be
 * determined from the IP address, the ISO 3166 2-character country code
 * is stored in the Drupal $user object as $user->country_iso_code_2.
 * If no country can be determined, this member is left unset.
 *
 * The database used is maintained by ARIN, the American Registry for
 * Internet Numbers (http://www.arin.net/about_us/index.html), which is
 * one of the 5 official Regional Internet Registries (RIR) responsible
 * for assigning IP addresses.  The claim is the database is 98% accurate,
 * with most of the problems coming from users in less-developed countries.
 * Regardless, there's no more-authoritive source of this information.
 *
 * @author Tim Rohaly.    <http://drupal.org/user/202830>
 */


/** Drupal core function country_get_list() which returns a list of countries */
include_once DRUPAL_ROOT . '/includes/locale.inc';

/******************************************************************************
 * Ajax Callbacks                                                             *
 ******************************************************************************/

/**
 * AJAX callback to update the IP to Country database.
 *
 * @param $rir
 *   String with name of IP registry.  One of afrinic, arin, lacnic, ripe.
 *
 * @return
 *   JSON object for display by jQuery script.
 */
function _ip2country_update($rir) {

  // Update DB from RIR
  ip2country_update_database($rir);

  if (variable_get('ip2country_watchdog', 1)) {
    watchdog('ip2country', 'Database updated from @registry server.', array('@registry' => drupal_strtoupper($rir)), WATCHDOG_NOTICE);
  }
  print drupal_json_encode(array(
    'count'   => t('@rows rows affected.',
                    array('@rows' => ip2country_get_count())),
    'server'  => $rir,
    'message' => t('The IP to Country database has been updated from @server.',
                   array('@server' => drupal_strtoupper($rir))),
  ));
  exit();
}


/**
 * AJAX callback to lookup an IP address in the database.
 *
 * @param $arg
 *   String with IP address.
 *
 * @return
 *   JSON object for display by jQuery script.
 */
function _ip2country_lookup($arg) {

  // Return results of manual lookup
  $country_code = ip2country_get_country($arg);
  if ($country_code) {
    $country_list = country_get_list();
    $country_name = $country_list[$country_code];
    print drupal_json_encode(array(
      'message' => t('IP Address @ip is assigned to @country (@code).',
                     array('@ip'      => $arg,
                           '@country' => $country_name,
                           '@code'    => $country_code)),
    ));
  }
  else {
    print drupal_json_encode(array(
      'message' => t('IP Address @ip is not assigned to a country.',
                     array('@ip' => $arg)),
    ));
  }
  exit();
}


/******************************************************************************
 * Menu Callbacks                                                             *
 ******************************************************************************/


/**
 * Default IP to Country administration settings.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 *
 * @see ip2country_admin_settings_submit()
 * @ingroup forms
 */
function ip2country_admin_settings(&$form_state) {
  drupal_add_js(drupal_get_path('module', 'ip2country') . '/ip2country.js');
  drupal_add_css(drupal_get_path('module', 'ip2country') . '/ip2country.css');

  // Define submit handler function
  $form['#submit'][] = 'ip2country_admin_settings_submit';

  // Container for database update preference forms
  $form['ip2country_database_update'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Database Updates'),
    '#collapsible'  => FALSE,
    '#collapsed'    => FALSE,
  );

  // Form to enable watchdog logging of updates
  $form['ip2country_database_update']['ip2country_watchdog'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Log database updates to watchdog'),
    '#default_value' => variable_get('ip2country_watchdog', 1),
  );

  // Form to choose RIR
  $form['ip2country_database_update']['ip2country_rir'] = array(
    '#type'          => 'select',
    '#title'         => t('Regional Internet Registry to use'),
    '#options'       => array('afrinic' => 'AFRINIC', 'apnic' => 'APNIC', 'arin' => 'ARIN', 'lacnic' => 'LACNIC', 'ripe' => 'RIPE'),
    '#default_value' => variable_get('ip2country_rir', 'arin'),
    '#description'   => t('You may find that the regional server nearest you has the best response time, but note that AFRINIC provides only its own subset of registration data.'),
  );

  $period = drupal_map_assoc(array(86400, 302400, 604800, 1209600, 2419200), 'format_interval');

  // Form to set automatic update interval
  $form['ip2country_database_update']['ip2country_update_interval'] = array(
    '#type'          => 'select',
    '#title'         => t('Database update frequency'),
    '#default_value' => variable_get('ip2country_update_interval', 604800),
    '#options'       => $period,
    '#description'   => t('Database will be automatically updated via cron.php.  Cron must be enabled for this to work.  Default period is 1 week (604800 seconds).'),
  );

  $update_time = variable_get('ip2country_last_update', 0);
  if (!empty($update_time)) {
    $message = t('Database last updated on @date at @time', array('@date' => format_date($update_time, 'custom', 'n/j/Y'), '@time' => format_date($update_time, 'custom', 'H:i:s T')));
  }
  else {
    $message = t('Database is empty.  You may fill the database by pressing the @update button.', array('@update' => t('Update')));
  }

  // Form to initiate manual updating of the IP-Country database
  $form['ip2country_database_update']['ip2country_update_database'] = array(
    '#type'        => 'button',
    '#value'       => t('Update'),
    '#executes_submit_callback' => FALSE,
    '#prefix'      => '<div>' . t('The IP to Country Database may be updated manually by pressing the Update button below.  Note, this may take several minutes.') . '</div>',
    '#suffix'      => '<span id="dbthrobber" class="message">' . $message . '</span>',
  );

  // Container for manual lookup
  $form['ip2country_manual_lookup'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Manual Lookup'),
    '#description' => t('Examine database values'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );

  // Form for IP address for manual lookup
  $form['ip2country_manual_lookup']['ip2country_lookup'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Manual Lookup'),
    '#description' => t('Enter IP address'),
  );

  // Form to initiate manual lookup
  $form['ip2country_manual_lookup']['ip2country_lookup_button'] = array(
    '#type'        => 'button',
    '#value'       => t('Lookup'),
    '#executes_submit_callback' => FALSE,
    '#prefix'      => '<div>' . t('An IP address may be looked up in the database by entering the address above then pressing the Lookup button below.') . '</div>',
    '#suffix'      => '<span id="lookup-message" class="message"></span>',
  );

  // Container for debugging preference forms
  $form['ip2country_debug_preferences'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Debug Preferences'),
    '#description' => t('Set debugging values'),
    '#collapsible' => FALSE,
    '#collapsed'   => FALSE,
  );

  // Form to turn on debugging
  $form['ip2country_debug_preferences']['ip2country_debug'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Admin Debug'),
    '#default_value' => variable_get('ip2country_debug', FALSE),
    '#description'   => t('Enables administrator to spoof an IP Address or Country for debugging purposes.'),
  );

  // Form to select Dummy Country or Dummy IP Address for testing
  $form['ip2country_debug_preferences']['ip2country_test_type'] = array(
    '#type'          => 'radios',
    '#title'         => t('Select which parameter to spoof'),
    '#default_value' => variable_get('ip2country_test_type', 0),
    '#options'       => array(t('Country'), t('IP Address')),
  );

  $default_country = variable_get('ip2country_test_country',
                                  ip2country_get_country(ip_address()));

  // Form to enter Country to spoof
  $form['ip2country_debug_preferences']['ip2country_test_country'] = array(
    '#type'          => 'select',
    '#title'         => t('Country to use for testing'),
    '#default_value' => $default_country,
    '#options'       => country_get_list(),
  );

  // Form to enter IP address to spoof
  $form['ip2country_debug_preferences']['ip2country_test_ip_address'] = array(
    '#type'          => 'textfield',
    '#title'         => t('IP address to use for testing'),
    '#default_value' => variable_get('ip2country_test_ip_address', ip_address()),
  );

  return system_settings_form($form);
}


/**
 * Processes forms submitted by IP to Country administration page.
 *
 * @see ip2country_admin_settings()
 */
function ip2country_admin_settings_submit($form, &$form_state) {
  global $user;

  // Exclude unnecessary elements from being saved in variable table
  unset($form_state['values']['ip2country_update_database'],
        $form_state['values']['ip2country_lookup'],
        $form_state['values']['ip2country_lookup_button']);

  // Check to see if debug set
  if ($form_state['values']['ip2country_debug']) {
    // Debug on
    if ($form_state['values']['ip2country_test_type']) {
      // Dummy IP Address
      $ip = $form_state['values']['ip2country_test_ip_address'];
      $country_code = ip2country_get_country($ip);
    }
    else {
      // Dummy Country
      $country_code = $form_state['values']['ip2country_test_country'];
    }
    $country_list = country_get_list();
    $country_name = $country_list[$country_code];
    drupal_set_message(t('Using DEBUG value for Country - @country (@code)', array('@country' => $country_name, '@code'    => $country_code)));
  }
  else {
    // Debug off - make sure we set/reset IP/Country to their real values
    $ip = ip_address();
    $country_code = ip2country_get_country($ip);
    drupal_set_message(t('Using ACTUAL value for Country - @country', array('@country' => $country_code)));
  }

  // Finally, save country, if it has been determined
  if ($country_code) {
    // Store the ISO country code in the $user object
    user_save($user, array('country_iso_code_2' => $country_code));
  }
}
