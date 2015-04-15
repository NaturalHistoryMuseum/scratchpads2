<?php

/**
 * @file
 * Override part of locale.inc library so we can manage string status
 */

/**
 * Parses Gettext Portable Object file information and inserts into database
 *
 * This is an improved version of _locale_import_po() to handle translation status
 *
 * @param $file
 *   Drupal file object corresponding to the PO file to import
 * @param $langcode
 *   Language code
 * @param $mode
 *   Should existing translations be replaced LOCALE_IMPORT_KEEP or LOCALE_IMPORT_OVERWRITE
 * @param $group
 *   Text group to import PO file into (eg. 'default' for interface translations)
 *
 * @return boolean
 *   Result array on success. FALSE on failure
 */
function _l10n_update_locale_import_po($file, $langcode, $mode, $group = NULL) {
  // Try to allocate enough time to parse and import the data.
  drupal_set_time_limit(240);

  // Check if we have the language already in the database.
  if (!db_query("SELECT COUNT(language) FROM {languages} WHERE language = :language", array(':language' => $langcode))->fetchField()) {
    drupal_set_message(t('The language selected for import is not supported.'), 'error');
    return FALSE;
  }

  // Get strings from file (returns on failure after a partial import, or on success)
  $status = _l10n_update_locale_import_read_po('db-store', $file, $mode, $langcode, $group);
  if ($status === FALSE) {
    // Error messages are set in _locale_import_read_po().
    return FALSE;
  }

  // Get status information on import process.
  list($header_done, $additions, $updates, $deletes, $skips) = _l10n_update_locale_import_one_string('db-report');

  if (!$header_done) {
    drupal_set_message(t('The translation file %filename appears to have a missing or malformed header.', array('%filename' => $file->filename)), 'error');
  }

  watchdog('locale', 'Imported %file into %locale: %number new strings added, %update updated and %delete removed.', array('%file' => $file->filename, '%locale' => $langcode, '%number' => $additions, '%update' => $updates, '%delete' => $deletes));
  if ($skips) {
    watchdog('locale', '@count disallowed HTML string(s) in %file', array('@count' => $skips, '%file' => $file->uri), WATCHDOG_WARNING);
  }

  // Return results of this import.
  return array(
    'file' => $file,
    'language' => $langcode,
    'add' => $additions,
    'update' => $updates,
    'delete' => $deletes,
    'skip' => $skips,
  );
}

/**
 * Parses Gettext Portable Object file into an array
 *
 * @param $op
 *   Storage operation type: db-store or mem-store
 * @param $file
 *   Drupal file object corresponding to the PO file to import
 * @param $mode
 *   Should existing translations be replaced LOCALE_IMPORT_KEEP or LOCALE_IMPORT_OVERWRITE
 * @param $lang
 *   Language code
 * @param $group
 *   Text group to import PO file into (eg. 'default' for interface translations)
 */
