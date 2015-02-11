<?php

/**
 * @file
 * Definition of webform_handler_filter_submission_data.
 */

/**
 * Extended version of the string filter handler specialized for Webform values.
 *
 * @ingroup views_filter_handlers
 */
class webform_handler_filter_submission_data extends views_handler_filter_string {
  /**
   * This kind of construct makes it relatively easy for a child class
   * to add or remove functionality by overriding this function and
   * adding/removing items from this array.
   */
  function operators() {
    $operators = parent::operators();

    // Add additional operators for date/time ranges.
    $operators['>'] = array(
      'title' => t('Greater than'),
      'short' => t('>'),
      'method' => 'op_greater_than',
      'values' => 1,
    );
    $operators['<'] = array(
      'title' => t('Less than'),
      'short' => t('>'),
      'method' => 'op_greater_than',
      'values' => 1,
    );

    return $operators;
  }

  /**
   * Build strings from the operators() for 'select' options
   */
  function operator_options($which = 'title') {
    $options = parent::operator_options($which);

    // Adjust the exposed filter options based on the component selected.
    if ($which === 'title') {
      $nid = $this->view->relationship[$this->options['relationship']]->options['webform_nid'];
      $cid = $this->view->relationship[$this->options['relationship']]->options['webform_cid'];

      if ($nid && $node = $node = node_load($nid)) {
        $component = $node->webform['components'][$cid];
        if (webform_component_feature($component['type'], 'views_range')) {
          $options['='] = t('Is');
          $options['!='] = t('Is not');
          $options['>'] = t('After');
          $options['<'] = t('Before');
          $options = array_intersect_key($options, array('=' => '=', '!=' => '!=', '>' => '>', '<' => '<'));
        }
      }
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
    // TODO: Adjust the exposed filter form based on component form.
    return parent::value_form($form, $form_state);
  }

  function op_greater_than($field) {
    $this->query->add_where($this->options['group'], $field, $this->value, '>');
  }

  function op_less_than($field) {
    $this->query->add_where($this->options['group'], $field, $this->value, '<');
  }
}
