<?php

/**
 * @file
 * Form elements and menu callbacks to provide conditional handling in Webform.
 */

/**
 * Form builder; Provide the form for adding conditionals to a webform node.
 */
function webform_conditionals_form($form, &$form_state, $node) {
  form_load_include($form_state, 'inc', 'webform', $name = 'includes/webform.components');
  form_load_include($form_state, 'inc', 'webform', $name = 'includes/webform.conditionals');

  // Add JavaScript settings to the page needed for conditional elements.
  _webform_conditional_expand_value_forms($node);

  if (isset($form_state['values']['conditionals'])) {
    // Remove the "new" conditional that always comes in.
    unset($form_state['values']['conditionals']['new']);

    $conditionals = $form_state['values']['conditionals'];
  }
  else {
    $conditionals = $node->webform['conditionals'];
  }
  // Empty out any conditionals that have no rules.
  foreach ($conditionals as $rgid => $conditional) {
    if (empty($conditional['rules'])) {
      unset($conditionals[$rgid]);
    }
  }

  $form['#tree'] = TRUE;
  $form['#node'] = $node;

  $form['#attached']['library'][] = array('webform', 'admin');
  $form['#attached']['css'][] = drupal_get_path('module', 'webform') . '/css/webform.css';

  // Wrappers used for AJAX addition/removal.
  $form['conditionals']['#theme'] = 'webform_conditional_groups';
  $form['conditionals']['#prefix'] = '<div id="webform-conditionals-ajax">';
  $form['conditionals']['#suffix'] = '</div>';

  // Keep track of the max conditional count to use as the range for weights.
  $form_state['conditional_count'] = isset($form_state['conditional_count']) ? $form_state['conditional_count'] : 1;
  $form_state['conditional_count'] = count($conditionals) > $form_state['conditional_count'] ? count($conditionals) : $form_state['conditional_count'];

  $source_list = webform_component_list($node, 'conditional', 'path', TRUE);
  $target_list = webform_component_list($node, TRUE, 'path', TRUE);
  $delta = $form_state['conditional_count'];
  $weight = -$delta - 1;
  $index = 0;
  foreach ($conditionals as $rgid => $conditional_group) {
    $weight = $conditional_group['weight'];
    $form['conditionals'][$rgid] = array(
      '#theme' => 'webform_conditional_group_row',
      '#even_odd' => ++$index % 2 ? 'odd' : 'even',
      '#weight' => $weight,
    );
    $form['conditionals'][$rgid]['conditional'] = array(
      '#type' => 'webform_conditional',
      '#default_value' => $conditional_group,
      '#nid' => $node->nid,
      '#sources' => $source_list,
      '#actions' => array(
        'hide' => t('hide'),
        'show' => t('show'),
      ),
      '#targets' => $target_list,
      '#parents' => array('conditionals', $rgid),
    );
    $form['conditionals'][$rgid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for rule group !rgid', array('!rgid' => $rgid)),
      '#title_display' => 'invisible',
      '#default_value' => $weight,
      '#delta' => $delta,
    );
  }

  $form['conditionals']['new']['#weight'] = $weight + 1;
  $form['conditionals']['new']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight for new rule group'),
    '#title_display' => 'invisible',
    '#default_value' => $weight + 1,
    '#delta' => $delta,
  );
  $form['conditionals']['new']['new'] = array(
    '#type' => 'submit',
    '#value' => t('+'),
    '#submit' => array('webform_conditionals_form_add'),
    '#ajax' => array(
      'progress' => 'none',
      'effect' => 'fade',
      'callback' => 'webform_conditionals_ajax',
    ),
  );

  $form['actions'] = array(
    '#type' => 'actions',
    '#tree' => FALSE,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save conditions'),
    '#validate' => array('webform_conditionals_form_validate'),
    '#submit' => array('webform_conditionals_form_submit'),
  );

  return $form;
}

/**
 * Submit handler for webform_conditionals_form(). Add an additional choice.
 */
