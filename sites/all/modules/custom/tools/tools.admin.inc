<?php

function tools_admin_form(){
  $form = array();
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
  foreach($features as $feature){
    $package = (isset($feature->info['package']) ? $feature->info['package'] : t('Tools'));
    if(!isset($form[$package])){
      $form[$package] = array(
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
    $form[$package]['status'][$feature->name] = array(
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
  $form['submit'] = array(
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
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * array_filter() callback for excluding non-tool modules.
 */
function tools_filter_modules($module){
  return isset($module->info['tool']) && $module->info['tool'];
}