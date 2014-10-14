<?php

/**
 * @file
 * Rules events, conditions, and actions hooks for Rules Forms module.
 */

/**
 * Implements hook_rules_file_info().
 */
function rules_forms_rules_file_info() {
  return array('includes/rules_forms.eval');
}

/**
 * Implements hook_rules_event_info().
 */
function rules_forms_rules_event_info() {
  $form_events = variable_get('rules_forms_event_info', array());
  $defaults = array(
    'group' => 'Rules Forms',
    'access callback' => 'rules_forms_integration_access',
  );

  $events = array();
  foreach ($form_events as $form_id => $info) {
    $events['rules_forms_' . $form_id . '_form_built'] = $defaults + array(
      'label' => t('@form is being built', array('@form' => $info['label'])),
      'variables' => rules_forms_event_variables() + rules_forms_element_variables($form_id),
    );
    $events['rules_forms_' . $form_id . '_form_submit'] = $defaults + array(
      'label' => t('@form is submitted', array('@form' => $info['label'])),
      'variables' => rules_forms_event_variables() + rules_forms_element_variables($form_id),
    );
    $events['rules_forms_' . $form_id . '_form_validate'] = $defaults + array(
      'label' => t('@form is being validated', array('@form' => $info['label'])),
      'variables' => rules_forms_event_variables() + rules_forms_element_variables($form_id),
    );

    if ($info['buttons'] && isset($info['submit'])) {
      foreach ($info['submit'] as $element_id => $label) {
        // Add button validate events.
        $events['rules_forms_'. $form_id .'_button_'. str_replace(':', '_', $element_id) .'_validate'] = $defaults + array(
          'label' => t('@form @button button is being validated', array('@form' => $info['label'], '@button' => strtolower($label))),
          'variables' => rules_forms_event_variables() + rules_forms_element_variables($form_id),
        );
        // Add button submit events.
        $events['rules_forms_'. $form_id .'_button_'. str_replace(':', '_', $element_id) .'_submit'] = $defaults + array(
          'label' => t('@form @button button is being submitted', array('@form' => $info['label'], '@button' => strtolower($label))),
          'variables' => rules_forms_event_variables() + rules_forms_element_variables($form_id),
        );
      }
    }
  }
  return $events;
}

/**
 * Returns some arguments suitable for hook form alter.
 *
 * @see rules_forms_rules_event_info()
 */
function rules_forms_event_variables() {
  return array(
    'form' => array(
      'type' => 'form',
      'label' => t('Form'),
    ),
    'form_state' => array(
      'type' => 'form_state',
      'label' => t('Form state'),
    ),
    'form_id' => array(
      'type' => 'text',
      'label' => t('Form ID'),
    ),
    'user' => array(
      'type' => 'user',
      'label' => t('Logged in user'),
    ),
  );
}

/**
 * Adds element variables for the form. Each form element ID is passed as
 * a variable to the rule for access in conditions and actions. This allows
 * us to provide a select list of elements rather than having to cut-and-paste
 * element IDs.
 */
function rules_forms_element_variables($form_id) {
  $variables = array();
  $events = variable_get('rules_forms_event_info', array());
  if (isset($events[$form_id]['elements']) && is_array($events[$form_id]['elements'])) {
    foreach ($events[$form_id]['elements'] as $key => $info) {
      $variables[$key] = array(
        'type' => 'form_element',
        'label' => $info['label'],
      );
    }
  }
  return $variables;
}

/**
 * Implements hook_rules_condition_info().
 */
