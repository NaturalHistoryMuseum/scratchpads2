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
    // FIXME - Change this from an alert to something a little nicer.
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
    //entity_delete_multiple($form_state['values']['entity_type'], $form_state['values']['entity_ids']);
    $commands[] = ajax_command_invoke('', 'slickgrid_messages', array(
      array(
        array(
          'type' => 'status',
          'message' => t('Deleted the selected rows')
        )
      )
    ));
    $commands[] = ajax_command_invoke('', 'slickgrid_refresh');
  }
  $commands[] = ajax_command_remove('#' . $form_state['values']['form_id'] . ' ~ div');
  return array(
    '#type' => 'ajax',
    '#commands' => $commands
  );
}
