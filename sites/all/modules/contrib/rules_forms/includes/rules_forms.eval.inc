<?php

/**
 * @file
 * Evaluation functions for Rules Forms module.
 */

/**
 * Condition: Check a form element value.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param ArrayObject $form_state
 *   A reference to the form state array of the form for which the
 *   event was triggered.
 * @param string $value
 *   The value to be compared against the actual form value to determine
 *   whether the condition is met.
 * @param bool $regex
 *   A boolean indicating whether this comparison should be performed
 *   using $value as a regular expression.
 *
 * @return bool
 *   Indicates whether the element's value matches $value.
 */
function rules_forms_condition_element_value($form, $form_state, $element, $value, $regex) {
  $form_element = &_rules_forms_get_element($form, $element);

  // First check if a value exists.
  if (isset($form_element['#value'])) {

    // Perform the comparison with a regular expression if necessary.
    if ($regex) {
      $result = preg_match($value, $form_element['#value']) == 1;
      return $result === FALSE ? FALSE : ($result == 1);
    }

    // Multiple values come in as array.
    if (is_array($form_element['#value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#value']);
    }
    return $form_element['#value'] === $value;
  }

  // If no #value existed, check against the default value.
  if (isset($form_element['#default_value'])) {

    // Perform the comparison with a regular expression if necessary.
    if ($regex) {
      $result = preg_match($value, $form_element['#default_value']);
      return $result === FALSE ? FALSE : ($result == 1);
    }

    if (is_array($form_element['#default_value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#default_value']);
    }
    return $form_element['#default_value'] === $value;
  }
  return FALSE;
}

/**
 * Condition: Form element value has changed.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param ArrayObject $form_state
 *   A reference to the form state array of the form for which the
 *   event was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 *
 * @return bool
 *   Indicates whether the element's value has changed since the
 *   form was built.
 */
function rules_forms_condition_element_changed($form, $form_state, $element) {
  $form_id = isset($form['#id']) ? str_replace('-', '_', $form['#id']) : '';

  if (isset($_SESSION['rules_forms_form_'. $form_id .'_values'])) {
    // Get the build form values and the element name being validated.
    $element_name = substr($element, strpos($element, ':') + 1);
    $build_values = $_SESSION['rules_forms_form_'. $form_id .'_values'];

    // Ensure that form values are set. This will prevent the condition from being evaluated during build.
    if (isset($build_values[$element]) && isset($form_state['values']) && isset($form_state['values'][$element_name])) {
      return $build_values[$element] !== $form_state['values'][$element_name];
    }
  }
  return FALSE;
}

/**
 * Condition: Form button was clicked.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param ArrayObject $form_state
 *   A reference to the form state array of the form for which the
 *   event was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 *
 * @return bool
 *   Indicates whether the indicated button was clicked.
 */
function rules_forms_condition_button_clicked($form, $form_state, $element) {
  if (isset($form_state['triggering_element'])) {
    $button = $form_state['triggering_element'];
    $form_element = _rules_forms_get_element($form, $element);
    if (isset($form_element['#name']) && isset($button['#name'])) {
      return $form_element['#name'] === $button['#name'];
    }
    elseif (isset($form_element['#value']) && isset($button['#value'])) {
      return $form_element['#value'] === $button['#value'];
    }
  }
  return FALSE;
}

/**
 * Returns TRUE if both arrays contain the same values, regardless
 * of their keys and value ordering.
 *
 * @param array $array1
 *   An array to compare against $array2.
 * @param array $array2
 *   An array to compare against $array1.
 *
 * @return bool
 *   Indicates whether the input arrays are equal.
 */
function rules_forms_equal_array_values($array1, $array2) {
  $diff1 = array_diff($array1, $array2);
  $diff2 = array_diff($array2, $array1);
  return empty($diff1) && empty($diff2);
}

/**
 * Action: Set the redirect target.
 *
 * @param ArrayObject $form_state
 *   A reference to the form state array of the form for which the
 *   event was triggered.
 * @param string $path
 *   The path to which to redirect the user.
 * @param string $query
 *   A query string for use in drupal_goto().
 * @param string $fragment
 *   A fragement string for use in drupal_goto().
 */
function rules_forms_action_redirect($form_state, $path, $query, $fragment) {
  // We manually check the form to see if redirect is okay.
  // Setting $form_state['redirect'] has proven to be unreliable.
  if (!empty($form_state['programmed']) || !empty($form_state['rebuild']) || !empty($form_state['no_redirect'])) {
    return;
  }

  // Check if a query was defined and build the options array.
  $options = $query != '' ? array('query' => array($query)) : array();
  $options['fragment'] = $fragment;

  // Redirect the user.
  drupal_goto($path, $options);
}

/**
 * Action: Set the title of a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $title
 *   The title to assign to the form element.
 */
function rules_forms_action_set_title($form, $element, $title) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#title'] = $title;
  }
}

/**
 * Action: Set the description of a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $description
 *   The description to assign to the form element.
 */
function rules_forms_action_set_description($form, $element, $description) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#description'] = $description;
  }
}

