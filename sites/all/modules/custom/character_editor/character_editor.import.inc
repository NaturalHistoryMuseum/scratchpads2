<?php

/**
 * character_editor_sdd_import_form
 *
 * This is a multi-ajax-step form.
 */
function character_editor_sdd_import_form($form, &$form_state){
  form_load_include($form_state, 'import.inc', 'character_editor');
  // Setup multi-step elements
  $form_state['step'] = isset($form_state['step']) ? $form_state['step'] : 'start';
  $form['#prefix'] = '<div id="character_editor_form_step">';
  $form['#suffix'] = '</div>';
  // Display messages if any
  if(!empty($form_state['sdd_messages'])){
    $form['messages'] = array(
      '#markup' => '<ul><li>' . implode('</li><li>', $form_state['sdd_messages']) . '</li></ul>'
    );
  }
  // Display main part of the form
  $submit_label = t('Continue');
  switch($form_state['step']){
    case 'start':
      $form['step_start'] = array(
        'sdd_file' => array(
          '#type' => 'file',
          '#title' => t('SDD file')
        )
      );
      $submit_label = t('Upload file');
      break;
    case 'info':
      // Get a list of existing character projects
      $efq = new EntityFieldQuery();
      $result = $efq->entityCondition('entity_type', 'character_editor_project')->execute();
      $character_projects = array();
      if(!empty($result['character_editor_project'])){
        $projects = entity_load('character_editor_project', array_keys($result['character_editor_project']));
        foreach($projects as $project){
          $character_projects[$project->id] = $project->title;
        }
      }
      $form['step_info'] = array();
      if(!empty($character_projects)){
        $form['step_info']['import_type'] = array(
          '#type' => 'select',
          '#options' => array(
            'new' => t('Import a new Character Project'),
            'old' => t('Import into an existing Character Project')
          )
        );
      }
      // Form for new characters
      $form['step_info']['new_character'] = array(
        '#type' => 'fieldset',
        '#title' => 'New character project settings',
        'title' => array(
          '#type' => 'textfield',
          '#title' => 'Title'
        ),
        'new_import_selection' => array(
          '#type' => 'select',
          '#options' => array(
            'all' => t('Import structure and data'),
            'structure' => t('Import structure only')
          )
        )
      );
      if(!empty($character_projects)){
        $form['step_info']['new_character']['#states'] = array(
          'visible' => array(
            ':input[name="import_type"]' => array(
              'value' => 'new'
            )
          )
        );
        // Form for old characters
        $form['step_info']['old_character'] = array(
          '#type' => 'fieldset',
          '#title' => 'Existing character project settings',
          'existing_project' => array(
            '#type' => 'select',
            '#title' => 'Character project',
            '#options' => $character_projects
          ),
          'old_import_selection' => array(
            '#type' => 'select',
            '#options' => array(
              'all' => t('Import structure and data'),
              'structure' => t('Import structure only'),
              'data' => t('Import data only')
            )
          ),
          'old_reuse' => array(
            '#type' => 'select',
            '#options' => array(
              'reuse' => t('Reuse existing characters with same name/type/states when possible'),
              'always' => t('Only use characters that already exist'),
              'never' => t('Always create new characters')
            ),
            '#states' => array(
              'invisible' => array(
                ':input[name="old_import_selection"]' => array(
                  'value' => 'data'
                )
              )
            )
          ),
          '#states' => array(
            'visible' => array(
              ':input[name="import_type"]' => array(
                'value' => 'old'
              )
            )
          )
        );
      }
      $submit_label = t('Import');
      break;
    case 'retry':
      $submit_label = t('Try again');
      break;
  }
  // Add the submit button
  $form['sdd_submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_label,
    '#ajax' => array(
      'callback' => 'character_editor_sdd_import_ajax',
      'wrapper' => 'character_editor_form_step'
    )
  );
  return $form;
}

/**
 * characater_editor_sdd_import_ajax
 *
 * Simply return the whole form, the step selection is done in the form
 * builder.
 */
function character_editor_sdd_import_ajax($form, &$form_state){
  return $form;
}

/**
 * character_editor_sdd_import_form_validate
 */
function character_editor_sdd_import_form_validate($form, $form_state){
  if($form_state['step'] == 'start'){
    if(empty($_FILES['files']['name']['sdd_file'])){
      form_set_error('sdd_file', t('No file selected'));
    }else if(!preg_match('/\.sdd$/', $_FILES['files']['name']['sdd_file'])){
      form_set_error('sdd_file', t('Allowed extention: .sdd'));
    }
  }else if($form_state['step'] == 'info'){
    if($form_state['values']['import_type'] == 'new' && empty($form_state['values']['title'])){
      form_set_error('title', t('Title is required'));
    }
  }
}

/**
 * character_editor_sdd_import_form_submit
 */
