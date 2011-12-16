<?php
// $Id: database_mysql_dump.inc,v 1.20 2010/09/04 00:24:29 sun Exp $

// Some older mysql client libs are missing this constant.
if (!defined('MYSQLI_BINARY_FLAG')) {
  define('MYSQLI_BINARY_FLAG', 128);
}

/**
 * Dump active database.
 *
 * @param $filename
 *   The filename including path to write the dump to.
 * @param $options
 *   An associative array of snapshot options, as described in demo_dump().
 */
function demo_dump_db($filename, $options = array()) {
  // Make sure we have permission to save our backup file.
  $directory = dirname($filename);
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    return FALSE;
  }

  if ($fp = fopen($filename, 'wb')) {
    $header = array();
    $header[] = '-- Demo module database dump';
    $header[] = '-- Version ' . DEMO_DUMP_VERSION;
    $header[] = '-- http://drupal.org/project/demo';
    $header[] = '--';
    $header[] = '-- Database: ' . _demo_get_database();
    $header[] = '-- Date: ' . format_date(REQUEST_TIME, 'small');
    $header[] = '-- Server version: ' . db_query('SELECT version()')->fetchField();
    $header[] = '-- PHP version: ' . PHP_VERSION;
    $header[] = '-- Drupal version: ' . VERSION;

    // Avoid auto value for zero values (required for user id 0).
    $header[] = '';
    $header[] = 'SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";';
    // Temporarily disable foreign key checks for the time of import.
    $header[] = 'SET FOREIGN_KEY_CHECKS = 0;';
    $header[] = '';

    // Set collations for the import. PMA and mysqldump use conditional comments
    // to exclude MySQL <4.1, but D6 requires >=4.1.
    $header[] = 'SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT;';
    $header[] = 'SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS;';
    $header[] = 'SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION;';
    $header[] = 'SET NAMES utf8;';
    $header[] = '';

    fwrite($fp, implode("\n", $header));

    foreach ($options['tables'] as $table => $dump_options) {
      if (!_demo_table_is_view($table)) {
        if ($dump_options['schema']) {
          _demo_dump_table_schema($fp, $table);
        }
        if ($dump_options['data']) {
          _demo_dump_table_data($fp, $table);
        }
      }
    }

    $footer = array();
    $footer[] = '';
    // Re-enable foreign key checks.
    $footer[] = 'SET FOREIGN_KEY_CHECKS = 1;';

    // Revert collations for potential subsequent database queries not belonging
    // to this module.
    // @todo Double-check this behavior according to the results of
    //   http://drupal.org/node/772678
    $footer[] = 'SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT;';
    $footer[] = 'SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS;';
    $footer[] = 'SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION;';
    $footer[] = '';

    $footer[] = '';
    fwrite($fp, implode("\n", $footer));

    fclose($fp);
    return TRUE;
  }

  return FALSE;
}

/**
 * Returns the name of the active database.
 */
function _demo_get_database() {
  $database = array_keys(db_query('SHOW TABLES')->fetchAssoc());
  $database = preg_replace('/^Tables_in_/i', '', $database[0]);
  return $database;
}

/**
 * Enumerate database tables.
 */
function _demo_enum_tables() {
  return db_query('SHOW TABLES')->fetchCol();
}

/**
 * Dump table schema.
 *
 * @param $fp
 *   The file handle of the output file.
 * @param $table
 *   A table name to export the schema for.
 */
