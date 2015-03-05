<?php

/**
 * @file
 * This file includes helper functions for creating reports for webform.module
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

// All functions within this file need the webform.submissions.inc.
module_load_include('inc', 'webform', 'includes/webform.submissions');

/**
 * Retrieve lists of submissions for a given webform.
 */
function webform_results_submissions($node, $user_filter, $pager_count) {
  global $user;

  // Determine whether views or hard-coded tables should be used for the submissions table.
  if (!webform_variable_get('webform_table')) {
    // Load the submissions view
    $view = webform_get_view($node, 'webform_submissions');
    if ($user_filter) {
      if ($user->uid) {
        drupal_set_title(t('Submissions for %user', array('%user' => $user->name)), PASS_THROUGH);
      }
      else {
        drupal_set_title(t('Your submissions'));
        webform_disable_page_cache();
      }
      return $view->preview('default', array($node->nid, $user->uid));
    }
    else {
      return $view->preview('default', array($node->nid));
    }
  }

  if (isset($_GET['results']) && is_numeric($_GET['results'])) {
    $pager_count = $_GET['results'];
  }

  $header = theme('webform_results_submissions_header', array('node' => $node));
  if ($user_filter) {
    if ($user->uid) {
      drupal_set_title(t('Submissions for %user', array('%user' => $user->name)), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Your submissions'));
      webform_disable_page_cache();
    }
    $submissions = webform_get_submissions(array('nid' => $node->nid, 'uid' => $user->uid), $header, $pager_count);
    $count = webform_get_submission_count($node->nid, $user->uid, NULL);
  }
  else {
    $submissions = webform_get_submissions($node->nid, $header, $pager_count);
    $count = webform_get_submission_count($node->nid, NULL, NULL);
  }

  $operation_column = end($header);
  $operation_total = $operation_column['colspan'];

  $rows = array();
  foreach ($submissions as $sid => $submission) {
    $row = array(
      $submission->is_draft ? t('@serial (draft)', array('@serial' => $submission->serial)) : $submission->serial,
      format_date($submission->submitted, 'short'),
    );
    if (webform_results_access($node, $user)) {
      $row[] = theme('username', array('account' => $submission));
      $row[] = $submission->remote_addr;
    }
    $row[] = l(t('View'), "node/$node->nid/submission/$sid");
    $operation_count = 1;
    // No need to call this multiple times, just reference this in a variable.
    $destination = drupal_get_destination();
    if (webform_submission_access($node, $submission, 'edit', $user)) {
      $row[] = l(t('Edit'), "node/$node->nid/submission/$sid/edit", array('query' => $destination));
      $operation_count++;
    }
    if (webform_submission_access($node, $submission, 'delete', $user)) {
      $row[] = l(t('Delete'), "node/$node->nid/submission/$sid/delete", array('query' => $destination));
      $operation_count++;
    }
    if ($operation_count < $operation_total) {
      $row[count($row) - 1] = array('data' => $row[count($row) - 1], 'colspan' => $operation_total - $operation_count + 1);
    }
    $rows[] = $row;
  }

  $element['#theme'] = 'webform_results_submissions';
  $element['#node'] = $node;
  $element['#submissions'] = $submissions;
  $element['#total_count'] = $count;
  $element['#pager_count'] = $pager_count;
  $element['#attached']['library'][] = array('webform', 'admin');

  $element['table']['#theme'] = 'table';
  $element['table']['#header'] = $header;
  $element['table']['#rows'] = $rows;
  $element['table']['#operation_total'] = $operation_total;

  return drupal_render($element);

}

/**
 * Returns the most appropriate view for this webform node.
 *
 * Site builders can customize the view that webform uses by webform node id or
 * by webform content type. For example, use webform_results_123 to for node
 * id 123 or webform_results_my_content_type for a node of type my_content_type.
 *
 * @param object $node
 *   Loaded webform node.
 * @param string $view_id
 *   machine_id of the view, such as webform_results or webform_submissions.
 * @return object|NULL
 *   The loaded view.
 */
function webform_get_view($node, $view_id) {
  foreach (array("{$view_id}_{$node->nid}", "{$view_id}_{$node->type}", $view_id) as $id) {
    $view = views_get_view($id);
    if ($view) {
      return $view;
    }
  }
}

/**
 * Theme the list of links for selecting the number of results per page.
 *
 * @param $total_count
 *   The total number of results available.
 * @param $pager_count
 *   The current number of results displayed per page.
 */
function theme_webform_results_per_page($variables) {
  $total_count = $variables['total_count'];
  $pager_count = $variables['pager_count'];
  $output = '';

  // Create a list of results-per-page options.
  $counts = array(
    '20' => '20',
    '50' => '50',
    '100' => '100',
    '200' => '200',
    '500' => '500',
    '1000' => '1000',
    '0' => t('All'),
  );

  $count_links = array();

  foreach ($counts as $number => $text) {
    if ($number < $total_count) {
      $count_links[] = l($text, $_GET['q'], array('query' => array('results' => $number), 'attributes' => array('class' => array($pager_count == $number ? 'selected' : ''))));
    }
  }

  $output .= '<div class="webform-results-per-page">';
  if (count($count_links) > 1) {
    $output .= t('Show !count results per page.', array('!count' => implode(' | ', $count_links)));
  }
  else {
    $output .= t('Showing all results.');
  }
  if ($total_count > 1) {
    $output .= ' ' . t('@total results total.', array('@total' => $total_count));
  }
  $output .= '</div>';

  return $output;
}

/**
 * Theme the header of the submissions table.
 *
 * This is done in it's own function so that webform can retrieve the header and
 * use it for sorting the results.
 */
function theme_webform_results_submissions_header($variables) {
  $node = $variables['node'];

  $columns = array(
    array('data' => t('#'), 'field' => 'sid', 'sort' => 'desc'),
    array('data' => t('Submitted'), 'field' => 'submitted'),
  );
  if (webform_results_access($node)) {
    $columns[] = array('data' => t('User'), 'field' => 'name');
    $columns[] = array('data' => t('IP Address'), 'field' => 'remote_addr');
  }
  $columns[] = array('data' => t('Operations'), 'colspan' => module_exists('print') ? 5 : 3);

  return $columns;
}

/**
 * Preprocess function for webform-results-submissions.tpl.php
 */
function template_preprocess_webform_results_submissions(&$vars) {
  $vars['node'] = $vars['element']['#node'];
  $vars['submissions'] = $vars['element']['#submissions'];
  $vars['table'] = $vars['element']['table'];
  $vars['total_count'] = $vars['element']['#total_count'];
  $vars['pager_count'] = $vars['element']['#pager_count'];
  $vars['is_submissions'] = (arg(2) == 'submissions')? 1 : 0;

  unset($vars['element']);
}

/**
 * Create a table containing all submitted values for a webform node.
 */
function webform_results_table($node, $pager_count = 0) {
  
  // Determine whether views or hard-coded tables should be used for the submissions table.
  if (!webform_variable_get('webform_table')) {
    // Load and preview the results view with a node id argument
    $view = webform_get_view($node, 'webform_results');
    return $view->preview('default', array($node->nid));
  }

  // Get all the submissions for the node
  if (isset($_GET['results']) && is_numeric($_GET['results'])) {
    $pager_count = $_GET['results'];
  }

  // Get all the submissions for the node.
  $header = theme('webform_results_table_header', array('node' => $node));
  $submissions = webform_get_submissions($node->nid, $header, $pager_count);
  $total_count = webform_get_submission_count($node->nid, NULL, NULL);

  $output = theme('webform_results_table', array('node' => $node, 'components' => $node->webform['components'], 'submissions' => $submissions, 'total_count' => $total_count, 'pager_count' => $pager_count));
  if ($pager_count) {
    $output .= theme('pager');
  }
  return $output;
}

function theme_webform_results_table_header($variables) {
  return array(
    array('data' => t('#'), 'field' => 'sid', 'sort' => 'desc'),
    array('data' => t('Submitted'), 'field' => 'submitted'),
    array('data' => t('User'), 'field' => 'name'),
    array('data' => t('IP Address'), 'field' => 'remote_addr'),
  );
}

/**
 * Theme the results table displaying all the submissions for a particular node.
 *
 * @param $node
 *   The node whose results are being displayed.
 * @param $components
 *   An associative array of the components for this webform.
 * @param $submissions
 *   An array of all submissions for this webform.
 * @param $total_count
 *   The total number of submissions to this webform.
 * @param $pager_count
 *   The number of results to be shown per page.
 */
function theme_webform_results_table($variables) {
  drupal_add_library('webform', 'admin');

  $node = $variables['node'];
  $components = $variables['components'];
  $submissions = $variables['submissions'];
  $total_count = $variables['total_count'];
  $pager_count = $variables['pager_count'];

  $header = array();
  $rows = array();
  $cell = array();

  // This header has to be generated separately so we can add the SQL necessary.
  // to sort the results.
  $header = theme('webform_results_table_header', array('node' => $node));

  // Generate a row for each submission.
  foreach ($submissions as $sid => $submission) {
    $link_text = $submission->is_draft ? t('@serial (draft)', array('@serial' => $submission->serial)) : $submission->serial;
    $cell[] = l($link_text, 'node/' . $node->nid . '/submission/' . $sid);
    $cell[] = format_date($submission->submitted, 'short');
    $cell[] = theme('username', array('account' => $submission));
    $cell[] = $submission->remote_addr;
    $component_headers = array();

    // Generate a cell for each component.
    foreach ($node->webform['components'] as $component) {
      $data = isset($submission->data[$component['cid']]) ? $submission->data[$component['cid']] : NULL;
      $submission_output = webform_component_invoke($component['type'], 'table', $component, $data);
      if ($submission_output !== NULL) {
        $component_headers[] = array('data' => check_plain($component['name']), 'field' => $component['cid']);
        $cell[] = $submission_output;
      }
    }

    $rows[] = $cell;
    unset($cell);
  }
  if (!empty($component_headers)) {
    $header = array_merge($header, $component_headers);
  }

  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('There are no submissions for this form. <a href="!url">View this form</a>.', array('!url' => url('node/' . $node->nid))), 'colspan' => 4));
  }


  $output = '';
  $output .= theme('webform_results_per_page', array('total_count' => $total_count, 'pager_count' => $pager_count));
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Delete all submissions for a node.
 *
 * @param $nid
 *   The node id whose submissions will be deleted.
 */
