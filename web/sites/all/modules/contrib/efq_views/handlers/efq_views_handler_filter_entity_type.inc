<?php
// $Id$

/**
 * Filter based on entity type.
 */
class efq_views_handler_filter_entity_type extends views_handler_filter_equality {
  function value_form(&$form, &$form_state) {
    $entities = entity_get_info();

    foreach ($entities as $type => $info) {
      $label = isset($info['label']) ? $info['label'] : $type;
      $options[$type] = $label;
    }

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Entity type'),
      '#options' => $options,
      '#default_value' => $this->value,
    );

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      if (!isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
    }
  }

  function query() {
    $this->query->query->entityCondition('entity_type', $this->value, $this->operator);
  }
}