<?php

/**
 * Preprocess field groups for pensoft_publication publications
 */
function pensoft_publication_preprocess_publication_field_group(&$variables) {
  $element = $variables['element'];
  
  $variables['element_id'] = $element['#id'];
  
  // Group label
  $variables['label'] = $element['#group']->label;
  
  // Sort authors and institutions
  if ($element['#id'] == 'publication_pensoft_full_group_publication_auth_contrib') {
    $authors = array($element['field_publication_submit_author'][0]);
    $institutions = array();
    if (!empty($element['field_publication_submit_author'][0]['#publication_author_institution'])) {
      $institutions[] = $element['field_publication_submit_author'][0]['#publication_author_institution'];
    }
    foreach (element_children($element['field_publication_authors']) as $key) {
      $authors[] = $element['field_publication_authors'][$key];
      if (!empty($element['field_publication_authors'][$key]['#publication_author_institution'])) {
        $institutions[] = $element['field_publication_authors'][$key]['#publication_author_institution'];
      }
    }

    $institutions = array_unique($institutions);
    
    foreach ($authors as $key => $author) {
      $authors[$key] = htmlspecialchars($author['#publication_author_name']);
      if (!empty($author['#publication_author_institution'])) {
        $inst = array_search($author['#publication_author_institution'], $institutions);
        $authors[$key] .= ' <sup>' . ($inst+1) . '</sup>';
      }
    }
    
    foreach ($institutions as $key => $inst) {
      $institutions[$key] = '<div class="field">' . ($key+1) . ' ' . $inst . '</div>';
    }
    
    
    $variables['authors'] = $authors;
    $variables['institutions'] = $institutions;
  }
}