function webform_results_clear($nid) {
  $node = node_load($nid);
  $submissions = webform_get_submissions($nid);
  foreach ($submissions as $submission) {
    webform_submission_delete($node, $submission);
  }
}

/**
 * Confirmation form to delete all submissions for a node.
 *
 * @param $nid
 *   ID of node for which to clear submissions.
 */
function webform_results_clear_form($form, $form_state, $node) {
  drupal_set_title(t('Clear Form Submissions'));

  $form = array();
  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $question = t('Are you sure you want to delete all submissions for this form?');

  return confirm_form($form, $question, 'node/' . $node->nid . '/webform-results', NULL, t('Clear'), t('Cancel'));
}

function webform_results_clear_form_submit($form, &$form_state) {
  webform_results_clear($form_state['values']['nid']);
  $node = node_load($form_state['values']['nid']);
  $title = $node->title;
  $message = t('Webform %title entries cleared.', array('%title' => $title));
  drupal_set_message($message);
  watchdog('webform', $message);
  $form_state['redirect'] = 'node/' . $form_state['values']['nid'] . '/webform-results';
}

/**
 * Form to configure the download of CSV files.
 */
function webform_results_download_form($form, &$form_state, $node) {
  module_load_include('inc', 'webform', 'includes/webform.export');
  module_load_include('inc', 'webform', 'includes/webform.components');

  $form['#attached']['js'][] = drupal_get_path('module', 'webform') . '/js/webform-admin.js';

  // If an export is waiting to be downloaded, redirect the user there after
  // the page has finished loading.
  if (isset($_SESSION['webform_export_info'])) {
    $download_url = url('node/' . $node->nid . '/webform-results/download-file', array('absolute' => TRUE));
    $form['#attached']['js'][] = array('data' => array('webformExport' => $download_url), 'type' => 'setting');
  }

  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );

  $form['format'] = array(
    '#type' => 'radios',
    '#title' => t('Export format'),
    '#options' => webform_export_list(),
    '#default_value' => webform_variable_get('webform_export_format'),
  );

  $form['delimiter'] = array(
    '#type' => 'select',
    '#title' => t('Delimited text format'),
    '#description' => t('This is the delimiter used in the CSV/TSV file when downloading Webform results. Using tabs in the export is the most reliable method for preserving non-latin characters. You may want to change this to another character depending on the program with which you anticipate importing results.'),
    '#default_value' => webform_variable_get('webform_csv_delimiter'),
    '#options' => array(
      ','  => t('Comma (,)'),
      '\t' => t('Tab (\t)'),
      ';'  => t('Semicolon (;)'),
      ':'  => t('Colon (:)'),
      '|'  => t('Pipe (|)'),
      '.'  => t('Period (.)'),
      ' '  => t('Space ( )'),
    ),
    '#states' => array(
      'visible' => array(
        ':input[name=format]' => array('value' => 'delimited'),
      ),
    ),
  );

  $form['header_keys'] = array(
    '#type' => 'radios',
    '#title' => t('Column header format'),
    '#options' => array(
      -1 => t('None'),
      0 => t('Label'),
      1 => t('Field Key'),
    ),
    '#default_value' => 0,
    '#description' => t('Choose whether to show the label or field key in each column header.'),
  );

  $form['select_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Select list options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['select_options']['select_keys'] = array(
    '#type' => 'radios',
    '#title' => t('Select keys'),
    '#options' => array(
      0 => t('Full, human-readable options (values)'),
      1 => t('Short, raw options (keys)'),
    ),
    '#default_value' => 0,
    '#description' => t('Choose which part of options should be displayed from key|value pairs.'),
  );

  $form['select_options']['select_format'] = array(
    '#type' => 'radios',
    '#title' => t('Select list format'),
    '#options' => array(
      'separate' => t('Separate'),
      'compact' => t('Compact'),
    ),
    '#default_value' => 'separate',
    '#attributes' => array('class' => array('webform-select-list-format')),
    '#theme' => 'webform_results_download_select_format',
  );

  $csv_components = array('info' => t('Submission information'));
  // Prepend information fields with "-" to indent.
  foreach (webform_results_download_submission_information($node) as $key => $title) {
    $csv_components[$key] = '-' . $title;
  }

  $csv_components += webform_component_list($node, 'csv', TRUE);

  $form['components'] = array(
    '#type' => 'select',
    '#title' => t('Included export components'),
    '#options' => $csv_components,
    '#default_value' => array_keys($csv_components),
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('The selected components will be included in the export.'),
    '#process' => array('webform_component_select'),
  );

  $form['range'] = array(
    '#type' => 'fieldset',
    '#title' => t('Download range options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#theme' => 'webform_results_download_range',
    '#element_validate' => array('webform_results_download_range_validate'),
    '#after_build' => array('webform_results_download_range_after_build'),
  );

  $form['range']['range_type'] = array(
    '#type' => 'radios',
    '#options' => array(
      'all' => t('All submissions'),
      'new' => t('Only new submissions since your last download'),
      'latest' => t('Only the latest'),
      'range_serial' => t('All submissions starting from'),
    ),
    '#default_value' => 'all',
  );
  $form['range']['latest'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 8,
  );
  $form['range']['start'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 8,
  );
  $form['range']['end'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 8,
  );

  // If drafts are allowed, provide options to filter download based on draft status.
  if ($node->webform['allow_draft'] || $node->webform['auto_save']) {
    $form['range']['completion_type'] = array(
      '#type' => 'radios',
      '#title' => t('Included submissions'),
      '#default_value' => 'all',
      '#options' => array(
        'all' => t('Finished and draft submissions'),
        'finished' => t('Finished submissions only'),
        'draft' => t('Drafts only'),
      ),
    );
  }

  // By default results are downloaded. User can override this value if
  // programmatically submitting this form.
  $form['download'] = array(
    '#type' => 'value',
    '#default_value' => TRUE
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download'),
  );

  return $form;
}

