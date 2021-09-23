<?php


/**
 * @file
 * Hooks provided by the Organic groups module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Add group permissions.
 */
function hook_og_permission() {
  return array(
    'subscribe' => array(
      'title' => t('Subscribe user to group'),
      'description' => t("Allow user to be a member of a group (approval required)."),
      'roles' => array(OG_ANONYMOUS_ROLE),
    ),
  );
}

/**
 * Alter the organic groups permissions.
 *
 * @param $perms
 *   The permissions passed by reference.
 */
function hook_og_permission_alter(&$perms) {

}


/**
 * Set a default role that will be used as a global role.
 *
 * A global role, is a role that is assigned by default to all new groups.
 */
function hook_og_default_roles() {
  return array('super admin');
}

/**
 * Alter the default roles.
 *
 * The anonymous and authenticated member roles are not alterable.
 *
 * @param $roles
 *   Array with the default roles name.
 */
function hook_og_default_roles_alter(&$roles) {
  // Remove a default role.
  unset($roles['super admin']);
}

/**
 * Allow modules to act upon new group role.
 *
 * @param $role
 *   The group role object.
 */
function hook_og_role_insert($role) {
}

/**
 * Allow modules to act upon existing group role update.
 *
 * @param $role
 *   The group role object.
 */
function hook_og_role_update($role) {

}

/**
 * Allow modules to act upon existing group role deletion.
 *
 * @param $role
 *   The deleted group role object. The object is actually a dummy, as the data
 *   is already deleted from the database. However, we pass the object to allow
 *   implementing modules to properly identify the deleted role.
 */
function hook_og_role_delete($role) {

}


function hook_og_role_grant($gid, $uid, $rid) {

}

function hook_og_role_revoke($gid, $uid, $rid) {

}

/**
 * Provide information about fields that are related to Organic groups.
 *
 * Using this info, Organic groups is aware of the fields, and allows adding
 * them to the correct bundle.
 *
 * - type: Array with the values "group" and/ or "group content". To define to
 *   which bundles the field may be attached.
 * - Description: The description of the field.
 * - field: The field info array as will be passed to field_create_field().
 * - instance: The field instance array as will be passed to
 *   field_info_instance().
 * - entity type: Optional; Array of the entity types this field can be attached
 *   to. The field will not be attachable to other entity types. Defaults to
 *   empty array.
 * - disable on node translate: Optional; If set to TRUE then on translated
 *   node, the field will be un-editable, and a message will be shown that the
 *   field can be only edited via the source node. Defaults to TRUE.
 */
function hook_og_fields_info() {
  $items = array();
  $items[OG_GROUP_FIELD] = array(
    'type' => array('group'),
    'description' => t('Determine if this should be a group.'),
    'field' => array(
      'field_name' => OG_GROUP_FIELD,
      'no_ui' => TRUE,
      'type' => 'list_boolean',
      'cardinality' => 1,
      'settings' => array(
        'allowed_values' => array(0 => 'Not a group type', 1 => 'Group type'),
        'allowed_values_function' => '',
      ),
    ),
    'instance' => array(
      'label' => t('Group type'),
      'widget_type' => 'options_select',
      'required' => TRUE,
      // Make the group type default.
      'default_value' => array(0 => array('value' => 1)),
      'view modes' => array(
        'full' => array(
          'label' => t('Full'),
          'type' => 'og_group_subscribe',
          'custom settings' => FALSE,
        ),
        'teaser' => array(
          'label' => t('Teaser'),
          'type' => 'og_group_subscribe',
          'custom settings' => FALSE,
        ),
      ),
    ),
  );
  return $items;
}

/**
 * TODO
 */
function hook_og_fields_info_alter(&$fields_info) {

}

/**
 * Act upon organic groups cache clearing.
 *
 * This can be used by implementing modules, that need to clear the cache
 * as-well.
 */
function hook_og_invalidate_cache($gids = array()) {
  $caches = array(
    'og_foo',
    'og_bar',
  );

  foreach ($caches as $cache) {
    drupal_static_reset($cache);
  }
}

