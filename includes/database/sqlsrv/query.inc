<?php

/**
 * SQL Server-specific implementation of INSERT.
 *
 * SQL Server doesn't supports multi-insert queries, and needs special handling for
 * binary columns.
 */
class InsertQuery_sqlsrv extends InsertQuery {

  public function execute() {
    if (!$this->preExecute()) {
      return NULL;
    }

    // Fetch the list of blobs and sequences used on that table.
    $columnInformation = $this->connection->schema()->queryColumnInformation($this->table);

    // Find out if there is an identity field set in this insert.
    foreach ($this->insertFields as $field) {
      if (isset($columnInformation['identities'][$field])) {
        $this->setIdentity = TRUE;
      }
    }

    // Each insert happens in its own query. However, we wrap it in a transaction
    // so that it is atomic where possible.
    if (empty($this->queryOptions['sqlsrv_skip_transactions'])) {
      $transaction = $this->connection->startTransaction();
    }

    if (!empty($this->fromQuery)) {
      // Re-initialize the values array so that we can re-use this query.
      $this->insertValues = array();

      $stmt = $this->connection->PDOPrepare($this->connection->prefixTables((string) $this));
      // Handle the case of SELECT-based INSERT queries first.
      $values = $this->fromQuery->getArguments();
      foreach ($values as $key => $value) {
        $stmt->bindParam($key, $values[$key]);
      }

      try {
        $stmt->execute();
      }
      catch (Exception $e) {
        // This INSERT query failed, rollback everything if we started a transaction earlier.
        if (!empty($transaction)) {
          $transaction->rollback();
        }
        // Rethrow the exception.
        throw $e;
      }

      return $this->connection->lastInsertId();
    }

    // Handle the case of full-default queries.
    if (empty($this->fromQuery) && (empty($this->insertFields) || empty($this->insertValues))) {
      // Re-initialize the values array so that we can re-use this query.
      $this->insertValues = array();

      $stmt = $this->connection->PDOPrepare($this->connection->prefixTables('INSERT INTO {' . $this->table . '} DEFAULT VALUES'));
      try {
        $stmt->execute();
      }
      catch (Exception $e) {
        // This INSERT query failed, rollback everything if we started a transaction earlier.
        if (!empty($transaction)) {
          $transaction->rollback();
        }
        // Rethrow the exception.
        throw $e;
      }

      return $this->connection->lastInsertId();
    }

    $query = (string) $this;
    $stmt = $this->connection->PDOPrepare($this->connection->prefixTables($query));

    // We use this array to store references to the blob handles.
    // This is necessary because the PDO will otherwise messes up with references.
    $data_values = array();

    foreach ($this->insertValues as $insert_index => $insert_values) {
      $max_placeholder = 0;
      foreach ($this->insertFields as $field_index => $field) {
        $placeholder = ':db_insert' . $max_placeholder++;
        if (isset($columnInformation['blobs'][$field])) {
          $data_values[$placeholder . $insert_index] = fopen('php://memory', 'a');
          fwrite($data_values[$placeholder . $insert_index], $insert_values[$field_index]);
          rewind($data_values[$placeholder . $insert_index]);

          $stmt->bindParam($placeholder, $data_values[$placeholder . $insert_index], PDO::PARAM_LOB, 0, PDO::SQLSRV_ENCODING_BINARY);
        }
        else {
          $data_values[$placeholder . $insert_index] = $insert_values[$field_index];
          $stmt->bindParam($placeholder, $data_values[$placeholder . $insert_index]);
        }
      }

      try {
        $stmt->execute();
      }
      catch (Exception $e) {
        // This INSERT query failed, rollback everything if we started a transaction earlier.
        if (!empty($transaction)) {
          $transaction->rollback();
        }
        // Rethrow the exception.
        throw $e;
      }

      $last_insert_id = $this->connection->lastInsertId();
    }

    // Re-initialize the values array so that we can re-use this query.
    $this->insertValues = array();

    return $last_insert_id;
  }

