<?php


/**
 * @file Views field handler for the description of a menu item.
 */
class menu_node_views_field_description extends views_handler_field {

  function query() {
    $this->add_additional_fields();
  }
  
  function construct() {
    parent::construct();
    
    $this->additional_fields = array();
    $this->additional_fields['description'] = array('table' => 'menu_links', 'field' => 'options');
  }
  
  function render($values) {
    $output = '';
    $data = unserialize($values->menu_links_options);
    if (!empty($data['attributes']['title'])) {
      $output = filter_xss($data['attributes']['title']);
    }
    return $output;
  }

}
