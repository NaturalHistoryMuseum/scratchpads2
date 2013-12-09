<?php

/**
 * Insert from view setup form
 * 
 */
function insert_from_view_setup_form($form, &$form_state){
  $inserts = variable_get('insert_from_view_settings', array());
  $views = views_get_all_views();
  $views_select = array();
  foreach($views as $v){
    $views_select[$v->name] = $v->human_name;
  }
  $form = array();
  $form['insert-from-view-container'] = array(
    '#type' => 'vertical_tabs'
  );
  foreach($inserts as $machine_name => $insert){
    if(empty($insert['type']) || $insert['type'] == 'ckeditor'){
      $element = _insert_from_view_setup_form_element($machine_name, $insert, $form_state, $views_select, $views);
      $form = array_merge($form, $element);
    }
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
  $form_root = array(
    "insert-$machine_name-group" => array(
      '#type' => 'fieldset',
      '#title' => $default_name ? $default_name : t('New'),
      '#group' => 'insert-from-view-container',
      "insert-$machine_name-container" => array(
        '#type' => 'container',
        '#prefix' => "<div id='insert-$machine_name-display-wrapper'>",
        '#suffix' => "</div>"
      )
    )
  );
  $form = & $form_root["insert-$machine_name-group"]["insert-$machine_name-container"];
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
    '#insert-from-view-key' => $machine_name,
    '#ajax' => array(
      'callback' => 'insert_from_view_select_view_callback',
      'wrapper' => "insert-$machine_name-display-wrapper"
    )
  );
  $form["insert-$machine_name-group"]["insert-$machine_name-field"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select field'),
    '#options' => $fields_select,
    '#default_value' => $default_field
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
  return $form_root;
}

/**
 * Ajax callback for view/display selection (ckeditor form)
 */
function insert_from_view_select_view_callback($form, &$form_state){
  $machine_name = $form_state['triggering_element']['#insert-from-view-key'];
  return $form["insert-$machine_name-group"];
}

/**
 * Settings form submit (ckeditor form)
 */
function insert_from_view_setup_form_submit($form, &$form_state){
  $settings = variable_get('insert_from_view_settings', array());
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
      $machine_name = md5(time() + rand());
      while(isset($settings[$machine_name])){
        $machine_name = md5(time() + rand());
      }
    }else{
      $machine_name = $temp_name;
    }
    if(!empty($form_state['values']["insert-$temp_name-remove"])){
      unset($settings[$machine_name]);
      continue;
    }
    $settings[$machine_name] = array(
      'type' => 'ckeditor',
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

/**
 * Implement hook_field_widget_settings_form
 *
 * The node/term reference widget settings form
 */
function insert_from_view_field_widget_settings_form($field, $instance){
  // Get form values
  if(isset($_POST['instance']['widget']['settings']["insert-from-view"])){
    // Ajax callback values
    $state = $_POST['instance']['widget']['settings']["insert-from-view"];
  }else{
    $state = $instance['widget']['settings']["insert-from-view"];
  }
  // Get the list of view and the default view
  $views = views_get_all_views();
  $views_select = array();
  foreach($views as $v){
    $views_select[$v->name] = $v->human_name;
  }
  asort($views_select);
  $default_view = FALSE;
  if(isset($state['insert-from-view-view'])){
    $default_view = $state['insert-from-view-view'];
  }
  if(!$default_view || !isset($views_select[$default_view])){
    $keys = array_keys($views_select);
    $default_view = reset($keys);
  }
  // Get the list of displays and the default display
  $displays_select = array();
  foreach($views[$default_view]->display as $display){
    $displays_select[$display->id] = $display->display_title;
  }
  $default_display = FALSE;
  if(isset($state['insert-from-view-display'])){
    $default_display = $state['insert-from-view-display'];
  }
  if(!$default_display || !isset($displays_select[$default_display])){
    $keys = array_keys($displays_select);
    $default_display = reset($keys);
  }
  // Get the list of fields and the default field
  $view = views_get_view($default_view);
  $view->set_display($default_display);
  $fields = $view->display_handler->get_option("fields");
  $fields_select = array();
  if(!empty($fields)){
    $fields = array_keys($fields);
    $fields = array_combine($fields, $fields);
    $fields_select = array_merge($fields_select, $fields);
  }
  $default_field = FALSE;
  if(isset($state['insert-from-view-field'])){
    $default_field = $state['insert-from-view-field'];
  }
  if(!$default_field || !isset($fields_select[$default_field])){
    $keys = array_keys($fields_select);
    $default_field = reset($keys);
  }
  // Setup the form
  $form = array();
  $form["insert-from-view"] = array(
    '#type' => 'fieldset',
    '#title' => t('Insert from view'),
    '#prefix' => "<div id='insert-from-view-display-wrapper'>",
    '#suffix' => "</div>"
  );
  $form["insert-from-view"]["insert-from-view-view"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select view'),
    '#description' => t('Select the view to display in the overlay'),
    '#options' => $views_select,
    '#default_value' => $default_view,
    '#ajax' => array(
      'callback' => 'insert_from_view_field_select_view_callback',
      'wrapper' => "insert-from-view-display-wrapper"
    ),
    '#value_callback' => 'insert_from_view_widget_settings_submit'
  );
  $form["insert-from-view"]["insert-from-view-display"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select display'),
    '#description' => t('Select the view display to display in the overlay'),
    '#options' => $displays_select,
    '#default_value' => $default_display,
    '#ajax' => array(
      'callback' => 'insert_from_view_field_select_view_callback',
      'wrapper' => "insert-from-view-display-wrapper"
    )
  );
  $form["insert-from-view"]["insert-from-view-field"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select field'),
    '#description' => t('Select the field that holds the value to insert. The value should either be a node id, or (preferably) a node id followed by \':\' followed by the node title. This can be achieved in Views by using a \'Custom Text\' field and rewrite pattern of the type [nid]:[title]'),
    '#options' => $fields_select,
    '#default_value' => $default_field
  );
  $form["insert-from-view"]["insert-from-view-width"] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Overlay width'),
    '#default_value' => isset($state['insert-from-view-width']) ? $state['insert-from-view-width'] : '75%'
  );
  $form["insert-from-view"]["insert-from-view-height"] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Overlay height'),
    '#default_value' => isset($state['insert-from-view-height']) ? $state['insert-from-view-height'] : '75%'
  );
  return $form;
}