  public function __toString() {
    // Create a sanitized comment string to prepend to the query.
    $prefix = $this->connection->makeComment($this->comments);

    // Enable direct insertion to identity columns if necessary.
    if (!empty($this->setIdentity)) {
      $prefix .= 'SET IDENTITY_INSERT {' . $this->table . '} ON;';
    }

    // If we're selecting from a SelectQuery, finish building the query and
    // pass it back, as any remaining options are irrelevant.
    if (!empty($this->fromQuery)) {
      return $prefix . "INSERT INTO {" . $this->table . '} (' . implode(', ', $this->connection->quoteIdentifiers($this->insertFields)) . ') ' . $this->fromQuery;
    }

    // Build the list of placeholders.
    $placeholders = array();
    for ($i = 0; $i < count($this->insertFields); ++$i) {
      $placeholders[] = ':db_insert' . $i;
    }

    return $prefix . 'INSERT INTO {' . $this->table . '} (' . implode(', ', $this->connection->quoteIdentifiers($this->insertFields)) . ') VALUES (' . implode(', ', $placeholders) . ')';
  }
}

/**
 * SQL Server-specific implementation of UPDATE.
 *
 * The specific parts are:
 *  - SQL Server returns the number of matched rows to an UPDATE, and Drupal
 *    requires the number of affected rows to be returned.
 *  - SQL Server requires a special handling for the blobs.
 */
class UpdateQuery_sqlsrv extends UpdateQuery {
  /**
   * Helper function that removes the fields that are already in a condition.
   *
   * @param $fields
   *   The fields.
   * @param QueryConditionInterface $condition
   *   A database condition.
   */
  protected function removeFieldsInCondition(&$fields, QueryConditionInterface $condition) {
    foreach ($condition->conditions() as $child_condition) {
      if ($child_condition['field'] instanceof QueryConditionInterface) {
        $this->removeFieldsInCondition($fields, $child_condition['field']);
      }
      else {
        unset($fields[$child_condition['field']]);
      }
    }
  }

  /**
   * Rewrite the query not to select non-affected rows.
   *
   * A query like this one:
   *   UPDATE test SET name = 'newname' WHERE tid = 1
   * will become:
   *   UPDATE test SET name = 'newname' WHERE tid = 1 AND name <> 'newname'
   */
  protected function excludeNonAffectedRows() {
    if (!empty($this->queryOptions['sqlsrv_return_matched_rows'])) {
      return;
    }

    // Get the fields used in the update query, and remove those that are already
    // in the condition.
    $fields = $this->expressionFields + $this->fields;
    $this->removeFieldsInCondition($fields, $this->condition);

    // Add the inverse of the fields to the condition.
    $condition = new DatabaseCondition('OR');
    foreach ($fields as $field => $data) {
      if (is_array($data)) {
        // The field is an expression.
        // Re-bind the placeholders.
        $expression = $data['expression'];
        $arguments = array();
        if (!empty($data['arguments'])) {
          foreach ($data['arguments'] as $placeholder => $value) {
            $new_placeholder = ':db_exclude_placeholder_' . $this->nextPlaceholder();
            $expression = str_replace($placeholder, $new_placeholder, $expression);
            $arguments[$new_placeholder] = $value;
          }
        }
        $condition->where($field . ' <> ' . $expression, $arguments);
        $condition->isNull($field);
      }
      elseif (!isset($data)) {
        // The field will be set to NULL.
        $condition->isNotNull($field);
      }
      else {
        $condition->condition($field, $data, '<>');
        $condition->isNull($field);
      }
    }
    if (count($condition)) {
      // Workaround for a bug in the base MergeQuery implementation:
      // a DatabaseCondition object is reused without being re-compiled,
      // leading to duplicate placeholders.
      $this->nextPlaceholder = 1000000;
      $this->condition($condition);
    }
  }

