<?php
// $Id$

/**
 * Filter on an entity property
 */
class efq_views_handler_filter_property extends views_handler_filter {
  /**
   * This filter is not exposable.
   */
  function can_expose() {
    return FALSE;
  }

  function admin_summary() {
    // Avoid generating notices if the field hasn't been set up yet.
    if (!is_array($this->value)) {
      return '';
    }

    $output = check_plain($this->value['column'] . ' ' . $this->operator);
    if (in_array($this->operator, $this->operator_values(2))) {
      $output .= ' ' . t('@min and @max', array('@min' => $this->value['min'], '@max' => $this->value['max']));
    }
    elseif (in_array($this->operator, $this->operator_values(1))) {
      $output .= ' ' . check_plain($this->value['value']);
    }

    return $output;
  }

  /**
   * Defines all possible operators (for both numeric and string operations).
   *  We do so because we don't know whether the property is numeric or string.
   */
  function operators() {
    $operators = array(
      '<' => array(
        'title' => t('Is less than'),
        'method' => 'op_simple',
        'short' => t('<'),
        'values' => 1,
      ),
      '<=' => array(
        'title' => t('Is less than or equal to'),
        'method' => 'op_simple',
        'short' => t('<='),
        'values' => 1,
      ),
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'method' => 'op_simple',
        'short' => t('!='),
        'values' => 1,
      ),
      '>=' => array(
        'title' => t('Is greater than or equal to'),
        'method' => 'op_simple',
        'short' => t('>='),
        'values' => 1,
      ),
      '>' => array(
        'title' => t('Is greater than'),
        'method' => 'op_simple',
        'short' => t('>'),
        'values' => 1,
      ),
      'between' => array(
        'title' => t('Is between'),
        'method' => 'op_between',
        'short' => t('between'),
        'values' => 2,
      ),
      'contains' => array(
        'title' => t('Contains'),
        'short' => t('contains'),
        'method' => 'op_contains',
        'values' => 1,
      ),
      'starts' => array(
        'title' => t('Starts with'),
        'short' => t('begins'),
        'method' => 'op_starts',
        'values' => 1,
      ),
    );

    return $operators;
  }

  function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      $options[$id] = $info[$which];
    }

    return $options;
  }

  function operator_values($values = 1) {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      if (isset($info['values']) && $info['values'] == $values) {
        $options[] = $id;
      }
    }

    return $options;
  }

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    $form['value']['column'] = array(
      '#type' => 'textfield',
      '#title' => t('Property column'),
      '#default_value' => $this->value['column'],
      '#description' => t('Name of the database column to use in the comparison.'),
    );

    // We know which properties are available, so we can show a select box.
    if (isset($this->definition['available_properties'])) {
      $form['value']['column']['#type'] = 'select';
      $form['value']['column']['#options'] = $this->definition['available_properties'];
    }

    $form['value']['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#size' => 30,
      '#default_value' => $this->value['value'],
      '#process' => array('efq_views_value_form_process', 'ctools_dependent_process'),
      '#dependency' => array('radio:options[operator]' => array_map('htmlentities', $this->operator_values(1))),
      '#column' => &$form['value']['column'],
      '#types' => $this->definition['types'],
    );
    $form['value']['min'] = array(
      '#type' => 'textfield',
      '#title' => t('Min'),
      '#size' => 30,
      '#default_value' => $this->value['min'],
      '#process' => array('efq_views_value_form_process', 'ctools_dependent_process'),
      '#dependency' => array('radio:options[operator]' => array_map('htmlentities', $this->operator_values(2))),
      '#column' => &$form['value']['column'],
      '#types' => $this->definition['types'],
    );
    $form['value']['max'] = array(
      '#type' => 'textfield',
      '#title' => t('And max'),
      '#size' => 30,
      '#default_value' => $this->value['max'],
      '#process' => array('efq_views_value_form_process', 'ctools_dependent_process'),
      '#dependency' => array('radio:options[operator]' => array_map('htmlentities', $this->operator_values(2))),
      '#column' => &$form['value']['column'],
      '#types' => $this->definition['types'],
    );
  }

  function query() {
    if (!$this->value['column']) {
      return;
    }

    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}();
    }
  }

  function op_simple() {
    $this->query->query->propertyCondition($this->value['column'], $this->value['value'], $this->operator);
  }

  function op_between() {
    $this->query->query->propertyCondition($this->value['column'], array($this->value['min'], $this->value['max']), "BETWEEN");
  }

  function op_contains() {
    $this->query->query->propertyCondition($this->value['column'], $this->value['value'], "CONTAINS");
  }

  function op_starts() {
    $this->query->query->propertyCondition($this->value['column'], $this->value['value'], "STARTS_WITH");
  }
}

/**
 * Cast the provided value to the needed datatype before filtering.
 * Databases such as mongodb require this.
 */
function efq_views_value_form_process($element, &$form_state) {
  $column = $element['#column']['#value'];
  // There is no type info for this column, nothing left to do.
  if (!isset($element['#types'][$column])) {
    return;
  }
  
  $type = $element['#types'][$column];
  switch ($type) {
    case 'int':
      $element['#value'] = (int) $element['#value'];
      form_set_value($element, $element['#value'], $form_state);
      break;
  }
  return $element;
}
