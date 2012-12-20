<?php

class ScrachpadsMenuMigration extends DrupalMenu6Migration{

  /**
   * Query for the basic menu data.
   *
   * @return QueryConditionInterface
   */
  protected function menuQuery(){
    $query = parent::menuQuery();
    $query->condition('menu_name', array(
      'navigation',
      'primary-links',
      'secondary-links'
    ), 'NOT IN');
    return $query;
  }
  
  protected function postImport() {
    parent::postImport();
    menu_cache_clear_all();
  }
  
}