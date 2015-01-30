<?php

/**
 * @file
 *   Gettext import for localization community.
 */

/**
 * User interface for the translation import screen.
 */
function l10n_community_import_page($langcode) {
  $language = l10n_community_get_language($langcode);
  drupal_set_title(t('Import to @language', array('@language' => t($language->name))), PASS_THROUGH);
  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore languages'), 'translate/languages'),
      l(t($language->name), 'translate/languages/' . $langcode),
    )
  );
  return drupal_get_form('l10n_community_import_form', $langcode);
}

/**
 * Translation import form.
 */
function l10n_community_import_form($form, &$form_state, $langcode) {
  global $user;

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['langcode'] = array(
    '#type' => 'value',
    '#value' => $langcode,
  );

  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Gettext .po file with translations'),
    '#size' => 50,
    '#description' => t('The maximum allowed size for uploads is %maxsize.', array('%maxsize' => format_size(file_upload_max_size()))),
  );

  if (($import_name = variable_get('l10n_community_import_user', '')) && ($import_account = array_shift(user_load_multiple(array(), array('name' => $import_name))))) {
    // If available, let the user import with attribution to a bulk import user.
    $form['import_uid'] = array(
      '#title' => t('Attribute import to'),
      '#type' => 'radios',
      '#default_value' => $user->uid,
      '#options' => array(
        $user->uid => t('Yourself (%name)', array('%name' => $user->name)),
        $import_account->uid => $import_name,
      ),
      '#description' => t('When importing the result of teamwork, If the imported translations were worked on by a team of people, it is common curtesy to not attribute them to your personal username to ensure you do not claim credit for work of the whole team.'),
    );
  }
  else {
    $form['import_uid'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
  }

  if (!l10n_community_access('moderate own suggestions') && (!l10n_community_access('moderate suggestions from others') || empty($import_account))) {
    // Either has no permission to approve own or approve others, or we do not
    // have a mass import account and no permission to approve own, so no way
    // we can offer importing translations directly.
    $form['is_suggestion'] = array(
      '#type' => 'value',
      '#value' => 1,
    );
  }
  else {
    // If can submit translations, offer option to pick mode.
    $form['is_suggestion'] = array(
      '#title' => t('Status'),
      '#type' => 'radios',
      '#default_value' => 0,
      '#options' => array(
        0 => t('Store as approved translations'),
        1 => t('Store as suggestions needing approval, possibly discussion'),
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * Validate the locale import form submission.
 */
function l10n_community_import_form_validate($form, &$form_state) {
  global $user;

  if (empty($form_state['values']['is_suggestion'])) {
    if ($form_state['values']['import_uid'] == $user->uid) {
      // Trying to submit translation in her own name.
      if (!l10n_community_access('moderate own suggestions')) {
        form_set_error('is_suggestion', t('You do not have sufficient permissions to submit translations directly in your name. Please submit as suggestions.'));
      }
    }
    else {
      // Trying to submit translation in the import_account's name.
      if (!l10n_community_access('moderate suggestions from others')) {
        form_set_error('is_suggestion', t('You do not have sufficient permissions to submit translations directly in the name of other users. Please submit as suggestions.'));
      }
    }
  }

}

/**
 * Process the locale import form submission.
 */
function l10n_community_import_form_submit($form, &$form_state) {
  // Save file in the local file system.
  $validators = array('file_validate_extensions' => array());
  if ($file = file_save_upload('file', $validators)) {

    // Increase time limit for PO parsing if possible.
    if (!ini_get('safe_mode')) {
      @set_time_limit(240);
    }

    // Do the actual parsing on the local file.
    if (l10n_server_parse_po($file, '_l10n_community_import_one_string', array($form_state['values']['langcode'], $form_state['values']['is_suggestion'], $form_state['values']['import_uid']))) {
      drupal_set_message(t('The translation was successfully imported.'));
      l10n_community_update_message();
      cache_clear_all('l10n:stats:' . $form_state['values']['langcode'], 'cache');
    }
  }
  else {
    drupal_set_message(t('File to import not found. Did you choose a .po file to upload which was under %maxsize?', array('%maxsize' => format_size(file_upload_max_size()))), 'error');
  }
}

/**
 * Imports a string into the database.
 *
 * @param $value
 *   Details of the string stored.
 * @param $langcode
 *   Language to store the string in.
 * @param $is_suggestion
 *   TRUE if the string to store is a suggestion, FALSE otherwise.
 * @param $uid
 *   User id used to save attribution information.
 */
function _l10n_community_import_one_string($value, $langcode = NULL, $is_suggestion = FALSE, $uid = NULL) {
  global $user;

  // Trim translation (we will apply source string based whitespace later).
  if (is_string($value['msgstr'])) {
    $value['msgstr'] = trim($value['msgstr']);
  }

  if (!empty($value['msgid']) && !empty($value['msgstr'])) {
    // We only save non-empty translations/suggestions.

    if (empty($uid)) {
      $uid = $user->uid;
    }

    // If the comment array for this value contains the ', fuzzy' flag, then
    // mark this as a suggestion import in all cases.
    if (!empty($value['#'])) {
      $is_suggestion = ($is_suggestion ? TRUE : in_array(', fuzzy', $value['#']));
    }

    // If context was not set, set to empty.
    $value['msgctxt'] = !empty($value['msgctxt']) ? $value['msgctxt'] : '';

    if ($sid = db_query("SELECT sid FROM {l10n_server_string} WHERE hashkey = :hashkey",
               array(':hashkey' => md5($value['msgid'] . $value['msgctxt'])))
      ->fetchField()) {
      // Merge plural versions into one for saving values.
      $value['msgstr'] = is_array($value['msgstr']) ? join("\0", $value['msgstr']) : $value['msgstr'];

      // Add this as a suggestion first.
      $tid = l10n_community_add_suggestion($sid, $value['msgstr'], $langcode, $uid, $user->uid, L10N_SERVER_MEDIUM_IMPORT);

      if ($tid) {
        if ($is_suggestion) {
          l10n_community_counter(L10N_COUNT_SUGGESTED);
        }
        else {
          l10n_community_approve_string($langcode, $sid, $tid);
          l10n_community_counter(L10N_COUNT_ADDED);
        }
      }
      elseif ($tid === FALSE) {
        l10n_community_counter(L10N_COUNT_DUPLICATE);
      }


    }
    else {
      // Source string not found, string ignored.
      l10n_community_counter(L10N_COUNT_IGNORED);
    }
  }
}
