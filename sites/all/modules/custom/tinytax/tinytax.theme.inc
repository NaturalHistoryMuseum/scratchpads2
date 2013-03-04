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
  $toggle = variable_get('tinytax_hidden_field_and_value_' . $variables['vid'], array());
  $toggle_form = '';
  if(count($toggle)){
    $toggle_form = '<form><input class="tinytax-toggle-checkbox" data-vid="' . $variables['vid'] . '" id="tinytax-toggle-' . $variables['vid'] . '" type="checkbox" value="0" name="tinytax_toggle_' . $variables['vid'] . '"/>
  <label class="option" for="tinytax-toggle-' . $variables['vid'] . '">' . t('Hide %values terms', array(
      '%values' => implode(', ', current($toggle))
    )) . '</label>
</form>';
  }
  return '<div class="tinytax">' . $toggle_form . theme('tinytax_branch', array(
    'vid' => $variables['vid'],
    'open_tids' => $variables['open_tids'],
    'ancestors' => $variables['ancestors'],
    'toggle' => $toggle
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
      'open_tids' => $variables['open_tids'],
      'toggle' => $variables['toggle']
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
  if($term->tid == $variables['term']->tid){
    $active = TRUE;
  }else{
    $active = FALSE;
  }
  $has_children = count(taxonomy_get_children($variables['term']->tid));
  $class = '';
  if($variables['toggle']){
    foreach($variables['toggle'] as $field_name => $toggle_values){
      foreach($variables['term']->{$field_name} as $lng => $values){
        foreach($values as $value){
          if(in_array($value['value'], $toggle_values)){
            $class = " class=\"vid-{$variables['term']->vid} toggleable\"";
          }
        }
      }
    }
  }
  $output = "<li id=\"tinytax-{$variables['term']->tid}\"$class>";
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
          // Suffixes
          case 'other':
          case 'unavailable, other':
            $term_text = $term_text . ' unplaced';
            break;
          case 'nomen oblitum':
            $term_text = $term_text . ' nom. oblitum';
            break;
          case 'nomen dubium':
            $term_text = $term_text . ' nom. dubium';
            break;
          case 'misapplied':
            $term_text = $term_text . ' misapplied';
            break;
          case 'junior homonym':
            $term_text = $term_text . ' homonym';
            break;
          case 'horticultural':
            $term_text = $term_text . ' horticultural';
            break;
          case 'database artifact':
          case 'unavailable, database artifact':
            $term_text = $term_text . ' artifact';
            break;
          case 'homonym (illegitimate)':
          case 'superfluous renaming (illegitimate)':
          case 'unavailable, nomen nudum':
            $term_text = $term_text . ' nom. illeg.';
            break;
          case 'unavailable, literature misspelling':
          case 'unavailable, incorrect original spelling':
          case 'invalidly published, nomen nudum':
          case 'invalidly published, other':
          case 'orthographic variant (misspelling)':
            $term_text = $term_text . ' nom. inval.';
            break;
          case 'rejected name':
          case 'unavailable, suppressed by ruling':
            $term_text = $term_text . ' nom. rej.';
            break;
          case 'subsequent name/combination':
            $term_text = $term_text . ' n. comb.';
            break;
          case 'unjustified emendation':
            $term_text = $term_text . ' unjustified emendation';
            break;
          case 'unnecessary replacement':
            $term_text = $term_text . ' unnecessary replacement';
            break;
          // Prefixes
          case 'objective synonym':
          case 'homotypic (nomenclatural) synonym':
            $term_text = '&equiv; ' . $term_text;
            break;
          case 'heterotypic (taxonomic) synonym':
          case 'synonym':
          case 'junior synonym':
          case 'subjective synonym':
            $term_text = '= ' . $term_text;
            break;
          // Both
          case 'pro parte':
            $term_text = '= ' . $term_text . ' p.p.';
            break;
          case 'homonym & junior synonym':
            $term_text = '= ' . $term_text . ' nom. illeg.';
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
