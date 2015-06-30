<?php

/**
 * @file
 * Webform module email component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_email() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => 0,
    'extra' => array(
      'multiple' => 0,
      'format' => 'short',
      'width' => '',
      'unique' => 0,
      'disabled' => 0,
      'title_display' => 0,
      'description' => '',
      'placeholder' => '',
      'attributes' => array(),
      'private' => FALSE,
      'analysis' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_email() {
  return array(
    'webform_email' => array(
      'render element' => 'element',
      'file' => 'components/email.inc',
    ),
    'webform_display_email' => array(
      'render element' => 'element',
      'file' => 'components/email.inc',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_email($component) {
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . ' ' . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
    '#attributes' => ($component['value'] == '[current-user:mail]' && count(form_get_errors()) == 0) ? array('disabled' => TRUE) : array(),
    '#id' => 'email-value',
  );
  $form['user_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('User email as default'),
    '#default_value' => $component['value'] == '[current-user:mail]' ? 1 : 0,
    '#description' => t('Set the default value of this field to the user email, if he/she is logged in.'),
    '#attributes' => array('onclick' => 'getElementById("email-value").value = (this.checked ? "[current-user:mail]" : ""); getElementById("email-value").disabled = this.checked;'),
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_email_validate'),
  );
  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Allow multiple e-mail addresses, separated by commas.'),
    '#weight' => 0,
  );
  if (webform_variable_get('webform_email_address_format') == 'long') {
    $form['extra']['format'] = array(
      '#type' => 'radios',
      '#title' => t('Format'),
      '#options' => array(
        'long' => t('Allow long format: "Example Name" &lt;name@example.com&gt;'),
        'short' => t('Short format only: name@example.com'),
      ),
      '#default_value' => $component['extra']['format'],
      '#description' => t('Not all servers support the "long" format.'),
    );
  }
  $form['display']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the textfield.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#parents' => array('extra', 'width'),
  );
  $form['display']['placeholder'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder'),
    '#default_value' => $component['extra']['placeholder'],
    '#description' => t('The placeholder will be shown in the field until the user starts entering a value.') . ' ' . t('Often used for example values, such as "john@example.com".'),
    '#parents' => array('extra', 'placeholder'),
  );
  $form['display']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 11,
    '#default_value' => $component['extra']['disabled'],
    '#parents' => array('extra', 'disabled'),
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
 * Element validation function for the email edit form.
 */
function _webform_edit_email_validate($element, &$form_state) {
  if ($form_state['values']['user_email']) {
    $form_state['values']['value'] = '[current-user:mail]';
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_email($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  global $user;
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'webform_email',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? webform_replace_tokens($component['value'], $node) : $component['value'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#attributes' => $component['extra']['attributes'],
    '#element_validate'  => array('_webform_validate_email'),
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  if ($component['required']) {
    $element['#attributes']['required'] = 'required';
  }

  // Add an e-mail class for identifying the difference from normal textfields.
  $element['#attributes']['class'][] = 'email';

  // Enforce uniqueness.
  if ($component['extra']['unique']) {
    $element['#element_validate'][] = 'webform_validate_unique';
  }

  if ($component['extra']['format'] == 'long') {
    // html5 email elements enforce short-form email validation in addition to
    // pattern validation. This means that long format email addresses must be
    // rendered as text.
    $element['#attributes']['type'] = 'text';
    
    // html5 patterns have implied delimters and start and end patterns.
    // The are also case sensitive, not global, and not multi-line.
    // See https://developer.mozilla.org/en-US/docs/Web/Guide/HTML/HTML5/Constraint_validation
    // See http://stackoverflow.com/questions/19605773/html5-email-validation
    $address = '[a-zA-Z0-9.!#$%&’*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*';
    $name = '("[^<>"]*?"|[^<>",]*?)';
    $short_long = "($name *<$address>|$address)";
    $element['#attributes']['pattern'] = $component['extra']['multiple']
                                            ? "$short_long(, *$short_long)*"
                                            : $short_long;
  }
  elseif ($component['extra']['multiple']) {
    $element['#attributes']['multiple'] = 'multiple';
  }

  if (isset($value[0])) {
    $element['#default_value'] = $value[0];
  }

  if ($component['extra']['placeholder']) {
    $element['#attributes']['placeholder'] = $component['extra']['placeholder'];
  }

  if ($component['extra']['disabled']) {
    if ($filter) {
      $element['#attributes']['readonly'] = 'readonly';
    }
    else {
      $element['#disabled'] = TRUE;
    }
  }

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element['#size'] = $component['extra']['width'];
  }

  return $element;
}

/**
 * Theme function to render an email component.
 */
function theme_webform_email($variables) {
  $element = $variables['element'];

  // This IF statement is mostly in place to allow our tests to set type="text"
  // because SimpleTest does not support type="email".
  if (!isset($element['#attributes']['type'])) {
    $element['#attributes']['type'] = 'email';
  }

  // Convert properties to attributes on the element if set.
  foreach (array('id', 'name', 'value', 'size') as $property) {
    if (isset($element['#' . $property]) && $element['#' . $property] !== '') {
      $element['#attributes'][$property] = $element['#' . $property];
    }
  }
  _form_set_class($element, array('form-text', 'form-email'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

/**
 * A Drupal Form API Validation function. Validates the entered values from
 * email components on the client-side form.
 *
 * @param $form_element
 *   The e-mail form element.
 * @param $form_state
 *   The full form state for the webform.
 * @return
 *   None. Calls a form_set_error if the e-mail is not valid.
 */
function _webform_validate_email($form_element, &$form_state) {
  $component = $form_element['#webform_component'];
  $format = webform_variable_get('webform_email_address_format') == 'long' ? $component['extra']['format'] : 'short';
  webform_email_validate($form_element['#value'],
                         implode('][', $form_element ['#parents']),
                         TRUE,  // Required validation is done elsewhere.
                         $component['extra']['multiple'],
                         FALSE, // No tokens are allowed in user input.
                         $format);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_email($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_email',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => isset($value[0]) ? $value[0] : '',
    '#translatable' => array('title'),
  );
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_email($variables) {
  $element = $variables['element'];
  $element['#value'] = empty($element['#value']) ? ' ' : $element['#value'];
  return $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_email($component, $sids = array(), $single = FALSE, $join = NULL) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('no', 'data'))
    ->condition('wsd.nid', $component['nid'])
    ->condition('wsd.cid', $component['cid']);

  if (count($sids)) {
    $query->condition('wsd.sid', $sids, 'IN');
  }

  if ($join) {
    $query->innerJoin($join, 'ws2_', 'wsd.sid = ws2_.sid');
  }

  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = $query->execute();
  foreach ($result as $data) {
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);

  $other[0] = array(t('Average submission length in words (ex blanks)'), ($nonblanks != 0 ? number_format($wordcount/$nonblanks, 2) : '0'));

  return array(
    'table_rows' => $rows,
    'other_data' => $other,
  );

}

/**
 * Implements _webform_table_component().
 */
function _webform_table_email($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}


/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_email($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_email($component, $export_options, $value) {
  return empty($value[0]) ? '' : $value[0];
}
