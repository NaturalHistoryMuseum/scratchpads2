<?php

/**
 * Extending the view_plugin_style class to provide a slickgrid style.
 */
class slickgrid_views_plugin extends views_plugin_style{

  function init(&$view, &$display, $options = NULL){
    parent::init($view, $display, $options);
    if($options['pager']){
      $this->display->handler->default_display->options['pager']['options']['items_per_page'] = 0;
    }
  }

  /**
   * Set default options
   */
  function option_definition(){
    $options = parent::option_definition();
    // Grouping
    $options['grouping_field'] = array(
      'default' => NULL
    );
    $options['collapse_groups_by_default'] = array(
      'default' => FALSE
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
    // maps to slickgrid option forceFitColumns
    $options['forceFitColumns'] = array(
      'default' => false
    );
    $options['pager'] = array(
      'default' => false
    );
    // maps to slickgrid option headerHeight
    $options['headerHeight'] = array(
      'default' => 42
    );
    $options['viewport_height'] = array(
      'default' => 500
    );
    // maps to slickgrid option rowHeight
    $options['rowHeight'] = array(
      'default' => 30
    );
    // Editing
    $options['multi_edit'] = array(
      'default' => false
    );
    $options['delete'] = array(
      'default' => TRUE
    );
    $options['clone'] = array(
      'default' => false
    );
    $options['undo'] = array(
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
        // first, make sure the field still exists.
        if(!isset($sanitized[$field])){
          continue;
        }
        $column_name = key($column);
        // If the field is the column, mark it so, or the column
        // it's set to is a column, that's ok
        if($field == $column_name || $columns[$column_name] == $column && !empty($sanitized[$column_name])){
          $sanitized[$field] = $column;
        }
         // Since we set the field to itself initially, ignoring
      // the condition is ok; the field will get its column
      // status back.
      }
    }
    return $sanitized;
  }

  /**
   * Add settings for the particular slickgrid.
   */
  function options_form(&$form, &$form_state){
    parent::options_form($form, $form_state);
    $handlers = $this->display->handler->get_handlers('field');
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
    // Create an array of fields and form items for setting width, sortable & available plugins    
    foreach($columns as $field_id => $column){
      if($handlers[$field_id]->options['exclude']){
        continue;
      }
      // Field name
      $form['columns'][$field_id]['name'] = array(
        '#markup' => $field_labels[$field_id]
      );
      // Field for setting a column's width
      $form['columns'][$field_id]['width'] = array(
        '#type' => 'textfield',
        '#default_value' => ($this->options['columns'][$field_id]['width'] ? $this->options['columns'][$field_id]['width'] : 100),
        '#size' => 10,
        '#maxlength' => 10
      );
      // Can this field be sorted?
      if($handlers[$field_id]->click_sortable()){
        // Field for setting if a column is sortable
        $form['columns'][$field_id]['sortable'] = array(
          '#type' => 'checkbox',
          '#default_value' => ($this->options['columns'][$field_id]['sortable'] ? $this->options['columns'][$field_id]['sortable'] : false),
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
        if(count($plugin_options = slickgrid_get_plugin_options_for_field($plugin_type, $handlers[$field_id]->options['type']))){
          $form['columns'][$field_id][$plugin_type] = array(
            '#type' => 'select',
            '#default_value' => ($this->options['columns'][$field_id][$plugin_type] ? $this->options['columns'][$field_id][$plugin_type] : ''),
            '#options' => $plugin_options
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
      '#title' => t('A sync editor loading'),
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
    $form['settings']['pager'] = array(
      '#type' => 'checkbox',
      '#title' => t('Override paging'),
      '#default_value' => $this->options['pager'],
      '#description' => t('If selected, use slickgrid\'s in-built pagination instead of views\'.'),
      '#parents' => array(
        'style_options',
        'pager'
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
    $form['editing']['multi_edit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Multi-edit'),
      '#description' => t('Users can select & edit multiple items at once.'),
      '#default_value' => $this->options['multi_edit'],
      '#parents' => array(
        'style_options',
        'multi_edit'
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
      )
    );
    $form['editing']['undo'] = array(
      '#type' => 'checkbox',
      '#title' => t('Undo'),
      '#description' => t('Allow users to undo updates. Warning: if turned on, all updates will create a node revision.'),
      '#default_value' => $this->options['undo'],
      '#parents' => array(
        'style_options',
        'undo'
      )
    );
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
    static $editable_fields = array();
    if(!isset($editable_fields[$field_name])){
      $editable_fields[$field_name] = slickgrid_field_is_editable($field_name);
    }
    return $editable_fields[$field_name];
  }
}
?>