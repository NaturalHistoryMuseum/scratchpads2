<?php

/**
* @file
* Views field handler. Contains all relevant VBO options and related logic.
* Implements the Views Form API.
*/

class views_bulk_operations_handler_field_operations extends views_handler_field {
  var $revision = FALSE;

  function init(&$view, &$options) {
    parent::init($view, $options);

    // Update old settings.
    if (!empty($options['vbo']['selected_operations'])) {
      foreach (array_filter($options['vbo']['selected_operations']) as $operation_id) {
        $this->options['vbo']['operations'][$operation_id]['selected'] = TRUE;
        $this->options['vbo']['operations'][$operation_id]['skip_confirmation'] = $options['vbo']['skip_confirmation'];
      }
      unset($this->options['vbo']['selected_operations']);
    }
    // Prefix all un-prefixed operations.
    foreach ($this->options['vbo']['operations'] as $operation_id => &$operation_options) {
      if (strpos($operation_id, '::') === FALSE) {
        $operations = views_bulk_operations_get_operation_info();
        // Basically, guess.
        foreach (array('action', 'rules_component') as $operation_type) {
          $new_operation_id = $operation_type . '::' . $operation_id;
          if (isset($operations[$new_operation_id])) {
            $this->options['vbo']['operations'][$new_operation_id] = $operation_options;
            break;
          }
        }

        // Remove the old operation in any case.
        unset($this->options['vbo']['operations'][$operation_id]);
      }
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['vbo'] = array(
      'contains' => array(
        'entity_load_capacity' => array('default' => 10),
        'operations' => array('default' => array()),
        'enable_select_all_pages' => array('default' => TRUE),
        'display_type' => array('default' => 0),
        'display_result' => array('default' => TRUE),
        'merge_single_action' => array('default' => FALSE),
        'force_single' => array('default' => FALSE),
      ),
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['vbo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Bulk operations'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['vbo']['display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display operations as'),
      '#default_value' => $this->options['vbo']['display_type'],
      '#options' => array(
        t('Dropdown selectbox with Submit button'),
        t('Each action as a separate button'),
      ),
    );
    $form['vbo']['enable_select_all_pages'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable "Select all items on all pages"'),
      '#default_value' => $this->options['vbo']['enable_select_all_pages'],
      '#description' => t('Check this box to enable the ability to select all items on all pages.'),
    );
    $form['vbo']['force_single'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force single'),
      '#default_value' => $this->options['vbo']['force_single'],
      '#description' => t('Check this box to restrict selection to a single value.'),
    );
    $form['vbo']['display_result'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display processing result'),
      '#description' => t('Check this box to let Drupal display a message with the result of processing the selected items.'),
      '#default_value' => $this->options['vbo']['display_result'],
    );
    $form['vbo']['merge_single_action'] = array(
      '#type' => 'checkbox',
      '#title' => t('Merge single action\'s form with the row selection view'),
      '#description' => t('In case only one action is selected *and* this action is configurable, display its action form along with the row selection view.'),
      '#default_value' => $this->options['vbo']['merge_single_action'],
    );
    $form['vbo']['entity_load_capacity'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of entities to load at once'),
      '#description' => t("Improve execution performance at the cost of memory usage. Set to '1' if you're having problems."),
      '#default_value' => $this->options['vbo']['entity_load_capacity'],
    );
    // Display operations and their settings.
    $form['vbo']['operations'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Selected operations'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $entity_type = $this->get_entity_type();
    $options = $this->options['vbo']['operations'];
    foreach (views_bulk_operations_get_applicable_operations($entity_type, $options) as $operation_id => $operation) {
      $operation_options = isset($options[$operation_id]) ? $options[$operation_id] : array();

      $dom_id = 'edit-options-vbo-operations-' . str_replace(array('_', ':'), array('-', ''), $operation_id);
      $form['vbo']['operations'][$operation_id]['selected'] = array(
        '#type' => 'checkbox',
        '#title' => $operation->adminLabel(),
        '#default_value' => !empty($operation_options['selected']),
      );
      if (!$operation->aggregate()) {
        $form['vbo']['operations'][$operation_id]['use_queue'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enqueue the operation instead of executing it directly'),
          '#default_value' => !empty($operation_options['use_queue']),
          '#dependency' => array(
            $dom_id . '-selected' => array(1),
          ),
        );
      }
      $form['vbo']['operations'][$operation_id]['skip_confirmation'] = array(
        '#type' => 'checkbox',
        '#title' => t('Skip confirmation step'),
        '#default_value' => !empty($operation_options['skip_confirmation']),
        '#dependency' => array(
          $dom_id . '-selected' => array(1),
        ),
      );

      $form['vbo']['operations'][$operation_id] += $operation->adminOptionsForm($dom_id);
    }
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    $entity_type = $this->get_entity_type();
    foreach ($form_state['values']['options']['vbo']['operations'] as $operation_id => &$options) {
      if (empty($options['selected'])) {
        continue;
      }

      $operation = views_bulk_operations_get_operation($operation_id, $entity_type, $options);
      $fake_form = $form['vbo']['operations'][$operation_id];
      $fake_form_state = array('values' => &$options);
      $error_element_base = 'vbo][operations][' . $operation_id . '][';
      $operation->adminOptionsFormValidate($fake_form, $fake_form_state, $error_element_base);
    }
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);

    $entity_type = $this->get_entity_type();
    foreach ($form_state['values']['options']['vbo']['operations'] as $operation_id => &$options) {
      if (empty($options['selected'])) {
        continue;
      }

      $operation = views_bulk_operations_get_operation($operation_id, $entity_type, $options);
      $fake_form = $form['vbo']['operations'][$operation_id];
      $fake_form_state = array('values' => &$options);
      $operation->adminOptionsFormSubmit($fake_form, $fake_form_state);
    }
  }

  /**
   * If the view is using a table style, provide a
   * placeholder for a "select all" checkbox.
   */
  function label() {
    if ($this->view->style_plugin instanceof views_plugin_style_table && !$this->options['vbo']['force_single']) {
      return '<!--views-bulk-operations-select-all-->';
    }
    else {
      return parent::label();
    }
  }

  function render($values) {
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * The form which replaces the placeholder from render().
   */
  function views_form(&$form, &$form_state) {
    // The view is empty, abort.
    if (empty($this->view->result)) {
      return;
    }

    $form[$this->options['id']] = array(
      '#tree' => TRUE,
    );
    // At this point, the query has already been run, so we can access the results
    // in order to get the base key value (for example, nid for nodes).
    foreach ($this->view->result as $row_index => $row) {
      $entity_id = $this->get_value($row);

      if ($this->options['vbo']['force_single']) {
        $form[$this->options['id']][$row_index] = array(
          '#type' => 'radio',
          '#parents' => array($this->options['id']),
          '#return_value' => $entity_id,
        );
      }
      else {
        $form[$this->options['id']][$row_index] = array(
          '#type' => 'checkbox',
          '#return_value' => $entity_id,
          '#default_value' => FALSE,
          '#attributes' => array('class' => array('vbo-select')),
        );
      }
    }
  }

  public function get_selected_operations() {
    global $user;

    $entity_type = $this->get_entity_type();
    $selected = array();
    foreach ($this->options['vbo']['operations'] as $operation_id => $options) {
      if (empty($options['selected'])) {
        continue;
      }

      $operation = views_bulk_operations_get_operation($operation_id, $entity_type, $options);
      if (!$operation || !$operation->access($user)) {
        continue;
      }
      $selected[$operation_id] = $operation;
    }

    return $selected;
  }

  /**
   *  Returns the options stored for the provided operation id.
   */
  public function get_operation_options($operation_id) {
    $options = $this->options['vbo']['operations'];
    return isset($options[$operation_id]) ? $options[$operation_id] : array();
  }

  /**
   * Determine the base table of the VBO field, and then use it to determine
   * the entity type that VBO is operating on.
   */
  public function get_entity_type() {
    $base_table = $this->view->base_table;

    // If the current field is under a relationship you can't be sure that the
    // base table of the view is the base table of the current field.
    // For example a field from a node author on a node view does have users as base table.
    if (!empty($this->options['relationship']) && $this->options['relationship'] != 'none') {
      $relationships = $this->view->display_handler->get_option('relationships');
      $options = $relationships[$this->options['relationship']];
      $data = views_fetch_data($options['table']);
      $base_table = $data[$options['field']]['relationship']['base'];
    }
    // The base table is now known, use it to determine the entity type.
    foreach (entity_get_info() as $entity_type => $info) {
      if (isset($info['base table']) && $info['base table'] == $base_table) {
        return $entity_type;
      }
      elseif (isset($info['revision table']) && $info['revision table'] == $base_table) {
        $this->revision = TRUE;
        return $entity_type;
      }
    }
    // This should never happen.
    _views_bulk_operations_report_error("Could not determine the entity type for VBO field on views base table %table", array('%table' => $base_table));
    return FALSE;
  }
}
