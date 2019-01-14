<?php

/**
 * @file
 * Define a MigrateSource for importing from Drupal connections
 */

/**
 * Implementation of MigrateSource, to handle imports from Drupal connections.
 */
class MigrateSourceSQL extends MigrateSource {

  /**
   * The SQL query objects from which to obtain data, and counts of data
   *
   * @var SelectQueryInterface
   */
  protected $originalQuery, $query, $countQuery, $alteredQuery;

  /**
   * Return a reference to the base query, in particular so Migration classes
   * can add conditions/joins/etc to the query for a source defined in a
   * base class.
   *
   * @return SelectQueryInterface
   */
  public function &query() {
    return $this->originalQuery;
  }

  /**
   * The result object from executing the query - traversed to process the
   * incoming data.
   *
   * @var DatabaseStatementInterface
   */
  protected $result;

  /**
   * Number of eligible rows processed so far (used for itemlimit checking)
   *
   * @var int
   */
  protected $numProcessed = 0;

  /**
   * Current data batch.
   *
   * @var int
   */
  protected $batch = 0;

  /**
   * Number of records to fetch from the database during each batch. A value
   * of zero indicates no batching is to be done.
   *
   * @var int
   */
  protected $batchSize = 0;

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * If the map is a MigrateSQLMap, and the table is compatible with the
   * source query, we can join directly to the map and make things much faster
   * and simpler.
   *
   * @var boolean
   */
  protected $mapJoinable = FALSE;

  // Dynamically set whether the map is joinable - not really for production use,
  // this is primarily to support simpletests
  public function setMapJoinable($map_joinable) {
    $this->mapJoinable = $map_joinable;
  }

  /**
   * Whether this source is configured to use a highwater mark, and there is
   * a highwater mark present to use.
   *
   * @var boolean
   */
  protected $usingHighwater = FALSE;

  /**
   * Whether, in the current iteration, we have reached the highwater mark.
   *
   * @var boolen
   */
  protected $highwaterSeen = FALSE;

  /**
   * Return an options array for PDO sources.
   *
   * @param boolean $map_joinable
   *  Indicates whether the map table can be joined directly to the source
   *   query.
   * @param boolean $cache_counts
   *  Indicates whether to cache counts of source records.
   */
  static public function options($map_joinable, $cache_counts) {
    return compact('map_joinable', 'cache_counts');
  }

  /**
   * Simple initialization.
   *
   * @param SelectQueryInterface $query
   *  The query we are iterating over.
   * @param array $fields
   *  Optional - keys are field names, values are descriptions. Use to override
   *  the default descriptions, or to add additional source fields which the
   *  migration will add via other means (e.g., prepareRow()).
   * @param SelectQueryInterface $count_query
   *  Optional - an explicit count query, primarily used when counting the
   *  primary query is slow.
   * @param boolean $options
   *  Options applied to this source.
   */
  public function __construct(SelectQueryInterface $query, array $fields = array(),
                              SelectQueryInterface $count_query = NULL, array $options = array()) {
    parent::__construct($options);
    $this->originalQuery = $query;
    $this->query = clone $query;
    $this->fields = $fields;
    if (is_null($count_query)) {
      $this->countQuery = clone $query->countQuery();
    }
    else {
      $this->countQuery = $count_query;
    }

    if (isset($options['batch_size'])) {
      $this->batchSize = $options['batch_size'];
      // Joining to the map table is incompatible with batching, disable it.
      $options['map_joinable'] = FALSE;
    }

    // If we're tracking changes, then we need to fetch all rows to see if
    // they've changed, we can't make that determination through a direct join.
    if (!empty($options['track_changes'])) {
      $options['map_joinable'] = FALSE;
    }

    if (isset($options['map_joinable'])) {
      $this->mapJoinable = $options['map_joinable'];
    }
    else {
      // TODO: We want to automatically determine if the map table can be joined
      // directly to the query, but this won't work unless/until
      // http://drupal.org/node/802514 is committed, assume joinable for now
      $this->mapJoinable = TRUE;
      /*      // To be able to join the map directly, it must be a PDO map on the same
            // connection, or a compatible connection
            $map = $migration->getMap();
            if (is_a($map, 'MigrateSQLMap')) {
              $map_options = $map->getConnection()->getConnectionOptions();
              $query_options = $this->query->connection()->getConnectionOptions();

              // Identical options means it will work
              if ($map_options == $query_options) {
                $this->mapJoinable = TRUE;
              }
              else {
                // Otherwise, the one scenario we know will work is if it's MySQL and
                // the credentials match (SQLite too?)
                if ($map_options['driver'] == 'mysql' && $query_options['driver'] == 'mysql') {
                  if ($map_options['host'] == $query_options['host'] &&
                      $map_options['port'] == $query_options['port'] &&
                      $map_options['username'] == $query_options['username'] &&
                      $map_options['password'] == $query_options['password']) {
                    $this->mapJoinable = TRUE;
                  }
                }
              }
            }*/
    }
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    $query = clone $this->query;
    $query = $query->extend('MigrateConnectionQuery');
    return $query->getString();
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();
    $queryFields = $this->query->getFields();

    if ($queryFields) {
      // Not much we can do in terms of describing the fields without manual intervention
      foreach ($queryFields as $field_name => $field_info) {
        $fields[$field_name] = $field_info['table'] . '.' . $field_info['field'];
      }
    }
    else {
      // Detect available fields
      $detection_query = clone $this->query;
      $result = $detection_query->range(0, 1)->execute();
      $row = $result->fetchAssoc();
      if (is_array($row)) {
        foreach ($row as $field_name => $field_value) {
          $fields[$field_name] = t('Example Content: !value', array('!value' => $field_value));
        }
      }
    }

    /*
     * Handle queries without explicit field lists
     * TODO: Waiting on http://drupal.org/node/814312
        $info = Database::getConnectionInfo($query->getConnection());
        $database = $info['default']['database'];
        foreach ($this->query->getTables() as $table) {
          if (isset($table['all_fields']) && $table['all_fields']) {

            $database = 'plants';
            $table = $table['table'];
            $sql = 'SELECT column_name
                    FROM information_schema.columns
                    WHERE table_schema=:database AND table_name = :table
                    ORDER BY ordinal_position';
            $result = dbtng_query($sql, array(':database' => $database, ':table' => $table));
            foreach ($result as $row) {
              $fields[$row->column_name] = $table . '.' . $row->column_name;
            }
          }
        }*/
    $expressionFields = $this->query->getExpressions();
    foreach ($expressionFields as $field_name => $field_info) {
      $fields[$field_name] = $field_info['alias'];
    }

    // Any caller-specified fields with the same names as extracted fields will
    // override them; any others will be added
    if ($this->fields) {
      $fields = $this->fields + $fields;
    }

    return $fields;
  }

