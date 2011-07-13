<?php
// $Id: taxonomy_csv.import.api.inc,v 1.1.2.5 2011/01/06 18:08:27 danielkm Exp $

/**
 * @file
 * Validate import options and manage import process.
 */

/**
 * Invoke associated include file.
 * taxonomy_csv.import.admin.inc is invoked only if user wants to check input
 * options.
 * taxonomy_csv.import.result.inc is invoked only if user wants result messages.
 */
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  require_once("$module_dir/taxonomy_csv.api.inc");
  require_once("$module_dir/import/taxonomy_csv.import.parser.api.inc");
  require_once("$module_dir/import/taxonomy_csv.import.line.api.inc");
  require_once("$module_dir/taxonomy_csv.vocabulary.api.inc");
  require_once("$module_dir/taxonomy_csv.term.api.inc");

/**
 * Process the import of an input.
 *
 * If not used in a form, don't forget to use batch_process().
 *
 * @param $options
 *   An associative array of options:
 *   - file           : object file if file is already uploaded
 *   - text           : csv text to import
 *   - url            : url to distant or local csv file
 *   - path           : path to local csv file
 *   - import_format  : see _taxonomy_csv_values (default: 'alone_terms')
 *   - keep_order     : boolean. keep order of imported terms or not (default)
 *   - delimiter      : 1 character csv delimiter (default: ',')
 *   - enclosure      : 0 or 1 character csv enclosure (default: none or '"')
 *   - check_line     : boolean. check or not (default) format of lines
 *   - check_utf8     : boolean. check or not (default) utf8 format
 *   - locale_custom  : string. specific locale of imported file
 *   - vocabulary_target: 'autocreate' (default), 'duplicate' or 'existing'
 *   - vocabulary_id  : vocabulary id to duplicate or to import into
 *   - check_hierarchy: boolean. check (default) or not vocabulary hierarchy
 *   - set_hierarchy  : if hierarchy isn't checked, set it (0, 1 or 2 (default))
 *   - existing_items : see _taxonomy_csv_values (default: 'update')
 *   // Specific to relations import:
 *   - relations_create_subrelations: boolean.
 *   - relations_all_vocabularies   : boolean.
 *   // Level of result process infos.
 *   - check_options  : boolean. check or not (default) this array of options
 *   - result_display : boolean. display or not (default). Only used with UI.
 *   - result_stats   : boolean. display or not (default) stats
 *   - result_terms   : boolean. display or not (default) list of imported terms
 *   - result_level   : log: 'first' (default), 'warnings', 'notices' or 'infos'
 *   - result_type    : display log 'by_message' (default) or 'by_line'
 *   Only one option is required: file or text or url or path. Other options
 *   have default values. Warning: default values are different with UI.
 *
 * @return
 *   Array of errors or nothing (need to execute batch process; result is logged
 *   in watchdog).
 */
function taxonomy_csv_import($options) {
  // Complete $options with default values if needed.
  // Default api and UI options are different.
  $options += _taxonomy_csv_values('import_default_api');

  // Preload text or file in order to check access to temporary folder.
  $messages = _taxonomy_csv_import_input_preload($options);
  if (count($messages)) {
    return $messages;
  }

  // Process import.
  taxonomy_csv_vocabulary_import($options);
}

/**
 * Preload input.
 *
 * Automatically determinate source choice (file, text, path or url).
 * Then check if there is write access and prepare file.
 * To use a file is not optimal with array and text input, but this solution
 * unifies input and avoids some memory crashes.
 *
 * @param $options
 *   Array of options.
 *
 * @return
 *   Array of messages errors if any.
 *   By reference options are cleaned and completed.
 */
