<?php

/**
 * @file
 * Functions that are used to import and export biblio data.
 *
 */
/*   biblio.import.export.inc
 *
 *   Copyright (C) 2006-2011  Ron Jerome
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
/**
 * Return a form select box populated with all the users of the site.
 *
 * @param $my_uid
 *   The user id of the person accessing the form so the select box defaults
 *   to their userid
 * @return
 *   An array which will be used by the form builder to add a select box to a form
 */
function _biblio_admin_build_user_select($my_uid) {
  $sql = 'SELECT DISTINCT u.uid, u.name, u.status, u.mail FROM {users} u  WHERE u.uid <> 0 ';
  $result = db_query($sql);
  foreach ($result as $user) {
    $users[$user->uid] = $user->name . " ($user->mail)";
  }
  asort($users);
  $select = array(
    '#type' => 'select',
    '#title' => t("Set user ID of entries in this file to"),
    '#options' => $users,
    '#default_value' => $my_uid,
    '#disabled' => (user_access('administer biblio')) ?  FALSE : TRUE
  );
  return $select;
}
/**
 * Return a form used to import files into biblio.
 *
 * @return
 *   An array which will be used by the form builder to build the import form
 */
function biblio_import_form($form, &$form_state) {
  global $user;
  $msg = '';
  $biblio_vocabs = array();

  if (biblio_access('import')) { // && !user_access('administer nodes')) {
    $form['#attributes']['enctype'] = 'multipart/form-data';
    $form['biblio_import_file'] = array(
        '#type' => 'file',
        '#title' => t('Import file'),
        '#default_value' => '',
        '#size' => 60
    );

    $import_options = module_invoke_all('biblio_import_options');
    if (count($import_options) > 1) {
      $form['filetype'] = array(
          '#type' => 'select',
          '#title' => t('File Type'),
          '#default_value' => 0,
          '#options' => array(
              '0' => t('Select type'),
          )
      );

      $form['filetype']['#options'] = array_merge($form['filetype']['#options'], $import_options);
      asort($form['filetype']['#options']);
    }
    elseif (count($import_options) == 1) {
      $form['biblio_import_file']['#description'] = t('Import type: @option', array('@option' => current($import_options)));
      $form['filetype'] = array(
          '#type' => 'value',
          '#value' => key($import_options),
      );
    }
    elseif (count($import_options) == 0) {
      $form['biblio_import_file']['#disabled'] = TRUE;
      drupal_set_message(t("You won't be able to select a file until you enable at least one import module."), 'error');

    }

    $form['batch_process'] = array(
        '#type' => 'checkbox',
        '#title' => t('Batch Process'),
        '#default_value' => 1,
        '#description' => t('You should use batch processing if your import file contains more than about 20 records, or if you are experiencing script timeouts during import'),
    );
    $form ['userid'] = _biblio_admin_build_user_select($user->uid);
    // Get the vocabularies  attached to the biblio node type ...
    foreach (field_info_instances('node', 'biblio') as $instance) {
      $field = field_info_field_by_id($instance['field_id']);
      if ($field['type'] == 'taxonomy_term_reference') {
        foreach ($field['settings']['allowed_values'] as $delta => $tree) {
          $biblio_vocabs[$tree['vocabulary']] = array('instance' => $instance, 'field' => $field);
        }
      }
    }

    // ... and print a form to select the terms in each of them
    $form['import_taxonomy'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Taxonomy Settings'),
        '#description' => t('Typically you don\'t have to do anything here, however if you wish, you may select terms to be assigned to imported records. This effectively adds a keyword to all entries being imported.'));
    if (count($biblio_vocabs)) {
      $vocabularies = module_invoke('taxonomy', 'get_vocabularies', 'biblio');
      if (variable_get('biblio_keyword_freetagging', 0)) {
        $freetag_vocab = $vocabularies[variable_get('biblio_keyword_vocabulary', 0)];
        unset($vocabularies[variable_get('biblio_keyword_vocabulary', 0)]);
        $msg = t('<b>NOTE:</b> Keyword "free tagging" is turned on, consequently all incomming keywords will be added to the <b>@name</b> vocabulary as specified in the "Keyword" section of the !url page.', array('@name' => $freetag_vocab->name, '!url' => l(t('admin/config/content/biblio'), 'admin/config/content/biblio')));
      }
      else {
        $msg = t('<b>NOTE:</b> Keyword "free tagging" is turned off, consequently keywords will <b>NOT</b> be added to the vocabulary as specified in the Taxonomy section of the !url page.', array('!url' => l(t('admin/config/content/biblio'), 'admin/config/content/biblio')));
      }
      $i = 0;
      $form += array('#parents' => array());
      $form['import_taxonomy']['vocabularies'] = array();
      $term_refs = array();

      foreach ($vocabularies as $vocabulary) {
        if (in_array($vocabulary->machine_name, array_keys($biblio_vocabs))) {
          $term_refs[] = $biblio_vocabs[$vocabulary->machine_name]['instance']['field_name'];
          $entity = new stdClass();
          $entity->type = 'biblio';
          $field = $biblio_vocabs[$vocabulary->machine_name]['field'];
          $instance = $biblio_vocabs[$vocabulary->machine_name]['instance'];
          $items = array();
          $form['import_taxonomy']['vocabularies'] += field_default_form('node', $entity, $field, $instance, 'und', $items, $form, $form_state);
        }
      }
      if (!empty($term_refs)) {
        $form['term_refs'] = array(
            '#type' => 'hidden',
            '#value' => $term_refs,
        );
        $form['import_taxonomy']['copy_to_biblio'] = array(
            '#type' => 'checkbox',
            '#title' => t('Copy these terms to the biblio keyword database'),
            '#return_value' => 1,
            '#default_value' =>  variable_get('biblio_copy_taxo_terms_to_keywords', 0),
            '#description' => t('If this option is selected, the selected taxonomy terms will be copied to the ' . check_plain(variable_get('biblio_base_title', 'Biblio')) . ' keyword database and be displayed as keywords (as well as taxonomy terms) for this entry.')
        );
      }
    }
    else {
      if (module_exists('taxonomy')) {
        $vocab_msg = t('There are currently no "Term references" attached to the biblio node type. If you would like to associate a Taxonomy vocabulary with the Biblio node type, go the the !url page and add one or more "Term reference" fields.', array('!url' => l(t('admin/structure/types/manage/biblio/fields'), 'admin/structure/types/manage/biblio/fields')));
      }
      else{
        $vocab_msg = '<div class="admin-dependencies">' . t('Depends on') . ': ' . t('Taxonomy') . ' (<span class="admin-disabled">' . t('disabled') . '</span>)</div>';

      }
      $form['import_taxonomy']['vocabulary_message'] = array(
          '#markup' => '<p><div>' . $vocab_msg . '</div></p>'
      );
    }
    $form['import_taxonomy']['freetagging_information'] = array(
        '#markup' => '<p><div>' . $msg . '</div></p>'
    );
    $form['button'] = array('#type' => 'submit', '#value' => t('Import'));
    return $form;
  }
  else {
    drupal_set_message(t("You are not authorized to access the biblio import page"), 'error');
  }
}

