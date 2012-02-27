<?php

/**
 * Allows a module to stipulate the default permissions that should be set on
 * all Scratchpads.  Each module should ideally only return permissions that
 * they define in a hook_permissions function, but for simplicity it is
 * possible to ignore this constraint.
 * 
 * The returned array follow the convention:
 * 
 * return array(
 * '{role_name}' => array(
 * '{permission}',
 * ...
 * ) 
 * );
 */
function hook_scratchpads_default_permissions(){
  return array(
    'contributor' => array(
      'create content'
    )
  );
}

/**
 * Similar to the above, but allows a module to alter permissions after they
 * have all been set.
 */
function hook_scratchpads_default_permissions_alter(&$permissions){}