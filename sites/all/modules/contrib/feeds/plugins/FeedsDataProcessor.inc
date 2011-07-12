<?php

/**
 * @file
 * Definition of FeedsDataProcessor.
 */

/**
 * Creates simple table records from feed items. Uses Data module.
 */
class FeedsDataProcessor extends FeedsProcessor {

  /**
   * Implementation of FeedsProcessor::process().
   */
  public function process(FeedsImportBatch $batch, FeedsSource $source) {

    // Count number of created and updated nodes.
    $inserted  = $updated = 0;
    $expiry_time = $this->expiryTime();

    while ($item = $batch->shiftItem()) {
      $id = $this->existingItemId($batch, $source);

      if ($id === FALSE || $this->config['update_existing']) {
        // Map item to a data record, feed_nid and timestamp are mandatory.
        $data = array();
        $data['feed_nid'] = $source->feed_nid;
        $data = $this->map($batch, $data);
        if (!isset($data['timestamp'])) {
          $data['timestamp'] = FEEDS_REQUEST_TIME;
        }

        // Only save if this item is not expired.
        if ($expiry_time != FEEDS_EXPIRE_NEVER && $data['timestamp'] < (FEEDS_REQUEST_TIME - $expiry_time)) {
          continue;
        }

        // Save data.
        if ($id !== FALSE) {
          $data['id'] = $id;
          $this->handler()->update($data, 'id');
          $updated++;
        }
        else {
          $this->handler()->insert($data);
          $inserted++;
        }
      }
    }

    // Set messages.
    if ($inserted) {
      drupal_set_message(format_plural($inserted, 'Created @number item.', 'Created @number items.', array('@number' => $inserted)));
    }
    if ($updated) {
      drupal_set_message(format_plural($updated, 'Updated @number item.', 'Updated @number items.', array('@number' => $updated)));
    }
    if (!$inserted && !$updated) {
      drupal_set_message(t('There are no new items.'));
    }
  }

  /**
   * Implementation of FeedsProcessor::clear().
   *
   * Delete all data records for feed_nid in this table.
   */
  public function clear(FeedsBatch $batch, FeedsSource $source) {
    $clause = array(
      'feed_nid' => $source->feed_nid,
    );
    $num = $this->handler()->delete($clause);
    if ($num) {
      drupal_set_message('All items have been deleted.');
    }
    else {
      drupal_set_message('There were no items to delete.');
    }
  }

