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
      l('Edit', 'admin/config/content/dwcarchiver/' . $result->did . '/edit'),
      l('Delete', 'admin/config/content/dwcarchiver/' . $result->did . '/delete'),
      l('Rebuild', 'admin/config/content/dwcarchiver/' . $result->did . '/rebuild'),
      l('Download', $result->machine_name . '.zip')
    );
  }
  return array(
    '#theme' => 'table',
    '#header' => array(
      t('Name'),
      t('Machine name'),
      t('Entity/Bundle'),
      array(
        'data' => t('Operations'),
        'colspan' => 4
      )
    ),
    '#rows' => $rows
  );
}

/**
 * Deliver the DwC-A
 */
function dwcarchiver_deliver_archive($did){
  $dwcarchiver = dwcarchiver_load($did);
  $filename = 'public://dwcarchiver/' . $dwcarchiver->machine_name . '.zip';
  if(file_exists($filename)){
    // Set headers.
    header('Content-type: application/octet-stream');
    header('Content-Disposition: attachment; filename="' . $dwcarchiver->machine_name . '.zip"');
    readfile($filename);
  }else{
    drupal_set_message(t('The archive "@archive_name" has not yet been created.', array(
      '@archive_name' => $dwcarchiver->name
    )), 'error');
    drupal_goto('');
  }
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
      'rowtype' => '',
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
  $options = array();
  foreach($core_types as $index => $core_type){
    $options[$index] = $core_type['label'];
  }
  $form = array(
    'core' => array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#collapsed' => FALSE,
      '#collapsible' => FALSE,
      '#title' => t('Core'),
      '#description' => t('Options for the core object in the archive.'),
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
            'core',
            'name'
          )
        )
      ),
      'entity_bundle' => array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => 'Entity/Bundle',
        '#default_value' => $dwcarchiver->core_entity . '/' . $dwcarchiver->core_bundle,
        '#required' => TRUE
      ),
      'rowtype' => array(
        '#type' => 'textfield',
        '#title' => t('Row Type/URI'),
        '#default_value' => $dwcarchiver->rowtype,
        '#maxlength' => 255
      )
    )
  );
  if($dwcarchiver->name){
    $form['extensions'] = array(
      '#tree' => TRUE
    );
    // Prevent editing of certain fields once the archive has been created.
    $form['core']['entity_bundle']['#disabled'] = TRUE;
    $form['core']['machine_name']['#disabled'] = TRUE;
    $form['core']['fields'] = _dwcarchiver_archive_form_fields($dwcarchiver->core_entity, $dwcarchiver->core_bundle, $dwcarchiver->maps);
    foreach($dwcarchiver->extensions as $extension){
      $form['extensions'][$extension->machine_name] = array(
        '#type' => 'fieldset',
        '#title' => $extension->name,
        '#description' => $extension->rowtype,
        'fields' => _dwcarchiver_archive_form_fields($extension->extension_entity, $extension->extension_bundle, $extension->maps)
      );
    }
    $form['extensions']['add'] = array(
      '#table' => array(
        'header' => array(
          t('Name'),
          t('Row Type'),
          t('Entity/Bundle')
        )
      ),
      'name' => array(
        '#type' => 'textfield',
        '#title' => t('Extension name'),
        '#title_display' => 'invisible',
        '#size' => 15,
        '#description' => t('Extension name'),
        '#required' => FALSE
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
        ),
        '#required' => FALSE
      ),
      'rowtype' => array(
        '#type' => 'textfield',
        '#title' => t('Row Type/URI'),
        '#title_display' => 'invisible',
        '#size' => 15,
        '#description' => t('Row Type/URI')
      ),
      'entity_bundle' => array(
        '#title_display' => 'invisible',
        '#type' => 'select',
        '#options' => $core_types[$dwcarchiver->core_entity . '/' . $dwcarchiver->core_bundle]['links'],
        '#title' => 'Entity/Bundle',
        '#default_value' => $dwcarchiver->core_entity . '/' . $dwcarchiver->core_bundle,
        '#required' => TRUE
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
 * Helper function to return a form array for selecting fields for a specific
 * entity/bundle.
 */
function _dwcarchiver_archive_form_fields($entity, $bundle, $maps = array()){
  $processors = dwcarchiver_get_processors();
  foreach($processors as $key => $value){
    $processors[$key] = $value['label'];
  }
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Field maps'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#table' => array(
      'header' => array(
        t('Label'),
        t('Maps to - Enter the URI that this field maps to.  If left blank, this field will not be included in the DwC-A.'),
        t('Processor')
      )
    ),
    '#theme' => 'dwcarchiver_archiver_form_fields_table'
  );
  // Add the fields that are relevant for the bundle.
  $fields = field_read_instances(array(
    'entity_type' => $entity,
    'bundle' => $bundle
  ));
  foreach($maps as $field => $map){
    $form[$field] = array(
      'label' => array(
        '#markup' => ''
      ),
      'term' => array(
        '#title_display' => 'invisible',
        '#type' => 'textfield',
        '#title' => t('Maps to'),
        '#default_value' => $map->uri,
        '#disabled' => TRUE
      ),
      'processor' => array(
        '#title_display' => 'invisible',
        '#type' => 'select',
        '#title' => t('Processor'),
        '#options' => $processors,
        '#default_value' => $map->processor,
        '#disabled' => TRUE
      )
    );
  }
  foreach($fields as $field){
    $form[$field['field_name']] = array(
      'label' => array(
        '#markup' => $field['label']
      ),
      'term' => array(
        '#title_display' => 'invisible',
        '#type' => 'textfield',
        '#title' => t('Maps to'),
        '#default_value' => !empty($maps[$field['field_name']]) ? $maps[$field['field_name']]->uri : ''
      ),
      'processor' => array(
        '#title_display' => 'invisible',
        '#type' => 'select',
        '#title' => t('Processor'),
        '#options' => $processors,
        '#default_value' => !empty($maps[$field['field_name']]) ? $maps[$field['field_name']]->processor : ''
      )
    );
  }
  return $form;
}

/**
 * Save the archive.
 */
function dwcarchiver_archive_form_validate(&$form, &$form_state){
  // If we are creating a new extension, we ensure that a URI has also been
  // submitted.
  if(!empty($form_state['values']['extensions']['add']['name']) && empty($form_state['values']['extensions']['add']['rowtype'])){
    form_set_error('extensions][add][rowtype', t('Row Type is required when creating a new extension'));
  }
}

/**
 * Save the archive.
 */
function dwcarchiver_archive_form_submit(&$form, &$form_state){
  // Update the archive object.
  $fields = $form_state['values']['core'];
  $core = explode('/', $fields['entity_bundle']);
  unset($fields['entity_bundle']);
  unset($fields['fields']);
  $fields['core_entity'] = $core[0];
  $fields['core_bundle'] = $core[1];
  $return = db_merge('dwcarchiver_archive')->key(array(
    'machine_name' => $form_state['values']['core']['machine_name']
  ))->fields($fields)->execute();
  $dwcarchiver = dwcarchiver_machine_name_load($form_state['values']['core']['machine_name']);
  // Create an insert which we'll use for the core and extension mappings.
  $insert_into_mappings = FALSE;
  $insert = db_insert('dwcarchiver_map')->fields(array(
    'eid',
    'did',
    'field',
    'uri',
    'processor'
  ));
  // If we have mappings for the core item, we also work with them!
  if(!empty($form_state['values']['core']['fields'])){
    foreach($form_state['values']['core']['fields'] as $field => $values){
      if($values['term'] && !in_array($field, array(
        'id',
        'coreid'
      ))){
        $insert_into_mappings = TRUE;
        $insert->values(array(
          'eid' => 0,
          'did' => $dwcarchiver->did,
          'field' => $field,
          'uri' => $values['term'],
          'processor' => $values['processor']
        ));
      }
    }
  }
  // If we have extensions, we work with them.
  if(!empty($form_state['values']['extensions'])){
    foreach($form_state['values']['extensions'] as $key => $values){
      switch($key){
        case 'add':
          if(!empty($form_state['values']['extensions']['add']['name'])){
            $fields = $form_state['values']['extensions']['add'];
            $fields['did'] = $dwcarchiver->did;
            $core = explode('/', $fields['entity_bundle']);
            unset($fields['entity_bundle']);
            $fields['field_name'] = $core[0];
            $fields['extension_entity'] = $core[1];
            $fields['extension_bundle'] = $core[2];
            db_insert('dwcarchiver_extension')->fields($fields)->execute();
          }
          break;
        default:
          // Add maps.
          // We need to get the eid for the extension based on the machine name
          // and the did of the archiver.
          $extension = dwcarchiver_extension_machine_name_load($key, FALSE, $form_state);
          if(!empty($form_state['values']['extensions'][$key]['fields'])){
            foreach($form_state['values']['extensions'][$key]['fields'] as $field => $values){
              if($values['term'] && !in_array($field, array(
                'id',
                'coreid'
              ))){
                $insert_into_mappings = TRUE;
                $insert->values(array(
                  'eid' => $extension->eid,
                  'did' => $dwcarchiver->did,
                  'field' => $field,
                  'uri' => $values['term'],
                  'processor' => $values['processor']
                ));
              }
            }
          }
          break;
      }
    }
  }
  // Insert the mappings if required
  if($insert_into_mappings){
    db_delete('dwcarchiver_map')->condition('did', $dwcarchiver->did)->execute();
    $insert->execute();
  }
  // Finally, we redirect to the edit form.
  $form_state['redirect'] = 'admin/config/content/dwcarchiver/' . $dwcarchiver->did . '/edit';
  // Rebuild the menu so that the link to the archive works
  menu_rebuild();
}

/**
 * Delete a mapping form.
 */
function dwcarchiver_archive_delete_form($form, $form_state, $dwcarchiver){
  $form['did'] = array(
    '#type' => 'value',
    '#value' => arg(4)
  );
  return confirm_form($form, t('Are you sure you want to delete %title?', array(
    '%title' => $dwcarchiver->name
  )), 'admin/config/content/exif_custom', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Executes node deletion.
 *
 * @see node_delete_confirm()
 */
function dwcarchiver_archive_delete_form_submit($form, &$form_state){
  if($form_state['values']['confirm']){
    db_delete('dwcarchiver_archive')->condition('did', $form_state['values']['did'])->execute();
    db_delete('dwcarchiver_extension')->condition('did', $form_state['values']['did'])->execute();
    db_delete('dwcarchiver_map')->condition('did', $form_state['values']['did'])->execute();
  }
  $form_state['redirect'] = 'admin/config/content/dwcarchiver';
  // Rebuild the menu so that the link to the archive works
  menu_rebuild();
}