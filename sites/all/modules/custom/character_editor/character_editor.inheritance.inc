<?php

/**
 * character_editor_process_inheritance
 * 
 * Inheritance page callback
 */
function character_editor_process_inheritance($project){
  $project_model = new CharacterProjectModel($project);
  $tree = $project_model->getInheritanceTree();
  $batch = array(
    'operations' => array(
      array(
        'character_editor_process_inheritance_up',
        array(
          $tree,
          $project->id
        )
      ),
      array(
        'character_editor_process_inheritance_down',
        array(
          $tree,
          $project->id
        )
      )
    ),
    'finished' => 'character_editor_process_inheritance_finished',
    'title' => t('Evaluating inheritance'),
    'init_message' => t('Inheritance evaluation is starting'),
    'progress_message' => t('@percentage'),
    'error_message' => t('Failed to evaluate the inheritance values, please try again'),
    'file' => drupal_get_path('module', 'character_editor') . '/character_editor.inheritance.inc'
  );
  batch_set($batch);
  if(isset($_COOKIE['character_editor_url'])){
    $url = parse_url($_COOKIE['character_editor_url']);
    $redirect = array(
      substr($url['path'], 1),
      array(
        'fragment' => $url['fragment']
      )
    );
    unset($_COOKIE['character_editor_url']);
  }else{
    $redirect = 'character-editor-project/' . $project->id . '/edit';
  }
  batch_process($redirect);
}

/**
 * character_editor_process_inheritance_up
 * 
 * Batch step to calculate upwards inheritance
 */
function character_editor_process_inheritance_up($tree, $project_id, &$context){
  if(!isset($context['sandbox']['up'])){
    $context['sandbox']['up'] = $tree['up'];
    $context['message'] = t('Processing up inheritance');
  }
  if(empty($context['sandbox']['up'])){return;}
  $character_ids = array_keys($context['sandbox']['up']);
  $character_id = $character_ids[0];
  if(empty($context['sandbox']['up'][$character_id])){
    unset($context['sandbox']['up'][$character_id]);
    return character_editor_process_inheritance_up($tree, $context);
  }
  $tid = array_shift($context['sandbox']['up'][$character_id]);
  if(empty($context['sandbox']['up'][$character_id])){
    unset($context['sandbox']['up'][$character_id]);
  }
  $metadata = array(
    'flag' => 'inherited'
  );
  $model = new CharacterModel('character_editor_character', $character_id);
  $value = $model->getTaxonAggregatedValue($tid, $tree, array(
    $tid
  ));
  $tid_w = character_editor_wrapper('taxonomy_term', $tid);
  character_editor_set_character_value($model->wrapper(), $tid_w, $value, $metadata);
  if(!empty($context['sandbox']['up'])){
    $context['finished'] = count($context['sandbox']['up'], COUNT_RECURSIVE) / count($tree['up'], COUNT_RECURSIVE);
  }
}

/**
 * character_editor_process_inheritance_down
 */
function character_editor_process_inheritance_down($tree, $project_id, &$context){
  // Setup batch operation
  if(!isset($context['sandbox']['down'])){
    $context['sandbox']['down'] = $tree['down'];
    $context['sandbox']['value'] = false;
    $context['sandbox']['entity_ids'] = array();
    $context['sandbox']['character_id'] = false;
    $context['message'] = t('Processing down inheritance');
  }
  // If we don't have entities to process any more, get a new batch.
  if(empty($context['sandbox']['entity_ids'])){
    $character_id = $context['sandbox']['character_id'];
    if(!$character_id || empty($context['sandbox']['down'][$character_id])){
      if($character_id){
        unset($context['sandbox']['down'][$character_id]);
      }
      if(empty($context['sandbox']['down'])){return;}
      $character_ids = array_keys($context['sandbox']['down']);
      $context['sandbox']['character_id'] = $character_ids[0];
      return character_editor_process_inheritance_down($tree, $project_id, $context);
    }
    $tid = array_shift($context['sandbox']['down'][$character_id]);
    if(empty($context['sandbox']['down'][$character_id])){
      unset($context['sandbox']['down'][$character_id]);
    }
    $character_model = new CharacterModel('character_editor_character', $character_id);
    $term_w = character_editor_wrapper('taxonomy_term', $tid);
    $values = character_editor_get_character_values($character_model->wrapper(), $term_w);
    $context['sandbox']['value'] = $values[0]['data'][0]['value'];
    $context['sandbox']['entity_ids'] = $character_model->getPassDownEntities($tid, $project_id, $tree);
    $context['finished'] = count($context['sandbox']['down'], COUNT_RECURSIVE) / count($tree['down'], COUNT_RECURSIVE);
    return;
  }
  // Update the next entity
  $character_w = character_editor_wrapper('character_editor_character', $context['sandbox']['character_id']);
  $entity_w = character_editor_wrapper(array_shift($context['sandbox']['entity_ids']));
  character_editor_set_character_value($character_w, $entity_w, $context['sandbox']['value'], array(
    'flag' => 'inherited'
  ));
  $context['finished'] = count($context['sandbox']['down'], COUNT_RECURSIVE) / count($tree['down'], COUNT_RECURSIVE);
}

/**
 * character_editor_process_inheritance_finished
 */
function character_editor_process_inheritance_finished($success, $results, $operations){
  drupal_set_message(t('Inheritance values updated'));
}
