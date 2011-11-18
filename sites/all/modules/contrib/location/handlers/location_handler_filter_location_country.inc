<?php

/**
 * Filter on country.
 */
class location_handler_filter_location_country extends views_handler_filter_in_operator {

  function option_definition() {
    $options = parent::option_definition();
    $options['operator'] = array('default' => 'IS');

    return $options;
  }


  function admin_summary() {
    return '';
//    $options = $this->operator_options('short');
//    return (!empty($this->options['exposed']) ? t('exposed') : $options[$this->operator]);
  }

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = location_get_iso3166_list();
  }

  /**
   * Provide widgets for filtering by country.
   */
  function value_form(&$form, &$form_state) {
    $this->get_value_options();
    $options = $this->value_options;
    $default_value = (array) $this->value;

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(1)) ? 'value' : 'none';
      }
      else {
        $source = 'edit-' . drupal_clean_css_identifier($this->options['expose']['operator']);
      }

      if (!empty($this->options['expose']['reduce'])) {
        $options = $this->reduce_value_options();

        if (empty($this->options['expose']['single']) && !empty($this->options['expose']['optional'])) {
          $default_value = array();
        }
      }

      if (!empty($this->options['expose']['single'])) {
        if (!empty($this->options['expose']['optional']) && (empty($default_value) || !empty($this->options['expose']['reduce']))) {
          $default_value = 'All';
        }
        else if (empty($default_value)) {
          $keys = array_keys($options);
          $default_value = array_shift($keys);
        }
        else {
          $copy = $default_value;
          $default_value = array_shift($copy);
        }
      }
    }

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#default_value' => $default_value,
      '#options' => $options,
      // Used by province autocompletion js.
      '#attributes' => array('class' => array('location_auto_country')),
      '#multiple' => TRUE, // views will change this as necessary when exposed.
    );

    // Let location_autocomplete.js find the correct fields to attach.
    $form['value']['#attributes']['class'][] = 'location_auto_join_' . $this->options['expose']['identifier'];
  }

  function operator_options($which = 'title') {
    if ($this->options['expose']['single']) {
      return array(
        'is' => t('Is'),
        'is not' => t('Is not'),
      );
    }
    else {
      return array(
        'is' => t('Is one of'),
        'is not' => t('Is not one of'),
      );
    }
  }

  function query() {
    if (empty($this->value)) {
      return;
    }

    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    // Normalize values.
    $value = $this->value;
    if (is_array($value)) {
      if (count($value) == 1) {
        // If multiple is allowed but only one was chosen, use a string instead.
        $value = reset($value);
      }
    }

    if (is_array($value)) {
      // Multiple values
      $operator = ($this->operator == 'is') ? 'IN' : 'NOT IN';
      $this->query->add_where($this->options['group'], $field, $value, $operator);
    }
    else {
      // Single value
      $operator = ($this->operator == 'is') ? '=' : '!=';
      $this->query->add_where($this->options['group'], $field, $value, $operator);
    }
  }
}
