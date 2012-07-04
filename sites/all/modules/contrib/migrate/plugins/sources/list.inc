<?php

/**
 * @file
 * Support for migration from sources with distinct means of listing items to
 * import and obtaining the items themselves.
 *
 * TODO: multiple-field source keys
 */


/**
 * Extend the MigrateList class to provide a means to obtain a list of IDs to
 * be migrated from a given source (e.g., MigrateListXML extends MigrateList to
 * obtain a list of IDs from an XML document).
 */
abstract class MigrateList {
  public function __construct() {}

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
   * Implementors are expected to return a count of IDs available to be migrated.
   *
   * @return int
   */
  abstract public function computeCount();
}

/**
 * Extend the MigrateItem class to provide a means to obtain the data for a
 * given migratable item given its ID as provided by the MigrateList class.
 */
abstract class MigrateItem {
  public function __construct() {}

  /**
   * Implementors are expected to return an object representing a source item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  abstract public function getItem($id);
}

/**
 * Implementation of MigrateSource, providing the semantics of iterating over
 * IDs provided by a MigrateList and retrieving data from a MigrateItem.
 */
class MigrateSourceList extends MigrateSource {
  /**
   * MigrateList object used to obtain ID lists.
   *
   * @var MigrateList
   */
  protected $listClass;

  /**
   * MigrateItem object used to obtain the source object for a given ID.
   *
   * @var MigrateItem
   */
  protected $itemClass;

  /**
   * Iterator of IDs from the listing class.
   *
   * @var Iterator
   */
  protected $idIterator;

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Simple initialization.
   */
  public function __construct(MigrateList $list_class, MigrateItem $item_class, $fields = array(),
      $options = array()) {
    parent::__construct($options);
    $this->listClass = $list_class;
    $this->itemClass = $item_class;
    $this->fields = $fields;
  }

  /**
   * Return a string representing the source.
   *
   * @return string
   */
  public function __toString() {
    return (string) $this->listClass;
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
    if (method_exists($this->listClass, 'computeCount')) {
      return $this->listClass->computeCount();
    }
    else {
      return $this->listClass->count();
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
    else {
      $this->idsToProcess = $this->listClass->getIdList();
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
      $ids = $this->idIterator->current();
      $this->idIterator->next();

      // Skip empty IDs
      if (empty($ids)) {
        continue;
      }

      // Got a good ID, get the data and get out.
      $row = $this->itemClass->getItem($ids);
      if ($row) {
        // No matter what $ids is, be it a string, integer, object, or array, we
        // cast it to an array so that it can be properly mapped to the source
        // keys as specified by the map. This is done after getItem is called so
        // that the ItemClass doesn't have to care about this requirement.
        $ids = (array) $ids;
        foreach (array_keys($this->activeMap->getSourceKey()) as $key_name) {
          // Grab the first id and advance the array cursor. Then save the ID
          // using the map source key - it will be used for mapping.
          list(, $id) = each($ids);
          $row->$key_name = $id;
        }
      }
      break;
    }
    return $row;
  }
}
