<?php

/**
 * Change the aliases defined for use by the entityfilter module.
 *
 * @param aliases
 *   An array with the values being the entity_type, and the keys being the 
 *   alias for that entity_type.
 */
function hook_entityfilter_entity_aliases_alter(&$aliases){
  $aliases['actual'] = 'actual_long_entity_type_for_aliasing';
}