/**
 * Implementation of hook_validate() for the biblio_import_form.
 */
function biblio_import_form_validate($form, & $form_state) {
  $op = $form_state['values']['op'];
  $filetype = isset($form_state['values']['filetype']) ? $form_state['values']['filetype'] : 0;
  if ($error = isset($_FILES['files']) ? $_FILES['files']['error']['biblio_import_file'] : '') {
    switch ($error) {
      case 1: form_set_error('biblio_import_form', t("The uploaded file exceeds the upload_max_filesize directive in php.ini."));
      break;
      case 2: form_set_error('biblio_import_form', t("The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form."));
      break;
      case 3: form_set_error('biblio_import_form', t("The uploaded file was only partially uploaded."));
      break;
      case 4: form_set_error('biblio_import_form', t("No file was uploaded."));
      break;
      case 6: form_set_error('biblio_import_form', t("Missing a temporary folder."));
      break;
      case 7: form_set_error('biblio_import_form', t("Failed to write file to disk."));
      break;
      case 8: form_set_error('biblio_import_form', t("File upload stopped by extension."));
    }
  }

  if ($op == t('Import') && $filetype == "0") {
    form_set_error('biblio_import_form', t("You did not select the file type"));
  }
}
/**
 * Implementation of hook_submit() for the biblio_import_form.
 */
