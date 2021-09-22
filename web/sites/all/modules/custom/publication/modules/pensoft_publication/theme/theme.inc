<?php

/**
 * Preprocess field groups for pensoft_publication publications
 */
function pensoft_publication_preprocess_publication_field_group(&$variables){
  $element = $variables['element'];
  $variables['element_id'] = $element['#id'];
  // Group label
  $variables['label'] = $element['#group']->label;
  // Sort authors and institutions
  if($element['#id'] == 'publication_pensoft_full_group_publication_auth_contrib'){
    // Find authors and institutions
    $authors = array();
    $institutions = array();
    foreach(element_children($element['field_publication_authors']) as $key){
      $collection = reset($element['field_publication_authors'][$key]['entity']['field_collection_item']);
      if(!empty($collection['field_publication_author_user'][0])){
        $author = $collection['field_publication_author_user'][0];
        $authors[] = $author;
        if(!empty($author['#publication_author_institution'])){
          $institutions[] = $author['#publication_author_institution'];
        }
      }
    }
    // Sort institutions
    $institutions = array_unique($institutions);
    foreach($authors as $key => $author){
      $authors[$key] = htmlspecialchars($author['#publication_author_name']);
      if(!empty($author['#publication_author_institution'])){
        $inst = array_search($author['#publication_author_institution'], $institutions);
        $authors[$key] .= ' <sup>' . ($inst + 1) . '</sup>';
      }
    }
    foreach($institutions as $key => $inst){
      $institutions[$key] = '<div class="field">' . ($key + 1) . ' ' . $inst . '</div>';
    }
    // Find contributors
    $contributors = array();
    foreach(element_children($element['field_publication_contributors']) as $key){
      $collection = reset($element['field_publication_contributors'][$key]['entity']['field_collection_item']);
      $contributor = $collection['field_publication_cbtor_user'][0]['#publication_author_name'];
      $role = $collection['field_publication_cbtor_role'][0]['#markup'];
      $contributors[$role] = $contributor;
    }
    // Set template variables
    $variables['authors'] = $authors;
    $variables['institutions'] = $institutions;
    $variables['contributors'] = $contributors;
    if(empty($authors) && empty($contributors)){
      $variables['skip_group'] = TRUE;
    }
  }
  // Remove empty figures/tables
  if($element['#id'] == 'publication_pensoft_full_group_pensoft_files'){
    $count = 0;
    foreach(element_children($element['field_publication_figures']) as $fig_key){
      if(!isset($element['field_publication_figures'][$fig_key]['entity'])){
        unset($variables['element']['field_publication_figures'][$fig_key]);
      } else {
        $count++;
      }
    }
    foreach(element_children($element['field_publication_tables']) as $table_key){
      if(!isset($element['field_publication_tables'][$table_key]['entity'])){
        unset($variables['element']['field_publication_tables'][$table_key]);
      } else {
        $count++;
      }
    }
    if ($count == 0) {
      $variables['skip_group'] = TRUE;
    }
  }
}