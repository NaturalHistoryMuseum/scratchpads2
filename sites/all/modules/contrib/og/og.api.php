<?php


/**
 * @file
 * Hooks provided by the Organic groups module.
 */

/**
 * @addtgrouproup hooks
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
 * @} End of "addtgrouproup hooks".
 */