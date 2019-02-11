<?php

/**
 * Implements hook_drush_command().
 */
function eolapi_drush_command()
{
  $commands = array();
  $commands['eolapi-delete-objects'] = array(
    'description' => 'Delete all EOL API Objects.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap at all.
    'aliases' => array('eold'),
  );
  return $commands;
}

/**
 * Drush remote-import-scratchpad command
 * Takes a list of sites to import
 */
function drush_eolapi_delete_objects()
{
  // $entities = entity_load('eolapi');

  $ids = db_select('eolapi', 'e')->fields('e', array(
    'eid'
  ))->range(0, 200)->execute()->fetchCol();



  $count = count($ids);

  if ($count == 0) {
    drush_print("There are $count EOL API objects.");
  } else if (drush_confirm("This will remove all $count EOL API objects. Are you sure you want to continue?")) {
    // foreach ($entities as $entity) {
    //   $lang = field_language('eolapi', $entity, 'eolapi_image');
      // if (@isset($eolapi->eolapi_image[$lang][0]['fid'])) {
      //   $file = file_load($eolapi->eolapi_image[$lang][0]['fid']);
      // }


    // }



    // foreach ($ids as $id) {
    //   $e = entity_load_single('eolapi', $id);
    //   var_dump($e);
    //   $lang = field_language('eolapi', $e, 'eolapi_image');
    //   if (is_array($e->eolapi_image[$lang])) {
    //     foreach ($e->eolapi_image[$lang] as $f) {
    //       var_dump($f);

    //     }
    //   }

    // }

    entity_delete_multiple('eolapi', $ids);

    drush_print("$count EOL API objects deleted");
  } else {
    drush_user_abort();
  }

}
