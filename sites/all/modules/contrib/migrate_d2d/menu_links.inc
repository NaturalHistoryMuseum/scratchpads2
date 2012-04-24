<?php

/*
 * Class for menu link migrations: from {menu_links} into {menu_links}.
 * Dependent on this patch for Migrate:
 * http://drupal.org/node/1403044#comment-5740118
 */
abstract class DrupalMenuLinksMigration extends DrupalMigration {

  protected $menu_migration = '';
  protected $node_migrations = array();
  protected $term_migrations = array();
  protected $path_migration = '';

  /**
   * @param array $arguments
   *     $arguments['menu_migration'] should be defined -- menu links are normally
   *        dependent on menus.
   *     $arguments['node_migrations'] is an optional array of node migrations,
   *        required in order to update nids in link_path
   *     $arguments['term_migrations'] is an optional array of term migrations,
   *        required in order to update tids in link_path
   *     $arguments['path_migration'] is an optional string defining the path migration,
   *        required in order to update URL aliases in link_path
   *
   * @todo: should the arguments be handled here or in d6/menu_links?
   */
  public function __construct(array $arguments) {
    if (!empty($arguments['menu_migration'])) {
      $this->menu_migration = $arguments['menu_migration'];
      $this->dependencies[] = $this->menu_migration;
    }
    if (!empty($arguments['node_migrations'])) {
      $this->node_migrations = $arguments['node_migrations'];
      foreach ($this->node_migrations as $node_migration) {
        $this->dependencies[] = $node_migration;
      }
    }
    if (!empty($arguments['term_migrations'])) {
      $this->term_migrations = $arguments['term_migrations'];
      foreach ($this->term_migrations as $term_migration) {
        $this->dependencies[] = $term_migration;
      }
    }
    if (!empty($arguments['path_migration'])) {
      $this->path_migration = $arguments['path_migration'];
      $this->dependencies[] = $this->path_migration;
    }

    parent::__construct($arguments);

    // Create our three main objects - source, destination, and map
    $this->source = new MigrateSourceSQL($this->menuLinksQuery(), $this->sourceFields,
      NULL, $this->sourceOptions);

    $this->destination = new MigrateDestinationMenuLinks();

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'mlid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'ID of destination link',
        ),
      ),
      MigrateDestinationMenuLinks::getKeySchema()
    );
  }

  /**
   * @abstract
   * Query for the basic menu_links data.
   *
   * @return QueryConditionInterface
   */
  abstract protected function menuLinksQuery();

  /**
   * Creates a stub menu link, for when a child is imported before its parent
   *
   * @param $migration
   *  The source migration
   * @return
   *  int $mlid on success
   *  FALSE on failure
   */
  protected function createStub($migration) {
    // if plid is 0, that means it has no parent, so don't create a stub
    if (!$migration->sourceValues->plid) {
      return FALSE;
    }
    $menu_link = array (
      'menu_name' => $migration->sourceValues->menu_name,
      'link_path' => 'stub-path',
      'router_path' => 'stub-path',
      'link_title' => t('Stub title'),
    );
    $mlid = menu_link_save($menu_link);
    if ($mlid) {
      return array($mlid);
    }
    else {
      return FALSE;
    }
  }
}
