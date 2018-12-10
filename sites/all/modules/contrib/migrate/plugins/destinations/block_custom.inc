<?php

/**
 * @file
 * Support for custom block destinations.
 */

/**
 * Destination class implementing migration into {block_custom}.
 */
class MigrateDestinationCustomBlock extends MigrateDestination {

  static public function getKeySchema() {
    return array(
      'bid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'ID of destination custom block',
      ),
    );
  }

  public function __construct() {
    parent::__construct();
  }

  public function __toString() {
    $output = t('Custom blocks');
    return $output;
  }

  /**
   * Returns a list of fields available to be mapped for custom blocks.
   *
   * @param Migration $migration
   *  Optionally, the migration containing this destination.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields($migration = NULL) {
    $fields = array(
      'bid' => t('The custom block ID (bid).'),
      'body' => t('Block contents.'),
      'info' => t('Block description.'),
      'format' => t('The {filter_format}.format of the block body.'),
    );
    return $fields;
  }

  /**
   * Import a single row.
   *
   * @param $block
   *  Custom block object to build. Prefilled with any fields mapped in the
   *   Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   *
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $block, stdClass $row) {
    // Updating previously-migrated content
    if (isset($row->migrate_map_destid1)) {
      $block->bid = $row->migrate_map_destid1;
    }

    // Load old values if necessary.
    $migration = Migration::currentMigration();
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($block->bid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination bid provided'));
      }
      if (!$old_block = $this->loadCustomBlock($block->bid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, and the provided bid could not be found'));
      }
      $block_to_update = (object) $old_block;
      foreach ($old_block as $key => $value) {
        if (!isset($block->{$key})) {
          $block->{$key} = $old_block[$key];
        }
      }
    }

    // Invoke migration prepare handlers
    $this->prepare($block, $row);

    // Custom blocks are handled as arrays, so clone the object to an array.
    $item = clone $block;
    $item = (array) $item;

    migrate_instrument_start('block_custom_save');

    // Check to see if this is a new custom block.
    $update = FALSE;
    if (isset($item['bid'])) {
      $update = TRUE;
      $bid = $this->saveCustomBlock($item);
    }
    else {
      $bid = $this->saveCustomBlock($item);
    }
    migrate_instrument_stop('block_custom_save');

    // Return the new id or FALSE on failure.
    if (!empty($bid)) {
      // Increment the count if the save succeeded.
      if ($update) {
        $this->numUpdated++;
      }
      else {
        $this->numCreated++;
      }
      // Return the primary key to the mapping table.
      $return = array($bid);
    }
    else {
      $return = FALSE;
    }

    // Invoke migration complete handlers.
    $block = (object) $this->loadCustomBlock($bid);
    $this->complete($block, $row);

    return $return;
  }

  /**
   * Implementation of MigrateDestination::prepare().
   */
  public function prepare($block, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $block->migrate = array(
      'machineName' => $migration->getMachineName(),
    );

    // Call any general handlers.
    migrate_handler_invoke_all('block_custom', 'prepare', $block, $row);
    // Then call any prepare handler for this specific Migration.
    if (method_exists($migration, 'prepare')) {
      $migration->prepare($block, $row);
    }
  }

  /**
   * Implementation of MigrateDestination::complete().
   */
  public function complete($block, stdClass $row) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    $block->migrate = array(
      'machineName' => $migration->getMachineName(),
    );
    // Call any general handlers.
    migrate_handler_invoke_all('block_custom', 'complete', $block, $row);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'complete')) {
      $migration->complete($block, $row);
    }
  }

  /**
   * Delete a batch of custom blocks at once.
   *
   * @param $bids
   *  Array of custom block IDs to be deleted.
   */
  public function bulkRollback(array $bids) {
    migrate_instrument_start('block_custom_delete_multiple');
    $this->prepareRollback($bids);
    $this->deleteMultipleCustomBlocks($bids);
    $this->completeRollback($bids);
    migrate_instrument_stop('block_custom_delete_multiple');
  }

  /**
   * Give handlers a shot at cleaning up before a block has been rolled back.
   *
   * @param $bid
   *  ID of the custom block about to be deleted.
   */
  public function prepareRollback($bid) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('block_custom', 'prepareRollback', $bid);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'prepareRollback')) {
      $migration->prepareRollback($bid);
    }
  }

  /**
   * Give handlers a shot at cleaning up after a block has been rolled back.
   *
   * @param $bid
   *  ID of the custom block which has been deleted.
   */
  public function completeRollback($bid) {
    // We do nothing here but allow child classes to act.
    $migration = Migration::currentMigration();
    // Call any general handlers.
    migrate_handler_invoke_all('block_custom', 'completeRollback', $bid);
    // Then call any complete handler for this specific Migration.
    if (method_exists($migration, 'completeRollback')) {
      $migration->completeRollback($bid);
    }
  }

  public function loadCustomBlock($bid) {
    return block_custom_block_get($bid);
  }

  public function saveCustomBlock($block) {
    drupal_alter('block_custom', $block);
    if (!empty($block['bid'])) {
      drupal_write_record('block_custom', $block, array('bid'));
      module_invoke_all('block_custom_update', $block);
    }
    else {
      drupal_write_record('block_custom', $block);
      module_invoke_all('block_custom_insert', $block);
    }
    return $block['bid'];
  }

  public function deleteCustomBlock($bid) {
    $this->deleteMultipleCustomBlocks(array($bid));
  }

  public function deleteMultipleCustomBlocks(array $bids) {
    db_delete('block_custom')->condition('bid', $bids, 'IN')->execute();
    db_delete('block')
      ->condition('module', 'block')
      ->condition('delta', $bids, 'IN')
      ->execute();
    db_delete('block_role')
      ->condition('module', 'block')
      ->condition('delta', $bids, 'IN')
      ->execute();
    db_delete('block_node_type')
      ->condition('module', 'block')
      ->condition('delta', $bids, 'IN')
      ->execute();
  }
}
