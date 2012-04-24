<?php

/*
 * Class for menu link migrations: from {menu_links} into {menu_links}.
 * Dependent on this patch for Migrate:
 * http://drupal.org/node/1403044#comment-5740118
 */
class DrupalMenuLinks6Migration extends DrupalMenuLinksMigration {
  /**
   * @param array $arguments
   *     $arguments['menu_migration'] should be defined -- menu links are normally
   *        dependent on menus.
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Most mappings are straightforward
    $this->addSimpleMappings(array('router_path', 'link_title',
        'module', 'hidden', 'external', 'has_children',
        'expanded', 'weight', 'depth', 'customized', 'p1', 'p2', 'p3', 'p4',
        'p5', 'p6', 'p7', 'p8', 'p9', 'updated'));

/*    // If menus have been migrated, menu_name is taken from menu_migration.
    // Otherwise, we can assume we are only adding to existing menus.
    // @todo: Would anyone ever migrate menu_links but not menu, or can we make it
    //   a forced dependency?
    if (!empty($arguments['menu_migration'])) {
      $menu_migration = $arguments['menu_migration'];
      $this->addFieldMapping('menu_name', 'menu_name')
           ->sourceMigration($menu_migration);
    }
    else {
      $this->addFieldMapping('menu_name', 'menu_name');
    }
*/
    $this->addFieldMapping('menu_name', 'menu_name');

    // plid is taken from this migration. If the parent hasn't yet been migrated,
    // we need to create a stub.
    $this->addFieldMapping('plid', 'plid')
         ->sourceMigration($this->getMachineName());

    // @todo: What do we do if pathing changes? Can/Should we make this dependent on
    //   an implementation of MigratePathEntityHandler, if it exists?
    // Most likely this will involve a callback.
    $this->addFieldMapping('link_path', 'link_path');

    // Ran into trouble with options, so I'm leaving it out for now
    $this->addUnmigratedSources(array('options'));
    $this->addUnmigratedDestinations(array('options'));
  }

  /**
   * Query for the basic menu_links data.
   *
   * @return QueryConditionInterface
   */
  protected function menuLinksQuery() {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('menu_links', 'm')
             ->fields('m');
    return $query;
  }

  /**
   * Review a data row after fetch, returning FALSE to skip it.
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    if (!($row->menu_name == 'primary-links' ||
        $row->menu_name == 'secondary-links' ||
        substr($row->menu_name, 0, 5) == 'menu-')) {
      return FALSE;
    }

    if ($row->menu_name == 'primary-links') {
      $row->menu_name = 'main-menu';
    }

    /*
     * Fun with link_path!
     *
     * check for nids
     *   if pattern 'node/nid', replace nid with handleSourceMigration($node_migrations, $nid)
     * check for tids
     *   if pattern 'term/tid', replace tid with handleSourceMigration($term_migrations, $tid)
     * check for potential alias (after basepath, I believe)
     *   if (handleSourceMigration($path_migration, $potentialalias), replace potentialalias with result
     */

    $matches = array();
    $value = '';

    // NIDs first
    if (!empty($this->node_migrations)) {
      // Grabs numbers preceded by 'node/', followed by end of string or '/'
      // eg: 'node/63' -> '63', 'node/17/edit' -> '17', 'node/53ea' -> NULL
      //
      // @todo: extend the lookbehind so it returns NULL for inputs like 'awetnode/34'
      $regex_nid = "/(?<=node\/)[\d]+(?=$|\/)/";

      if (preg_match($regex_nid, $row->link_path, $matches)) {
        if ($value = $this->handleSourceMigration($this->node_migrations, $matches[0])) {
          $new_path = preg_replace('/' . $matches[0] . '/', $value, $row->link_path);
          if ($new_path) {
            $row->link_path = $new_path;
          }
          else {
          }
        }
        else {
        }
      }
    }

    // @todo: handle tid and url_alias
  }

  /**
   * Actions to take after a menu has been saved.
   *
   * @param $menu
   * @param $row
   */
  public function complete($menu, $row) {
    // @todo: anything needed here? Keeping complete() as a placeholder for now
  }
}
