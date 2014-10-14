<?php

/**
 * @file
 * Contains the RdfIndexerAlterEntityPublic class.
 */

/**
 * Exclude private entities from indexes.
 */
class RdfIndexerAlterEntityPublic extends SearchApiAbstractAlterCallback {

  /**
   * Check whether this data-alter callback is applicable for a certain index.
   *
   * Returns TRUE only if the entity API module is enabled.
   *
   * @param SearchApiIndex $index
   *   The index to check for.
   *
   * @return boolean
   *   TRUE if the callback can run on the given index; FALSE otherwise.
   */
  public function supportsIndex(SearchApiIndex $index) {
    return module_exists('entity');
  }

  /**
   * Alter items before indexing.
   *
   * Items which are removed from the array won't be indexed, but will be marked
   * as clean for future indexing.
   *
   * @param array $items
   *   An array of items to be altered, keyed by item IDs.
   */
  public function alterItems(array &$items) {
    foreach ($items as $id => &$item) {
      $item_type = $this->index->item_type;
      if (!entity_access('view', $item_type, entity_load_single($item_type, $id), user_load(0))) {
        //watchdog('rdf_indexer', t('Excluded private @type entity @id from index @index', array('@type' => $item_type, '@id' => $id, '@index' => $this->index->name)), NULL, WATCHDOG_INFO);
        unset($items[$id]);
      }
    }
  }
}
