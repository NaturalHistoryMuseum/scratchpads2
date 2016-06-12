<?php

/**
 * Theme function for an empty results page.
 */
function theme_ecoint_search($variables) {
  $rows = array();
  foreach($variables['results'] as $result){
    $ecoint_node = node_load($result['node']->entity_id);
    $row = array();

    $left_term = taxonomy_term_load($ecoint_node->field_taxonomic_name['und'][0]['tid']);
    $row[] = l($left_term->name, 'taxonomy/term/'.$left_term->tid)." ".$ecoint_node->field_life_cycle_stage['und'][0]['safe_value']." ".$ecoint_node->field_organism_part['und'][0]['safe_value'];

    $interaction = taxonomy_term_load($ecoint_node->field_interaction_type['und'][0]['tid']);
    $row[] = $interaction->name;

    $right_fields = field_collection_item_load($ecoint_node->field_int_collection['und'][0]['value']);
    $right_term = taxonomy_term_load($right_fields->field_taxonomic_name['und'][0]['tid']);
    //print "<PRE>";print_r($right_fields);exit;
    $row[] = l($right_term->name, 'taxonomy/term/'.$right_term->tid)." ".$right_fields->field_life_cycle_stage_2['und'][0]['safe_value']." ".$right_terms->field_organism_part_2['und'][0]['safe_value'];
    
    $row[] = l('view', 'node/'.$ecoint_node->nid);

    $rows[] = $row;
  }
  return theme('table', array(
      'header' => array(
          array(
              'data' => t('Species A'),
              'field' => '',
              'sort' => 'asc'
          ),
          array(
              'data' => t('Interaction'),
              'field' => '',
              'sort' => ''
          ),
          array(
              'data' => t('Species B'),
              'field' => '',
              'sort' => ''
          ),
          array(
              'data' => t('Link'),
              'field' => '',
              'sort' => '',
          ),
      ),
      'rows' => $rows,
      'attributes' => array(
          'class' => array(
              'ecoint'
          )
      )
  )) . theme('pager', array(
      'tags' => NULL
  ));
}
 
function theme_ecoint_search_empty($variables){
  return t('<p>Either no ecological interaction content has been added to this site, or it has not yet been indexed. Indexing can take up to one hour, so please check back later.</p>');
}