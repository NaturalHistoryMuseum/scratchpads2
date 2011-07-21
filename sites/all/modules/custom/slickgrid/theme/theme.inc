<?php

function slickgrid_preprocess_views_view_slickgrid(&$variables){
  global $user;
  $path = drupal_get_path('module', 'slickgrid');
  $view = & $variables['view'];
  // Get any attached export links
  foreach($view->display as $id => $display){
    if($display->display_plugin == 'views_data_export' && isset($display->display_options['displays']) && in_array($view->current_display, $display->display_options['displays'])){
      $view->style_plugin->options['export'] = true;
    }
  }
  // Get any bespoke user settings
  if(is_array($user_settings = slickgrid_get_settings(array(
    'uid' => $user->uid,
    'view_name' => $view->name,
    'display_id' => $view->current_display
  )))){
    $view->style_plugin->options = array_merge($view->style_plugin->options, $user_settings);
  }
  // Set the entity type
  $view->style_plugin->options['entity_type'] = $view->base_table;
  $handlers = $view->style_plugin->display->handler->get_handlers('field');
  // arrays to store ordered / unordered columns in
  $ordered_columns = array();
  $unordered_columns = array();
  $added_plugins = array(); // Keep a record of plugins added to speed things up a bit
  $tabs = array();
  // Build the slickgrid columns  
  foreach($handlers as $field_id => $field_handler){
    // If this field is ecluded from the display, continue to the next one
    if($field_handler->options['exclude']){
      continue;
    }
    // Get the column width
    if(isset($view->style_plugin->options['column_width'][$field_id])){
      $column_width = $view->style_plugin->options['column_width'][$field_id];
    }elseif(isset($view->style_plugin->options['columns'][$field_id]['width'])){
      $column_width = $view->style_plugin->options['columns'][$field_id]['width'];
    }else{
      $column_width = 100;
    }
    $column = array(
      'id' => $field_id, // The ID of the field from the view - NO DUPLICATES ALLOWED
      'name' => $field_handler->options['label'], // Column title / label
      'field' => $field_id, // The ID of the field from the view - used to populate data so NO DUPLICATES ALLOWED (although it won't break anything),
      'width' => $column_width,
      'cssClass' => 'cell-title',
      'resizable' => ($view->style_plugin->options['enableColumnResize'] ? 1 : 0)
    );
    if(isset($field_handler->definition['field_name'])){
      $column['fieldName'] = $field_handler->definition['field_name'];
    }else{
      $column['fieldName'] = $field_handler->real_field;
    }
    // Loop through all the plugin types and see if it's been set for this column
    foreach(array_keys(slickgrid_get_plugin_types()) as $plugin_type){
      // Is there an plugin defined for this column
      if(isset($view->style_plugin->options['columns'][$field_id][$plugin_type]) & !empty($view->style_plugin->options['columns'][$field_id][$plugin_type])){
        // Set the plugin type for the column definition
        $plugin = ctools_get_plugins('slickgrid', $plugin_type, $view->style_plugin->options['columns'][$field_id][$plugin_type]);
        if(isset($plugin['js']['class'])){
          $column[$plugin_type] = $plugin['js']['class'];
        }else{
          $column[$plugin_type] = $view->style_plugin->options['columns'][$field_id][$plugin_type];
        }
        // Load the plugin & add any associated js / css files
        if(!in_array($view->style_plugin->options['columns'][$field_id][$plugin_type], $added_plugins)){
          foreach(array(
            'js',
            'css'
          ) as $file_type){
            if(isset($plugin[$file_type]) && $plugin[$file_type]['file']){
              $func = 'drupal_add_' . $file_type;
              $func($plugin['path'] . '/' . $plugin[$file_type]['file']);
            }
          }
          // Add this plugin to the the added_plugins array so it is not added again
          $added_plugins[] = $view->style_plugin->options['columns'][$field_id][$plugin_type];
        }
        // Add extra options for particular plugin types
        switch($plugin_type){
          case 'editor':
            // If just one column is editable, turn on editable & cell navigation for the grid
            $view->style_plugin->options['editable'] = TRUE;
            $view->style_plugin->options['enableCellNavigation'] = TRUE;
            break;
          case 'filter':
            $view->style_plugin->options['filterable'] = TRUE;
            // DOn't show header row if this is a collapsibleFilter plugin - that happens within the data
            if($view->style_plugin->options['columns'][$field_id][$plugin_type] != 'collapsibleFilter'){
              $view->style_plugin->options['showHeaderRow'] = TRUE;
            }
            break;
        }
      }
    }
    // Is this field sortable?
    // If any fields are sortable, set style option so we can access it in the js
    if(isset($view->style_plugin->options['columns'][$field_id]['sortable']) && $view->style_plugin->options['columns'][$field_id]['sortable']){
      $view->style_plugin->options['sortable_columns'] = TRUE;
      $column['sortable'] = 1;
    }
    // We need to know the column label for the group by function in slickgrid.js
    // Add it to the views plugin options so we can access it efficiantly (otherwise I'll need to loop thru columns array)
    $view->style_plugin->options['columns'][$field_id]['label'] = $field_handler->options['label'];
    // If this is an order column, add it to the ordered columns array in the approprioate place
    if(isset($view->style_plugin->options['ordered_columns']) && is_numeric($ordered_column_position = array_search($field_id, $view->style_plugin->options['ordered_columns']))){ // This is an ordered column
      if(!empty($view->style_plugin->options['columns'][$field_id]['tab'])){
        $column['tab'] = $view->style_plugin->options['columns'][$field_id]['tab'];
        $tabs[$view->style_plugin->options['columns'][$field_id]['tab']]['ordered_columns'][$ordered_column_position] = $column;
      }else{
        $ordered_columns[$ordered_column_position] = $column;
      }
    }else{ // There is no ordering for this column, another column might have been added to the view
      if(!empty($view->style_plugin->options['columns'][$field_id]['tab'])){
        $column['tab'] = $view->style_plugin->options['columns'][$field_id]['tab'];
        $tabs[$view->style_plugin->options['columns'][$field_id]['tab']]['unordered_columns'][] = $column;
      }else{
        $unordered_columns[] = $column;
      }
    }
  }
  // sort the ordered columns based on key (the ordered column position)
  ksort($ordered_columns);
  // Merge ordered & unordered columns 
  // Any unordered columns are added to the end to allow for new columns added through the view
  $view->columns = array_merge($ordered_columns, $unordered_columns);
  if(count($tabs)){
    $tab_offset = 0;
    // All tabbed columns are added after un-tabbed columns
    $view->style_plugin->options['tabs'] = TRUE;
    drupal_add_css($path . '/css/slickgrid.controls.css');
    drupal_add_js($path . '/js/controls/slickgrid.tabs.js');
    foreach($tabs as $tab_name => $tab){
      if(isset($tab['ordered_columns'])){
        ksort($tab['ordered_columns']);
      }else{
        $tab['ordered_columns'] = array();
      }
      if(!isset($tab['unordered_columns'])){
        $tab['unordered_columns'] = array();
      }
      $view->columns = array_merge($view->columns, $tab['ordered_columns'], $tab['unordered_columns']);
    }
    $variables['tabs'] = theme('slickgrid_tabs', array(
      'options' => $view->style_plugin->options,
      'columns' => $view->columns,
      'view' => $view
    ));
  }
  $parents = array(); // array of tid => array(nid => nid, indent => indent) so we can retrive the nid & indentation of a parent node
  $view->data = array(); // The array of data to be used by slickgrid
  // Loop through all the rows, constructing the data set
  if(is_array($variables['rows'])){
    // Keep a record of all ids & trigger an error on duplication	
    $ids = array();
    foreach($variables['rows'] as $count => $row){
      $view->row_index = $count;
      $item = array();
      if(!isset($row->{$view->base_field})){
        watchdog('slickgrid', t('@base_field is missing.'), array(
          '@base_field' => $view->base_field
        ), WATCHDOG_ERROR);
        continue;
      }
      // Has this already been added to the data grid?
      if(in_array($row->{$view->base_field}, $ids)){
        drupal_set_message(t('There are duplicate @base_fields in this grid - each row must be unique.', array(
          '@base_field' => $view->base_field
        )), 'error');
        watchdog('slickgrid', t('Duplicate @base_fields - @id is not unique.'), array(
          '@base_field' => $view->base_field,
          '@id' => $row->{$view->base_field}
        ), WATCHDOG_ERROR);
        break;
      }else{
        // Add the data fields
        foreach($view->columns as $column){
          if(!($view->field[$column['id']]->options['exclude'])){
            $item[$column['id']] = $view->render_field($column['id'], $count);
            // If the value is null, make it an empty string ""
            if(is_null($item[$column['id']])){
              $item[$column['id']] = '';
            }
          }
        }
        $item['id'] = $row->{$view->base_field};
        $ids[] = $row->{$view->base_field};
        // If this has collapsible_taxonomy_field set, we will have added slickgrid_parent_tid & slickgrid_tid in hook_views_pre_view
        // These are aliased in $row so get the field aliases
        if(isset($view->field['slickgrid_tid']) && property_exists($row, $view->field['slickgrid_tid']->field_alias) && $slickgrid_tid = $row->{$view->field['slickgrid_tid']->field_alias}){
          if($parent = & $parents[$row->{$view->field['slickgrid_parent_tid']->field_alias}]){
            // If we can find a parent, set the parent nid & indentation++
            $item['indent'] = $parent['indent'] + 1;
            $item['parent'] = $parent['nid'];
            $key = ++$parent['key'];
          }else{
            // Otherwise set indent & parent to 0 - this breaks if left blank
            $item['indent'] = 0;
            $item['parent'] = 0;
            $key = $count;
          }
          // Store the indent & nid so then can be used to calculate child term indentation 
          $parents[$slickgrid_tid] = array(
            'nid' => $row->nid,
            'indent' => $view->data[$count]['indent'],
            'key' => $key
          );
        }else{
          $key = $count;
        }
        // If the key already exists, we need to add the item to the middle of the array
        if(array_key_exists($key, $view->data)){
          array_splice($view->data, $key, 0, $key);
          $view->data[$key] = $item;
        }else{
          $view->data[$key] = $item;
        }
      }
    }
  }
  if($errors = drupal_get_messages('error', false)){
    $variables['slickgrid'] = theme('status_messages', array(
      'display' => 'error'
    ));
  }else{
    // Create the slickgrid - an instance of a slickgrid theme
    $variables['slickgrid'] = theme('slickgrid', array(
      'view' => $view
    ));
    $variables['controls'] = theme('slickgrid_controls', array(
      'view' => $view
    ));
  }
  $variables['class'] = 'views-view-grid cols-' . count($view->columns);
}

