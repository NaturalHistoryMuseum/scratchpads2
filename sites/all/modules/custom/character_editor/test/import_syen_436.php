<?php
/**
 * Drush script to import syen_436_* files into a new character project
 */

// Read and parse input
$matrix_text = file_get_contents(drupal_get_path('module', 'character_editor') . '/test/syen_436_sm-matrix.txt');
if (empty($matrix_text)){
  echo "Failed to read matrix input\n";
  return;
}
$matrix = array();
foreach (explode("\n", $matrix_text) as $matrix_line){
  if (empty($matrix_line)){
    continue;
  }
  if (preg_match('/^(\S+)\s+([-0-9?]+)\s*$/', $matrix_line, $matches)){
    $taxon = trim(str_replace('_', ' ', $matches[1]));
    $matrix[$taxon] = str_split($matches[2]);
  } else {
    echo "Failed to parse matrix line $matrix_line\n";
    return;
  }
}
if (empty($matrix)){
  echo "No matrix data found\n";
  return;
}
$character_text = file_get_contents(drupal_get_path('module', 'character_editor') . '/test/syen_436_sm-characters.txt');
if (empty($character_text)){
  echo "Failed to read character definition\n";
  return;
}
$characters = array();
foreach (explode("\n", $character_text) as $char_line){
  if (preg_match('/^\d+ - (.*?) (\(0\).+)[;.]$/', $char_line, $matches)){
    $description = $matches[1];
    $abbr = make_char_abbr($description);
    $states = array();
    foreach (preg_split('/\(\d+\)/', $matches[2]) as $state){
      $state = trim($state);
      if (!empty($state)){
        $states[] = array(
          'name' => $state
        );
      }
    }
    if (empty($states)){
      echo "Failed to parse states for line $char_line\n";
      return;
    }
    $characters[] = array(
      'abbr' => $abbr,
      'description' => $description,
      'states' => $states
    );
  } else {
    echo "Failed to parse character line $char_line\n";
    return;
  }
}
if (empty($characters)){
  echo "No character data found\n";
  return;
}

// Get/create the vocabulary for the taxons
$vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'cimicomorpha'")->fetchField();
if (!$vid){
  taxonomy_vocabulary_save((object)(array(
    'name' => 'Cimicomorpha',
    'machine_name' => 'cimicomorpha',
    'show_synonym_data' => FALSE,
    'biological_classification' => 1,
    'use_scratchpads_species_pages' => 1,
  )));
  $vid = db_query("SELECT vid FROM {taxonomy_vocabulary} WHERE machine_name = 'cimicomorpha'")->fetchField();
  if (!$vid){
    echo "Failed to create vocabulary\n";
    return;
  }
  $biological_vids = variable_get('biological_vids', array());
  $biological_vids[$vid] = 1;
  variable_set('biological_vids', $biological_vids);
}
// Get/create the taxons
$tids = array();
foreach ($matrix as $taxon => $row){
  $tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name", array(
    ':name' => $taxon
  ))->fetchField();
  if (!$tid){
    taxonomy_term_save((object)(array(
    'vid' => $vid,
    'name' => $taxon
    )));
    $tid = db_query("SELECT tid FROM {taxonomy_term_data} WHERE name = :name", array(
      ':name' => $taxon
    ))->fetchField();
    if (!$tid){
      echo "Failed to create taxon $taxon\n";
      return;
    }
  }
  $tids[$taxon] = $tid;
}

// Create a new character project
global $user;
$project = entity_create('character_editor_project', array(
  'uid' => $user->uid,
  'created' => time(),
  'changed' => time(),
  'type' => 'default_character_project',
  'title' => 'Cimicomorpha morphology',
  'status' => 1
));
foreach($tids as $tid){
  $project->field_taxonomic_name['und'][] = array(
    'tid' => $tid
  );
}
$project_w = entity_metadata_wrapper('character_editor_project', $project);
$project_w->save();
// Create characters
foreach ($characters as $cid => $char_desc){
  $character = entity_create('character_editor_character', array(
    'uid' => $user->uid,
    'created' => time(),
    'changed' => time(),
    'type' => 'controlled'
  ));
  $character_w = entity_metadata_wrapper('character_editor_character', $character);
  $character_w->title = $char_desc['abbr'];
  $character_w->field_char_and_or = 'OR';
  $character_w->field_char_ordered = 0;
  $character_w->field_char_description->value = $char_desc['description'];
  foreach ($char_desc['states'] as $sid => $state_desc){
    $state = entity_create('field_collection_item', array(
      'field_name' => 'field_char_states'
    ));
    $state->field_char_state_label = array(
      'und' => array(
        array(
          'value' => $state_desc['name']
        )
      )
    );
    $state->setHostEntity('character_editor_character', $character_w->raw());
    $state->save(TRUE);
    $characters[$cid]['states'][$sid]['sid'] = $state->item_id;
  }
  $character_w->save();
  $characters[$cid]['w'] = $character_w;
  character_editor_associate_character($project_w, $character_w);
}

// Now import values!
foreach ($matrix as $taxon => $row){
  $taxon_entity = taxonomy_term_load($tids[$taxon]);
  $taxon_w = entity_metadata_wrapper('taxonomy_term', $taxon_entity);
  foreach (array_slice($row, 0, 5) as $idx => $sidx){
    $character_w = $characters[$idx]['w'];
    $data = "'" . $characters[$idx]['states'][$sidx]['sid'] . "'";
    character_editor_set_character_value($character_w, $taxon_w, $data);
  }
}

/**
 * make_char_abbr
 * 
 * Create an abbreviation from a character description
 */
function make_char_abbr($d){
  if (strlen($d) <= 5){
    return $d;
  }
  $words = explode(' ', $d);
  $out = '';
  foreach (array_slice($words, 0, 3) as $w){
    if (strlen($w) > 3){
      $out .= substr($w, 0, 2) . '.';
    }
  }
  if (empty($out)){
    // Relax our restrictions
    foreach ($words as $w){
      $out .= substr($w, 0, 2) . '.';
    }
  }
  return $out;
}
