<?php

/**
 * Returns a form to the browser which can then be submitted - avoiding CSRF!
 */
function slickgrid_get_form($form_id){
  switch($form_id){
    case 'slickgrid_clone_form':
    case 'slickgrid_delete_form':
      $form = drupal_get_form($form_id, $_POST['entity_type'], $_POST['entity_ids']);
      break;
    case 'slickgrid_settings_form':
      $form = drupal_get_form($form_id, $_POST['view'], $_POST['setting'], $_POST['display_id'], $_POST['value']);
      break;
    case 'slickgrid_undo_form':
      $form = drupal_get_form($form_id, $_POST['entity_type'], $_POST['command']);
      break;
  }
  $form['#prefix'] = '<div style="position:absolute;z-index:10000;border:solid 1px #ccc;padding:10px;background-color:white;">';
  $form['#suffix'] = '</div>';
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_invoke('.' . $form_id, 'slickgrid_add_form', array(
        drupal_render($form)
      ))
    )
  );
}

/**
 * Settings form.
 */
function slickgrid_settings_form($form, &$form_state, $view, $setting, $display_id, $value){
  // The whole form can be hidden, all we need to do is submit it via JS.
  global $user;
  return array(
    'uid' => array(
      '#type' => 'hidden',
      '#value' => $user->uid
    ),
    'view' => array(
      '#type' => 'hidden',
      '#value' => $view
    ),
    'setting' => array(
      '#type' => 'hidden',
      '#value' => $setting
    ),
    'display_id' => array(
      '#type' => 'hidden',
      '#value' => $display_id
    ),
    'value' => array(
      '#type' => 'hidden',
      '#value' => $value
    ),
    'actions' => array(
      'do' => array(
        '#type' => 'submit',
        '#value' => t('Save settings'),
        '#ajax' => array(
          'callback' => 'slickgrid_return_commands_from_form'
        )
      )
    )
  );
}

/**
 * Submit function for the deletion of entities.
 */
function slickgrid_settings_form_submit($form, &$form_state){
  // do not save settings for anonymous users
  if($form_state['values']['uid']){
    slickgrid_set_settings($form_state['values']['uid'], $form_state['values']['view'], $form_state['values']['display_id'], array(
      $form_state['values']['setting'] => $form_state['values']['value']
    ));
  }
}

/**
 * Clone entities form.
 */
function slickgrid_clone_form($form, $form_state, $entity_type, $entity_ids){
  $form = slickgrid_delete_form($form, $form_state, $entity_type, $entity_ids);
  $form['description_text']['#markup'] = '<p>' . format_plural(count($entity_ids), t('Do you want to clone the selected row'), t('Do you want to clone the @num selected rows', array(
    '@num' => count($entity_ids)
  ))) . '</p>';
  $form['actions']['do']['#value'] = t('Clone');
  return $form;
}

/**
 * Submit function for the clone of entities.
 */
function slickgrid_clone_form_submit($form, &$form_state){
  $commands = array();
  if($form_state['values']['op'] == $form_state['values']['do']){
    //entity_delete_multiple($form_state['values']['entity_type'], $form_state['values']['entity_ids']);
    module_load_include('ui.inc', 'entity', 'includes/entity');
    $entities_to_clone = entity_load($form_state['values']['entity_type'], $form_state['values']['entity_ids']);
    foreach($entities_to_clone as $entity_to_clone){
      $new_entity = _slickgrid_clone_entity($form_state['values']['entity_type'], $entity_to_clone);
      entity_save($form_state['values']['entity_type'], $new_entity);
    }
    $commands[] = ajax_command_invoke('', 'slickgrid_messages', array(
      array(
        array(
          'type' => 'status',
          'message' => format_plural(count($form_state['values']['entity_ids']), t('Cloned the selected row'), t('Cloned the @num selected rows', array(
            '@num' => count($form_state['values']['entity_ids'])
          )))
        )
      )
    ));
    $commands[] = ajax_command_invoke('', 'slickgrid_refresh');
  }
  $commands[] = ajax_command_remove('#' . $form_state['values']['form_id'] . ' ~ div');
  $form_state['slickgrid_commands'] = $commands;
}

/**
 * 
 */
function slickgrid_return_commands_from_form($form, $form_state){
  return array(
    '#type' => 'ajax',
    '#commands' => $form_state['slickgrid_commands']
  );
}

/**
 * Clones the entity object and makes sure it will get saved as new entity.
 *
 * This is a fixed version of "entity_ui_clone_entity"
 */
