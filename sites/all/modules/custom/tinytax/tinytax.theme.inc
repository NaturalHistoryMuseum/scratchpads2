<?php

/**
 * @file
 * 
 * Description of the tinytax.theme.inc file
 */
/**
 * tinytax_
 */
function theme_tinytax_block($variables){
  return '<div class="tinytax">' . theme('tinytax_branch', array(
    'vid' => $variables['vid'],
    'open_tids' => $variables['open_tids'],
    'ancestors' => $variables['ancestors']
  )) . '</div>';
}

/**
 * tinytax_branch theme function
 */
function theme_tinytax_branch($variables){
  // Get the terms to render
  $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1, TRUE);
  $output = '<ul>';
  foreach($terms as $term){
    $output .= theme('tinytax_term', array(
      'term' => $term,
      'ancestors' => $variables['ancestors'],
      'open_tids' => $variables['open_tids']
    ));
  }
  return $output . '</ul>';
}

/**
 * Theme the term count
 */
function theme_tinytax_term_count($variables){
  if($variables['count'] && is_numeric($variables['count'])){
    return ' <span class="tinytax-child-count">(' . $variables['count'] . ')</span>';
  }else{
    return '';
  }
}

/**
 * Get the count of children for this term.
 */
function _tinytax_get_children_count($tid, $recurse = FALSE){
  // If not recursing, we can keep things simple, else we need the children tids.
  if($recurse){
    $total = 0;
    $results = db_select('taxonomy_term_hierarchy', 't')->condition('parent', $tid)->fields('t', array(
      'tid'
    ))->execute();
    foreach($results as $row){
      $total++;
      $total += _tinytax_get_children_count($row->tid, TRUE);
    }
    return $total;
  }else{
    return count(taxonomy_get_children($tid));
  }
}

/**
 * tinytax_term theme function
 */
function theme_tinytax_term($variables){
  $term = menu_get_object('taxonomy_term', 2);
  if($term->tid == $variables['term']->tid){
    $active = TRUE;
  }else{
    $active = FALSE;
  }
  $has_children = _tinytax_get_children_count($variables['term']->tid, $variables['ancestors']);
  $output = '<li>';
  if($has_children){
    // Plus or minus.
    if(in_array($variables['term']->tid, $variables['open_tids'])){
      // Minus
      $output .= theme('image', array(
        'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/minus.gif'),
        'alt' => t('Close'),
        'title' => t('Close'),
        'attributes' => array(
          'class' => array(
            'click',
            'minus'
          ),
          'id' => $variables['term']->tid
        )
      ));
    }else{
      // Plus
      $output .= theme('image', array(
        'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/plus.gif'),
        'alt' => t('Open'),
        'title' => t('Open'),
        'attributes' => array(
          'class' => array(
            'click',
            'plus'
          ),
          'id' => $variables['term']->tid
        )
      ));
    }
  }else{
    // Leaf
    $output .= theme('image', array(
      'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/leaf.gif')
    ));
  }
  if(isset($variables['term']->field_usage)){
    if(@in_array($variables['term']->field_usage[LANGUAGE_NONE][0]['value'], array(
      'invalid',
      'not accepted'
    ))){
      $output .= "= ";
    }
  }
  $term_uri = taxonomy_term_uri($variables['term']);
  $term_text = theme('scratchpads_species_name', array(
    'term' => $variables['term']
  ));
  if(!$term_text){
    $term_text = check_plain($variables['term']->name);
  }
  $output .= l($term_text, $term_uri['path'], array(
    'html' => TRUE,
    'attributes' => array(
      'class' => $active ? array(
        'tinytax-bold'
      ) : array()
    )
  ));
  if(in_array($variables['term']->tid, $variables['open_tids']) && $has_children){ // Check here for open!
    $output .= theme('tinytax_term_count', array(
      'count' => $has_children
    )) . theme('tinytax_branch', array(
      'open_tids' => $variables['open_tids'],
      'tid' => $variables['term']->tid,
      'vid' => $variables['term']->vid
    ));
  }else{
    $output .= theme('tinytax_term_count', array(
      'count' => $has_children
    ));
  }
  return $output . '</li>';
}
