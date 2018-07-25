<?php

/**
 * Extending the view_plugin_style class to provide a slickgrid style.
 */
class slickgrid_views_plugin extends views_plugin_style{

  var $locked_fields = array();

  /**
   * Set default options
   */
  function option_definition(){
    $options = parent::option_definition();
    // Columns
    $options['columns'] = array(
      'default' => array()
    );
    // Grouping
    $options['grouping_field'] = array(
      'default' => NULL
    );
    $options['collapse_groups_by_default'] = array(
      'default' => FALSE
    );
    // Tabs
    $options['tabs'] = array(
      'default' => NULL
    );
    $options['default_active_tab'] = array(
      'default' => NULL
    );
    // Settings
    $options['collapsible_taxonomy_field'] = array(
      'default' => NULL
    );
    // maps to slickgrid option enableColumnResize
    $options['enableColumnResize'] = array(
      'default' => TRUE
    );
    // maps to slickgrid option enableColumnReorder
    $options['enableColumnReorder'] = array(
      'default' => TRUE
    );
    $options['select_columns'] = array(
      'default' => TRUE
    );
    // maps to slickgrid option asyncEditorLoading
    $options['asyncEditorLoading'] = array(
      'default' => TRUE
    );
    // maps to slickgrid option syncColumnCellResize
    $options['syncColumnCellResize'] = array(
      'default' => TRUE
    );
    // maps to slickgrid option forceFitColumns
    $options['forceFitColumns'] = array(
      'default' => false
    );
    // maps to slickgrid option headerHeight
    $options['headerHeight'] = array(
      'default' => 42
    );
    $options['viewport_height'] = array(
      'default' => 500
    );
    $options['height_expand'] = array(
      'default' => TRUE
    );
    // maps to slickgrid option rowHeight
    $options['rowHeight'] = array(
      'default' => 30
    );
    // Editing
    $options['row_selection_checkbox'] = array(
      'default' => false
    );
    // If row selection checkbox is enabled, the following values are available    
    $options['multi_edit'] = array(
      'default' => false
    );
    $options['delete'] = array(
      'default' => false
    );
    $options['clone'] = array(
      'default' => false
    );
    $options['export_selected_rows'] = array(
      'default' => false
    );
    // Undo editing
    $options['undo'] = array(
      'default' => false
    );
    // Node add button
    $options['add'] = array(
      'default' => false
    );
    // maps to slickgrid option autoEdit
    $options['autoEdit'] = array(
      'default' => FALSE
    );
    return $options;
  }

  /**
   * Normalize a list of columns based upon the fields that are
   * available. This compares the fields stored in the style handler
   * to the list of fields actually in the view, removing fields that
   * have been removed and adding new fields in their own column.
   *
   * - Each field must be in a column.
   * - Each column must be based upon a field, and that field
   * is somewhere in the column.
   * - Any fields not currently represented must be added.
   * - Columns must be re-ordered to match the fields.
   *
   * @param $columns
   * An array of all fields; the key is the id of the field and the
   * value is the id of the column the field should be in.
   * @param $fields
   * The fields to use for the columns. If not provided, they will
   * be requested from the current display. The running render should
   * send the fields through, as they may be different than what the
   * display has listed due to access control or other changes.
   */
  function sanitize_columns($columns, $fields = NULL){
    $sanitized = array();
    if($fields === NULL){
      $fields = $this->display->handler->get_option('fields');
    }
    // Preconfigure the sanitized array so that the order is retained.
    foreach($fields as $field => $info){
      // Set to itself so that if it isn't touched, it gets column
      // status automatically.
      $sanitized[$field] = $field;
    }
    if(is_array($columns)){
      foreach($columns as $field => $column){
        // Make sure the field still exists.
        if(isset($sanitized[$field])){
          $sanitized[$field] = $column;
        }
      }
    }
    return $sanitized;
  }