function _demo_dump_table_schema($fp, $table) {
  $output = "\n";
  $output .= "--\n";
  $output .= "-- Table structure for table '$table'\n";
  $output .= "--\n\n";

  $data = db_query("SHOW CREATE TABLE `$table`")->fetchAssoc();
  $status = db_query('SHOW TABLE STATUS LIKE :table', array(':table' => $table))->fetchAssoc();

  // Column keys in $status start with a lower-case letter in PDO and with a
  // upper-case letter otherwise. We convert all to lower-case.
  foreach ($status as $key => $value) {
    $key_lower = strtolower($key);
    if ($key[0] != $key_lower[0]) {
      $status[$key_lower] = $value;
      unset($status[$key]);
    }
  }

  // Add IF NOT EXISTS to CREATE TABLE, replace double quotes with MySQL quotes.
  $output .= preg_replace(
    array('/^CREATE TABLE/', '/"/'),
    array('CREATE TABLE IF NOT EXISTS', '`'),
    $data['create table']
  );

  // @todo Rethink the following code. Perhaps try to strip + parse the existing
  //   table definition (after leading ")" on last line) and merge anything
  //   missing into it, and re-append it again. There are too many differences
  //   between MySQL 5.0 and 5.1+, and PHP mysql(i) and pdo_mysql extensions.

  // PDO is missing the table engine.
  if (!strpos($output, ' ENGINE=')) {
    $output .= ' ENGINE=' . $status['engine'];
  }

  // Always add charset and collation info to table definitions.
  // SHOW CREATE TABLE does not contain collation information, if the collation
  // is equal to the default collation of the connection. Since dumps can be
  // moved across servers, we need to ensure correct collations.
  // Note that [DEFAULT] CHARSET or [DEFAULT] CHARACTER SET is always contained
  // on MySQL 5.1, even if it is equal to the default.
  // This addition assumes that a collation specified for a table is taken over
  // for the table's columns. The MySQL manual does not state whether this is
  // the case, but manual tests confirmed that it works that way.
  // Like Drupal core, we need to enforce UTF8 as character set and
  // utf8_general_ci as default database collation, if not overridden via
  // settings.php.
  if (!strpos($output, 'COLLATE=')) {
    // Only if the collation contains a underscore, the first string up to the
    // first underscore is the character set.
    // @see PMA_exportDBCreate()
    if (strpos($status['collation'], '_')) {
      $collate = 'COLLATE=' . $status['collation'];
    }
    // If there is a character set defined already, just append the collation.
    if (strpos($output, 'CHARSET') || strpos($output, 'CHARACTER SET')) {
      // @todo This may also hit column definitions instead of the table
      //   definition only. Should technically also be case-insensitive.
      $output = preg_replace('@((?:DEFAULT )?(?:CHARSET|CHARACTER SET) \w+)@', '$1 ' . $collate, $output);
    }
    else {
      $output .= ' DEFAULT CHARSET=utf8 ' . $collate;
    }
  }

  // Add the table comment, if any.
  if (!preg_match('@^\) .*COMMENT.+$@', $output) && !empty($status['comment'])) {
    // On PHP 5.2.6/Win32 with PDO MySQL 5.0 with InnoDB, the table comment has
    // a trailing "; InnoDB free: 84992 kB".
    $status['comment'] = preg_replace('@; InnoDB free: .+$@', '', $status['comment']);
    $output .= " COMMENT='" . $status['comment'] . "'";
  }

  // @todo Depends on whether we dump data and table existence on import.
//  if (!empty($status['auto_increment'])) {
//    $output .= ' AUTO_INCREMENT=' . $status['auto_increment'];
//  }

  $output .= ";\n";
  fwrite($fp, $output);
}

/**
 * Dump table data.
 *
 * This code has largely been stolen from the phpMyAdmin project.
 *
 * @param $fp
 *   The file handle of the output file.
 * @param $table
 *   A table name to export the data for.
 */
