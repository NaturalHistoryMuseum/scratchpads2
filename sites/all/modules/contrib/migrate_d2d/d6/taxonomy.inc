<?php

/**
 * Handling specific to a Drupal 6 source for taxonomy terms.
 */
class DrupalTerm6Migration extends DrupalTermMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    // Drupal 6 had no format for terms.
    $this->addFieldMapping('format');
  }

  /**
   * Implementation of DrupalTermMigration::termQuery().
   *
   * @return SelectQueryInterface
   */
  protected function termQuery() {
    // Note the explode - this supports the (admittedly unusual) case of
    // consolidating multiple vocabularies into one.
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('term_data', 'td')
             ->fields('td', array('tid', 'name', 'description', 'weight'))
             ->condition('vid', explode(',', $this->sourceVocabulary), 'IN')
             ->orderBy('parent')
             ->distinct();
    // Join to the hierarchy so we can sort on parent, but we'll pull the
    // actual parent values in separately in case there are multiples.
    $query->leftJoin('term_hierarchy', 'th', 'td.tid=th.tid');
    return $query;
  }

  /**
   * Implementation of Migration::prepareRow().
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Add the (potentially multiple) parents
    $result = Database::getConnection('default', $this->sourceConnection)
              ->select('term_hierarchy', 'th')
              ->fields('th', array('parent'))
              ->condition('tid', $row->tid)
              ->execute();
    foreach ($result as $parent_row) {
      $row->parent[] = $parent_row->parent;
    }
  }
}