function _taxonomy_csv_import_input_preload(&$options) {
  $messages = array();

  // File import. Use to with UI import in a second time.
  if (isset($options['file']) && is_object($options['file'])) {
    // Ready.
  }
  // Text import.
  elseif (isset($options['text']) && $options['text'] != '') {
    // Prepare import by text: save text as a temp file to simplify process.
    $filename = file_unmanaged_save_data(
      $options['text'],
      'public://taxocsv.csv',
      'FILE_EXISTS_RENAME');
    $options['file'] = (object) array(
      'filename' => basename($filename),
      'filepath' => drupal_realpath($filename),
      'filesize' => filesize($filename)
    );
  }
  // Url import.
  elseif (isset($options['url']) && $options['url'] != '') {
    $filename = file_unmanaged_save_data(
      file_get_contents($options['url']),
      'public://taxocsv.csv',
      'FILE_EXISTS_RENAME');
    $options['file'] = (object) array(
      'filename' => basename($filename),
      'filepath' => drupal_realpath($filename),
      'filesize' => filesize($filename),
    );
  }
  // Path import. With UI, path is already uploaded as a file.
  elseif (isset($options['path']) && $options['path'] != '') {
    // Load source local file.
    $options['file'] = file_save_upload($options['path']);
    // fopen and fseek need a real path.
    if ($options['file']) {
      $options['file']->filepath = drupal_realpath($options['file']->uri);
    }
  }
  else {
    $messages['source_choice'] = t('Source choice should be a text, an url or a file path and should not be empty.');
    return $messages;
  }

  // Check file.
  if (!is_object($options['file'])
      || !$options['file']->filesize) {
    $messages['file'] = t('Import failed.') . '<br />'
      . t('- Check size of your input : it cannot be null.') . '<br />'
      . t('- Check your server configuration and your rights : server needs permission to access and to read file.') . '<br />'
      . t('- Check access rights to temp directory : import needs permission to write and to read in it.');
    return $messages;
  }

  // Check if input format is utf8, if file has no bom and convert it if needed.
  if (!_taxonomy_csv_import_clean_utf8($options['file'], $options['check_utf8'])) {
    $messages['file'] = t("Your file is not utf-8 formatted. Possible solutions below.") . '<br />'
      . t('- Convert your file to utf-8.') . '<br />'
      . t('- Install iconv, GNU recode or mbstring for PHP.') . '<br />'
      . t('- Disable "Check utf-8" option.');
    return $messages;
  }

  return $messages;
}

/**
 * Helper function to check if file is utf8 encoded and to remove bom if needed.
 *
 * See http://drupal.org/node/364832.
 * This function remove utf8 byte order mark if needed.
 *
 * @param $file
 *   By reference file object to check.
 *
 * @param $check_utf8
 *   Boolean. Check utf8 format of the file (default) or not.
 *
 * @return
 *   TRUE if input is utf8 formatted or FALSE else. File is updated if needed.
 */
function _taxonomy_csv_import_clean_utf8(&$file, $check_utf8 = TRUE) {
  $content = file_get_contents($file->filepath, TRUE);
  $flag = FALSE;

  // Check encoding.
  if ($check_utf8) {
    if (!function_exists('mb_detect_encoding')) {
      return FALSE;
    }
    $enc = mb_detect_encoding($content, 'UTF-8, ISO-8859-1, ISO-8859-15', TRUE);
    if ($enc != 'UTF-8') {
      $content = drupal_convert_to_utf8($content, $enc);
      if (!$content) {
        $messages[] = 320; // Error convert.
        return FALSE;
      }
      $flag = TRUE;
    }
  }

  // Skip eventual UTF-8 byte order mark.
  if (strncmp($content, "\xEF\xBB\xBF", 3) === 0) {
    $content = substr($content, 3);
    $flag = TRUE;
  }

  // Update content in file if needed.
  if ($flag) {
    $filename = file_unmanaged_save_data(
      $content,
      $file->uri,
      'FILE_EXISTS_REPLACE');
    $file = (object) array(
      'filename' => basename($filename),
      'filepath' => drupal_realpath($filename),
      'filesize' => filesize($filename),
    );
  }
  return TRUE;
}

