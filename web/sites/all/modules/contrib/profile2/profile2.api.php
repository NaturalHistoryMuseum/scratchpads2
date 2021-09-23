<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
* Act on profiles being loaded from the database.
*
* This hook is invoked during profile loading, which is handled by
* entity_load(), via the EntityCRUDController.
*
* @param $entities
*   An array of profile2 entities being loaded, keyed by id.
*
* @see hook_entity_load()
*/
function hook_profile2_load($entities) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
* Respond when a profile is inserted.
*
* This hook is invoked after the profile is inserted into the database.
*
* @param profile
*   The profile that is being inserted.
*
* @see hook_entity_insert()
*/
function hook_profile2_insert($profile) {
  db_insert('mytable')
    ->fields(array(
      'pid' => $profile->pid,
      'extra' => $profile->extra,
    ))
    ->execute();
}

/**
* Act on a profile being inserted or updated.
*
* This hook is invoked before the profile is saved to the database.
*
* @param $profile
*   The profile that is being inserted or updated.
*
* @see hook_entity_presave()
*/
function hook_profile2_presave($profile) {
  $profile->extra = 'foo';
}

/**
* Respond to a profile being updated.
*
* This hook is invoked after the profile has been updated in the database.
*
* @param $profile
*   The $profile that is being updated.
*
* @see hook_entity_update()
*/
function hook_profile2_update($profile) {
  db_update('mytable')
    ->fields(array('extra' => $profile->extra))
    ->condition('pid', $profile->pid)
    ->execute();
}

/**
* Respond to profile deletion.
*
* This hook is invoked after the profile has been removed from the database.
*
* @param $profile
*   The profile that is being deleted.
*
* @see hook_entity_delete()
*/
function hook_profile2_delete($profile) {
  db_delete('mytable')
    ->condition('pid', $profile->pid)
    ->execute();
}

/**
* Act on a profile that is being assembled before rendering.
*
* @param $profile
*   The profile entity.
* @param $view_mode
*   The view mode the profile is rendered in.
* @param $langcode
*   The language code used for rendering.
*
* The module may add elements to $profile->content prior to rendering. The
* structure of $profile->content is a renderable array as expected by
* drupal_render().
*
* @see hook_entity_prepare_view()
* @see hook_entity_view()
*/
function hook_profile2_view($profile, $view_mode, $langcode) {
  $profile->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
* Alter the results of entity_view() for profiles.
*
* @param $build
*   A renderable array representing the profile content.
*
* This hook is called after the content has been assembled in a structured
* array and may be used for doing processing which requires that the complete
* profile content structure has been built.
*
* If the module wishes to act on the rendered HTML of the profile rather than
* the structured content array, it may use this hook to add a #post_render
* callback. Alternatively, it could also implement hook_preprocess_profile2().
* See drupal_render() and theme() documentation respectively for details.
*
* @see hook_entity_view_alter()
*/
function hook_profile2_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Act on profile type being loaded from the database.
 *
 * This hook is invoked during profile type loading, which is handled by
 * entity_load(), via the EntityCRUDController.
 *
 * @param $types
 *   An array of profiles being loaded, keyed by profile type names.
 */
function hook_profile2_type_load($types) {
  if (isset($types['main'])) {
    $types['main']->userCategory = FALSE;
    $types['main']->userView = FALSE;
  }
}

/**
 * Respond when a profile type is inserted.
 *
 * This hook is invoked after the profile type is inserted into the database.
 *
 * @param $type
 *   The profile type that is being inserted.
 */
function hook_profile2_type_insert($type) {
  db_insert('mytable')
    ->fields(array(
      'id' => $type->id,
      'extra' => $type->extra,
    ))
    ->execute();
}

/**
 * Act on a profile type being inserted or updated.
 *
 * This hook is invoked before the profile type is saved to the database.
 *
 * @param $type
 *   The profile type that is being inserted or updated.
 */
function hook_profile2_type_presave($type) {
  $type->extra = 'foo';
}

/**
 * Respond to updates to a profile.
 *
 * This hook is invoked after the profile type has been updated in the database.
 *
 * @param $type
 *   The profile type that is being updated.
 */
function hook_profile2_type_update($type) {
  db_update('mytable')
    ->fields(array('extra' => $type->extra))
    ->condition('id', $type->id)
    ->execute();
}

/**
 * Respond to profile type deletion.
 *
 * This hook is invoked after the profile type has been removed from the
 * database.
 *
 * @param $type
 *   The profile type that is being deleted.
 */
function hook_profile2_type_delete($type) {
  db_delete('mytable')
    ->condition('id', $type->id)
    ->execute();
}

/**
 * Define default profile type configurations.
 *
 * @return
 *   An array of default profile types, keyed by profile type names.
 */
function hook_default_profile2_type() {
  $types['main'] = new ProfileType(array(
      'type' => 'main',
      'label' => t('Profile'),
      'weight' => 0,
      'locked' => TRUE,
  ));
  return $types;
}

/**
* Alter default profile type configurations.
*
* @param $defaults
*   An array of default profile types, keyed by type names.
*
* @see hook_default_profile2_type()
*/
function hook_default_profile2_type_alter(&$defaults) {
  $defaults['main']->label = 'custom label';
}

/**
 * Alter profile2 forms.
 *
 * Modules may alter the profile2 entity form regardless to which form it is
 * attached by making use of this hook or the profile type specifiy
 * hook_form_profile2_edit_PROFILE_TYPE_form_alter(). #entity_builders may be
 * used in order to copy the values of added form elements to the entity, just
 * as described by entity_form_submit_build_entity().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 *
 * @see profile2_attach_form()
 */
function hook_form_profile2_form_alter(&$form, &$form_state) {
  // Your alterations.
}

/**
 * Control access to profiles.
 *
 * Modules may implement this hook if they want to have a say in whether or not
 * a given user has access to perform a given operation on a profile.
 *
 * @param $op
 *   The operation being performed. One of 'view', 'edit' (being the same as
 *   'create' or 'update') and 'delete'.
 * @param $profile
 *   (optional) A profile to check access for. If nothing is given, access for
 *   all profiles is determined.
 * @param $account
 *   (optional) The user to check for. If no account is passed, access is
 *   determined for the global user.
 * @return boolean
 *   Return TRUE to grant access, FALSE to explicitly deny access. Return NULL
 *   or nothing to not affect the operation.
 *   Access is granted as soon as a module grants access and no one denies
 *   access. Thus if no module explicitly grants access, access will be denied.
 *
 * @see profile2_access()
 */
function hook_profile2_access($op, $profile = NULL, $account = NULL) {
  if (isset($profile)) {
    // Explicitly deny access for a 'secret' profile type.
    if ($profile->type == 'secret' && !user_access('custom permission')) {
      return FALSE;
    }
    // For profiles other than the default profile grant access.
    if ($profile->type != 'main' && user_access('custom permission')) {
      return TRUE;
    }
    // In other cases do not alter access.
  }
}

/**
 * @}
 */
