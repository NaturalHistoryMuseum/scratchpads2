<?php

function _tools_features_sort($a, $b){
  $a_package = (isset($a->info['package']) ? $a->info['package'] : t('Tools'));
  $b_package = (isset($b->info['package']) ? $b->info['package'] : t('Tools'));
  return strcasecmp($a_package, $b_package);
}
function tools_admin_form($form, &$form_state){
  $path = drupal_get_path('module', 'tools');
  $form['#attached']['css'] = array(
    $path . '/css/tools.css'
  );
  // Clear & rebuild key caches
  features_get_info(NULL, NULL, TRUE);
  features_rebuild();
  $modules = array_filter(features_get_modules(), 'tools_filter_modules');
  $features = array_filter(features_get_features(), 'tools_filter_modules');
  $features = array_merge($modules, $features);
  usort($features, '_tools_features_sort');
  $form['left'] = array('#prefix' => '<div class="tools-left">', '#suffix' => '</div>');
  $form['right'] = array('#prefix' => '<div class="tools-right">', '#suffix' => '</div><div class="clearfix"></div>');
  $side = 'left';
  $last_package = FALSE;
  $half_way = count($features)/2;
  $index = 0;
  foreach($features as $feature){
    $package = (isset($feature->info['package']) ? $feature->info['package'] : t('Tools'));
    // Note, this hasn't been set as ">=" to try to ensure that the left side is longer than the right (looks better).
    if($index > $half_way && $package != $last_package){
      $side = 'right';
    }
    $last_package = $package;
    $index ++;
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
    $form[$side][$package]['status'][$feature->name] = array(
      '#type' => 'switch',
      '#title' => $feature->info['name'],
      '#description' => $feature->info['description'],
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
  return $form;
}

/**
 * Submit handler for the 'manage features' form
 */
function tools_admin_form_submit(&$form, &$form_state){
  $status = $form_state['values']['status'];
  $install = array_keys(array_filter($status));
  $disable = array_diff(array_keys($status), $install);
  // Disable first. If there are any features that are disabled that are
  // dependencies of features that have been queued for install, they will
  // be re-enabled.
  module_disable($disable);
  features_install_modules($install);
  drupal_flush_all_caches();
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * array_filter() callback for excluding non-tool modules.
 */
function tools_filter_modules($module){
  return isset($module->info['tool']) && $module->info['tool'];
}
