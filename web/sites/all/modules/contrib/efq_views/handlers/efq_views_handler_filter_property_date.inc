<?php
// $Id$

/**
 * Filter handler for date properties.
 */
class efq_views_handler_filter_property_date extends views_handler_filter_date {
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    unset($form['value']['type']);
    $form['value']['value']['#description'] = t('A date in any machine readable format. CCYY-MM-DD HH:MM:SS is preferred.');
  }

  function query() {
    $info = $this->operators();
    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($this->real_field);
    }
  }

  function op_between($field) {
    if ($this->operator == 'between') {
      $a = intval(strtotime($this->value['min'], 0));
      $b = intval(strtotime($this->value['max'], 0));
    }
    else {
      $a = intval(strtotime($this->value['max'], 0));
      $b = intval(strtotime($this->value['min'], 0));
    }
    $this->query->query->propertyCondition($field, array($a, $b), 'BETWEEN');
  }

  function op_simple($field) {
    $value = intval(strtotime($this->value['value'], 0));
    $this->query->query->propertyCondition($field, $value, $this->operator);
  }
}
