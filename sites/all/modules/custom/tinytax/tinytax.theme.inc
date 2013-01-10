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
  // Check the total number of terms in this taxonomy.  If too many, and we're
  // set to display ancestors, then disable the display of ancestors.
  if($variables['ancestors']){
    $row = db_query('SELECT COUNT(*) AS num_terms FROM {taxonomy_term_data} WHERE vid = :vid', array(
      ':vid' => $variables['vid']
    ))->fetch();
    if($row->num_terms > TINYTAX_MAX_TERMS_ANCESTORS_NUM){
      $variables['ancestors'] = 0;
    }
  }
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
  uasort($terms, '_tinytax_sort_terms');
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
 * Sort the terms in a branch.  Do this according to weight, but also according
 * to the validity of the term (tight link here to the ITIS module which is a 
 * little shitty).
 */
function _tinytax_sort_terms($a, $b){
  $a_usage = isset($a->field_usage[LANGUAGE_NONE][0]['value']) ? $a->field_usage[LANGUAGE_NONE][0]['value'] : 'valid';
  $b_usage = isset($b->field_usage[LANGUAGE_NONE][0]['value']) ? $b->field_usage[LANGUAGE_NONE][0]['value'] : 'valid';
  if($a_usage != $b_usage){
    if($a_usage == 'invalid' || $a_usage == 'not accepted'){
      return FALSE;
    }else{
      return TRUE;
    }
  }
  return strcmp($a->name, $b->name);
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
 * tinytax_term theme function
 */
function theme_tinytax_term($variables){
  $term = menu_get_object('taxonomy_term', 2);
  if($term->tid == $variables['term']->tid){
    $active = TRUE;
  }else{
    $active = FALSE;
  }
  $has_children = count(taxonomy_get_children($variables['term']->tid));
  $output = '<li id="tinytax-' . $variables['term']->tid . '">';
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
  $term_uri = taxonomy_term_uri($variables['term']);
  $term_text = theme('scratchpads_species_name', array(
    'term' => $variables['term']
  ));
  if(!$term_text){
    $term_text = check_plain($variables['term']->name);
  }
  $term_text = l($term_text, $term_uri['path'], array(
    'html' => TRUE,
    'attributes' => array(
      'class' => $active ? array(
        'tinytax-bold'
      ) : array()
    )
  ));
  if(isset($variables['term']->field_usage)){
    if(@in_array($variables['term']->field_usage[LANGUAGE_NONE][0]['value'], array(
      'invalid',
      'not accepted'
    ))){
      if(@isset($variables['term']->field_unacceptability_reason[LANGUAGE_NONE][0]['value'])){
        switch($variables['term']->field_unacceptability_reason[LANGUAGE_NONE][0]['value']){
          case 'objective synonym':
          case 'homotypic (nomenclatural) synonym':
            $term_text = '&equiv; ' . $term_text;
            break;
          case 'heterotypic (taxonomic) synonym':
          case 'synonym':
            $term_text = '= ' . $term_text;
            break;
          case 'homonym (illegitimate)':
          case 'superfluous renaming (illegitimate)':
            $term_text = $term_text . ' nom. illeg.';
            break;
          case 'invalidly published, nomen nudum':
          case 'invalidly published, other':
          case 'orthographic variant (misspelling)':
            $term_text = $term_text . ' nom. inval.';
            break;
          case 'rejected name':
            $term_text = $term_text . ' nom. rej..';
            break;
          default:
            // Do nothing.
            break;
        }
      }
    }
  }
  $output .= $term_text;
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