  /**
   * Return a count of all available source records.
   */
  public function computeCount() {
    $count = $this->countQuery->execute()->fetchField();
    return $count;
  }

  /**
   * Implementation of MigrateSource::performRewind().
   *
   * We could simply execute the query and be functionally correct, but
   * we will take advantage of the PDO-based API to optimize the query up-front.
   */
  public function performRewind() {
    $this->result = NULL;
    $this->query = clone $this->originalQuery;
    $this->batch = 0;

    // Get the key values, for potential use in joining to the map table, or
    // enforcing idlist.
    $keys = array();
    foreach ($this->activeMap->getSourceKey() as $field_name => $field_schema) {
      if (isset($field_schema['alias'])) {
        $field_name = $field_schema['alias'] . '.' . $field_name;
      }
      $keys[] = $field_name;
    }

    // The rules for determining what conditions to add to the query are as
    // follows (applying first applicable rule)
    // 1. If idlist is provided, then only process items in that list (AND key
    //    IN (idlist)). Only applicable with single-value keys.
    if ($this->idList) {
      $simple_ids = array();
      $compound_ids = array();
      $key_count = count($keys);

      foreach ($this->idList as $id) {
        // Look for multi-key separator. If there is only 1 key, ignore.
        if (strpos($id, $this->multikeySeparator) === FALSE || $key_count == 1) {
          $simple_ids[] = $id;
          continue;
        }

        $compound_ids[] = explode($this->multikeySeparator, $id);
      }

      // Check for compunded ids. If present add them with subsequent OR statements.
      if (!empty($compound_ids)) {
        $condition = db_or();
        if (!empty($simple_ids)) {
          $condition->condition($keys[0], $simple_ids, 'IN');
        }

        foreach ($compound_ids as $values) {
          $temp_and = db_and();
          foreach ($values as $pos => $value) {
            $temp_and->condition($keys[$pos], $value);
          }

          $condition->condition($temp_and);
        }

        $this->query->condition($condition);
      }
      else {
        $this->query->condition($keys[0], $simple_ids, 'IN');
      }
    }
    else {
      // 2. If the map is joinable, join it. We will want to accept all rows
      //    which are either not in the map, or marked in the map as NEEDS_UPDATE.
      //    Note that if highwater fields are in play, we want to accept all rows
      //    above the highwater mark in addition to those selected by the map
      //    conditions, so we need to OR them together (but AND with any existing
      //    conditions in the query). So, ultimately the SQL condition will look
      //    like (original conditions) AND (map IS NULL OR map needs update
      //      OR above highwater).
      $conditions = db_or();
      $condition_added = FALSE;
      if ($this->mapJoinable) {
        // Build the join to the map table. Because the source key could have
        // multiple fields, we need to build things up.
        $count = 1;

        foreach ($this->activeMap->getSourceKey() as $field_name => $field_schema) {
          if (isset($field_schema['alias'])) {
            $field_name = $field_schema['alias'] . '.' . $field_name;
          }
          $map_key = 'sourceid' . $count++;
          if (!isset($map_join)) {
            $map_join = '';
          }
          else {
            $map_join .= ' AND ';
          }
          $map_join .= "$field_name = map.$map_key";
        }

        $alias = $this->query->leftJoin($this->activeMap->getQualifiedMapTable(),
          'map', $map_join);
        $conditions->isNull($alias . '.sourceid1');
        $conditions->condition($alias . '.needs_update', MigrateMap::STATUS_NEEDS_UPDATE);
        $condition_added = TRUE;

        // And as long as we have the map table, add its data to the row.
        $count = 1;
        foreach ($this->activeMap->getSourceKey() as $field_name => $field_schema) {
          $map_key = 'sourceid' . $count++;
          $this->query->addField($alias, $map_key, "migrate_map_$map_key");
        }
        $count = 1;
        foreach ($this->activeMap->getDestinationKey() as $field_name => $field_schema) {
          $map_key = 'destid' . $count++;
          $this->query->addField($alias, $map_key, "migrate_map_$map_key");
        }
        $this->query->addField($alias, 'needs_update', 'migrate_map_needs_update');
      }
      // 3. If we are using highwater marks, also include rows above the mark.
      //    But, include all rows if the highwater mark is not set.
      if (isset($this->highwaterField['name']) && $this->activeMigration->getHighwater() !== '') {
        // But, if there are any existing items marked as needing update which
        // fall below the highwater mark, and map_joinable is FALSE, those
        // items will be skipped. Thus, in that case do not add the highwater
        // optimization to the query.
        $add_highwater_condition = TRUE;
        if (!$this->mapJoinable) {
          $count_needs_update = db_query('SELECT COUNT(*) FROM {' .
            $this->activeMap->getQualifiedMapTable() . '} WHERE needs_update = 1')
            ->fetchField();
          if ($count_needs_update > 0) {
            $add_highwater_condition = FALSE;
          }
        }
        if ($add_highwater_condition) {
          if (isset($this->highwaterField['alias'])) {
            $highwater = $this->highwaterField['alias'] . '.' . $this->highwaterField['name'];
          }
          else {
            $highwater = $this->highwaterField['name'];
          }
          $conditions->condition($highwater, $this->activeMigration->getHighwater(), '>');
          $condition_added = TRUE;
        }
      }
      if ($condition_added) {
        $this->query->condition($conditions);
      }

      // 4. Download data in batches for performance.
      if ($this->batchSize > 0) {
        $this->query->range($this->batch * $this->batchSize, $this->batchSize);
      }
    }

    // Save our fixed-up query so getNextBatch() matches it.
    $this->alteredQuery = clone $this->query;

    migrate_instrument_start('MigrateSourceSQL execute');
    $this->result = $this->query->execute();
    migrate_instrument_stop('MigrateSourceSQL execute');
  }