/**
 * Alter the permissions of a user in a group.
 *
 * @param $perm
 *   The permissions of a user, passed by reference.
 * @param $context
 *   Array with:
 *   - string: The permission asked for the user.
 *   - group: The group object.
 *   - account: The user account.
 */
function hook_og_user_access_alter(&$perm, $context) {
  // If user ID 2 doesn't already have a permission then enable it.
  if (empty($perm['foo']) && $context['account']->uid = 2) {
    $perm['foo'] = TRUE;
  }
}

/**
 * Alter the groups audience fields options.
 *
 * @param $options
 *   All the groups in the site divided into the "content groups" array and
 *   "other groups" array.
 * @param $opt_group
 *   TRUE if the user should see also "other groups" options.
 * @param $account
 *   The user object for which the field is built.
 */
function hook_og_audience_options_alter(&$options, &$opt_group, $account) {
  // Hide every group from the user.
  if ($account->uid == 5) {
    $options['content groups'] = array();
  }
}

/**
* Acts on OG groups being loaded from the database.
*
* This hook is invoked during OG group loading, which is handled by
* entity_load(), via the EntityCRUDController.
*
* @param array og_groups
*   An array of OG group entities being loaded, keyed by id.
*
* @see hook_entity_load()
*/
function hook_group_load(array $og_groups) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
* Responds when a OG group is inserted.
*
* This hook is invoked after the OG group is inserted into the database.
*
* @param OgGroup $og_group
*   The OG group that is being inserted.
*
* @see hook_entity_insert()
*/
function hook_group_insert(OgGroup $og_group) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('group', $og_group),
      'extra' => print_r($og_group, TRUE),
    ))
    ->execute();
}

/**
* Acts on a OG group being inserted or updated.
*
* This hook is invoked before the OG group is saved to the database.
*
* @param OgGroup $og_group
*   The OG group that is being inserted or updated.
*
* @see hook_entity_presave()
*/
function hook_group_presave(OgGroup $og_group) {
  $og_group->name = 'foo';
}

/**
* Responds to a OG group being updated.
*
* This hook is invoked after the OG group has been updated in the database.
*
* @param OgGroup $og_group
*   The OG group that is being updated.
*
* @see hook_entity_update()
*/
function hook_group_update(OgGroup $og_group) {
  db_update('mytable')
    ->fields(array('extra' => print_r($og_group, TRUE)))
    ->condition('id', entity_id('group', $og_group))
    ->execute();
}

/**
* Responds to OG group deletion.
*
* This hook is invoked after the OG group has been removed from the database.
*
* @param OgGroup $og_group
*   The OG group that is being deleted.
*
* @see hook_entity_delete()
*/
function hook_group_delete(OgGroup $og_group) {
  db_delete('mytable')
    ->condition('pid', entity_id('group', $og_group))
    ->execute();
}


/**
* Acts on OG membership types being loaded from the database.
*
* This hook is invoked during OG membership type loading, which is handled by
* entity_load(), via the EntityCRUDController.
*
* @param array $og_membership_types
*   An array of OG membership type entities being loaded, keyed by id.
*
* @see hook_entity_load()
*/
function hook_og_membership_type_load(array $og_membership_types) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
* Responds when a OG membership type is inserted.
*
* This hook is invoked after the OG membership type is inserted into the database.
*
* @param OgMembershipType $og_membership
*   The OG membership type that is being inserted.
*
* @see hook_entity_insert()
*/
function hook_og_membership_type_insert(OgMembershipType $og_membership) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('og_membership_type', $og_membership),
      'extra' => print_r($og_membership, TRUE),
    ))
    ->execute();
}

/**
* Acts on a OG membership type being inserted or updated.
*
* This hook is invoked before the OG membership type is saved to the database.
*
* @param OgMembershipType $og_membership
*   The OG membership type that is being inserted or updated.
*
* @see hook_entity_presave()
*/
function hook_og_membership_type_presave(OgMembershipType $og_membership) {
  $og_membership->name = 'foo';
}

