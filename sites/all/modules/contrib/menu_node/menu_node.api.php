<?php

// $Id: menu_node.api.php,v 1.1 2011/01/02 20:00:29 agentken Exp $

/**
 * @file
 * Menu Node API
 * API documentation file for the Menu Node API module.
 */

/**
 * When a menu item is created for a node, notify other modules.
 *
 * @param $link
 *   An object representing a single row from the {menu_links} table.
 *   This object defines the menu link and can be used to load additional
 *   data using menu_get_item().
 * @param $node
 *   The node object being acted upon.
 *
 * @see menu_get_item()
 * @see menu_save_item()
 */
function hook_menu_node_insert($link, $node) {
  // Store data in my custom table, which tracks the owners of nodes
  // placed in the site menu.
  $record = array(
    'nid' => $node->nid,
    'mlid' => $link->mlid,
    'uid' => $node->uid,
  );
  drupal_write_record('mytable', $record);
}

/**
 * When a node or its menu item are updated, notify other modules.
 *
 * Note that this hook runs for each menu item that belongs to the node
 * (yes, core allows that), so normally you would use $link->mlid as the
 * primary key.
 *
 * @param $link
 *   An object representing a single row from the {menu_links} table.
 *   This object defines the menu link and can be used to load additional
 *   data using menu_get_item().
 * @param $node
 *   The node object being acted upon.
 */
function hook_menu_node_update($link, $node) {
  // Update data in my custom table, which tracks the owners of nodes
  // placed in the site menu.
  $record = array(
    'nid' => $node->nid,
    'mlid' => $link->mlid,
    'uid' => $node->uid,
  );
  drupal_write_record('mytable', $record, array('mlid', 'uid'));
}

/**
 * When a node or its menu item are deleted, notify other modules.
 *
 * Note that this hook runs for each menu item that belongs to the node
 * (yes, core allows that), so normally you would use $link->mlid as the
 * primary key.
 *
 * @param $link
 *   An object representing a single row from the {menu_links} table.
 *   This object defines the menu link and can be used to load additional
 *   data using menu_get_item().
 * @param $node
 *   The node object being acted upon.
 */
function hook_menu_node_delete($link, $node) {
  // Delete data in my custom table, which tracks the owners of nodes
  // placed in the site menu.
  db_delete('mytable')
    ->condition('mlid', $link->mlid)
    ->execute();
}
