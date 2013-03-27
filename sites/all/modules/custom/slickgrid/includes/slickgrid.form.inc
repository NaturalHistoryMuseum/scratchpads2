<?php

/**
 * Returns a form to the browser which can then be submitted - avoiding CSRF!
 */
function slickgrid_get_form($form_id){
  $form = drupal_get_form($form_id, $_POST['entity_type'], $_POST['entity_ids']);
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
 * Clone entities form.
 */
function slickgrid_clone_form($form, $form_state, $entity_type, $entity_ids){
  $form = slickgrid_delete_form($form, $form_state, $entity_type, $entity_ids);
  $form['description_text']['#markup'] = '<p>' . format_plural(count($entity_ids), t('Do you want to clone the selected row'), t('Do you want to clone the @num selected rows', array(
    '@num' => count($entity_ids)
  ))) . '</p>';
  $form['actions']['cancel']['#ajax']['callback'] = 'slickgrid_clone_form_submit';
  $form['actions']['do']['#ajax']['callback'] = 'slickgrid_clone_form_submit';
  $form['actions']['do']['#value'] = t('Clone');
  return $form;
}

/**
 * Submit function for the clone of entities.
 */
function slickgrid_clone_form_submit($form, &$form_state){
  $commands = array();
  if($form_state['values']['op'] == $form_state['values']['do']){
    // FIXME - Actually do the clone here.
    // FIXME - Change this from an alter to something a little nicer.
    $commands[] = ajax_command_alert(t('Cloned the selected rows'));
  }
  $commands[] = ajax_command_remove('#' . $form_state['values']['form_id'] . ' ~ div');
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
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
          'callback' => 'slickgrid_delete_form_submit'
        )
      ),
      'do' => array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#ajax' => array(
          'callback' => 'slickgrid_delete_form_submit'
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
    // FIXME - Actually do the delete here.
    // FIXME - Change this from an alter to something a little nicer.
    $commands[] = ajax_command_alert(t('Deleted the selected rows'));
  }
  $commands[] = ajax_command_remove('#' . $form_state['values']['form_id'] . ' ~ div');
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
  /*
//Callback function - delete a node
function slickgrid_callback_delete(){
  foreach($_POST['entity_ids'] as $entity_id){
    entity_delete($_POST['entity_type'], $entity_id);
  }
  drupal_set_message(format_plural(count($_POST['entity_ids']), '1 item was deleted.', '@count items were deleted.'));
  return array(
    'deleted' => $_POST['entity_ids']
  );
}*/
}
