<?php

/**
 * @file
 * Views integration for Feeds tables.
 *
 * @see http://drupal.org/project/views
 */

/**
 * Implements hook_views_data().
 */
function feeds_views_data() {
  $data = array();

  /**
   * Expose feeds_source table to views.
   */
  $data['feeds_source']['table'] = array(
    'group' => 'Feeds source',
    'base' => array(
      'field' => array('feed_nid', 'id'),
      'title' => 'Feeds source',
      'help' => 'The source information for a feed, for example its URL',
    ),
  );
  $data['feeds_source']['feed_nid'] = array(
    'title' => 'Feed node id',
    'help' => 'Contains the node id of a feed node if the feed\'s configuration is attached to a content type, otherwise contains 0.',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
      'help' => 'Filter on a Feeds Source\'s feed_nid field.',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'validate type' => 'nid',
      'help' => 'Argument on a Feeds Source\'s feed_nid field.',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => 'Sort Feeds Source\'s feed_nid field.',
    ),
  );
  $data['feeds_source']['config'] = array(
    'title' => 'Source',
    'help' => 'The resource identifier for this feed. For instance a URL.',
    'field' => array(
      'handler' => 'feeds_views_handler_field_source',
      'click sortable' => FALSE,
    ),
  );
  $data['feeds_source']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'feed_nid',
      'type' => 'LEFT',
    ),
  );

  /**
   * Expose feeds_node_item table to views.
   */
  $data['feeds_node_item']['table'] = array(
    'group' => 'Feeds item',
    'join' => array(
      'node' => array(
        'left_field' => 'nid',
        'field' => 'nid',
        'type' => 'LEFT',
      ),
    ),
  );
  $data['feeds_node_item']['feed_nid'] = array(
    'title' => t('Owner feed nid'),
    'help' => t('The node id of the owner feed node if available.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
      'help' => t('Filter on Feed Items by the Feed they were generated from using the Node Id of the Feed Node.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'validate type' => 'nid',
      'help' => t('Argument on Feed Items by the Feed they were generated from using the Node Id of the Feed Node.'),
    ),
    'sort' => array(
      'help' => t('Sort Feed Items by the Feed they were generated from using the Node Id of the Feed Node.'),
    ),
    'relationship' => array(
      'title' => t('Owner feed'),
      'help' => t('Relate a feed item to its owner feed node if available.'),
      'label' => t('Owner feed'),
      'base' => 'node',
      'base field' => 'nid',
    ),
  );
  $data['feeds_node_item']['url'] = array(
    'title' => t('Item URL'),
    'help' => t('Contains the URL of the feed item.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
      'help' => t('Filter on a Feeds Item\'s URL field.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'numeric' => TRUE,
      'validate type' => 'nid',
      'help' => t('Argument on a Feeds Item\'s URL field.'),
    ),
    'sort' => array(
      'help' => t('Sort on a Feeds Item\'s URL field.'),
    ),
  );
  $data['feeds_node_item']['guid'] = array(
    'title' => t('Item GUID'),
    'help' => t('Contains the GUID of the feed item.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
      'help' => t('Filter on a Feeds Item\'s GUID field.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'numeric' => TRUE,
      'validate type' => 'nid',
      'help' => t('Argument on a Feeds Item\'s GUID field.'),
    ),
    'sort' => array(
      'help' => t('Sort on a Feeds Item\'s GUID field.'),
    ),
  );
  $data['feeds_node_item']['imported'] = array(
    'title' => t('Import date'),
    'help' => t('Contains the import date of the feed item.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort on a Feeds Item\'s import date field.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'help' => t('Filter on a Feeds Item\'s import date field.'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'numeric' => TRUE,
      'validate type' => 'nid',
      'help' => t('Argument on a Feeds Item\'s import date field.'),
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function feeds_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'feeds') .'/views',
    ),
    'handlers' => array(
      // field handlers
      'feeds_views_handler_field_source' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