  /**
   * Implementation of MigrateSource::getNextRow().
   *
   * @return object
   */
  public function getNextRow() {
    $row = $this->result->fetchObject();

    // We might be out of data entirely, or just out of data in the current batch.
    // Attempt to fetch the next batch and see.
    if (!is_object($row) && $this->batchSize > 0) {
      $this->getNextBatch();
      $row = $this->result->fetchObject();
    }
    if (is_object($row)) {
      return $row;
    }
    else {
      return NULL;
    }
  }

  /**
   * Downloads the next set of data from the source database.
   */
  protected function getNextBatch() {
    $this->batch++;
    $query = clone $this->alteredQuery;
    $query->range($this->batch * $this->batchSize, $this->batchSize);
    $this->result = $query->execute();
  }

}

/**
 * Query extender for retrieving the connection used on the query.
 */
class MigrateConnectionQuery extends SelectQueryExtender {

  public function __construct(SelectQueryInterface $query, DatabaseConnection $connection) {
    parent::__construct($query, $connection);
    // Add the connection as metadata if anything else wants to access it.
    $query->addMetaData('connection', $connection);
  }

  /**
   * Return a string representing the source query.
   *
   * This is copied from devel module's dpq() function.
   *
   * @param bool $prefix
   *   If the tables should be prefixed. If FALSE will return tables names in
   *   the query like {tablename}.
   *
   * @return string
   *   The SQL query.
   */
  public function getString($prefix = TRUE) {
    $query = $this;
    if (method_exists($this, 'preExecute')) {
      $query->preExecute();
    }
    $sql = (string) $this;
    $quoted = array();
    foreach ((array) $this->arguments() as $key => $val) {
      $quoted[$key] = $this->connection->quote($val);
    }
    $sql = strtr($sql, $quoted);
    if ($prefix) {
      $sql = $this->connection->prefixTables($sql);
    }
    return $sql;
  }
}
