<?php

/**
 * @file
 * Manage variables and features of module.
 *
 * More infos (schemas of file, use as an api, etc.) in TECHINFO.txt.
 */

/**
 * Available internal import/export formats.
 */
define('TAXONOMY_CSV_FORMAT_STRUCTURE',     'structure');
define('TAXONOMY_CSV_FORMAT_FLAT',          'flat');
define('TAXONOMY_CSV_FORMAT_TREE',          'tree');
define('TAXONOMY_CSV_FORMAT_POLYHIERARCHY', 'polyhierarchy');
define('TAXONOMY_CSV_FORMAT_FIELDS',        'fields');
define('TAXONOMY_CSV_FORMAT_TRANSLATE',     'translate');

/**
 * Available import options.
 */
define('TAXONOMY_CSV_EXISTING_UPDATE',          'update');
define('TAXONOMY_CSV_EXISTING_IGNORE',          'ignore');
// Internal use only.
define('TAXONOMY_CSV_EXISTING_IGNORE_PREVIOUS', 'ignore_previous');

/**
 * List of process levels matching watchdog levels.
 *
 * See _taxonomy_csv_message_watchdog_type and _taxonomy_csv_message_text.
 */
define('TAXONOMY_CSV_PROCESS_ERROR',   300); // Stop import process.
define('TAXONOMY_CSV_PROCESS_WARNING', 400); // Stop line process and go next.
define('TAXONOMY_CSV_PROCESS_NOTICE',  500); // Continue current line process.
define('TAXONOMY_CSV_PROCESS_INFO',    600); // Successfully processed.
define('TAXONOMY_CSV_PROCESS_DEBUG',   700); // Internal use only.

/**
 * Information about import process.
 *
 * Use too default Drupal constants:
 * - SAVED_NEW     = 1
 * - SAVED_UPDATED = 2
 * Possibly use of:
 * - SAVED_DELETED = 3
 */
define('TAXONOMY_CSV_ERROR',       0);
define('TAXONOMY_CSV_NEW_UPDATED', 4);
define('TAXONOMY_CSV_UNCHANGED',   5);

/**
 * Helper to remember some items and to describe features.
 *
 * @param $list
 *   A string matching list to be returned:
 *   - 'import_format'     : available formats for import.
 *   - 'export_format'     : available formats for export.
 *   - 'import_default_ui' : default options to import by user interface.
 *   - 'import_default_api': default options to import by api.
 *   - 'export_default_ui' : default options to export by user interface.
 *   - 'export_default_api': default options to export by api.
 *
 * @return
 *   Array of wanted content.
 */