/**
 * Ajax callback for view/display selection (field form)
 */
function insert_from_view_field_select_view_callback($form, &$form_state){
  return $form['instance']['widget']['settings']["insert-from-view"];
}

/**
 * Submit (actually value callback) for widget settings form
 */
function insert_from_view_widget_settings_submit($element, $input, $form_state){
  if($input !== FALSE){
    $instance = $form_state['values']['instance'];
    $machine_name = '_field_' . $instance['field_name'] . '_' . $instance['entity_type'] . '_' . $instance['bundle'];
    $setting = array(
      'type' => 'field',
      'view' => $_POST['instance']['widget']['settings']['insert-from-view']["insert-from-view-view"],
      'display' => $_POST['instance']['widget']['settings']['insert-from-view']["insert-from-view-display"],
      'field' => $_POST['instance']['widget']['settings']['insert-from-view']["insert-from-view-field"],
      'width' => $_POST['instance']['widget']['settings']['insert-from-view']["insert-from-view-width"],
      'height' => $_POST['instance']['widget']['settings']['insert-from-view']["insert-from-view-height"],
      'machine_name' => $machine_name
    );
    $inserts = variable_get('insert_from_view_settings', array());
    $inserts[$machine_name] = $setting;
    variable_set('insert_from_view_settings', $inserts);
  }
}