/**
 * FormAPI element validate function for the range fieldset.
 */
function webform_results_download_range_validate($element, $form_state) {
  switch ($element['range_type']['#value']) {
    case 'latest':
      // Download latest x submissions.
      if ($element['latest']['#value'] == '') {
        form_error($element['latest'], t('Latest number of submissions field is required.'));
      }
      else{
        if (!is_numeric($element['latest']['#value'])) {
          form_error($element['latest'], t('Latest number of submissions must be numeric.'));
        }
        else{
          if ($element['latest']['#value'] <= 0) {
            form_error($element['latest'], t('Latest number of submissions must be greater than 0.'));
          }
        }
      }
      break;
    case 'range_serial':
      // Download Start-End range of submissions.
      // Start submission number.
      if ($element['start']['#value'] == '') {
        form_error($element['start'], t('Start submission number is required.'));
      }
      else{
        if (!is_numeric($element['start']['#value'])) {
          form_error($element['start'], t('Start submission number must be numeric.'));
        }
        else{
          if ($element['start']['#value'] <= 0) {
            form_error($element['start'], t('Start submission number must be greater than 0.'));
          }
        }
      }
      // End submission number.
      if ($element['end']['#value'] != '') {
        if (!is_numeric($element['end']['#value'])) {
          form_error($element['end'], t('End submission number must be numeric.'));
        }
        else{
          if ($element['end']['#value'] <= 0) {
            form_error($element['end'], t('End submission number must be greater than 0.'));
          }
          else{
            if ($element['end']['#value'] < $element['start']['#value']) {
              form_error($element['end'], t('End submission number may not be less than Start submission number.'));
            }
          }
        }
      }
      // Check that the range will return something at all.
      $form_state['values']['sids'] = webform_download_sids($form_state['values']['node']->nid, array('range_type' => $element['range_type']['#value'], 'start' => $element['start']['#value'], 'end' => $element['end']['#value']));
      if (empty($form_state['values']['sids'])) {
        form_error($element['start'], t('The specified range will not return any results.'));
      }
      break;
  }

}

/**
 * FormAPI after build function for the download range fieldset.
 */
function webform_results_download_range_after_build($element, &$form_state) {
  $node = $form_state['values']['node'];

  // Build a list of counts of new and total submissions.
  $count = webform_get_submission_count($node->nid, NULL, NULL);
  $sids = webform_download_sids($node->nid, array('range_type' => 'new'));

  $last_download = webform_download_last_download_info($node->nid);

  $element['#webform_download_info']['sid'] = $last_download ? $last_download['sid'] : 0;
  $element['#webform_download_info']['serial'] = $last_download ? $last_download['serial'] : NULL;
  $element['#webform_download_info']['requested'] = $last_download ? $last_download['requested'] : $node->created;
  $element['#webform_download_info']['total'] = $count;
  $element['#webform_download_info']['new'] = count($sids);

  return $element;
}

/**
 * Theme the output of the export range fieldset.
 */
function theme_webform_results_download_range($variables) {
  drupal_add_library('webform', 'admin');

  $element = $variables['element'];
  $download_info = $element['#webform_download_info'];

  // Set description for total of all submissions.
  $element['range_type']['all']['#theme_wrappers'] = array('webform_inline_radio');
  $element['range_type']['all']['#description'] = '(' . t('@count total', array('@count' => $download_info['total'])) . ')';

  // Set description for "New submissions since last download".
  $format = webform_date_format('short');
  $requested_date = format_date($download_info['requested'], 'custom', $format);
  $element['range_type']['new']['#theme_wrappers'] = array('webform_inline_radio');
  $element['range_type']['new']['#description'] = '(' . t('@count new since @date', array('@count' => $download_info['new'], '@date' => $requested_date)) . ')';


  // Disable option if there are no new submissions.
  if ($download_info['new'] == 0) {
    $element['range_type']['new']['#attributes']['disabled'] = 'disabled';
  }

  // Render latest x submissions option.
  $element['latest']['#attributes']['class'][] = 'webform-set-active';
  $element['latest']['#theme_wrappers'] = array();
  $element['range_type']['latest']['#theme_wrappers'] = array('webform_inline_radio');
  $element['range_type']['latest']['#title'] = t('Only the latest !number submissions', array('!number' => drupal_render($element['latest'])));

  // Render Start-End submissions option.
  $element['start']['#attributes']['class'][] = 'webform-set-active';
  $element['end']['#attributes']['class'][] = 'webform-set-active';
  $element['start']['#theme_wrappers'] = array();
  $element['end']['#theme_wrappers'] = array();
  $element['range_type']['range_serial']['#theme_wrappers'] = array('webform_inline_radio');
  $element['range_type']['range_serial']['#title'] = t('All submissions starting from: !start and optionally to: !end', array('!start' => drupal_render($element['start']), '!end' => drupal_render($element['end'])));

  $last_serial = $download_info['serial'] ? $download_info['serial'] : drupal_placeholder(t('none'));
  $element['range_type']['range_serial']['#description'] = '(' . t('Last downloaded end submission number: !serial.', array('!serial' => $last_serial)) . ')';

  return drupal_render_children($element);
}