function rules_forms_rules_condition_info() {
  $conditions = array();
  $conditions['rules_forms_element_value'] = array(
    'label' => t('Form element has value'),
    'group' => 'Rules Forms',
    'parameter' => array(
      'form' => array(
        'type' => 'form',
        'label' => t('Form'),
      ),
      'form_state' => array(
        'type' => 'form_state',
        'label' => t('Form state'),
      ),
      'element' => array(
        'type' => 'form_element',
        'label' => t('Form element'),
        'description' => t('The form element to be targeted.'),
      ),
      'value' => array(
        'type' => 'form_value',
        'label' => t('Value(s)'),
        'optional' => TRUE,
        'description' => t('Value(s) assigned to the form element. If the form element allows multiple values, enter one value per line.'),
      ),
      'regex' => array(
        'type' => 'boolean',
        'label' => t('Evaluate as regular expression'),
        'optional' => TRUE,
        'description' => t('Perform the data comparison using the provided value as a regular expression.'),
      ),
    ),
    'base' => 'rules_forms_condition_element_value',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $conditions['rules_forms_element_changed'] = array(
    'label' => t('Form element value has changed'),
    'group' => 'Rules Forms',
    'parameter' => array(
      'form' => array(
        'type' => 'form',
        'label' => t('Form'),
      ),
      'form_state' => array(
        'type' => 'form_state',
        'label' => t('Form state'),
      ),
      'element' => array(
        'type' => 'form_element',
        'label' => t('Form element'),
        'description' => t('The form element to be targeted.'),
      ),
    ),
    'base' => 'rules_forms_condition_element_changed',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $conditions['rules_forms_button_clicked'] = array(
    'label' => t('Form button was clicked'),
    'group' => 'Rules Forms',
    'parameter' => array(
      'form' => array(
        'type' => 'form',
        'label' => t('Form'),
      ),
      'form_state' => array(
        'type' => 'form_state',
        'label' => t('Form state'),
      ),
      'element' => array(
        'type' => 'form_element',
        'label' => t('Form element'),
        'description' => t('The form button that was clicked.'),
      ),
    ),
    'base' => 'rules_forms_condition_button_clicked',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function rules_forms_rules_action_info() {
  $element_params = array(
    'form' => array('type' => 'form', 'label' => t('Form')),
    'element' => array(
      'type' => 'form_element',
      'label' => t('Form element'),
      'restriction' => 'input',
      'description' => t('The form element to be targeted.'),
    ),
  );

  $actions = array();
  $actions['rules_forms_set_title'] = array(
    'label' => t('Set the title of an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'title' => array(
        'type' => 'text',
        'label' => t('Title'),
        'optional' => TRUE,
      ),
    ),
    'base' => 'rules_forms_action_set_title',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_description'] = array(
    'label' => t('Set the description of an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'description' => array(
        'type' => 'text',
        'label' => t('Description'),
        'optional' => TRUE,
      ),
    ),
    'base' => 'rules_forms_action_set_description',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_access'] = array(
    'label' => t('Hide an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'access' => array(
        'type' => 'boolean',
        'label' => t('Hide'),
        'optional' => TRUE,
        'description' => t('Hides a form element.'),
      ),
    ),
    'base' => 'rules_forms_action_set_access',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_disabled'] = array(
    'label' => t('Disable an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'disabled' => array(
        'type' => 'boolean',
        'label' => t('Disable'),
        'optional' => TRUE,
        'description' => t('Disables a form element.'),
      ),
    ),
    'base' => 'rules_forms_action_set_disabled',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_required'] = array(
    'label' => t('Require an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'require' => array(
        'type' => 'boolean',
        'label' => t('Required'),
        'optional' => TRUE,
        'description' => t('Requires a form element.'),
      ),
    ),
    'base' => 'rules_forms_action_set_required',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_options'] = array(
    'label' => t('Set multiple value options of an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'options' => array(
        'type' => 'text',
        'label' => t('Options'),
        'description' => t('<strong>Key-value pairs MUST be specified as "safe_key|Some readable option"</strong>. Use of only alphanumeric characters and underscores is recommended in keys. One option per line.'),
      ),
    ),
    'base' => 'rules_forms_action_set_options',
    'validate' => 'rules_forms_action_set_options_validate',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_default'] = array(
    'label' => t('Set the default value of an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'value' => array(
        'type' => 'form_value',
        'label' => t('Value(s)'),
        'optional' => TRUE,
        'description' => t('Value(s) to assign to the form element. If the form element allows multiple values, enter one value per line.'),
      ),
    ),
    'base' => 'rules_forms_action_set_default',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_weight'] = array(
    'label' => t('Adjust weight of an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'weight' => array(
        'type' => 'integer',
        'label' => t('Element weight'),
        'options list' => '_rules_forms_weight_options',
        'description' => t('Low numbers make the element bubble up, high numbers sink it down.'),
      ),
    ),
    'base' => 'rules_forms_action_set_weight',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_prefix_suffix'] = array(
    'label' => t('Insert HTML into the prefix/suffix of an element in the form'),
    'group' => 'Rules Forms',
    'parameter' => $element_params + array(
      'prefix' => array(
        'type' => 'text',
        'label' => t('Prefixed HTML'),
        'optional' => TRUE,
        'description' => t('HTML inserted before the element.'),
      ),
      'suffix' => array(
        'type' => 'text',
        'label' => t('Suffixed HTML'),
        'optional' => TRUE,
        'description' => t('HTML inserted after the element.'),
      ),
    ),
    'base' => 'rules_forms_action_set_prefix_suffix_html',
    'callbacks' => array(
      'access' => 'rules_forms_integration_access',
      'form_alter' => 'rules_forms_action_set_attribute_form_alter',
    ),
  );
  $actions['rules_forms_set_error'] = array(
    'label' => t('Set an error on the form'),
    'group' => 'Rules Forms',
    'parameter' =>  array(
      'form' => array('type' => 'form', 'label' => t('Form')),
      'element' => array(
        'type' => 'form_element',
        'label' => t('Form element'),
        'restriction' => 'input',
        'optional' => TRUE,
        'description' => t('The form element to be targeted.'),
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'optional' => TRUE,
        'description' => t('The message that should be displayed to the user.'),
      ),
    ),
    'base' => 'rules_forms_action_set_error',
    'access callback' => 'rules_forms_integration_access',
  );
  $actions['rules_forms_redirect'] = array(
    'label' => t('Set the redirect target of the form'),
    'group' => 'Rules Forms',
    'parameter' => array(
      'form_state' => array('type' => 'form_state', 'label' => t('Form state')),
      'path' => array('type' => 'text', 'label' => t('Path')),
      'query' => array('type' => 'text', 'label' => t('Query'), 'optional' => TRUE),
      'fragment' => array('type' => 'text', 'label' => t('Fragment'), 'optional' => TRUE),
    ),
    'base' => 'rules_forms_action_redirect',
    'access callback' => 'rules_forms_integration_access',
    'help' => t('Enter a Drupal path, path alias, or external URL to redirect to. Enter (optional) queries after "?" and (optional) anchor after "#".'),
  );
  return $actions;
}

