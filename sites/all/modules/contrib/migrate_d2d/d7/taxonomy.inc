<?php


class DrupalTerm7Migration extends DrupalTermMigration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    // @todo: Support translation of formats
    $this->addFieldMapping('format', 'format');
  }

  protected function termQuery() {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('taxonomy_term_data', 'td')
             ->fields('td', array('tid', 'name', 'description', 'weight', 'format'))
             ->orderBy('parent')
             ->distinct();
    $query->leftJoin('taxonomy_term_hierarchy', 'th', 'td.tid=th.tid');
    $query->fields('th', array('parent'));
    $query->innerJoin('taxonomy_vocabulary', 'v', 'td.vid=v.vid');
    $query->condition('v.machine_name', $this->sourceVocabulary, 'IN');
    return $query;
  }
}
