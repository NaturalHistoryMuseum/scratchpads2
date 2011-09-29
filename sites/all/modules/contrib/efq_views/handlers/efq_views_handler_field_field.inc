<?php
// $Id$

/**
 * Field handler for fields.
 */
class efq_views_handler_field_field extends views_handler_field_field {

  /**
   * Return TRUE if the user has access to view this field.
   */
  function access() {
    return field_access('view', $this->field_info, $this->definition['entity_type']);
  }

  /**
   * Determine if this field is click sortable..
   */
  function click_sortable() {
    if (empty($this->definition['click sortable'])) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Called to determine what to tell the clicksorter.
   */
  function click_sort($order) {
    $this->query->query->fieldOrderBy($this->field, $this->options['click_sort_column'], $order);
  }

  /**
   * Override the parent's query method, since it doesn't need to actually
   * do anything.
   */
  function query($use_groupby = FALSE) {}

  /**
   * Provide options for multiple value fields.
   *
   * This module only supports showing all values in a single row, so the
   * options need to change accordingly.
   */
  function multiple_options_form(&$form, &$form_state) {
    parent::multiple_options_form($form, $form_state);

    $form['group_rows'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );

    // No reason for elements to depend on group_rows anymore.
    unset($form['multi_type']['#dependency']);
    unset($form['separator']['#dependency']['edit-options-group-rows']);
    unset($form['separator']['#dependency_count']);
    unset($form['delta_limit']['#dependency']);
    unset($form['delta_offset']['#dependency']);
    unset($form['delta_reversed']['#dependency']);
  }

  /**
   * Prepare $values to a format that the parent::render method understands,
   * so that we don't have to reinvent it.
   */
  function post_execute(&$values) {
    $this->field_alias = $this->field;

    foreach ($values as $id => $entity) {
      if (!isset($entity->{$this->field_alias})) {
        continue;
      }

      $values[$id]->_field_data[$this->field_alias] = array(
        'entity_type' => $entity->entity_type,
        'entity' => $entity,
      );
    }

    // Now, transfer the data back into the resultset so it can be easily used.
    foreach ($values as $row_id => &$value) {
      $value->{'field_' . $this->options['id']} = $this->set_items($value, $row_id);
    }
  }
}