function _demo_dump_table_data($fp, $table) {
  $output = "\n";
  $output .= "--\n";
  $output .= "-- Dumping data for table '$table'\n";
  $output .= "--\n\n";

  // Dump table data.
  $result = db_query("SELECT * FROM `$table`", array(), array('fetch' => PDO::FETCH_ASSOC));

  // Get table fields.
  if ($fields = _demo_get_fields($result)) {
    // Disable indices to speed up import.
    $output .= "/*!40000 ALTER TABLE $table DISABLE KEYS */;\n";

    // Escape backslashes, PHP code, special chars
    $search = array('\\', "'", "\x00", "\x0a", "\x0d", "\x1a");
    $replace = array('\\\\', "''", '\0', '\n', '\r', '\Z');

    $insert_cmd = "INSERT INTO `$table` VALUES\n";
    $insert_buffer = '';
    $current_row = 0;
    $query_size = 0;

    foreach ($result as $row) {
      $current_row++;
      $values = array();
      $field = 0;
      foreach ($row as $value) {
        // NULL
        if (!isset($value) || is_null($value)) {
          $values[] = 'NULL';
        }
        // A number
        // timestamp is numeric on some MySQL 4.1, BLOBs are sometimes numeric
        else if ($fields[$field]->numeric && !$fields[$field]->timestamp && !$fields[$field]->blob) {
          $values[] = $value;
        }
        // A true BLOB
        // - mysqldump only generates hex data when the --hex-blob
        //   option is used, for fields having the binary attribute
        //   no hex is generated
        // - a TEXT field returns type blob but a real blob
        //   returns also the 'binary' flag
        else if ($fields[$field]->binary && $fields[$field]->blob) {
          // Empty blobs need to be different, but '0' is also empty :-(
          if (empty($value) && $value != '0') {
            $values[] = "''";
          }
          else {
            $values[] = '0x' . bin2hex($value);
          }
        }
        // Something else -> treat as a string
        else {
          $values[] = "'" . str_replace($search, $replace, $value) . "'";
        }
        $field++;
      }

      if ($current_row == 1) {
        $insert_buffer = $insert_cmd . '(' . implode(', ', $values) . ')';
      }
      else {
        $insert_buffer = '(' . implode(', ', $values) . ')';

        // Check if buffer size exceeds 50KB.
        if ($query_size + strlen($insert_buffer) > 50000) {
          // Flush to disc and start new buffer.
          fwrite($fp, $output . ";\n");
          $output = '';
          $current_row = 1;
          $query_size = 0;
          $insert_buffer = $insert_cmd . $insert_buffer;
        }
      }
      $query_size += strlen($insert_buffer);

      $output .= ($current_row == 1 ? '' : ",\n") . $insert_buffer;
    }

    if ($current_row > 0) {
      $output .= ";\n";
    }

    // Enable indices again.
    $output .= "/*!40000 ALTER TABLE $table ENABLE KEYS */;\n";
  }

  fwrite($fp, $output);
}

/**
 * Return table fields and their properties.
 */
function _demo_get_fields($result) {
  $fields = array();

  switch (db_driver()) {
    case 'mysql':
      $i = 0;
      while ($meta = $result->getColumnMeta($i)) {
        settype($meta, 'object');
        // pdo_mysql does not add a native type for INT fields.
        if (isset($meta->native_type)) {
          // Enhance the field definition for mysql-extension compatibilty.
          $meta->numeric = (strtolower($meta->native_type) == 'short');
          $meta->blob = (strtolower($meta->native_type) == 'blob');
          // Add custom properties.
          $meta->timestamp = (strtolower($meta->native_type) == 'long');
        }
        else {
          $meta->numeric = $meta->blob = $meta->timestamp = FALSE;
        }
        $meta->binary = (array_search('not_null', $meta->flags));
        $fields[] = $meta;
        $i++;
      }
      break;
  }

  return $fields;
}

/**
 * Determine whether the given table is a VIEW.
 */
function _demo_table_is_view($table) {
  static $tables = array();
  if (!isset($tables[$table])) {
    $status = db_query('SHOW TABLE STATUS LIKE :table', array(':table' => $table))->fetchAssoc();
    $tables[$table] = (strtoupper(substr($status['comment'], 0, 4)) == 'VIEW');
  }
  return $tables[$table];
}

