<?php

/**
 * @file
 * Include file for Views hooks
 */


/**
 * Implementation of hook_views_data().
 *
 * Provides the Page Title as a Views field for Views 2.
 */
function page_title_views_data() {
  $data = array();

  // Define the table.
  $data['page_title']['table']['group'] = t('Page Title');

  // Join the node table.
  $data['page_title']['table']['join'] = array(
    'node' => array(
      'table' => 'page_title',
      'left_field' => 'nid',
      'field' => 'id',
      'extra' => array(
        array('field' => 'type', 'value' => 'node', 'operator' => '='),
      ),
    ),
  );

  // Define the field.
  $data['page_title']['page_title'] = array(
    'title' => t('Title'),
    'help' => t('A Page Title alternative to the Node: Title field.'),
    'field' => array(
      'field' => 'page_title', // The real field.
      'group' => t('Page Title'), // The group it appears in on the UI.
      'handler' => 'views_handler_field_node_page_title',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a Page Title as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_string', // We can "borrow" the node handler.
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string', // We can "borrow" the node handler.
    ),
  );
  return $data;
}


/**
 * Implementation of hook_views_handlers().
 */
function page_title_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'page_title'),
    ),
    'handlers' => array(
      // field handlers
      'views_handler_field_node_page_title' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
