<?php
function slickgrid_preprocess_views_view_slickgrid(&$variables){
  global $user;
  $view = $variables['view'];
  // Get any bespoke user settings
  if(is_array($user_settings = slickgrid_get_settings($user->uid, $view->name))){
    $view->style_plugin->options = array_merge($view->style_plugin->options, $user_settings);
  }
  // Set the entity type
  $view->style_plugin->options['entity_type'] = $view->base_table;
  
  // TODO - double check this works when displaying a view which isn't using the defaults
  $handlers = $view->style_plugin->display->handler->get_handlers('field');
  // arrays to store ordered / unordered columns in
  $ordered_columns = array();
  $unordered_columns = array();
  $added_plugins = array(); // Keep a record of plugins added to speed things up a bit
  // Build the slickgrid columns  
  foreach($handlers as $field_id => $field_handler){
    // If this field is ecluded from the display, continue to the next one
    if($field_handler->options['exclude']){
      continue;
    }
    // Get the column width
    $column_width = 100;
    $column = array(
      'id' => $field_id, // The ID of the field from the view - NO DUPLICATES ALLOWED
      'name' => $field_handler->options['label'], // Column title / label
      'field' => $field_handler->real_field, // The actual field name - DUPLICATE NAMES ARE ALLOWED
      'width' => $column_width,
      'cssClass' => 'cell-title',
      'resizable' => ($view->style_plugin->options['enableColumnResize'] ? 1 : 0),
    );
    // If filters have been set for this column, set the option
    if(isset($view->style_plugin->options['columns'][$field_id]['filter'])){
      $view->style_plugin->options['has_filters'] = TRUE;
      $view->style_plugin->options['showHeaderRow'] = TRUE;
    }
    // Loop through all the plugin types and see if it's been set for this column
    foreach(array_keys(slickgrid_get_plugin_types()) as $plugin_type){
      // Is there an plugin defined for this column
      if(isset($view->style_plugin->options['columns'][$field_id][$plugin_type]) & !empty($view->style_plugin->options['columns'][$field_id][$plugin_type])){
        // Set the plugin type for the column definition
        $column[$plugin_type] = $view->style_plugin->options['columns'][$field_id][$plugin_type];
        // Load the plugin & add any associated js / css files
        if(!in_array($view->style_plugin->options['columns'][$field_id][$plugin_type], $added_plugins)){
          $plugin = ctools_get_plugins('slickgrid', $plugin_type, $view->style_plugin->options['columns'][$field_id][$plugin_type]);
          foreach(array(
            'js',
            'css'
          ) as $file_type){
            if(array_key_exists($file_type, $plugin)){
              $func = 'drupal_add_' . $file_type;
              $func($plugin['path'] . '/' . $plugin[$file_type]);
            }
          }
          // Add this plugin to the the added_plugins array so it is not added again
          $added_plugins[] = $view->style_plugin->options['columns'][$field_id][$plugin_type];
        }
        // If just one column is editable, turn on editable & cell navigation for the grid
        if($plugin_type == 'editor'){
          $view->style_plugin->options['editable'] = TRUE;
          $view->style_plugin->options['enableCellNavigation'] = TRUE;
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
      $ordered_columns[$ordered_column_position] = $column;
    }else{ // There is no ordering for this column, another column might have been added to the view
      $unordered_columns[] = $column;
    }
  }
  // sort the ordered columns based on key (the ordered column position)
  ksort($ordered_columns);
  // Merge ordered & unordered columns 
  // Any unordered columns are added to the end to allow for new columns added through the view
  $columns = array_merge($ordered_columns, $unordered_columns);
  // Allow other modules to change the columns before they're output as a slickgrid
  // TODO - Are any modules requiring drupal alter?
  //  drupal_alter('slickgrid', $data, 'columns', $view);
  $parents = array(); // array of tid => array(nid => nid, indent => indent) so we can retrive the nid & indentation of a parent node
  $data = array(); // The array of data to be used by slickgrid
  // Loop through all the rows, constructing the data set
  if(is_array($variables['rows'])){
    foreach($variables['rows'] as $count => $row){
      // Add the data fields
      foreach($columns as $column){
        if(!($view->field[$column['field']]->options['exclude'])){
          $data[$count][$column['field']] = $view->field[$column['id']]->theme($row);
        }
      }
      $data[$count]['id'] = $row->nid;
    }
  }
  // Create the slickgrid - an instance of a slickgrid theme
  $variables['slickgrid'] = theme('slickgrid', array(
    'options' => $view->style_plugin->options,
    'columns' => $columns,
    'data' => $data,
    'view' => $view
  ));
  // Create the slickgrid controls
  $variables['slickgrid_controls'] = theme('slickgrid_controls', array(
    'options' => $view->style_plugin->options,
    'columns' => $columns,
    'data' => $data,
    'view' => $view
  ));
  $variables['class'] = 'views-view-grid cols-' . count($columns);
}

/**
 * The main theme function for the slickgrid
 * @param array $options
 * @param array $columns
 * @param array $data
 * @param array $view_name
 */
function theme_slickgrid($variables){
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
  if($variables['options']['editable']){
    slickgrid_add_modal();
  }
  // Inline JS defining the slickgrid
  $js = array();
  $js[] = 'var options = ' . drupal_json_encode($variables['options']) . ';';
  $js[] = 'var data = [];';
  if(count($variables['data'])){
    $js[] = 'data = ' . drupal_json_encode($variables['data']) . ';';
  }
  $js[] = 'var columns = ' . slickgrid_encode_columns($variables['columns']) . ';';
  $js[] = 'var slickgrid;';
  $js[] = '(function($) {';
  $js[] = '$(document).ready(function(){';
  $js[] = 'slickgrid = new Slickgrid("#slickgrid", "' . $variables['view']->name . '", "' . $variables['view']->current_display . '", "' . url(SLICKGRID_CALLBACK_PATH) . '");';
  $js[] = '});';
  $js[] = '})(jQuery);';
  drupal_add_js(implode("\n", $js), array(
    'type' => 'inline'
  ));
  $output = '<div id="slickgrid" style="width:100%;height:' . $variables['options']['viewport_height'] . 'px;" class="hideCols hideRows' . (isset($variables['options']['editable']) && $variables['options']['editable'] ? ' editable' : '') . '"></div>';
  return $output;
}

/**
 * Theme function for the slickgrid control interface
 * @param array $options
 * @param array $columns
 * @param array $data
 * @param array $view_name
 */
function theme_slickgrid_controls($variables){
  $path = drupal_get_path('module', 'slickgrid');
  drupal_add_css($path . '/css/slickgrid.controls.css');
  $library_path = libraries_get_path('slickgrid');
  $output = '';
  // Add controls defined by the slickgrid plugin (pager)
  if(isset($variables['options']['pager']) && $variables['options']['pager']){
    drupal_add_js($library_path . '/js/slickgrid/controls/slick.pager.js');
    drupal_add_css($library_path . '/js/slickgrid/controls/slick.pager.css');
    $output .= '<div class="slickgrid-control" id="slickgrid-pager"></div>';
  }
  // Add controls defined by our slickgrid module (undo, export, delete)
  if($variables['options']['undo']){
    drupal_add_js($path . '/js/controls/slickgrid.undo.js');
    $output .= '<div class="slickgrid-control" id="slickgrid-undo"></div>';
  }
  if($variables['options']['delete']){
    drupal_add_js($path . '/js/controls/slickgrid.delete.js');
    $output .= '<div class="slickgrid-control" id="slickgrid-delete"></div>';
  }
  // TODO export is more complicated. needs to plug into views export module 
  //  if($variables['options']['export']){
  //    drupal_add_js($path . '/js/controls/slickgrid.undo.js');
  //    $output .= '<div id="slickgrid-undo" title="Undo"></div>';
  //  }
  $output .= '<div id="slickgrid-status"></div>';
  $output .= '<div id="slickgrid-loading-bar"></div>';
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
    t('Width'),
    t('Sortable')
  );
  $variables['rows'] = array();
  $plugin_types = slickgrid_get_plugin_types();
  $variables['header'] += $plugin_types;
  foreach(element_children($form['columns']) as $id){
    $row = array();
    $row[] = drupal_render($form['columns'][$id]['name']);
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