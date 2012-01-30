<?php

function theme_field_group_stripy_div($variables){
  $output = '';
  $element = $variables['element'];
  $output .= '<div class="stripy-div-group clearfix">';
  if(isset($variables['label'])){
    $output .= '<label>' . $variables['label'] . '</label>';
  }
  if(isset($variables['links'])){
    $output .= $variables['links'];
  }
  $output .= $element['#children'] . '</div>';
  return $output;
}