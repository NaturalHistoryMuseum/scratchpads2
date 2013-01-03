<?php
/**
 * Insert from view setup form
 * 
 */
function insert_from_view_setup_form($form, &$form_state) {
  $form = array();
  $inserts = variable_get('insert_from_view_settings', array());
  
  $views = views_get_all_views();
  $views_select = array();
  foreach ($views as $v) {
    $views_select[$v->name] = $v->human_name;
  }
  
  foreach ($inserts as $key => $insert) {
    $element = _insert_from_view_setup_form_element($key, $insert, $form_state, $views_select, $views);
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
function _insert_from_view_setup_form_element($key, $settings, $form_state, $views_select, $views) {
  $form = array();  
  
  // Find default view
  $default_view = array_shift(array_keys($views));
  if (isset($form_state['values']["insert-$key-view"])) {
    $default_view = $form_state['values']["insert-$key-view"];
  } else if (isset($settings['view'])) {
    $default_view = $settings['view'];
  }
  
  // Get displays for view
  $displays_select = array();
  foreach ($views[$default_view]->display as $display) {
    $displays_select[$display->id] = $display->display_title;
  }
  
  // Default display
  if (isset($form_state['values']["insert-$key-display"])) {
    $default_display = $form_state['values']["insert-$key-display"];
  } else if (isset($settings['display'])) {
    $default_display = $settings['display'];    
  }
  if (!isset($displays_select[$default_display])) {
    $default_display = array_shift(array_keys($displays_select));
  }

  // Get fields for view/display
  $view = views_get_view($default_view);
  $view->set_display($default_display);
  $fields = $view->display_handler->get_option("fields");
  $fields_select = array(INSERT_FROM_VIEW_FIELD_ROW => t('- Rendered row -'));
  if (!empty($fields)) {
    $fields = array_keys($fields);
    $fields = array_combine($fields, $fields);
    $fields_select = array_merge($fields_select, $fields);
  }

  // Default field
  $default_field = INSERT_FROM_VIEW_FIELD_ROW;
  if (isset($form_state['values']["insert-$key-field"])) {
    $default_field = $form_state['values']["insert-$key-field"];
  } else if (isset($settings['field'])) {
    $default_field = $settings['field'];
  }
  if (!in_array($default_field, $fields_select)) {
    $default_field = INSERT_FROM_VIEW_FIELD_ROW;
  }
  
  
  // Default name
  $default_name = '';
  if (isset($form_state['values']["insert-$key-name"])) {
    $default_name = $form_state['values']["insert-$key-name"];
  } else if (isset($settings['name'])) {
    $default_name = $settings['name'];
  }  
  
  // Default label
  $default_label= '';
  if (isset($form_state['values']["insert-$key-label"])) {
    $default_label = $form_state['values']["insert-$key-label"];
  } else if (isset($settings['label'])) {
    $default_label = $settings['label'];
  }
  
  // Default width/height
  $default_width = '75%';
  if (isset($form_state['values']["insert-$key-width"])) {
    $default_width = $form_state['values']["insert-$key-width"];
  } else if (isset($settings['width'])) {
    $default_width = $settings['width'];
  }
  $default_height = '75%';
  if (isset($form_state['values']["insert-$key-height"])) {
    $default_height = $form_state['values']["insert-$key-height"];
  } else if (isset($settings['height'])) {
    $default_height = $settings['height'];
  }
  
  // Default icon
  $default_icon = drupal_get_path('module', 'insert_from_view') . '/plugins/insert_from_view/insert_from_view.png';
  if (isset($form_state['values']["insert-$key-icon"])) {
    $default_icon = $form_state['values']["insert-$key-icon"];
  } else if (isset($settings['icon'])) {
    $default_icon = $settings['icon'];
  }
  
  // The form  
  $form["insert-$key-group"] = array(
    '#type' => 'fieldset',
    '#title' => $default_name ? $default_name : t('New')
  );
  
  if (!empty($settings)) {
    $form["insert-$key-group"]["insert-$key-remove"] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove this button'),
      '#default_value' => 0,
    );
  }
  
  $form["insert-$key-group"]["insert-$key-name"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Name'),
    '#default_value' => $default_name,
  );
  
  $form["insert-$key-group"]["insert-$key-label"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Label'),
    '#default_value' => $default_label,
  );
  
  $form["insert-$key-group"]["insert-$key-view"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select view'),
    '#options' => $views_select,
    '#default_value' => $default_view,
    '#insert-from-view-key' => $key,
    '#ajax' => array(
      'callback' => 'insert_from_view_select_view_callback',
      'wrapper' => "insert-$key-display-wrapper",
    ),
  );
  
  $form["insert-$key-group"]["insert-$key-display"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select display'),
    '#options' => $displays_select,
    '#default_value' => $default_display,
    '#prefix' => "<div id='insert-$key-display-wrapper'>",
    '#suffix' => "</div>",
    '#insert-from-view-key' => $key,
    '#ajax' => array(
      'callback' => 'insert_from_view_select_display_callback',
      'wrapper' => "insert-$key-field-wrapper",
    ),
  );
  
  $form["insert-$key-group"]["insert-$key-field"] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Select field'),
    '#options' => $fields_select,
    '#default_value' => $default_field,
    '#prefix' => "<div id='insert-$key-field-wrapper'>",
    '#suffix' => '</div>'
  );
  
  $form["insert-$key-group"]["insert-$key-width"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Overlay width'),
    '#default_value' => $default_width,
  );

  $form["insert-$key-group"]["insert-$key-height"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Overlay height'),
    '#default_value' => $default_height,
  );

  $form["insert-$key-group"]["insert-$key-icon"] = array(
    '#type' => 'textfield',
    '#required' => !empty($settings),
    '#title' => t('Icon'),
    '#default_value' => $default_icon,
    '#field_prefix' => $GLOBALS['base_url'] . '/',
  );
  
  return $form;
}

