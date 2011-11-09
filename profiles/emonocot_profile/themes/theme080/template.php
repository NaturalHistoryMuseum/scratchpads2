<?php

function phptemplate_menu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL){
  static $zebra = FALSE;
  $zebra = !$zebra;
  $class = ($menu ? 'expanded' : ($has_children ? 'collapsed' : 'leaf'));
  if(!empty($extra_class)){
    $class .= ' ' . $extra_class;
  }
  if($in_active_trail){
    $class .= ' active-trail';
  }
  if($zebra){
    $class .= ' even';
  }else{
    $class .= ' odd';
  }
  return '<li class="' . $class . '">' . $link . $menu . "</li>\n";
}
?>