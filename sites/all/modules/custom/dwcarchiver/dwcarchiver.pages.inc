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
 * Settings for the DwC-Archiver
 */
function dwcarchiver_settings($form, &$form_state){
  $form = system_settings_form(array(
    'dwcarchiver_rebuild_frequency' => array(
      '#type' => 'select',
      '#title' => t('Rebuild frequency'),
      '#options' => array(
        'change' => t('When required'),
        '24' => t('Once a day'),
        '168' => t('Once a week'),
        '672' => t('Once a month')
      ),
      '#default_value' => variable_get('dwcarchiver_rebuild_frequency', 'change'),
      '#description' => t('Select how frequently ALL archives should be rebuilt. Note, "When required" will attempt to detect when changes have been made, and will automatically rebuild an archive.')
    )
  ));
  // Need to specify the submit function, as the default system_settings_form
  // prevents ours from being executed.
  $form['#submit'][] = 'dwcarchiver_settings_submit';
  return $form;
}

/**
 * Submit for the above simply to redirect.
 */
function dwcarchiver_settings_submit($form, &$form_state){
  $form_state['redirect'] = 'admin/config/content/dwcarchiver';
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
        '#maxlength' => 255,
        '#required' => TRUE
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
      $form['extensions'][$extension->machine_name]['fields']['#suffix'] = '<div id="' . $extension->machine_name . '-add-more-wrapper"></div>';
      $form['extensions'][$extension->machine_name]['add_another_mapping'] = array(
        '#type' => 'submit',
        '#name' => $extension->machine_name . '-add-more',
        '#value' => t('Add another mapping'),
        '#submit' => array(
          'dwcarchiver_archive_form_add_another_mapping'
        ),
        '#ajax' => array(
          'callback' => 'dwcarchiver_archive_form_add_another_mapping_js',
          'wrapper' => $extension->machine_name . '-add-more-wrapper',
          'effect' => 'fade'
        )
      );
      $form['extensions'][$extension->machine_name]['delete'] = array(
        '#theme' => 'link',
        '#path' => 'admin/config/content/dwcarchiver/extension/' . $extension->eid . '/delete',
        '#options' => array(
          'attributes' => array(
            'style' => array(
              'float:right;'
            )
          ),
          'html' => FALSE
        ),
        '#text' => t('Delete extension')
      );
    }
    $form['extensions']['add'] = array(
      '#title' => t('Add extension'),
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
 * JS bit (return just the bit we're interested in)
 */
function dwcarchiver_archive_form_add_another_mapping_js($form, $form_state){
  $extension_machine_name = substr($form_state['triggering_element']['#name'], 0, -9);
  $element = $form['extensions'][$extension_machine_name]['fields'][count(element_children($form['extensions'][$extension_machine_name]['fields'])) - 1];
  $element['#suffix'] = '<div id="' . $extension_machine_name . '-add-more-wrapper"></div>';
  return $element;
}

/**
 * Submit function to add another mapping.
 */
function dwcarchiver_archive_form_add_another_mapping($form, &$form_state){
  // The triggering element has the machine name of the extension, so we look at
  // that, so that we can get the extension ID from the dwcarchiver object.
  $extension_machine_name = substr($form_state['triggering_element']['#name'], 0, -9);
  foreach($form_state['dwcarchiver']->extensions as $eid => $extension){
    if($extension->machine_name == $extension_machine_name){
      $extension->maps[] = array(
        (object)array(
          'eid' => $eid,
          'did' => $form_state['dwcarchiver']->did,
          'field' => 'uuid',
          'uri' => 'coreid',
          'processor' => 'dwcarchiver_raw_text'
        )
      );
      // Need to add magic to the dwcarchiver object.
      $form['extensions'][$extension_machine_name]['fields'] = _dwcarchiver_archive_form_fields($extension->extension_entity, $extension->extension_bundle, $extension->maps);
    }
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Helper function to return a form array for selecting fields for a specific
 * entity/bundle.
 */
function _dwcarchiver_archive_form_fields($entity_type, $bundle, $maps = array(), $prefix = ''){
  $form = array();
  foreach(array_keys($maps) as $delta){
    $form[$delta] = array(
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
      'entity_type' => $entity_type,
      'bundle' => $bundle
    ));
    // Get the entity info so that we can tell whether the label field should be
    // disabled (it shouldn't).
    $info = entity_get_info($entity_type);
    $actual_prefix = '';
    if($prefix){
      $actual_prefix = $prefix . ':';
    }
    // We run through the maps first to ensure they're at the top, and that any
    // none field maps (such as id) are included.
    $index = 0;
    foreach($maps[$delta] as $map){
      $index++;
      $index_str = str_pad($index, 4, '0', STR_PAD_LEFT);
      if((!$prefix && !strpos($map->field, ':')) || substr($map->field, 0, strlen($prefix) + 1) == $prefix . ':'){
        $field_instance = field_info_instance($entity_type, ($actual_prefix ? substr($map->field, strlen($actual_prefix)) : $map->field), $bundle);
        $label = ucfirst($map->field);
        $disabled = TRUE;
        $type = 'text';
        if($field_instance){
          $label = $field_instance['label'];
          $disabled = FALSE;
          $field_info = field_info_field($field_instance['field_name']);
          $type = $field_info['type'];
        }elseif(substr($map->field, 0, 7) == 'biblio_'){
          //$label = $field_instance['label'];
          $disabled = FALSE;
          $type = 'biblio';
        }        //elseif($map->field == $info['entity keys']['label']){
else{
          $disabled = FALSE;
        }
        $form[$delta]["mapped-$index_str:" . $map->field] = _dwcarchiver_archive_form_fields_row($label, $type, $map->uri, $map->processor, $disabled);
      }
    }
    // We add default maps for certain entity types.  All entity types have a 
    // label, so we add that first.
    $form[$delta][$actual_prefix . $entity_type] = _dwcarchiver_archive_form_fields_row(!empty($info['entity keys']['label']) ? t(ucfirst($info['entity keys']['label'])) : ucfirst($entity_type), 'text');
    $form[$delta][$actual_prefix . 'uuid'] = _dwcarchiver_archive_form_fields_row('UUID', 'text');
    switch($entity_type){
      case 'node':
        if(!$prefix){
          $form[$delta]['user'] = array(
            '#type' => 'fieldset',
            '#title' => t('Contributor'),
            'fields' => _dwcarchiver_archive_form_fields('user', 'user', array(
              $maps[$delta]
            ), 'user')
          );
        }
        $form[$delta][$actual_prefix . 'created'] = _dwcarchiver_archive_form_fields_row('Created', 'date');
        $form[$delta][$actual_prefix . 'edited'] = _dwcarchiver_archive_form_fields_row('Edited', 'date');
        break;
      case 'file':
        if(!$prefix){
          $form[$delta]['user'] = array(
            '#type' => 'fieldset',
            '#title' => t('Contributor'),
            'fields' => _dwcarchiver_archive_form_fields('user', 'user', array(
              $maps[$delta]
            ), 'user')
          );
        }
        $form[$delta][$actual_prefix . 'uri'] = _dwcarchiver_archive_form_fields_row('Path', 'uri');
        $form[$delta][$actual_prefix . 'filename'] = _dwcarchiver_archive_form_fields_row('Filename', 'entity_property');
        $form[$delta][$actual_prefix . 'filemime'] = _dwcarchiver_archive_form_fields_row('Mime', 'entity_property');
        break;
      case 'taxonomy_term':
        // We create a fieldset, and add all of the fields from the linked/bundle
        // entity to that fieldset.
        if(!$prefix){
          $form[$delta]['parent'] = array(
            '#type' => 'fieldset',
            '#title' => t('Parent'),
            'fields' => _dwcarchiver_archive_form_fields($entity_type, $bundle, array(
              $maps[$delta]
            ), 'parent')
          );
        }
        break;
    }
    foreach($fields as $field_instance){
      // If we have some form of reference field (e.g. node reference, user
      // reference, or term reference), then we check for the number of values
      // that can be referenced.  If it's only one, we add ALL fields from the 
      // linked item to a field set.  We do this only once to avoid loops.
      $field_info = field_info_field($field_instance['field_name']);
      switch($field_info['type']){
        case 'user_reference':
          break;
        case 'node_reference':
          // We create a fieldset, and add all of the fields from the linked/bundle
          // entity to that fieldset.
          if(!$prefix){
            $node_reference_bundle = array_filter($field_info['settings']['referenceable_types']);
            // We only attempt to add the fields if there is only one referenceable type.
            if(count($node_reference_bundle == 1)){
              $node_reference_bundle = array_pop($node_reference_bundle);
              $form[$delta][$field_instance['field_name']] = array(
                '#type' => 'fieldset',
                '#title' => $field_instance['label'],
                'fields' => _dwcarchiver_archive_form_fields('node', $node_reference_bundle, array(
                  $maps[$delta]
                ), $field_instance['field_name'])
              );
            }
          }
          break;
        case 'taxonomy_term_reference':
          // We create a fieldset, and add all of the fields from the linked/bundle
          // entity to that fieldset.
          if(!$prefix){
            $taxonomy_term_bundle = array();
            foreach($field_info['settings']['allowed_values'] as $key => $values){
              $taxonomy_term_bundle[] = $values['vocabulary'];
            }
            // We only attempt to add the fields if there is only one referenceable type.
            if(count($taxonomy_term_bundle == 1)){
              $taxonomy_term_bundle = array_pop($taxonomy_term_bundle);
              $form[$delta][$field_instance['field_name']] = array(
                '#type' => 'fieldset',
                '#title' => $field_instance['label'],
                'fields' => _dwcarchiver_archive_form_fields('taxonomy_term', $taxonomy_term_bundle, array(
                  $maps[$delta]
                ), $field_instance['field_name'])
              );
            }
          }
          break;
        default:
          $form[$delta][$actual_prefix . $field_instance['field_name']] = _dwcarchiver_archive_form_fields_row($field_instance['label'], $field_info['type']);
          break;
      }
    }
    if($entity_type == 'node' && $bundle == 'biblio' && function_exists('biblio_field_extra_fields')){
      // Biblio is a specieal case, so we need to add its special fields. GRRR!
      $biblio_fields = biblio_field_extra_fields();
      foreach($biblio_fields['node']['biblio']['form'] as $biblio_field_name => $biblio_field_values){
        $form[$delta][$actual_prefix . $biblio_field_name] = _dwcarchiver_archive_form_fields_row($biblio_field_values['label'], 'biblio');
      }
    }
  }
  return $form;
}

/**
 * Helper function to create a row for a field form.
 */
function _dwcarchiver_archive_form_fields_row($label, $field_type = 'ALL', $default_value_term = '', $default_value_processor = '', $disabled = FALSE){
  $processors = dwcarchiver_get_processors($field_type);
  if(count($processors)){
    foreach($processors as $key => $value){
      $processors[$key] = $value['label'];
    }
    return array(
      'label' => array(
        '#markup' => $label
      ),
      'term' => array(
        '#title_display' => 'invisible',
        '#type' => 'textfield',
        '#title' => t('Maps to'),
        '#default_value' => $default_value_term,
        '#disabled' => $disabled
      ),
      'processor' => array(
        '#title_display' => 'invisible',
        '#type' => 'select',
        '#title' => t('Processor'),
        '#options' => $processors,
        '#default_value' => $default_value_processor,
        '#disabled' => $disabled
      )
    );
  }else{
    return array(
      'label' => array(
        '#markup' => $label
      ),
      'term' => array(
        '#markup' => t('Unable to handle field of type "%type"', array(
          '%type' => $field_type
        ))
      ),
      'processor' => array(
        '#markup' => ''
      )
    );
  }
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
    'delta',
    'field',
    'uri',
    'processor'
  ));
  // If we have mappings for the core item, we also work with them!
  if(!empty($form_state['values']['core']['fields'][0])){
    foreach($form_state['values']['core']['fields'][0] as $field => $values){
      if(!empty($values['term']) && !in_array($field, array(
        'id',
        'coreid'
      ))){
        $insert_into_mappings = TRUE;
        $insert->values(array(
          'eid' => 0,
          'did' => $dwcarchiver->did,
          'delta' => 0,
          'field' => preg_replace('/mapped-[0-9]*:/', '', $field),
          'uri' => $values['term'],
          'processor' => $values['processor']
        ));
      }elseif(!empty($values['fields']) && count($values['fields'])){
        foreach($values['fields'][0] as $sub_field => $sub_values){
          if($sub_values['term']){
            $insert_into_mappings = TRUE;
            $insert->values(array(
              'eid' => 0,
              'did' => $dwcarchiver->did,
              'delta' => 0,
              'field' => preg_replace('/mapped-[0-9]*:/', '', $sub_field),
              'uri' => $sub_values['term'],
              'processor' => $sub_values['processor']
            ));
          }
        }
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
            foreach(array_keys($form_state['values']['extensions'][$key]['fields']) as $delta){
              if($delta == 2){
                $break = 'here';
              }
              foreach($form_state['values']['extensions'][$key]['fields'][$delta] as $field => $values){
                if(!empty($values['term']) && !in_array($field, array(
                  'id',
                  'coreid'
                ))){
                  $insert_into_mappings = TRUE;
                  $insert->values(array(
                    'eid' => $extension->eid,
                    'did' => $dwcarchiver->did,
                    'delta' => $delta,
                    'field' => preg_replace('/mapped-[0-9]*:/', '', $field),
                    'uri' => $values['term'],
                    'processor' => $values['processor']
                  ));
                }elseif(!empty($values['fields']) && count($values['fields'])){
                  foreach($values['fields'][0] as $sub_field => $sub_values){
                    if($sub_values['term']){
                      $insert_into_mappings = TRUE;
                      $insert->values(array(
                        'eid' => $extension->eid,
                        'did' => $dwcarchiver->did,
                        'delta' => $delta,
                        'field' => preg_replace('/mapped-[0-9]*:/', '', $sub_field),
                        'uri' => $sub_values['term'],
                        'processor' => $sub_values['processor']
                      ));
                    }
                  }
                }
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
  // Delete the zip file if we need to
  $dwcarchiver = dwcarchiver_load($form_state['values']['did']);
  drupal_unlink('public://dwcarchiver/' . $dwcarchiver->machine_name . '.zip');
  if($form_state['values']['confirm']){
    db_delete('dwcarchiver_archive')->condition('did', $form_state['values']['did'])->execute();
    db_delete('dwcarchiver_extension')->condition('did', $form_state['values']['did'])->execute();
    db_delete('dwcarchiver_map')->condition('did', $form_state['values']['did'])->execute();
  }
  $form_state['redirect'] = 'admin/config/content/dwcarchiver';
  // Rebuild the menu so that the link to the archive works
  menu_rebuild();
}

/**
 * Delete an extension form.
 */
function dwcarchiver_archive_extension_delete_form($form, $form_state, $dwcarchiver_extension){
  $form['eid'] = array(
    '#type' => 'value',
    '#value' => arg(5)
  );
  return confirm_form($form, t('Are you sure you want to delete the extension %title?', array(
    '%title' => $dwcarchiver_extension->name
  )), 'admin/config/content/dwcarchiver/' . $dwcarchiver_extension->did . '/edit', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Executes node deletion.
 *
 * @see node_delete_confirm()
 */
function dwcarchiver_archive_extension_delete_form_submit($form, &$form_state){
  $dwcarchiver_extension = dwcarchiver_extension_load($form_state['values']['eid']);
  if($form_state['values']['confirm']){
    db_delete('dwcarchiver_extension')->condition('eid', $form_state['values']['eid'])->execute();
    db_delete('dwcarchiver_map')->condition('eid', $form_state['values']['eid'])->execute();
  }
  $form_state['redirect'] = 'admin/config/content/dwcarchiver/' . $dwcarchiver_extension->did . '/edit';
}