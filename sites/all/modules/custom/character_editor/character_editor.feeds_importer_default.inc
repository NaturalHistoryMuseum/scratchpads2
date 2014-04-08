<?php

function character_editor_feeds_importer_default(){
  $projects = db_select('character_editor_project', 'c')->fields('c', array(
    'id',
    'title'
  ))->execute()->fetchAllKeyed();
  $export = array();
  foreach($projects as $id => $project){
    $feeds_importer = new stdClass();
    $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
    $feeds_importer->api_version = 1;
    $feeds_importer->id = 'character_editor_import_' . $id;
    $feeds_importer->config = array(
      'name' => t('Import into: Character Editor "@title"', array(
        '@title' => $project
      )),
      'description' => t('Import data into an existing character project.'),
      'fetcher' => array(
        'plugin_key' => 'FeedsFileFetcher',
        'config' => array(
          'allowed_extensions' => 'xls xlsx',
          'direct' => 0,
          'directory' => 'private://feeds',
          'allowed_schemes' => array(
            'public' => 'public',
            'private' => 'private'
          )
        )
      ),
      'parser' => array(
        'plugin_key' => 'FeedsExcelParser',
        'config' => array(
          'all_worksheets' => 0,
          'max_rows' => 65535
        )
      ),
      'processor' => array(
        'plugin_key' => 'FeedsCharacterEditorProcessor',
        'config' => array(
          'character_project' => '1',
          'mappings' => array(
            array(
              'source' => 'Taxon or Specimen',
              'target' => 'id',
              'unique' => TRUE
            )
          )
        )
      ),
      'content_type' => '',
      'update' => 0,
      'import_period' => 1800,
      'expire_period' => 3600,
      'import_on_create' => TRUE,
      'process_in_background' => FALSE
    );
    $project_w = character_editor_wrapper('character_editor_project', $id);
    $tree = character_editor_get_tree($project_w);
    foreach($tree as $character){
      if($character['wrapper']->type->value() != 'group'){
        $feeds_importer->config['processor']['config']['mappings'][] = array(
          'source' => $character['wrapper']->title->value(),
          'target' => 'character_' . $character['wrapper']->id->value() . '_',
          'unique' => FALSE
        );
      }
    }
    $export[$feeds_importer->id] = $feeds_importer;
  }
  return $export;
}