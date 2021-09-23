<?php
/**
 * @file
 */


/**
 * Extends Views in operator filter.
 */
class cck_select_other_handler_filter extends views_handler_filter_in_operator {

  /**
   * Init
   */
  function init(&$view, &$options) {
    parent::init($view, $options);

    $this->instance = field_info_instance($this->definition['entity_type'], $this->definition['field_name'], $this->definition['bundle']);
  }

  /**
   * Exposed form
   */
  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);

    $identifier = $this->options['expose']['identifier'];

    // Populate other default value if any.
    $otherdef = '';
    $other_values = array_diff($this->value, $this->value_options);
    if (!empty($other_values)) {
      $otherdef = array_pop($other_values);
      $form[$identifier]['select_other_list']['#default_value'][] = 'other';
    }

    $form[$identifier]['select_other_text_input'] = array(
      '#type' => 'textfield',
      '#title' => t('Other'),
      '#title_display' => 'invisible',
      '#attributes' => array(
        'class' => array('form-text select_other_text_input'),
      ),
      '#default_value' => $otherdef,
      '#parents' => array('options', 'value', 'select_other_text_input'),
      '#size' => 10,
    );

    $form[$identifier]['#parents'] = array('options', $identifier);
    $form[$identifier]['select_other_list']['#type'] = 'select';
    $form[$identifier]['select_other_list']['#multiple'] = $this->options['expose']['multiple'];
    $form[$identifier]['select_other_list']['#parents'] = array('options', $identifier, 'select_other_list');
    $form[$identifier]['select_other_text_input']['#parents'] = array('options', $identifier, 'select_other_text_input');

    $field_id = str_replace('_', '-', 'options-' . $identifier);
    drupal_add_js(array('CCKSelectOther' => array(array('field_id' => $field_id))), array('type' => 'setting'));
  }

  function exposed_submit(&$form, &$form_state) {
    $identifier = $this->options['expose']['identifier'];
    $values = $form_state['values']['options'][$identifier];
    $form_state['values'][$identifier] = array();

    if (is_array($values['select_other_list'])) {
      // Multiple values will be stored in an array.
      foreach ($values['select_other_list'] as $key => $value) {
        if ($value && $key == $value) {
          if ($value <> 'other') {
            $form_state['values'][$identifier][] = $value;
          }
          else {
            // Set the other value instead of 'other'
            $form_state['values'][$identifier][] = $values['select_other_text_input'];
          }
        }
      }
    }
    else {
      if ($values['select_other_list'] == 'other') {
        $form_state['values'][$identifier] = $values['select_other_text_input'];
      }
      else {
        $form_state['values'][$identifier] = $values['select_other_list'];
      }
    }

  }

  /**
   * Value form
   */
  function value_form(&$form, &$form_state) {
    static $js;

    parent::value_form($form, $form_state);

    $default = array();
    foreach ($this->value as $key => $value) {
      // Populate default values.
      if (in_array($value, $this->value_options)) {
        $default[] = $value;
      }
      else {
        $default[] = 'other';
      }
    }

    $options = $form['value']['#options'];
    $form['value'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      '#parents' => array('options', 'value'),
      'select_other_list' => array(
        // @note I am using checkboxes instead of select in the filter.
        '#type' => 'checkboxes',
        '#options' => $options,
        '#default_value' => $default,
        '#parents' => array('options', 'value', 'select_other_list'),
      ),
    );

    if (!$js) {
      drupal_add_js(drupal_get_path('module', 'cck_select_other') . '/cck_select_other.js');
      $js = TRUE;
    }
    drupal_add_js(array('CCKSelectOther' => array(array('field_id' => 'options-value'))), array('type' => 'setting'));
  }

  function value_submit($form, &$form_state) {
    // Capture the values into a separate array and reset the value array.
    $values = $form_state['values']['options']['value'];
    $form_state['values']['options']['value'] = array();

    foreach ($values['select_other_list'] as $key => $value) {
      if ($value && $key == $value) {
        $form_state['values']['options']['value'][] = $value;
      }
    }
  }

  function get_value_options() {
    $this->value_options = cck_select_other_options($this->instance);

    // Get rid of none options.
    unset($this->value_options['']);
  }
  
  function accept_exposed_input($input) {
    $ret = parent::accept_exposed_input($input);
    return $ret;
  }

  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    $info = $this->operators();

    $this->get_value_options();

    if (!is_array($this->value)) {
      return;
    }

    $operator = check_plain($info[$this->operator]['short']);
    $values = '';

    $count = count($this->value);
    $i = 1;
    foreach ($this->value as $value) {
      if (isset($this->value_options[$value])) {
        $values .= check_plain($this->value_options[$value]);
      }
      else {
        $values .= t('%value', array('%value' => $value));
      }
      if ($i < $count) {
        $values .= ', ';
      }
      $i++;
    }

    return $operator . (($values !== '') ? ' (' . $values . ')' : '');
  }

  function op_simple() {
    if (empty($this->value)) {
      return;
    }

    if (in_array('other', $this->value)) {
      // We need to reverse the operator and find the inverse.
      $this->ensure_my_table();

      $values = array_diff($this->value_options, $this->value);
      $operator = ($this->operator == 'in') ? 'not in' : 'in';
      $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field", array_values($values), $operator);
    }
    else {
      // Query as normal.
      parent::op_simple();
    }
  }
}