  /**
   * Add settings for the particular slickgrid.
   */
  function options_form(&$form, &$form_state){
    parent::options_form($form, $form_state);
    $form['#attached']['js'] = array(
      drupal_get_path('module', 'slickgrid') . '/js/slickgrid.admin.js'
    );
    $handlers = $this->display->handler->get_handlers('field');
    if(!isset($this->options['columns'])){
      $this->options['columns'] = array();
    }
    $columns = $this->sanitize_columns($this->options['columns']);
    $field_labels = $this->display->handler->get_field_labels();
    if(empty($columns)){
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your slickgrid settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>'
      );
      return;
    }
    $tabs = array(
      '' => '<none>'
    );
    if(isset($this->options['tabs'])){
      foreach(explode(PHP_EOL, $this->options['tabs']) as $tab){
        $tab = trim($tab);
        $tabs[$tab] = $tab;
      }
    }
    // Create an array of fields and form items for setting width, sortable & available plugins    
    foreach($columns as $field_id => $column){
      if($handlers[$field_id]->options['exclude']){
        continue;
      }
      // Field name
      $form['columns'][$field_id]['name'] = array(
        '#markup' => $field_labels[$field_id]
      );
      // Tabs
      $form['columns'][$field_id]['tab'] = array(
        '#type' => 'select',
        '#default_value' => (isset($this->options['columns'][$field_id]['tab']) ? $this->options['columns'][$field_id]['tab'] : ''),
        '#options' => $tabs
      );
      // Field for setting a column's width
      $form['columns'][$field_id]['width'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($this->options['columns'][$field_id]['width']) ? $this->options['columns'][$field_id]['width'] : 100),
        '#size' => 10,
        '#maxlength' => 10
      );
      // Can this field be sorted?
      if($handlers[$field_id]->click_sortable()){
        // Field for setting if a column is sortable
        $form['columns'][$field_id]['sortable'] = array(
          '#type' => 'checkbox',
          '#default_value' => (isset($this->options['columns'][$field_id]['sortable']) ? $this->options['columns'][$field_id]['sortable'] : false),
          '#size' => 10
        );
      }
      foreach(slickgrid_get_plugin_types() as $plugin_type => $plugin_label){
        // Only allow editing (and therefore validation) if this is an editable field
        if(($plugin_type == 'editor' || $plugin_type == 'validator') & !$this->field_is_editable($field_id)){
          // Field locked so do not try and find an editor
          // Add HTML warning & continue() to next to plugin type
          $form['columns'][$field_id][$plugin_type] = array(
            '#markup' => t('This field cannot be edited.')
          );
          continue;
        }
        // If this is an entity title field, use text_default as field type
        if(!isset($handlers[$field_id]->options['type'])){
          if($this->entity_label_field($handlers[$field_id])){
            $handlers[$field_id]->options['type'] = 'text_default';
          }
        }
        if(count($plugin_options = slickgrid_get_plugin_options_for_field($plugin_type, (isset($handlers[$field_id]->options['type']) ? $handlers[$field_id]->options['type'] : NULL)))){
          $form['columns'][$field_id][$plugin_type] = array(
            '#type' => 'select',
            '#default_value' => (isset($this->options['columns'][$field_id][$plugin_type]) ? $this->options['columns'][$field_id][$plugin_type] : ''),
            '#options' => $plugin_options,
            '#attributes' => array(
              'class' => array(
                $plugin_type
              )
            )
          );
        }else{
          $form['columns'][$field_id][$plugin_type] = array(
            '#markup' => t('No %plugin_label plugins for this field.', array(
              '%plugin_label' => strtolower($plugin_label)
            ))
          );
        }
      }
    }
    // Grouping field
    if($this->uses_fields()){
      $options = array(
        '' => t('<None>')
      );
      $options += $this->display->handler->get_field_labels();
      // If there are no fields, we can't group on them.
      if(count($options) > 1){
        $form['grouping'] = array(
          '#type' => 'fieldset',
          '#title' => t('Grouping')
        );
        $form['grouping']['grouping_field'] = array(
          '#type' => 'select',
          '#title' => t('Grouping field'),
          '#options' => $options,
          '#default_value' => $this->options['grouping_field'],
          '#description' => t('You may optionally specify a field by which to group the records. Leave blank to not group.'),
          '#parents' => array(
            'style_options',
            'grouping_field'
          )
        );
        $form['grouping']['collapse_groups_by_default'] = array(
          '#type' => 'checkbox',
          '#title' => t('Collapse groups by default'),
          '#description' => t('All groups should be collapsed by default.'),
          '#default_value' => $this->options['collapse_groups_by_default'],
          '#states' => array(
            'invisible' => array(
              '#edit-style-options-grouping-field' => array(
                'value' => ''
              )
            )
          ),
          '#parents' => array(
            'style_options',
            'collapse_groups_by_default'
          )
        );
      }
    }
    // Tabs
    $form['tab_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Tabs')
    );
    $form['tab_settings']['tabs'] = array(
      '#type' => 'textarea',
      '#title' => t('Tab names'),
      '#default_value' => (isset($this->options['tabs']) ? $this->options['tabs'] : ''),
      '#parents' => array(
        'style_options',
        'tabs'
      )
    );
    $form['tab_settings']['default_active_tab'] = array(
      '#type' => 'select',
      '#title' => t('Default active tab'),
      '#options' => $tabs,
      '#default_value' => (isset($this->options['default_active_tab']) ? $this->options['default_active_tab'] : ''),
      '#parents' => array(
        'style_options',
        'default_active_tab'
      )
    );
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings')
    );
    $form['settings']['enableColumnResize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable resizing columns'),
      '#default_value' => $this->options['enableColumnResize'],
      '#parents' => array(
        'style_options',
        'enableColumnResize'
      )
    );
    $form['settings']['enableColumnReorder'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable reordering columns'),
      '#default_value' => $this->options['enableColumnReorder'],
      '#parents' => array(
        'style_options',
        'enableColumnReorder'
      )
    );
    $form['settings']['height_expand'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable automatic resizing of the grid to fill available vertical space'),
      '#default_value' => $this->options['height_expand'],
      '#parents' => array(
        'style_options',
        'height_expand'
      )
    );
    $form['settings']['select_columns'] = array(
      '#type' => 'checkbox',
      '#title' => t('Column visibility'),
      '#description' => t('Allow users to show &amp; hide columns'),
      '#default_value' => $this->options['select_columns'],
      '#parents' => array(
        'style_options',
        'select_columns'
      )
    );
    $form['settings']['asyncEditorLoading'] = array(
      '#type' => 'checkbox',
      '#title' => t('Async editor loading'),
      '#default_value' => $this->options['asyncEditorLoading'],
      '#parents' => array(
        'style_options',
        'asyncEditorLoading'
      )
    );
    $form['settings']['forceFitColumns'] = array(
      '#type' => 'checkbox',
      '#title' => t('Force fit columns'),
      '#default_value' => $this->options['forceFitColumns'],
      '#description' => t('Force column widths to fit the grid.'),
      '#parents' => array(
        'style_options',
        'forceFitColumns'
      )
    );
    $form['settings']['headerHeight'] = array(
      '#title' => t('Header height'),
      '#type' => 'textfield',
      '#default_value' => $this->options['headerHeight'],
      '#size' => 10,
      '#maxlength' => 10,
      '#parents' => array(
        'style_options',
        'headerHeight'
      )
    );
    $form['settings']['viewport_height'] = array(
      '#title' => t('Viewport height'),
      '#type' => 'textfield',
      '#default_value' => $this->options['viewport_height'],
      '#size' => 10,
      '#maxlength' => 10,
      '#parents' => array(
        'style_options',
        'viewport_height'
      )
    );
    $form['settings']['rowHeight'] = array(
      '#title' => t('Row height'),
      '#type' => 'textfield',
      '#default_value' => $this->options['rowHeight'],
      '#size' => 10,
      '#maxlength' => 10,
      '#parents' => array(
        'style_options',
        'rowHeight'
      )
    );
    $form['editing'] = array(
      '#type' => 'fieldset',
      '#title' => t('Editing')
    );
    foreach($columns as $field_id => $column){
      $form['editing']['#states']['invisible']['#edit-style-options-columns-' . drupal_html_id($field_id) . '-editors'] = array(
        'value' => ''
      );
    }
    $form['editing']['row_selection_checkbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Row selection checkbox'),
      '#description' => t('Add a row selection checkbox.'),
      '#default_value' => $this->options['row_selection_checkbox'],
      '#parents' => array(
        'style_options',
        'row_selection_checkbox'
      )
    );
    $form['editing']['multi_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Multi-edit'),
      '#description' => t('Users can select & edit multiple items at once.'),
      '#default_value' => $this->options['multi_edit'],
      '#parents' => array(
        'style_options',
        'multi_edit'
      ),
      '#prefix' => '<div style="padding-left: 30px">',
      '#states' => array(
        'invisible' => array(
          'input[name="style_options[row_selection_checkbox]"]' => array(
            'checked' => FALSE
          )
        )
      )
    );
    $form['editing']['delete'] = array(
      '#title' => t('Delete items'),
      '#description' => t('Allow users to delete items from within the slickgrid.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['delete'],
      '#parents' => array(
        'style_options',
        'delete'
      ),
      '#states' => array(
        'invisible' => array(
          'input[name="style_options[row_selection_checkbox]"]' => array(
            'checked' => FALSE
          )
        )
      )
    );
    $form['editing']['clone'] = array(
      '#title' => t('Clone items'),
      '#description' => t('Allow users to clone items from within the slickgrid.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['clone'],
      '#parents' => array(
        'style_options',
        'clone'
      ),
      '#states' => array(
        'invisible' => array(
          'input[name="style_options[row_selection_checkbox]"]' => array(
            'checked' => FALSE
          )
        )
      )
    );
    $form['editing']['export_selected_rows'] = array(
      '#title' => t('Export selected rows'),
      '#description' => t('Allow users to select rows to export (requires views data export).'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['export_selected_rows'],
      '#parents' => array(
        'style_options',
        'export_selected_rows'
      ),
      '#suffix' => '</div>',
      '#states' => array(
        'invisible' => array(
          'input[name="style_options[row_selection_checkbox]"]' => array(
            'checked' => FALSE
          )
        )
      )
    );
    $form['editing']['Add'] = array(
      '#type' => 'select',
      '#title' => t('Add'),
      '#options' => array_merge(array(
        '<none>'
      ), slickgrid_list_addable_entities()),
      '#description' => t('Allow users to add an entity from within slickgrid.'),
      '#default_value' => $this->options['add'],
      '#multiple' => true,
      '#parents' => array(
        'style_options',
        'add'
      )
    );
    $form['editing']['undo'] = array(
      '#type' => 'checkbox',
      '#title' => t('Undo'),
      '#default_value' => $this->options['undo'],
      '#parents' => array(
        'style_options',
        'undo'
      )
    );
    // Only allow undo for nodes
    if($this->view->base_table == 'node'){
      $form['editing']['undo']['#description'] = t('Allow users to undo updates. Warning: if turned on, all updates will create a node revision.');
    }else{
      $form['editing']['undo']['#disabled'] = true;
      $form['editing']['undo']['#description'] = t('Versioning is required for undo so this feature is only available for nodes.');
      $form['editing']['undo']['#value'] = false;
    }
    $form['editing']['autoEdit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto edit'),
      '#default_value' => $this->options['autoEdit'],
      '#description' => t('Activate edit on entry to cell, otherwise double click to edit.'),
      '#parents' => array(
        'style_options',
        'autoEdit'
      )
    );
    $form['#theme'] = 'slickgrid_views_plugin_table';
  }

  function field_is_editable($field_name){
    if(!count($this->locked_fields)){
      $this->locked_fields = slickgrid_get_entity_keys();
    }
    // Field is only editable if it's not a locked field (an entity key field)
    return !in_array($field_name, $this->locked_fields);
  }

  function entity_label_field($field){
    static $entity_labels = array();
    if(!count($entity_labels)){
      foreach($entities_info = entity_get_info() as $entity_info){
        $entity_labels[$entity_info['base table']] = $entity_info['entity keys']['label'];
      }
    }
    if(array_key_exists($field->options['table'], $entity_labels) && $entity_labels[$field->options['table']] == $field->options['field']){return true;}
  }

  function query(){
    // We always want the query to be distinct - this also adds the correct base field
    $this->view->query->set_distinct();
  }
}

/**
 * Helper function for the above class.  Returns an array of entity keys which
 * must not be edited.
 */
function slickgrid_get_entity_keys($entity_type = null, $keys_to_ignore = array('label')){
  $entity_keys = array();
  if($entity_type){
    $entities_info = array(
      entity_get_info($entity_type)
    );
  }else{
    $entities_info = entity_get_info();
  }
  foreach($entities_info as $entity_info){
    foreach($keys_to_ignore as $key_to_ignore){
      unset($entity_info['entity keys'][$key_to_ignore]);
    }
    $entity_keys += array_values($entity_info['entity keys']);
  }
  return $entity_keys;
}