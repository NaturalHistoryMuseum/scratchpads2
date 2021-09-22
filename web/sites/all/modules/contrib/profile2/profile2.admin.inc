<?php

/**
 * @file
 * Profile type editing UI.
 */

/**
 * UI controller.
 */
class Profile2TypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage profiles, including fields.';
    return $items;
  }
}

/**
 * Generates the profile type editing form.
 */
function profile2_type_form($form, &$form_state, $profile_type, $op = 'edit') {

  if ($op == 'clone') {
    $profile_type->label .= ' (cloned)';
    $profile_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $profile_type->label,
    '#description' => t('The human-readable name of this profile type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($profile_type->type) ? $profile_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $profile_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'profile2_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show during user account registration.'),
    '#default_value' => !empty($profile_type->data['registration']),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#weight' => 40,
  );

  if (!$profile_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('profile2_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function profile2_type_form_submit(&$form, &$form_state) {
  $profile_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $profile_type->save();
  $form_state['redirect'] = 'admin/structure/profiles';
}

/**
 * Form API submit callback for the delete button.
 */
function profile2_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/profiles/manage/' . $form_state['profile2_type']->type . '/delete';
}