/**
 * Theme the output of the select list format radio buttons.
 */
function theme_webform_results_download_select_format($variables) {
  drupal_add_library('webform', 'admin');

  $element = $variables['element'];
  $output = '';

  // Build an example table for the separate option.
  $header = array(t('Option A'), t('Option B'), t('Option C'));
  $rows = array(
    array('X', '', ''),
    array('X', '', 'X'),
    array('', 'X', 'X'),
  );

  $element['separate']['#attributes']['class'] = array();
  $element['separate']['#description'] = theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE));
  $element['separate']['#description'] .= t('Separate options are more suitable for building reports, graphs, and statistics in a spreadsheet application.');
  $output .= drupal_render($element['separate']);

  // Build an example table for the compact option.
  $header = array(t('My select list'));
  $rows = array(
    array('Option A'),
    array('Option A,Option C'),
    array('Option B,Option C'),
  );

  $element['compact']['#attributes']['class'] = array();
  $element['compact']['#description'] = theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE));
  $element['compact']['#description'] .= t('Compact options are more suitable for importing data into other systems.');
  $output .= drupal_render($element['compact']);

  return $output;
}

/**
 * Submit handler for webform_results_download_form().
 */
function webform_results_download_form_submit(&$form, &$form_state) {
  $node = $form_state['values']['node'];
  $format = $form_state['values']['format'];

  $options = array(
    'delimiter' => $form_state['values']['delimiter'],
    'components' => array_keys(array_filter($form_state['values']['components'])),
    'header_keys' => $form_state['values']['header_keys'],
    'select_keys' => $form_state['values']['select_keys'],
    'select_format' => $form_state['values']['select_format'],
    'range' => $form_state['values']['range'],
    'download' => $form_state['values']['download'],
  );

  $defaults = webform_results_download_default_options($node, $format);
  $options += $defaults;
  $options['range'] += $defaults['range'];

  // Use a pre-built list of SIDs provided by validate handlers (if any).
  if (isset($form_state['values']['sids'])) {
    $options['sids'] = $form_state['values']['sids'];
  }
  // Retrieve the list of required SIDs.
  elseif ($options['range']['range_type'] != 'all') {
    $options['sids'] = webform_download_sids($node->nid, $options['range']);
  }

  // Determine an appropriate batch size based on the form and server specs.
  if (!isset($options['range']['batch_size'])) {
    // Start the batch size at 50,000 per batch, but divide by number of
    // components in the form. e.g. If a form had 10 components, it would
    // export 5,000 submissions at a time.
    $batch_size = ceil(50000 / max(1, count($form_state['values']['node']->webform['components'])));

    // Every 32MB of additional memory after 64MB adds a multiplier in size.
    $memory_limit = parse_size(ini_get('memory_limit'));
    $mb = 1048576;
    $memory_modifier = max(1, ($memory_limit - (64 * $mb)) / (32 * $mb));
    $batch_size = ceil($batch_size * $memory_modifier);

    // For time reasons, limit the batch size to 10,000.
    $batch_size = min($batch_size, 10000);

    // Allow a non-UI configuration to override the batch size.
    $batch_size = variable_get('webform_export_batch_size', $batch_size);
    
    $options['range']['batch_size'] = $batch_size;
  }

  $options['file_name'] = _webform_export_tempname();

  // Set up a batch to export the results.
  $batch = webform_results_export_batch($node, $format, $options);
  batch_set($batch);
}

/**
 * Returns a temporary export filename.
 */
function _webform_export_tempname() {
  return drupal_tempnam(variable_get('webform_export_path', 'temporary://'), 'webform_');
}

/**
 * Generate a Excel-readable CSV file containing all submissions for a Webform.
 *
 * Note that this function is generally no longer utilized. Instead Batch API
 * should be used to generate a file over multiple requests.
 *
 * @see webform_results_export_batch()
 *
 * @return array|null
 *   The array of export info or null if the file could not be opened.
 */
function webform_results_export($node, $format = 'delimited', $options = array()) {
  global $user;
  module_load_include('inc', 'webform', 'includes/webform.export');
  module_load_include('inc', 'webform', 'includes/webform.components');

  $defaults = webform_results_download_default_options($node, $format);
  $options += $defaults;
  $options['range'] += $defaults['range'];

  // Open a new Webform exporter object.
  $exporter = webform_export_create_handler($format, $options);

  $file_name = _webform_export_tempname();
  $handle = fopen($file_name, 'w');
  if (!$handle) {
    return;
  }

  // Add the beginning of file marker (little-endian usually for MS Excel).
  $exporter->bof($handle);

  // Add headers to the file.
  $row_count = 0;
  $col_count = 0;
  $headers = webform_results_download_headers($node, $options);
  foreach ($headers as $row) {
    // Output header if header_keys is non-negative. -1 means no headers.
    if ($options['header_keys'] >= 0) {
      $exporter->add_row($handle, $row, $row_count);
      $row_count++;
    }
    $col_count = count($row) > $col_count ? count($row) : $col_count;
  }

  // Write data from submissions.
  $rows = webform_results_download_rows($node, $options);
  foreach ($rows as $row) {
    $exporter->add_row($handle, $row, $row_count);
    $row_count++;
  }

  // Add the closing bytes.
  $exporter->eof($handle, $row_count, $col_count);

  // Close the file.
  @fclose($handle);

  $export_info['format'] = $format;
  $export_info['options'] = $options;
  $export_info['file_name'] = $file_name;
  $export_info['row_count'] = $row_count;
  $export_info['col_count'] = $col_count;
  $export_info['last_sid'] = $sid;

  return $export_info;
}

/**
 * Return a Batch API array of commands that will generate an export.
 *
 * @param $node
 *   The webform node on which to generate the analysis.
 * @param string $format
 *   (optional) Delimiter of the exported file.
 * @param array $options
 *   (optional) An associative array of options that define the output format.
 *   These are generally passed through from the GUI interface. Possible options
 *   include:
 *   - sids: An array of submission IDs to which this export may be filtered.
 *     May be used to generate exports that are per-user or other groups of
 *     submissions.
 *
 * @return array
 *   A Batch API array suitable to pass to batch_set().
 */
function webform_results_export_batch($node, $format = 'delimited', $options = array()) {
  $defaults = webform_results_download_default_options($node, $format);
  $options += $defaults;
  $options['range'] += $defaults['range'];

  return array(
    'operations' => array(
      array('webform_results_batch_bof', array($node, $format, $options)),
      array('webform_results_batch_headers', array($node, $format, $options)),
      array('webform_results_batch_rows', array($node, $format, $options)),
      array('webform_results_batch_eof', array($node, $format, $options)),
      array('webform_results_batch_post_process', array($node, $format, $options)),
      array('webform_results_batch_results', array($node, $format, $options)),
    ),
    'finished' => 'webform_results_batch_finished',
    'title' => t('Exporting submissions'),
    'init_message' => t('Creating export file'),
    'error_message' => t('The export file could not be created because an error occurred.'),
    'file' => drupal_get_path('module', 'webform') . '/includes/webform.report.inc',
  );
}