/**
 * Action: Hide a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param bool $access
 *   A boolean value to assign to the element's #access attribute.
 *   This value is reversed so that it makes sense in the Rules interface.
 *   The user chooses to 'Hide a form element', so FALSE means hide and
 *   TRUE means don't hide.
 */
function rules_forms_action_set_access($form, $element, $access) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#access'] = $access == 0;
  }
}

/**
 * Action: Disable a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param bool $disabled
 *   A boolean value to assign to the element's #disabled attribute.
 */
function rules_forms_action_set_disabled($form, $element, $disabled) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#disabled'] = $disabled == 1;
  }
}

/**
 * Action: Require a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param bool $required
 *   A boolean value to assign to the element's #required attribute.
 */
function rules_forms_action_set_required($form, $element, $required) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#required'] = $required == 1;
  }
}

/**
 * Action: Set the default value of a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $value
 *   A value to assign to the form element. If the form element uses
 *   #options then the value is split into an array.
 */
function rules_forms_action_set_default($form, $element, $value) {
  $form_element = &_rules_forms_get_element($form, $element);

  // Try to determine if this is a multiple option element.
  if (isset($form_element['#options']) && is_array($form_element['#options'])) {
    $form_element['#default_value'] = explode("\r\n", $value);
  }
  else {
    $form_element['#default_value'] = $value;
  }
}

/**
 * Action: Set the prefix of a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $prefix
 *   A value to assign to the element's #prefix attribute.
 * @param string $suffix
 *   A value to assign to the element's #suffix attribute.
 */
function rules_forms_action_set_prefix_suffix_html($form, $element, $prefix, $suffix) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#prefix'] = $prefix;
    $form_element['#suffix'] = $suffix;
  }
}

/**
 * Action: Adjust weight of a form element.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $weight
 *   A value to assign to the element's #weight attribute. This
 *   value will be converted to an integer.
 */
function rules_forms_action_set_weight($form, $element, $weight) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (isset($form_element['#type'])) {
    $form_element['#weight'] = (int) $weight;
  }
}

/**
 * Action: Set form error.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $message
 *   The error message to set on the form element.
 */
function rules_forms_action_set_error($form, $element, $message) {
  if (strpos($element, ':') !== FALSE) {
    $form_element = &_rules_forms_get_element($form, $element);

    // Remove the element type information.
    $element = substr($element, strpos($element, ':') + 1);

    if (isset($form_element['#parents'])) {
      $element = implode('][', $form_element['#parents']);
    }
    else {
      $element = str_replace(':', '][', $element);
    }
  }
  form_set_error($element, $message);
}

/**
 * Validation callback for set options action.
 */
function rules_forms_action_set_options_validate($element) {
  // Check for duplicate key values to prevent unexpected data loss. Require
  // all options to include a safe_key.
  $lines = explode("\n", trim($element->settings['options']));
  $existing_keys = array();
  $duplicate_keys = array();
  $missing_keys = array();
  $long_keys = array();
  $group = '';

  // Check each option for validity - length, existence, and duplication.
  foreach ($lines as $line) {
    $matches = array();
    $line = trim($line);
    if (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
      $key = $matches[1];

      // Validate the length of the key.
      if (strlen($key) > 128) {
        $long_keys[] = $key;
      }
    }
    // The key was not found.
    else {
      $missing_keys[] = $line;
    }

    // Ensure this is not a duplicate key.
    if (isset($key)) {
      if (isset($existing_keys[$group][$key])) {
        $duplicate_keys[$key] = $key;
      }
      else {
        $existing_keys[$group][$key] = $key;
      }
    }
  }

  // Throw exceptions for validation errors.
  if (!empty($missing_keys)) {
    throw new RulesIntegrityException(t('Every option must have a key specified. Specify each option as "safe_key|Some readable option'), $element);
  }

  if (!empty($long_keys)) {
    throw new RulesIntegrityException(t('Option keys must be less than 128 characters. The following keys exceed this limit:') . theme('item_list', $long_keys), $element);
  }

  if (!empty($duplicate_keys)) {
    throw new RulesIntegrityException(t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', array('items' => $duplicate_keys)), $element);
  }
}

/**
 * Action: Set multiple value options of a form element.
 * Note: For multiple option values each key value pair is on its own line
 * and formatted key|value.
 *
 * @param ArrayObject $form
 *   A reference to the form array of the form for which the event
 *   was triggered.
 * @param string $element_id
 *   The element ID of the element being checked in the form of
 *   element_type:element_id.
 * @param string $value
 *   A value to assign to the element's #options attribute. This value
 *   is split into an array.
 */
function rules_forms_action_set_options($form, $element_id, $value) {
  $form_element = &_rules_forms_get_element($form, $element_id);
  $lines = explode("\r\n", trim($value));

  $processed_options = array();
  foreach ($lines as $line) {
    $line = trim($line);
    if (preg_match('/^([^|]+)\|(.*)$/', $line, $matches)) {
      $processed_options[$matches[1]] = $matches[2];
    }
  }
  if (isset($form_element['#type'])) {
    $form_element['#options'] = $processed_options;
  }
}