  public function execute() {
    // Rewrite the query to exclude the non-affected rows.
    $this->excludeNonAffectedRows();

    // Now perform the special handling for BLOB fields.
    $max_placeholder = 0;

    // Because we filter $fields the same way here and in __toString(), the
    // placeholders will all match up properly.
    $stmt = $this->connection->PDOPrepare($this->connection->prefixTables((string)$this));

    // Fetch the list of blobs and sequences used on that table.
    $columnInformation = $this->connection->schema()->queryColumnInformation($this->table);

    // Expressions take priority over literal fields, so we process those first
    // and remove any literal fields that conflict.
    $fields = $this->fields;
    $expression_fields = array();
    foreach ($this->expressionFields as $field => $data) {
      if (!empty($data['arguments'])) {
        foreach ($data['arguments'] as $placeholder => $argument) {
          // We assume that an expression will never happen on a BLOB field,
          // which is a fairly safe assumption to make since in most cases
          // it would be an invalid query anyway.
          $stmt->bindParam($placeholder, $data['arguments'][$placeholder]);
        }
      }
      unset($fields[$field]);
    }

    // We use this array to store references to the blob handles.
    // This is necessary because the PDO will otherwise messes up with references.
    $blobs = array();
    $blob_count = 0;

    foreach ($fields as $field => $value) {
      $placeholder = ':db_update_placeholder_' . ($max_placeholder++);

      if (isset($columnInformation['blobs'][$field])) {
        $blobs[$blob_count] = fopen('php://memory', 'a');
        fwrite($blobs[$blob_count], $value);
        rewind($blobs[$blob_count]);
        $stmt->bindParam($placeholder, $blobs[$blob_count], PDO::PARAM_LOB, 0, PDO::SQLSRV_ENCODING_BINARY);
        $blob_count++;
      }
      else {
        $stmt->bindParam($placeholder, $fields[$field]);
      }
    }

    if (count($this->condition)) {
      $this->condition->compile($this->connection, $this);

      $arguments = $this->condition->arguments();
      foreach ($arguments as $placeholder => $value) {
        $stmt->bindParam($placeholder, $arguments[$placeholder]);
      }
    }

    $options = $this->queryOptions;
    $options['already_prepared'] = TRUE;
    $stmt->execute();

    return $stmt->rowCount();
  }

  public function __toString() {
    // Create a sanitized comment string to prepend to the query.
    $prefix = $this->connection->makeComment($this->comments);

    // Expressions take priority over literal fields, so we process those first
    // and remove any literal fields that conflict.
    $fields = $this->fields;
    $update_fields = array();
    foreach ($this->expressionFields as $field => $data) {
      $update_fields[] = $this->connection->quoteIdentifier($field) . '=' . $data['expression'];
      unset($fields[$field]);
    }

    $max_placeholder = 0;
    foreach ($fields as $field => $value) {
      $update_fields[] = $this->connection->quoteIdentifier($field) . '=:db_update_placeholder_' . ($max_placeholder++);
    }

    $query = $prefix . 'UPDATE {' . $this->connection->escapeTable($this->table) . '} SET ' . implode(', ', $update_fields);

    if (count($this->condition)) {
      $this->condition->compile($this->connection, $this);
      // There is an implicit string cast on $this->condition.
      $query .= "\nWHERE " . $this->condition;
    }

    return $query;
  }
}

/**
 * SQL Server-specific implementation of TRUNCATE.
 */
class TruncateQuery_sqlsrv extends TruncateQuery {
  public function __toString() {
    // Create a sanitized comment string to prepend to the query.
    $prefix = $this->connection->makeComment($this->comments);

    return $prefix . 'TRUNCATE TABLE {' . $this->connection->escapeTable($this->table) . '} ';
  }
}

/**
 * SQL Server-specific implementation of the MERGE operation.
 */
class MergeQuery_sqlsrv extends MergeQuery {
  public function execute() {
    // We don't need INSERT or UPDATE queries to trigger additional transactions.
    $this->queryOptions['sqlsrv_skip_transactions'] = TRUE;

    return parent::execute();
  }
}
