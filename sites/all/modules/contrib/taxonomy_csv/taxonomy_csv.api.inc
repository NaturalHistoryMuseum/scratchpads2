<?php
// $Id: taxonomy_csv.api.inc,v 2.1.2.6.2.6 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 * Manage variables and features of module.
 *
 * More infos (schemas of file, use as an api, etc.) in TECHINFO.txt.
 */

/**
 * Available internal import/export schemas.
 */
define('TAXONOMY_CSV_FORMAT_ALONE_TERMS',       'alone_terms');
define('TAXONOMY_CSV_FORMAT_DEFINITION_LINKS',  'def_links');
define('TAXONOMY_CSV_FORMAT_FLAT',              'flat');
define('TAXONOMY_CSV_FORMAT_TREE_STRUCTURE',    'tree_structure');
define('TAXONOMY_CSV_FORMAT_POLYHIERARCHY',     'polyhierarchy');
define('TAXONOMY_CSV_FORMAT_PARENTS',           'parents');
define('TAXONOMY_CSV_FORMAT_CHILDREN',          'children');
define('TAXONOMY_CSV_FORMAT_RELATIONS',         'relations');
define('TAXONOMY_CSV_FORMAT_SYNONYMS',          'synonyms');
define('TAXONOMY_CSV_FORMAT_DEFINITIONS',       'definitions');
define('TAXONOMY_CSV_FORMAT_DESCRIPTIONS',      'descriptions');
define('TAXONOMY_CSV_FORMAT_WEIGHTS',           'weights');
define('TAXONOMY_CSV_FORMAT_FIELDS',            'fields');

/**
 * Available import options.
 */
define('TAXONOMY_CSV_EXISTING_UPDATE',          'update');
define('TAXONOMY_CSV_EXISTING_UPDATE_MERGE',    'update_merge');
define('TAXONOMY_CSV_EXISTING_UPDATE_REPLACE',  'update_replace');
define('TAXONOMY_CSV_EXISTING_IGNORE',          'ignore');
define('TAXONOMY_CSV_EXISTING_IGNORE_CREATE',   'ignore_create');
define('TAXONOMY_CSV_EXISTING_IGNORE_ALL',      'ignore_all');
define('TAXONOMY_CSV_EXISTING_IGNORE_PREVIOUS', 'ignore_previous');
define('TAXONOMY_CSV_EXISTING_IGNORE_QUICK',    'ignore_quick');

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
 * Information about import process. Use too default Drupal constants:
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
 *   - 'import_format'     : internal and external available formats for import.
 *   - 'export_format'     : internal and external available formats for export.
 *   - 'import_default_ui' : default options to import by user interface.
 *   - 'import_default_api': default options to import by api.
 *   - 'export_default_ui' : default options to export by user interface.
 *   - 'export_default_api': default options to export by api.
 *   - 'import_option'     : descriptions of options for existing items.
 *   - 'import_allowed'    : allowed options for existing items when importing.
 *   Lists are completed with external formats.
 *
 * @return
 *   Array of wanted content.
 */