/**
* Responds to a OG membership type being updated.
*
* This hook is invoked after the OG membership type has been updated in the database.
*
* @param OgMembershipType $og_membership
*   The OG membership type that is being updated.
*
* @see hook_entity_update()
*/
function hook_og_membership_type_update(OgMembershipType $og_membership) {
  db_update('mytable')
    ->fields(array('extra' => print_r($og_membership, TRUE)))
    ->condition('id', entity_id('og_membership_type', $og_membership))
    ->execute();
}

/**
* Responds to OG membership type deletion.
*
* This hook is invoked after the OG membership type has been removed from the database.
*
* @param OgMembershipType $og_membership
*   The OG membership type that is being deleted.
*
* @see hook_entity_delete()
*/
function hook_og_membership_type_delete(OgMembershipType $og_membership) {
  db_delete('mytable')
    ->condition('pid', entity_id('og_membership_type', $og_membership))
    ->execute();
}

/**
* Define default OG membership type configurations.
*
* @return
*   An array of default OG membership types, keyed by machine names.
*
* @see hook_default_og_membership_type_alter()
*/
function hook_default_og_membership_type() {
  $defaults['main'] = entity_create('og_membership_type', array(
    // É
  ));
  return $defaults;
}

/**
* Alter default OG membership type configurations.
*
* @param array $defaults
*   An array of default OG membership types, keyed by machine names.
*
* @see hook_default_og_membership_type()
*/
function hook_default_og_membership_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}


/**
* Acts on OG memberships being loaded from the database.
*
* This hook is invoked during OG membership loading, which is handled by
* entity_load(), via the EntityCRUDController.
*
* @param array $og_memberships
*   An array of OG membership entities being loaded, keyed by id.
*
* @see hook_entity_load()
*/
function hook_og_membership_load(array $og_memberships) {
  $result = db_query('SELECT pid, foo FROM {mytable} WHERE pid IN(:ids)', array(':ids' => array_keys($entities)));
  foreach ($result as $record) {
    $entities[$record->pid]->foo = $record->foo;
  }
}

/**
* Responds when a OG membership is inserted.
*
* This hook is invoked after the OG membership is inserted into the database.
*
* @param OgMembership $og_membership
*   The OG membership that is being inserted.
*
* @see hook_entity_insert()
*/
function hook_og_membership_insert(OgMembership $og_membership) {
  db_insert('mytable')
    ->fields(array(
      'id' => entity_id('og_membership', $og_membership),
      'extra' => print_r($og_membership, TRUE),
    ))
    ->execute();
}

/**
* Acts on a OG membership being inserted or updated.
*
* This hook is invoked before the OG membership is saved to the database.
*
* @param OgMembership $og_membership
*   The OG membership that is being inserted or updated.
*
* @see hook_entity_presave()
*/
function hook_og_membership_presave(OgMembership $og_membership) {
  $og_membership->name = 'foo';
}

/**
* Responds to a OG membership being updated.
*
* This hook is invoked after the OG membership has been updated in the database.
*
* @param OgMembership $og_membership
*   The OG membership that is being updated.
*
* @see hook_entity_update()
*/
function hook_og_membership_update(OgMembership $og_membership) {
  db_update('mytable')
    ->fields(array('extra' => print_r($og_membership, TRUE)))
    ->condition('id', entity_id('og_membership', $og_membership))
    ->execute();
}

/**
* Responds to OG membership deletion.
*
* This hook is invoked after the OG membership has been removed from the database.
*
* @param OgMembership $og_membership
*   The OG membership that is being deleted.
*
* @see hook_entity_delete()
*/
function hook_og_membership_delete(OgMembership $og_membership) {
  db_delete('mytable')
    ->condition('pid', entity_id('og_membership', $og_membership))
    ->execute();
}


/**
 * @} End of "addtogroup hooks".
 */