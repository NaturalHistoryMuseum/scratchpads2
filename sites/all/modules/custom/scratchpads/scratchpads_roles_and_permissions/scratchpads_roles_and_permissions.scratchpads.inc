<?php

/**
 * Implements hook_scratchpads_default_permissions_alter().
 * 
 * Simply enforces inheritance, so that editor's have all the contributor permissions,
 * and maintainers have all the editor permissions.
 */
function scratchpads_roles_and_permissions_scratchpads_default_permissions_alter(&$permissions){
  $permissions['authenticated user'] = array_merge($permissions['anonymous user'], $permissions['authenticated user']);
  $permissions['contributor'] = array_merge($permissions['authenticated user'], $permissions['contributor']);
  $permissions['editor'] = array_merge($permissions['contributor'], $permissions['editor']);
  $permissions['maintainer'] = array_merge($permissions['editor'], $permissions['maintainer']);
}

/**
 * Implements hook_scratchpads_default_permissions().
 */
function scratchpads_roles_and_permissions_scratchpads_default_permissions(){
  return array(
    'anonymous user' => array(),
    'authenticated user' => array(),
    'contributor' => array(),
    'editor' => array(),
    'maintainer' => array()
  );
}

/**
 * Biblio module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function biblio_scratchpads_default_permissions(){
  return array(
    'contributor' => array(
      'import from file'
    )
  );
}

/**
 * Node module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function node_scratchpads_default_permissions(){
  // Missing permissions:
  // 'bypass node access'
  // 'administer content types'
  // 'administer nodes'
  $permissions = array(
    'anonymous user' => array(
      'access content',
      'view revisions'
    ),
    'authenticated user' => array(
      'view own unpublished content'
    ),
    'contributor' => array(
      'access content overview'
    ),
    'editor' => array(
      'revert revisions',
      'delete revisions'
    ),
    'maintainer' => array()
  );
  foreach(node_permissions_get_configured_types() as $type){
    $permissions['contributor'][] = "create $type content";
    $permissions['contributor'][] = "edit own $type content";
    $permissions['contributor'][] = "delete own $type content";
    $permissions['editor'][] = "edit any $type content";
    $permissions['editor'][] = "delete any $type content";
  }
  return $permissions;
}