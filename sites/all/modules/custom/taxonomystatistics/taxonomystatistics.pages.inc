<?php

function taxonomystatistics_statistics_page($vocabulary){
  // We start with the general statistics.
  $total_terms = db_select('taxonomy_term_data', 't')->condition('vid', $vocabulary->vid)->countQuery()->execute()->fetchField();
  $subquery = db_select('taxonomy_term_hierarchy', 'h')->fields('h', array(
    'parent'
  ));
  $leaf_terms = db_select('taxonomy_term_data', 't')->condition('vid', $vocabulary->vid)->condition('tid', $subquery, 'NOT IN')->countQuery()->execute()->fetchField();
  // Next the field statistics
  $field_instances = field_info_instances('taxonomy_term', $vocabulary->machine_name);
  $fields = array();
  foreach($field_instances as $field_instance){
    $field = field_info_field($field_instance['field_name']);
    if($field['module'] == 'list'){
      $fields[$field_instance['field_name']] = array(
        'label' => $field_instance['label'],
        'values' => array()
      );
      $subquery = db_select('taxonomy_term_data', 't')->fields('t', array(
        'tid'
      ))->condition('vid', $vocabulary->vid);
      $query = db_select('field_data_' . $field_instance['field_name'], 'f');
      $query->addExpression('f.' . $field_instance['field_name'] . '_value', 'value');
      $query->addExpression('COUNT(*)', 'count');
      $results = $query->condition('entity_id', $subquery, 'IN')->groupBy($field_instance['field_name'] . '_value')->execute();
      $total = 0;
      foreach($results as $row){
        $row->percentage = round(($row->count/$total_terms)*100, 1);
        $fields[$field_instance['field_name']]['values'][] = (array)$row;
        $total += $row->count;
      }
      if(!$fields[$field_instance['field_name']]['values']){
        unset($fields[$field_instance['field_name']]);
      }
    }
  }
  return array(
    '#theme' => 'taxonomystatistics_page',
    '#name' => $vocabulary->name,
    '#allterms' => $total_terms,
    '#leafterms' => $leaf_terms,
    '#fields' => $fields
  );
}