/**
 * Print the header rows for the downloadable webform data.
 *
 * @param $node
 *   The webform node on which to generate the analysis.
 * @param array $options
 *   A list of options that define the output format. These are generally passed
 *   through from the GUI interface.
 */
function webform_results_download_headers($node, $options) {
  module_load_include('inc', 'webform', 'includes/webform.components');
  $submission_information = webform_results_download_submission_information($node, $options);

  // Fill in the header for the submission information (if any).
  $header[2] = $header[1] = $header[0] = count($submission_information) ? array_fill(0, count($submission_information), '') : array();
  if (count($submission_information)) {
    $header[0][0] = $node->title;

    if ($options['header_keys']) {
      $header[1][0] = t('submission_details');
      $submission_information_headers = array_keys($submission_information);
    }
    else {
      $header[1][0] = t('Submission Details');
      $submission_information_headers = array_values($submission_information);
    }
    foreach ($submission_information_headers as $column => $label) {
      $header[2][$column] = $label;
    }
  }

  // Compile header information for components.
  foreach ($options['components'] as $cid) {
    if (isset($node->webform['components'][$cid])) {
      $component = $node->webform['components'][$cid];

      // Let each component determine its headers.
      if (webform_component_feature($component['type'], 'csv')) {
        $component_header = (array) webform_component_invoke($component['type'], 'csv_headers', $component, $options);
        // Allow modules to modify the component CSV header.
        drupal_alter('webform_csv_header', $component_header, $component);

        // Merge component CSV header to overall CSV header
        $header[0] = array_merge($header[0], (array) $component_header[0]);
        $header[1] = array_merge($header[1], (array) $component_header[1]);
        $header[2] = array_merge($header[2], (array) $component_header[2]);
      }
    }
  }

  return $header;
}

/**
 * Returns rows of downloadable webform data.
 *
 * @param $node
 *   The webform node on which to generate the analysis.
 * @param array $options
 *   A list of options that define the output format. These are generally passed
 *   through from the GUI interface.
 * @param $serial_start
 *   The starting position for the Serial column in the output.
 *
 * @return $rows
 *   An array of rows built according to the provided $serial_start and
 *   $pager_count variables. Note that the current page number is determined
 *   by the super-global $_GET['page'] variable.
 */
function webform_results_download_rows($node, $options, $serial_start = 0) {
  module_load_include('inc', 'webform', 'includes/webform.components');

  // Get all the required submissions for the download.
  $filters['nid'] = $node->nid;
  if (isset($options['sids'])){
    $filters['sid'] = $options['sids'];
  }
  elseif (!empty($options['completion_type']) && $options['completion_type'] !== 'all') {
    $filters['is_draft'] = (int) ($options['completion_type'] === 'draft');
  }

  $submissions = webform_get_submissions($filters, NULL);
  $submission_information = webform_results_download_submission_information($node, $options);

  // Generate a row for each submission.
  $row_count = 0;
  $sid = 0;
  $rows = array();
  foreach ($submissions as $sid => $submission) {
    $row_count++;

    $row = array();
    // Add submission information.
    foreach (array_keys($submission_information) as $token) {
      $cell = module_invoke_all('webform_results_download_submission_information_data', $token, $submission, $options, $serial_start, $row_count);
      $context = array('token' => $token, 'submission' => $submission, 'options' => $options, 'serial_start' => $serial_start, 'row_count' => $row_count);
      drupal_alter('webform_results_download_submission_information_data', $cell, $context);
      // implode() to ensure everything from a single value goes into one column, even if more than one module responds to this item.
      $row[] = implode(', ', $cell);
    }

    foreach ($options['components'] as $cid) {
      if (isset($node->webform['components'][$cid])) {
        $component = $node->webform['components'][$cid];
        // Let each component add its data.
        $raw_data = isset($submission->data[$cid]) ? $submission->data[$cid] : NULL;
        if (webform_component_feature($component['type'], 'csv')) {
          $data = webform_component_invoke($component['type'], 'csv_data', $component, $options, $raw_data);

          // Allow modules to modify the CSV data.
          drupal_alter('webform_csv_data', $data, $component, $submission);

          if (is_array($data)) {
            $row = array_merge($row, array_values($data));
          }
          else {
            $row[] = isset($data) ? $data : '';
          }
        }
      }
    }

    $rows[$serial_start + $row_count] = $row;
  }

  return $rows;
}

/**
 * Default columns for submission information.
 *
 * By default all exports have several columns of generic information that
 * applies to all submissions. This function returns the list of generic columns
 * plus columns added by other modules.
 *
 * @param $options
 *   Filter down the list of columns based on a provided column list.
 */
function webform_results_download_submission_information($node, $options = array()) {
  $submission_information = module_invoke_all('webform_results_download_submission_information_info');
  drupal_alter('webform_results_download_submission_information_info', $submission_information);

  if (isset($options['components'])) {
    foreach ($submission_information as $key => $label) {
      if (!in_array($key, $options['components'])) {
        unset($submission_information[$key]);
      }
    }
  }

  return $submission_information;
}

/**
 * Implements hook_webform_results_download_submission_information_info().
 */
function webform_webform_results_download_submission_information_info() {
  return array(
    'webform_serial' => t('Serial'),
    'webform_sid' => t('SID'),
    'webform_time' => t('Time'),
    'webform_draft' => t('Draft'),
    'webform_ip_address' => t('IP Address'),
    'webform_uid' => t('UID'),
    'webform_username' => t('Username'),
  );
}

/**
 * Implements hook_webform_results_download_submission_information_data().
 */
function webform_webform_results_download_submission_information_data($token, $submission, array $options, $serial_start, $row_count) {
  switch ($token) {
    case 'webform_serial':
      return $submission->serial;
    case 'webform_sid':
      return $submission->sid;
    case 'webform_time':
      // Return timestamp in local time (not UTC).
      if (!empty($options['iso8601_date'])) {
        return format_date($submission->submitted, 'custom', 'Y-m-d\TH:i:s');
      }
      else {
        return format_date($submission->submitted, 'short');
      }
    case 'webform_draft':
      return $submission->is_draft;
    case 'webform_ip_address':
      return $submission->remote_addr;
    case 'webform_uid':
      return $submission->uid;
    case 'webform_username':
      return $submission->name;
  }
}

/**
 * Get options for creating downloadable versions of the webform data.
 *
 * @param $node
 *   The webform node on which to generate the analysis.
 * @param string $format
 *   The export format being used.
 * @param array $options
 *   A list of options that define the output format. These are generally passed
 *   through from the GUI interface.
 */
function webform_results_download_default_options($node, $format) {
  $submission_information = webform_results_download_submission_information($node);

  $options = array(
    'delimiter' => webform_variable_get('webform_csv_delimiter'),
    'components' => array_merge(array_keys($submission_information), array_keys(webform_component_list($node, 'csv', TRUE))),
    'header_keys' => 0,
    'select_keys' => 0,
    'select_format' => 'separate',
    'range' => array(
      'range_type' => 'all',
      'completion_type' => 'all',
    ),
  );

  // Allow exporters to merge in additional options.
  $exporter_information = webform_export_fetch_definition($format);
  if (isset($exporter_information['options'])) {
    $options = array_merge($options, $exporter_information['options']);
  }

  return $options;
}

