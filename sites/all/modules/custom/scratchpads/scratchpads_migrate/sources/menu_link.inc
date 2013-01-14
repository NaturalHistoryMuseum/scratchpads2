<?php

class ScratchpadMenuLinkMigration extends DrupalMenuLinks6Migration{

  protected function menuLinksQuery(){
    $query = parent::menuLinksQuery();
    $query->condition('menu_name', array(
      'navigation',
      'primary-links',
      'secondary-links'
    ), 'NOT IN');
    return $query;
  }
}