/**
 * Helper to check if delimiter is a soft tab and to prepare file if needed.
 *
 * @param $file
 *   By reference file object to check.
 *
 * @param $delimiter
 *   String. Soft tab delimiter to switch with a true tab.
 *
 * @return
 *   TRUE if delimiter isn't a soft tab or if file has been updated.
 *   FALSE if file can't be updated with a true tab (currently not used).
 */
function _taxonomy_csv_import_soft_tab(&$file, $delimiter) {
  if (drupal_strlen($delimiter) > 1) {
    $content = file_get_contents($file->filepath, TRUE);

    // Switch soft tab delimiter with a true tab.
    $content = str_replace($delimiter, "\t", $content);

    // Save updated file.
    $filename = file_unmanaged_save_data(
      $content,
      $file->uri,
      'FILE_EXISTS_REPLACE');
    $file = (object) array(
      'filename' => basename($filename),
      'filepath' => drupal_realpath($filename),
      'filesize' => filesize($filename),
    );
  }
  return TRUE;
}

/**
 * Prepare the import of a vocabulary.
 * If not used in a form, don't forget to use batch_process().
 *
 * @param $options
 *   Array. Same as taxonomy_csv_import. See above.
 *
 * @return
 *   Array of errors or nothing (batch process to execute).
 */
function taxonomy_csv_vocabulary_import($options) {
  // Check options and return array of messages in case of errors.
  if ($options['check_options']) {
    $module_dir = drupal_get_path('module', 'taxonomy_csv');
    require_once("$module_dir/import/taxonomy_csv.import.admin.inc");
    $result = _taxonomy_csv_import_check_options($options);
    if (count($result)) {
      return $result;
    }
  }

  // Complete $options.
  // Switch soft tab delimiter with a true one if needed.
  if (drupal_strlen($options['delimiter']) > 1) {
    $result = _taxonomy_csv_import_soft_tab($options['file'], $options['delimiter']);
    $options['delimiter'] = "\t";
  }

  // Calculates number of lines to be imported. File is already checked.
  $options['total_lines'] = count(file($options['file']->filepath));

  // Prepare vocabularies.
  $name = _taxonomy_csv_import_vocabulary_prepare($options);

  // Set locale if needed.
  // See http://drupal.org/node/872366
  $options['locale_previous'] = setlocale(LC_CTYPE, 0);
  if ($options['locale_custom']) {
    setlocale(LC_CTYPE, $options['locale_custom']);
  }

  // Prepare import batch.
  // Use a one step batch in order to avoid memory crash in case of big import.
  $batch = array(
    'title'            => ($name != '') ?
        t('Importing !total_lines lines from CSV file "%filename"...', array(
          '%filename'    => $name,
          '!total_lines' => $options['total_lines'])) :
        t('Importing !total_lines lines from text...', array(
          '!total_lines' => $options['total_lines'])),
    'init_message'     => t('Starting uploading of datas...'),
    'progress_message' => '',
    'error_message'    => t('An error occurred during the import.'),
    'finished'         => '_taxonomy_csv_vocabulary_import_finished',
    'file'             => drupal_get_path('module', 'taxonomy_csv') . '/import/taxonomy_csv.import.api.inc',
    'progressive'      => TRUE,
    'operations'       => array(
      0 => array('_taxonomy_csv_vocabulary_import_process', array($options)),
    ),
  );

  batch_set($batch);
}

/**
 * Batch process of vocabulary import.
 *
 * @param $options
 *   Array of batch options.
 * @param &$context
 *   Batch context to keep results and messages.
 *
 * @return
 *   NULL because use of &$context.
 */
