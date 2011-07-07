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
  return drupal_render(drupal_get_form('tinytax_search_form', $variables['vid'])) . '<div class="tinytax">' . theme('tinytax_branch', array(
    'vid' => $variables['vid'],
    'open_tids' => $variables['open_tids']
  )) . '</div>';
}

/**
 * Search form
 */
function tinytax_search_form($form, $form_state){
  return array(
    'autocomplete' => array(
      '#title' => '',
      '#type' => 'textfield',
      '#autocomplete_path' => 'tinytax/autocomplete/' . $form_state['build_info']['args'][0],
      '#attributes' => array(
        'class' => array(
          'tinytax-search-field'
        )
      ),
      '#id' => 'tinytax-search-field-' . $form_state['build_info']['args'][0]
    )
  );
}

/**
 * tinytax_branch theme function
 */
function theme_tinytax_branch($variables){
  // Add the CSS and JS
  drupal_add_css(drupal_get_path('module', 'tinytax') . '/css/tinytax.css');
  drupal_add_js(drupal_get_path('module', 'tinytax') . '/js/tinytax.js');
  drupal_add_js(array(
    'tinytax' => array(
      'callback' => url('tinytax/get'),
      'search_callback' => url('tinytax/search')
    )
  ), 'setting');
  // Get the terms to render
  $terms = taxonomy_get_tree($variables['vid'], $variables['tid'], 1, TRUE);
  $output = '<ul>';
  foreach($terms as $term){
    $output .= theme('tinytax_term', array(
      'term' => $term,
      'open_tids' => $variables['open_tids']
    ));
  }
  return $output . '</ul>';
}

/**
 * tinytax_term theme function
 */
function theme_tinytax_term($variables){
  $has_children = count(taxonomy_get_children($variables['term']->tid));
  $output = '<li>';
  drupal_add_js(array(
    'tinytax' => array(
      'minus' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/minus.gif'),
      'load' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/load.gif'),
      'plus' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/plus.gif')
    )
  ), 'setting');
  if($has_children){
    // Plus or minus.
    if(in_array($variables['term']->tid, $variables['open_tids'])){
      // Minus
      $output .= theme('image', array(
        'path' => file_create_url(drupal_get_path('module', 'tinytax') . '/images/minus.gif'),
        'alt' => t('Close'),
        'title' => t('Close'),
        'attributes' => array(
          'class' => 'click',
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
          'class' => 'click',
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
  $output .= l($variables['term']->name, $term_uri['path']);
  if(in_array($variables['term']->tid, $variables['open_tids']) && $has_children){ // Check here for open!
    $output .= theme('tinytax_branch', array(
      'open_tids' => $variables['open_tids'],
      'tid' => $variables['term']->tid,
      'vid' => $variables['term']->vid
    ));
  }
  return $output . '</li>';
}