  /**
   * Implement expire().
   */
  public function expire($time = NULL) {
    if ($time === NULL) {
      $time = $this->expiryTime();
    }
    if ($time == FEEDS_EXPIRE_NEVER) {
      return FEEDS_BATCH_COMPLETE;
    }
    $clause = array(
      'timestamp' => array(
        '<',
        FEEDS_REQUEST_TIME - $time,
      ),
    );
    $num = $this->handler()->delete($clause);
    drupal_set_message(format_plural($num, 'Expired @number record from @table.', 'Expired @number records from @table.', array('@number' => $num, '@table' => $this->tableName())));
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Return expiry time.
   */
  public function expiryTime() {
    return $this->config['expire'];
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $schema = $this->table()->get('table_schema');
    $meta = $this->table()->get('meta');

    // Collect all existing fields except id and field_nid and offer them as
    // mapping targets.
    $existing_fields = $new_fields = array();
    if (isset($schema['fields'])) {
      foreach ($schema['fields'] as $field_name => $field) {
        if (!in_array($field_name, array('id', 'feed_nid'))) {
          // Any existing field can be optionally unique.
          // @todo Push this reverse mapping of spec to short name into data
          // module.
          $type = $field['type'];
          if ($type == 'int' && $field['unsigned']) {
            $type = 'unsigned int';
          }
          $existing_fields[$field_name] = array(
            'name' => empty($meta['fields'][$field_name]['label']) ? $field_name : $meta['fields'][$field_name]['label'],
            'description' => t('Field of type !type.', array('!type' => $type)),
            'optional_unique' => TRUE,
          );
        }
      }
    }

    // Do the same for every joined table.
    foreach ($this->handler()->joined_tables as $table) {
      $schema = data_get_table($table)->get('table_schema');
      if (isset($schema['fields'])) {
        foreach ($schema['fields'] as $field_name => $field) {
          if (!in_array($field_name, array('id', 'feed_nid'))) {
            // Fields in joined tables can't be unique.
            $type = $field['type'];
            if ($type == 'int' && $field['unsigned']) {
              $type = 'unsigned int';
            }
            $existing_fields["$table.$field_name"] = array(
              'name' => $table .'.'. (empty($meta['fields'][$field_name]['label']) ? $field_name : $meta['fields'][$field_name]['label']),
              'description' => t('Joined field of type !type.', array('!type' => $type)),
              'optional_unique' => FALSE,
            );
          }
        }
      }
    }

    // Now add data field types as mapping targets.
    $field_types = drupal_map_assoc(array_keys(data_get_field_definitions()));
    foreach ($field_types as $k => $v) {
      $new_fields['new:'. $k] = array(
        'name' => t('[new] !type', array('!type' => $v)),
        'description' => t('Creates a new column of type !type.', array('!type' => $v)),
      );
    }
    $fields = $new_fields + $existing_fields;
    drupal_alter('feeds_data_processor_targets', $fields, $this->table()->get('name'));
    return $fields;
  }

  /**
   * Set target element, bring element in a FeedsDataHandler format.
   */
  public function setTargetElement(&$target_item, $target_element, $value) {
    if (empty($value)) {
      return;
    }
    if (strpos($target_element, '.')) {
      /**
      Add field in FeedsDataHandler format.

      This is the tricky part, FeedsDataHandler expects an *array* of records
      at #[joined_table_name]. We need to iterate over the $value that has
      been mapped to this element and create a record array from each of
      them.
      */
      list($table, $field) = explode('.', $target_element);

      $values = array();
      $value = is_array($value) ? $value : array($value);
      foreach ($value as $v) {
        // Create a record array.
        $values[] = array(
          $field => $v,
        );
      }
      if (is_array($target_item["#$table"])) {
        $target_item["#$table"] = array_merge($target_item["#$table"], $values);
      }
      else {
        $target_item["#$table"] = $values;
      }
    }
    else {
      if (is_array($target_item[$target_element]) && is_array($value)) {
        $target_item[$target_element] = array_merge($target_item[$target_element], $value);
      }
      else {
        $target_item[$target_element] = $value;
      }
    }
  }

  /**
   * Iterate through unique targets and try to load existing records.
   * Return id for the first match.
   */
  protected function existingItemId(FeedsImportBatch $batch, FeedsSource $source) {
    foreach ($this->uniqueTargets($batch) as $target => $value) {
      if ($records = $this->handler()->load(array('feed_nid' => $source->feed_nid, $target => $value))) {
        return $records[0]['id'];
      }
    }
    return FALSE;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'update_existing' => FEEDS_SKIP_EXISTING,
      'expire' => FEEDS_EXPIRE_NEVER, // Don't expire items by default.
      'mappings' => array(),
      'delete_with_source' => FALSE,
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600, 43200, 86400, 259200, 604800, 604800 * 4, 604800 * 12, 604800 * 24, 31536000), 'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire items'),
      '#options' => $period,
      '#description' => t('Select after how much time data records should be deleted. The timestamp target value will be used for determining the item\'s age, see Mapping settings.'),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Replace existing records'),
      '#description' => t('If an existing record is found for an imported record, replace it. Existing records will be determined using mappings that are a "unique target".'),
      '#default_value' => $this->config['update_existing'],
    );
    $form['delete_with_source'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete items with source'),
      '#description' => t('If enabled, any feed items associated with a source node will be removed along with the node. Not available for standalone importers.'),
      '#default_value' => $this->config['delete_with_source'],
      '#disabled' => empty(feeds_importer($this->id)->config['content_type']) ? TRUE : FALSE,
    );
    return $form;
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    if ($this->config['expire'] != $values['expire']) {
      feeds_reschedule($this->id);
    }
    parent::configFormSubmit($values);
  }

  /**
   * Return the data table name for this feed.
   */
  protected function tableName() {
    return variable_get('feeds_data_'. $this->id, 'feeds_data_'. $this->id);
  }

  /**
   * Return the data table for this feed.
   *
   * @throws Exception $e
   *   Throws this exception if a table cannot be found and cannot be created.
   *
   * @todo Make *Data module* throw exception when table can't be found or
   *   can't be created.
   */
  protected function table() {
    if ($table = data_get_table($this->tableName())) {
      return $table;
    }
    else {
      if ($table = data_create_table($this->tableName(), $this->baseSchema(), feeds_importer($this->id)->config['name'])) {
        return $table;
      }
    }
    throw new Exception(t('Could not create data table.'));
  }

  /**
   * Return a data handler for this table.
   *
   * Avoids a call to table() to not unnecessarily instantiate DataTable.
   */
  protected function handler() {
    data_include('DataHandler');
    feeds_include('FeedsDataHandler');
    return FeedsDataHandler::instance($this->tableName(), 'id');
  }

  /**
   * Every Feeds data table must have these elements.
   */
  protected function baseSchema() {
    return array(
      'fields' => array(
        'feed_nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'id' => array(
          'type' => 'serial',
          'size' => 'normal',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'timestamp' => array(
          'description' => 'The Unix timestamp for the data.',
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
        ),
      ),
      'indexes' => array(
        'feed_nid' => array('feed_nid'),
        'id' => array('id'),
        'timestamp' => array('timestamp'),
       ),
       'primary key' => array(
         '0' => 'id',
       ),
    );
  }

  /**
   * Override parent::addConfig().
   */
  public function addConfig($config) {
    $this->processMappings($config);
    return parent::addConfig($config);
  }

  /**
   * Override parent::setConfig().
   */
  public function setConfig($config) {
    $this->processMappings($config);
    return parent::setConfig($config);
  }

  /**
   * Create a new field for each new: mapping.
   */
  protected function processMappings(&$config) {
    if (!empty($config['mappings'])) {
      foreach ($config['mappings'] as &$mapping) {
        @list($new, $type) = explode(':', $mapping['target']);

        // Create a new field with targets that start with "new:"
        if ($new == 'new') {
          // Build a field name from the source key.
          $field_name = data_safe_name($mapping['source']);
          // Get the full schema spec from data.
          $type = data_get_field_definition($type);
          // Add the field to the table.
          $schema = $this->table()->get('table_schema');
          if (!isset($schema['fields'][$field_name])) {
            $mapping['target'] = $this->table()->addField($field_name, $type);
            // Let the user know.
            drupal_set_message(t('Created new field "@name".', array('@name' => $field_name)));
          }
          else {
            throw new Exception(t('Field @field_name already exists as a mapping target. Remove it from mapping if you would like to map another source to it. Remove it from !data_table table if you would like to change its definition.', array('@field_name' => $field_name, '!data_table' => l($this->table()->get('name'), 'admin/content/data'))));
          }
        }
      }
    }
  }
}
