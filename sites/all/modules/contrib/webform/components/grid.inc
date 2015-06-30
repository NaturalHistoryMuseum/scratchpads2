<?php

/**
 * @file
 * Webform module grid component.
 */

// Grid depends on functions provided by select.
webform_component_include('select');

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_grid() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'options' => '',
      'questions' => '',
      'optrand' => 0,
      'qrand' => 0,
      'unique' => 0,
      'title_display' => 0,
      'custom_option_keys' => 0,
      'custom_question_keys' => 0,
      'sticky' => TRUE,
      'description' => '',
      'private' => FALSE,
      'analysis' => TRUE,
    ),
  );
}


/**
 * Implements _webform_theme_component().
 */
function _webform_theme_grid() {
  return array(
    'webform_grid' => array(
      'render element' => 'element',
      'file' => 'components/grid.inc',
    ),
    'webform_display_grid' => array(
      'render element' => 'element',
      'file' => 'components/grid.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_grid($component) {
  $form = array();

  if (module_exists('options_element')) {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Options'),
      '#collapsible' => TRUE,
      '#description' => t('Options to select across the top. Usually these are ratings such as "poor" through "excellent" or "strongly disagree" through "strongly agree".'),
      '#attributes' => array('class' => array('webform-options-element')),
      '#element_validate' => array('_webform_edit_validate_options'),
    );
    $form['options']['options'] = array(
      '#type' => 'options',
      '#options' => _webform_select_options_from_text($component['extra']['options'], TRUE),
      '#default_value' => $component['value'],
      '#default_value_allowed' => TRUE,
      '#optgroups' => FALSE,
      '#key_type' => 'mixed',
      '#key_type_toggle' => t('Customize option keys (Advanced)'),
      '#key_type_toggled' => $component['extra']['custom_option_keys'],
      '#default_value_pattern' => '^%.+\[.+\]$',
    );

    $form['questions'] = array(
      '#type' => 'fieldset',
      '#title' => t('Questions'),
      '#collapsible' => TRUE,
      '#description' => t('Questions list down the side of the grid.'),
      '#attributes' => array('class' => array('webform-options-element')),
      '#element_validate' => array('_webform_edit_validate_options'),
    );
    $form['questions']['options'] = array(
      '#type' => 'options',
      '#options' => _webform_select_options_from_text($component['extra']['questions'], TRUE),
      '#optgroups' => FALSE,
      '#default_value' => FALSE,
      '#default_value_allowed' => FALSE,
      '#key_type' => 'mixed',
      '#key_type_toggle' => t('Customize question keys (Advanced)'),
      '#key_type_toggled' => $component['extra']['custom_question_keys'],
    );
  }
  else {
    $form['extra']['options'] = array(
      '#type' => 'textarea',
      '#title' => t('Options'),
      '#default_value' => $component['extra']['options'],
      '#description' => t('Options to select across the top. One option per line. <strong>Key-value pairs MUST be specified as "safe_key|Some readable option"</strong>. Use of only alphanumeric characters and underscores is recommended in keys.') . ' ' . theme('webform_token_help'),
      '#cols' => 60,
      '#rows' => 5,
      '#weight' => -3,
      '#required' => TRUE,
      '#wysiwyg' => FALSE,
      '#element_validate' => array('_webform_edit_validate_select'),
    );
    $form['extra']['questions'] = array(
      '#type' => 'textarea',
      '#title' => t('Questions'),
      '#default_value' => $component['extra']['questions'],
      '#description' => t('Questions list down the side of the grid. One question per line. <strong>Key-value pairs MUST be specified as "safe_key|Some readable question"</strong>. For a heading column on the right, append "|" and the right-side heading. Use of only alphanumeric characters and underscores is recommended in keys.') . ' ' . theme('webform_token_help'),
      '#cols' => 60,
      '#rows' => 5,
      '#weight' => -2,
      '#required' => TRUE,
      '#wysiwyg' => FALSE,
      '#element_validate' => array('_webform_edit_validate_select'),
    );
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Default value'),
      '#default_value' => $component['value'],
      '#description' => t('The default option of the grid identified by its key.') . ' ' . theme('webform_token_help'),
      '#size' => 60,
      '#maxlength' => 1024,
      '#weight' => 1,
    );
  }

  $form['display']['optrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Options'),
    '#default_value' => $component['extra']['optrand'],
    '#description' => t('Randomizes the order of options on the top when they are displayed in the form.'),
    '#parents' => array('extra', 'optrand')
  );
  $form['display']['qrand'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Questions'),
    '#default_value' => $component['extra']['qrand'],
    '#description' => t('Randomize the order of the questions on the side when they are displayed in the form.'),
    '#parents' => array('extra', 'qrand')
  );
  $form['display']['sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky table header'),
    '#default_value' => $component['extra']['sticky'],
    '#description' => t('Use a sticky (non-scrolling) table header.'),
    '#parents' => array('extra', 'sticky')
  );

  $form['validation']['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique'),
    '#return_value' => 1,
    '#description' => t('Check that all entered values for this field are unique. The same value is not allowed to be used twice.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['unique'],
    '#parents' => array('extra', 'unique'),
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_grid($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  if ($filter) {
    $questions = _webform_select_replace_tokens($questions, $node);
    $options = _webform_select_replace_tokens($options, $node);
  }

  $element = array(
    '#type' => 'webform_grid',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#grid_questions' => $questions,
    '#grid_options' => $options,
    '#default_value' => isset($value) || !strlen($component['value']) ? $value : array_fill_keys(array_keys($questions), $component['value']),
    '#grid_default' => $component['value'],
    '#optrand' => $component['extra']['optrand'],
    '#qrand' => $component['extra']['qrand'],
    '#sticky' => $component['extra']['sticky'],
    '#theme' => 'webform_grid',
    '#theme_wrappers' => array('webform_element'),
    '#process' => array('webform_expand_grid'),
    '#translatable' => array('title', 'description', 'grid_options', 'grid_questions'),
  );

  // Enforce uniqueness.
  if ($component['extra']['unique']) {
    $element['#element_validate'][] = '_webform_edit_grid_unique_validate';
  }

  return $element;
}

/**
 * A Form API #process function for Webform grid fields.
 */
function webform_expand_grid($element) {
  $options = $element['#grid_options'];
  $questions = $element['#grid_questions'];

  if (!empty($element['#optrand'])) {
    _webform_shuffle_options($options);
  }

  if (!empty($element['#qrand'])) {
    _webform_shuffle_options($questions);
  }

  foreach ($questions as $key => $question) {
    if ($question != '') {
      $element[$key] = array(
        '#title' => $question,
        '#required' => $element['#required'],
        '#options' => $options,
        '#type' => 'radios',
        '#process' => array('form_process_radios', 'webform_expand_select_ids'),

        // Webform handles validation manually.
        '#validated' => TRUE,
        '#webform_validated' => FALSE,
        '#translatable' => array('title'),
      );

      // Add HTML5 required attribute, if needed.
      if ($element['#required']) {
        $element[$key]['#attributes']['required'] = 'required';
      }
    }
  }

  $value = isset($element['#default_value']) ? $element['#default_value'] : array();
  foreach (element_children($element) as $key) {
    if (isset($value[$key])) {
      $element[$key]['#default_value'] = ($value[$key] !== '') ? $value[$key] : NULL;
    }
    else {
      $element[$key]['#default_value'] = NULL;
    }
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_grid($component, $value, $format = 'html') {
  $node = node_load($component['nid']);
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $questions = _webform_select_replace_tokens($questions, $node);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  $options = _webform_select_replace_tokens($options, $node);

  $element = array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#format' => $format,
    '#grid_questions' => $questions,
    '#grid_options' => $options,
    '#sticky' => $component['extra']['sticky'],
    '#theme' => 'webform_display_grid',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#sorted' => TRUE,
    '#translatable' => array('#title', '#grid_questions', '#grid_options'),
  );

  foreach ($questions as $key => $question) {
    if ($question !== '') {
      $element[$key] = array(
        '#title' => $question,
        '#value' => isset($value[$key]) ? $value[$key] : NULL,
        '#translatable' => array('#title', '#value'),
      );
    }
  }

  return $element;
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_grid($variables) {
  $element = $variables['element'];

  $component = $element['#webform_component'];
  $format = $element['#format'];

  if ($format == 'html') {
    $right_titles = _webform_grid_right_titles($element);
    $rows = array();
    $title = array('data' => '', 'class' => array('webform-grid-question'));
    $header = array($title);
    foreach ($element['#grid_options'] as $option) {
      $header[] = array('data' => webform_filter_xss($option), 'class' => array('checkbox', 'webform-grid-option'));
    }
    if ($right_titles) {
      $header[] = $title;
    }
    foreach ($element['#grid_questions'] as $question_key => $question) {
      $row = array();
      $questions = explode('|', $question, 2);
      $row[] = array('data' => webform_filter_xss($questions[0]), 'class' => array('webform-grid-question'));
      foreach ($element['#grid_options'] as $option_value => $option_label) {
        if (strcmp($element[$question_key]['#value'], $option_value) == 0) {
          $row[] = array('data' => '<strong>X</strong>', 'class' => array('checkbox', 'webform-grid-option'));
        }
        else {
          $row[] = array('data' => '&nbsp;', 'class' => array('checkbox', 'webform-grid-option'));
        }
      }
      if ($right_titles) {
        $row[] = array('data' => isset($questions[1]) ? webform_filter_xss($questions[1]) : '', 'class' => array('webform-grid-question'));
      }
      $rows[] = $row;
    }

    $option_count = count($header) - 1;
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => $element['#sticky'], 'attributes' => array('class' => array('webform-grid', 'webform-grid-' . $option_count))));
  }
  else {
    $items = array();
    foreach (element_children($element) as $key) {
      $items[] = ' - ' . _webform_grid_question_header($element[$key]['#title']) . ': ' .
                 (isset($element['#grid_options'][$element[$key]['#value']]) ? $element['#grid_options'][$element[$key]['#value']] : '');
    }
    $output = implode("\n", $items);
  }

  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_grid($component, $sids = array(), $single = FALSE, $join = NULL) {
  // Generate the list of options and questions.
  $node = node_load($component['nid']);
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $questions = _webform_select_replace_tokens($questions, $node);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  $options = _webform_select_replace_tokens($options, $node);

  // Generate a lookup table of results.
  $query = db_select('webform_submitted_data', 'wsd')
    ->fields('wsd', array('no', 'data'))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid'])
    ->condition('wsd.data', '', '<>')
    ->groupBy('wsd.no')
    ->groupBy('wsd.data');
  $query->addExpression('COUNT(wsd.sid)', 'datacount');

  if (count($sids)) {
    $query->condition('wsd.sid', $sids, 'IN');
  }

  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }

  $result = $query->execute();
  $counts = array();
  foreach ($result as $data) {
    $counts[$data->no][$data->data] = $data->datacount;
  }

  // Create an entire table to be put into the returned row.
  $rows = array();
  $header = array('');

  // Add options as a header row.
  foreach ($options as $option) {
    $header[] = webform_filter_xss($option);
  }

  // Add questions as each row.
  foreach ($questions as $qkey => $question) {
    $row = array(webform_filter_xss($question));
    foreach ($options as $okey => $option) {
      $row[] = !empty($counts[$qkey][$okey]) ? $counts[$qkey][$okey] : 0;
    }
    $rows[] = $row;
  }

  return array(
    'table_header' => $header,
    'table_rows' => $rows,
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_grid($component, $value) {
  $node = node_load($component['nid']);
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $questions = _webform_select_replace_tokens($questions, $node);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  $options = _webform_select_replace_tokens($options, $node);

  $output = '';
  // Set the value as a single string.
  foreach ($questions as $key => $label) {
    if (isset($value[$key]) && isset($options[$value[$key]])) {
      $output .= webform_filter_xss(_webform_grid_question_header($label)) . ': ' . webform_filter_xss($options[$value[$key]]) . '<br />';
    }
  }

  return $output;
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_grid($component, $export_options) {
  $node = node_load($component['nid']);
  $items = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $items = _webform_select_replace_tokens($items, $node);

  $header = array();
  $header[0] = array('');
  $header[1] = array($export_options['header_keys'] ? $component['form_key'] : $component['name']);
  $count = 0;
  foreach ($items as $key => $item) {
    // Empty column per sub-field in main header.
    if ($count != 0) {
      $header[0][] = '';
      $header[1][] = '';
    }
    // The value for this option.
    $header[2][] = $export_options['header_keys'] ? $key : _webform_grid_question_header($item);
    $count++;
  }

  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_grid($component, $export_options, $value) {
  $node = node_load($component['nid']);
  $questions = _webform_select_options_from_text($component['extra']['questions'], TRUE);
  $questions = _webform_select_replace_tokens($questions, $node);
  $options = _webform_select_options_from_text($component['extra']['options'], TRUE);
  $options = _webform_select_replace_tokens($options, $node);

  $return = array();
  foreach ($questions as $key => $question) {
    if (isset($value[$key]) && isset($options[$value[$key]])) {
      $return[] = $export_options['select_keys'] ? $value[$key] : $options[$value[$key]];
    }
    else {
      $return[] = '';
    }
  }
  return $return;
}

/**
 * A Form API element validate function to check that all choices are unique.
 */
function _webform_edit_grid_unique_validate($element) {
  $nr_unique = count(array_unique($element['#value']));
  $nr_values = count($element['#value']);
  $nr_possible = count($element['#grid_options']);
  if (strlen($element['#grid_default']) && isset($element['#grid_options'][$element['#grid_default']])) {
    // A default is defined and is one of the options. Don't count default values
    // toward uniqueness.
    $nr_defaults = count(array_keys($element['#value'], $element['#grid_default']));
    if ($nr_defaults) {
      $nr_values -= $nr_defaults;
      $nr_unique--;
    }
  }
  if ($nr_unique < $nr_values && $nr_unique < $nr_possible) {
    // Fewer unique values than values means that at least one value is duplicated.
    // Fewer unique values than possible values means that there is a possible choice
    // that wasn't used.
    form_error($element, t('!title is not allowed to have the same answer for more than one question.', array('!title' => $element['#title'])));
  }
}

function theme_webform_grid($variables) {
  $element = $variables['element'];
  $right_titles = _webform_grid_right_titles($element);

  $rows = array();
  $title = array('data' => '', 'class' => array('webform-grid-question'));
  $header = array($title);
  // Set the header for the table.
  foreach ($element['#grid_options'] as $option) {
    $header[] = array('data' => webform_filter_xss($option), 'class' => array('checkbox', 'webform-grid-option'));
  }
  if ($right_titles) {
    $header[] = $title;
  }

  foreach (element_children($element) as $key) {
    $question_element = $element[$key];
    $question_titles = explode('|', $question_element['#title'], 2);

    // Create a row with the question title.
    $title = array('data' => webform_filter_xss($question_titles[0]), 'class' => array('webform-grid-question'));
    $row = array($title);

    // Render each radio button in the row.
    $radios = form_process_radios($question_element);
    foreach (element_children($radios) as $key) {
      $radios[$key]['#title'] = $question_element['#title'] . ' - ' . $radios[$key]['#title'];
      $radios[$key]['#title_display'] = 'invisible';
      $row[] = array('data' => drupal_render($radios[$key]), 'class' => array('checkbox', 'webform-grid-option'));
    }
    if ($right_titles) {
      $row[] = array('data' => isset($question_titles[1]) ? webform_filter_xss($question_titles[1]) : '', 'class' => array('webform-grid-question'));
    }
    $rows[] = $row;
  }

  $option_count = count($header) - 1;
  return theme('table', array('header' => $header, 'rows' => $rows, 'sticky' => $element['#sticky'], 'attributes' => array('class' => array('webform-grid', 'webform-grid-' . $option_count))));
}

/**
 * Determine if a right-side title column has been specified.
 */
function _webform_grid_right_titles($element) {
  foreach ($element['#grid_questions'] as $question_key => $question) {
    if (substr_count($question, '|')) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Create a question header for left, right or left/right question headers.
 */
function _webform_grid_question_header($text) {
  return implode('/', array_filter(explode('|', $text)));
}