/**
 * Send a generated webform results file to the user's browser.
 *
 * @param $node
 *   The webform node.
 * @param $export_info
 *   Export information array retrieved from webform_results_export().
 */
function webform_results_download($node, $export_info) {
  global $user;

  // If the exporter provides a custom download method, use that.
  if (method_exists($export_info['exporter'], 'download')) {
    $export_info['exporter']->download($node);
  }
  // Otherwise use the set_headers() method to set headers and then read in the
  // file directly. Delete it when complete.
  else {
    $export_name = _webform_safe_name($node->title);
    if (!strlen($export_name)) {
      $export_name  = t('Untitled');
    }
    $export_info['exporter']->set_headers($export_name);
    @readfile($export_info['file_name']);  // The @ makes it silent.
    @unlink($export_info['file_name']);  // Clean up, the @ makes it silent.
  }

  // Update user last downloaded sid if required.
  if ((isset($export_info['options']['sids']) || $export_info['options']['range']['range_type'] != 'range') && !empty($export_info['last_sid'])) {
    // Delete existing record.
    db_delete('webform_last_download')
      ->condition('nid', $node->nid)
      ->condition('uid', $user->uid)
      ->execute();
    // Write new record.
    db_insert('webform_last_download')
      ->fields(array(
        'nid' => $node->nid,
        'uid' => $user->uid,
        'sid' => $export_info['last_sid'],
        'requested' => REQUEST_TIME,
      ))
      ->execute();
  }

  exit();
}

/**
 * Menu callback; Download an exported file.
 *
 * This callabck requires that an export file be already generated by a batch
 * process. The $_SESSION settings are usually put in place by the
 * webform_results_export_results() function.
 *
 * @param $node
 *   The webform $node whose export file is being downloaded.
 *
 * @return
 *   Either an export file is downloaded with the appropriate HTTP headers set,
 *   or an error page is shown if now export is available to download.
 */
function webform_results_download_callback($node) {
  if (isset($_SESSION['webform_export_info'])) {
    module_load_include('inc', 'webform', 'includes/webform.export');

    $export_info = $_SESSION['webform_export_info'];
    $export_info['exporter'] = webform_export_create_handler($export_info['format'], $export_info['options']);

    unset($_SESSION['webform_export_info']);
    if (isset($_COOKIE['webform_export_info'])) {
      unset($_COOKIE['webform_export_info']);
      $params = session_get_cookie_params();
      setcookie('webform_export_info', '', -1, $params['path'], $params['domain'], $params['secure'], $params['httponly']);
    }

    webform_results_download($node, $export_info);
  }
  else {
    return t('No export file ready for download. The file may have already been downloaded by your browser. Visit the <a href="!href">download export form</a> to create a new export.', array('!href' => url('node/' . $node->nid . '/webform-results/download')));
  }
}

/**
 * Batch API callback; Write the opening byte in the export file.
 */
function webform_results_batch_bof($node, $format = 'delimited', $options = array(), &$context) {
  module_load_include('inc', 'webform', 'includes/webform.export');

  $exporter = webform_export_create_handler($format, $options);
  $handle = fopen($options['file_name'], 'w');
  if (!$handle) {
    return;
  }
  $exporter->bof($handle);
  @fclose($handle);
}

/**
 * Batch API callback; Write the headers of the export to the export file.
 */
function webform_results_batch_headers($node, $format = 'delimited', $options = array(), &$context) {
  module_load_include('inc', 'webform', 'includes/webform.export');

  $exporter = webform_export_create_handler($format, $options);
  $handle = fopen($options['file_name'], 'a');
  if (!$handle) {
    return;
  }
  $headers = webform_results_download_headers($node, $options);
  $row_count = 0;
  $col_count = 0;
  foreach ($headers as $row) {
    // Output header if header_keys is non-negative. -1 means no headers.
    if ($options['header_keys'] >= 0) {
      $exporter->add_row($handle, $row, $row_count);
      $row_count++;
    }
    $col_count = count($row) > $col_count ? count($row) : $col_count;
  }
  $context['results']['row_count'] = $row_count;
  $context['results']['col_count'] = $col_count;
  @fclose($handle);
}

/**
 * Batch API callback; Write the rows of the export to the export file.
 */
function webform_results_batch_rows($node, $format = 'delimited', $options = array(), &$context) {
  module_load_include('inc', 'webform', 'includes/webform.export');

  if (!isset($context['sandbox']['batch_number'])) {
    $context['sandbox']['batch_number'] = 0;
    $sids = isset($options['sids']) ? $options['sids'] : webform_download_sids($node->nid, $options['range']);
    $context['sandbox']['sid_count'] = count($sids);
    $context['sandbox']['batch_max'] = max(1, ceil($context['sandbox']['sid_count'] / $options['range']['batch_size']));
    $context['sandbox']['serial'] = 0;
    $context['sandbox']['last_sid'] = 0;
  }
  // Retrieve the submissions for this batch process.
  $options['range']['batch_number'] = $context['sandbox']['batch_number'];
  $sids = isset($options['sids']) ? $options['sids'] : webform_download_sids($node->nid, $options['range']);
  $options['sids'] = $sids;
  $rows = webform_results_download_rows($node, $options, $context['sandbox']['serial']);

  // Write these submissions to the file.
  $exporter = webform_export_create_handler($format, $options);
  $handle = fopen($options['file_name'], 'a');
  if (!$handle) {
    return;
  }
  foreach ($rows as $row) {
    $exporter->add_row($handle, $row, $context['results']['row_count']);
    $context['results']['row_count']++;
  }
  $context['sandbox']['serial'] += count($sids);
  $context['sandbox']['last_sid'] = end($sids);
  $context['sandbox']['batch_number']++;

  @fclose($handle);

  // Display status message
  $context['message'] = t('Exported @count of @total submissions...', array('@count' => $context['sandbox']['serial'], '@total' => $context['sandbox']['sid_count']));
  $context['finished'] = $context['sandbox']['batch_number'] < $context['sandbox']['batch_max']
                            ? $context['sandbox']['batch_number'] / $context['sandbox']['batch_max']
                            : 1.0;
  $context['results']['last_sid'] = $context['sandbox']['last_sid'];
}

/**
 * Batch API callback; Write the closing bytes in the export file.
 */
function webform_results_batch_eof($node, $format = 'delimited', $options = array(), &$context) {
  module_load_include('inc', 'webform', 'includes/webform.export');

  $exporter = webform_export_create_handler($format, $options);

  // We open the file for reading and writing, rather than just appending for
  // exporters that need to adjust the beginning of the files as well as the
  // end, i.e. webform_exporter_excel_xlsx::eof().
  $handle = fopen($options['file_name'], 'r+');
  if (!$handle) {
    return;
  }
  fseek($handle, 0, SEEK_END); // Move pointer to the end of the file.
  $exporter->eof($handle, $context['results']['row_count'], $context['results']['col_count']);
  @fclose($handle);
}

/**
 * Batch API callback; Do any last processing on the finished export.
 */
