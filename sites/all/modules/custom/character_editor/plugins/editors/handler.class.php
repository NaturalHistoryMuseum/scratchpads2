<?php

/**
 * Base class for export UI.
 */
class character_editor_handler extends slickgrid_editors{

  function get_result(){
    // Clear the message queue
    drupal_get_messages();
    // Add our own messages
    if($count_updated = count($this->updated)){
      drupal_set_message(format_plural($count_updated, 'Character was updated.', '@count characters were updated.'));
    }
    if($count_errors = count($this->errors)){
      $message = format_plural($count_errors, 'Update failed: there was an error', 'Update failed: There were @count errors.');
      $message .= theme('item_list', array(
        'items' => $this->errors
      ));
      drupal_set_message($message, 'error');
    }
    // Return array of data to be returned to the grid
    return array(
      'errors' => $this->errors,
      'updated' => $this->updated,
      'field_id' => $this->field_id,
      'op' => 'update'
    );
  }
}

/**
 * character_metadata_editor_handler
 *
 * Special handler for character metadata
 */
class character_metadata_editor_handler{

  /**
   * __construct
   */
  function __construct($definition){
    $this->errors = array();
    $this->metadata = array();
    if(isset($_POST['flag'])){
      $this->metadata['flag'] = $_POST['flag'];
    }
    if(isset($_POST['aggregate'])){
      $this->metadata['aggregate'] = $_POST['aggregate'];
    }
    if(isset($_POST['send_down'])){
      $this->metadata['sendDown'] = $_POST['send_down'];
    }
    // Get the character
    if(isset($_POST['column_id']) && preg_match('/^character_(\d+)_(\d+)$/', $_POST['column_id'], $matches)){
      $character_id = $matches[1];
      $relation_id = $matches[2]; // Relation to parent character
      $this->character_w = character_editor_wrapper('character_editor_character', $character_id);
      if(!$this->character_w){
        $this->errors[] = t('Could not load character.');
      }
    }else{
      $this->errors[] = t('No column defined ; could not update character flag');
    }
    // Get the entity
    if(isset($_POST['entity_ids'])){
      $this->entity_ids = $_POST['entity_ids'];
      $this->wrappers = array();
      foreach($this->entity_ids as $id){
        $this->wrappers[$id] = character_editor_wrapper($id);
        if(empty($this->wrappers[$id])){
          $this->errors[] = t('Could not load entity %entity_id', array(
            '%entity_id' => $id
          ));
        }
      }
    }else{
      $this->errors[] = t('No entities defined');
    }
  }

  /**
   * update
   */
  function update(){
    foreach($this->wrappers as $entity_w){
      if(!$entity_w){
        continue;
      }
      $r = character_editor_set_character_value($this->character_w, $entity_w, NULL, $this->metadata);
      if(!$r){
        $this->errors[] = t("Could not update or save the value for entity %entity", array(
          '%entity' => $entity_w->label()
        ));
      }
    }
    return $this->get_result();
  }

  /**
   * get_result
   */
  function get_result(){
    // Clear the message queue
    drupal_get_messages();
    $count_errors = count($this->errors);
    if($count_errors){
      $message = format_plural($count_errors, 'Update failed: there was an error', 'Update failed: There were @count errors.');
      $message .= theme('item_list', array(
        'items' => $this->errors
      ));
      drupal_set_message($message, 'error');
    }
    // Return array of data to be returned to the grid
    return array(
      'errors' => $this->errors,
      'updated' => $count_errors == 0 ? array(
        $this->entity_id
      ) : array(),
      'field_id' => $this->column_id,
      'op' => 'update'
    );
  }
}

/**
 * character_entity_editor_handler
 *
 * Special handler for handling character entities
 */
class character_entity_editor_handler{

  /**
   * __construct
   */
  function __construct($definition){
    $this->errors = array();
    $this->metadata = array();
    if(isset($_POST['modal_character_id'])){
      $this->character_id = preg_replace('/^(?:character_)?(\d+)(?:_\d+)?$/', '$1', $_POST['modal_character_id']);
    }else{
      $this->errors[] = t('Missing character id');
    }
  }

  /**
   * update
   */
  function update(){
    if($this->errors){return array(
        'errors' => $this->errors
      );}
    $character_w = character_editor_wrapper('character_editor_character', $this->character_id);
    $form_state = array(
      'build_info' => array(
        'args' => array(
          $character_w->raw()
        )
      ),
      'ajax' => TRUE,
      'modal' => 'CharacterEntity',
      're_render' => FALSE,
      'no_redirect' => TRUE
    );
    $form = drupal_build_form('entity_admin_entity_form', $form_state);
    if(!$form_state['executed'] || $form_state['rebuild']){
      print ajax_render(ctools_modal_form_render($form_state, $form));
      exit();
    }else{
      $output = array();
      $output[] = ctools_modal_command_dismiss();
      $project_w = character_editor_get_character_project($character_w);
      $args = array(
        $project_w->getIdentifier()
      );
      $result = array();
      $result = module_invoke_all('slickgrid_add_entity', 'character_editor_character', $character_w->getBundle(), $args, $character_w->raw());
      slickgrid_callback_add_messages($result);
      $output[] = array(
        'command' => 'slickgrid',
        'response' => array(
          'result' => array_merge(array(
            'data' => array(
              'length' => 0
            )
          ), $result)
        )
      );
      print ajax_render($output);
      exit();
    }
  }
}
