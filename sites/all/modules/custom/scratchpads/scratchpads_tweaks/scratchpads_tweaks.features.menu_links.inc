<?php

/**
 * Implements hook_menu_default_menu_links().
 */
function scratchpads_tweaks_menu_default_menu_links(){
  $menu_links = array();
  // Exported menu link: management:http://help.scratchpads.org/
  $menu_links['management:http://help.scratchpads.org/'] = array(
    'menu_name' => 'management',
    'link_path' => 'http://help.scratchpads.org/',
    'router_path' => '',
    'link_title' => 'Help',
    'options' => array(
      'attributes' => array(
        'title' => ''
      )
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '50',
    'parent_path' => 'admin'
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Help');
  return $menu_links;
}