function _slickgrid_clone_entity($entity_type, $entity){
  // Clone the entity and make sure it will get saved as a new entity.
  $entity = clone $entity;
  $entity_info = entity_get_info($entity_type);
  $entity->{$entity_info['entity keys']['id']} = FALSE;
  if(!empty($entity_info['entity keys']['name'])){
    $entity->{$entity_info['entity keys']['name']} = FALSE;
  }
  if(!empty($entity_info['entity keys']['revision'])){
    $entity->{$entity_info['entity keys']['revision']} = FALSE;
  }
  if(!empty($entity_info['entity keys']['uuid'])){
    $entity->{$entity_info['entity keys']['uuid']} = FALSE;
  }
  if(!empty($entity_info['entity keys']['vuuid'])){
    $entity->{$entity_info['entity keys']['vuuid']} = FALSE;
  }
  $entity->is_new = TRUE;
  // Make sure the status of a cloned exportable is custom.
  if(!empty($entity_info['exportable'])){
    $status_key = isset($entity_info['entity keys']['status']) ? $entity_info['entity keys']['status'] : 'status';
    $entity->$status_key = ENTITY_CUSTOM;
  }
  return $entity;
}

/**
 * Delete entities form.
 */
function slickgrid_delete_form($form, $form_state, $entity_type, $entity_ids){
  return array(
    'entity_type' => array(
      '#type' => 'hidden',
      '#value' => $entity_type
    ),
    'entity_ids' => array(
      '#type' => 'hidden',
      '#value' => $entity_ids
    ),
    'description_text' => array(
      '#markup' => '<p>' . format_plural(count($entity_ids), t('Do you want to delete the selected row'), t('Do you want to delete the @num selected rows', array(
        '@num' => count($entity_ids)
      ))) . '</p>'
    ),
    'actions' => array(
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#ajax' => array(
          // We need an empty callback, as all the logic is handled in the submit
          // function.
          'callback' => 'slickgrid_return_commands_from_form'
        )
      ),
      'do' => array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#ajax' => array(
          'callback' => 'slickgrid_return_commands_from_form'
        )
      )
    )
  );
}

/**
 * Submit function for the deletion of entities.
 */
function slickgrid_delete_form_submit($form, &$form_state){
  $commands = array();
  if($form_state['values']['op'] == $form_state['values']['do']){
    entity_delete_multiple($form_state['values']['entity_type'], $form_state['values']['entity_ids']);
    $commands[] = ajax_command_invoke('', 'slickgrid_messages', array(
      array(
        array(
          'type' => 'status',
          'message' => format_plural(count($form_state['values']['entity_ids']), t('Deleted the selected row'), t('Delete the @num selected rows', array(
            '@num' => count($form_state['values']['entity_ids'])
          )))
        )
      )
    ));
    $commands[] = ajax_command_invoke('', 'slickgrid_refresh');
  }
  $commands[] = ajax_command_remove('#' . $form_state['values']['form_id'] . ' ~ div');
  $form_state['slickgrid_commands'] = $commands;
}

/**
 * Undo form.
 */
function slickgrid_undo_form($form, &$form_state, $entity_type, $command){
  $keys = array_keys($command);
  $id = array_pop($keys);
  $entity = entity_load_single($entity_type, $id);
  $info = entity_get_info('node');
  return array(
    'entity_type' => array(
      '#type' => 'hidden',
      '#value' => $entity_type
    ),
    'entity_id' => array(
      '#type' => 'hidden',
      '#value' => $id
    ),
    'revision_id' => array(
      '#type' => 'hidden',
      '#value' => $command[$id]
    ),
    'description_text' => array(
      '#markup' => '<p>' . t('Do you want to undo the latest change to @entity_type %entity_label?', array(
        '@entity_type' => $entity_type,
        '%entity_label' => $entity->{$info['entity keys']['label']}
      )) . '</p>'
    ),
    'actions' => array(
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#ajax' => array(
          // We need an empty callback, as all the logic is handled in the submit
          // function.
          'callback' => 'slickgrid_return_commands_from_form'
        )
      ),
      'do' => array(
        '#type' => 'submit',
        '#value' => t('Undo'),
        '#ajax' => array(
          'callback' => 'slickgrid_return_commands_from_form'
        )
      )
    )
  );
}

/**
 * Submit function for the deletion of entities.
 */
function slickgrid_undo_form_submit($form, &$form_state){
  $commands = array();
  if($form_state['values']['op'] == $form_state['values']['do']){
    // I'm not happy about this, but we're hard coding entity types for undo
    // here.  A generic solution needs to be implemented.
    switch($form_state['values']['entity_type']){
      case 'node':
        // We get the highest vid for this node which is earlier than the
        // current one.
        $node_revision = node_load($form_state['values']['entity_id'], $form_state['values']['revision_id']['vid']);
        $node_revision->revision = 1;
        $node_revision->log = t('Copy of the revision from %date.', array(
          '%date' => format_date($node_revision->revision_timestamp)
        ));
        node_save($node_revision);
    }
    $commands[] = ajax_command_invoke('', 'slickgrid_undo_pop');
    $commands[] = ajax_command_invoke('', 'slickgrid_messages', array(
      array(
        array(
          'type' => 'status',
          'message' => t('Undone')
        )
      )
    ));
    $commands[] = ajax_command_invoke('', 'slickgrid_refresh');
  }
  $commands[] = ajax_command_remove('#' . $form_state['values']['form_id'] . ' ~ div');
  $form_state['slickgrid_commands'] = $commands;
}