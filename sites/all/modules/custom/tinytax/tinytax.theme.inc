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
  if(module_exists('relation_add-')){
    // FIXME - The toggle will not work when not loading the full term.  We
    // therefore hide the toggle when the relation_add module is enabled.
    $toggle = array();
  }
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
  if(module_exists('relation_add-')){
    // Due to the way the relation_add module loads relaion entities, and the
    // fact that it is a HUGE performance overhead, we do not load the full
    // entity if it is enabled.  This means the sorting and display of terms
    // will be broken.
    // FIXME - A better solution for this is needed.
    $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1);
  }else{
    $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1, TRUE);
  }
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
  // Compare Usage
  $a_usage = !empty($a->field_usage[LANGUAGE_NONE][0]['value']) ? $a->field_usage[LANGUAGE_NONE][0]['value'] : 'valid';
  $b_usage = !empty($b->field_usage[LANGUAGE_NONE][0]['value']) ? $b->field_usage[LANGUAGE_NONE][0]['value'] : 'valid';
  if($a_usage != $b_usage){
    // As we use 'valid' for default value, it may be that we have one as 'valid' and one as 'accepted'
    $a_usage = ($a_usage == 'valid' || $a_usage == 'accepted');
    $b_usage = ($b_usage == 'valid' || $b_usage == 'accepted');
    if($a_usage != $b_usage){return $a_usage ? 1 : -1;}
  }
  // Compare Weight
  if(!empty($a->weight) && !empty($b->weight)){
    if($a->weight != $b->weight){return $a->weight > $b->weight;}
  }
  // Compare Name
  return strcasecmp($a->name, $b->name);
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
  $class = '';
  if($variables['toggle']){
    foreach($variables['toggle'] as $field_name => $toggle_values){
      foreach($variables['term']->{$field_name} as $lng => $values){
        foreach($values as $value){
          if(in_array($value['value'], $toggle_values)){
            $class = " class=\"vid-{$variables['term']->vid} toggleable\"";
            break;
          }
        }
      }
    }
  }
  $output = "<li id=\"tinytax-{$variables['term']->tid}\"$class>";
  $has_children = db_select('taxonomy_term_hierarchy', 't')->condition('parent', $variables['term']->tid)->countQuery()->execute()->fetchField();
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
  if($variables['term']->tid == arg(2) && strpos(arg(0), 'axonomy') && arg(1) == 'term'){
    $active = TRUE;
  }else{
    $active = FALSE;
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
      'vid' => $variables['term']->vid,
      'toggle' => $variables['toggle']
    ));
  }else{
    $output .= theme('tinytax_term_count', array(
      'count' => $has_children
    ));
  }
  return $output . '</li>';
}
