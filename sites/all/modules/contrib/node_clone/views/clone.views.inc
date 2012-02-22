<?php
// $Id: clone.views.inc,v 1.1 2008/10/14 23:16:56 pwolanin Exp $

/**
* Implementation of hook_views_handlers()
*/
function clone_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'clone') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_node_link_clone' => array(
        'parent' => 'views_handler_field_node_link',
      ),
    ),
  );
}

/**
* Implementation of hook_views_data_alter()
*/
function clone_views_data_alter(&$views_data) {
  $views_data['node']['clone_node'] = array(
    'field' => array(
      'title' => t('Clone link'),
      'help' => t('Provide a simple link to clone the node.'),
      'handler' => 'views_handler_field_node_link_clone',
    ),
  );
}

