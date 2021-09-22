<?php

/**
 * @file
 *   Development helpers for localization server.
 */

// == Language generation ======================================================

/**
 * User interface to generate a number of languages.
 */
function l10n_server_generate_languages_form($form, &$form_state) {
  include_once DRUPAL_ROOT . '/' . 'includes/iso.inc';
  $predefined = _locale_get_predefined_list();
  $form['num'] = array(
    '#type' => 'textfield',
    '#title' => t('How many languages would you like to generate (maximum @count possible)?', array('@count' => count($predefined))),
    '#default_value' => 30,
    '#size' => 10,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Do it!'),
  );
  return $form;
}

/**
 * Submission function to generate a number of languages.
 */
function l10n_server_generate_languages_form_submit($form_id, &$form_state) {
  include_once DRUPAL_ROOT . '/' . 'includes/iso.inc';
  db_delete('languages') ->condition('language', 'en', '<>') ->execute();
  db_delete('locales_target') ->execute();
  drupal_set_message(t('Languages deleted.'));
  $num = $form_state['values']['num'];
  if ($num > 0) {
    $predefined = _locale_get_predefined_list();
    if ($num > count($predefined)) {
      $languages = $predefined;
    }
    else {
      $languages = array();
      while (count($languages) < $num) {
        $key = array_rand($predefined);
        $languages[$key] = $predefined[$key];
        unset($predefined[$key]);
      }
    }

    $formulas = l10n_server_plural_formulas();
    $default_formula = $default = 'nplurals=2; plural=(n!=1);';
    foreach ($languages as $langcode => $language) {
      $plural_formula = (isset($formulas['langcode']) ? $formulas['langcode'] : $default_formula);
      // We know we get a valid formula here so no need for error handling.
      list($plurals, $formula) = l10n_server_parse_plural_formula($plural_formula);
      db_insert('languages')
	->fields(array(
            'language' => $langcode,
            'name' => $language[0],
            'native' => isset($language[1]) ? $language[1] : $language[0],
            'direction' => isset($language[2]) ? $language[2] : LANGUAGE_LTR,
            'enabled' => 1,
            'plurals' => $plurals,
            'formula' => $formula,
            'domain' => '',
            'prefix' => $langcode,
            'weight' => 0,
            'javascript' => '',
          ))
	->execute();
    }
    drupal_set_message(format_plural($num, '1 language created.', '@count languages created.'));
  }
}

// == Translation / suggestion generation ======================================

/**
 * User interface to generate translations and languages.
 */
function l10n_server_generate_translations_form($form, &$form_state) {
  $count = db_query('SELECT COUNT(*) FROM {l10n_server_string}')->fetchField();
  if (empty($count)) {
    drupal_set_message(t('No source strings found, cannot generate translations and suggestions.'));
    return;
  }
  else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Do it!'),
    );
    return $form;
  }
}

/**
 * Submission function to generate translations and suggestions.
 *
 * @todo Add suggestion generation.
 */
function l10n_server_generate_translations_form_submit($form_id, &$form_state) {
  db_delete('l10n_server_translation') ->execute();
  drupal_set_message(t('Previous translations and suggestions deleted.'));

  $count = db_query('SELECT COUNT(*) FROM {l10n_server_string}')->fetchField();
  $languages = language_list();
  $limit = (int) $count * 0.75;
  foreach ($languages as $language) {
    if ($language->language != 'en') {
      $uid1 = db_query_range('SELECT uid FROM {users} ORDER BY RAND()', 0, 1);
      $uid2 = db_query_range('SELECT uid FROM {users} ORDER BY RAND()', 0, 1);

      db_query("
INSERT INTO {l10n_server_translation}
SELECT NULL, sid, :lng, value, :uid1, :uid2, :rt1, :rt2, 0, 0, 1
FROM {l10n_server_string} LIMIT 0, :limit",
        array(':lng' => $language->language,
          ':uid1' => $uid1,
          ':uid2' => $uid2,
          ':rt1' => REQUEST_TIME,
          ':rt2' => REQUEST_TIME,
          ':limit' => $limit)
      );
    }
  }
  drupal_set_message(format_plural($limit * count($langauges), '1 translation created.', '@count translations created.'));
}