function webform_results_batch_post_process($node, $format = 'delimited', $options = array(), &$context) {
  module_load_include('inc', 'webform', 'includes/webform.export');

  $context['results']['node'] = $node;
  $context['results']['file_name'] = $options['file_name'];

  $exporter = webform_export_create_handler($format, $options);
  $exporter->post_process($context['results']);
}

/**
 * Batch API callback; Set the $_SESSION variables used to download the file.
 *
 * Because we want the user to be returned to the main form first, we have to
 * temporarily store information about the created file, send the user to the
 * form, then use JavaScript to request node/x/webform-results/download-file,
 * which will execute webform_results_download_file().
 */
function webform_results_batch_results($node, $format, $options, &$context) {

  $export_info = array(
    'format' => $format,
    'options' => $options,
    'file_name' => $context['results']['file_name'],
    'row_count' => $context['results']['row_count'],
    'last_sid' => $context['results']['last_sid'],
  );

  // Set a session variable containing the information referencing the exported
  // file. A cookie is also set to allow the browser to ensure the redirect
  // to the file only happens one time.
  $_SESSION['webform_export_info'] = $export_info;
  $params = session_get_cookie_params();
  setcookie('webform_export_info', '1', REQUEST_TIME + 120, $params['path'], $params['domain'], $params['secure'], FALSE);
}

/**
 * Batch API completion callback; Display completion message and cleanup.
 */
function webform_results_batch_finished($success, $results, $operations) {
  if ($success) {
    $download_url = url('node/' . $results['node']->nid . '/webform-results/download-file');
    drupal_set_message(t('Export creation complete. Your download should begin now. If it does not start, <a href="!href">download the file here</a>. This file may only be downloaded once.', array('!href' => $download_url)));
  }
  else {
    drupal_set_message(t('An error occurred while generating the export file.'));
    if (isset($results['file_name']) && is_file($results['file_name'])) {
      @unlink($results['file_name']);
    }
  }
}

/**
 * Provides a simple analysis of all submissions to a webform.
 *
 * @param $node
 *   The webform node on which to generate the analysis.
 * @param $sids
 *   An array of submission IDs to which this analysis may be filtered. May be
 *   used to generate results that are per-user or other groups of submissions.
 * @param $analysis_component
 *   A webform component. If passed in, additional information may be returned
 *   relating specifically to that component's analysis, such as a list of
 *   "Other" values within a select list.
 */
function webform_results_analysis($node, $sids = array(), $analysis_component = NULL) {
  if (!is_array($sids)) {
    $sids = array();
  }

  // Build a renderable for the content of this page.
  $analysis = array(
    '#theme' => array('webform_analysis__' . $node->nid, 'webform_analysis'),
    '#node' => $node,
    '#component' => $analysis_component,
  );

  // If showing all components, display selection form.
  if (!$analysis_component) {
    $analysis['form'] = drupal_get_form('webform_analysis_components_form', $node);
  }

  // Add all the components to the analysis renderable array.
  $components = isset($analysis_component) ? array($analysis_component['cid']) : webform_analysis_enabled_components($node);
  foreach ($components as $cid) {
    // Do component specific call.
    $component = $node->webform['components'][$cid];
    if ($data = webform_component_invoke($component['type'], 'analysis', $component, $sids, isset($analysis_component))) {
      drupal_alter('webform_analysis_component_data', $data, $node, $component);
      $analysis['data'][$cid] = array(
        '#theme' => array('webform_analysis_component__' . $node->nid . '__' . $cid, 'webform_analysis_component__' . $node->nid, 'webform_analysis_component'),
        '#node' => $node,
        '#component' => $component,
        '#data' => $data,
      );
      $analysis['data'][$cid]['basic'] = array(
        '#theme' => array('webform_analysis_component_basic__' . $node->nid . '__' . $cid, 'webform_analysis_component_basic__' . $node->nid, 'webform_analysis_component_basic'),
        '#component' => $component,
        '#data' => $data,
      );
    }
  }

  drupal_alter('webform_analysis', $analysis);
  return drupal_render($analysis);
}

/**
 * Prerender function for webform-analysis.tpl.php.
 */
function template_preprocess_webform_analysis(&$variables) {
  $analysis = $variables['analysis'];
  $variables['node'] = $analysis['#node'];
  $variables['component'] = $analysis['#component'];
}

/**
 * Prerender function for webform-analysis-component.tpl.php.
 */
function template_preprocess_webform_analysis_component(&$variables) {
  $component_analysis = $variables['component_analysis'];
  $variables['node'] = $component_analysis['#node'];
  $variables['component'] = $component_analysis['#component'];

  // Ensure defaults.
  $variables['component_analysis']['#data'] += array(
    'table_header' => NULL,
    'table_rows' => array(),
    'other_data' => array(),
  );
  $variables['classes_array'][] = 'webform-analysis-component-' . $variables['component']['type'];
  $variables['classes_array'][] = 'webform-analysis-component--' . str_replace('_', '-', implode('--', webform_component_parent_keys($variables['node'], $variables['component'])));
}

/**
 * Render an individual component's analysis data in a table.
 *
 * @param $variables
 *   An array of theming variables for this theme function. Included keys:
 *   - $component: The component whose analysis is being rendered.
 *   - $data: An array of array containing the analysis data. Contains the keys:
 *     - table_header: If this table has more than a single column, an array
 *       of header labels.
 *     - table_rows: If this component has a table that should be rendered, an
 *       array of values
 */
function theme_webform_analysis_component_basic($variables) {
  $data = $variables['data'];

  // Ensure defaults.
  $data += array(
    'table_header' => NULL,
    'table_rows' => array(),
    'other_data' => array(),
  );

  // Combine the "other data" into the table rows by default.
  if (is_array($data['other_data'])) {
    foreach ($data['other_data'] as $other_data) {
      if (is_array($other_data)) {
        $data['table_rows'][] = $other_data;
      }
      else {
        $data['table_rows'][] = array(array(
          'colspan' => 2,
          'data' => $other_data,
        ));
      }
    }
  }
  elseif (strlen($data['other_data'])) {
    $data['table_rows'][] = array(
      'colspan' => 2,
      'data' => $data['other_data'],
    );
  }

  return theme('table', array(
    'header' => $data['table_header'],
    'rows' => $data['table_rows'],
    'sticky' => FALSE,
    'attributes' => array('class' => array('webform-analysis-table')),
  ));
}

/**
 * Return a list of components that should be displayed for analysis.
 *
 * @param $node
 *   The node whose components' data is being analyzed.
 * @return array
 *   An array of component IDs.
 */
function webform_analysis_enabled_components($node) {
  $cids = array();
  foreach ($node->webform['components'] as $cid => $component) {
    if (!empty($component['extra']['analysis'])) {
      $cids[] = $cid;
    }
  }
  return $cids;
}

/**
 * Form for selecting which components should be shown on the analysis page.
 */