/**
 * The main theme function for the slickgrid
 * @param array $view_name
 */
function theme_slickgrid($variables){
  $view = $variables['view'];
  // Add all the CSS & JS
  // Add the core slickgrid library
  drupal_add_library('slickgrid', 'slickgrid');
  // Add this modules slickgrid files
  $path = drupal_get_path('module', 'slickgrid');
  // Add bespoke slickgrid js/css
  drupal_add_js($path . '/js/slickgrid.js');
  drupal_add_js($path . '/js/slickgrid.theme.js');
  drupal_add_css($path . '/css/slickgrid.css');
  // Add beautytips
  beautytips_add_beautytips();
  // If this slickgrid is editable, add the ctools modal plugins
  if(isset($view->style_plugin->options['editable']) || isset($view->style_plugin->options['add'])){
    slickgrid_add_modal();
  }
  // Inline JS defining the slickgrid
  $js = array();
  $js[] = 'var options = ' . drupal_json_encode($view->style_plugin->options) . ';';
  $js[] = 'var data = [];';
  if(count($view->data)){
    $js[] = 'data = ' . drupal_json_encode($view->data) . ';';
  }
  $js[] = 'var columns = ' . slickgrid_encode_columns($view->columns) . ';';
  $js[] = 'var slickgrid;';
  $js[] = '(function($) {';
  $js[] = '$(document).ready(function(){';
  $js[] = 'slickgrid = new Slickgrid("#slickgrid", "' . $view->name . '", "' . $view->current_display . '", "' . url(SLICKGRID_CALLBACK_PATH) . '");';
  $js[] = '});';
  $js[] = '})(jQuery);';
  drupal_add_js(implode("\n", $js), array(
    'type' => 'inline'
  ));
  $output = '<div id="slickgrid" style="width:100%;height:' . $view->style_plugin->options['viewport_height'] . 'px;" class="hideCols hideRows' . (isset($view->style_plugin->options['editable']) && $view->style_plugin->options['editable'] ? ' editable' : '') . '"></div>';
  return $output;
}

