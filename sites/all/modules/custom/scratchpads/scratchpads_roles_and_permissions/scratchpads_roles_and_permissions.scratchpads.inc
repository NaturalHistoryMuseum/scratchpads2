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
 * Search module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function search_scratchpads_default_permissions(){
  // administer search
  // use advanced search
  return array(
    'anonymous user' => array(
      'search content'
    )
  );
}

/**
 * Overlay module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function overlay_scratchpads_default_permissions(){
  return array(
    'authenticated user' => array(
      'access overlay'
    )
  );
}

/**
 * Clone module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function clone_scratchpads_default_permissions(){
  return array(
    'contributor' => array(
      'clone node',
      'clone own nodes'
    )
  );
}

/**
 * Menu module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function menu_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer menu'
    )
  );
}

/**
 * File entity module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function file_entity_scratchpads_default_permissions(){
  return array(
    'anonymous user' => array(
      'view file'
    ),
    'editor' => array(
      'administer files',
      'edit file'
    )
  );
}

/**
 * Field group module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function field_group_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer fieldgroups'
    )
  );
}

/**
 * Feeds module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function feeds_scratchpads_default_permissions(){
  $perms = array(
    'maintainer' => array(
      'administer feeds'
    ),
    'editor' => array(),
    'contributor' => array()
  );
  foreach (feeds_importer_load_all() as $importer) {
    $perms['contributor'][] = "import $importer->id feeds";
    $perms['editor'][] = "clear $importer->id feeds";
  }
  return $perms;
}

/**
 * Contextual module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function contextual_scratchpads_default_permissions(){
  return array(
    'anonymous user' => array(
      'access contextual links'
    )
  );
}

/**
 * Dashboard module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function dashboard_scratchpads_default_permissions(){
  return array(
    'contributor' => array(
      'access dashboard'
    )
  );
}

/**
 * Comment module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function comment_scratchpads_default_permissions(){
  return array(
    'anonymous user' => array(
      'access comments',
      'post comments'
    ),
    'authenticated user' => array(
      'skip comment approval'
    ),
    'editor' => array(
      'edit own comments',
      'administer comments'
    )
  );
}

/**
 * Biblio module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function biblio_scratchpads_default_permissions(){
  // Missing permissions:
  // administer biblio
  return array(
    'anonymous user' => array(
      'access biblio content',
      'show export links',
      'show download links',
      'show own download links',
      'show filter tab',
      'show sort links',
      'view full text'
    ),
    'contributor' => array(
      'import from file',
      'edit biblio authors',
      'edit by all biblio authors'
    )
  );
}

/**
 * Block module.
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function block_scratchpads_default_permissions(){
  return array(
    'editor' => array(
      'administer blocks'
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
