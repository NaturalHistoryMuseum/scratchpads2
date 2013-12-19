<?php

function _tools_features_sort($a, $b){
  $a_package = (isset($a->info['package']) ? $a->info['package'] : t('Tools'));
  $b_package = (isset($b->info['package']) ? $b->info['package'] : t('Tools'));
  $return_value = strcasecmp($a_package, $b_package);
  if($return_value !== 0){
    return $return_value;
  }else{
    return strcasecmp($a->info['name'], $b->info['name']);
  }
}

function tools_admin_form($form, &$form_state){
  // Get an array of feature objects 
  $features = tools_get_features();
  // Confirmation form
  if(isset($form_state['storage']['confirm'])){
    $path = drupal_get_path('module', 'tools');
    $form['#attached']['css'] = array(
      $path . '/css/tools_confirmation.css'
    );
    $enabled_features = array();
    foreach($features as $feature){
      if($feature->status == 1){
        $enabled_features[$feature->name] = $feature->info['name'];
      }
    }
    $tools_to_remove = array();
    foreach($form_state['values']['status'] as $feature_name => $feature_status){
      $features[$feature_name]->status = $feature_status;
      // If we are disabling a feature
      if(($feature_status == 0) && (array_key_exists($feature_name, $enabled_features))){
        $tools_to_remove[] = $enabled_features[$feature_name];
      }
    }
    // Format the array as a string
    $tools_to_remove_string = join(' and ', array_filter(array_merge(array(
      join(', ', array_slice($tools_to_remove, 0, -1))
    ), array_slice($tools_to_remove, -1))));
    // Pass the form values to the confirm form
    foreach($features as $feature){
      $form['status'][$feature->name] = array(
        '#type' => 'value',
        '#value' => $feature->status,
        '#parents' => array(
          'status',
          $feature->name
        )
      );
    }
    // Allow other modules to add module specific settings
    $pre_disable_info = array();
    foreach(module_implements('tools_pre_disable') as $module){
      $pre_disable_info[$module] = module_invoke($module, 'tools_pre_disable');
    }
    $confirmation_descriptions = array();
    $module_list = system_rebuild_module_data();
    foreach($pre_disable_info as $module_machine_name => $module_settings){
      $module_name = $module_list[$module_machine_name]->info['name'];
      if((in_array($module_name, $tools_to_remove)) && (!$module_settings['can_disable'])){
        if(isset($module_settings['cannot_disable_message'])){
          drupal_set_message($module_settings['cannot_disable_message']);
        }
        drupal_goto('admin/structure/tools');
      }
      if((in_array($module_name, $tools_to_remove)) && (isset($module_settings['disable_message']))){
        $confirmation_descriptions[] = $module_settings['disable_message'];
      }
    }
    // We set an empty string default value to avoid the drupal default message  
    $confirmation_description_text = '';
    foreach($confirmation_descriptions as $confirmation_description){
      $confirmation_description_text .= '<p>' . $confirmation_description . '</p>';
    }
    return confirm_form($form, t('Are you sure you want to disable %tools_to_remove?', array(
      '%tools_to_remove' => $tools_to_remove_string
    )), 'admin/structure/tools', t($confirmation_description_text), 'Yes', 'Cancel');
  }
  // Tools form
  $path = drupal_get_path('module', 'tools');
  $form['#attached']['css'] = array(
    $path . '/css/tools.css'
  );
  usort($features, '_tools_features_sort');
  $form['left'] = array(
    '#prefix' => '<div class="tools-container"><div class="tools-left">',
    '#suffix' => '</div>'
  );
  $form['right'] = array(
    '#prefix' => '<div class="tools-right">',
    '#suffix' => '</div><div class="clearfix"></div></div>'
  );
  $side = 'left';
  $last_package = FALSE;
  $half_way = count($features) / 2;
  $index = 0;
  foreach($features as $feature){
    $package = (isset($feature->info['package']) ? $feature->info['package'] : t('Tools'));
    // Note, this hasn't been set as ">=" to try to ensure that the left side is longer than the right (looks better).
    if($index > $half_way && $package != $last_package){
      $side = 'right';
    }
    $last_package = $package;
    $index++;
    if(!isset($form[$side][$package])){
      $form[$side][$package] = array(
        '#type' => 'fieldset',
        '#title' => $package,
        '#collapsible' => FALSE,
        '#attributes' => array(
          'class' => array(
            'tool-options'
          )
        ),
        '#tree' => FALSE
      );
    }
    $dummy_filter = new stdClass();
    $dummy_filter->settings = array(
      'filter_url_length' => 255
    );
    $form[$side][$package]['status'][$feature->name] = array(
      '#type' => 'switch',
      '#title' => $feature->info['name'],
      '#description' => _filter_url($feature->info['description'], $dummy_filter) . ($feature->status && isset($feature->info['configure']) && drupal_valid_path($feature->info['configure'], FALSE) ? '<br/>' . l('Configure ' . $feature->info['name'], $feature->info['configure']) : ''),
      '#default_value' => $feature->status,
      '#parents' => array(
        'status',
        $feature->name
      ),
      // Optional span for adding in icons
      '#prefix' => '<span class="tools-icon ' . drupal_html_class($feature->name . '-icon') . '"></span>'
    );
  }
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  if(module_exists('overlay') && overlay_get_mode() == 'child'){
    // If we are in an overlay, then ensure the parent frame admin paths
    // get refreshed as these might change when enabling modules.
    // See https://drupal.org/node/2129815
    $admin_paths = path_get_admin_paths();
    drupal_add_js('jQuery(document).ready(function(){
      try{
        window.parent.Drupal.settings.overlay.paths.admin = "' . str_replace("\n", '\\n', $admin_paths['admin']) . '";
        window.parent.Drupal.settings.overlay.paths.non_admin = "' . str_replace("\n", '\\n', $admin_paths['non_admin']) . '";
        window.parent.Drupal.overlay.adminPathRegExp = false;
      } catch(e){
      }
    });', 'inline');
  }
  return $form;
}

