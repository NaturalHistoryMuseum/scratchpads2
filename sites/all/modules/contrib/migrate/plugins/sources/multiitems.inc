<?php

/**
 * @file
 * Support for migration from sources where data spans multiple lines
 * (ex. xml, json) and IDs for the items are part of each item and multiple
 * items reside in a single file.
 */

/**
 * Extend the MigrateItems class to provide a means to obtain a list of IDs to
 * be migrated from a given source (e.g., MigrateItemsXML extends MigrateItem to
 * obtain a list of IDs from an XML document).  This class also provides a means
 * to obtain the data for a given migratable item given its ID.
 */
abstract class MigrateItems {

  public function __construct() {
  }

  /**
   * Implementors are expected to return a string representing where the listing
   * is obtained from (a URL, file directory, etc.)
   *
   * @return string
   */
  abstract public function __toString();

  /**
   * Implementors are expected to return an array of unique IDs, suitable for
   * passing to the MigrateItem class to retrieve the data for a single item.
   *
   * @return Mixed, iterator or array
   */
  abstract public function getIdList();

  /**
   * Implementors are expected to return a count of IDs available to be
   * migrated.
   *
   * @return int
   */
  abstract public function computeCount();

  /**
   * Implementors are expected to return an object representing a source item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  abstract public function getItem($id);

  /**
   * Implementors may optionally implement a hash function, applied to the
   * entire source row, if this particular item type makes it difficult to
   * do on the raw row.
   *
   * @param $row
   *
   * @return mixed
   */
  //abstract public function hash($row);
}

/**
 * Implementation of MigrateItems, for providing a list of IDs and for
 * retrieving a parsed XML document given an ID from this list.
 */

/**
 * Implementation of MigrateSource, providing the semantics of iterating over
 * IDs provided by a MigrateItems and retrieving data from a MigrateItems.
 */
class MigrateSourceMultiItems extends MigrateSource {

  /**
   * MigrateItems object used to obtain the list of IDs and source for
   * all objects.
   *
   * @var MigrateItems
   */
  protected $itemsClass;

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Iterator of IDs from the listing class.
   *
   * @var Iterator
   */
  protected $idIterator;

  /**
   * List of item IDs to iterate.
   *
   * @var array
   */
  protected $idsToProcess = array();

  /**
   * Simple initialization.
   */
  public function __construct(MigrateItems $items_class, $fields = array(), $options = array()) {
    parent::__construct($options);

    $this->itemsClass = $items_class;
    $this->fields = $fields;
  }

  /**
   * Return a string representing the source.
   *
   * @return string
   */
  public function __toString() {
    return (string) $this->itemsClass;
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   * Since we can't reliably figure out what "fields" are in the source,
   * it's up to the implementing Migration constructor to fill them in.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    return $this->fields;
  }

  /**
   * It's the list class that knows how many records are available, so ask it.
   *
   * @return int
   */
  public function computeCount() {
    // @API: Support old count method for now.
    if (method_exists($this->itemsClass, 'computeCount')) {
      return $this->itemsClass->computeCount();
    }
    else {
      return $this->itemsClass->count();
    }
  }

  /**
   * Implementation of MigrateSource::performRewind().
   *
   * @return void
   */
  public function performRewind() {
    // If there isn't a specific ID list passed in, get it from the list class.
    if ($this->idList) {
      $this->idsToProcess = $this->idList;
    }
    elseif (!$this->idsToProcess = $this->itemsClass->getIdList()) {
      $this->idsToProcess = array();
    }
    $this->idIterator = ($this->idsToProcess instanceof Iterator) ?
      $this->idsToProcess : new ArrayIterator($this->idsToProcess);
    $this->idIterator->rewind();
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * @return null|stdClass
   */
  public function getNextRow() {
    $row = NULL;
    while ($this->idIterator->valid()) {
      $id = $this->idIterator->current();
      $this->idIterator->next();

      // Skip empty IDs
      if (empty($id)) {
        continue;
      }

      // Got a good ID, get the data and get out.
      $row = $this->itemsClass->getItem($id);
      if ($row) {
        // Save the ID using the map source key - it will be used for mapping
        $sourceKey = $this->activeMap->getSourceKey();
        $key_name = key($sourceKey);
        $row->{$key_name} = $id;
        break;
      }
    }
    return $row;
  }

  /**
   * Overrides MigrateSource::hash().
   */
  protected function hash($row) {
    // Let the item class override the default hash function.
    if (method_exists($this->itemsClass, 'hash')) {
      $hash = $this->itemsClass->hash($row);
    }
    else {
      $hash = parent::hash($row);
    }
    return $hash;
  }
}
