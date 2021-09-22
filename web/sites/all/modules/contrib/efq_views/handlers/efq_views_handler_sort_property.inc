<?php
// $Id$

/**
 * Sort handler for entity properties.
 */
class efq_views_handler_sort_property extends views_handler_sort {
  function can_expose() {
    return FALSE;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['column'] = array('default' => '');

    return $options;
  }

  function admin_summary() {
    switch ($this->options['order']) {
      case 'ASC':
      case 'asc':
      default:
        return $this->options['column'] . ' ' . t('asc');
        break;
      case 'DESC';
      case 'desc';
        return $this->options['column'] . ' ' . t('desc');
        break;
    }
  }

  function show_sort_form(&$form, &$form_state) {
    $options = $this->sort_options();
    if (!empty($options)) {
      $form['order'] = array(
        '#type' => 'radios',
        '#options' => $options,
        '#default_value' => $this->options['order'],
      );
    }

    $form['column'] = array(
      '#type' => 'textfield',
      '#title' => t('Property column'),
      '#default_value' => $this->options['column'],
      '#description' => t('Name of the database column to sort on.'),
    );

    // We know which properties are available, so we can show a select box.
    if (isset($this->definition['available_properties'])) {
      $form['column']['#type'] = 'select';
      $form['column']['#options'] = $this->definition['available_properties'];
    }
  }

  function query() {
    $this->query->query->propertyOrderBy($this->options['column'], $this->options['order']);
  }
}
