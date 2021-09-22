<?php

/**
 * Menu callback: publication content administration.
 */
function entity_ui_content_form($form, &$form_state, $entity_type, $controller){
	$entity_info = $controller->get_entity_info();
  $schema = drupal_get_schema($entity_info['base table']);
  $fields = array_intersect(array_keys($schema['fields']), array(
    'type',
    'title',
    'uid',
    'changed'
  ));
  $class = $entity_info['admin ui']['controller class'];
  $id = $entity_info['entity keys']['id'];

  $form = array();
  // Build the sortable table header.
  $header = array();
  foreach($fields as $field){
    switch($field){
      case 'uid':
        $header[$field] = array(
          'data' => t('Author'),
          'field' => 't.' . $field
        );
        break;
      case 'changed':
        $header[$field] = array(
          'data' => t('Updated'),
          'field' => 't.' . $field,
          'sort' => 'desc'
        );
        break;
      default:
        $header[$field] = array(
          'data' => ucfirst($field),
          'field' => 't.' . $field
        );
        break;
    }
  }
  if(count($controller->ops())){
    $header['operations'] = array(
      'data' => t('Operations')
    );
  }
  $query = db_select($entity_info['base table'], 't')->extend('PagerDefault')->extend('TableSort');
  $ids = $query->fields('t', array(
    $id
  ))->limit(50)->orderByHeader($header)->execute()->fetchCol();
  $entities = entity_ui_load_multiple($entity_type, $ids);
  $rows = array();
  if(count($entities)){
    $destination = drupal_get_destination();
    foreach($entities as $entity){
      $rows[$entity->{$id}] = array(
        'title' => array(
          'data' => array(
            '#type' => 'link',
            '#title' => $entity->title,
            '#href' => 'publication/' . $entity->{$id},
            '#suffix' => ' ' . theme('mark', array(
              'type' => node_mark($entity->{$id}, $entity->changed)
            ))
          )
        ),
        'type' => check_plain($entity->type),
        'author' => theme('username', array(
          'account' => user_load($entity->uid)
        )),
        'changed' => format_date($entity->changed, 'short')
      );
      // Build a list of all the accessible operations for the current publication.
      $operations = array();
      if(publication_access('update', $publication)){
        $operations['edit'] = array(
          'title' => t('Edit'),
          'href' => 'publication/' . $publication->pid . '/edit',
          'query' => $destination
        );
      }
      if(publication_access('delete', $publication)){
        $operations['delete'] = array(
          'title' => t('Delete'),
          'href' => 'publication/' . $publication->pid . '/delete',
          'query' => $destination
        );
      }
      $rows[$publication->pid]['operations'] = array();
      if(count($operations) > 1){
        // Render an unordered list of operations links.
        $rows[$publication->pid]['operations'] = array(
          'data' => array(
            '#theme' => 'links__node_operations',
            '#links' => $operations,
            '#attributes' => array(
              'class' => array(
                'links',
                'inline'
              )
            )
          )
        );
      }elseif(!empty($operations)){
        // Render the first and only operation as a link.
        $link = reset($operations);
        $rows[$publication->pid]['operations'] = array(
          'data' => array(
            '#type' => 'link',
            '#title' => $link['title'],
            '#href' => $link['href'],
            '#options' => array(
              'query' => $link['query']
            )
          )
        );
      }
    } // end of foreach publications
  }
  $form['publications'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No content available.')
  );
  $form['pager'] = array(
    '#markup' => theme('pager')
  );
  return $form;
}
