<?php

/**
 * UI controller.
 */
class PublicationTypeUIController extends EntityDefaultUIController{

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu(){
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage publications.';
    return $items;
  }
}

/**
 * Generates the publication type editing form.
 */
function publication_type_form($form, &$form_state, $publication_type, $op = 'edit'){
  if($op == 'clone'){
    $publication_type->label .= ' (cloned)';
    $publication_type->type = '';
  }
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => isset($publication_type->label) ? $publication_type->label : '',
    '#description' => t('The human-readable name of this publication type.'),
    '#required' => TRUE,
    '#size' => 30
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($publication_type->type) ? $publication_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'publication_get_types',
      'source' => array(
        'label'
      )
    ),
    '#description' => t('A unique machine-readable name for this publication type. It must only contain lowercase letters, numbers, and underscores.')
  );
  $form['data']['#tree'] = TRUE;
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save publication type')
  );
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/structure/publication_types')
  );
  return $form;
}

function publication_form_cancel(){
  $form_state['redirect'] = 'admin/structure/publication_types';
}

/**
 * Form API submit callback for the type form.
 */
function publication_type_form_submit(&$form, &$form_state){
  $publication_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $publication_type->save();
  $form_state['redirect'] = 'admin/structure/publication_types';
}

/**
 * Form API submit callback for the delete button.
 */
function publication_type_form_submit_delete(&$form, &$form_state){
  $form_state['redirect'] = 'admin/structure/publication_types/manage/' . $form_state['publication_type']->type . '/delete';
}