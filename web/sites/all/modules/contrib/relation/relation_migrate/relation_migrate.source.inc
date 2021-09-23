<?php

/**
 * @file
 * Source plugin for *reference fields.
 */

/**
 * Migration source for *ference field. This source is primary used in
 * relation_migrate to convert *reference entries into relation entities.
 */
abstract class MigrateSourceReference extends MigrateSource {

  /**
   * Place where data is stored during import.
   */
  protected $result = array();

  /**
   * ID of a row, that will be imported during next iteration.
   */
  protected $next_row = 0;

  /**
   * Machine names of fields that will be imported.
   */
  protected $fields = array();

  /**
   * Field type.
   */
  protected $field_type;

  /**
   * Constructor.
   *
   * @param string $field Field type machine name.
   * @param array $fields List of fields to be migrated.
   */
  public function __construct($field_type, array $fields, array $options = array()) {
    parent::__construct($options);
    $this->fields = $fields;
    $this->field_type = $field_type;
  }

  /**
   * Return a string representing the source, for display in the UI.
   */
  public function __toString() {
    return t('Migrate %type fields: %fields', array('%type' => $this->field_type, '%fields' => implode(',', $this->fields)));
  }

  /**
   * Returns a list of fields available to be mapped from the source,
   * keyed by field name.
   */
  public function fields() {
    return array(
      'source_type' => t('Source entity type'),
      'source_id' => t('Source entity ID'),
      'destination_type' => t('Destination entity type'),
      'destination_id' => t('Destination entity ID'),
      'field_name' => t('Field name'),
    );
  }

  /**
   * Return the number of available source records.
   */
  public function computeCount() {
    $rows_count = 0;
    foreach ($this->fields as $field_name) {
      $rows_count += db_query('SELECT count(*) FROM {field_data_' . $field_name . '} WHERE deleted = 0')->fetchField();
    }
    return $rows_count;
  }

  /**
   * Do whatever needs to be done to start a fresh traversal of the source data.
   */
  public function performRewind() {
    $this->result = array();
    $this->next_row = 0;

    // Load data for each field and merge all records in result array.
    foreach ($this->fields as $field_name) {
      $field_info = field_read_field($field_name);
      $columns = array_keys($field_info['columns']);

      if (!empty($field_info) && $field_info['type'] == $this->field_type) {
        $field_data = db_select('field_data_' . $field_name, 'f')
          ->fields('f', array('entity_type', 'entity_id', 'delta', $field_name . '_' . $columns[0]))
          ->condition('deleted', 0);
        $field_data->addExpression(":name", 'field_name', array(':name' => $field_name));
        $field_data = $field_data->execute()->fetchAll();

        $this->result = array_merge($this->result, $field_data);
      }
    }

  }

  /**
   * Constructs row object, that should be returned from $this->getNextRow().
   *
   * @param stdClass $item Row item as returned from DB.
   * @param string $destination_type Destiantion entity type.
   * @param string $field_name Field's machine name.
   */
  protected function _constructRow(stdClass $item, $destination_type, $field_name) {
      $field_info = field_read_field($field_name);
      $columns = array_keys($field_info['columns']);
      $id_key = $field_name . '_' . $columns[0];

      $ret = array(
        'source_type' => $item->entity_type,
        'source_id' => $item->entity_id,
        'destination_type' => $destination_type,
        'destination_id' => $item->{$id_key},
        'field_name' => $field_name,
        'delta' => $item->delta,
      );

      return (object)$ret;
  }
}

/**
 * Source migration plugin for entityreference.
 */
class MigrateSourceEntityReference extends MigrateSourceReference {
  /**
   * Constructor.
   *
   * @param array $fields List of fields to be migrated.
   */
  function __construct(array $fields, array $options = array()) {
    parent::__construct('entityreference', $fields, $options);
  }

  /**
   * Fetch the next row of data, returning it as an object. Return FALSE
   * when there is no more data available.
   */
  public function getNextRow() {
    if (!empty($this->result[$this->next_row])) {
      $item = $this->result[$this->next_row];
      $dest_type = field_read_field($item->field_name);
      $this->next_row++;

      return $this->_constructRow($item, $dest_type['settings']['target_type'], $item->field_name);
    }

    return FALSE;
  }
}

/**
 * Source migration plugin for node_reference.
 */
class MigrateSourceNodeReference extends MigrateSourceReference {
  /**
   * Constructor.
   *
   * @param array $fields List of fields to be migrated.
   */
  function __construct(array $fields, array $options = array()) {
    parent::__construct('node_reference', $fields, $options);
  }

  /**
   * Fetch the next row of data, returning it as an object. Return FALSE
   * when there is no more data available.
   */
  public function getNextRow() {
    if (!empty($this->result[$this->next_row])) {
      $item = $this->result[$this->next_row];
      $this->next_row++;

      return $this->_constructRow($item, 'node', $item->field_name);
    }

    return FALSE;
  }
}

/**
 * Source migration plugin for user_reference.
 */
class MigrateSourceUserReference extends MigrateSourceReference {
  /**
   * Constructor.
   *
   * @param array $fields List of fields to be migrated.
   */
  function __construct(array $fields, array $options = array()) {
    parent::__construct('user_reference', $fields, $options);
  }

  /**
   * Fetch the next row of data, returning it as an object. Return FALSE
   * when there is no more data available.
   */
  public function getNextRow() {
    if (!empty($this->result[$this->next_row])) {
      $item = $this->result[$this->next_row];
      $this->next_row++;

      return $this->_constructRow($item, 'user', $item->field_name);
    }

    return FALSE;
  }
}

/**
 * Source migration plugin for taxonomy_term_reference.
 */
class MigrateSourceTermReference extends MigrateSourceReference {
  /**
   * Constructor.
   *
   * @param array $fields List of fields to be migrated.
   */
  function __construct(array $fields, array $options = array()) {
    parent::__construct('taxonomy_term_reference', $fields, $options);
  }

  /**
   * Fetch the next row of data, returning it as an object. Return FALSE
   * when there is no more data available.
   */
  public function getNextRow() {
    if (!empty($this->result[$this->next_row])) {
      $item = $this->result[$this->next_row];
      $this->next_row++;

      return $this->_constructRow($item, 'taxonomy_term', $item->field_name);
    }

    return FALSE;
  }
}