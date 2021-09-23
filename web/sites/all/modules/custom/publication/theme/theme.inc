<?php

/**
 * Displays the list of available publication types for publication creation.
 *
 * @ingroup themeable
 */
function theme_publication_add_list($variables){
  $content = $variables['content'];
  $output = '';
  if($content){
    $output = '<dl class="publication-type-list">';
    foreach($content as $item){
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }else{
    if(user_access('administer publication types')){
      $output = '<p>' . t('Publications cannot be added because you have not created any publication types yet. Go to the <a href="@create-publication-type">publication type creation page</a> to add a new publication type.', array(
        '@create-publication-type' => url('admin/structure/publication_types/add')
      )) . '</p>';
    }else{
      $output = '<p>' . t('No publication types have been created yet for you to use.') . '</p>';
    }
  }
  return $output;
}

function publication_preprocess_publication_field_group(&$variables){
  $element = $variables['element'];
  $variables['label'] = $element['#group']->label;
}

function theme_publication_field_group($variables){
  $output = '';
  $element = $variables['element'];
  $output .= '<div class="publication-group clearfix"><label>' . $variables['label'] . '</label>';
  if(isset($variables['links'])){
    $output .= $variables['links'];
  }
  $output .= $element['#children'] . '</div>';
  return $output;
}

