<?php

/**
 * @file
 * Administration functions for the Legal module.
 */

/**
 * Module settings form.
 */
function legal_administration($form_state) {
  $form       = array();
  $conditions = legal_get_conditions();

  if (module_exists('locale')) {
    $languages        = locale_language_list();
    $language_default = language_default();
    $language         = $language_default->language;
    $version_options  = array('version' => t('All users (new version)'), 'revision' => t('Language specific users (a revision)'));
    $version_handling = 'version';
  }
  else {
    $languages        = array('en' => t('English'));
    $language         = 'en';
    $version_handling = 'version';
  }

  $form = array_merge($form, legal_display_fields($conditions));

  $form['conditions'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Terms & Conditions'),
    '#default_value' => $conditions['conditions'],
    '#description'   => t('Your Terms & Conditions'),
    '#required'      => TRUE,
  );

  // Set this here or array will flatten out and override real values.
  $form['legal']['#tree'] = TRUE;

  // Overide accept checbox requirement on preview.
  $form['legal']['legal_accept']['#required'] = FALSE;

  // Overide display setting.
  $form['display'] = array(
    '#type'          => 'radios',
    '#title'         => t('Display Style'),
    '#default_value' => variable_get('legal_display', '0'),
    '#options'       => array(t('Scroll Box'), t('Scroll Box (CSS)'), t('HTML Text'), t('Page Link')),
    '#description'   => t('How terms & conditions should be displayed to users.'),
    '#required'      => TRUE,
  );

  // Only display options if there's more than one language available.
  if (count($languages) > 1) {
    // Language and version handling options.
    $form['language'] = array(
      '#type'        => 'fieldset',
      '#title'       => t('Language'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE,
    );

    $form['language']['language'] = array(
      '#type'          => 'select',
      '#title'         => t('Language'),
      '#options'       => $languages,
      '#default_value' => $language,
    );

    $form['language']['version_handling'] = array(
      '#type'          => 'select',
      '#title'         => t('Ask To Re-accept'),
      '#description'   => t('<strong>All users</strong>: all users will be asked to accept the new version of the T&C, including users who accepted a previous version.<br />
                           <strong>Language specific</strong>: only new users, and users who accepted the T&C in the same language as this new revision will be asked to re-accept.'),
      '#options'       => $version_options,
      '#default_value' => $version_handling,
    );
  }
  else {
    $form['language']['language']         = array('#type' => 'value', '#value' => $language);
    $form['language']['version_handling'] = array('#type' => 'value', '#value' => $version_handling);

  }

  // Additional checkboxes.
  $form['extras'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Additional Checkboxes'),
    '#description' => t('Each field will be shown as a checkbox which the user must tick to register.'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#tree'        => TRUE,
  );

  $extras_count = ((count($conditions['extras']) < 10) ? 10 : count($conditions['extras']));

  for ($counter = 1; $counter <= $extras_count; $counter++) {
    $extra = isset($conditions['extras']['extras-' . $counter]) ? $conditions['extras']['extras-' . $counter] : '';

    $form['extras']['extras-' . $counter] = array(
      '#type'          => 'textarea',
      '#title'         => t('Label'),
      '#default_value' => $extra,
    );

    // Overide extra checkboxes.
    if (!empty($conditions['extras']['extras-' . $counter])) {
      $form['legal']['extras-' . $counter] = array(
        '#type'          => 'checkbox',
        '#title'         => filter_xss_admin($extra),
        '#default_value' => 0,
        '#weight'        => 2,
        '#required'      => FALSE,
      );
    }
  }

  // Notes about changes to T&C.
  $form['changes'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Explain Changes'),
    '#description' => t('Explain what changes were made to the T&C since the last version. This will only be shown to users who accepted a previous version. Each line will automatically be shown as a bullet point.'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['changes']['changes'] = array(
    '#type'  => 'textarea',
    '#title' => t('Changes'),
  );

  $form['#after_build'] = array('legal_preview');

  $form['preview'] = array(
    '#type'  => 'button',
    '#value' => t('Preview'),
  );

  $form['save'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 *  After build function for legal_administration form.
 */
function legal_preview($form, $form_values) {

  switch ($form['display']['#value']) {
    case 1: // Scroll box (CSS).
    case 2: // HTML.
      $form['legal']['conditions']             = array(
        '#markup' => filter_xss_admin($form['conditions']['#value']),
      );
      $form['legal']['legal_accept']['#title'] = t('<strong>Accept</strong> Terms & Conditions of Use');
      break;
    case 3: // Page Link.
      $form['legal']['conditions']             = array(
        '#markup' => '',
      );
      $form['legal']['legal_accept']['#title'] = t('<strong>Accept</strong> <a href="@terms">Terms & Conditions</a> of Use', array('@terms' => url('legal')));
      break;
    default: // Scroll box (HTML).
      $form['legal']['conditions'] = array(
        '#id'         => 'preview',
        '#name'       => 'preview',
        '#type'       => 'textarea',
        '#title'      => t('Terms & Conditions'),
        '#value'      => $form['conditions']['#value'],
        '#parents'    => array('legal'),
        '#rows'       => 10,
        '#attributes' => array('readonly' => 'readonly'),
      );

      $form['legal']['legal_accept']['#title'] = t('<strong>Accept</strong> Terms & Conditions of Use');
  }

  // Overide additional checkboxes in preview.
  if (!empty($form_values['extras'])) {
    foreach ($form_values['extras'] as $key => $label) {
      if (empty($label)) {
        unset($form['legal'][$key]);
      }
      else {
        $form['legal'][$key]['#title'] = filter_xss_admin($label);
      }
    }
  }

  return $form;
}

function theme_legal_administration($variables) {

  $form     = $variables['form'];
  $language = '';

  if (empty($form['current_id']['#value'])) {
    $output = '<p><strong>' . t('Terms & Conditions will not be shown to users, as no T&C have been saved.') . '</strong></p>';
  }
  else {
    if (module_exists('locale')) {
      $languages = locale_language_list();
      $language  = $form['language_value']['#value'];
      $language  = check_plain($languages[$language]);
    }

    $output = '<h4>' . t('Most Recent Version/Revision') . '</h4>';
    $output .= '<p><strong>' . t('Version ID:') . '</strong> ' . $form['current_id']['#value'] . '<br />';
    if (!empty($language)) $output .= '<strong>' . t('Language:') . '</strong> ' . $language . '<br />';
    if (!empty($language)) $output .= '<strong>' . t('Revision:') . '</strong> ' . $form['revision_id']['#value'] . '<br />';
    $output .= '<strong>' . t('Created:') . '</strong> ' . date("l jS \of F Y h:i:s A", $form['current_date']['#value']) . '</p>';
  }

  // Preview.
  if (empty($form['legal']['conditions']['#markup'])) {
    $output .= drupal_render($form['legal']);
  }
  else {
    $form = theme('legal_display', array('form' => $form));
    $output .= '<div id="preview">';
    $output .= '<h3>' . t('Preview') . '</h3>';
    $output .= drupal_render($form['legal']);
    $output .= '</div>';
  }

  $output .= '<h4>' . t('Create New Version / Translation') . '</h4>';
  $output .= drupal_render_children($form);

  return $output;
}

function legal_administration_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Preview request, don't save anything.
  if ($form_state['triggering_element']['#value'] == t('Preview')) {
    return;
  }

  if (variable_get('legal_display', '0') != $values['display']) {
    variable_set('legal_display', $values['display']);
    drupal_set_message(t('Display setting has been saved.'));
  }

  // If new conditions are different from current permisions, enter in database.
  if (legal_conditions_updated($values)) {
    $version = legal_version($values['version_handling'], $values['language']);

    db_insert('legal_conditions')
      ->fields(array(
      'version'    => $version['version'],
      'revision'   => $version['revision'],
      'language'   => $values['language'],
      'conditions' => $values['conditions'],
      'date'       => time(),
      'extras'     => serialize($values['extras']),
      'changes'    => $values['changes'],
    ))
      ->execute();

    drupal_set_message(t('Terms & Conditions have been saved.'));
  }

  // Empty all cache.
  // @todo: is this necessary?
  cache_clear_all();
}

/**
 * Check if T&Cs have been updated.
 */
function legal_conditions_updated($new) {
  $previous_same_language = legal_get_conditions($new['language']);
  $previous               = legal_get_conditions();

  if (($previous_same_language['conditions'] != $new['conditions']) && ($previous['conditions'] != $new['conditions'])) {
    return TRUE;
  }

  $count = count($new['extras']);

  for ($counter = 1; $counter <= $count; $counter++) {
    $previous_same_language_extra = isset($previous_same_language['extras']['extras-' . $counter]) ? $previous_same_language['extras']['extras-' . $counter] : '';
    $previous_extra               = isset($previous['extras']['extras-' . $counter]) ? $previous['extras']['extras-' . $counter] : '';

    if (($previous_same_language_extra != $new['extras']['extras-' . $counter]) && ($previous_extra != $new['extras']['extras-' . $counter])) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Determine version ID of T&C.
 * If it's new version determine next version id,
 * if it's a revision return the ID of the current version increment revision ID by 1.
 */
function legal_version($version_handling, $language) {

  $version = (int)db_select('legal_conditions', 'lc')
    ->fields('lc', array('version'))
    ->orderBy('version', 'desc')
    ->range(0, 1)
    ->execute()
    ->fetchField();

  // make new version
  if ($version_handling == 'version') {
    $versioning['version']  = empty($version) ? 1 : $version + 1;
    $versioning['revision'] = 1;
  }

  // make new revision
  if ($version_handling == 'revision') {

    $revision = db_select('legal_conditions', 'lc')
      ->fields('lc', array('revision'))
      ->condition('version', $version)
      ->condition('language', $language)
      ->orderBy('revision', 'DESC')
      ->execute()
      ->fetchField();

    $versioning['version']  = $version;
    $versioning['revision'] = empty($revision) ? 1 : $revision + 1;
  }

  return $versioning;
}

/**
 * Languages administration form.
 */
function legal_languages() {

  $latest_header = array(t('Language'), t('Version'), t('Revision'));
  $latest_rows   = legal_versions_latest_get();
  $rows          = array();

  foreach ($latest_rows as $language_name => $language) {
    $row    = array();
    $row[]  = check_plain($language_name);
    $row[]  = empty($language['version']) ? '-' : $language['version'];
    $row[]  = empty($language['revision']) ? '-' : $language['revision'];
    $rows[] = $row;
  }

  $form['latest'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Latest Version'),
  );

  $form['latest']['#value'] = theme('table', array('header' => $latest_header, 'rows' => $rows));

  return $form;
}

/**
 * Get latest version for each language.
 */
function legal_versions_latest_get($language = NULL) {
  $conditions      = array();
  $current_version = db_select('legal_conditions', 'lc')
    ->fields('lc', array('version'))
    ->orderBy('version', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchField();

  // get latest version for each language
  if (empty($language)) {
    $languages = locale_language_list();

    foreach ($languages as $language_id => $language_name) {
      $result = db_select('legal_conditions', 'lc')
        ->fields('lc')
        ->condition('version', $current_version)
        ->condition('language', $language_id)
        ->orderBy('revision', 'DESC')
        ->range(0, 1)
        ->execute()
        ->fetchAllAssoc('tc_id');
      $row    = count($result) ? (object)array_shift($result) : FALSE;

      $conditions[$language_name] = legal_versions_latest_get_data($row);
    }

  } // get latest version for specific language
  else {
    $result = db_select('legal_conditions', 'lc')
      ->fields('lc')
      ->condition('language', $language)
      ->groupBy('language')
      ->orderBy('version', 'DESC')
      ->range(0, 1)
      ->execute()
      ->fetchAllAssoc('tc_id');
    $row    = count($result) ? (object)array_shift($result) : FALSE;

    $conditions[$language] = legal_versions_latest_get_data($row);
  }

  return $conditions;
}

function legal_versions_latest_get_data($data) {
  $row['revision']   = isset($data->revision) ? $data->revision : '';
  $row['language']   = isset($data->language) ? $data->language : '';
  $row['conditions'] = isset($data->conditions) ? $data->conditions : '';
  $row['date']       = isset($data->date) ? $data->date : '';
  $row['extras']     = isset($data->extras) ? $data->extras : '';
  $row['changes']    = isset($data->changes) ? $data->changes : '';

  return $row;
}