/**
 * Ajax callback for view/display selection
 */
function insert_from_view_select_view_callback($form, &$form_state) {
  $key = $form_state['triggering_element']['#insert-from-view-key'];
  return $form["insert-$key-group"]["insert-$key-display"];
}
function insert_from_view_select_display_callback($form, &$form_state) {
  $key = $form_state['triggering_element']['#insert-from-view-key'];
  return $form["insert-$key-group"]["insert-$key-field"];
}

/**
 * Settings form submit
 */
function insert_from_view_setup_form_submit($form, &$form_state) {
  $settings = array();

  // Read the number of groups and their key
  foreach($form_state['values'] as $value_name => $value) {
    if (!preg_match('/^insert-(.+)-name$/', $value_name, $matches)) {
      continue;
    }
    if (empty($value)) {
      continue;
    }
    
    $key = $matches[1];
    if ($key == 'insert_from_view_new') {
      // XXX potential race condition. Should use db to save settings instead
      $key_to_use = variable_get('insert_from_view_next_key', 0);
      variable_set('insert_from_view_next_key', $key_to_use+1);      
      $key_to_use = 'insert_from_view_' . $key_to_use;
    } else {
      $key_to_use = $key;
    }
    if (!empty($form_state['values']["insert-$key-remove"])) {
      continue;
    }
    
    $settings[$key_to_use] = array(
      'view' => $form_state['values']["insert-$key-view"],
      'display' => $form_state['values']["insert-$key-display"],
      'field' => $form_state['values']["insert-$key-field"],
      'name' => $form_state['values']["insert-$key-name"],
      'label' => $form_state['values']["insert-$key-label"],
      'width' => $form_state['values']["insert-$key-width"],
      'height' => $form_state['values']["insert-$key-height"],
      'icon' => $form_state['values']["insert-$key-icon"],
      'key' => $key_to_use,
    );
  }
  
  variable_set('insert_from_view_settings', $settings);
  
  cache_clear_all('*', 'cache_bootstrap');
  drupal_set_message(t('Settings have been saved'), 'status');
}
