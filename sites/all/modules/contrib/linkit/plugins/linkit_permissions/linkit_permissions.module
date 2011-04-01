<?php 

/**
 * @file
 * Extend Linkit with permissions.
 */

/**
 * Implements hook_perm().
 */
function linkit_permissions_permission() {
  $perms = array(
    'use all linkit plugins' => array(
      'title' => t('Use all Linkit plguins'), 
    ),
  );
  foreach (module_invoke_all('linkit_info_plugins') as $plugin) {
    $perms['use linkit ' . check_plain($plugin['type']) . ' plugin'] = array(
      'title' => t('Use Linkit %type plugin', array('%type' => $plugin['type'])), 
      );
    };
  return $perms;
}