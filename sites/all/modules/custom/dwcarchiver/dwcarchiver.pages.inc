<?php

/**
 * Callback for listing archives.
 */
function dwcarchiver_overview_page(){
  // Load the archives, and list 'em.
  $results = db_select('dwcarchiver_archive', 'd')->fields('d')->execute();
  $rows = array();
  foreach($results as $result){
    $rows[] = array(
      $result->name,
      $result->machine_name,
      // FIXME - Improve display of entity/bundle (use label rather than machine name).
      $result->core_entity . '/' . $result->core_bundle,
      l('View', 'admin/config/content/dwcarchiver/' . $result->did . '/view'),
      l('Edit', 'admin/config/content/dwcarchiver/' . $result->did . '/edit'),
      l('Delete', 'admin/config/content/dwcarchiver/' . $result->did . '/delete'),
      l('Rebuild', 'admin/config/content/dwcarchiver/' . $result->did . '/rebuild')
    );
  }
  return array(
    '#theme' => 'table',
    '#header' => array(
      'Name',
      'Machine name',
      'Entity/Bundle',
      array(
        'data' => 'Operations',
        'colspan' => 4
      )
    ),
    '#rows' => $rows
  );
}

/**
 * Form for adding a new archive
 */
function dwcarchiver_archive_form($form, &$form_state, $edit = array()){
  if(!isset($form_state['dwcarchiver'])){
    $dwcarchiver = is_object($edit) ? $edit : (object)$edit;
    $defaults = array(
      'name' => '',
      'machine_name' => '',
      'core_entity' => '',
      'core_bundle' => '',
      'extensions' => array()
    );
    foreach($defaults as $key => $value){
      if(!isset($dwcarchiver->$key)){
        $dwcarchiver->$key = $value;
      }
    }
    $form_state['dwcarchiver'] = $dwcarchiver;
  }else{
    $dwcarchiver = $form_state['dwcarchiver'];
  }
  $core_types = array();
  drupal_alter('dwcarchiver_core_types', $core_types);
  $form = array(
    'config' => array(
      '#tree' => TRUE,
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#default_value' => $dwcarchiver->name,
        '#maxlength' => 255,
        '#required' => TRUE
      ),
      'machine_name' => array(
        '#type' => 'machine_name',
        '#default_value' => $dwcarchiver->machine_name,
        '#maxlength' => 255,
        '#machine_name' => array(
          'exists' => 'dwcarchiver_machine_name_load',
          'source' => array(
            'config',
            'name'
          )
        ),
        '#disabled' => $dwcarchiver->name
      ),
      'core' => array(
        '#type' => 'radios',
        '#options' => $core_types,
        '#title' => 'Core',
        '#default_value' => $dwcarchiver->core_entity . '/' . $dwcarchiver->core_bundle,
        '#required' => TRUE
      )
    )
  );
  if($dwcarchiver->name){
    $form['extensions'] = array(
      '#tree' => TRUE,
      '#table' => array(
        '#headers' => array(
          ''
        )
      )
    );
    foreach($dwcarchiver->extensions as $extension){
      $form['extensions'][$extension->machine_name] = array(
        '#type' => 'fieldset',
        '#title' => $extension->name,
        '#description' => $extension->rowtype
      );
    }
    $form['extensions']['add'] = array(
      '#table' => array(
        'header' => array(
          t('Name'),
          t('Row Type')
        )
      ),
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Extension name'),
        '#title_display' => 'invisible',
        '#size' => 15,
        '#description' => t('Extension name')
      ),
      'machine_name' => array(
        '#type' => 'machine_name',
        '#default_value' => '',
        '#maxlength' => 255,
        '#machine_name' => array(
          'exists' => 'dwcarchiver_extension_machine_name_load',
          'source' => array(
            'extensions',
            'add',
            'name'
          )
        )
      ),
      'rowtype' => array(
        '#type' => 'textfield',
        '#title' => t('Row Type/URI'),
        '#title_display' => 'invisible',
        '#size' => 15,
        '#description' => t('Row Type/URI')
      )
    );
  }
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => $dwcarchiver->name ? t('Save configuration') : t('Create archive')
    )
  );
  return $form;
}

/**
 * Save the archive.
 */
function dwcarchiver_archive_form_submit(&$form, &$form_state){
  print_r($form_state['values']);
  exit();
  $fields = $form_state['values']['config'];
  $core = explode('/', $fields['core']);
  unset($fields['core']);
  $fields['core_entity'] = $core[0];
  $fields['core_bundle'] = $core[1];
  db_merge('dwcarchiver_archive')->key(array(
    'machine_name' => $form_state['values']['config']['machine_name']
  ))->fields($fields)->execute();
}