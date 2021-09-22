<?php

/**
 * Insert from view setup form
 * 
 */
function insert_from_view_setup_form($form, &$form_state){
  $form = array();
  $inserts = variable_get('insert_from_view_settings', array());
  $views = views_get_all_views();
  $views_select = array();
  foreach($views as $v){
    $views_select[$v->name] = $v->human_name;
  }
  foreach($inserts as $machine_name => $insert){
    $element = _insert_from_view_setup_form_element($machine_name, $insert, $form_state, $views_select, $views);
    $form = array_merge($form, $element);
  }
  $empty_element = _insert_from_view_setup_form_element('insert_from_view_new', array(), $form_state, $views_select, $views);
  $form = array_merge($form, $empty_element);
  $form['insert-from-view-submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save'
  );
  return $form;
}

/**
 * Single element for the view setup form (see insert_from_view_setup_form)
 * 
 */
function _insert_from_view_setup_form_element($machine_name, $settings, $form_state, $views_select, $views){
  $form = array();
  // Find default view
  $default_view = array_shift(array_keys($views));
  if(isset($form_state['values']["insert-$machine_name-view"])){
    $default_view = $form_state['values']["insert-$machine_name-view"];
  }else if(isset($settings['view'])){
    $default_view = $settings['view'];
  }
  // Get displays for view
  $displays_select = array();
  foreach($views[$default_view]->display as $display){
    $displays_select[$display->id] = $display->display_title;
  }
  // Default display
  if(isset($form_state['values']["insert-$machine_name-display"])){
    $default_display = $form_state['values']["insert-$machine_name-display"];
  }else if(isset($settings['display'])){
    $default_display = $settings['display'];
  }
  if(!isset($displays_select[$default_display])){
    $default_display = array_shift(array_keys($displays_select));
  }
  // Get fields for view/display
  $view = views_get_view($default_view);
  $view->set_display($default_display);
  $fields = $view->display_handler->get_option("fields");
  $fields_select = array(
    INSERT_FROM_VIEW_FIELD_ROW => t('- Rendered row -')
  );
  if(!empty($fields)){
    $fields = array_keys($fields);
    $fields = array_combine($fields, $fields);
    $fields_select = array_merge($fields_select, $fields);
  }
  // Default field
  $default_field = INSERT_FROM_VIEW_FIELD_ROW;
  if(isset($form_state['values']["insert-$machine_name-field"])){
    $default_field = $form_state['values']["insert-$machine_name-field"];
  }else if(isset($settings['field'])){
    $default_field = $settings['field'];
  }
  if(!in_array($default_field, $fields_select)){
    $default_field = INSERT_FROM_VIEW_FIELD_ROW;
  }
  // Default name
  $default_name = '';
  if(isset($form_state['values']["insert-$machine_name-name"])){
    $default_name = $form_state['values']["insert-$machine_name-name"];
  }else if(isset($settings['name'])){
    $default_name = $settings['name'];
  }
  // Default label
  $default_label = '';
  if(isset($form_state['values']["insert-$machine_name-label"])){
    $default_label = $form_state['values']["insert-$machine_name-label"];
  }else if(isset($settings['label'])){
    $default_label = $settings['label'];
  }
  // Default width/height
  $default_width = '75%';
  if(isset($form_state['values']["insert-$machine_name-width"])){
    $default_width = $form_state['values']["insert-$machine_name-width"];
  }else if(isset($settings['width'])){
    $default_width = $settings['width'];
  }
  $default_height = '75%';
  if(isset($form_state['values']["insert-$machine_name-height"])){
    $default_height = $form_state['values']["insert-$machine_name-height"];
  }else if(isset($settings['height'])){
    $default_height = $settings['height'];
  }
  // Default icon
  $default_icon = drupal_get_path('module', 'insert_from_view') . '/plugins/insert_from_view/insert_from_view.png';
  if(isset($form_state['values']["insert-$machine_name-icon"])){
    $default_icon = $form_state['values']["insert-$machine_name-icon"];
  }else if(isset($settings['icon'])){
    $default_icon = $settings['icon'];
  }
  // Default row wrap tag
  $default_row_tag = 'span';
  if(isset($form_state['values']["insert-$machine_name-row_tag"])){
    $default_row_tag = $form_state['values']["insert-$machine_name-row_tag"];
  }else if(isset($settings['row_tag'])){
    $default_row_tag = $settings['row_tag'];
  }
  // Default entry wrap tag
  $default_wrap_tag = '';
  if(isset($form_state['values']["insert-$machine_name-wrap_tag"])){
    $default_wrap_tag = $form_state['values']["insert-$machine_name-wrap_tag"];
  }else if(isset($settings['wrap_tag'])){
    $default_wrap_tag = $settings['wrap_tag'];
  }
  // The form  
  $form['insert-from-view-container'] = array(
    '#type' => 'vertical_tabs'
  );
  $form["insert-$machine_name-group"] = array(
    '#type' => 'fieldset',
    '#title' => $default_name ? $default_name : t('New'),
    '#group' => 'insert-from-view-container'
  );
  if(!empty($settings)){
    $form["insert-$machine_name-group"]["insert-$machine_name-remove"] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove this button'),
      '#default_value' => 0
    );
  }
  $form["insert-$machine_name-group"]["insert-$machine_name-name"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Name'),
    '#default_value' => $default_name
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-label"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Label'),
    '#default_value' => $default_label
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-view"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select view'),
    '#options' => $views_select,
    '#default_value' => $default_view,
    '#insert-from-view-key' => $machine_name,
    '#ajax' => array(
      'callback' => 'insert_from_view_select_view_callback',
      'wrapper' => "insert-$machine_name-display-wrapper"
    )
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-display"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select display'),
    '#options' => $displays_select,
    '#default_value' => $default_display,
    '#prefix' => "<div id='insert-$machine_name-display-wrapper'>",
    '#suffix' => "</div>",
    '#insert-from-view-key' => $machine_name,
    '#ajax' => array(
      'callback' => 'insert_from_view_select_display_callback',
      'wrapper' => "insert-$machine_name-field-wrapper"
    )
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-field"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select field'),
    '#options' => $fields_select,
    '#default_value' => $default_field,
    '#prefix' => "<div id='insert-$machine_name-field-wrapper'>",
    '#suffix' => '</div>'
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-wrap_tag"] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Wrap tag'),
    '#default_value' => $default_wrap_tag,
    '#description' => t('HTML tag used to wrap inserted entries (as a group)')
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-row_tag"] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Row tag'),
    '#default_value' => $default_row_tag,
    '#description' => t('HTML tag used to wrap each inserted row')
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-width"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Overlay width'),
    '#default_value' => $default_width
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-height"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Overlay height'),
    '#default_value' => $default_height
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-icon"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Icon'),
    '#default_value' => $default_icon,
    '#field_prefix' => $GLOBALS['base_url'] . '/'
  );
  return $form;
}

