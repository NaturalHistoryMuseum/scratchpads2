<?php

/**
 * Theme the term for the lexicon term page. Added this so that we can add any
 * custom fields we desire.
 */
function theme_scratchpads_lexicon_overview_item($variables){
  //   print_r($variables['term']);
  //   exit();
  $term = $variables['term'];
  $o = '<dt><a id="' . $term->id . '"></a>' . $term->safe_name;
  if(isset($term->extralinks)){
    $o .= ' <span class="lexicon-extralinks">';
    foreach($term->extralinks as $link){
      $o .= l($link["name"], $link["path"], $link["attributes"]);
    }
    $o .= '</span>';
  }
  $o .= '</dt>';
  if(isset($term->safe_description) || isset($term->synonyms) || isset($term->image) || isset($term->related)){
    $o .= '<dd>';
    if(isset($term->image)){
      $o .= '<img src="' . image_style_url('thumbnail', $term->image["uri"]) . '" alt="' . $term->image["alt"] . '" title="' . $term->image["title"] . '" />';
    }
    if(isset($term->safe_description)){
      $o .= $term->safe_description;
    }
    if(isset($term->field_reference)){
      $ref_renders = array();
      foreach($term->field_reference as $lang => $references){
        foreach($references as $reference){
          $ref = node_load($reference['nid']);
          $ref_renders[$ref->nid] = theme('biblio_style', array(
            'node' => $ref,
            'base' => variable_get('biblio_base', 'biblio'),
            'style_name' => biblio_get_style()
          ));
        }
      }
      $o .= theme('item_list', array(
        'items' => $ref_renders
      ));
    }
    if(isset($term->related)){
      $o .= '<p class="lexicon-related">' . t('See also') . ': ';
      foreach($term->related as $related_term){
        if(isset($related_term["link"])){
          $o .= l($related_term["name"], $related_term["link"]["path"], array(
            'fragment' => $related_term["link"]["fragment"]
          ));
        }else{
          $o .= $related_term["name"];
        }
      }
      $o .= '</p>';
    }
    if(isset($term->synonyms)){
      $o .= '<p class="lexicon-synonyms">' . t('Synonyms') . ': ';
      foreach($term->synonyms as $synonym){
        $o .= $synonym;
      }
      $o .= '</p>';
    }
    $o .= '</dd>';
  }
  return $o;
}