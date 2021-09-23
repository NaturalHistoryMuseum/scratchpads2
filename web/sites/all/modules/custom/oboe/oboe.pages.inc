<?php

/**
 * Menu callback. 
 */
function oboe_add_job(){
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  // Bypass the node/add listing if only one content type is available.
  if(count($content) == 1){
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('node_add_list', array(
    'content' => $content
  ));
}

/**
 * Returns an OBOE submission form.
 */
function oboe_add($type){
  $types = OBOEService::get_job_types();
  global $user;
  $oboe = (object)array(
    'uid' => $user->uid,
    'type' => $type
  );
  drupal_set_title(t('Create @name job', array(
    '@name' => $types[$type]
  )), PASS_THROUGH);
  $output = drupal_get_form($type . '_oboe_form', $oboe);
  return $output;
}

/**
 * Confirm delete an oboe entity.
 */
function oboe_delete_confirm($form, &$form_state, $oboe){
  $form['#oboe'] = $oboe;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['oid'] = array(
    '#type' => 'value',
    '#value' => $oboe->oid
  );
  return confirm_form($form, t('Are you sure you want to delete %title?', array(
    '%title' => $oboe->label
  )), 'oboe/' . $oboe->oid, t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Execute oboe deletion
 */
function oboe_delete_confirm_submit($form, &$form_state){
  if($form_state['values']['confirm']){
    oboe_delete($form_state['values']['oid']);
  }
  $form_state['redirect'] = '<front>';
}

/**
 * Callback defined in hook_forms().
 */
function oboe_job_form($form, &$form_state, $oboe){
  // Firstly, we need to get the parameters for this oboe type.
  $parameters = OBOEService::get_parameters_by_type($oboe->type);
  foreach($parameters as $key => $parameter){
    if(isset($parameter['format']) && (!isset($parameter['hidden']) || $parameter['hidden'] == 'no')){
      $format = $parameter['format'];
      $colon_position = strpos($parameter['format'], ':');
      if($colon_position){
        $format = substr($format, 0, $colon_position);
      }
      switch($format){
        case 'URL':
        case 'text':
        case 'integer':
          // Check for "map".
          if($parameter['name'] == 'Coordinates'){
            module_load_include('theme.inc', 'gm3');
            $form['map'] = gm3_get_map(array(
                'id' => $key,
                'libraries' => array(
                  'polygon' => array(),
                  'rectangle' => array(
                    'rectangles' => array()
                  ),
                  'field_rectangle' => array(
                    'module' => 'gm3_field'
                  )
                ),
                'tools' => array(
                  theme('gm3_rectangle_button', array(
                    'id' => $key
                  ))
                )
            ));
            $form[$key] = array(
              '#type' => 'textfield',
              '#title' => isset($parameter['name']) ? $parameter['name'] : $key,
              '#description' => isset($parameter['description']) ? $parameter['description'] : '',
              '#required' => ((isset($parameter['required']) && $parameter['required'] == 'yes') ? TRUE : FALSE),
              '#attributes' => array(
                'class' => array(
                  $key . '-rectangle'
                )
              ),
              '#maxlength' => 1024
            );
          }else{
            $form[$key] = array(
              '#type' => 'textfield',
              '#title' => isset($parameter['name']) ? $parameter['name'] : $key,
              '#description' => isset($parameter['description']) ? $parameter['description'] : '',
              '#required' => ((isset($parameter['required']) && $parameter['required'] == 'yes') ? TRUE : FALSE)
            );
          }
          break;
        case 'boolean':
          $options = array();
          if(isset($parameter['required']) && $parameter['required'] == 'no'){
            $options[] = 'Use default';
          }
          $options['yes'] = t('Yes');
          $options['no'] = t('No');
          $form[$key] = array(
            '#type' => 'radios',
            '#options' => $options,
            '#title' => isset($parameter['name']) ? $parameter['name'] : $key,
            '#description' => isset($parameter['description']) ? $parameter['description'] : '',
            '#required' => ((isset($parameter['required']) && $parameter['required'] == 'yes') ? TRUE : FALSE),
            '#default_value' => '0'
          );
          break;
        case 'array':
          $array_data = json_decode(trim(str_replace("'", '"', substr($parameter['format'], $colon_position + 1))));
          $array_data = drupal_map_assoc($array_data);
          $form[$key] = array(
            '#type' => 'select',
            '#title' => isset($parameter['name']) ? $parameter['name'] : $key,
            '#description' => isset($parameter['description']) ? $parameter['description'] : '',
            '#required' => ((isset($parameter['required']) && $parameter['required'] == 'yes') ? TRUE : FALSE),
            '#empty_option' => 'Select: ' . (isset($parameter['name']) ? $parameter['name'] : $key),
            '#options' => $array_data,
            '#default_value' => count($array_data) == 1 ? current($array_data) : ''
          );
          break;
        case 'free text':
          $form[$key] = array(
            '#type' => 'textarea',
            '#title' => isset($parameter['name']) ? $parameter['name'] : $key,
            '#description' => isset($parameter['description']) ? $parameter['description'] : '',
            '#required' => ((isset($parameter['required']) && $parameter['required'] == 'yes') ? TRUE : FALSE)
          );
          break;
        default:
          drupal_set_message('A field type that we do not understand.  Please contact the Scratchpad team.');
          break;
      }
    }else{
      switch($key){
        case 'infile':
          if($parameter['required'] == 'yes' || $parameter['required'] == 'optional'){
            $form[$key] = array(
              '#type' => 'file',
              '#title' => t('Input file'),
              '#description' => isset($parameter['description']) ? $parameter['description'] : '',
              '#required' => ((isset($parameter['required']) && $parameter['required'] == 'yes') ? TRUE : FALSE)
            );
          }
          break;
        default:
          if($parameter['required'] == 'yes'){
            // We have a hidden field that is required. WTF!
            drupal_set_message('Hidden required field.  Please contact the Scratchpad team.');
          }
      }
    }
  }
  $form['job_name'] = array(
    '#type' => 'hidden',
    '#value' => $oboe->type . '_' . time()
  );
  $form['job_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description')
  );
  $form['#oboe'] = $oboe;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create')
  );
  return $form;
}

/**
 * Submit for the above function
 */
function oboe_job_form_submit(&$form, &$form_state){
  try{
    $oboe_service = new OBOEService();
    $oboe_service->set_type($form['#oboe']->type);
    $parameters = $form_state['values'];
    unset($parameters['op'], $parameters['form_id'], $parameters['form_token'], $parameters['form_build_id'], $parameters['submit']);
    foreach($parameters as $key => $value){
      if($value == ''){
        unset($parameters[$key]);
      }
    }
    $oboe_service->set_parameters($parameters);
    if($oboe_service->run()){
      // The job has run correctly, we can create the entity for this job.
      $oboe = $form['#oboe'];
      $oboe->label = $oboe_service->get_job_name();
      $oboe->description = $oboe_service->get_job_description();
      $oboe->data = $oboe_service->get_response();
      $oboe->created = time();
      $oboe->changed = time();
      entity_save('oboe', $oboe);
      $form_state['redirect'] = 'oboe/' . $oboe->oid;
    }else{
      drupal_set_message('Your submission failed.  Please try again later.  If you continue to experience problems, then please consult the user support.', 'error');
    }
  }
  catch(OBOEServiceException $e){
    watchdog('oboe_error', print_r($e, 1));
    drupal_set_message('Your submission failed.  Please try again later.  If you continue to experience problems, then please consult the user support.', 'error');
  }
}