<?php

/**
 * @file
 * Support for menu link destinations.
 */

/**
 * Destination class implementing migration into {menu_links}.
 */
class MigrateDestinationMenuLinks extends MigrateDestination {

  static public function getKeySchema() {
    return array(
      'mlid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'ID of destination link',
      ),
    );
  }

  public function __construct() {
    parent::__construct();
  }

  public function __toString() {
    $output = t('Menu links');
    return $output;
  }

  /**
   * Returns a list of fields available to be mapped for menu links.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array(
      'menu_name' => t('The menu name. All links with the same menu name (such as \'navigation\') are part of the same menu.'),
      'mlid' => t('The menu link ID (mlid) is the integer primary key.'),
      'plid' => t('The parent link ID (plid) is the mlid of the link above in the hierarchy, or zero if the link is at the top level in its menu.'),
      'link_path' => t('The Drupal path or external path this link points to.'),
      'router_path' => t('For links corresponding to a Drupal path (external = 0), this connects the link to a {menu_router}.path for joins.'),
      'link_title' => t('The text displayed for the link, which may be modified by a title callback stored in {menu_router}.'),
      'options' => t('A serialized array of options to be passed to the url() or l() function, such as a query string or HTML attributes.'),
      'module' => t('The name of the module that generated this link.'),
      'hidden' => t('A flag for whether the link should be rendered in menus. (1 = a disabled menu item that may be shown on admin screens, -1 = a menu callback, 0 = a normal, visible link)'),
      'external' => t('A flag to indicate if the link points to a full URL starting with a protocol, like http:// (1 = external, 0 = internal).'),
      'has_children' => t('Flag indicating whether any links have this link as a parent (1 = children exist, 0 = no children).'),
      'expanded' => t('Flag for whether this link should be rendered as expanded in menus - expanded links always have their child links displayed, instead of only when the link is in the active trail (1 = expanded, 0 = not expanded)'),
      'weight' => t('Link weight among links in the same menu at the same depth.'),
      'depth' => t('The depth relative to the top level. A link with plid == 0 will have depth == 1.'),
      'customized' => t('A flag to indicate that the user has manually created or edited the link (1 = customized, 0 = not customized).'),
      'p1' => t('The first mlid in the materialized path. If N = depth, then pN must equal the mlid. If depth > 1 then p(N-1) must equal the plid. All pX where X > depth must equal zero. The columns p1 .. p9 are also called the parents.'),
      'p2' => t('The second mlid in the materialized path. See p1.'),
      'p3' => t('The third mlid in the materialized path. See p1.'),
      'p4' => t('The fourth mlid in the materialized path. See p1.'),
      'p5' => t('The fifth mlid in the materialized path. See p1.'),
      'p6' => t('The sixth mlid in the materialized path. See p1.'),
      'p7' => t('The seventh mlid in the materialized path. See p1.'),
      'p8' => t('The eighth mlid in the materialized path. See p1.'),
      'p9' => t('The ninth mlid in the materialized path. See p1.'),
      'updated' => t('Flag that indicates that this link was generated during the update from Drupal 5.'),
    );
    return $fields;
  }

  /**
   * Import a single row.
   *
   * @param $menu_link
   *  Menu link object to build. Prefilled with any fields mapped in the
   *   Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $menu_link, stdClass $row) {
    // Updating previously-migrated content
    if (isset($row->migrate_map_destid1)) {
      $menu_link->mlid = $row->migrate_map_destid1;
    }

    // Load old values if necessary.
    $migration = Migration::currentMigration();
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($menu_link->mlid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination mlid provided'));
      }
      if (!$old_menu_link = menu_link_load($menu_link->mlid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, and the provided mlid could not be found'));
      }
      $menu_link_to_update = (object) $old_menu_link;
      foreach ($old_menu_link as $key => $value) {
        if (!isset($menu_link->{$key})) {
          $menu_link->{$key} = $old_menu_link[$key];
        }
      }
    }

    // Invoke migration prepare handlers
    // @todo derive existing mlids?
    $this->prepare($menu_link, $row);

    // Menu links are handled as arrays, so clone the object to an array.
    $item = clone $menu_link;
    $item = (array) $item;

    migrate_instrument_start('menu_link_save');

    // Check to see if this is a new menu item.
    $update = FALSE;
    if (isset($item['mlid'])) {
      $update = TRUE;
      $mlid = menu_link_save($item);
    }
    else {
      // menu_link_save() should return an mlid integer.
      $mlid = menu_link_save($item);
    }
    migrate_instrument_stop('menu_link_save');

    // Return the new id or FALSE on failure.
    if (!empty($mlid)) {
      // Increment the count if the save succeeded.
      if ($update) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      // Return the primary key to the mapping table.
      $return = array($mlid);
    }
    else {
      $return = FALSE;
    }

    // Invoke migration complete handlers.
    $menu_link = (object) menu_link_load($mlid);
    $this->complete($menu_link, $row);

    return $return;
  }

  /**
   * Implementation of MigrateDestination::prepare().
   */
  public function prepare($menu_link, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $menu_link->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any general handlers.
    migrate_handler_invoke_all('menu_links', 'prepare', $menu_link, $row);
    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($menu_link, $row);
    }
  }

  /**
   * Implementation of MigrateDestination::complete().
   */
  public function complete($menu_link, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $menu_link->migrate = array(
      'machineName' => $migration->getMachineName(),
    );
    // Call any general handlers.
    migrate_handler_invoke_all('menu_links', 'complete', $menu_link, $row);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($menu_link, $row);
    }
  }

  /**
   * Implementation of MigrateDestination::postImport().
   */
  public function postImport() {
    // Clear the cache after all menu links are imported.
    menu_cache_clear_all();
  }

  /**
   * Delete a single menu item.
   *
   * @param $id
   *  Array of fields representing the key (in this case, just mlid).
   */
  public function rollback($id) {
    $mlid = reset($id);

    migrate_instrument_start('menu_link_delete');
    $this->prepareRollback($mlid);
    // @todo: any error checking here? For example, menu.inc has:
    //   if ($menu = menu_load($menu_name)) { menu_delete($menu) }
    menu_link_delete($mlid);
    $this->completeRollback($mlid);
    migrate_instrument_stop('menu_link_delete');
  }

  /**
   * Give handlers a shot at cleaning up before a menu has been rolled back.
   *
   * @param $mlid
   *  ID of the menu link about to be deleted.
   */
  public function prepareRollback($mlid) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('menu_links', 'prepareRollback', $mlid);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'prepareRollback')) {
      $migration->prepareRollback($mlid);
    }
  }

  /**
   * Give handlers a shot at cleaning up after a menu has been rolled back.
   *
   * @param $mlid
   *  ID of the menu link which has been deleted.
   */
  public function completeRollback($mlid) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('menu_links', 'completeRollback', $mlid);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'completeRollback')) {
      $migration->completeRollback($mlid);
    }
  }

  /**
   * Implementation of MigrateDestination::postRollback().
   */
  public function postRollback() {
    // Clear the cache after all menu links are rolled back.
    menu_cache_clear_all();
  }
}
