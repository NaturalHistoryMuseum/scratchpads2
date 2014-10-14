<?php

/**
 * @file
 * Create taxonomy csv export form and validate user input.
 */

/**
 * Invoke associated files.
 */
$module_dir = drupal_get_path('module', 'taxonomy_csv');
require_once($module_dir . '/taxonomy_csv.api.inc');

/**
 * Generates the taxonomy CSV export form.
 *
 * Form contain three fieldsets:
 * - 1. Which vocabulary to export ?
 * - 2. How to export (format of csv file) ?
 *   - 1. Csv delimiter
 *   - 2. Csv enclosure
 *   - 3. Csv end of line
 * - 3. Order and specific options
 *
 * @ingroup forms
 * @see taxonomy_csv_export_form_validate()
 * @see taxonomy_csv_export_form_submit()
 * @see taxonomy_csv_export_form_default_values_validate()
 * @see taxonomy_csv_export_form_default_values_submit()
 */
function taxonomy_csv_export_form($form, &$form_state) {
  // Remember previous values to use in particular when reloading form.
  // If not reloading form, use saved values if exist, else recommended ones.
  $list_recommended_values = _taxonomy_csv_values('export_default_ui');
  $list_previous_values = array();
  foreach ($list_recommended_values as $key => $value) {
    $list_previous_values[$key] = isset($form_state['values'][$key]) ?
        $form_state['values'][$key] :
        variable_get("taxonomy_csv_{$key}", $value);
  }

  $list_export_format = _taxonomy_csv_values('export_format');

  $list_export_delimiter = array(
    'comma'            => t('« , » (Comma)'),
    'semicolon'        => t('« ; » (Semicolon)'),
    'tabulation'       => t('«   » (Tabulation)'),
    'pipe'             => t('« | » (Pipe)'),
    'space'            => t('«   » (Space)'),
    'currency_sign'    => t('« ¤ » (Currency sign)'),
    'custom_delimiter' => t('Custom delimiter'),
  );

  $list_export_enclosure = array(
    'none'             => t('None'),
    'quotation'        => t('« " » (Quotation mark)'),
    'quote'            => t("« ' » (Quote)"),
    'custom_enclosure' => t('Custom enclosure'),
  );

  $list_export_line_ending = array(
    'Unix'             => t('Unix / Linux'),
    'Mac'              => t('Apple Mac'),
    'MS-DOS'           => t('Microsoft DOS'),
  );

  $list_export_order = array(
    'name'             => t('Alphabetic order'),
    'weight'           => t('Weight'),
    'tid'              => t('Internal order (tid)'),
  );

  // Build form.
  $form = array();

  $list_vocabularies = taxonomy_vocabulary_get_names();

  if (count($list_vocabularies) == 0) {
    $form['info'] = array(
      '#type'  => 'item',
      '#markup' => t("As there isn't any vocabulary, nothing can be exported..."),
    );

    return $form;
  }
  // Else there are vocabularies.

  $form['tab'] = array(
    '#type'        => 'vertical_tabs',
    '#default_tab' => 'content',
  );

  $form['tab']['content'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('1. What do you want to export?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['content']['export_format'] = array(
    '#type'          => 'radios',
    '#title'         => 'CSV format',
    '#options'       => $list_export_format,
    '#default_value' => $list_previous_values['export_format'],
  );

  $form['tab']['content']['info'] = array(
    '#type'        => 'item',
    '#description' => t('See <a href="!more_help_link">advanced help</a> for informations about formats.', array('!more_help_link' => url('admin/help/taxonomy_csv'))) . '<br />'
    . t('In all cases, you will be notified if a duplicate is found.'),
  );

  $form['tab']['content']['export_vocabulary_id'] = array(
    '#type'          => 'select',
    '#title'         => t('Vocabularies to export'),
    '#options'       => array(
      0 => t('All vocabularies'),
    ),
    '#multiple'      => TRUE,
    '#default_value' => $list_previous_values['export_vocabulary_id'],
    '#description'   => t('The vocabularies you want to export.'),
    '#size'          => min(12, count($list_vocabularies)) + 1,
  );
  foreach ($list_vocabularies as $vocabulary) {
      $form['tab']['content']['export_vocabulary_id']['#options'][$vocabulary->vid] = $vocabulary->name;
  }

  $form['tab']['csv_format'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('2. How do you want to format your CSV file?'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['csv_format']['export_delimiter'] = array(
    '#type'          => 'select',
    '#title'         => t('CSV value delimiter'),
    '#options'       => $list_export_delimiter,
    '#default_value' => $list_previous_values['export_delimiter'],
    '#description'   => t('Choose the delimiter to use in the CSV file.'),
  );

  $form['tab']['csv_format']['export_delimiter_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Custom delimiter',
    '#default_value' => $list_previous_values['export_delimiter_custom'],
    '#size'          => 2,
    '#maxlength'     => 1,
    '#description'   => t('Specify your custom delimiter.'),
    '#states' => array(
      'visible' => array(
        ':input[name=export_delimiter]' => array('value' => 'custom_delimiter'),
      ),
    ),
  );

  $form['tab']['csv_format']['export_enclosure'] = array(
    '#type'          => 'select',
    '#title'         => t('CSV value enclosure'),
    '#options'       => $list_export_enclosure,
    '#default_value' => $list_previous_values['export_enclosure'],
    '#description'   => t('Choose the enclosure used in the CSV file you want to export. Warning: enclosure should not be used in term definitions, specially in descriptions. Furthermore, an enclosure is needed if a field contains a line ending character. Export process will stop in case of problem.'),
  );

  $form['tab']['csv_format']['export_enclosure_custom'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Custom enclosure',
    '#default_value' => $list_previous_values['export_enclosure_custom'],
    '#size'          => 2,
    '#maxlength'     => 1,
    '#description'   => t('Specify your custom enclosure.'),
    '#states' => array(
      'visible' => array(
        ':input[name=export_enclosure]' => array('value' => 'custom_enclosure'),
      ),
    ),
  );

  $form['tab']['csv_format']['export_line_ending'] = array(
    '#type'          => 'select',
    '#title'         => t('Line ending'),
    '#options'       => $list_export_line_ending,
    '#default_value' => $list_previous_values['export_line_ending'],
    '#description'   => t('Choose the end of line to use.'),
  );

  $form['tab']['advanced_options'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('3. Advanced and specific options'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  $form['tab']['advanced_options']['export_order'] = array(
    '#type'          => 'select',
    '#title'         => t('Terms order'),
    '#options'       => $list_export_order,
    '#default_value' => $list_previous_values['export_order'],
    '#description'   => t('Choose order of exported terms.'),
    '#states' => array(
      'invisible' => array(
        ':input[name=export_format]' => array('value' => TAXONOMY_CSV_FORMAT_TREE),
      ),
    ),
  );

  $form['tab']['advanced_options']['info'] = array(
    '#type'          => 'item',
    '#description'   => t('Specific options are shown only if suitable.'),
  );

  $form['tab']['advanced_options']['result_duplicates'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Show duplicate terms after export'),
    '#default_value' => $list_previous_values['result_duplicates'],
    '#description'   => '',
  );

  $form['export_submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Export'),
  );

  $form['export_default_values'] = array(
    '#type'     => 'submit',
    '#value'    => t('Reset to defaults'),
    '#validate' => array('taxonomy_csv_export_form_default_values_validate'),
    '#submit'   => array('taxonomy_csv_export_form_default_values_submit'),
  );

  return $form;
}

/**
 * Handles CSV export form validation.
 *
 * @see taxonomy_csv_export_form()
 */
function taxonomy_csv_export_form_validate($form, &$form_state) {
  // Invoke taxonomy_csv api (defines and functions).
  $module_dir = drupal_get_path('module', 'taxonomy_csv');
  require_once($module_dir . '/export/taxonomy_csv.export.api.inc');

  $options = &$form_state['values'];

  // 1. Presave a file in order to check it.
  // Define vocabulary id (use simple api name of it).
  $options['vocabulary_id'] = $options['export_vocabulary_id'];
  $messages = _taxonomy_csv_export_output_presave($options);

  // 2, Simplify values to be compatible with api checks.
  // Define true delimiter.
  $delimiter = array(
    'comma'            => ',',
    'semicolon'        => ';',
    'tabulation'       => "\t",
    'pipe'             => '|',
    'space'            => ' ',
    'currency_sign'    => '¤',
    'custom_delimiter' => $options['export_delimiter_custom'],
  );
  $options['delimiter'] = $delimiter[$options['export_delimiter']];

  // Define true enclosure.
  $enclosure = array(
    'none'             => '',
    'quotation'        => '"',
    'quote'            => "'",
    'custom_enclosure' => $options['export_enclosure_custom'],
  );
  $options['enclosure'] = $enclosure[$options['export_enclosure']];

  // Define true line ending and order.
  $options['line_ending'] = $options['export_line_ending'];
  $options['order']       = $options['export_order'];

  // 3, Make api checks and eventually update options by reference.
  $messages = array_merge($messages, _taxonomy_csv_export_check_options($options));

  // Use form set error for api errors.
  foreach (array(
      'vocabulary_id' => 'export_vocabulary_id',
      'delimiter'     => 'export_delimiter',
      'enclosure'     => 'export_enclosure',
      'line_ending'   => 'export_line_ending',
      'order'         => 'export_order',
    ) as $key => $value) {
    if (isset($message[$key])) {
      $message[$value] = $message[$key];
      unset($message[$key]);
    }
  }

  // 4. Make non api checks.
  if (($options['export_delimiter'] == 'custom_delimiter')
      && (empty($options['export_delimiter_custom']))) {
    $messages['export_delimiter_custom'] = t('You choose to use a custom delimiter, but your delimiter is empty.');
  }

  if (($options['export_enclosure'] == 'custom_enclosure')
      && (empty($options['export_enclosure_custom']))) {
    $messages['export_enclosure_custom'] = t('You choose to use a custom enclosure, but your enclosure is empty.');
  }

  if (($options['export_delimiter'] == 'custom_delimiter')
      && (drupal_strlen($options['export_delimiter_custom']) > 1)) {
    $messages['export_delimiter_custom'] = t('Delimiter should have only one character.');
  }
  if (($options['export_enclosure'] == 'custom_enclosure')
      && (drupal_strlen($options['export_enclosure_custom']) > 1)) {
    $messages['export_enclosure_custom'] = t('Enclosure should have only zero or one character.');
  }

  // 5. Finish validatation of form.
  foreach ($messages as $item => $message) {
    form_set_error(check_plain($item), filter_xss($message));
  }
}

/**
 * Validate options of exported vocabulary.
 *
 * @param $options
 *   An associative array of options.
 *
 * @return
 *   Array of messages errors if any.
 *   By reference options are cleaned and completed.
 */
function _taxonomy_csv_export_check_options(&$options) {
  $messages = array();

  if ($options['export_format'] == TAXONOMY_CSV_FORMAT_TRANSLATE && !module_exists('i18n_taxonomy')) {
    $messages['export_format'] = t('You cannot use Translations if i18n_taxonomy is not enabled.');
  }

  $list_vocabularies = taxonomy_get_vocabularies();
  if (!$list_vocabularies) {
    $messages['vocabulary_id'] = t('No vocabulary to export.');
  }
  elseif ($options['vocabulary_id']) {
    // Replace machine name with vocabulary id.
    if (!(is_numeric($options['vocabulary_id']) || is_array($options['vocabulary_id']))) {
      $vocabulary = taxonomy_vocabulary_machine_name_load($options['vocabulary_id']);
      $options['vocabulary_id'] = array($vocabulary->vid);
    }
    // Replace vocabulary_id by an array if only one item is selected.
    elseif (!is_array($options['vocabulary_id'])) {
      $options['vocabulary_id'] = array($options['vocabulary_id']);
    }

    if ((count($options['vocabulary_id']) > 1) && in_array(0, $options['vocabulary_id'])) {
      $messages['vocabulary_id'] = t('You choose to export all vocabularies, but you select some individual vocabularies too.');
    }

    foreach ($options['vocabulary_id'] as $item) {
      if (($item != 0) && !isset($list_vocabularies[$item])) {
        $messages['vocabulary_id'] = t("You choose to export a vocabulary, but it doesn't exist.");
      }
    }
  }

  // Delimiter and enclosure greater than one character are forbidden.
  if (drupal_strlen($options['delimiter']) != 1) {
    $messages['delimiter'] = t('Delimiter should be a one character string.');
  }
  if (drupal_strlen($options['enclosure']) > 1) {
    $messages['enclosure'] = t('Enclosure lenght cannot be greater than one character.');
  }
  if ($options['delimiter'] == $options['enclosure']) {
    $messages['delimiter'] = t('Delimiter and enclosure cannot be same character.');
  }

  if (!in_array($options['line_ending'], array(
      'Unix',
      'Mac',
      'MS-DOS',
    ))) {
    $messages['line_ending'] = t('Line ending should be "Unix", "Mac" or "MS-DOS".');
  }

  if (!in_array($options['order'], array(
      'name',
      'tid',
      'weight',
    ))) {
    $messages['order'] = t('Order should be "name", "tid" or "weight".');
  }

  // Calculates number of terms to be exported.
  $options['total_terms'] = taxonomy_csv_vocabulary_count_terms($options['vocabulary_id']);
  if (!$options['total_terms']) {
    $messages['vocabulary_id'] = t('Vocabulary has no term to export. Export finished.');
  }

  return $messages;
}

/**
 * Handles CSV export form submission and launch batch set.
 *
 * @see taxonomy_csv_export_form()
 */
function taxonomy_csv_export_form_submit($form, &$form_state) {
  // Remember last preferences and prepare only options to be sent to api.
  foreach (array(
      'export_format',
      'export_vocabulary_id',
      'export_delimiter',
      'export_delimiter_custom',
      'export_enclosure',
      'export_enclosure_custom',
      'export_line_ending',
      'export_order',
      'result_duplicates',
    ) as $option) {
    variable_set('taxonomy_csv_' . $option, $form_state['values'][$option]);
    $options[$option] = $form_state['values'][$option];
  }
  // Finish to prepare $options. Unset useless options for api.
  unset($options['export_vocabulary_id']);
  unset($options['export_delimiter']);
  unset($options['export_delimiter_custom']);
  unset($options['export_enclosure']);
  unset($options['export_enclosure_custom']);
  unset($options['export_line_ending']);
  unset($options['export_order']);
  $options['delimiter']   = $form_state['values']['delimiter'];
  $options['enclosure']   = $form_state['values']['enclosure'];
  $options['line_ending'] = $form_state['values']['line_ending'];
  $options['order']       = $form_state['values']['order'];
  $options['file']        = $form_state['values']['file'];
  $options['vocabulary_id'] = $form_state['values']['vocabulary_id'];
  $options['check_options'] = FALSE; // Already done.
  $options['result_display'] = TRUE;

  // Prepares process batch (will be automatically processed when returns).
  taxonomy_csv_vocabulary_export($options);
}

/**
 * Restore recommended default values in the export form. Empty validate hook.
 */
function taxonomy_csv_export_form_default_values_validate($form, &$form_state) {
}

/**
 * Restore recommended default values in the export form.
 */
function taxonomy_csv_export_form_default_values_submit($form, &$form_state) {
  foreach (_taxonomy_csv_values('export_default_ui') as $option => $value) {
    variable_set("taxonomy_csv_$option", $value);
  }
  unset($form_state['values']);
  unset($form_state['storage']);

  drupal_set_message(t('Export options have been reset to default.'));
}