function _taxonomy_csv_vocabulary_import_process($options, &$context) {
  // First callback.
  if (empty($context['sandbox'])) {
    // Remember options as batch_set can't use form_storage.
    // It allows too that first line in result is numbered 1 and not 0.
    $context['results'][0] = $options;

    // Automatically detect line endings.
    ini_set('auto_detect_line_endings', '1');

    // Initialize some variables.
    $context['results'][0]['current_line'] = 0;
    $context['results'][0]['worst_line'] = 0;
    $context['results'][0]['worst_message'] = 799;
    $context['results'][0]['handle'] = fopen($options['file']->filepath, 'r');
    $context['sandbox']['handle_pointer'] = 0;
    $context['sandbox']['max'] = $options['total_lines'];
    $context['sandbox']['previous_items'] = array(
      'name' => array(),
      'tid'  => array(),
    );
    $context['sandbox']['terms_count'] = 0;
  }
  elseif (!is_resource($context['results'][0]['handle'])) {
    // Recall file and set pointer in case of memory or time out.
    $context['results'][0]['handle'] = fopen($options['file']->filepath, 'r');
    fseek($context['results'][0]['handle'], $context['sandbox']['handle_pointer']);
  }

  // Load and process one line.
  $worst_line     = &$context['results'][0]['worst_line'];
  $worst_message  = &$context['results'][0]['worst_message'];
  $handle         = &$context['results'][0]['handle'];
  $line_number    = &$context['results'][0]['current_line'];
  $previous_items = &$context['sandbox']['previous_items'];
  $terms_count    = &$context['sandbox']['terms_count'];

  // To set locale is needed with fgetcsv and it's needed each time this
  // function is called. See http:/php.net/manual/en/function.fgetcsv.php.
  // See http://drupal.org/node/872366
  // @todo Use of preg_split?
  if ($options['locale_custom']) {
    setlocale(LC_CTYPE, $options['locale_custom']);
  }
  $line = fgetcsv($handle, 4096, $options['delimiter'], $options['enclosure']);

  if ($line) {
    ++$line_number;

    // Remember pointer in case of memory or time out.
    $context['sandbox']['handle_pointer'] = ftell($handle);

    // Process import of current line.
    $result = taxonomy_csv_line_import_process($line, $options, $previous_items, $terms_count);

    // Remember processed line.
    $previous_items['name'] = $result['name'];
    $previous_items['tid']  = $result['tid'];
    $terms_count            = $result['terms_count'];

    // Remember worst message of imported lines.
    $worst_message_new = _taxonomy_csv_worst_message($result['msg']);
    if ($worst_message > $worst_message_new) {
      $worst_message = $worst_message_new;
      $worst_line    = $line_number;
    };

    // Remember only wanted messages.
    if (($options['result_stats'] || $options['result_terms'])
        && $result['tid']) {
      // Some formats use $result to save other infos, so it needs to be cleaned
      // before to be saved.
      $format = taxonomy_csv_format_get($options['import_format']);
      if (($options['import_format'] == TAXONOMY_CSV_FORMAT_DEFINITION_LINKS)
          && isset($result['tid']['term'])) {
        // Def_links saved all terms each times in a sub array.
        $context['results'][0]['imported_terms'][0] = array_values($result['tid']['term']);
      }
      elseif ($format && $format['import_previous']) {
        // Don't remember previous previous items.
        $context['results'][0]['imported_terms'][0] = array_values($result['tid']);
      }
      else {
        $context['results'][0]['imported_terms'][$line_number] = $result['tid'];
      }
    }
    if ($options['result_level'] != 'first') {
      $list_messages = array();
      switch ($options['result_level']) {
        // case 'first':
        //   break;
        case 'warnings':
          foreach ($result['msg'] as $msg) {
            if ($msg < TAXONOMY_CSV_PROCESS_NOTICE) {
              $list_messages[] = $msg;
            }
          }
          break;
        case 'notices':
          foreach ($result['msg'] as $msg) {
            if ($msg < TAXONOMY_CSV_PROCESS_INFO) {
              $list_messages[] = $msg;
            }
          }
          break;
        case 'infos':
          $list_messages = $result['msg'];
          break;
      }
      if (count($list_messages)) {
        $context['results'][$line_number] = $list_messages;
      }
    }

    // Inform about progress.
    $context['message'] = t('Line !line_number of !total_lines processed: !line', array(
      '!line_number' => $line_number,
      '!total_lines' => $options['total_lines'],
      '!line'        => implode(', ', $line),
    ));

    // Check worst message of imported lines and update progress.
    if ($worst_message >= TAXONOMY_CSV_PROCESS_WARNING) {
      // Count should be <= 0.99 to avoid batch stop before end (Drupal 7 bug).
      $context['finished'] = floor($line_number / $context['sandbox']['max'] * 100) / 100;
    }
    else {
      $context['finished'] = 1;
    }
  }

  // With some formats, remember vocabularies to be checked.
  if (in_array($options['import_format'], array(
      TAXONOMY_CSV_FORMAT_DEFINITION_LINKS,
    ))) {
    $context['results'][0]['vocabularies'] = $previous_items['tid']['vocabulary'];
  }
}

