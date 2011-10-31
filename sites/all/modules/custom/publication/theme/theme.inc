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
  if(isset($element['#group']->format_settings['instance_settings']['links'])){
    $links = $element['#group']->format_settings['instance_settings']['links'];
    $item_list_vars = array();
    $entity_id = entity_id($element['#entity_type'], $element['#entity']);
    if($links['edit_link']){
      $item_list_vars['items'][] = l('Edit', 'publication/' . $entity_id . '/edit', array(
        'query' => array(
          'step' => $element['#group']->group_name
        )
      ));
    }
    if(isset($links['additional_link']['url'])){
      $links['additional_link']['url'] = token_replace($links['additional_link']['url'], array(
        'publication' => $element['#entity']
      ));
      $item_list_vars['items'][] = l(($links['additional_link']['title'] ? $links['additional_link']['title'] : $links['additional_link']['url']), $links['additional_link']['url'], array(
        'query' => array(
          'destination' => request_uri()
        )
      ));
    }
    if(count($item_list_vars['items'])){
      $variables['links'] = theme('item_list', $item_list_vars);
    }
  }
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

