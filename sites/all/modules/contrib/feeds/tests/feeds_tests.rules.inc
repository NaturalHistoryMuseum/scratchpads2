<?php

/**
 * @file
 * Includes any rules integration provided by the module.
 */

/**
 * Implements hook_rules_event_info().
 */
function feeds_tests_rules_event_info() {
  return array(
    'feeds_tests_rules_event' => array(
      'label' => t('Test event'),
      'group' => t('Feeds'),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function feeds_tests_rules_action_info() {
  $items['feeds_tests_create_node'] = array(
    'label' => t('Create a node'),
    'group' => t('Rules test'),
  );
  return $items;
}

/**
 * Rules action callback: creates a node.
 */
function feeds_tests_create_node() {
  $node = new stdClass();
  $node->title = 'Test node';
  $node->type = 'page';
  $node->status = 1;
  $node->uid = 0;
  node_object_prepare($node);
  node_save($node);
}