/**
 * Theme function for the slickgrid tabs
 * @param array $options
 * @param array $columns
 * @param array $view_name
 */
function theme_slickgrid_tabs($variables){
  $path = drupal_get_path('module', 'slickgrid');
  drupal_add_css($path . '/css/slickgrid.controls.css');
  drupal_add_js($path . '/js/controls/slickgrid.tabs.js');
  $output = '<div id="slickgrid-tabs"></div>';
  return $output;
}

/**
 * Theme function for the slickgrid control interface
 * @param array $options
 * @param array $columns
 * @param array $view_name
 */
function theme_slickgrid_controls($variables){
  $view = $variables['view'];
  $path = drupal_get_path('module', 'slickgrid');
  drupal_add_css($path . '/css/slickgrid.controls.css');
  $library_path = libraries_get_path('slickgrid');
  $output = '';
  // Add controls defined by the slickgrid plugin (pager)
  if(isset($view->style_plugin->options['pager']) && $view->style_plugin->options['pager']){
    drupal_add_js($library_path . '/controls/slick.pager.js');
    drupal_add_css($library_path . '/controls/slick.pager.css');
    $output .= '<div class="slickgrid-control" id="slickgrid-pager"></div>';
  }
  if(isset($view->style_plugin->options['grouping_field']) && !empty($view->style_plugin->options['grouping_field'])){
    drupal_add_js($library_path . '/slick.groupitemmetadataprovider.js');
  }
  $output .= '<div id="slickgrid-status"></div>';
  if($view->style_plugin->options['add']){
    $output .= '<div class="slickgrid-control" id="slickgrid-add">' . ctools_modal_text_button('<span class="slickgrid-control-button enabled"></span>', SLICKGRID_CALLBACK_PATH . '/add/' . $view->style_plugin->options['add'] . '/' . $variables['view']->name . '/' . $variables['view']->current_display, t('Add'), 'ctools-modal-ctools-modal-slickgrid-scale') . '</div>';
  }
  // Add controls defined by our slickgrid module (undo, export, delete)
  if($view->style_plugin->options['undo']){
    drupal_add_js($path . '/js/controls/slickgrid.undo.js');
    $output .= '<div class="slickgrid-control" id="slickgrid-undo"></div>';
  }
  if($view->style_plugin->options['delete']){
    drupal_add_js($path . '/js/controls/slickgrid.delete.js');
    $output .= '<div class="slickgrid-control" id="slickgrid-delete"></div>';
  }
  if(isset($view->style_plugin->options['export'])){
    drupal_add_js($path . '/js/controls/slickgrid.export.js');
    $output .= '<div class="slickgrid-control" id="slickgrid-export"></div>';
  }
  if(isset($view->style_plugin->options['clone'])){
    drupal_add_js($path . '/js/controls/slickgrid.clone.js');
    $output .= '<div class="slickgrid-control" id="slickgrid-clone"></div>';
  }
  return $output;
}

/**
 * Theme the form for the matrix style plugin
 */
function theme_slickgrid_views_plugin_table($variables){
  $form = $variables['form'];
  $output = drupal_render($form['description_markup']);
  $variables['header'] = array(
    t('Field'),
    t('Tab'),
    t('Width'),
    t('Sortable')
  );
  $variables['rows'] = array();
  $plugin_types = slickgrid_get_plugin_types();
  $variables['header'] += $plugin_types;
  foreach(element_children($form['columns']) as $id){
    $row = array();
    $row[] = drupal_render($form['columns'][$id]['name']);
    $row[] = drupal_render($form['columns'][$id]['tab']);
    $row[] = drupal_render($form['columns'][$id]['width']);
    $row[] = drupal_render($form['columns'][$id]['sortable']);
    foreach(array_keys($plugin_types) as $plugin_type){
      $row[] = drupal_render($form['columns'][$id][$plugin_type]);
    }
    $variables['rows'][] = $row;
  }
  $output .= theme('table', $variables);
  $output .= drupal_render_children($form);
  return $output;
}