/**
 * Ajax callback for view/display selection
 */
function insert_from_view_select_view_callback($form, &$form_state){
  $machine_name = $form_state['triggering_element']['#insert-from-view-key'];
  return $form["insert-$machine_name-group"]["insert-$machine_name-display"];
}

function insert_from_view_select_display_callback($form, &$form_state){
  $machine_name = $form_state['triggering_element']['#insert-from-view-key'];
  return $form["insert-$machine_name-group"]["insert-$machine_name-field"];
}

/**
 * Settings form submit
 */
function insert_from_view_setup_form_submit($form, &$form_state){
  $settings = array();
  // Read the number of groups and their machine name
  foreach($form_state['values'] as $value_name => $value){
    if(!preg_match('/^insert-(.+)-name$/', $value_name, $matches)){
      continue;
    }
    if(empty($value)){
      continue;
    }
    $temp_name = $matches[1];
    if($temp_name == 'insert_from_view_new'){
      $inserts = variable_get('insert_from_view_settings', array());
      $machine_name = md5(time() + rand());
      while(isset($inserts[$machine_name])){
        $machine_name = md5(time() + rand());
      }
    }else{
      $machine_name = $temp_name;
    }
    if(!empty($form_state['values']["insert-$temp_name-remove"])){
      continue;
    }
    $settings[$machine_name] = array(
      'view' => $form_state['values']["insert-$temp_name-view"],
      'display' => $form_state['values']["insert-$temp_name-display"],
      'field' => $form_state['values']["insert-$temp_name-field"],
      'name' => $form_state['values']["insert-$temp_name-name"],
      'label' => $form_state['values']["insert-$temp_name-label"],
      'width' => $form_state['values']["insert-$temp_name-width"],
      'height' => $form_state['values']["insert-$temp_name-height"],
      'icon' => $form_state['values']["insert-$temp_name-icon"],
      'row_tag' => $form_state['values']["insert-$temp_name-row_tag"],
      'wrap_tag' => $form_state['values']["insert-$temp_name-wrap_tag"],
      'machine_name' => $machine_name
    );
  }
  variable_set('insert_from_view_settings', $settings);
  cache_clear_all('*', 'cache_bootstrap');
  drupal_set_message(t('Settings have been saved'), 'status');
}