function webform_analysis_components_form($form, &$form_state, $node) {
  form_load_include($form_state, 'inc', 'webform', 'includes/webform.components');
  $form['#node'] = $node;

  $component_list = webform_component_list($node, 'analysis', TRUE);
  $enabled_components = webform_analysis_enabled_components($node);
  if (empty($component_list)) {
    $help = t('No components have added that support analysis. <a href="!url">Add components to your form</a> to see calculated data.', array('!url' => url('node/' . $node->nid . '/webform')));
  }
  elseif (empty($enabled_components)) {
    $help = t('No components have analysis enabled in this form. Enable analysis under the "Add analysis components" fieldset.');
  }
  else {
    $help = t('This page shows analysis of submitted data, such as the number of submissions per component value, calculations, and averages. Additional components may be added under the "Add analysis components" fieldset.');
  }

  $form['help'] = array(
    '#markup' => '<p>' . $help . '</p>',
    '#access' => !empty($help),
    '#weight' => -100,
  );

  $form['components'] = array(
    '#type' => 'select',
    '#title' => t('Add analysis components'),
    '#options' => $component_list,
    '#default_value' => $enabled_components,
    '#multiple' => TRUE,
    '#size' => 10,
    '#description' => t('The selected components will be included on the analysis page.'),
    '#process' => array('webform_component_select'),
    '#access' => count($component_list),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#access' => count($component_list),
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update analysis display'),
  );

  return $form;
}

/**
 * Submit handler for webform_analysis_components_form().
 */
function webform_analysis_components_form_submit($form, $form_state) {
  $node = $form['#node'];
  foreach ($form_state['values']['components'] as $cid => $enabled) {
    $node->webform['components'][$cid]['extra']['analysis'] = (bool) $enabled;
  }
  node_save($node);
}

/**
 * Output the content of the Analysis page.
 *
 * @see webform_results_analysis()
 */
function theme_webform_results_analysis($variables) {
  $node = $variables['node'];
  $data = $variables['data'];
  $sids = $variables['sids'];
  $analysis_component = $variables['component'];

  $rows = array();
  $question_number = 0;
  $single = isset($analysis_component);

  $header = array(
    $single ? $analysis_component['name'] : t('Q'),
    array('data' => $single ? '&nbsp;' : t('responses'), 'colspan' => '10')
  );

  foreach ($data as $cid => $row_data) {
    $question_number++;

    if (is_array($row_data)) {
      $row = array();
      if (!$single) {
        $row['data'][] = array('data' => '<strong>' . $question_number . '</strong>', 'rowspan' => count($row_data) + 1, 'valign' => 'top');
        $row['data'][] = array('data' => '<strong>' . check_plain($node->webform['components'][$cid]['name']) . '</strong>', 'colspan' => '10');
        $row['class'][] = 'webform-results-question';
      }
      $rows = array_merge($rows, array_merge(array($row), $row_data));
    }
  }

  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('There are no submissions for this form. <a href="!url">View this form</a>.', array('!url' => url('node/' . $node->nid))), 'colspan' => 20));
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => FALSE, 'attributes' => array('class' => array('webform-results-analysis'))));
}

/**
 * Given a set of range options, retrieve a set of SIDs for a webform node.
 */
function webform_download_sids($nid, $range_options, $uid = NULL) {
  $query = db_select('webform_submissions', 'ws')
    ->fields('ws', array('sid'))
    ->condition('nid', $nid)
    ->addTag('webform_download_sids');

  switch ($range_options['range_type']) {
    case 'all':
      // All Submissions.
      $query->orderBy('sid', 'ASC');
      break;
    case 'new':
      // All Since Last Download.
      $download_info = webform_download_last_download_info($nid, $uid);
      $last_sid = $download_info ? $download_info['sid'] : 0;
      $query
        ->condition('sid', $last_sid, '>')
        ->orderBy('sid', 'ASC');
      break;
    case 'latest':
      // Last x Submissions.
      $start_sid = webform_download_latest_start_sid($nid, $range_options['latest'], $range_options['completion_type']);
      $query->condition('sid', $start_sid, '>=');
      break;
    case 'range':
      // Submissions Start-End.
      $query->condition('sid', $range_options['start'], '>=');
      if ($range_options['end']){
        $query->condition('sid', $range_options['end'], '<=');
      }
      $query->orderBy('sid', 'ASC');
      break;
    case 'range_serial':
      // Submissions Start-End, using serial numbers.
      $query->condition('serial', $range_options['start'], '>=');
      if ($range_options['end']){
        $query->condition('serial', $range_options['end'], '<=');
      }
      $query->orderBy('serial', 'ASC');
      break;
  }

  // Filter down to draft or finished submissions.
  if (!empty($range_options['completion_type']) && $range_options['completion_type'] !== 'all') {
    $query->condition('is_draft', (int) ($range_options['completion_type'] === 'draft'));
  }

  // Set batch properties. Unfortunately the pager query class reads directly
  // from $_GET['page'] to determine the current page number, so we have to set
  // this super-global and then restore it after executing the query.
  if (isset($range_options['batch_number']) && isset($range_options['batch_size'])) {
    if (isset($_GET['page'])) {
      $current_page = $_GET['page'];
    }
    $_GET['page'] = $range_options['batch_number'];
    $query = $query->extend('PagerDefault');
    $query->limit($range_options['batch_size']);
    $query->element(0);
  }

  $sids = $query->execute()->fetchCol();

  // Restore the current page number (if any).
  if (isset($current_page)) {
    $_GET['page'] = $current_page;
  }

  return $sids;
}

/**
 * Get this user's last download information, including the SID and timestamp.
 *
 * This function provides an array of information about the last download that
 * a user had for a particular Webform node. Currently it only returns an array
 * with two keys:
 *  - sid: The last submission ID that was downloaded.
 *  - requested: The timestamp of the last download request.
 *
 * @param $nid
 *   The Webform NID.
 * @param $uid
 *   The user account ID for which to retrieve download information.
 * @return
 *   An array of download information or FALSE if this user has never downloaded
 *   results for this particular node.
 */
function webform_download_last_download_info($nid, $uid = NULL) {
  $uid = isset($uid) ? $uid : $GLOBALS['user']->uid;

  $query = db_select('webform_last_download', 'wld');
  $query->leftJoin('webform_submissions', 'wfs', 'wld.sid = wfs.sid');
  $info = $query
    ->fields('wld')
    ->fields('wfs', array('serial'))
    ->condition('wld.nid', $nid)
    ->condition('wld.uid', $uid)
    ->execute()
    ->fetchAssoc();

  return $info;
}

/**
 * Get an SID based a requested latest count.
 *
 * @param int $nid
 *   The webform NID.
 * @param int $latest_count
 *   The latest count on which the SID will be retrieved.
 * @param string $completion_type
 *   The completion type, either "finished", "draft", or "all".
 * @return
 *   The submission ID that starts the latest sequence of submissions.
 */
function webform_download_latest_start_sid($nid, $latest_count, $completion_type = 'all') {
  // TODO: Find a more efficient DBTNG query to retrieve this number.
  $query = db_select('webform_submissions', 'ws')
    ->fields('ws', array('sid'))
    ->condition('nid', $nid)
    ->orderBy('ws.sid', 'DESC')
    ->range(0, $latest_count)
    ->addTag('webform_download_latest_start_sid');

  if ($completion_type !== 'all') {
    $query->condition('is_draft', (int) ($completion_type === 'draft'));
  }

  $latest_sids = $query->execute()->fetchCol();
  return min($latest_sids);
}