function _taxonomy_csv_values($list) {
  switch ($list) {
    case 'import_format':
      $items = array(
        TAXONOMY_CSV_FORMAT_ALONE_TERMS      => t('Term names (ignore additional columns)'),
        TAXONOMY_CSV_FORMAT_DEFINITION_LINKS => t('Full term definition and links'),
        TAXONOMY_CSV_FORMAT_FLAT             => t('Terms (flat vocabulary)'),
        TAXONOMY_CSV_FORMAT_TREE_STRUCTURE   => t('Hierarchical tree structure or one term by line structure'),
        TAXONOMY_CSV_FORMAT_POLYHIERARCHY    => t('Polyhierarchical structure'),
        TAXONOMY_CSV_FORMAT_PARENTS          => t('First level parents'),
        TAXONOMY_CSV_FORMAT_CHILDREN         => t('First level children'),
        TAXONOMY_CSV_FORMAT_RELATIONS        => t('Related terms'),
        TAXONOMY_CSV_FORMAT_SYNONYMS         => t('Synonyms terms'),
        TAXONOMY_CSV_FORMAT_DEFINITIONS      => t('Full term definitions'),
        TAXONOMY_CSV_FORMAT_DESCRIPTIONS     => t('Term descriptions'),
        TAXONOMY_CSV_FORMAT_WEIGHTS          => t('Term weights'),
        // @todo // Unused in Drupal 6.
        // TAXONOMY_CSV_FORMAT_FIELDS        => t('Fields'),
      );
      break;

    case 'export_format':
      $items = array(
        TAXONOMY_CSV_FORMAT_ALONE_TERMS      => t('Term names'),
        TAXONOMY_CSV_FORMAT_DEFINITION_LINKS => t('Full term definition and links'),
        // @todo, but useless: use alone_terms.
        // TAXONOMY_CSV_FORMAT_FLAT             => t('Terms (flat vocabulary)'),
        // TAXONOMY_CSV_FORMAT_TREE_STRUCTURE   => t('Hierarchical tree structure or one term by line structure'),
        // TAXONOMY_CSV_FORMAT_POLYHIERARCHY    => t('Polyhierarchical structure'),
        TAXONOMY_CSV_FORMAT_PARENTS          => t('First level parents'),
        TAXONOMY_CSV_FORMAT_CHILDREN         => t('First level children'),
        TAXONOMY_CSV_FORMAT_RELATIONS        => t('Related terms'),
        TAXONOMY_CSV_FORMAT_SYNONYMS         => t('Synonyms terms'),
        TAXONOMY_CSV_FORMAT_DEFINITIONS      => t('Full term definitions'),
        TAXONOMY_CSV_FORMAT_DESCRIPTIONS     => t('Term descriptions'),
        TAXONOMY_CSV_FORMAT_WEIGHTS          => t('Term weights'),
        // @todo // Unused in Drupal 6.
        // TAXONOMY_CSV_FORMAT_FIELDS           => t('Fields'),
      );
      break;

    case 'import_default_ui':
      return array(
        'import_format'                 => TAXONOMY_CSV_FORMAT_ALONE_TERMS,
        'keep_order'                    => FALSE,
        'source_choice'                 => 'text',
        'import_delimiter'              => 'comma',
        'import_delimiter_soft_tab_width' => '2',
        'import_delimiter_custom'       => '',
        'import_enclosure'              => 'none',
        'import_enclosure_custom'       => '',
        'check_line'                    => TRUE,
        'check_utf8'                    => TRUE,
        'locale_custom'                 => '',
        'vocabulary_target'             => 'autocreate',
        'vocabulary_id'                 => 'choose_vocabulary',
        'check_hierarchy'               => TRUE,
        'set_hierarchy'                 => 2, // Polyhierarchy.
        'existing_items'                => TAXONOMY_CSV_EXISTING_UPDATE,
        // Specific to relations import.
        'relations_create_subrelations' => FALSE,
        'relations_all_vocabularies'    => FALSE,
        // General options.
        'result_stats'                  => 'result_stats',
        'result_terms'                  => 'result_terms',
        'result_level'                  => 'notices',
        'result_type'                   => 'by_message',
      );

    case 'import_default_api':
      return array(
        'import_format'                 => TAXONOMY_CSV_FORMAT_ALONE_TERMS,
        'keep_order'                    => FALSE,
        'delimiter'                     => ',',
        'enclosure'                     => '',
        'check_line'                    => FALSE,
        'check_utf8'                    => FALSE,
        'locale_custom'                 => '',
        'vocabulary_target'             => 'autocreate',
        'vocabulary_id'                 => 'choose_vocabulary',
        'check_hierarchy'               => TRUE,
        'set_hierarchy'                 => 2, // Polyhierarchy.
        'existing_items'                => TAXONOMY_CSV_EXISTING_UPDATE,
        // Specific to relations import.
        'relations_create_subrelations' => FALSE,
        'relations_all_vocabularies'    => FALSE,
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
        'export_format'                 => TAXONOMY_CSV_FORMAT_ALONE_TERMS,
        'export_vocabulary_id'          => 0,
        'export_delimiter'              => 'comma',
        'export_delimiter_custom'       => '',
        'export_enclosure'              => 'none',
        'export_enclosure_custom'       => '',
        'export_line_ending'            => 'Unix',
        'export_order'                  => 'name',
        'result_duplicates'             => TRUE,
        // Default options of specific imports.
        'def_links_terms_ids'           => 'name_if_needed',
        'def_links_vocabularies_ids'    => 'none',
      );

    case 'export_default_api':
      return array(
        'export_format'                 => TAXONOMY_CSV_FORMAT_ALONE_TERMS,
        'vocabulary_id'                 => 0,
        'delimiter'                     => ',',
        'enclosure'                     => '"',
        'line_ending'                   => 'Unix',
        'order'                         => 'name',
        'result_duplicates'             => FALSE,
        'check_options'                 => FALSE,
        'result_display'                => FALSE,
        // Default options of specific imports.
        'def_links_terms_ids'           => 'name_if_needed',
        'def_links_vocabularies_ids'    => 'none',
      );

    case 'import_option':
      return array(
        TAXONOMY_CSV_EXISTING_UPDATE         => t('Update existing term (avoid duplicate terms)'),
        TAXONOMY_CSV_EXISTING_UPDATE_MERGE   => t('Update (merge) existing term and items if exist else create'),
        TAXONOMY_CSV_EXISTING_UPDATE_REPLACE => t('Update (replace) existing term and items if exist else create'),
        TAXONOMY_CSV_EXISTING_IGNORE         => t('Duplicate existing term'),
        TAXONOMY_CSV_EXISTING_IGNORE_CREATE  => t('Ignore existing term and create a new term'),
        TAXONOMY_CSV_EXISTING_IGNORE_ALL     => t('Ignore existing term and create a new term for each term on the line'),
        TAXONOMY_CSV_EXISTING_IGNORE_QUICK   => t('Ignore existing terms (quicker, but duplicates may be created)'),
      );

    case 'import_allowed':
      $items = array(
        TAXONOMY_CSV_FORMAT_ALONE_TERMS      => array(
          TAXONOMY_CSV_EXISTING_UPDATE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_DEFINITION_LINKS => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
        ),
        TAXONOMY_CSV_FORMAT_FLAT             => array(
          TAXONOMY_CSV_EXISTING_UPDATE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_TREE_STRUCTURE   => array(
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_POLYHIERARCHY    => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
        ),
        TAXONOMY_CSV_FORMAT_PARENTS          => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
//           TAXONOMY_CSV_EXISTING_IGNORE_ALL,
        ),
        TAXONOMY_CSV_FORMAT_CHILDREN         => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
//           TAXONOMY_CSV_EXISTING_IGNORE_ALL,
        ),
        TAXONOMY_CSV_FORMAT_RELATIONS        => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
//           TAXONOMY_CSV_EXISTING_IGNORE_ALL,
        ),
        TAXONOMY_CSV_FORMAT_SYNONYMS         => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_DEFINITIONS      => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_DESCRIPTIONS     => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_WEIGHTS          => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_IGNORE_QUICK,
        ),
        TAXONOMY_CSV_FORMAT_FIELDS           => array(
          TAXONOMY_CSV_EXISTING_UPDATE_MERGE,
          TAXONOMY_CSV_EXISTING_UPDATE_REPLACE,
          TAXONOMY_CSV_EXISTING_IGNORE_CREATE,
        ),
      );
      break;

    default:
      return;
  }

  // Complete some lists with external formats.
  $formats = taxonomy_csv_formats_get();
  foreach ($formats as $format) {
    if (isset($format[$list])) {
      $items[$format['format']] = $format[$list];
    }
  }

  return $items;
}

