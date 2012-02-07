<?php /* -*- mode: php; indent-tabs-mode: nil; tab-width: 2; -*- */

/**
 * @file
 * This file handles views api definitions.
 * We describe the table to Views module 
 * as well as other necessary stuff to integrate
 */

/**
 * Implementation of hook_views_data()
 */ 
function content_lock_views_data(){
  $data = array();

  // Our group in Views UI
  $data['content_lock']['table']['group'] = t('Content Lock');

  // Our join type. Only with node table
  $data['content_lock']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'user' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    )
  );

  // Relationship. This might be useful
  $data['content_lock']['nid'] = array(
    'title' => t('Node locked'),
    'help' => t('The node being locked'),
    'relationship' => array(
      'label' => t('Node locked'),
      'base' => 'node',
      'base field' => 'nid',
      'skip base' => array('node'),
    ),
  );
  $data['content_lock']['uid'] = array(
    'title' => t('Lock Owner'),
    'help' => t('The user locking the node'),
    'relationship' => array(
      'label' => t('Lock Owner'),
      'base' => 'users',
      'base field' => 'uid',
      'skip base' => array('user'),
    ),
  );
  
  // Our Handlers:
  $data['content_lock']['persistent'] = array(
    'title' => t('Is persistent'),
    'help' => t('Whether this node has been persistently locked'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Persistent'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['content_lock']['timestamp'] = array(
    'title' => t('Lock Date/Time'),
    'help' => t('Timestamp of the lock'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['content_lock']['is_locked'] = array(
    'title' => t('Is Locked'),
    'help' => t('Whether the node is currently locked'),
    'field' => array(
      'handler' => 'views_handler_field_is_locked',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_is_locked',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_is_locked',
      'click sortable' => TRUE,
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_handlers()
 */ 
function content_lock_views_handlers(){
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'content_lock') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_is_locked' => array(
        'parent' => 'views_handler_field_boolean',
      ),
      'views_handler_sort_is_locked' => array(
        'parent' => 'views_handler_sort',
      ),
      'views_handler_filter_is_locked' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}