/**
 * Callback for finished batch import and display result informations.
 */
function _taxonomy_csv_vocabulary_import_finished($success, $results, $operations) {
  // $results[0] is used to save options and some infos (imported terms), as
  // batch process can't use $form_state.
  $options = &$results[0];
  unset($results[0]);

  // Close imported file.
  if ($options['handle']) {
    fclose($options['handle']);
  }

  // Set previous locale if needed.
  if ($options['locale_custom']) {
    setlocale(LC_CTYPE, $options['locale_previous']);
  }

  // Invoke import stats file if user wants to display texts for result.
  if ($options['result_display']) {
    $module_dir = drupal_get_path('module', 'taxonomy_csv');
    require_once("$module_dir/import/taxonomy_csv.import.result.inc");
  }

  // Short summary information is different if batch succeeded or not.
  if ($success) {
    $variables = array(
      '!line'        => $options['worst_line'],
      '!total_lines' => $options['total_lines'],
      '!worst_msg'   => $options['result_display'] ?
          _taxonomy_csv_message_text($options['worst_message']) :
          t('Message code') . ' = ' . $options['worst_message'],
    );
    $messages = array(
      WATCHDOG_DEBUG   => t('No error, warnings or notices have been reported during import process of !total_lines lines.', $variables),
      WATCHDOG_INFO    => t('No error, warnings or notices have been reported during import process of !total_lines lines.', $variables),
      WATCHDOG_NOTICE  => t('Notices have been reported during import process (bad formatted or empty lines). !total_lines lines processed. First notice occurred on line !line [!worst_msg].', $variables),
      WATCHDOG_WARNING => t('Warnings have been reported during import process (bad formatted lines). !total_lines lines processed. First line skipped is line !line [!worst_msg].', $variables),
      WATCHDOG_ERROR   => t('Errors have been reported during import process. Process failed at line !line of a total of !total_lines [!worst_msg].', $variables),
    );
    $worst_level = intval($options['worst_message'] / 100);
    $message = $messages[$worst_level];
  }
  else {
    $message = t('Importation failed. Import process was successful until the line !line_count of a total of !total_lines. You can first check your file on this line and check file uploading.', array(
        '!line_count'  => $options['current_line'],
        '!total_lines' => $options['total_lines'],
      )) . '<br />'
      . t('This issue is related to import process or to size import and probably not to content. You can disable hierarchy check and reduce log level. You can divide your import file into lighter files. You can increase php and sql memory. If problem does not disappear, you can reinstall module from a fresh release or submit an issue on <a href="!link">Taxonomy CSV import/export module</a>.', array(
        '!link' => url('http://drupal.org/project/issues/taxonomy_csv/'),
      ));
    $worst_level = WATCHDOG_ERROR;
  }

  // Set result message in watchdog and eventually in user interface.
  // Use of a $message variable is unrecommended, but simpler and working.
  // See http://drupal.org/node/323101
  watchdog('taxonomy_csv', $message, NULL, $worst_level);
  if ($options['result_display']) {
    _taxonomy_csv_import_result($options, $worst_level, $message, $results);
  }
}

