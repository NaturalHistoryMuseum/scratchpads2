<?php
// $Id: demo.api.php,v 1.1 2009/11/09 23:28:08 sun Exp $

/**
 * @file
 * Documentation for Demonstration site module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter snapshot options before a snapshot is created.
 *
 * @param &$options
 *   A structured array consisting of submitted form values:
 *   - filename: The base output filename, without extension.
 *   - default: Whether to set this dump as new default snapshot.
 *   - description: A description for the snapshot. If a snapshot with the same
 *     name already exists and this is left blank, the new snapshot will reuse
 *     the existing description.
 *   - tables: An array of tables to dump, keyed by table name (including table
 *     prefix, if any). The value is an array of dump options:
 *     - schema: Whether to dump the table schema.
 *     - data: Whether to dump the table data.
 */
function hook_demo_dump_alter(&$options) {
  // Only export the table schema of table cache_table, but not the data.
  // Commonly used for cache tables.
  $options['tables']['cache_table']['data'] = FALSE;

  // Completely ignore tables starting with a certain prefix.
  foreach ($options['tables'] as $table => $dump_options) {
    // Test if the table name starts with 'unrelated_'.
    if (strncmp($table, 'unrelated_', 10) == 0) {
      unset($options['tables'][$table]);
    }
  }
}

/**
 * @} End of "addtogroup hooks".
 */
