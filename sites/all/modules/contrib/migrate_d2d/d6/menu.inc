<?php

/*
 * Class for menu migrations: from {menu_custom} into {menu_custom}.
 * Dependent on this patch for Migrate:
 * http://drupal.org/node/1403044#comment-5740118
 */
class DrupalMenu6Migration extends DrupalMenuMigration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);
  }

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function menuQuery() {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('menu_custom', 'm')
             ->fields('m');
    return $query;
  }

  /**
   * Review a data row after fetch, returning FALSE to skip it.
   *
   * @param $row
   * @return bool
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // D6 prefixes menus added through the UI with 'menu-'
    if ($row->menu_name == 'secondary-links' ||
        substr($row->menu_name, 0, 5) === 'menu-') {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
}
