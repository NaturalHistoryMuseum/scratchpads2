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
 * Contact module
 */
function contact_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer contact forms'
    ),
    'anonymous user' => array(
      'access site-wide contact form',
      'access user contact forms'
    )
  );
}

/**
 * Simplenews module
 * 
 * Implements hook_scratchpads_default_permissions()
 */
function simplenews_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer newsletters',
      'administer simplenews subscriptions',
      'administer simplenews settings',
      'send newsletter'
    ),
    'anonymous user' => array(
      'subscribe to newsletters'
    )
  );
}

/**
 * role_delegation module
 * 
 * Implements hook_scratchpads_default_permissions()
 */
function role_delegation_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'assign all roles'
    )
  );
}

/**
 * Filter module
 * 
 * Implements hook_scratchpads_default_permissions()
 */
function filter_scratchpads_default_permissions(){
  // Missing:
  // administer filters
  return array(
    'authenticated user' => array(
      'use text format filtered_html'
    )
  );
}

/**
 * Redirect module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function redirect_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer redirects'
    )
  );
}

/**
 * Webform module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function webform_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'access all webform results',
      'edit all webform submissions',
      'delete all webform submissions'
    ),
    'editor' => array(
      'delete own webform submissions'
    ),
    'contributor' => array(
      'access own webform results',
      'access own webform submissions',
      'edit own webform submissions'
    )
  );
}

/**
 * Pathauto module
 * 
 * Implements hook_scratchpads_default_permissions()
 */
function pathauto_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer pathauto'
    )
  );
}

/**
 * Lexicon module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function lexicon_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer lexicon'
    ),
    'anonymous user' => array(
      'access lexicon'
    )
  );
}

/**
 * Google analytics module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function googleanalytics_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer google analytics'
    )
  );
}

/**
 * Subpermissions module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function subpermissions_scratchpads_default_permissions(){
  // Missing:
  // administer subpermission
  return array(
    'maintainer' => array(
      'access subpermission form'
    )
  );
}

/**
 * Formflow module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function formflow_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer form flows',
      'create form flows',
      'edit form flows'
    ),
    'anonymous user' => array(
      'access form flows'
    )
  );
}

/**
 * Hashcash module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function hashcash_scratchpads_default_permissions(){
  // Missing:
  // administer hashcash
  return array(
    'authenticated user' => array(
      'submit form without hashcash'
    )
  );
}

/**
 * Femail module
 *
 * Implements hook_scratchpads_default_permissions().
 */
function femail_scratchpads_default_permissions(){
  return array(
    'authenticated user' => array(
      'post by femail'
    )
  );
}

/**
 * Tools module
 *
 * Note, because this is a custom module that may be release to drupal.org, this specific
 * Scratchpad code belongs here.
 *
 * Implements hook_scratchpads_default_permissions().
 */
function tools_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'access tools'
    )
  );
}

/**
 * Views module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function views_scratchpads_default_permissions(){
  return array(
    'maintainer' => array(
      'administer views',
      'access all views'
    )
  );
}

/**
 * User module
 *
 * Implements hook_scratchpads_default_permissions().
 */
function user_scratchpads_default_permissions(){
  // administer permissions
  // select account cancellation method
  return array(
    'maintainer' => array(
      'administer users'
    ),
    'anonymous user' => array(
      'access user profiles'
    ),
    'authenticated user' => array(
      'change own username',
      'cancel account'
    )
  );
}

/**
 * Toolbar module
 *
 * Implements hook_scratchpads_default_permissions().
 */
function toolbar_scratchpads_default_permissions(){
  return array(
    'authenticated user' => array(
      'access toolbar'
    )
  );
}

/**
 * Taxonomy module
 *
 * Implements hook_scratchpads_default_permissions().
 */
function taxonomy_scratchpads_default_permissions(){
  // Reset the vocabulary cache.
  taxonomy_vocabulary_static_reset();
  $permissions = array(
    'maintainer' => array(
      'administer taxonomy'
    ),
    'editor' => array()
  );
  foreach(taxonomy_get_vocabularies() as $vocabulary){
    $permissions['editor'][] = 'edit terms in ' . $vocabulary->vid;
    $permissions['editor'][] = 'delete terms in ' . $vocabulary->vid;
  }
  return $permissions;
}

/**
 * System module
 *
 * Implements hook_scratchpads_default_permissions().
 */
function system_scratchpads_default_permissions(){
  // administer modules
  // block IP addresses
  // access site reports
  // administer software updates
  // administer actions
  return array(
    'maintainer' => array(
      'administer site configuration',
      'administer themes',
      'access administration pages',
      'access site in maintenance mode'
    ),
    'authenticated user' => array(
      'view the administration theme'
    )
  );
}

/**
 * Search module
 * 
 * Implements hook_scratchpads_default_permissions().
 */
function shortcut_scratchpads_default_permissions(){
  // switch shortcut sets
  return array(
    'maintainer' => array(
      'administer shortcuts'
    ),
    'contributor' => array(
      'customize shortcut links'
    )
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
    'anonymous user' => array(
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
  // Reset ctools cache.
  drupal_static_reset('ctools_export_load_object_all');
  drupal_static_reset('ctools_export_load_object');
  $perms = array(
    'maintainer' => array(
      'administer feeds'
    ),
    'editor' => array(),
    'contributor' => array()
  );
  foreach(feeds_importer_load_all(TRUE) as $importer){
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
    'maintainer' => array(
      'administer content types',
      'administer nodes',
      'bypass node access'
    )
  );
  // Rebuild the node types
  _node_types_build(TRUE);
  foreach(node_permissions_get_configured_types() as $type){
    $permissions['contributor'][] = "create $type content";
    $permissions['contributor'][] = "edit own $type content";
    $permissions['contributor'][] = "delete own $type content";
    $permissions['editor'][] = "edit any $type content";
    $permissions['editor'][] = "delete any $type content";
  }
  return $permissions;
}