/**
 * Provides numeric options for setting the weight of a form element.
 */
function _rules_forms_weight_options() {
  $options = array();
  for ($i = -20; $i < 21; $i++) {
    $options[(string) $i] = $i;
  }
  return $options;
}

/**
 * Provides a standard explanaition of how to use element IDs with
 * conditions and actions.
 */
function _rules_forms_element_description() {
  return t('Examples on the "Create Article" form: "title" for the title field or "body[und][0][value]" for the body field.');
}

/**
 * Implements hook_rules_data_type_info().
 */
function rules_forms_rules_data_info() {
  return array(
    'form' => array(
      'label' => t('form'),
      'group' => 'Rules Forms',
    ),
    'form_state' => array(
      'label' => t('form state'),
      'group' => 'Rules Forms',
    ),
    'form_element' => array(
      'label' => t('form element'),
      'group' => 'Rules Forms',
      'ui class' => 'RulesFormsDataElement',
    ),
    'form_value' => array(
      'label' => t('form value'),
      'group' => 'Rules Forms',
      'ui class' => 'RulesFormsDataValue',
    ),
  );
}

/**
 * Alters the form for action: Set element property. Taken from Rules module.
 */
function rules_forms_action_set_attribute_form_alter(&$form, &$form_state, $options, RulesAbstractPlugin $element) {
  $first_step = empty($element->settings['element']);
  $form['reload'] = array(
    '#weight' => 10,
    '#type' => 'submit',
    '#name' => 'reload',
    '#value' => $first_step ? t('Continue') : t('Reload form'),
    '#limit_validation_errors' => array(array('parameter', 'element')),
    '#submit' => array('rules_action_type_form_submit_rebuild'),
    '#ajax' => rules_ui_form_default_ajax(),
  );

  // Use ajax and trigger as the reload button.
  $form['parameter']['type']['settings']['element']['#ajax'] = $form['reload']['#ajax'] + array(
    'event' => 'change',
    'trigger_as' => array('name' => 'reload'),
  );

  // Hide all form elements other than the element selector.
  if ($first_step) {

    // In the first step show only the type select.
    foreach (element_children($form['parameter']) as $key) {
      if ($key != 'element') {
        unset($form['parameter'][$key]);
      }
    }

    unset($form['submit']);
    unset($form['provides']);
    unset($form['negate']);
  }
  else {
    // Hide the reload button in case js is enabled and it's not the first step.
    $form['reload']['#attributes'] = array('class' => array('rules-hide-js'));
  }
}

/**
 * Rules Forms integration access callback.
 */
function rules_forms_integration_access($type, $name) {
  return user_access('administer rules forms rules');
}