/**
 * Fetch available external formats.
 *
 * Scan the sub directory "formats" of the module for *.format.inc files.
 * An included file should contain appropriate functions to get infos, to create
 * a vocabulary, to import a line and to export a term.
 *
 * @return
 *   Array of available formats indexed by format name.
 */
function taxonomy_csv_formats_get() {
  static $formats = array();

  if (!$formats) {
    $module_dir = drupal_get_path('module', 'taxonomy_csv');
    $files = file_scan_directory("$module_dir/formats", '/.format.inc$/');

    $formats = array();
    foreach ($files as $filepath => $file) {
      // Use this format only if it works.
      $format_name = preg_replace('/.format.inc$/', '', $file->filename);
      $funcname = "taxonomy_csv_format_{$format_name}";
      if (taxonomy_csv_format_check($format_name, $funcname)) {
        $format = $funcname();
        if (!isset($format['needed_module'])
            || ($format['needed_module'] == '')
            || module_exists($format['needed_module'])) {
          $formats[$format_name] = $format;
        }
      }
    }
  }
  return $formats;
}

/**
 * Helper to get a format.
 *
 * Wrapper of taxonomy_csv_formats_get().
 *
 * @param $format
 *   String. Format to get.
 *
 * @return
 *   Asked format array, FALSE else.
 */
function taxonomy_csv_format_get($format) {
  $formats = taxonomy_csv_formats_get();
  return isset($formats[$format]) ?
    $formats[$format] :
    FALSE;
}

/**
 * Helper to include format files each time it's needed and to check function.
 *
 * @param $format
 *   String. Format to check and include.
 * @param $funcname
 *   (Optional) String. Function to check if needed.
 *
 * @return
 *   TRUE if no error, FALSE else.
 */
function taxonomy_csv_format_check($format, $funcname = '') {
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  $file = "$module_dir/formats/$format.format.inc";
  if (file_exists($file)) {
    include_once($file);
    if ($funcname == '') {
      return TRUE;
    }
    if (function_exists($funcname)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Helper to check if a vocabulary has specific fields.
 *
 * @param $vid
 *   Vocabulary id to check.
 * @param $format
 *   (Optional) String. If set, check only only for this format.
 *
 * @return
 *   Array of all formats of this specific vocabulary.
 */
function taxonomy_csv_format_check_vocabulary($vid, $format = '') {
  $vocabulary_formats = array();

  $formats = empty($format) ?
    taxonomy_csv_formats_get() :
    array($format);

  foreach ($formats as $format) {
    $funcname = "taxonomy_csv_vocabulary_check_{$format['format']}";
    if (function_exists($funcname) && $funcname($vid)) {
      $vocabulary_formats[] = $format['format'];
    }
  }
  return $vocabulary_formats;
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