/**
 * Submit handler for the 'manage features' form
 */
function tools_admin_form_submit($form, &$form_state){
  // Compare submitted values to enabled modules
  $confirmation = false;
  $features = tools_get_features();
  $enabled_features = array();
  foreach($features as $feature){
    if($feature->status == 1){
      $enabled_features[] = $feature->name;
    }
  }
  foreach($form_state['values']['status'] as $feature_name => $feature_status){
    // Are we disabling a feature?
    if(($feature_status == 0) && (in_array($feature_name, $enabled_features))){
      $confirmation = true;
    }
  }
  // Only show confirmation form if a feature is being disabled
  if((!isset($form_state['storage']['confirm'])) && $confirmation){
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }else{
    // Process the form
    // Increase memory limits 
    ini_set('memory_limit', '512M');
    ini_set('max_execution_time', '60');
    $status = $form_state['values']['status'];
    $install = array_keys(array_filter($status));
    $disable = array_diff(array_keys($status), $install);
    // Disable first. If there are any features that are disabled that are
    // dependencies of features that have been queued for install, they will
    // be re-enabled.
    module_disable($disable);
    features_install_modules($install);
    if(module_exists('views')){
      ctools_include('export');
      ctools_export_load_object_reset('views_view');
    }
    menu_rebuild();
    menu_cache_clear_all();
    drupal_flush_all_caches();
    drupal_set_message(t('The configuration options have been saved.'));
  }
}

/**
 * Returns an array of feature objects
 */
function tools_get_features(){
  features_get_info(NULL, NULL, TRUE);
  features_rebuild();
  $modules = array_filter(features_get_modules(), 'tools_filter_modules');
  $features = array_filter(features_get_features(), 'tools_filter_modules');
  return (array_merge($modules, $features));
}

/**
 * array_filter() callback for excluding non-tool modules.
 */
function tools_filter_modules($module){
  return isset($module->info['tool']) && $module->info['tool'];
}

function tools_admin_form_confirm_delete($form, &$form_state, $comment){
  $form['#comment'] = $comment;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $comment->cid
  );
  return confirm_form($form, t('Are you sure you want to delete all darwin core data?', array(
    '%title' => $comment->subject
  )), 'node/' . $comment->nid, t('Any replies to this comment will be lost. This action cannot be undone.'), t('Delete'), t('Cancel'), 'comment_confirm_delete');
}
