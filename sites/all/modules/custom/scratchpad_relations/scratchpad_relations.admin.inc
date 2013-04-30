<?php

/**
 * Menu callback for admin/content/relation. Displays all relations on the site.
 */
function scratchpad_relations_admin_content($relation_type){
  // Set up header row.
  $header = array(
    'label' => array(
      'data' => t('Title'),
      'field' => 'r.rid',
      'sort' => 'asc'
    ),
    'type' => array(
      'data' => t('Type'),
      'field' => 'r.relation_type'
    ),
    t('Relation'),
    'operations' => array(
      'data' => t('Operations'),
      'colspan' => '2'
    )
  );
  // Grab all relations.
  $query = db_select('relation', 'r')->extend('PagerDefault')->extend('TableSort');
  $query->condition('r.relation_type', $relation_type, '=');
  $query->fields('r', array(
    'rid',
    'relation_type'
  ))->limit(50)->orderByHeader($header);
  $relations = $query->execute();
  return theme('scratchpad_relations_admin_content', array(
    'relations' => $relations,
    'header' => $header
  ));
}

/**
 * Generate a table of all relations on this site.
 */
function theme_scratchpad_relations_admin_content($variables){
  drupal_set_title('Scratchpad relations');
  $relations = $variables['relations'];
  $header = $variables['header'];
  $types = relation_get_types();
  $rows = array();
  if(empty($relations)){
    // Give a message if there are no relations returned.
    $message = t('There are currently no relations on your site.');
    $rows[] = array(
      array(
        'data' => $message,
        'colspan' => 5
      )
    );
  }else{
    foreach($relations as $relation){
      // Load the relation.
      $r = relation_load($relation->rid);
      // Get the endpoints for this relation.
      $endpoints = field_get_items('relation', $r, 'endpoints');
      // Get the type for this relation
      $type = $types[$r->relation_type];
      if(!empty($endpoints)){
        $relation_entities = array();
        foreach($endpoints as $endpoint){
          $entities = entity_load($endpoint['entity_type'], array(
            $endpoint['entity_id']
          ));
          $entity = reset($entities);
          $title = entity_label($endpoint['entity_type'], $entity);
          $path = entity_uri($endpoint['entity_type'], $entity);
          // Logic to process how the different entities return a uri.
          // see this issue: http://drupal.org/node/1057242
          if($endpoint['entity_type'] == 'file'){
            $path = array(
              'path' => file_create_url($path)
            );
          }
          if($endpoint['entity_type'] == 'taxonomy_vocabulary'){
            $path = array(
              'path' => 'admin/structure/taxonomy/' . $entity->machine_name
            );
          }
          $relation_entities[] = array(
            'title' => $title,
            'path' => $path['path']
          );
        }
      }
      // Build the column for the relation entities.
      $relation_column = array();
      foreach($relation_entities as $entity){
        $relation_column[] = l($entity['title'], $entity['path']);
      }
      // Build the rows to pass to the table theme function.
      // Directional is implemented, not sure how well it works.
      $rows[] = array(
        l(t('Relation') . ' ' . $relation->rid, 'relation/' . $relation->rid),
        $type->label . ' (' . $relation->relation_type . ')',
        implode(($type->directional) ? " → " : " ↔ ", $relation_column),
        user_access('edit relations') ? l(t('Edit'), 'relation/' . $relation->rid . '/edit') : '',
        user_access('delete relations') ? l(t('Delete'), 'relation/' . $relation->rid . '/delete') : ''
      );
    }
  }
  return theme('table', array(
    'header' => $header,
    'rows' => $rows
  )) . theme('pager');
}