function _taxonomy_csv_values($list) {
  switch ($list) {
    case 'import_format':
      return array(
        TAXONOMY_CSV_FORMAT_STRUCTURE => t('Structure'),
        TAXONOMY_CSV_FORMAT_FIELDS    => t('Fields'),
        TAXONOMY_CSV_FORMAT_TRANSLATE => t('Translation'),
      );

    case 'export_format':
      return array(
        TAXONOMY_CSV_FORMAT_FLAT      => t('Term names'),
        TAXONOMY_CSV_FORMAT_TREE      => t('Hierarchical tree structure'),
        TAXONOMY_CSV_FORMAT_FIELDS    => t('Fields'),
        TAXONOMY_CSV_FORMAT_TRANSLATE => t('Translation'),
      );

    case 'import_default_ui':
      return array(
        'import_format'                 => TAXONOMY_CSV_FORMAT_STRUCTURE,
        'structure_type'                => TAXONOMY_CSV_FORMAT_FLAT,
        'import_fields_format'          => 'name',
        'translate_by'                  => 'name',
        'translate_languages'           => '',
        'keep_order'                    => FALSE,
        'source_choice'                 => 'text',
        'import_delimiter'              => 'comma',
        'import_delimiter_soft_tab_width' => '2',
        'import_delimiter_custom'       => '',
        'import_enclosure'              => 'none',
        'import_enclosure_custom'       => '',
        'filter_format'                 => 'plain_text',
        'filter_format_custom'          => 'none',
        'import_language'               => 'und', // Undefined.
        'check_line'                    => TRUE,
        'check_utf8'                    => TRUE,
        'locale_custom'                 => '',
        'vocabulary_target'             => 'autocreate',
        'vocabulary_id'                 => 'choose_vocabulary',
        'i18n_mode'                     => 0, // I18N_MODE_NONE.
        'language'                      => 'und', // Undefined.
        'import_fields_custom'          => '',
        'delete_terms'                  => FALSE,
        'check_hierarchy'               => TRUE,
        'set_hierarchy'                 => 2, // Polyhierarchy.
        'update_or_ignore'                => TAXONOMY_CSV_EXISTING_UPDATE,
        // General options.
        'result_stats'                  => 'result_stats',
        'result_terms'                  => 'result_terms',
        'result_level'                  => 'notices',
        'result_type'                   => 'by_message',
      );

    case 'import_default_api':
      return array(
        'import_format'                 => TAXONOMY_CSV_FORMAT_FLAT,
        'fields_format'                 => array(0 => 'name'),
        'translate_by'                  => 'name',
        'translate_languages'           => '',
        'keep_order'                    => FALSE,
        'delimiter'                     => ',',
        'enclosure'                     => '',
        'filter_format'                 => 'plain_text',
        'filter_format_custom'          => 'none',
        // Warning: in API, language is option for terms.
        'language'                      => 'und', // Undefined.
        'check_line'                    => FALSE,
        'check_utf8'                    => FALSE,
        'locale_custom'                 => '',
        'vocabulary_target'             => 'autocreate',
        'vocabulary_id'                 => 'choose_vocabulary',
        'i18n_mode'                     => 0, // I18N_MODE_NONE.
        'vocabulary_language'           => 'und', // Undefined.
        'fields_custom'                 => array(),
        'delete_terms'                  => FALSE,
        'check_hierarchy'               => TRUE,
        'set_hierarchy'                 => 2, // Polyhierarchy.
        'update_or_ignore'                => TAXONOMY_CSV_EXISTING_UPDATE,
        // General options.
        'check_options'                 => FALSE,
        'result_display'                => FALSE,
        'result_stats'                  => FALSE,
        'result_terms'                  => FALSE,
        'result_level'                  => 'first',
        'result_type'                   => 'by_message',
      );

    case 'export_default_ui':
      return array(
        'export_format'                 => TAXONOMY_CSV_FORMAT_FLAT,
        'export_fields_format'          => 'name',
        'export_vocabulary_id'          => 0,
        'export_delimiter'              => 'comma',
        'export_delimiter_custom'       => '',
        'export_enclosure'              => 'quotation',
        'export_enclosure_custom'       => '',
        'export_line_ending'            => 'Unix',
        // Default options of specific imports.
        'def_links_terms_ids'           => 'name_if_needed',
        'def_links_vocabularies_ids'    => 'none',
        // General options.
        'export_order'                  => 'name',
        'result_duplicates'             => TRUE,
      );

    case 'export_default_api':
      return array(
        'export_format'                 => TAXONOMY_CSV_FORMAT_FLAT,
        'fields_format'                 => array(0 => 'name'),
        'vocabulary_id'                 => 0,
        'delimiter'                     => ',',
        'enclosure'                     => '"',
        'line_ending'                   => 'Unix',
        'order'                         => 'name',
        // Default options of specific imports.
        'def_links_terms_ids'           => 'name_if_needed',
        'def_links_vocabularies_ids'    => 'none',
        // General options.
        'result_duplicates'             => FALSE,
        'check_options'                 => FALSE,
        'result_display'                => FALSE,
      );
  }
}

/**
 * Returns worst message of a set of message codes.
 *
 * @todo Move into another included file or remove it.
 *
 * @param $messages
 *   Array of message code (000 to 799).
 *
 * @return
 *   Worst message code.
 */
function _taxonomy_csv_worst_message($messages) {
  return count($messages) ?
    min($messages) :
    799;
}

/**
 * Escapes carriage return and linefeed.
 *
 * This function is used for description field of terms and allows to get only
 * one csv line for one term.
 *
 * @param $string
 *   String to update.
 *
 * @return
 *   Updated string.
 */
function _taxonomy_csv_escape_line_break($string) {
  return str_replace(
    array("\r", "\n"),
    array('\r', '\n'),
    $string);
}

/**
 * Remove escapes carriage return and linefeed.
 *
 * This function is used for description field of terms and allows to import a
 * multiline text.
 *
 * @param $string
 *   String to update.
 *
 * @return
 *   Updated string.
 */
function _taxonomy_csv_set_line_break($string) {
  return str_replace(
    array('\r', '\n'),
    array("\r", "\n"),
    $string);
}
