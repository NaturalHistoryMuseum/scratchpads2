<?php

function taxonomystatistics_statistics_page($vocabulary, $term){
  if(arg(0) == 'taxonomy' && arg(1) == 'term' && ($plugin = context_get_plugin('condition', 'taxonomy_term')) != FALSE){
    $plugin->execute($term, 'view');
  }
  // If we have a term, we probably don't have the vocabulary
  $content = array();
  if($term && !$vocabulary){
    drupal_set_title(t('!taxonomyname statistics', array(
      '!taxonomyname' => theme('scratchpads_species_name', array(
        'term' => $term
      ))
    )), PASS_THROUGH);
    $vocabulary = taxonomy_vocabulary_load($term->vid);
  }else{
    drupal_set_title(t('@vocabularyname statistics', array(
      '@vocabularyname' => $vocabulary->name
    )));
  }
  if($term){
    $content[] = taxonomystatistics_statistics_helper_page($vocabulary, $term);
  }
  $content[] = taxonomystatistics_statistics_helper_page($vocabulary, FALSE);
  return $content;
}

/**
 * Helper function for the above page callback
 */
function taxonomystatistics_statistics_helper_page($vocabulary, $term){
  $tids = FALSE;
  if($term){
    $tids = taxonomystatistics_get_tids_below_tid($term->tid);
    if(!$tids){return array(
        'error' => array(
          '#markup' => '<h2>' . t('%termname statistics', array(
            '%termname' => $term->name
          )) . '</h2><p>' . t('Unable to generate statistics for leaf terms') . '</p>'
        )
      );}
  }
  // We start with the general statistics.
  $query = db_select('taxonomy_term_data', 't')->condition('vid', $vocabulary->vid);
  if($tids){
    $query->condition('t.tid', $tids);
  }
  $total_terms = $query->countQuery()->execute()->fetchField();
  $subquery = db_select('taxonomy_term_hierarchy', 'h')->fields('h', array(
    'parent'
  ));
  $query = db_select('taxonomy_term_data', 't')->condition('vid', $vocabulary->vid)->condition('tid', $subquery, 'NOT IN');
  if($tids){
    $query->condition('t.tid', $tids);
  }
  $leaf_terms = $query->countQuery()->execute()->fetchField();
  // Next the field statistics
  $field_instances = field_info_instances('taxonomy_term', $vocabulary->machine_name);
  $fields = array();
  foreach($field_instances as $field_instance){
    $field = field_info_field($field_instance['field_name']);
    if($field['module'] == 'list'){
      $fields[$field_instance['field_name']] = array(
        'label' => $field_instance['label'],
        'description' => $field_instance['description'],
        'values' => array()
      );
      $query = db_select('field_data_' . $field_instance['field_name'], 'f');
      $query->addExpression('f.' . $field_instance['field_name'] . '_value', 'value');
      $query->addExpression('COUNT(*)', 'count');
      if($tids){
        $query->condition('entity_id', $tids);
      }else{
        $subquery = db_select('taxonomy_term_data', 't')->fields('t', array(
          'tid'
        ))->condition('vid', $vocabulary->vid);
        $query->condition('entity_id', $subquery, 'IN');
      }
      $results = $query->groupBy($field_instance['field_name'] . '_value')->execute();
      while(($row = $results->fetchAssoc()) != FALSE){
        if($row['value'] == ''){
          $row['value'] = t('"BLANK"');
        }
        $row['count'] = (int)$row['count'];
        $row = array_values($row);
        $fields[$field_instance['field_name']]['values'][] = $row;
      }
      if(!$fields[$field_instance['field_name']]['values']){
        unset($fields[$field_instance['field_name']]);
      }
    }
  }
  return array(
    '#theme' => 'taxonomystatistics_page',
    '#name' => ($term ? t('%termname descendants statistics', array(
      '%termname' => $term->name
    )) : t('Overall statistics')),
    '#allterms' => $total_terms,
    '#leafterms' => $leaf_terms,
    '#fields' => $fields
  );
}

/**
 * Simple function to return an array of tids below another tid.
 */
function taxonomystatistics_get_tids_below_tid($parent_tid){
  $tids_to_process = array(
    $parent_tid
  );
  $tids = array();
  while(($tid = array_pop($tids_to_process)) != FALSE){
    $result_tids = db_select('taxonomy_term_hierarchy', 't')->condition('parent', $tid)->fields('t', array(
      'tid'
    ))->execute()->fetchCol();
    $tids_to_process = array_merge($tids_to_process, $result_tids);
    $tids = array_merge($tids, $result_tids);
  }
  return $tids;
}