function _l10n_update_locale_import_read_po($op, $file, $mode = NULL, $lang = NULL, $group = 'default') {

  $fd = fopen($file->uri, "rb"); // File will get closed by PHP on return
  if (!$fd) {
    _locale_import_message('The translation import failed, because the file %filename could not be read.', $file);
    return FALSE;
  }

  $context = "COMMENT"; // Parser context: COMMENT, MSGID, MSGID_PLURAL, MSGSTR and MSGSTR_ARR
  $current = array();   // Current entry being read
  $plural = 0;          // Current plural form
  $lineno = 0;          // Current line

  while (!feof($fd)) {
    $line = fgets($fd, 10*1024); // A line should not be this long
    if ($lineno == 0) {
      // The first line might come with a UTF-8 BOM, which should be removed.
      $line = str_replace("\xEF\xBB\xBF", '', $line);
    }
    $lineno++;
    $line = trim(strtr($line, array("\\\n" => "")));

    if (!strncmp("#", $line, 1)) { // A comment
      if ($context == "COMMENT") { // Already in comment context: add
        $current["#"][] = substr($line, 1);
      }
      elseif (($context == "MSGSTR") || ($context == "MSGSTR_ARR")) { // End current entry, start a new one
        _l10n_update_locale_import_one_string($op, $current, $mode, $lang, $file, $group);
        $current = array();
        $current["#"][] = substr($line, 1);
        $context = "COMMENT";
      }
      else { // Parse error
        _locale_import_message('The translation file %filename contains an error: "msgstr" was expected but not found on line %line.', $file, $lineno);
        return FALSE;
      }
    }
    elseif (!strncmp("msgid_plural", $line, 12)) {
      if ($context != "MSGID") { // Must be plural form for current entry
        _locale_import_message('The translation file %filename contains an error: "msgid_plural" was expected but not found on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 12));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgid"] = $current["msgid"] . "\0" . $quoted;
      $context = "MSGID_PLURAL";
    }
    elseif (!strncmp("msgid", $line, 5)) {
      if (($context == "MSGSTR") || ($context == "MSGSTR_ARR")) {   // End current entry, start a new one
        _l10n_update_locale_import_one_string($op, $current, $mode, $lang, $file, $group);
        $current = array();
      }
      elseif ($context == "MSGID") { // Already in this context? Parse error
        _locale_import_message('The translation file %filename contains an error: "msgid" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 5));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgid"] = $quoted;
      $context = "MSGID";
    }
    elseif (!strncmp("msgctxt", $line, 7)) {
      if (($context == "MSGSTR") || ($context == "MSGSTR_ARR")) {   // End current entry, start a new one
        _l10n_update_locale_import_one_string($op, $current, $mode, $lang, $file, $group);
        $current = array();
      }
      elseif (!empty($current["msgctxt"])) { // Already in this context? Parse error
        _locale_import_message('The translation file %filename contains an error: "msgctxt" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 7));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgctxt"] = $quoted;
      $context = "MSGCTXT";
    }
    elseif (!strncmp("msgstr[", $line, 7)) {
      if (($context != "MSGID") && ($context != "MSGCTXT") && ($context != "MSGID_PLURAL") && ($context != "MSGSTR_ARR")) { // Must come after msgid, msgxtxt, msgid_plural, or msgstr[]
        _locale_import_message('The translation file %filename contains an error: "msgstr[]" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      if (strpos($line, "]") === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $frombracket = strstr($line, "[");
      $plural = substr($frombracket, 1, strpos($frombracket, "]") - 1);
      $line = trim(strstr($line, " "));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgstr"][$plural] = $quoted;
      $context = "MSGSTR_ARR";
    }
    elseif (!strncmp("msgstr", $line, 6)) {
      if (($context != "MSGID") && ($context != "MSGCTXT")) {   // Should come just after a msgid or msgctxt block
        _locale_import_message('The translation file %filename contains an error: "msgstr" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 6));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgstr"] = $quoted;
      $context = "MSGSTR";
    }
    elseif ($line != "") {
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      if (($context == "MSGID") || ($context == "MSGID_PLURAL")) {
        $current["msgid"] .= $quoted;
      }
      elseif ($context == "MSGCTXT") {
        $current["msgctxt"] .= $quoted;
      }
      elseif ($context == "MSGSTR") {
        $current["msgstr"] .= $quoted;
      }
      elseif ($context == "MSGSTR_ARR") {
        $current["msgstr"][$plural] .= $quoted;
      }
      else {
        _locale_import_message('The translation file %filename contains an error: there is an unexpected string on line %line.', $file, $lineno);
        return FALSE;
      }
    }
  }

  // End of PO file, flush last entry.
  if (($context == "MSGSTR") || ($context == "MSGSTR_ARR")) {
    _l10n_update_locale_import_one_string($op, $current, $mode, $lang, $file, $group);
  }
  elseif ($context != "COMMENT") {
    _locale_import_message('The translation file %filename ended unexpectedly at line %line.', $file, $lineno);
    return FALSE;
  }

}

/**
 * Imports a string into the database
 *
 * @param $op
 *   Operation to perform: 'db-store', 'db-report', 'mem-store' or 'mem-report'
 * @param $value
 *   Details of the string stored
 * @param $mode
 *   Should existing translations be replaced LOCALE_IMPORT_KEEP or LOCALE_IMPORT_OVERWRITE
 * @param $lang
 *   Language to store the string in
 * @param $file
 *   Object representation of file being imported, only required when op is 'db-store'
 * @param $group
 *   Text group to import PO file into (eg. 'default' for interface translations)
 */
function _l10n_update_locale_import_one_string($op, $value = NULL, $mode = NULL, $lang = NULL, $file = NULL, $group = 'default') {
  $report = &drupal_static(__FUNCTION__, array('additions' => 0, 'updates' => 0, 'deletes' => 0, 'skips' => 0));
  $header_done = &drupal_static(__FUNCTION__ . ':header_done', FALSE);
  $strings = &drupal_static(__FUNCTION__ . ':strings', array());

  switch ($op) {
    // Return stored strings
    case 'mem-report':
      return $strings;

    // Store string in memory (only supports single strings)
    case 'mem-store':
      $strings[isset($value['msgctxt']) ? $value['msgctxt'] : ''][$value['msgid']] = $value['msgstr'];
      return;

    // Called at end of import to inform the user
    case 'db-report':
      return array($header_done, $report['additions'], $report['updates'], $report['deletes'], $report['skips']);

    // Store the string we got in the database.
    case 'db-store':
      // We got header information.
      if ($value['msgid'] == '') {
        $languages = language_list();
        if (($mode != LOCALE_IMPORT_KEEP) || empty($languages[$lang]->plurals)) {
          // Since we only need to parse the header if we ought to update the
          // plural formula, only run this if we don't need to keep existing
          // data untouched or if we don't have an existing plural formula.
          $header = _locale_import_parse_header($value['msgstr']);

          // Get the plural formula and update in database.
          if (isset($header["Plural-Forms"]) && $p = _locale_import_parse_plural_forms($header["Plural-Forms"], $file->uri)) {
            list($nplurals, $plural) = $p;
            db_update('languages')
              ->fields(array(
                'plurals' => $nplurals,
                'formula' => $plural,
              ))
              ->condition('language', $lang)
              ->execute();
          }
          else {
            db_update('languages')
              ->fields(array(
                'plurals' => 0,
                'formula' => '',
              ))
              ->condition('language', $lang)
              ->execute();
          }
        }
        $header_done = TRUE;
      }

      else {
        // Some real string to import.
        $comments = _locale_import_shorten_comments(empty($value['#']) ? array() : $value['#']);

        if (strpos($value['msgid'], "\0")) {
          // This string has plural versions.
          $english = explode("\0", $value['msgid'], 2);
          $entries = array_keys($value['msgstr']);
          for ($i = 3; $i <= count($entries); $i++) {
            $english[] = $english[1];
          }
          $translation = array_map('_locale_import_append_plural', $value['msgstr'], $entries);
          $english = array_map('_locale_import_append_plural', $english, $entries);
          foreach ($translation as $key => $trans) {
            if ($key == 0) {
              $plid = 0;
            }
            $plid = _l10n_update_locale_import_one_string_db($report, $lang, isset($value['msgctxt']) ? $value['msgctxt'] : '', $english[$key], $trans, $group, $comments, $mode, L10N_UPDATE_STRING_DEFAULT, $plid, $key);
          }
        }

        else {
          // A simple string to import.
          $english = $value['msgid'];
          $translation = $value['msgstr'];
          _l10n_update_locale_import_one_string_db($report, $lang, isset($value['msgctxt']) ? $value['msgctxt'] : '', $english, $translation, $group, $comments, $mode);
        }
      }
  } // end of db-store operation
}

/**
 * Import one string into the database.
 *
 * @param $report
 *   Report array summarizing the number of changes done in the form:
 *   array(inserts, updates, deletes).
 * @param $langcode
 *   Language code to import string into.
 * @param $context
 *   The context of this string.
 * @param $source
 *   Source string.
 * @param $translation
 *   Translation to language specified in $langcode.
 * @param $textgroup
 *   Name of textgroup to store translation in.
 * @param $location
 *   Location value to save with source string.
 * @param $mode
 *   Import mode to use, LOCALE_IMPORT_KEEP or LOCALE_IMPORT_OVERWRITE.
 * @param $status
 *   Status of translation if created: L10N_UPDATE_STRING_DEFAULT or L10N_UPDATE_STRING_CUSTOM
 * @param $plid
 *   Optional plural ID to use.
 * @param $plural
 *   Optional plural value to use.
 * @return
 *   The string ID of the existing string modified or the new string added.
 */
function _l10n_update_locale_import_one_string_db(&$report, $langcode, $context, $source, $translation, $textgroup, $location, $mode, $status = L10N_UPDATE_STRING_DEFAULT, $plid = 0, $plural = 0) {
  $lid = db_query("SELECT lid FROM {locales_source} WHERE source = :source AND context = :context AND textgroup = :textgroup", array(':source' => $source, ':context' => $context, ':textgroup' => $textgroup))->fetchField();

  if (!empty($translation)) {
    // Skip this string unless it passes a check for dangerous code.
    // Text groups other than default still can contain HTML tags
    // (i.e. translatable blocks).
    if ($textgroup == "default" && !locale_string_is_safe($translation)) {
      $report['skips']++;
      $lid = 0;
      watchdog('locale', 'Disallowed HTML detected. String not imported: %string', array('%string' => $translation), WATCHDOG_WARNING);
    }
    elseif ($lid) {
      // We have this source string saved already.
      db_update('locales_source')
        ->fields(array(
          'location' => $location,
        ))
        ->condition('lid', $lid)
        ->execute();

      $exists = db_query("SELECT lid, l10n_status FROM {locales_target} WHERE lid = :lid AND language = :language", array(':lid' => $lid, ':language' => $langcode))->fetchObject();

      if (!$exists) {
        // No translation in this language.
        db_insert('locales_target')
          ->fields(array(
            'lid' => $lid,
            'language' => $langcode,
            'translation' => $translation,
            'plid' => $plid,
            'plural' => $plural,
          ))
          ->execute();

        $report['additions']++;
      }
      elseif (($exists->l10n_status == L10N_UPDATE_STRING_DEFAULT && $mode == LOCALE_UPDATE_OVERRIDE_DEFAULT) || $mode == LOCALE_IMPORT_OVERWRITE) {
        // Translation exists, only overwrite if instructed.
        db_update('locales_target')
          ->fields(array(
            'translation' => $translation,
            'plid' => $plid,
            'plural' => $plural,
          ))
          ->condition('language', $langcode)
          ->condition('lid', $lid)
          ->execute();

        $report['updates']++;
      }
    }
    else {
      // No such source string in the database yet.
      $lid = db_insert('locales_source')
        ->fields(array(
          'location' => $location,
          'source' => $source,
          'context' => (string) $context,
          'textgroup' => $textgroup,
        ))
        ->execute();

      db_insert('locales_target')
        ->fields(array(
           'lid' => $lid,
           'language' => $langcode,
           'translation' => $translation,
           'plid' => $plid,
           'plural' => $plural,
           'l10n_status' => $status,
        ))
        ->execute();

      $report['additions']++;
    }
  }
  elseif ($mode == LOCALE_IMPORT_OVERWRITE) {
    // Empty translation, remove existing if instructed.
    db_delete('locales_target')
      ->condition('language', $langcode)
      ->condition('lid', $lid)
      ->condition('plid', $plid)
      ->condition('plural', $plural)
      ->execute();

    $report['deletes']++;
  }

  return $lid;
}
