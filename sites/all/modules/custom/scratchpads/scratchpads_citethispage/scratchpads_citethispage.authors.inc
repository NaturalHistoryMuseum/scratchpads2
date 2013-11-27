<?php

/***
 * This file contains various tools used to gather and format authors
 * in various ways (as required by different types of citations)
 */
/**
 * Implement hook_view_post_execute
 * 
 * This is used to gather the authors of all items displayed by views on the current
 * page.
 */
function scratchpads_citethispage_views_post_execute(&$view){
  if(!preg_match('%^taxonomy/term/(\d+)%', current_path())){return;}
  if($view->base_table == 'node' || $view->base_table == 'file_managed'){
    // These get called but are not used in this context    
    if($view->name == 'species_media' && $view->is_attachment){return;}
    $type_map = array(
      'node' => 'node',
      'file_managed' => 'file'
    );
    $entity_type = $type_map[$view->base_table];
    foreach($view->result as $delta => $row){
      if(!empty($row->{$view->base_field})){
        $author_data = _scratchpads_citethispage_authors_of_entity($entity_type, $row->{$view->base_field});
        _scratchpads_citethispage_list_authors($author_data);
      }
    }
  }
}

/**
 * _scratchpads_citethispage_list_authors
 * 
 * This is used to save/get the list of authors on page (gathered through
 * hook_view_post_execute
 */
function _scratchpads_citethispage_list_authors($data = NULL){
  $authors = &drupal_static(__FUNCTION__);
  if(!isset($authors)){
    $authors = array(
      'authors' => array(
        'current' => array(),
        'initial' => array(),
        'others' => array()
      ),
      'full_authors' => array(
        'current' => array(),
        'initial' => array(),
        'others' => array()
      )
    );
  }
  if($data){
    $authors = array_merge_recursive($authors, $data);
  }
  return $authors;
}

/**
 * _scratchpads_citethispages_authors_of_entity
 * 
 * This gathers the list of authors of the given entity, and returns
 * the information as formatted by _scratchpads_citethispage_format_author
 * 
 * It is assumed that entities given here have a 'uid' field.
 */
function _scratchpads_citethispage_authors_of_entity($type, $id){
  $info = entity_get_info($type);
  $has_revisions = !empty($info['revision table']);
  $output = array(
    'authors' => array(
      'initial' => array(),
      'current' => array(),
      'others' => array()
    ),
    'full_authors' => array(
      'initial' => array(),
      'current' => array(),
      'others' => array()
    )
  );
  // Get current author
  $current = db_query("
    SELECT b.uid
      FROM {" . $info['base table'] . "} b
     WHERE b." . $info['entity keys']['id'] . " = :base_id
  ", array(
    ':base_id' => $id
  ))->fetchField();
  $full = _scratchpads_citethispage_format_author($current);
  $output['full_authors']['current'][0] = $full['data'];
  $output['authors']['current'][0] = $full['short'];
  // Get revision authors
  if($has_revisions){
    $result = db_query("
      SELECT r.uid
      FROM {" . $info['revision table'] . "} r
      WHERE r." . $info['entity keys']['id'] . " = :base_id
      ORDER BY r." . $info['entity keys']['revision'] . " ASC
    ", array(
      ':base_id' => $id
    ));
    while(($uid = $result->fetchField()) !== FALSE){
      $full = _scratchpads_citethispage_format_author($uid);
      $output['full_authors']['others'][] = $full['data'];
      $output['authors']['others'][] = $full['short'];
    }
    $output['full_authors']['initial'][0] = array_shift($output['full_authors']['others']);
    $output['authors']['initial'][0] = array_shift($output['authors']['others']);
  }else{
    $output['full_authors']['initial'][0] = $output['full_authors']['current'][0];
    $output['authors']['initial'][0] = $output['authors']['current'][0];
  }
  return $output;
}

/**
 * _scratchpads_citethispage_format_author
 * 
 * Given a user id, return an array defining:
 * 'short': The author's short name for citation,
 * 'data': The author's full information, including:
 * - firstname
 * - lastname
 */
function _scratchpads_citethispage_format_author($user){
  static $format = array();
  if(is_numeric($user)){
    if(isset($format[$user])){return $format[$user];}
    $user = user_load($user);
  }
  if(!isset($format[$user->uid])){
    $data = array();
    $last = field_get_items('user', $user, 'field_user_family_name');
    $first = field_get_items('user', $user, 'field_user_given_names');
    if(empty($last)){
      $data['short'] = check_plain($user->name);
      $data['data'] = array(
        'firstname' => '',
        'lastname' => check_plain($user->name)
      );
    }else{
      $data = array(
        'short' => $last[0]['safe_value'],
        'data' => array(
          'lastname' => $last[0]['safe_value']
        )
      );
      if(!empty($first)){
        $data['short'] .= ' ' . $first[0]['safe_value'][0] . '.';
        $data['data']['firstname'] = $first[0]['safe_value'];
      }else{
        $data['data']['firstname'] = '';
      }
    }
    $format[$user->uid] = $data;
  }
  return $format[$user->uid];
}