/**
 * Prepare a vocabulary for import.
 *
 * @param $options
 *   Array of batch options.
 *
 * @return
 *   Name of the prepared vocabulary.
 */
function _taxonomy_csv_import_vocabulary_prepare(&$options) {
  // Use name of file or url as vocabulary name.
  if (isset($options['url']) && $options['url'] != '') {
    $name = basename($options['url']);
  }
  elseif (isset($options['text']) && $options['text'] != '') {
    $name = '';
    // Remove useless option, because text is now saved.
    $options['text'] = '';
  }
  else {
    $name = $options['file']->filename;
  }

  // With some formats (def_links), vocabularies are created during import.
  if (in_array($options['import_format'], array(
      TAXONOMY_CSV_FORMAT_DEFINITION_LINKS,
    ))) {
    return $name;
  }
  // Else.

  // Create, duplicate or use an existing vocabulary.
  switch ($options['vocabulary_target']) {
    case 'autocreate':
      $vocabulary = taxonomy_csv_vocabulary_create($name);
      $options['vocabulary_id'] = $vocabulary->vid;
      break;

    case 'duplicate':
      $vocabulary = taxonomy_csv_vocabulary_duplicate($options['vocabulary_id']);
      $options['vocabulary_id'] = $vocabulary->vid;
      break;

    case 'existing':
      $vocabulary = taxonomy_vocabulary_load($options['vocabulary_id']);
      break;
  }

  // Prepare default synonyms field if needed.
  $synonym_field = (in_array($options['import_format'], array(
      TAXONOMY_CSV_FORMAT_DEFINITIONS,
      TAXONOMY_CSV_FORMAT_SYNONYMS,
    ))) ?
    array() :
    FALSE;

  // Prepare default relations field if needed.
  $relation_field = (in_array($options['import_format'], array(
      TAXONOMY_CSV_FORMAT_RELATIONS,
    ))) ?
    array() :
    FALSE;

  // Create thesaurus synonyms and relations fields if needed.
  $options['vocabulary'] = taxonomy_csv_thesaurus_create($vocabulary->name, $synonym_field, $relation_field);

  // Check external formats. Use it only if it works.
  $funcname = "_taxonomy_csv_import_vocabulary_prepare_{$options['import_format']}";
  if (taxonomy_csv_format_check($options['import_format'], $funcname)) {
    $result = $funcname($vocabulary);
  }

  return $vocabulary->name;
}

/**
 * Import a line that contains a term and other items matching the options.
 *
 * @param $line
 *   Array which contains items of a csv line.
 * @param $options
 *   An associative array of import options:
 *   - import_format : see _taxonomy_csv_values('import_format')
 *   - keep_order    : boolean. keep order of imported terms or not (default)
 *   - vocabulary_id : vocabulary id to import into
 *   - existing_items: see _taxonomy_csv_values('import_option')
 *   - check_line    : boolean. Tweak to check (default) or not format of lines
 *   - check_utf8    : boolean. Tweak to check (default) or not utf8 format
 *   // Specific to relations import:
 *   - relations_create_subrelations: boolean.
 *   - relations_all_vocabularies   : boolean.
 * @param $previous_items
 *   (Optional) Cleaned and checked previous imported line names and tids array.
 *   Needed with some contents as one term array structure or Taxonomy manager.
 *   Specificities:
 *   - taxonomy_manager:
 *       tid array is indexed with old tids.
 *   - def_links:
 *       'name' and 'tid' arrays contain sub-arrays 'vocabulary' and 'term' with
 *       imported identifiant as key and name or tid/vid as value in order to
 *       allow duplicate names import and use of number identifiants.
 * @param $terms_count
 *   (Optional integer) Total of imported terms (duplicate included) is needed
 *   to set weight of terms and to keep order of items, if wished.
 *
 * @return Result array:
 *   'name' => array of imported terms names,
 *   'tid'  => array of imported terms tids,
 *   'msg'  => messages arrays:
 *     term position => array of status messages of term,
 *     'line'        => array of status messages of line,
 *   'terms_count' => Total of imported terms.
 */