function webform_conditionals_form_add($form, &$form_state) {
  // Build a default new conditional.
  unset($form_state['values']['conditionals']['new']);
  $weight = count($form_state['values']['conditionals']) > 10 ? -count($form_state['values']['conditionals']) : -10;
  foreach ($form_state['values']['conditionals'] as $key => $conditional) {
    $weight = max($weight, $conditional['weight']);
  }

  // Add the conditional to form state and rebuild the form.
  $form_state['values']['conditionals'][] = array(
    'rules' => array(
      array(
        'source' => NULL,
        'operator' => NULL,
        'value' => NULL,
      ),
    ),
    'andor' => 'and',
    'action' => 'hide',
    'target' => NULL,
    'weight' => $weight + 1,
  );
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate handler for webform_conditionals_form().
 *
 * Prohibit the source and target of a conditional rule from being the same.
 */
function webform_conditionals_form_validate($form, &$form_state) {
  // Skip validation unless this is saving the form.
  $button_key = end($form_state['triggering_element']['#array_parents']);
  if ($button_key !== 'submit') {
    return;
  }

  foreach ($form_state['complete form']['conditionals'] as $conditional_key => $element) {
    if (substr($conditional_key, 0, 1) !== '#' && $conditional_key !== 'new') {
      $conditional = $element['conditional'];
      foreach ($conditional['rules'] as $rule_key => $rule) {
        if (substr($rule_key, 0, 1) !== '#' && $rule['source']['#value'] === $conditional['target']['#value']) {
          form_set_error('conditionals][' . $conditional_key . '][rules][' . $rule_key . '][source', t('The subject of the conditional cannot be the same as the component that is shown or hidden (%target).', array('%target' => $conditional['target']['#options'][$conditional['target']['#value']])));
        }
      }
    }
  }

  // Form validation will not rebuild the form, so we need to ensure
  // necessary JavaScript will still exist.
  _webform_conditional_expand_value_forms($form['#node']);
}

/**
 * Submit handler for webform_conditionals_form().
 */
function webform_conditionals_form_submit($form, &$form_state) {
  $node = $form['#node'];

  // Remove the new conditional placeholder.
  unset($form_state['values']['conditionals']['new']);

  $conditionals = array();
  $targets = array();

  // Fill in missing properties for each value so that it can save properly.
  // TODO: Remove hard-coded source and target type.
  foreach ($form_state['values']['conditionals'] as $rgid => $conditional) {
    $conditional['rgid'] = $rgid;
    $conditional['target_type'] = 'component';
    foreach ($conditional['rules'] as $rid => $rule) {
      $conditional['rules'][$rid]['source_type'] = 'component';
    }
    $conditionals[$rgid] = $conditional;
    // Warn if there are multiple conditionals targetting the same component.
    if (isset($targets[$conditional['target']])) {
      drupal_set_message(t('More than one conditional hides or shows component "@name". Only the most-recently evaluated conditional will determine whether "@name" is displayed.',
                           array('@name' => $node->webform['components'][$conditional['target']]['name'])), 'warning', FALSE);
    }
    $targets[$conditional['target']] = TRUE;
  }

  $node->webform['conditionals'] = $conditionals;
  node_save($node);
  drupal_set_message(t('Conditionals for %title saved.', array('%title' => $node->title)));
}

/**
 * AJAX callback to render out adding a new condition.
 */
function webform_conditionals_ajax($form, $form_state) {
  $rgids = element_children($form['conditionals']);
  $new_rgid = max($rgids);
  $form['conditionals'][$new_rgid]['#ajax_added'] = TRUE;

  $commands = array('#type' => 'ajax');
  $commands['#commands'][] = ajax_command_before('.webform-conditional-new-row', drupal_render($form['conditionals'][$new_rgid]));
  $commands['#commands'][] = ajax_command_restripe('#webform-conditionals-table');
  return $commands;
}

/**
 * Theme the $form['conditionals'] of webform_conditionals_form().
 */
function theme_webform_conditional_groups($variables) {
  $element = $variables['element'];
  drupal_add_tabledrag('webform-conditionals-table', 'order', 'sibling', 'webform-conditional-weight');
  drupal_add_js('Drupal.theme.prototype.tableDragChangedMarker = function() { return ""; }', 'inline');
  drupal_add_js('Drupal.theme.prototype.tableDragChangedWarning = function() { return "<span>&nbsp;</span>"; }', 'inline');

  $output = '<table id="webform-conditionals-table"><tbody>';
  $element_children = element_children($element, TRUE);
  $element_count = count($element_children);
  foreach ($element_children as $index => $key) {
    if ($key === 'new') {
      $data = '';
      if ($element_count === 1) {
        $data = t('There are no conditional actions on this form.') . ' ';
      }
      $even_odd = ($index + 1) % 2 ? 'odd' : 'even';
      $element[$key]['weight']['#attributes']['class'] = array('webform-conditional-weight');
      $data = '<div class="webform-conditional-new">' . $data . t('Add a new condition: ') . drupal_render($element[$key]['new']) . '</div>';
      $output .= '<tr class="webform-conditional-new-row ' . $even_odd . '">';
      $output .= '<td>' . $data . '</td>';
      $output .= '<td>' . drupal_render($element[$key]['weight']) . '</td>';
      $output .= '</tr>';
    }
    else {
      $output .= drupal_render($element[$key]);
    }
  }
  $output .= '</tbody></table>';
  $output .= drupal_render_children($element);

  return $output;
}

/**
 * Theme an individual conditional row of webform_conditionals_form().
 */
function theme_webform_conditional_group_row($variables) {
  $element = $variables['element'];

  $element['weight']['#attributes']['class'] = array('webform-conditional-weight');
  $weight = drupal_render($element['weight']);
  $classes = array('draggable');
  if (!empty($element['#even_odd'])) {
    $classes[] = $element['#even_odd'];
  }
  if (!empty($element['#ajax_added'])) {
    $classes[] = 'ajax-new-content';
  }

  $output = '';
  $output .= '<tr class="' . implode(' ', $classes) . '">';
  $output .= '<td>' . drupal_render_children($element) . '</td>';
  $output .= '<td>' . $weight . '</td>';
  $output .= '</tr>';

  return $output;
}

/**
 * Form API #process function to expand a webform conditional element.
 */
function _webform_conditional_expand($element) {
  $element['#tree'] = TRUE;
  $element['#default_value'] += array(
    'andor' => 'and',
  );

  $wrapper_id = drupal_clean_css_identifier(implode('-', $element['#parents'])) . '-ajax';
  $element['#prefix'] = '<div id="' . $wrapper_id . '">';
  $element['#suffix'] = '</div>';

  foreach ($element['#default_value']['rules'] as $rid => $conditional) {
    $element['rules'][$rid]['source'] = array(
      '#type' => 'select',
      '#title' => t('Source'),
      '#options' => $element['#sources'],
      '#default_value' => $element['#default_value']['rules'][$rid]['source'],
    );
    $element['rules'][$rid]['operator'] = array(
      '#type' => 'select',
      '#title' => t('Operator'),
      '#options' => webform_conditional_operators_list(),
      '#default_value' => $element['#default_value']['rules'][$rid]['operator'],
    );
    $element['rules'][$rid]['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 30,
      '#default_value' => $element['#default_value']['rules'][$rid]['value'],
    );
    $element['rules'][$rid]['remove'] = array(
      '#type' => 'submit',
      '#value' => t('-'),
      '#submit' => array('webform_conditional_element_remove'),
      '#name' => implode('_', $element['#parents']) . '_rules_' . $rid . '_remove',
      '#attributes' => array('class' => array('webform-conditional-rule-remove')),
      '#ajax' => array(
        'progress' => 'none',
        'callback' => 'webform_conditional_element_ajax',
        'wrapper' => $wrapper_id,
        'event' => 'click',
      ),
    );
    $element['rules'][$rid]['add'] = array(
      '#type' => 'submit',
      '#value' => t('+'),
      '#submit' => array('webform_conditional_element_add'),
      '#name' => implode('_', $element['#parents']) . '_rules_' . $rid . '_add',
      '#attributes' => array('class' => array('webform-conditional-rule-add')),
      '#ajax' => array(
        'progress' => 'none',
        'callback' => 'webform_conditional_element_ajax',
        'wrapper' => $wrapper_id,
        'event' => 'click',
      ),
    );

    // The and/or selector is shown for every rule, even though the whole
    // conditional group shares a single and/or property. They are made to match
    // via JavaScript (though they all share the same "name" attribute so only
    // a single value is ever submitted via POST).
    $element['rules'][$rid]['andor'] = array(
      '#type' => 'select',
      '#title' => t('And/or'),
      '#options' => array(
        'and' => t('and'),
        'or' => t('or'),
      ),
      '#parents' => array_merge($element['#parents'], array('andor')),
      '#default_value' => $element['#default_value']['andor'],
    );
  }

  // Remove the last and/or.
  unset($element['rules'][$rid]['andor']);

  $element['action'] = array(
    '#type' => 'select',
    '#title' => t('Action'),
    '#options' => $element['#actions'],
    '#default_value' => $element['#default_value']['action'],
  );
  $element['target'] = array(
    '#type' => 'select',
    '#title' => t('Operator'),
    '#options' => $element['#targets'],
    '#default_value' => $element['#default_value']['target'],
  );

  return $element;
}

/**
 * Expand out all the value forms that could potentially be used.
 *
 * These forms are added to the page via JavaScript and swapped in only when
 * needed. Because the user may change the source and operator at any time,
 * all these forms need to be generated ahead of time and swapped in. This
 * could have been done via AJAX, but having all forms available makes for a
 * faster user experience.
 *
 * @param $node
 *   The Webform node for which these forms are being generated.
 * @return
 *   An array settings suitable for adding to the page via JavaScript. This
 *   array contains the following keys:
 *   - operators: An array containing a map of data types, operators, and form
 *     keys. This array is structured as follows:
 *     @code
 *   - sources[$source_key] = array(
 *       'data_type' => $data_type,
 *     );
 *     $operators[$data_type][$operator] = array(
 *       'form' => $form_key,
 *     );
 *     @endcode
 *   - forms[$form_key]: A string representing an HTML form for an operator.
 *   - forms[$form_key][$source]: Or instead of a single form for all components,
 *     if each component requires its own form, key each component by its source
 *     value (currently always the component ID).
 */
function _webform_conditional_expand_value_forms($node, $add_to_page = TRUE) {
  static $value_forms_added;

  $operators = webform_conditional_operators();
  $data = array();
  foreach ($operators as $data_type => $operator_info) {
    foreach ($operator_info as $operator => $data_operator_info) {
      $data['operators'][$data_type][$operator]['form'] = 'default';
      if (isset($data_operator_info['form callback'])) {
        $form_callback = $data_operator_info['form callback'];
        $data['operators'][$data_type][$operator]['form'] = $form_callback;
        if ($form_callback !== FALSE && !isset($value_forms_added[$form_callback])) {
          $data['forms'][$form_callback] = $form_callback($node);
        }
      }
    }
  }

  foreach ($node->webform['components'] as $cid => $component) {
    if (webform_component_feature($component['type'], 'conditional')) {
      $data['sources'][$cid]['data_type'] = webform_component_property($component['type'], 'conditional_type');
    }
  }

  if (!isset($value_forms_added) && $add_to_page) {
    $value_forms_added = TRUE;
    drupal_add_js(array('webform' => array('conditionalValues' => $data)), 'setting');
  }

  return $data;
}

/**
 * Submit handler for webform_conditional elements to add a new rule.
 */
function webform_conditional_element_add($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $parents = $button['#parents'];
  $action = array_pop($parents);
  $rid = array_pop($parents);

  // Recurse through the form values until we find the root Webform conditional.
  $parent_values = &$form_state['values'];
  foreach ($parents as $key) {
    if (array_key_exists($key, $parent_values)) {
      $parent_values = &$parent_values[$key];
    }
  }

  // Split the list of rules in this conditional and inject into the right spot.
  $rids = array_keys($parent_values);
  $offset = array_search($rid, $rids);
  $first = array_slice($parent_values, 0, $offset + 1);
  $second = array_slice($parent_values, $offset + 1);
  $new[0] = $parent_values[$rid];

  $parent_values = array_merge($first, $new, $second);

  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for webform_conditional elements to remove a rule.
 */
function webform_conditional_element_remove($form, &$form_state) {
  $button = $form_state['clicked_button'];
  $parents = $button['#parents'];
  $action = array_pop($parents);
  $current_rid = array_pop($parents);

  // Recurse through the form values until we find the root Webform conditional.
  $parent_values = &$form_state['values'];
  foreach ($parents as $key) {
    if (array_key_exists($key, $parent_values)) {
      $parent_values = &$parent_values[$key];
    }
  }

  // Remove this rule from the list of conditionals.
  unset($parent_values[$current_rid]);

  $form_state['rebuild'] = TRUE;
}


/**
 * AJAX callback to render out adding a new condition.
 */
function webform_conditional_element_ajax($form, $form_state) {
  $button = $form_state['clicked_button'];
  $parents = $button['#parents'];

  // Trim down the parents to go back up to the level of this elements wrapper.
  array_pop($parents); // The button name (add/remove).
  array_pop($parents); // The rule ID.
  array_pop($parents); // The "rules" grouping.

  $element = $form;
  foreach ($parents as $key) {
    $element = $element[$key];
  }

  return drupal_render($element['conditional']);
}

/**
 * Theme the form for a conditional action.
 */
function theme_webform_conditional($variables) {
  $element = $variables['element'];

  $output = '';
  $output .= '<div class="webform-conditional">';
  $output .= '<span class="webform-conditional-if">' . t('If') . '</span>';

  foreach (element_children($element['rules']) as $rid) {
    // Hide labels.
    $element['rules'][$rid]['source']['#title_display'] = 'none';
    $element['rules'][$rid]['operator']['#title_display'] = 'none';
    $element['rules'][$rid]['value']['#title_display'] = 'none';
    $element['rules'][$rid]['andor']['#title_display'] = 'none';

    $source = '<div class="webform-conditional-source">' . drupal_render($element['rules'][$rid]['source']) . '</div>';
    $operator = '<div class="webform-conditional-operator">' . drupal_render($element['rules'][$rid]['operator']) . '</div>';
    $value = '<div class="webform-conditional-value">' . drupal_render($element['rules'][$rid]['value']) . '</div>';

    $source_phrase = t('!source !operator !value', array(
      '!source' => $source,
      '!operator' => $operator,
      '!value' => $value,
    ));

    $output .= '<div class="webform-conditional-rule">';
    $output .= '<div class="webform-container-inline webform-conditional-condition">';
    $output .= $source_phrase;
    $output .= '</div>';

    if (isset($element['rules'][$rid]['andor'])) {
      $output .= '<div class="webform-conditional-andor webform-container-inline">';
      $output .= drupal_render($element['rules'][$rid]['andor']);
      $output .= '</div>';
    }

    if (isset($element['rules'][$rid]['add']) || isset($element['rules'][$rid]['remove'])) {
      $output .= '<span class="webform-conditional-operations webform-container-inline">';
      $output .= drupal_render($element['rules'][$rid]['remove']);
      $output .= drupal_render($element['rules'][$rid]['add']);
      $output .= '</span>';
    }

    $output .= '</div>';
  }

  // Hide labels.
  $element['action']['#title_display'] = 'none';
  $element['target']['#title_display'] = 'none';

  $action = '<div class="webform-conditional-action">' . drupal_render($element['action']) . '</div>';
  $target = '<div class="webform-conditional-target">' . drupal_render($element['target']) . '</div>';

  $target_phrase = t('then !action !target', array(
    '!action' => $action,
    '!target' => $target,
  ));

  $output .= '<div class="webform-container-inline">';
  $output .= $target_phrase;
  $output .= '</div>';

  $output .= '</div>';

  return $output;
}

/**
 * Return a list of all Webform conditional operators.
 */
function webform_conditional_operators() {
  static $operators;

  if (!isset($operators)) {
    $operators = module_invoke_all('webform_conditional_operator_info');
    drupal_alter('webform_conditional_operators', $operators);
  }

  return $operators;
}

/**
 * Return a nested list of all available operators, suitable for a select list.
 */
function webform_conditional_operators_list() {
  $options = array();
  $operators = webform_conditional_operators();

  foreach ($operators as $data_type => $type_operators) {
    $options[$data_type] = array();
    foreach ($type_operators as $operator => $operator_info) {
      $options[$data_type][$operator] = $operator_info['label'];
    }
  }

  return $options;
}

/**
 * Internal implementation of hook_webform_conditional_operator_info().
 *
 * Called from webform.module's webform_webform_conditional_operator_info().
 */
function _webform_conditional_operator_info() {
  // General operators:
  $operators['string']['equal'] = array(
    'label' => t('is'),
    'comparison callback' => 'webform_conditional_operator_string_equal',
    'js comparison callback' => 'conditionalOperatorStringEqual',
    // A form callback is not needed here, since we can use the default,
    // non-JavaScript textfield for all text and numeric fields.
    // 'form callback' => 'webform_conditional_operator_text',
  );
  $operators['string']['not_equal'] = array(
    'label' => t('is not'),
    'comparison callback' => 'webform_conditional_operator_string_not_equal',
    'js comparison callback' => 'conditionalOperatorStringNotEqual',
  );
  $operators['string']['contains'] = array(
    'label' => t('contains'),
    'comparison callback' => 'webform_conditional_operator_string_contains',
    'js comparison callback' => 'conditionalOperatorStringContains',
  );
  $operators['string']['does_not_contain'] = array(
    'label' => t('does not contain'),
    'comparison callback' => 'webform_conditional_operator_string_does_not_contain',
    'js comparison callback' => 'conditionalOperatorStringDoesNotContain',
  );
  $operators['string']['begins_with'] = array(
    'label' => t('begins with'),
    'comparison callback' => 'webform_conditional_operator_string_begins_with',
    'js comparison callback' => 'conditionalOperatorStringBeginsWith',
  );
  $operators['string']['ends_with'] = array(
    'label' => t('ends with'),
    'comparison callback' => 'webform_conditional_operator_string_ends_with',
    'js comparison callback' => 'conditionalOperatorStringEndsWith',
  );
  $operators['string']['empty'] = array(
    'label' => t('is blank'),
    'comparison callback' => 'webform_conditional_operator_string_empty',
    'js comparison callback' => 'conditionalOperatorStringEmpty',
    'form callback' => FALSE, // No value form at all.
  );
  $operators['string']['not_empty'] = array(
    'label' => t('is not blank'),
    'comparison callback' => 'webform_conditional_operator_string_not_empty',
    'js comparison callback' => 'conditionalOperatorStringNotEmpty',
    'form callback' => FALSE, // No value form at all.
  );

  // Numeric operators.
  $operators['numeric']['equal'] = array(
    'label' => t('is equal to'),
    'comparison callback' => 'webform_conditional_operator_numeric_equal',
    'js comparison callback' => 'conditionalOperatorNumericEqual',
  );
  $operators['numeric']['not_equal'] = array(
    'label' => t('is not equal to'),
    'comparison callback' => 'webform_conditional_operator_numeric_not_equal',
    'js comparison callback' => 'conditionalOperatorNumericNotEqual',
  );
  $operators['numeric']['greater_than'] = array(
    'label' => t('is greater than'),
    'comparison callback' => 'webform_conditional_operator_numeric_greater_than',
    'js comparison callback' => 'conditionalOperatorNumericGreaterThan',
  );
  $operators['numeric']['less_than'] = array(
    'label' => t('is less than'),
    'comparison callback' => 'webform_conditional_operator_numeric_less_than',
    'js comparison callback' => 'conditionalOperatorNumericLessThan',
  );
  $operators['numeric']['empty'] = array(
    'label' => t('is blank'),
    'comparison callback' => 'webform_conditional_operator_string_empty',
    'js comparison callback' => 'conditionalOperatorStringEmpty',
    'form callback' => FALSE, // No value form at all.
  );
  $operators['numeric']['not_empty'] = array(
    'label' => t('is not blank'),
    'comparison callback' => 'webform_conditional_operator_string_not_empty',
    'js comparison callback' => 'conditionalOperatorStringNotEmpty',
    'form callback' => FALSE, // No value form at all.
  );

  // Select operators.
  $operators['select']['equal'] = array(
    'label' => t('is'),
    'comparison callback' => 'webform_conditional_operator_string_equal',
    'js comparison callback' => 'conditionalOperatorStringEqual',
    'form callback' => 'webform_conditional_form_select',
  );
  $operators['select']['not_equal'] = array(
    'label' => t('is not'),
    'comparison callback' => 'webform_conditional_operator_string_not_equal',
    'js comparison callback' => 'conditionalOperatorStringNotEqual',
    'form callback' => 'webform_conditional_form_select',
  );
  $operators['select']['empty'] = array(
    'label' => t('is empty'),
    'comparison callback' => 'webform_conditional_operator_string_empty',
    'js comparison callback' => 'conditionalOperatorStringEmpty',
    'form callback' => FALSE, // No value form at all.
  );
  $operators['select']['not_empty'] = array(
    'label' => t('is not empty'),
    'comparison callback' => 'webform_conditional_operator_string_not_empty',
    'js comparison callback' => 'conditionalOperatorStringNotEmpty',
    'form callback' => FALSE, // No value form at all.
  );

  // Date operators:
  $operators['date']['equal'] = array(
    'label' => t('is on'),
    'comparison callback' => 'webform_conditional_operator_datetime_equal',
    'comparison prepare js' => 'webform_conditional_prepare_date_js',
    'js comparison callback' => 'conditionalOperatorDateEqual',
    'form callback' => 'webform_conditional_form_date',
  );
  $operators['date']['before'] = array(
    'label' => t('is before'),
    'comparison callback' => 'webform_conditional_operator_datetime_before',
    'comparison prepare js' => 'webform_conditional_prepare_date_js',
    'js comparison callback' => 'conditionalOperatorDateBefore',
    'form callback' => 'webform_conditional_form_date',
  );
  $operators['date']['after'] = array(
    'label' => t('is after'),
    'comparison callback' => 'webform_conditional_operator_datetime_after',
    'comparison prepare js' => 'webform_conditional_prepare_date_js',
    'js comparison callback' => 'conditionalOperatorDateAfter',
    'form callback' => 'webform_conditional_form_date',
  );

  // Time operators:
  $operators['time']['equal'] = array(
    'label' => t('is at'),
    'comparison callback' => 'webform_conditional_operator_datetime_equal',
    'comparison prepare js' => 'webform_conditional_prepare_time_js',
    'js comparison callback' => 'conditionalOperatorTimeEqual',
    'form callback' => 'webform_conditional_form_time',
  );
  $operators['time']['before'] = array(
    'label' => t('is before'),
    'comparison callback' => 'webform_conditional_operator_datetime_before',
    'comparison prepare js' => 'webform_conditional_prepare_time_js',
    'js comparison callback' => 'conditionalOperatorTimeBefore',
    'form callback' => 'webform_conditional_form_time',
  );
  $operators['time']['after'] = array(
    'label' => t('is after'),
    'comparison callback' => 'webform_conditional_operator_datetime_after',
    'comparison prepare js' => 'webform_conditional_prepare_time_js',
    'js comparison callback' => 'conditionalOperatorTimeAfter',
    'form callback' => 'webform_conditional_form_time',
  );

  return $operators;
}

/**
 * Form callback for select-type conditional fields.
 *
 * Unlike other built-in conditional value forms, the form callback for select
 * types provides an array of forms, keyed by the $cid, which is the "source"
 * for the condition.
 */
function webform_conditional_form_select($node) {
  static $count = 0;
  $forms = array();
  webform_component_include('select');
  foreach ($node->webform['components'] as $cid => $component) {
    if (webform_component_property($component['type'], 'conditional_type') == 'select') {
      // TODO: Use a pluggable mechanism for retrieving select list values.
      $options = _webform_select_options($component);
      $element = array(
        '#type' => 'select',
        '#multiple' => FALSE,
        '#size' => NULL,
        '#attributes' => array(),
        '#id' => NULL,
        '#name' => 'webform-conditional-select-' . $cid . '-' . $count,
        '#options' => $options,
        '#parents' => array(),
      );
      $forms[$cid] = drupal_render($element);
    }
  }
  $count++;
  return $forms;
}

/**
 * Form callback for date conditional fields.
 */
function webform_conditional_form_date($node) {
  static $count = 0;
  $element = array(
    '#title' => NULL,
    '#title_display' => 'none',
    '#size' => 24,
    '#attributes' => array('placeholder' => t('@format or valid date', array('@format' => webform_date_format('short')))),
    '#type' => 'textfield',
    '#name' => 'webform-conditional-date-' . $count++,
  );
  return drupal_render($element);
}

/**
 * Form callback for time conditional fields.
 */
function webform_conditional_form_time($node) {
  static $count = 0;
  $element = array(
    '#title' => NULL,
    '#title_display' => 'none',
    '#size' => 24,
    '#attributes' => array('placeholder' => t('HH:MMam or valid time')),
    '#type' => 'textfield',
    '#name' => 'webform-conditional-time-' . $count++,
  );
  return drupal_render($element);
}

/**
 * Load a conditional setting from the database.
 */
function webform_conditional_load($rgid, $nid) {
  $node = node_load($nid);

  $conditional = isset($node->webform['conditionals'][$rgid]) ? $node->webform['conditionals'][$rgid] : FALSE;

  return $conditional;
}

/**
 * Insert a conditional rule group into the database.
 */
function webform_conditional_insert($conditional) {
  drupal_write_record('webform_conditional', $conditional);
  foreach ($conditional['rules'] as $rid => $rule) {
    $rule['nid'] = $conditional['nid'];
    $rule['rgid'] = $conditional['rgid'];
    $rule['rid'] = $rid;
    drupal_write_record('webform_conditional_rules', $rule);
  }
}

/**
 * Update a conditional setting in the database.
 */
function webform_conditional_update($node, $conditional) {
  webform_conditional_delete($node, $conditional);
  webform_conditional_insert($conditional);
}

/**
 * Delete a conditional rule group.
 */
function webform_conditional_delete($node, $conditional) {
  db_delete('webform_conditional')
    ->condition('nid', $node->nid)
    ->condition('rgid', $conditional['rgid'])
    ->execute();
  db_delete('webform_conditional_rules')
    ->condition('nid', $node->nid)
    ->condition('rgid', $conditional['rgid'])
    ->execute();
}

/**
 * Loop through all the conditional settings and add needed JavaScript settings.
 *
 * We do a bit of optimization for JavaScript before adding to the page as
 * settings. We remove unnecessary data structures and provide a "source map"
 * so that JavaScript can quickly determine if it needs to check rules when a
 * field on the page has been modified.
 */
function webform_conditional_prepare_javascript($node, $submission_data) {
  $settings = array(
    'ruleGroups' => array(),
    'sourceMap' => array(),
    'values' => array(),
  );
  $operators = webform_conditional_operators();
  foreach ($node->webform['conditionals'] as $conditional) {
    // Assemble the main conditional group settings.
    if ($conditional['target_type'] == 'component') {
      $target_component = $node->webform['components'][$conditional['target']];
      $target_parents = webform_component_parent_keys($node, $target_component);
      $target_id = 'webform-component--' . str_replace('_', '-', implode('--', $target_parents));
      $settings['ruleGroups'][$conditional['rgid']]['target'] = $target_id;
      $settings['ruleGroups'][$conditional['rgid']]['andor'] = $conditional['andor'];
      $settings['ruleGroups'][$conditional['rgid']]['action'] = $conditional['action'];
    }
    // Add on the list of rules to the conditional group.
    foreach ($conditional['rules'] as $rule) {
      if ($rule['source_type'] == 'component') {
        $source_component = $node->webform['components'][$rule['source']];
        $source_parents = webform_component_parent_keys($node, $source_component);
        $source_id = 'webform-component--' . str_replace('_', '-', implode('--', $source_parents));

        // If this source has a value set, add that as a setting.
        if (isset($submission_data[$source_component['cid']])) {
          $source_value = $submission_data[$source_component['cid']];
          $settings['values'][$source_id] = is_array($source_value) ? $source_value : array($source_value);
        }

        $conditional_type = webform_component_property($source_component['type'], 'conditional_type');
        $operator_info = $operators[$conditional_type][$rule['operator']];
        $rule_settings = array();
        $rule_settings['source'] = $source_id;
        $rule_settings['value'] = $rule['value'];
        $rule_settings['callback'] = $operator_info['js comparison callback'];
        if (isset($operator_info['comparison prepare js'])) {
          $callback = $operator_info['comparison prepare js'];
          $rule_settings['value'] = $callback($rule['value']);
        }
        $settings['ruleGroups'][$conditional['rgid']]['rules'][$rule['rid']] = $rule_settings;
        $settings['sourceMap'][$source_id][$conditional['rgid']] = $conditional['rgid'];
      }
    }
  }

  return $settings;
}

/**
 * Prepare a conditional value for adding as a JavaScript setting.
 */
function webform_conditional_prepare_date_js($rule_value) {
  // Convert the time/date string to a UTC timestamp for comparison. Note that
  // this means comparisons against immediate times (such as "now") may be
  // slightly stale by the time the comparison executes. Timestamps are in
  // milliseconds, as to match JavaScript's Date.toString() method.
  $date = webform_strtodate('c', $rule_value, 'UTC');
  return webform_strtotime($date);
}

/**
 * Prepare a conditional value for adding as a JavaScript setting.
 */
function webform_conditional_prepare_time_js($rule_value) {
  $date = webform_conditional_prepare_date_js($rule_value);
  $today = webform_strtodate('c', 'today', 'UTC');
  $today = webform_strtotime($today);
  return $date - $today;
}

/**
 * Conditional callback for string comparisons.
 */
function webform_conditional_operator_string_equal($input_values, $rule_value) {
  foreach ($input_values as $value) {
    // Checkbox values come in as 0 integers for unchecked boxes.
    $value = ($value === 0) ? '' : $value;
    if (strcasecmp($value, $rule_value) === 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Conditional callback for string comparisons.
 */
function webform_conditional_operator_string_not_equal($input_values, $rule_value) {
  return !webform_conditional_operator_string_equal($input_values, $rule_value);
}

/**
 * Conditional callback for string comparisons.
 */
function webform_conditional_operator_string_contains($input_values, $rule_value) {
  foreach ($input_values as $value) {
    if (stripos($value, $rule_value) !== FALSE) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Conditional callback for string comparisons.
 */
function webform_conditional_operator_string_does_not_contain($input_values, $rule_value) {
  return !webform_conditional_operator_string_contains($input_values, $rule_value);
}

/**
 * Conditional callback for string comparisons.
 */
function webform_conditional_operator_string_begins_with($input_values, $rule_value) {
  foreach ($input_values as $value) {
    if (stripos($value, $rule_value) === 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Conditional callback for string comparisons.
 */
function webform_conditional_operator_string_ends_with($input_values, $rule_value) {
  foreach ($input_values as $value) {
    if (strripos($value, $rule_value) === strlen($value) - strlen($rule_value)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Conditional callback for checking for empty fields.
 */
function webform_conditional_operator_string_empty($input_values, $rule_value) {
  $empty = TRUE;
  foreach ($input_values as $value) {
    if ($value !== '' && $value !== NULL && $value !== 0) {
      $empty = FALSE;
      break;
    }
  }
  return $empty;
}

/**
 * Conditional callback for checking for empty fields.
 */
function webform_conditional_operator_string_not_empty($input_values, $rule_value) {
  return !webform_conditional_operator_string_empty($input_values, $rule_value);
}

/**
 * Conditional callback for numeric comparisons.
 */
function webform_conditional_operator_numeric_equal($input_values, $rule_value) {
  return empty($input_values) ? FALSE : webform_compare_floats($input_values[0], $rule_value) === 0;
}

/**
 * Conditional callback for numeric comparisons.
 */
function webform_conditional_operator_numeric_not_equal($input_values, $rule_value) {
  return !webform_conditional_operator_numeric_equal($input_values, $rule_value);
}

/**
 * Conditional callback for numeric comparisons.
 */
function webform_conditional_operator_numeric_greater_than($input_values, $rule_value) {
  return empty($input_values) ? FALSE : webform_compare_floats($input_values[0], $rule_value) > 0;
}

/**
 * Conditional callback for numeric comparisons.
 */
function webform_conditional_operator_numeric_less_than($input_values, $rule_value) {
  return empty($input_values) ? FALSE : webform_compare_floats($input_values[0], $rule_value) < 0;
}

/**
 * Conditional callback for date and time comparisons.
 */
function webform_conditional_operator_datetime_equal($input_values, $rule_value) {
  $input_values = webform_conditional_value_datetime($input_values);
  return empty($input_values) ? FALSE : webform_strtotime($input_values[0]) === webform_strtotime($rule_value);
}

/**
 * Conditional callback for date and time comparisons.
 */
function webform_conditional_operator_datetime_after($input_values, $rule_value) {
  $input_values = webform_conditional_value_datetime($input_values);
  return empty($input_values) ? FALSE : webform_strtotime($input_values[0]) > webform_strtotime($rule_value);
}

/**
 * Conditional callback for date and time comparisons.
 */
function webform_conditional_operator_datetime_before($input_values, $rule_value) {
  $input_values = webform_conditional_value_datetime($input_values);
  return empty($input_values) ? FALSE : webform_strtotime($input_values[0]) < webform_strtotime($rule_value);
}

/**
 * Utility function to convert incoming time and dates into strings.
 */
function webform_conditional_value_datetime($input_values) {
  // Convert times into a string.
  $input_values = isset($input_values['hour']) ? array(webform_date_string(webform_time_convert($input_values, '24-hour'), 'time')) : $input_values;
  // Convert dates into a string.
  $input_values = isset($input_values['month']) ? array(webform_date_string($input_values, 'date')) : $input_values;
  return $input_values;
}
