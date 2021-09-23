<?php

/**
 * Implements hook_scratchpads_default_permissions_alter().
 * 
 * Simply enforces inheritance, so that editor's have all the contributor permissions,
 * and maintainers have all the editor permissions.
 */
function scratchpads_group_scratchpads_default_permissions_alter(&$permissions){
  // We remove the group content type from contributors and editors, to ensure
  // that only maintainers can add groups on a site.
  $group_permissions = array(
    'create group content',
    'edit own group content',
    'delete own group content',
    'edit any group content',
    'delete any group content'
  );
  // Remove the permissions from all users.
  foreach($permissions as $role => $role_permissions){
    foreach($role_permissions as $key => $permission){
      if(in_array($permission, $group_permissions)){
        unset($permissions[$role][$key]);
      }
    }
  }
  // Add the permissions to the maintainer.
  foreach($group_permissions as $permission){
    $permissions['maintainer'][] = $permission;
    $permissions['editor'][] = $permission;
  }
}