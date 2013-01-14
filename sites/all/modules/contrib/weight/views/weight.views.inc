<?php

/**
 * Implements hook_views_data().
 */
function weight_views_data() {
  $data['weight_weights']['table'] = array(
    'group' => t('Weight'),
    'join' => array(
      'node' => array(
        'left_field' => 'nid',
        'field' => 'entity_id',
      ),
    ),
  );

  $data['weight_weights']['weight'] = array(
    'title' => t('Weight'),
    'help' => t('The weight of the node, formatted as a form element.'),
    'field' => array(
      'handler' => 'weight_handler_field_weight',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['weight_weights']['weight_value'] = array(
    'title' => t('Weight value'),
    'help' => t('The weight of the node.'),
    'real field' => 'weight',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );

  $data['weight_settings']['table']['join']['node'] = array(
    'left_field' => 'type',
    'field' => 'type',
  );

  $data['weight_settings']['weight_enabled'] = array(
    'group' => t('Weight'),
    'title' => t('Enabled'),
    'help' => t('Displays nodes that are Weight enabled'),
    'filter' => array(
      'handler' => 'weight_handler_filter_weight_enabled',
    ),
  );

  return $data;
}

/**
 * Implements of hook_views_handlers().
 */
function weight_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'weight') . '/views',
    ),
    'handlers' => array(
      'weight_handler_field_weight' => array(
        'parent' => 'views_handler_field',
      ),
      'weight_handler_filter_weight_enabled' => array(
        'parent' => 'views_handler_filter',
      ),
    ),
  );
}
