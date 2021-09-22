<?php

/**
 * Displays the list of available publication types for publication creation.
 *
 * @ingroup themeable
 */
function theme_entity_ui_add_list($variables){
  $content = $variables['content'];
  $output = '';
  if($content){
    $output = '<dl class="entity-type-list">';
    foreach($content as $item){
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }else{
    $output = '<p>' . t('No entity types have been created yet for you to use.') . '</p>';
  }
  return $output;
}