function taxonomy_csv_line_import_process($line, $options, $previous_items = array(), $terms_count = 0) {
  // Define default values.
  $line_messages = array();
  $items_messages = array();
  $result = array(
    'name' => array(),
    'tid'  => array(),
    'msg'  => array(),
    'terms_count' => $terms_count,
  );

  // 1. Validate and clean line.
  if ($options['check_line']) {
    $line = _taxonomy_csv_line_import_clean(
      $line,
      $line_messages);

    // 2. Check items of line if no error occurs and if line is not empty.
    if ((_taxonomy_csv_worst_message($line_messages) >= TAXONOMY_CSV_PROCESS_NOTICE)
        && ((count($line_messages) == 0) || ($line_messages[0] != 696))) {
      $line = _taxonomy_csv_line_import_check(
        $line,
        $options,
        $previous_items,
        $items_messages);
      $line_messages = array_merge($line_messages, $items_messages);

      // 3. Process import items with full checked line.
      if (_taxonomy_csv_worst_message($items_messages) >= TAXONOMY_CSV_PROCESS_NOTICE) {
        $result = taxonomy_csv_line_import(
          $line,
          $options,
          $previous_items,
          $terms_count);
        // Add line level message of bad or successful import.
        $line_messages[] = (_taxonomy_csv_worst_message($result['msg']) >= TAXONOMY_CSV_PROCESS_NOTICE) ? 699 : 499;
      }
    }
  }
  else {
    // No checks, so directly import line after a simple trim.
    $result = taxonomy_csv_line_import(
      array_values(array_map('trim', $line)),
      $options,
      $previous_items,
      $terms_count);
    // Add line level message of bad or successful import.
    $line_messages[] = (_taxonomy_csv_worst_message($result['msg']) >= TAXONOMY_CSV_PROCESS_NOTICE) ? 699 : 499;
  }

  // Keep previous items in case of an empty or an unprocessed line.
  if (count($result['name']) == 0) {
    $result['name'] = $previous_items['name'];
    $result['tid']  = $previous_items['tid'];
  }

  // Add line level messages and clean result.
  $result['msg'] = array_unique(array_merge($result['msg'], $line_messages));
  sort($result['msg']);

  return $result;
}

/**
 * Helper function to clean an imported line.
 *
 * @todo To be simplified.
 *
 * @param $line
 *   Array of items to be processed.
 * @param &$messages
 *   (Optional) By reference array of messages codes to be returned.
 *
 * @return Array of cleaned imported line.
 */
function _taxonomy_csv_line_import_clean($line, &$messages = array()) {
  $cleaned_line = array();

  // Example: string "Term 1".
  if (!is_array($line)) {
    $messages[] = 310; // Error not a line array.
  }
  // Example: " " or unrecognized line.
  elseif ((count($line) == 0)
      || empty($line)
      || ((count($line) == 1) && (trim($line[0]) == ''))) {
    $messages[] = 696; // Info empty line.
  }
  else {
    $cleaned_line = $line;

    foreach ($line as $item) {
      if (!drupal_validate_utf8($item)) {
        $messages[] = 321; // Error validate.
      }
    }

    // Trim and check empty line: useful for some non-Ascii lines.
    $line = array_map('trim', $cleaned_line);

    // @todo To simplify.
    // Example: " , , ".
    $test_line = array_unique($line);
    if (count($test_line) == 1 && in_array('', $test_line)) {
      $messages[] = 491; // Warning no item.
    }
    else {
      $cleaned_line = array_values($line);
    }
  }

  return $cleaned_line;
}
