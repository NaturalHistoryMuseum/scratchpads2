<?php

/**
 * @file
 * Main filter handler for Apache Solr Views.
 */

class apachesolr_views_handler_filter extends views_handler_filter {

  /**
   * Add this filter to the query.
   */
  public function query() {
    $this->trim_value();

    if (!empty($this->value)) {
      if (is_array($this->value)) {
        $this->value = implode(' OR ', $this->value);
      }
      $this->query->add_where($this->options['group'], $this->real_field, $this->value, $this->operator);
    }
  }

  /**
   * Trim filter value.
   */
  public function trim_value() {
    // Trim both array or scalar value.
    if (is_array($this->value)) {
      array_walk($this->value, 'apachesolr_views_handler_filter::trim');
      $this->value = array_filter($this->value);
    }
    else {
      $this->value = trim($this->value);
    }
  }

  /**
   * Trim value.
   */
  public function trim(&$value) {
    $value = trim($value);
  }

  /**
   * Provide a simple textfield for equality.
   */
  public function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t($this->definition['title']),
      '#default_value' => $this->value,
    );
  }
}