function biblio_import_form_submit($form, & $form_state) {
  global $user;
  $batch_proc = FALSE;
  $extensions = 'xml bib enw mrc ris txt';
  $validators['file_validate_extensions'] = array();
  $validators['file_validate_extensions'][0] = $extensions;
  if ($form_state['values']['op'] == t('Import') && isset ($form_state['values']['filetype'])) {
    if ($import_file = file_save_upload('biblio_import_file', $validators)) {
      if ($form_state['values']['batch_process'] == 1) {
        $batch_proc = TRUE;  // we will use batch import for larger files.
      }
      // Concatenate all the terms of the different vocabularies
      // in a single array to be sent to biblio_import
      $terms = array();
      if (isset($form_state['values']['term_refs'])) {
        foreach ($form_state['values']['term_refs'] as $key) {
          if (isset($form_state['values'][$key]) && !empty($form_state['values'][$key])) {
            foreach ($form_state['values'][$key]['und'] as $id => $item) {
              if (!is_array($item) || (empty($item['tid']) && (string) $item['tid'] !== '0')) {
                unset($form_state['values'][$key]['und'][$id]);
              }
            }
            if (empty($form_state['values'][$key]['und'])) {
              unset($form_state['values'][$key]);
            }
            else {
              $terms[$key] = $form_state['values'][$key];
            }
          }
          //       }

          //FIXME     if (count($terms) && $key == 'copy_to_biblio') $terms['copy_to_biblio'] = $form_state['values'][$key];
        }
      }

      // Added the $terms argument
      // the array of terms to be attached to the node(s)
      $userid = (isset($form_state['values']['userid'])) ? $form_state['values']['userid'] : $user->uid;
      $filetype = $form_state['values']['filetype'];
      $filesize = sprintf("%01.1f", $import_file->filesize / 1000);
      $filesize = " ($filesize KB)";
      if ($batch_proc) {
        $session_id = microtime();
        $batch_op = array(
          'title' => t('Importing @filename', array('@filename' => $import_file->filename . $filesize)),
          'operations' => array(
            array('biblio_import', array($import_file, $filetype, $userid, $terms, $batch_proc, $session_id)),
            array('biblio_import_batch_operations', array($session_id, $user, $userid, $terms))
          ),
          'progressive' => TRUE,
          'finished' => 'biblio_import_batch_finished',
          'init_message' => t('Parsing file...'),
          'progress_message' => t('Saving nodes...'),
          'file' => './' . drupal_get_path('module', 'biblio') . '/includes/biblio.import.export.inc'
        );
        batch_set($batch_op);

        $base = variable_get('biblio_base', 'biblio');
        batch_process("$base/import");

      }
      else{ //not batch processing the file
        $session_id = microtime();
        $context = array();
        biblio_import($import_file, $filetype, $userid, $terms, $batch_proc, $session_id, $context);
        biblio_import_finalize(TRUE, $context['results']);
      }
      file_delete($import_file);
    }
    else {
      drupal_set_message(t("File was NOT successfully uploaded"), 'error');
    }
  }
}