function character_editor_sdd_import_form_submit($form, &$form_state){
  module_load_include('inc', 'character_editor', 'includes/reverse_schema_array');
  // Check which step to submit for
  if($form_state['step'] == 'start'){
    $file = file_save_upload('sdd_file', array(
      'file_validate_extensions' => array(
        'sdd'
      )
    ));
    $import = new CharacterEditorSDDImport($file->uri);
    if($import->validateFile()){
      $form_state['sdd_uri'] = $file->uri;
      $form_state['step'] = 'info';
    }else{
      drupal_set_message("SDD file does not validate", "error");
      $form_state['step'] = 'retry';
    }
    foreach($import->getMessages() as $msg){
      drupal_set_message($msg[0], $msg[1]);
    }
  }else if($form_state['step'] == 'info'){
    $options = array();
    if(empty($form_state['values']['import_type']) || $form_state['values']['import_type'] == 'new'){
      $project_w = $form_state['values']['title'];
      $options['import_selection'] = $form_state['values']['new_import_selection'];
      $options['reuse'] = 'never';
    }else{
      $project = entity_load_single('character_editor_project', $form_state['values']['existing_project']);
      $project_w = entity_metadata_wrapper('character_editor_project', $project);
      $options['import_selection'] = $form_state['values']['old_import_selection'];
      if($options['import_selection'] == 'data'){
        $options['reuse'] = 'always';
      }else{
        $options['reuse'] = $form_state['values']['old_reuse'];
      }
    }
    $import = new CharacterEditorSDDImport($form_state['sdd_uri'], $options);
    $project_w = $import->import($project_w);
    foreach($import->getMessages() as $msg){
      drupal_set_message($msg[0], $msg[1]);
    }
    if($project_w){
      drupal_set_message(t("SDD import succesful. View the imported character project at !link", array(
        '!link' => l('character-editor-project/' . $project_w->getIdentifier(), 'character-editor-project/' . $project_w->getIdentifier())
      )), 'status');
    }else{
      drupal_set_message(t("SDD import failed."), 'error');
    }
    $form_state['step'] = 'start';
  }else if($form_state['step'] == 'retry'){
    $form_state['step'] = 'start';
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * character_editor_reverse_schema_array_sdd_taxonomy_term
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD Taxon Name entry is mapped to a taxonmy term.
 * $variables may define:
 * - 'available_vids': List of vocabulary IDs we can use when
 *   fetching existing terms.
 *
 * Note: This does not defined '#create' as we do not need it for
 * character imports. Will need to be implemented for full
 * SDD import.
 */
function character_editor_reverse_schema_array_sdd_taxonomy_term($variables){
  return array(
    'taxonomy_term' => array(
      '#xpath' => '/Datasets/Dataset/TaxonNames/TaxonName',
      '#id' => '@id',
      '#get' => array(
        'character_editor_sdd_import_get_taxon_name',
        isset($variables['available_vids']) ? $variables['available_vids'] : array()
      )
    )
  );
}

/**
 * character_editor_sdd_import_get_taxon_name
 *
 * Given an SDD taxon name, attempt to find the matching
 * existing taxonomy term
 */
function character_editor_sdd_import_get_taxon_name($xml_map, $xml_node, $available_vids){
  $attempts = array();
  $results = array();
  $attempts[] = $xml_map->nodeToString($xml_node->xpath('CanonicalName/Simple[1]'));
  $attempts[] = $xml_map->nodeToString($xml_node->xpath('Representation/Label[1]'));
  foreach($attempts as $attempt){
    if(empty($attempt)){
      continue;
    }
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'taxonomy_term');
    if(!empty($available_vids)){
      $efq->propertyCondition('vid', $available_vids);
    }
    $efq->propertyCondition('name', $attempt);
    $result = $efq->execute();
    if(count($result['taxonomy_term']) > 1){
      $xml_map->addMessage("Multiple matches for " . implode('/', $attempts) . " - this taxon name will not be used.");
    }else if(count($result['taxonomy_term']) == 1){
      $result_keys = array_keys($result['taxonomy_term']);
      $term = entity_load_single('taxonomy_term', reset($result_keys));
      return entity_metadata_wrapper('taxonomy_term', $term);
    }
  }
  $xml_map->addMessage("Could not find a match for " . implode('/', $attempts) . " - this taxon name will not be used.");
  return NULL;
}

/**
 * character_editor_reverse_schema_array_sdd_specimen
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD Specimen entry is mapped to a specimen. This only
 * checks the name, it does not try to check whether there is TaxonName
 * tag and equivalent taxon name.
 *
 * Note: This does not defined '#create' as we do not need it for
 * character imports. Will need to be implemented for full
 * SDD import.
 */
function character_editor_reverse_schema_array_sdd_specimen($variables){
  return array(
    'node' => array(
      '#xpath' => '/Datasets/Dataset/Specimens/Specimen',
      '#id' => 'Representation/Label[1]',
      '#get' => array(
        'character_editor_sdd_import_get_specimen'
      )
    )
  );
}

/**
 * character_editor_sdd_import_get_specimen
 *
 * Given an SDD specimen name, attempt to find the matching
 * existing specimen
 */
function character_editor_sdd_import_get_specimen($xml_map, $xml_node){
  $name = $xml_map->nodeToString($xml_node->xpath('Representation/Label[1]'));
  if(!$name){
    $xml_map->addMessage("Could not find a match for specimen $name - this specimen will not be used.");
    return NULL;
  }
  $efq = new EntityFieldQuery();
  $efq->entityCondition('entity_type', 'node');
  $efq->propertyCondition('type', 'specimen_observation');
  $efq->propertyCondition('title', $name);
  $result = $efq->execute();
  if(count($result['node']) > 1){
    $xml_map->addMessage("Multiple matches for specimen $name - this specimen will not be used.");
    return NULL;
  }else if(count($result['node']) == 1){
    $result_keys = array_keys($result['node']);
    $node = entity_load_single('node', reset($result_keys));
    return entity_metadata_wrapper('node', $node);
  }else{
    $xml_map->addMessage("Could not find a match for specimen $name - this specimen will not be used.");
    return NULL;
  }
}

/**
 * character_editor_reverse_schema_array_sdd_media_object
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how a Media Object is imported
 *
 */
function character_editor_reverse_schema_array_sdd_media_object($variables){
  global $user;
  return array(
    'file' => array(
      '#xpath' => '/Datasets/Dataset/MediaObjects/MediaObject/Source[@href]',
      '#id' => '../@id',
      '#get' => array(
        'character_editor_sdd_import_get_media_object'
      ),
      '#create' => array(
        'uid' => isset($variables['uid']) ? $variables['uid'] : $user->uid,
        'timestamp' => isset($variables['timestamp']) ? $variables['timestamp'] : time(),
        'type' => 'image',
        'status' => 1
      ),
      'fid' => array(
        '#xpath' => '@href',
        '#set' => 'character_editor_sdd_import_set_media_object'
      ),
      'field_description:value' => '../Representation/Detail[1]'
    )
  );
}

/**
 * character_editor_sdd_import_get_media_object
 *
 * Try to match a media object
 */
function character_editor_sdd_import_get_media_object($xml_map, $xml_node){
  $href = $xml_map->nodeToString($xml_node->xpath('@href'));
  if(!$href){return FALSE;}
  $efq = new EntityFieldQuery();
  $efq->entityCondition('entity_type', 'file');
  $efq->propertyCondition('type', 'image');
  $efq->propertyCondition('filename', basename($href));
  $results = $efq->execute();
  if(!empty($results['file'])){
    $id = reset(array_keys($results['file']));
    $entity = entity_load_single('file', $id);
    $entity_w = entity_metadata_wrapper('file', $entity);
    return $entity_w;
  }
  return NULL;
}

/**
 * character_editor_sdd_import_set_media_object
 *
 * Create the media object
 */
function character_editor_sdd_import_set_media_object($xml_map, $entity_w, $field_name, $property, $value){
  $href = $xml_map->nodeToString($value);
  // Test protocol
  if(!preg_match('%^https?://%', $href)){
    if($entity_w->getIdentifier()){      // We already have data - so no need to worry about not being able to download it.
return;}
    if(preg_match('%^file:/%', $href) || !preg_match('/:/', $href)){
      $xml_map->addMessage(t("Please upload %ref separately (if needed) prior to importing the project. The corresponding Media Object will not be imported.", array(
        '%ref' => $href
      )));
    }else{
      $xml_map->addMessage(t("Only http(s):// protocol is supported for for Media Objects. The Media Object will not be imported."));
    }
    throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
  }
  // Test mime type
  $mime = file_get_mimetype($href);
  if(!$mime){
    $xml_map->addMessage(t("File %file is of unknown type. The Media Object will not be imported."));
    throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
  }
  if(!preg_match('/^image\/(jpeg|gif|png|svg+xml|tiff)$/', $mime)){
    $xml_map->addMessage(t("File %file is not an image (jpeg, gif, png, svg and tiff are supported). The Media Object will not be imported.", array(
      '%file' => $href
    )));
    throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
  }
  // Get the file
  $entity = $entity_w->raw();
  $default_loc = 'public://';
  $default_mode = FILE_EXISTS_RENAME;
  if($entity_w->getIdentifier()){
    // This was already downloaded - check file
    // modification time
    $headers = get_headers($href, 1);
    if($headers && !empty($headers['Last-Modified'])){
      $mtime = strtotime($headers['Last-Modified']);
      if($mtime && $mtime <= $entity_w->timestamp->value()){return;}
    }
    $default_loc = $entity->uri;
    $default_mode = FILE_EXISTS_REPLACE;
  }
  // Get the file
  $uri = system_retrieve_file($href, $default_loc, FALSE, $default_mode);
  if(!$uri){
    $xml_map->addMessage(t("Could not load file %file. The Media Object will not be imported.", array(
      '%file' => $href
    )));
    throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
  }
  $info = image_get_info($uri);
  if(!$info){
    file_unmanaged_delete($uri);
    $xml_map->addMessage(t("The file %file cannot be parsed, and does not seem to be an image. The Media Object will not be imported.", array(
      '%file' => $href
    )));
    throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
  }
  // Set the fields. The wrapper is not usefull for this, do it on the entity directly.
  $entity->filename = basename($href);
  $entity->title = $xml_map->nodeToString($value->xpath('../Representation/Label[1]'));
  $entity->uri = $uri;
  $entity->filemime = $mime;
  $entity->filesize = filesize($uri);
  $entity->image_dimensions = array(
    'width' => $info['width'],
    'height' => $info['height']
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_group
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD Descriptive Context is mapped to a Character Group.
 *
 */
function character_editor_reverse_schema_array_sdd_character_group($variables){
  global $user;
  return array(
    'character_editor_character#group' => array(
      '#xpath' => '/Datasets/Dataset/DescriptiveConcepts/DescriptiveConcept[@id=/Datasets/Dataset/CharacterTrees/CharacterTree/Nodes/Node/DescriptiveConcept/@ref]',
      '#id' => '@id',
      '#get' => array(
        'character_editor_sdd_import_get_character',
        'group',
        isset($variables['project_w']) ? $variables['project_w'] : NULL
      ),
      '#create' => array(
        'uid' => isset($variables['uid']) ? $variables['uid'] : $user->uid,
        'created' => isset($variables['created']) ? $variables['created'] : time(),
        'changed' => isset($variables['changed']) ? $variables['changed'] : time(),
        'type' => 'group',
        'title' => '(group)'
      ),
      'title' => 'Representation/Label[1]',
      'field_char_description' => 'Representation/Detail[1]',
      'field_char_image' => array(
        '#xpath' => 'Representation/MediaObject/@ref',
        '#process' => array(
          'reverse_schema_array_process_map',
          'file',
          array(
            'array' => TRUE,
            'error' => "MediaObject reference %ref is missing",
            'throw' => TRUE,
            'action' => REVERSE_SCHEMA_ARRAY_SKIP_VALUE
          )
        )
      )
    )
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_text
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD TextCharacter is mapped to a Character.
 *
 */
function character_editor_reverse_schema_array_sdd_character_text($variables){
  global $user;
  return array(
    'character_editor_character#text' => array(
      '#xpath' => '/Datasets/Dataset/Characters/TextCharacter',
      '#id' => '@id',
      '#get' => array(
        'character_editor_sdd_import_get_character',
        'text',
        isset($variables['project_w']) ? $variables['project_w'] : NULL
      ),
      '#create' => array(
        'uid' => isset($variables['uid']) ? $variables['uid'] : $user->uid,
        'created' => isset($variables['created']) ? $variables['created'] : time(),
        'changed' => isset($variables['changed']) ? $variables['changed'] : time(),
        'type' => 'text',
        'title' => '(text character)'
      ),
      'title' => 'Representation/Label[1]',
      'field_char_description' => 'Representation/Detail[1]',
      'field_char_image' => array(
        '#xpath' => 'Representation/MediaObject/@ref',
        '#process' => array(
          'reverse_schema_array_process_map',
          'file',
          array(
            'array' => TRUE,
            'error' => "MediaObject reference %ref is missing",
            'throw' => TRUE,
            'action' => REVERSE_SCHEMA_ARRAY_SKIP_VALUE
          )
        )
      )
    )
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_numeric
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD QuantitativeCharacter is mapped to a Character.
 *
 */
function character_editor_reverse_schema_array_sdd_character_numeric($variables){
  global $user;
  return array(
    'character_editor_character#numeric' => array(
      '#xpath' => '/Datasets/Dataset/Characters/QuantitativeCharacter',
      '#id' => '@id',
      '#get' => array(
        'character_editor_sdd_import_get_character',
        'numeric',
        isset($variables['project_w']) ? $variables['project_w'] : NULL
      ),
      '#create' => array(
        'uid' => isset($variables['uid']) ? $variables['uid'] : $user->uid,
        'created' => isset($variables['created']) ? $variables['created'] : time(),
        'changed' => isset($variables['changed']) ? $variables['changed'] : time(),
        'type' => 'numeric',
        'title' => '(numeric character)'
      ),
      'title' => 'Representation/Label[1]',
      'field_char_description' => 'Representation/Detail[1]',
      'field_char_unit' => array(
        '#xpath' => 'MeasurementUnit/Label[1]',
        '#process' => 'character_editor_sdd_import_process_measurement_unit'
      ),
      'field_char_image' => array(
        '#xpath' => 'Representation/MediaObject/@ref',
        '#process' => array(
          'reverse_schema_array_process_map',
          'file',
          array(
            'array' => TRUE,
            'error' => "MediaObject reference %ref is missing",
            'throw' => TRUE,
            'action' => REVERSE_SCHEMA_ARRAY_SKIP_VALUE
          )
        )
      )
    )
  );
}

/**
 * character_editor_sdd_import_process_measurement_unit
 *
 * Attempts to map possible measurement units to one of our
 * standard types.
 */
function character_editor_sdd_import_process_measurement_unit($xml_map, $value){
  $map = array(
    'mm' => 'mm',
    'millimetre' => 'mm',
    'millimeter' => 'mm',
    'cm' => 'cm',
    'centimetre' => 'cm',
    'centimeter' => 'cm',
    'm' => 'm',
    'metre' => 'm',
    'meter' => 'm',
    'gr' => 'gr',
    'gram' => 'gr',
    'kg' => 'kg',
    'kilogram' => 'kg'
  );
  $value = $xml_map->nodeToString($value);
  $value_clean = trim(strtolower($value));
  if(isset($map[$value_clean])){
    return $map[$value_clean];
  }else{
    return $value;
  }
}

/**
 * character_editor_reverse_schema_array_sdd_character_dna
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD SequenceCharacter is mapped to a Character.
 *
 */
function character_editor_reverse_schema_array_sdd_character_dna($variables){
  global $user;
  return array(
    'character_editor_character#dna' => array(
      '#xpath' => '/Datasets/Dataset/Characters/SequenceCharacter',
      '#id' => '@id',
      '#get' => array(
        'character_editor_sdd_import_get_character',
        'dna',
        isset($variables['project_w']) ? $variables['project_w'] : NULL
      ),
      '#create' => array(
        'uid' => isset($variables['uid']) ? $variables['uid'] : $user->uid,
        'created' => isset($variables['created']) ? $variables['created'] : time(),
        'changed' => isset($variables['changed']) ? $variables['changed'] : time(),
        'type' => 'dna',
        'title' => '(dna character)'
      ),
      'title' => 'Representation/Label[1]',
      'field_char_description' => 'Representation/Detail[1]',
      'field_char_image' => array(
        '#xpath' => 'Representation/MediaObject/@ref',
        '#process' => array(
          'reverse_schema_array_process_map',
          'file',
          array(
            'array' => TRUE,
            'error' => "MediaObject reference %ref is missing",
            'throw' => TRUE,
            'action' => REVERSE_SCHEMA_ARRAY_SKIP_VALUE
          )
        )
      )
    )
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_controlled
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD CategoricalCharacter is mapped to a Character.
 *
 */
function character_editor_reverse_schema_array_sdd_character_controlled($variables){
  global $user;
  return array(
    'character_editor_character#controlled' => array(
      '#xpath' => '/Datasets/Dataset/Characters/CategoricalCharacter',
      '#id' => '@id',
      '#get' => array(
        'character_editor_sdd_import_get_character',
        'controlled',
        isset($variables['project_w']) ? $variables['project_w'] : NULL
      ),
      '#create' => array(
        'uid' => isset($variables['uid']) ? $variables['uid'] : $user->uid,
        'created' => isset($variables['created']) ? $variables['created'] : time(),
        'changed' => isset($variables['changed']) ? $variables['changed'] : time(),
        'type' => 'controlled',
        'title' => '(controlled character)'
      ),
      'title' => 'Representation/Label[1]',
      'field_char_description' => 'Representation/Detail[1]',
      'field_char_states' => array(
        // Alternatively we could create the states before hand, and use '@id' here to map them. This
        // would require the state definitions to always have an ID however.
        '#xpath' => 'States/StateDefinition',
        '#set' => 'character_editor_sdd_import_set_character_state'
      ),
      'field_char_image' => array(
        '#xpath' => 'Representation/MediaObject/@ref',
        '#process' => array(
          'reverse_schema_array_process_map',
          'file',
          array(
            'array' => TRUE,
            'error' => "MediaObject reference %ref is missing",
            'throw' => TRUE,
            'action' => REVERSE_SCHEMA_ARRAY_SKIP_VALUE
          )
        )
      )
    )
  );
}

/**
 * character_editor_sdd_import_set_character_state
 *
 * Create the associated character state as a field collection,
 * and return an array that can be inserted in a field.
 *
 */
function character_editor_sdd_import_set_character_state($xml_map, $entity_w, $field_name, $property, $value){
  $state = entity_create('field_collection_item', array(
    'field_name' => 'field_char_states'
  ));
  $state->field_char_state_label = array(
    LANGUAGE_NONE => array(
      array(
        'value' => $xml_map->nodeToString($value->xpath('Representation/Label[1]'))
      )
    )
  );
  $mo_ref = reset($value->xpath('Representation/MediaObject/@ref'));
  if($mo_ref){
    $img = reverse_schema_array_process_map($xml_map, $mo_ref, 'file', array(
      'array' => TRUE,
      'error' => "MediaObject reference %ref is missing"
    ));
    if($img){
      $state->field_char_state_image = array(
        LANGUAGE_NONE => array(
          $img
        )
      );
    }
  }
  $state->setHostEntity($entity_w->type(), $entity_w->raw());
  // Add it to the map so it can be referenced
  $xml_id = $xml_map->nodeToString($value->xpath('@id'));
  $state_w = entity_metadata_wrapper('field_collection_item', $state);
  $xml_map->addMapItem($xml_id, $state_w);
}

/**
 * character_editor_sdd_import_get_character
 *
 * Try to match an XML Node to an existing character
 *
 * $map: The ReverseSchemaArrayXMLMap object
 * $node: The XML node to map
 * $type: The type of character
 * $project: Optional project to which the character must belong
 */
function character_editor_sdd_import_get_character($xml_map, $node, $type, $project_w){
  $efq = new EntityFieldQuery();
  $efq->entityCondition('entity_type', 'character_editor_character');
  $efq->propertyCondition('type', $type);
  // We map the title to Representation/Label and the
  // body to Representation/Detail for all character types.
  $title = $xml_map->nodeToString($node->xpath('Representation/Label[1]'));
  $description = $xml_map->nodeToString($node->xpath('Representation/Detail[1]'));
  $efq->propertyCondition('title', $title);
  if($description){
    $efq->fieldCondition('field_char_description', 'value', $description);
  }
  // Character type specific conditions
  switch($type){
    case 'numeric':
      $unit = character_editor_sdd_import_process_measurement_unit($xml_map, $node->xpath('MeasurementUnit/Label[1]'));
      if($unit){
        $efq->fieldCondition('field_char_unit', 'value', $unit);
      }
      break;
  }
  // Get results
  $results = $efq->execute();
  if(empty($results['character_editor_character'])){return FALSE;}
  $entities = entity_load('character_editor_character', array_keys($results['character_editor_character']));
  // Now filter the values manually
  foreach($entities as $entity){
    $entity_w = entity_metadata_wrapper('character_editor_character', $entity);
    // Check for empty description
    if(!$description && $entity_w->field_char_description->value()){
      continue;
    }
    // Restrict result to the project
    if(!character_editor_association_exists($project_w, $entity_w, 2)){
      continue;
    }
    if($type == 'numeric' && !$unit){
      if($entity_w->field_char_unit->value() != ''){
        continue;
      }
    }
    // For controlled characters, compare the available state
    $state_map = FALSE;
    if($type == 'controlled'){
      $states = array();
      $state_ids = array();
      foreach($node->xpath('States/StateDefinition') as $state_def_node){
        $states[] = $xml_map->nodeToString($state_def_node->xpath('Representation/Label[1]'));
        $state_ids[] = $xml_map->nodeToString($state_def_node->xpath('@id'));
      }
      $state_map = array();
      $state_found = 0;
      foreach($entity_w->field_char_states as $char_state){
        $label = $char_state->field_char_state_label->value();
        if(($pos = array_search($label, $states)) !== FALSE){
          $id = $state_ids[$pos];
          $state_map[$id] = $char_state;
          $state_found++;
        }else{
          break;
        }
      }
      if($state_found != count($states) || $state_found != count($entity_w->field_char_states)){
        continue;
      }
    }
    // The first one to make it wins!!!
    if($state_map !== FALSE){
      // Add the states to the XML map object so we have them available later.
      $xml_map->addMapItem($state_map);
    }
    return $entity_w;
  }
  $xml_map->addMessage(t("Character %title could not be mapped to an existing character.", array(
    '%title' => $title
  )));
  return FALSE;
}

/**
 * character_editor_reverse_schema_array_sdd_character_data_numeric
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD Quantitative coded description is saved.
 *
 * If $variables['sample'] is set and true, return the definition used for samples
 * as opposed to taxons.
 *
 * XXX we don't support ranges - test for them!
 */
function character_editor_reverse_schema_array_sdd_character_data_numeric($variables){
  $taxon = empty($variables['sample']);
  if($taxon){
    $spath = 'SummaryData';
    $value = 'Measure/@value';
  }else{
    $spath = 'SampleData/SamplingEvent/SamplingUnit';
    $value = '@value';
  }
  return array(
    'relation#character_state_numeric' => array(
      '#xpath' => "/Datasets/Dataset/CodedDescriptions/CodedDescription/$spath/Quantitative",
      '#get' => 'character_editor_sdd_import_get_character_data',
      '#create' => array(
        'relation_type' => 'character_state',
        'uid' => $user->uid,
        'created' => time(),
        'changed' => time()
      ),
      'endpoints' => array(
        '#xpath' => '.',
        '#combine' => TRUE,
        '#set' => array(
          'character_editor_sdd_import_set_character_state_data'
        )
      ),
      'field_character_state_data' => $value
    )
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_data_text
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD TextChar coded description is saved.
 *
 * If $variables['sample'] is set and true, return the definition used for samples
 * as opposed to taxons.
 */
function character_editor_reverse_schema_array_sdd_character_data_text($variables){
  $taxon = empty($variables['sample']);
  if($taxon){
    $spath = 'SummaryData';
  }else{
    $spath = 'SampleData/SamplingEvent/SamplingUnit';
  }
  return array(
    'relation#character_state_numeric' => array(
      '#xpath' => "/Datasets/Dataset/CodedDescriptions/CodedDescription/$spath/Text",
      '#get' => 'character_editor_sdd_import_get_character_data',
      '#create' => array(
        'relation_type' => 'character_state',
        'uid' => $user->uid,
        'created' => time(),
        'changed' => time()
      ),
      'endpoints' => array(
        '#xpath' => '.',
        '#combine' => TRUE,
        '#set' => array(
          'character_editor_sdd_import_set_character_state_data'
        )
      ),
      'field_character_state_data' => 'Content'
    )
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_data_dna
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD Sequence coded description is saved.
 *
 * If $variables['sample'] is set and true, return the definition used for samples
 * as opposed to taxons.
 */
function character_editor_reverse_schema_array_sdd_character_data_dna($variables){
  $taxon = empty($variables['sample']);
  if($taxon){
    $spath = 'SummaryData';
  }else{
    $spath = 'SampleData/SamplingEvent/SamplingUnit';
  }
  return array(
    'relation#character_state_numeric' => array(
      '#xpath' => "/Datasets/Dataset/CodedDescriptions/CodedDescription/$spath/Sequence",
      '#get' => 'character_editor_sdd_import_get_character_data',
      '#create' => array(
        'relation_type' => 'character_state',
        'uid' => $user->uid,
        'created' => time(),
        'changed' => time()
      ),
      'endpoints' => array(
        '#xpath' => '.',
        '#combine' => TRUE,
        '#set' => array(
          'character_editor_sdd_import_set_character_state_data'
        )
      ),
      'field_character_state_data' => 'Sequence'
    )
  );
}

/**
 * character_editor_reverse_schema_array_sdd_character_data_controlled
 *
 * Implement hook_reverse_schema_array_<schema>
 *
 * Defines how an SDD Categorical coded description is saved.
 *
 * If $variables['sample'] is set and true, return the definition used for samples
 * as opposed to taxons.
 */
function character_editor_reverse_schema_array_sdd_character_data_controlled($variables){
  $taxon = empty($variables['sample']);
  if($taxon){
    $spath = 'SummaryData';
  }else{
    $spath = 'SampleData/SamplingEvent/SamplingUnit';
  }
  return array(
    'relation#character_state_controlled' => array(
      '#xpath' => "/Datasets/Dataset/CodedDescriptions/CodedDescription/$spath/Categorical",
      '#get' => 'character_editor_sdd_import_get_character_data',
      '#create' => array(
        'relation_type' => 'character_state',
        'uid' => $user->uid,
        'created' => time(),
        'changed' => time()
      ),
      'endpoints' => array(
        '#xpath' => '.',
        '#combine' => TRUE,
        '#set' => array(
          'character_editor_sdd_import_set_character_state_data'
        )
      ),
      'field_character_state_data' => array(
        '#xpath' => 'State/@ref[1]',
        '#process' => array(
          'reverse_schema_array_process_map',
          'field_collection_item',
          array(
            'field' => 'item_id',
            'error' => "CodedDescription could not be imported as the Categorial state %ref is missing",
            'throw' => TRUE,
            'action' => REVERSE_SCHEMA_ARRAY_SKIP_ENTITY
          )
        )
      )
    )
  );
}

/**
 * character_editor_sdd_import_get_character_data
 *
 * Get a character data entry if it exists. There can be only
 * one entry between a given taxon/specimen and character - so we look
 * for that. We don't check if the value has changed, as we might
 * be updating it.
 */
function character_editor_sdd_import_get_character_data($xml_map, $xml_node){
  $parent = reset($xml_node->xpath('..'));
  if($parent->getName() == 'SummaryData'){
    $taxon_ref = $xml_map->nodeToString($xml_node->xpath('../../Scope/TaxonName/@ref[1]'));
    $item_w = $xml_map->getEntity('taxonomy_term', $taxon_ref);
  }else{
    $specimen_ref = $xml_map->nodeToString($xml_node->xpath('../../@id[1]'));
    $item_w = $xml_map->getEntity('node', $specimen_ref);
  }
  $character_ref = $xml_map->nodeToString($xml_node->xpath('@ref'));
  $char_w = $xml_map->getEntity('character_editor_character', $character_ref);
  if(!$item_w || !$char_w){return FALSE;}
  $values = character_editor_get_character_values($char_w, $item_w);
  if(empty($values)){
    return FALSE;
  }else{
    return $values[0]['relation_w'];
  }
}

/**
 * character_editor_sdd_import_set_character_state_data
 *
 * From a CodedDescription set the endpoints of the corresponding
 * relation. We must use a 'set' function for this as Relation
 * doesn't give us access to it's data via wrappers.
 */
function character_editor_sdd_import_set_character_state_data($xml_map, $entity_w, $field_name, $property, $value){
  $value = reset($value);
  $relation = $entity_w->raw();
  // Find the item
  $parent = reset($value->xpath('..'));
  if($parent->getName() == 'SummaryData'){
    $taxon_ref = $xml_map->nodeToString($value->xpath('../../Scope/TaxonName/@ref[1]'));
    $item_w = $xml_map->getEntity('taxonomy_term', $taxon_ref);
    if(!$item_w){
      $xml_map->addMessage("CodedDescription could not be imported due to invalid TaxonName reference $taxon_ref", "warning");
      throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
    }
  }else{
    $specimen_ref = $xml_map->nodeToString($value->xpath('../../@id[1]'));
    $item_w = $xml_map->getEntity('node', $specimen_ref);
    if(!$item_w){
      $xml_map->addMessage("CodedDescription could not be imported due to invalid Specimen reference $specimen_ref", "warning");
      throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
    }
  }
  // Find the character
  $character_ref = $xml_map->nodeToString($value->xpath('@ref'));
  $char_w = $xml_map->getEntity('character_editor_character', $character_ref);
  if(!$char_w){
    $xml_map->addMessage("CodedDescription could not be imported due to invalid character reference $character_ref", "warning");
    throw new ReverseSchemaArrayException(REVERSE_SCHEMA_ARRAY_SKIP_ENTITY);
  }
  $relation->endpoints[LANGUAGE_NONE] = array(
    array(
      'entity_type' => $item_w->type(),
      'entity_id' => $item_w->getIdentifier(),
      'r_index' => 0
    ),
    array(
      'entity_type' => $char_w->type(),
      'entity_id' => $char_w->getIdentifier(),
      'r_index' => 1
    )
  );
}

/**
 * class CharacterEditorSDDImport
 *
 * This class is used to import SDD files into character projects.
 *
 * Usage:
 *
 * $import = new CharacterEditorSDDImport($filename, $options);
 * if (!$import->validateFile()){
 *   drupal_set_message("SDD file does not validate", "error");
 * } else {
 *   $project_w = $import->import("A project");
 *   if (!$project_w){
 *     drupal_set_message("SDD file failed importing", "error");
 *   } else {
 *     drupal_set_message("SDD file successfully imported with id " . $project_w_>getIdentifier(), "status");
 *   }
 * }
 * foreach($import->getMessages() as $msg){
 *   drupal_set_message($msg[0], $msg[1]);
 * }
 *
 * $options is an array that may define:
 * message_count: Maximum number of one type to include in the reports
 *                Default 10
 *
 * import_selection: Defines what to import from the file, and can be one of:
 *   - 'all' : Import structure and data (Default);
 *   - 'structure' : Import structure only ;
 *   - 'data' : Import data only
 *
 * reuse: Defines whether we should attemp to reuse existing characters, and can be one of:
 *   - 'always': Always reuse characters, never create new ones. Do not import data of un-matched characters;
 *   - 'never' : Never reuse characters, always create new ones
 *   - 'reuse' : Reuse characters when possible, create new ones if not (Default).
 *
 */
class CharacterEditorSDDImport{
  /* The SDD file */
  private $filename;
  /* Options */
  private $options;
  /* The Reverse Schema Array mode used for creating characters */
  private $rsa_mode;
  /* Messages */
  private $messages = array();
  /*
   * Following variables are used during one import process
   */
  /* The SimpleXMLElement root */
  private $dom;
  /* The RSA object */
  private $map;
  /* The character project wrapper */
  private $project_w;

  /**
   * __construct
   */
  function __construct($filename, $options = array()){
    $this->filename = $filename;
    $this->options = array_merge(array(
      'import_selection' => 'all',
      'reuse' => 'reuse',
      'message_count' => 10
    ), $options);
    if($this->options['import_selection'] == 'data' || $this->options['reuse'] == 'always'){
      $this->rsa_mode = REVERSE_SCHEMA_ARRAY_XGET;
    }else if($this->options['reuse'] == 'never'){
      $this->rsa_mode = REVERSE_SCHEMA_ARRAY_CREATE;
    }else{
      $this->rsa_mode = REVERSE_SCHEMA_ARRAY_XGET | REVERSE_SCHEMA_ARRAY_CREATE;
    }
  }

  /**
   * validateFile
   *
   * Do basic validation on the file (exists, contains XML, contains
   * data we can import...), set messages accordingly and return
   * TRUE if the file validates and FALSE otherwise.
   *
   * Note that a TRUE return value here is not guarentee that the
   * import will succeed - as more complex failures can only be
   * detected during the import.
   *
   * This is called by 'import', so only call this separately if you
   * wish to report initial errors to the user.
   */
  public function validateFile(){
    // Read the content
    $content = file_get_contents($this->filename);
    if(!$content){
      $this->addMessage(t('The file is empty or could not be loaded.'), 'error');
      return FALSE;
    }
    // Check it's valid XML
    $use_errors = libxml_use_internal_errors(TRUE);
    $dom = simplexml_load_string(preg_replace('/xmlns\s*=\s*["\'][^"\']+["\']/', '', $content));
    if(!$dom){
      $errors = libxml_get_errors();
      $this->addMessage(t('The file does not contain valid XML. XML Parsing errors:'), 'error');
      foreach($errors as $error){
        $this->addMessage('line ' . $error->line . ', col ' . $error->column . ': ' . $error->message, 'error', 'xml error');
      }
      libxml_use_internal_errors($use_errors);
      return FALSE;
    }
    libxml_use_internal_errors($use_errors);
    // Look for content we won't import
    $fail = FALSE;
    if($dom->xpath('/Datasets/Dataset/IdentificationKeys')){
      $this->addMessage(t('Identification keys are not supported and will not be imported.'), 'warning');
    }
    if(!$dom->xpath('/Datasets/Dataset/TaxonNames') || !$dom->xpath('/Datasets/Dataset/Characters')){
      $this->addMessage(t('The SDD file cannot be imported: it must define some Taxon Names and some Characters.'), 'error');
      $fail = TRUE;
    }
    if($dom->xpath('/Datasets/Dataset/CharacterTrees/CharacterTree/Nodes/Node/Parent')){
      $this->addMessage(t('The SDD file cannot be imported: we do not support character trees that have a depth of more than 1.'), 'error');
      $fail = TRUE;
    }
    if($dom->xpath('/Datasets/Dataset/DescriptiveConcepts/ConceptStates')){
      $this->addMessage(t('The SDD file cannot be imported: we do not support descriptive concepts with states.'), 'error');
      $fail = TRUE;
    }
    if($dom->xpath('/Datasets/Dataset/Characters/QuantitativeCharacter/Default')){
      $this->addMessage(t('Quantitative character default values are not supported and will not be imported.'), 'warning');
    }
    if($dom->xpath('/Datasets/Dataset/Characters/QuantitativeCharacter/Mappings')){
      $this->addMessage(t('Quantitative character mappings are not supported and will not be imported.'), 'warning');
    }
    if($dom->xpath('/Datasets/Dataset/Characters/QuantitativeCharacter/Assumptions')){
      $this->addMessage(t('Quantitative character mappings are not supported and will not be imported.'), 'warning');
    }
    if($dom->xpath('/Datasets/Dataset/MediaObjects/MediaObject/EncodedData')){
      $this->addMessage(t('MediaObjects with Encoded Data will not be imported'), 'warning');
    }
    foreach($dom->xpath('/Datasets/Dataset/CodedDescriptions/CodedDescription/SummaryData/Categorical') as $dom_node){
      if(count($dom_node->xpath('State')) > 1){
        $this->addMessage(t('The SDD file cannot be imported: The character editor only supports one state per controlled (categorical) character.'), 'error');
        $fail = TRUE;
        break;
      }
    }
    if($fail){return FALSE;}
    // Try to match the taxon names to existing vids
    $map = new ReverseSchemaArrayMapXML($dom, 'http://rs.tdwg.org/UBIF/2006/', array(
      'autosave' => FALSE
    ));
    $available_vids = array_keys(variable_get('biological_vids', array()));
    try{
      $map->mapXML(reverse_schema_array('sdd_taxonomy_term', REVERSE_SCHEMA_ARRAY_XGET, array(
        'available_vids' => $available_vids
      )));
    }
    catch(Exception $e){
      $this->addMessage('The SDD file cannot be imported. Failed to import taxon names: ' . $e->getMessage(), 'error');
      return FALSE;
    }
    $info = $map->getInfo();
    $total_usable = $info['total'];
    if($info['skipped']){
      $this->addMessage(t('There were %count taxon names in the file that were successfully mapped to existing taxon names, and %badcount taxon names that could not be mapped and will not be imported:', array(
        '%count' => $info['total'],
        '%badcount' => $info['skipped']
      )), 'warning');
      $this->addMessage($map->getMessages(), 'warning', 'map');
    }else{
      $this->addMessage(t('There were %count taxon names in the file that were successfuly mapped to existing taxon names.', array(
        '%count' => $info['total']
      )), 'status');
    }
    // Try to match specimen to existing nodes
    try{
      $map->flushInfo();
      $map->flushMessages();
      $map->mapXML(reverse_schema_array('sdd_specimen', REVERSE_SCHEMA_ARRAY_XGET));
    }
    catch(Exception $e){
      $this->addMessage('The SDD file cannot be imported. Failed to import specimen: ' . $e->getMessage(), 'error');
      return FALSE;
    }
    $info = $map->getInfo();
    if($info['total'] == 0){
      if($total_usable == 0){
        $this->addMessage("There were no Taxon Names nor specimens that could be imported.", 'error');
        $this->addMessage($map->getMessages(), 'warning', 'mapsp');
        return FALSE;
      }
    }
    if($info['skipped']){
      $this->addMessage(t('There were %count specimen in the file that were successfully mapped to existing taxon names, and %badcount specimen that could not be mapped and will not be imported:', array(
        '%count' => $info['total'],
        '%badcount' => $info['skipped']
      )), 'warning');
      $this->addMessage($map->getMessages(), 'warning', 'mapsp');
    }else{
      $this->addMessage(t('There were %count taxon names in the file that were successfuly mapped to existing specimen.', array(
        '%count' => $info['total']
      )), 'status');
    }
    // Additional file tests
    $missing_files = array();
    $potential = $dom->xpath('/Datasets/Dataset/MediaObjects/MediaObject/Source[starts-with(@href, \'file:/\')]');
    $potential = array_merge($potential, $dom->xpath('/Datasets/Dataset/MediaObjects/MediaObject/Source[not(contains(@href, \':\'))]'));
    foreach($potential as $dom_node){
      $e = character_editor_sdd_import_get_media_object($map, $dom_node);
      if(!$e){
        $missing_files[] = $map->nodeToString($dom_node->xpath('@href'));
      }
    }
    if(count($missing_files)){
      $this->addMessage(t("The SDD file contains references to local images that cannot be found on the site. You will need to upload those separately first if you want them included in the character project. The list of missing images is:"), 'warning');
      $this->addMessage($missing_files, 'warning', 'missing-file');
    }
    return TRUE;
  }

  /**
   * import
   *
   * Perform the import. $project_w may be a string
   * (in which case a new project with that title is created) or
   * an existing project, represented as entity metadata wrapper.
   *
   * Return the project metadata wrapper on success, and FALSE on
   * failure.
   */
  public function import($project_w){
    // Ensure the file is valid
    if(!$this->validateFile()){return FALSE;}
    // Create the project
    if(is_string($project_w)){
      global $user;
      $project = entity_create('character_editor_project', array(
        'uid' => $user->uid,
        'created' => time(),
        'changed' => time(),
        'type' => 'default_character_project',
        'title' => $project_w,
        'status' => 1
      ));
      $this->project_w = entity_metadata_wrapper('character_editor_project', $project);
    }else{
      $this->project_w = $project_w;
    }
    // Prepare CharacterEditorMapXML Object
    $content = file_get_contents($this->filename);
    $this->dom = simplexml_load_string(preg_replace('/xmlns\s*=\s*["\'][^"\']+["\']/', '', $content));
    $this->map = new ReverseSchemaArrayMapXML($this->dom, 'http://rs.tdwg.org/UBIF/2006/', array(
      'autosave' => FALSE
    ));
    $this->importTerms();
    $this->importSpecimen();
    if(!$this->importMedia()){return FALSE;}
    // XXX We must do this before they can be linked to other values. We should delete them if the import later fails!
    $this->map->saveEntities();
    if(!$this->importGroups()){return FALSE;}
    if(!$this->importCharacters()){return FALSE;}
    // We made it this far without failure - we can save all the entities before we start creating relations
    if(!$this->project_w->getIdentifier()){
      $project_entity = $this->project_w->raw();
      $project_entity->no_default_group = TRUE;
      $this->project_w->save();
    }
    $this->map->saveEntities();
    if($this->options['import_selection'] != 'data'){
      $this->importStructure();
    }
    if($this->options['import_selection'] != 'structure'){
      if(!$this->importData()){return FALSE;}
      $this->map->saveEntities();
    }
    return $this->project_w;
  }

  /**
   * importTerms
   *
   * Import the taxonomy terms, and associate them with the project
   */
  private function importTerms(){
    // Load taxonomy terms and map of SDD ids to tids. We know this will work
    // as that test is done in validateFile already.
    $available_vids = array_keys(variable_get('biological_vids', array()));
    $this->map->mapXML(reverse_schema_array('sdd_taxonomy_term', REVERSE_SCHEMA_ARRAY_XGET, array(
      'available_vids' => $available_vids
    )));
    // These messages are already included from running validateFile, so flush them out.
    $this->map->flushMessages();
    $this->map->flushInfo();
    // Assign taxonomy terms to the project. This is both part of the structure and the data, so we do
    // it regardless of import_selection
    $existing_tids = array();
    foreach($this->project_w->field_taxonomic_name as $term_w){
      $existing_tids[] = $term_w->getIdentifier();
    }
    foreach($this->map->getEntities('taxonomy_term') as $term_w){
      if(!in_array($term_w->getIdentifier(), $existing_tids)){
        $this->project_w->field_taxonomic_name[] = $term_w->raw();
      }
    }
  }

  /**
   * importSpecimen
   *
   * Import the specimen, and associate them with the project
   */
  private function importSpecimen(){
    // Load specimen and map of SDD ids to nids. We know this will work
    // as that test is done in validateFile already.
    $this->map->mapXML(reverse_schema_array('sdd_specimen', REVERSE_SCHEMA_ARRAY_XGET));
    // These messages are already included from running validateFile, so flush them out.
    $this->map->flushMessages();
    $this->map->flushInfo();
    // Assign specimen to the project. This is both part of the structure and the data, so we do
    // it regardless of import_selection
    $existing_nids = array();
    foreach($this->project_w->field_char_proj_specimen as $node_w){
      $existing_nids[] = $node_w->getIdentifier();
    }
    foreach($this->map->getEntities('node') as $node_w){
      if($node_w->getBundle() == 'specimen_observation'){
        if(!in_array($node_w->getIdentifier(), $existing_nids)){
          $this->project_w->field_char_proj_specimen[] = $node_w->raw();
        }
      }
    }
  }

  /**
   * importMedia
   *
   * Import media items
   */
  private function importMedia(){
    try{
      $this->map->mapXML(reverse_schema_array('sdd_media_object', REVERSE_SCHEMA_ARRAY_GET | REVERSE_SCHEMA_ARRAY_CREATE));
    }
    catch(Exception $e){
      $this->addMessage('The SDD file cannot be imported. Failed to import Media Objects: ' . $e->getMessage(), 'error');
      return FALSE;
    }
    $info = $this->map->getInfo();
    if($info['new'] > 0){
      $this->addMessage(t("Successfully created %count media objects.", array(
        '%count' => $info['new']
      )), 'status');
    }
    if($info['loaded'] > 0){
      $this->addMessage(t("Successfully loaded %count existing media objects.", array(
        '%count' => $info['loaded']
      )), 'status');
    }
    if($info['skipped']){
      $this->addMessage(t("%count media objects could not be mapped to existing media objects and were skipped. Missing items:", array(
        '%count' => $info['skipped']
      )), 'warning');
      $this->addMessage($this->map->getMessages(), 'warning', 'map-media');
    }
    $this->map->flushMessages();
    $this->map->flushInfo();
    return TRUE;
  }

  /**
   * importGroups
   *
   * Import the groups
   */
  private function importGroups(){
    try{
      $this->map->mapXML(reverse_schema_array('sdd_character_group', $this->rsa_mode, array(
        'project_w' => $this->project_w
      )));
    }
    catch(Exception $e){
      $this->addMessage('The SDD file cannot be imported. Failed to import descriptive concepts: ' . $e->getMessage(), 'error');
      return FALSE;
    }
    $info = $this->map->getInfo();
    if($info['new'] > 0){
      $this->addMessage(t("Successfully created %count character groups.", array(
        '%count' => $info['new']
      )), 'status');
    }
    if($info['loaded'] > 0){
      $this->addMessage(t("Successfully loaded %count existing character groups.", array(
        '%count' => $info['loaded']
      )), 'status');
    }
    if($info['skipped']){
      $this->addMessage(t("%count character groups could not be mapped to existing character groups and were skipped. Character belonging to those groups will get associated with the Default group instead. Missing groups:", array(
        '%count' => $info['skipped']
      )), 'warning');
      $this->addMessage($this->map->getMessages(), 'warning', 'map');
    }
    if($info['total'] == 0){
      if($options['reuse'] == 'always'){
        $this->addMessage(t("The SDD file cannot be imported without creating new groups."), 'error');
        return FALSE;
      }else{
        $this->addMessage(t("No groups were imported - will create a default group"), 'warning');
      }
    }
    $this->map->flushMessages();
    $this->map->flushInfo();
    return TRUE;
  }

  /**
   * importCharacters
   */
  private function importCharacters(){
    try{
      $character_types = array(
        'sdd_character_text',
        'sdd_character_numeric',
        'sdd_character_dna',
        'sdd_character_controlled'
      );
      foreach($character_types as $character_type){
        $this->map->mapXML(reverse_schema_array($character_type, $this->rsa_mode, array(
          'project_w' => $this->project_w
        )));
      }
    }
    catch(Exception $e){
      $this->addMessage('The SDD file cannot be imported. Failed to import characters: ' . $e->getMessage(), 'error');
      return FALSE;
    }
    $info = $this->map->getInfo();
    if($info['new'] > 0){
      $this->addMessage(t("Successfully created %count characters.", array(
        '%count' => $info['new']
      )), 'status');
    }
    if($info['loaded'] > 0){
      $this->addMessage(t("Successfully loaded %count existing characters.", array(
        '%count' => $info['loaded']
      )), 'status');
    }
    if($info['skipped']){
      $this->addMessage(t("%count characters could not be mapped to existing characters and were skipped. Missing characters:", array(
        '%count' => $info['skipped']
      )), 'warning');
      $this->addMessage($this->map->getMessages(), 'warning', 'map');
    }
    if($info['total'] == 0){
      $this->addMessage(t("The SDD file cannot be imported without characters."), 'error');
      return FALSE;
    }
    $this->map->flushMessages();
    $this->map->flushInfo();
    return TRUE;
  }

  /**
   * importStructure
   *
   * Create the relations betweens project, groups and characters
   */
  private function importStructure(){
    // Associate all the groups with the project
    $characters_to_associate = array();
    foreach($this->map->getEntities('character_editor_character') as $entity_w){
      if($entity_w->getBundle() == 'group'){
        if(!character_editor_association_exists($this->project_w, $entity_w)){
          character_editor_associate_character($this->project_w, $entity_w);
        }
      }else{
        $characters_to_associate[$entity_w->getIdentifier()] = $entity_w;
      }
    }
    // Associates all the characters with their group
    $inconsistent = FALSE;
    foreach($this->dom->xpath('/Datasets/Dataset/CharacterTrees/CharacterTree/Nodes/CharNode') as $tree_node){
      $character_ref = $this->map->nodeToString($tree_node->xpath('Character/@ref[1]'));
      if(!$character_ref){
        $this->addMessage("Missing character reference in CharacterTree description, the tree will be incomplete.", 'warning', 'tree');
        continue;
      }
      if(count($tree_node->xpath('Parent')) == 0){
        // This will get added to the Default group
        continue;
      }
      // XXX could we have more than one parent here?
      $node_ref = $this->map->nodeToString($tree_node->xpath('Parent/@ref[1]'));
      if(!$node_ref){
        $this->addMessage("Missing parent node reference in CharacterTree description, the tree will be incomplete. Character $character_ref will be associated with default group instead", 'warning', 'tree');
        continue;
      }
      $group_ref = $this->map->nodeToString($tree_node->xpath('../Node[@id=\'' . $node_ref . '\']/DescriptiveConcept/@ref'));
      if(!$group_ref){
        $this->addMessage("Missing group reference in CharacterTree description, the tree will be incomplete. Character $character_ref will be associated with default group instead", 'warning', 'tree');
        continue;
      }
      $group_w = $this->map->getEntity('character_editor_character', $group_ref);
      $character_w = $this->map->getEntity('character_editor_character', $character_ref);
      if(!$character_w){
        $this->addMessage("Could not find Character with reference $character_ref in Character Tree, skipping.", 'warning', 'tree');
      }
      if(!$group_w){
        $this->addMessage("Could not find group with reference $group_ref in CharacterTree, the tree will be incomplete. Character $character_ref will be associated with the default group instead", 'warning', 'tree');
        continue;
      }
      unset($characters_to_associate[$character_w->getIdentifier()]);
      if(!character_editor_association_exists($group_w, $character_w)){
        character_editor_associate_character($group_w, $character_w);
      }
    }
    // Associate remaining characters with the default group.
    if(!empty($characters_to_associate)){
      // Fetch/create the default group
      $default_group_w = NULL;
      $groups = character_editor_get_characters($this->project_w);
      foreach($groups as $project_group_w){
        if($project_group_w->title == 'Default'){
          $default_group_w = $project_group_w;
          break;
        }
      }
      if(!$default_group_w){
        $default_group = entity_create('character_editor_character', array(
          'type' => 'group',
          'title' => 'Default'
        ));
        $default_group_w = entity_metadata_wrapper('character_editor_character', $default_group);
        $default_group_w->save();
        character_editor_associate_character($this->project_w, $default_group_w);
      }
      // Associate all the characters with the default group
      foreach($characters_to_associate as $entity_w){
        character_editor_associate_character($default_group_w, $entity_w);
      }
    }
  }

  /**
   * importData
   */
  private function importData(){
    try{
      $character_data_types = array(
        'sdd_character_data_numeric',
        'sdd_character_data_controlled',
        'sdd_character_data_text',
        'sdd_character_data_dna'
      );
      $variables = array(
        array(),
        array(
          'sample' => true
        )
      );
      // XXX we need to update the data if it exists !!!
      foreach($variables as $vars){
        foreach($character_data_types as $character_data_type){
          $this->map->mapXML(reverse_schema_array($character_data_type, REVERSE_SCHEMA_ARRAY_GET | REVERSE_SCHEMA_ARRAY_CREATE, $vars));
        }
      }
    }
    catch(Exception $e){
      $this->addMessage('The SDD file cannot be imported. Failed to import character data ' . $e->getMessage(), 'error');
      return FALSE;
    }
    $info = $this->map->getInfo();
    if($info['updated'] > 0){
      $this->addMessage(t("Successfully updated %count coded descriptions.", array(
        '%count' => $info['saved']
      )), 'status');
    }
    if($info['new'] > 0){
      $this->addMessage(t("Successfully created %count new coded descriptions.", array(
        '%count' => $info['saved']
      )), 'status');
    }
    if($info['skipped']){
      $this->addMessage(t("%count coded descriptions were not imported:", array(
        '%count' => $info['skipped']
      )), 'warning');
      $this->addMessage($this->map->getMessages(), 'warning');
    }
    $this->map->flushMessages();
    $this->map->flushInfo();
    return TRUE;
  }

  /**
   * getMessages
   *
   * Return the messages
   */
  public function getMessages(){
    return $this->messages;
  }

  /**
   * addMessage
   *
   * Adds a message (or array of messages). Ensures that at most $options['message_count']
   * messages of a given type are added at once.
   */
  private function addMessage($message, $status = 'status', $type = 'general', $prefix = '&rarr; '){
    if(is_array($message)){
      foreach($message as $msg){
        $this->addMessage($prefix . $msg, $status, $type);
      }
    }else{
      if(isset($this->_last_message_type) && $this->_last_message_type == $status . $type){
        if($this->last_message_count >= $this->options['message_count']){
          if($this->last_messagE_count == $this->options['message_count']){
            $last_pos = count($this->messages);
          }else{
            $last_pos = count($this->messages) - 1;
          }
          $this->messages[$last_pos] = array(
            t("... %count more messages ...", array(
              '%count' => $this->last_message_count
            )),
            $status
          );
          $this->_last_message_count++;
          return;
        }
      }else{
        $this->_last_message_type = $status . $type;
        $this->_last_message_count = 1;
      }
      $this->messages[] = array(
        $message,
        $status
      );
    }
  }
}
