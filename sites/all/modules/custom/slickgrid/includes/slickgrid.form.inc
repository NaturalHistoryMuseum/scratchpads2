<?php

/**
 * Returns a form to the browser which can then be submitted - avoiding CSRF!
 */
function slickgrid_get_form($form_id){
  $form = drupal_get_form($form_id, $_GET['entity_type'], $_GET['entity_ids']);
  $form['#prefix'] = '<div style="position:absolute; top: 0; left: 0; z-index:10000">';
  $form['#suffix'] = '</div>';
  // FIXME - Add selector so that the form can be associated with the button 
  // that was clicked.
  return array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_invoke('', 'slickgrid_add_form', array(
        drupal_render($form)
      ))
    )
  );
}

/**
 * Delete entities form.
 */
function slickgrid_delete_form($form, $form_state, $entity_type = 'node', $entity_ids = array(1)){
  return array(
    'entity_type' => array(
      '#type' => 'hidden',
      '#value' => $entity_type
    ),
    'entity_ids' => array(
      '#type' => 'hidden',
      '#value' => $entity_ids
    ),
    'something' => array(
      '#markup' => '<p>' . t('Do you want to delete the @num selected @rows', array(
        '@num' => count($entity_ids),
        '@rows' => format_plural(count($entity_ids), t('row'), t('rows'))
      )) . '</p>'
    ),
    'actions' => array(
      'cancel' => array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#ajax' => array(
          'callback' => 'slickgrid_delete_form_submit'
        )
      ),
      'delete' => array(
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
  echo "FUCK";exit;
}