function biblio_import_batch_operations($session_id, $user, $userid, $terms, &$context) {
 $limit = 10;
 if (empty($context['sandbox'])) {
    // Initiate multistep processing.
    $context['results']['session_id'] = $session_id;
    $context['results']['userid']     = $userid;
    $context['results']['user']       = $user;
    $context['results']['terms']      = $terms;
    $context['sandbox']['progress']   = 0;
    $context['sandbox']['current_id'] = 0;
    $context['results']['nids'] = array();
    $context['sandbox']['max'] = db_query("SELECT COUNT(DISTINCT(id)) FROM {biblio_import_cache} WHERE session_id = :sessid", array(':sessid' => $session_id))->fetchField();
    $context['sandbox']['itters'] = $context['sandbox']['max'] / $limit;
    $context['sandbox']['eta'] = 0;
  }
  // Bail out if the cache is empty.
  if ($context['sandbox']['max'] == 0) {
    return;
  }

  // Process the next 20 nodes.
  timer_start('biblio_import');

  $result = db_query_range("SELECT id, data FROM {biblio_import_cache} WHERE id > :id AND session_id = :sessid ORDER BY id ASC", 0, $limit, array(':id' => $context['sandbox']['current_id'], ':sessid' => $session_id));
  foreach ($result as $row) {
    if ($node = unserialize(base64_decode($row->data))) {
      biblio_save_node($node, $terms);
      $context['results']['nids'][] = $node->nid;
    }
    $context['sandbox']['progress']++;
    $context['sandbox']['current_id'] = $row->id;
  }
  $looptime = timer_stop('biblio_import');
  $context['sandbox']['eta'] += $looptime['time'];
  $itters = $context['sandbox']['progress'] / $limit;
  if ($itters) {
    $average_time = $context['sandbox']['eta'] / $itters;
    $eta = (($context['sandbox']['itters'] * $average_time) - ($average_time * $itters)) / 1000;
    if ($eta >= 60) {
      $min = (int) $eta / 60;
    }
    else {
      $min = 0;
    }
    $sec = $eta % 60;
    $eta = sprintf("%d:%02d", $min, $sec);
    $progress = sprintf("%d / %d", $context['sandbox']['progress'],  $context['sandbox']['max'] );
    $context['message'] = t('<br>Nodes saved: %progress <br> Time remaining: %eta min.<br>' , array('%progress' => $progress, '%eta' => $eta));

  }
  // Multistep processing : report progress.
  if ($context['sandbox']['progress'] <= $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}
function biblio_import_batch_finished($success, $results, $operations) {

  biblio_import_finalize($success, $results);
  //clean up import cache...
  db_delete('biblio_import_cache')
    ->condition('session_id', $results['session_id'])
    ->execute();
}

function biblio_import_finalize($success, $results) {
  global $user;
  $format = $results['format'];
  $nids = $results['nids'];
  $dups = $results['dups'];
  $total = count($nids) + count($dups);
  //    drupal_set_message(t("<i><b>%count</b></i> of <i><b>%total</b></i> nodes were successfully imported.", array('%count' => count($nids), '%total' => $total)), (count($nids) != $total)?'warning':'status');

  if ($success && (count($nids) || count($dups))) {
    $message = t("The file <i><b>@file</b></i> was successfully uploaded.", array('@file' => $results['file']->filename));
    drupal_set_message($message, 'status');
    watchdog($format, $message);
    $count = count($nids);
    $message = format_plural($count, 'One of @total node imported.', '@count of @total nodes imported.', array('@total' => $total));
    drupal_set_message($message, 'status');
    watchdog($format, $message, array('@count' => $count, '@total' => $total), WATCHDOG_INFO);
    if (count($dups)) {
      $count = count($dups);
      $message = format_plural($count, 'One duplicate node skipped.', '@count duplicate nodes skipped.');
      drupal_set_message($message, 'status');
      watchdog($format, $message, array('@count' => $count), WATCHDOG_INFO);
      foreach ($dups as $nid) {
        $message = '';
        $message = t('The item you are trying to import already exists in the database, see');
        $message .= ' ' . l('node/' . $nid, 'node/' . $nid);

        drupal_set_message($message, 'status');
        watchdog($format, $message, array(), WATCHDOG_ERROR);
      }
    }
  }
  else {
    $count = count($nids);
    $message = t('Import finished with an error!') . '  ' .  format_plural($count, 'One node imported.', '@count nodes imported.');
    drupal_set_message($message, 'error');
    watchdog($format, $message, array('@count' => $count), WATCHDOG_ERROR);
  }

  $userid = isset($results['userid']) ? $results['userid'] : $user->uid;

  if (user_access('administer biblio') && count($nids) && $user->uid != $userid) {
    db_update('node')
      ->fields(array('uid' => $results['userid']))
      ->condition('nid', $nids, 'IN')
      ->execute();
    db_update('node_revision')
      ->fields(array('uid' => $results['userid']))
      ->condition('nid', $nids, 'IN')
      ->execute();
  }

}

function biblio_import_from_url($URL) {
  $handle = fopen($URL, "r"); // fetch data from URL in read mode
  $data = "";
  if ($handle) {
    while (!feof($handle)) {
      $data .= fread($handle, 4096); // read data in chunks
    }
    fclose($handle);
  }
  else {
    $errorMessage = t("Error occurred: Failed to open %url", array('%url', $URL)); // network error
    drupal_set_message($errorMessage, 'error');
  }

  return $data;
}

function biblio_export_form() {
  $form['pot'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('POT Export'),
    '#description' => t('Here you may export a ".pot" file which contains the titles and hints from the database which are not normally captured by translation extractors)')
  );
  $form['pot']['button'] = array(
    '#type' => 'submit',
    '#value' => t('Export translation data')
  );

  return $form;
}
function biblio_export_form_submit($form, & $form_state) {
  if ($form_state['values']['op'] == t('Export translation data')) {
    biblio_dump_db_data_for_pot();
  }

}

/**
 * Import data from a file and return the node ids created.
 *
 * @param $userid
 *   The user id that will be assigned to each node imported
 * @param $filename
 *   The name of the file containing the data to import
 * @param $type
 *   The format of the file to be imported (tagged, XML, RIS, bibTEX)
 * @param $terms
 *   the vocabulary that the imported nodes will be associated with
 * @return
 *   An array of node id's of the items imported
 */
function biblio_import($import_file, $type, $userid = 1, $terms = NULL, $batch = FALSE, $session_id = NULL, &$context ) {
  global $user;
  $parsed = 0;
  $nids = array();
  $dups = array();

  if (isset($context['message']))  $context['message'] = t('Parsing file');
  switch ($type) {
    case 'csv' : // comma separated variable file
      //        $file_content = @ file_get_contents($import_file->uri);
      //        $parsed = biblio_csv_import($file_content, $node_template, $node_array);
      break;
    case 'biblio_backup' : // a complete backup of all biblio information
      $file_content = @ file_get_contents($import_file->uri);
      $parsed = biblio_restore($file_content, $node_template, $node_array);
      break;
    default:
      list($nids, $dups) = module_invoke($type, 'biblio_import', $import_file, $terms, $batch, $session_id);
      break;
  }
  $context['results']['nids']   = $nids;
  $context['results']['dups']   = $dups;
  $context['results']['format'] = $type;
  $context['results']['userid'] = $userid;
  $context['results']['user']   = $user;
  $context['results']['file']   = $import_file;

  return $batch ? NULL : $nids;
}
/**
 * Export nodes in a given file format.
 *
 * @param $format
 *   The file format to export the nodes in (tagged, XML, bibTEX)
 * @param $nid
 *   If not NULL, then export only the given nodeid, else we will
 *   use the session variable which holds the most recent query. If neither
 *   $nid or the session variable are set, then nothing is exported
 * @param $version
 *   The version of EndNote XML to use.  There is one format for ver. 1-7 and
 *   a different format for versions 8 and greater.
 * @return
 *   none
 */
function biblio_export($format = "tagged", $nid = NULL, $popup = FALSE) {
  $params = array();
  $nids = array();
  $arg_list = array();
  module_load_include('inc', 'biblio', 'includes/biblio.contributors');
  if ($nid === NULL ) {
    module_load_include('inc', 'biblio', 'includes/biblio.pages');
    $uri = drupal_parse_url(request_uri());
    $arg_list += $uri['query'];
    $arg_list['page_limit'] = 0;
    list($nids,,) = biblio_build_query($arg_list);
  }
  elseif (!empty ($nid)) {
    $nids[] = $nid;
  }
  elseif (!count($nids)) {
    return;
  }

  module_invoke('biblio_' . $format, 'biblio_export', $nids);

}

/**
 * Save node imported from a file.
 *
 * @param $node_array
 *   a 2 dimensional array containing all the node information
 * @return
 *   The node ids of the saved nodes
 */
function biblio_save_imported_nodes(& $node_array) {
  $dup_count = 0;
  if (function_exists('node_save')) {
    foreach ($node_array as $imp_node) {
      $node_ids[] = biblio_save_node($imp_node);
    }
  }
/*  if ($dup_count)
    drupal_set_message(t("Detected @dupcount duplicate node(s) when importing", array('@dupcount' => $dup_count)), 'error');

  drupal_set_message(t("Succesfully imported @count entries.", array('@count' => count($node_ids))), 'status');
*/
  return $node_ids;
}
function biblio_save_node($node, $terms = array(), $batch = FALSE, $session_id = NULL, $save_node = TRUE) {
  global $user;

  if ($batch && $session_id) { // we are batch processing some import data
    $cache['session_id'] = $session_id;
    $cache['data'] = base64_encode(serialize($node));// base64_encode to avoid problems unserializing strings with embeded quotes.
    drupal_write_record('biblio_import_cache', $cache);
    return;
  }

  $node->type = 'biblio';
  // Persist the node revision log since it will be overridden by
  // node_object_prepare().
  $created = !empty($node->created)? $node->created : NULL;
  $revision_log = !empty($node->log)? $node->log : NULL;
  node_object_prepare($node);
  $node->created = $created;
  $node->log = $revision_log;

  if (!empty($terms)) {
    foreach ($terms as $key => $value) {
      $node->{$key} = $value;
    }
  }

  $node->language = 'und';  //start by setting the language to undefined and then try to refine it.
  if (module_exists('locale')) {
    $node->language = locale_language_url_fallback();
  }
  if (module_exists('i18n') && variable_get('i18n_node_biblio', 0) && variable_get('language_content_type_biblio', 0) ) {
    $node->language = module_invoke('i18n', 'default_language');
  }

  if (!isset($node->biblio_type)) {
    $node->biblio_type = 129; // default to misc if not set.
  }


  if ($save_node) { // $save_node = TRUE, the normal save path
    $validation_errors = array();
    $validation_errors = biblio_save_node_validate($node);
    node_save($node);
    if (!empty($validation_errors)) {
      foreach ($validation_errors as $field => $value) {
        $message = $field . ' ' . t('was truncated to fit into the database column');
        $link = l('node/' . $node->nid, 'node/' . $node->nid);
        drupal_set_message($message . '; ' . $link, 'warning');
        watchdog('biblio - import', $message, array(), WATCHDOG_ALERT, $link);
      }
    }
    return; // (isset($node->nid)) ? $node->nid : 0;
  }
  else { // $save_node = FALSE, primarily used to parse data and return it to the input form
    return (array)$node;
  }
}

function biblio_save_node_validate($node) {
  static $schema = array();
  $error = array();

  if (empty($schema)) {
    $schema['biblio'] = drupal_get_schema('biblio');
  }
  if (isset($node->title) && strlen($node->title) > 255) {
    $error['title'] = $node->title;
    $node->title = substr($node->title, 0, 255);
  }
  if (isset($node->biblio_keywords) && is_array($node->biblio_keywords)) {
    foreach ($node->biblio_keywords as $key => $word) {
      if (strlen($word) > 255) {
        $error['keyword'] = $word;
        $node->biblio_keywords[$key] = substr($word, 0, 255);
      }
    }
  }
  if (isset($node->biblio_contributors) && is_array($node->biblio_contributors)) {
    foreach ($node->biblio_contributors as $key => $author) {
      if (strlen($author['name']) > 255) {
        $error['author'] = $author['name'];
        $node->biblio_contributors[$key]['name'] = substr($author['name'], 0, 255);
      }
    }
  }
  foreach ($schema['biblio']['fields'] as $field => $spec) {
    if (isset($node->$field)) {
      switch ($spec['type']) {
        case 'varchar':
          if (strlen($node->$field) > $spec['length']) {
            $error[$field] = $node->$field;
            $node->$field = substr($node->$field, 0, $spec['length']);
          }
          break;
      }
    }
  }
  return $error;
}


function biblio_csv_export_2($result, $bfields) {
  //  $query_biblio_fields = 'SELECT name, title FROM {biblio_fields}';
  //  $res_biblio_fields = db_query($query_biblio_fields);
  //  while ($rec = db_fetch_object($res_biblio_fields)) {
  //    $bfields[$rec->name] = $rec->title;
  //  }
  $bfields = biblio_get_db_fields('all');
  $query_biblio_types = 'SELECT tid, name FROM {biblio_types}';
  $res_biblio_types = db_query($query_biblio_types);
  foreach ($res_biblio_types as $rec) {
    $btypes[$rec->tid] = $rec->name;
  }
  switch (variable_get('biblio_csv_field_sep', 'tab')) {
    case 'tab' :
      $filedsep = "\t";
      break;
    case 'comma' :
      $filedsep = ',';
      break;
  }
  switch (variable_get('biblio_csv_text_sep', 'dquote')) {
    case 'dquote' :
      $textsep = '"';
      break;
    case 'quote' :
      $textsep = '\'';
      break;
  }
  $label = (variable_get('biblio_csv_col_head', 'label') == 'label' ? 1 : 0); // or 'col_name'
  $linebreak = variable_get('biblio_linebreak_exp', 1);
  foreach ($result as $rec) {
    $node_id = $rec->nid;
    $node_array[$node_id]['type'] = $btypes[$rec->biblio_type]; // there is no "label" for "type"
    $col_array['type'] = 'Type';
    foreach (array_keys($bfields) as $fieldname) {
      if (!empty ($rec-> $fieldname) && !in_array($fieldname, array(
          'biblio_citekey',
          'biblio_coins'
        ))) {
        $col_array[$fieldname] = $bfields[$fieldname]; // mark field as in use
        $text = strtr($rec-> $fieldname, $textsep, "$textsep$textsep");
        if ($linebreak) {
          $text = strtr($text, ';', "\n");
        }
        $node_array[$node_id][$fieldname] = trim($text);
      }
    }
  } //end while
  if ($label) { // head line containing column names
    $csv = $textsep . join("$textsep$filedsep$textsep", array_values($col_array)) . "$textsep\n";
  }
  else { // original DB field names
    $csv = $textsep . join("$textsep$filedsep$textsep", array_keys($col_array)) . "$textsep\n";
  }
  // Enclosing text in "<text>" is neccessary to enshure
  // multi line fields (like author) are handled correctly.
  // Therefore existing " must be excaped before.
  $csv = '"' . join("\"\t\"", array_keys($col_array)) . "\"\n";
  foreach ($node_array as $line_array) {
    $csv_line = '';
    foreach (array_keys($col_array) as $col) {
      $csv_line .= "$filedsep$textsep" . $line_array[$col] . $textsep;
    }
    $csv .= substr($csv_line, 1) . "\n"; // cut off leading fieldsep and append EOL
  }
  drupal_add_http_header('Content-Type', 'text/plain; charset=utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=biblio_export.csv');
  return $csv;
}
//function _biblio_cck_join($biblio_fields = array()) {    // works not with php4
function _biblio_cck_join(& $biblio_fields) {
  $cck_join = '';
  $biblio_fields['nid'] = 'Node-ID'; // identify records for update operations
  $query_cck_fields = "SELECT field_name, label from {node_field_instance} where type_name='biblio' and not (widget_type='image')";
  $res_cck_fields = db_query($query_cck_fields);
  foreach ($$res_cck_fields as $rec) {
    $cck_table = 'content_' . $rec->field_name;
    $cck_field = $rec->field_name . '_value';
    $biblio_fields[$cck_field] = $rec->label;
    $cck_join .= ' left join {' . $cck_table . '} on b.vid=' . $cck_table . '.vid';
  }
  return $cck_join;
}

function biblio_backup() {

  $csv_function = (!function_exists('fputcsv')) ? 'biblio_fputcsv' : 'fputcsv';
  $count_sql = "SELECT COUNT(*)
                FROM {biblio} b, {node} n, {node_revision} nr
                WHERE b.vid = n.vid and nr.vid = n.vid;";
  $field_type_sql = "SELECT *  FROM {biblio_field_type} ";
  $field_type_data_sql = "SELECT *  FROM {biblio_field_type_data} ";
  $field_fields_sql = "SELECT *  FROM {biblio_fields} ";
  $types_sql = "SELECT *  FROM {biblio_types} ";
  $sql = "SELECT b.*,
          n.type, n.language, n.title, n.uid, n.status, n.created,
          n.changed, n.comment, n.promote, n.moderate, n.sticky,
          n.tnid, n.translate,
          nr.title, nr.body, nr.teaser, nr.log, nr.timestamp, nr.format
          FROM {biblio} b, {node} n, {node_revision} nr
          WHERE b.vid = n.vid and nr.vid = n.vid;";

  $biblio_count = db_result(db_query($count_sql));
  if ($biblio_count) {
    drupal_add_http_header('Content-Type', 'text/plain; charset=utf-8');
    drupal_add_http_header('Content-Disposition', 'attachment; filename=Biblio-export.csv');
    $biblio_nodes = db_query($sql);
    while ($node = db_fetch_array($biblio_nodes)) {
      $results[] = $node;
    }
    print biblio_csv_export($results);
    unset($results);
    $result = db_query($field_type_data_sql, 'biblio_field_type_data.csv');
    while ($data = db_fetch_array($result)) {
      $results[] = $data;
    }
    print biblio_csv_export($results);
    unset($results);
    $result = db_query($field_fields_sql, 'biblio_fields.csv');
    while ($data = db_fetch_array($result)) {
      $results[] = $data;
    }
    print biblio_csv_export($results);
    unset($results);
    $result = db_query($types_sql, 'biblio_types.csv');
    while ($data = db_fetch_array($result)) {
      $results[] = $data;
    }
    print biblio_csv_export($results);
    unset($results);
    $result = db_query($field_type_sql, 'biblio_field_type.csv');
    while ($data = db_fetch_array($result)) {
      $results[] = $data;
    }
    print biblio_csv_export($results);
  }
}

function biblio_restore(& $csv_content, $mode = 'create') {

}

function biblio_csv_export($results) {
  $csv = '';
  if (!is_array($results)) {
    $result_array[] = (array) $results;
  }
  else {
    $result_array = $results;
  }
  $fieldnames = NULL;
  foreach ((array)$result_array as $rec) {
    if (empty($fieldnames)) {
      $fieldnames = array_keys($rec);
      $csv .= biblio_strcsv($fieldnames);
    }
    $csv .= biblio_strcsv($rec);
  }
  return $csv;
}

function biblio_strcsv($fields = array(), $delimiter = ',', $enclosure = '"') {
  $str = '';
  $escape_char = '\\';
  foreach ($fields as $value) {
    if (strpos($value, $delimiter) !== FALSE || strpos($value, $enclosure) !== FALSE || strpos($value, "\n") !== FALSE || strpos($value, "\r") !== FALSE || strpos($value, "\t") !== FALSE || strpos($value, ' ') !== FALSE) {
      $str2 = $enclosure;
      $escaped = 0;
      $len = strlen($value);
      for ($i = 0; $i < $len; $i++) {
        if ($value[$i] == $escape_char) {
          $escaped = 1;
        }
        else
          if (!$escaped && $value[$i] == $enclosure) {
            $str2 .= $enclosure;
          }
          else {
            $escaped = 0;
          }
        $str2 .= $value[$i];
      }
      $str2 .= $enclosure;
      $str .= $str2 . $delimiter;
    }
    else {
      $str .= $value . $delimiter;
    }
  }
  $str = substr($str, 0, -1);
  $str .= "\n";
  return $str;
}
function biblio_dump_db_data_for_pot() {
  $query = "SELECT name, description FROM {biblio_types} ";
  $result = db_query($query);
  $strings = array();
  foreach ($result as $type) {
    $strings[] = $type->name;
    if (!empty($type->description)) $strings[] = $type->description;
  }
  $query = "SELECT title, hint FROM {biblio_field_type_data} ";
  $result = db_query($query);
  foreach ($result as$type_data) {
    $strings[] = $type_data->title;
    if (!empty($type_data->hint)) $strings[] = $type_data->hint;
  }
  $query = "SELECT title, hint FROM {biblio_contributor_type_data} ";
  $result = db_query($query);
  foreach ($result as $type_data ) {
    $strings[] = $type_data->title;
    if (!empty($type_data->hint)) $type_data->hint;
  }
  $strings = array_unique($strings);
  foreach ($strings as $string) {
    $output .= "t(\"$string\"\);\n";
  }

  drupal_add_http_header('Content-Type', 'text/plain; charset=utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=biblio_db_values.pot');
  print $output;
}

