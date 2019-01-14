<?php

/**
 * @file
 * Drush support for the migrate module
 */

/**
 * Implements hook_drush_command().
 */
function migrate_drush_command() {
  $migration_options = array(
    'limit' => 'Limit on the length of each migration process, expressed in seconds or number of items',
    'feedback' => 'Frequency of progress messages, in seconds or items processed',
    'idlist' => 'A comma delimited list of ids to import or rollback. If unspecified, migrate imports all pending items or rolls back all items for the content set.',
    'all' => 'Process all migrations that come after the specified migration. If no value is supplied, all migrations are processed.',
    'instrument' => 'Capture performance information (timer, memory, or all)',
    'force' => 'Force an operation to run, even if all dependencies are not satisfied',
    'group' => 'Name of the migration group to run',
    'notify' => 'Send email notification upon completion of operation',
    'wildcard' => 'Process migrations that match a certain pattern. For example, Content*.',
  );
  $items['migrate-status'] = array(
    'description' => 'List all migrations with current status.',
    'options' => array(
      'refresh' => 'Recognize new migrations and update counts',
      'group' => 'Name of the migration group to list',
      'names-only' => 'Only return names, not all the details (faster)',
    ),
    'arguments' => array(
      'migration' => 'Restrict to a single migration. Optional',
    ),
    'examples' => array(
      'migrate-status' => 'Retrieve status for all migrations',
      'migrate-status BeerNode' => 'Retrieve status for just one migration',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('ms'),
  );
  $items['migrate-fields-destination'] = array(
    'description' => 'List the fields available for mapping in a destination.',
    'options' => array(
      'all' => $migration_options['all'],
      'group' => $migration_options['group'],
    ),
    'arguments' => array(
      'migration' => 'Name of the migration or destination class to query for fields',
    ),
    'examples' => array(
      'migrate-fields-destination MyNode' => 'List fields for the destination in the MyNode migration',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mfd'),
  );
  $items['migrate-fields-source'] = array(
    'description' => 'List the fields available for mapping from a source.',
    'arguments' => array(
      'migration' => 'Name of the migration or destination class to query for fields',
    ),
    'options' => array(
      'all' => $migration_options['all'],
      'group' => $migration_options['group'],
    ),
    'examples' => array(
      'migrate-fields-destination MyNode' => 'List fields in the source query for the MyNode migration',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mfs'),
  );
  $items['migrate-mappings'] = array(
    'description' => 'View information on all field mappings in a migration.',
    'options' => array(
      'all' => $migration_options['all'],
      'group' => $migration_options['group'],
      'csv' => 'Export information as a CSV',
      'full' => 'Include more information on each mapping',
    ),
    'arguments' => array(
      'migration' => 'Name of the migration',
    ),
    'examples' => array(
      'migrate-mappings MyNode' => 'Show mappings for the MyNode migration',
      'migrate-mappings MyNode --csv --full' => 'Export full mapping information in CSV format',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mm'),
  );
  $items['migrate-messages'] = array(
    'description' => 'View any messages associated with a migration.',
    'options' => array(
      'csv' => 'Export messages as a CSV',
    ),
    'arguments' => array(
      'migration' => 'Name of the migration',
    ),
    'examples' => array(
      'migrate-messages MyNode' => 'Show all messages for the MyNode migration',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mmsg'),
  );
  $items['migrate-analyze'] = array(
    'description' => 'Analyze the source fields for a migration.',
    'options' => array(
      'all' => $migration_options['all'],
      'group' => $migration_options['group'],
    ),
    'arguments' => array(
      'migration' => 'Name of the migration',
    ),
    'examples' => array(
      'migrate-analyze MyNode' => 'Report on field values for the MyNode migration',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('maz'),
  );
  $items['migrate-audit'] = array(
    'description' => 'View information on problems in a migration.',
    'options' => array(
      'all' => $migration_options['all'],
      'group' => $migration_options['group'],
    ),
    'arguments' => array(
      'migration' => 'Name of the migration',
    ),
    'examples' => array(
      'migrate-audit MyNode' => 'Report on problems in the MyNode migration',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('ma'),
  );
  $items['migrate-rollback'] = array(
    'description' => 'Roll back the destination objects from a given migration',
    'options' => $migration_options,
    'arguments' => array(
      'migration' => 'Name of migration(s) to roll back. Delimit multiple using commas.',
    ),
    'examples' => array(
      'migrate-rollback Article' => 'Roll back the article migration',
      'migrate-rollback Article --idlist=4,9' => 'Roll back two articles. The ids refer to the value of the primary key in base table',
      'migrate-rollback User --limit="50 items"' =>
        'Roll back up to 50 items from the migration named User',
      'migrate-rollback User --feedback="60 seconds"' => 'Display a progress message every 60 seconds or less',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mr'),
  );
  $migration_options['update'] = 'In addition to processing unprocessed items from the source, update previously-imported items with new data';
  $migration_options['needs-update'] =
    'Reimport up to 10K records where needs_update=1. This option is only needed when your Drupal DB is on a different DB server from your source data. Otherwise, these records get migrated with just migrate-import.';
  $migration_options['stop'] = 'Stop specified migration(s) if applicable.';
  $migration_options['rollback'] = 'Rollback specified migration(s) if applicable.';
  $migration_options['file_function'] = 'Override file function to use when migrating images.';
  $migration_options['ignore-highwater'] = 'Ignore the highwater field during migration';
  $items['migrate-import'] = array(
    'description' => 'Perform one or more migration processes',
    'options' => $migration_options,
    'arguments' => array(
      'migration' => 'Name of migration(s) to import. Delimit multiple using commas.',
    ),
    'examples' => array(
      'migrate-import Article' => 'Import new articles',
      'migrate-import Article --update' => 'Import new items, and also update previously-imported items',
      'migrate-import Article --idlist=4,9' => 'Import two specific articles. The ids refer to the value of the primary key in base table',
      'migrate-import Article --idlist=450:pasta,451' => 'Import two specific articles. A colon can be used to separate parts of compound keys; otherwise, compound keys match by the first key field.',
      'migrate-import Article --limit="60 seconds" --stop --rollback' =>
        'Import for up to 60 seconds after stopping and rolling back the Article migration.',
      'migrate-import Article --limit="100 items"' =>
        'Import up to 100 items from the migration named Article.',
      'migrate-import User --feedback="1000 items"' => 'Display a progress message every 1000 processed items or less',
      'migrate-import --all=User' => 'Perform User migrations and all that follow it.',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mi'),
  );
  $items['migrate-stop'] = array(
    'description' => 'Stop an active migration operation',
    'options' => array(
      'all' => 'Stop all active migration operations',
      'group' => 'Name of a specific migration group to stop',
    ),
    'arguments' => array(
      'migration' => 'Name of migration to stop',
    ),
    'examples' => array(
      'migrate-stop Article' => 'Stop any active operation on the Article migration',
      'migrate-stop --all' => 'Stop all active migration operations',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mst'),
  );
  $items['migrate-reset-status'] = array(
    'description' => 'Reset a active migration\'s status to idle',
    'options' => array('all' => 'Reset all active migration operations'),
    'arguments' => array(
      'migration' => 'Name of migration to reset',
    ),
    'examples' => array(
      'migrate-reset-status Article' => 'Reset any active operation on the Article migration',
      'migrate-reset-status --all' => 'Reset all active migration operations',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mrs'),
  );
  $items['migrate-deregister'] = array(
    'description' => 'Remove all tracking of a migration',
    'options' => array(
      'orphans' => 'Remove tracking for any migrations whose implementing class no longer exists',
      'group' => 'Remove tracking of a migration group, and any migrations assigned to it',
    ),
    'arguments' => array(
      'migration' => 'Name of migration to deregister',
    ),
    'examples' => array(
      'migrate-deregister Article' => 'Deregister the Article migration',
      'migrate-deregister --orphans' => 'Deregister any no-longer-implemented migrations',
      'migrate-deregister --group=myblog' => 'Deregister the myblog group and all migrations within it',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mdreg'),
  );
  $items['migrate-auto-register'] = array(
    'description' => 'Register any newly defined migration classes',
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mar'),
  );
  $items['migrate-register'] = array(
    'description' => 'Register or reregister any statically defined migrations',
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mreg'),
  );
  $items['migrate-wipe'] = array(
    'description' => 'Delete all nodes from specified content types.',
    'examples' => array(
      "migrate-wipe story article" => 'Delete all story and article nodes.',
    ),
    'arguments' => array(
      'type' => 'A space delimited list of content type machine readable Ids.',
    ),
    'drupal dependencies' => array('migrate'),
    'aliases' => array('mw'),
  );
  return $items;
}

/**
 * Get the value of all migrate related options. Used when spawning a subshell.
 * Don't pass along all, stop, update, and rollback options.
 *
 * @return
 *   An array of command specific options and their values.
 */
function drush_migrate_get_options() {
  $options = array();
  $blacklist = array('stop', 'rollback', 'update', 'all', 'group');
  $command = drush_parse_command();
  $global_options = drush_get_global_options();
  $opts = array_merge($command['options'], $global_options);
  foreach ($opts as $key => $value) {
    // Strip leading --
    $key = ltrim($key, '-');
    if (!in_array($key, $blacklist)) {
      $value = drush_get_option($key);
      if (isset($value)) {
        $options[$key] = $value;
      }
    }
  }
  return $options;
}

/*
 * Spawn a subshell which runs the same command we are currently running.
 */
function drush_migrate_invoke_process($migrations = '') {
  $args = drush_get_arguments();
  $options = drush_migrate_get_options();
  if (intval(DRUSH_MAJOR_VERSION) < 4) {
    // @todo: use drush_backend_invoke_args() as per http://drupal.org/node/658420.
    return drush_backend_invoke(implode(' ', $args), $options);
  }
  else {
    // $args[0] is the command name, $args[1] is the list of migrations.
    if (empty($migrations)) {
      $command_args = array($args[1]);
    }
    else {
      $command_args = array($migrations);
    }
    $return = drush_invoke_process('@self', $args[0], $command_args, $options);
    return $return;
  }
}

/**
 * A simplified version of the dashboard page.
 */
function drush_migrate_status($name = NULL) {
  try {
    $refresh = drush_get_option('refresh');
    $group_option = drupal_strtolower(drush_get_option('group'));
    $names_only = drush_get_option('names-only');

    // Validate input and load Migration(s).
    if ($name) {
      if ($migration = MigrationBase::getInstance($name)) {
        $migrations = array($migration);
      }
      else {
        return drush_set_error(dt('Unrecognized migration: !cn', array('!cn' => $name)));
      }
    }
    else {
      $migrations = migrate_migrations();
    }

    $groups = MigrateGroup::groups();
    $table = array();
    foreach ($groups as $group) {
      if ($group_option && drupal_strtolower($group->getName()) != $group_option) {
        continue;
      }
      $group_members_count = 0;
      foreach ($migrations as $migration) {
        if ($migration->getGroup() != $group) {
          // This migration is not from this group.
          continue;
        }
        ++$group_members_count;
        if ($group_members_count == 1) {
          // An empty line and the headers.
          $table[] = array('');
          if ($names_only) {
            $table[] = array(
              dt('Group: !name',
                array('!name' => $group->getName())),
            );
          }
          else {
            $table[] = array(
              dt('Group: !name',
                array('!name' => $group->getName())),
              dt('Total'),
              dt('Imported'),
              dt('Unprocessed'),
              dt('Status'),
              dt('Last imported'),
            );
          }
        }
        if (!$names_only) {
          $has_counts = TRUE;
          if (method_exists($migration, 'sourceCount')) {
            $total = $migration->sourceCount($refresh);
            if ($total < 0) {
              $has_counts = FALSE;
              $total = dt('N/A');
            }
          }
          else {
            $has_counts = FALSE;
            $total = dt('N/A');
          }
          if (method_exists($migration, 'importedCount')) {
            $imported = $migration->importedCount();
            $processed = $migration->processedCount();
          }
          else {
            $has_counts = FALSE;
            $imported = dt('N/A');
          }
          if ($has_counts) {
            $unimported = $total - $processed;
          }
          else {
            $unimported = dt('N/A');
          }
          $status = $migration->getStatus();
          switch ($status) {
            case MigrationBase::STATUS_IDLE:
              $status = dt('Idle');
              break;
            case MigrationBase::STATUS_IMPORTING:
              $status = dt('Importing');
              break;
            case MigrationBase::STATUS_ROLLING_BACK:
              $status = dt('Rolling back');
              break;
            case MigrationBase::STATUS_STOPPING:
              $status = dt('Stopping');
              break;
            case MigrationBase::STATUS_DISABLED:
              $status = dt('Disabled');
              break;
            default:
              $status = dt('Unknown');
              break;
          }
          $table[] = array(
            $migration->getMachineName(),
            $total,
            $imported,
            $unimported,
            $status,
            $migration->getLastImported(),
          );
        }
        else {
          $table[] = array($migration->getMachineName());
        }
      }
    }
    drush_print_table($table);
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

// TODO: Use drush_choice for detailed field info
function drush_migrate_fields_destination($args = NULL) {
  try {
    $migrations = drush_migrate_get_migrations($args);
    foreach ($migrations as $name => $migration) {
      drush_print("\n" . dt('@migration Destination Fields', array('@migration' => $name)) . "\n");

      $destination = $migration->getDestination();
      if (method_exists($destination, 'fields')) {
        $table = array();
        foreach ($destination->fields($migration) as $machine_name => $description) {
          $table[] = array(strip_tags($description), $machine_name);
        }
        drush_print_table($table);
      }
      else {
        drush_print(dt('No fields were found.'));
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

function drush_migrate_fields_source($args = NULL) {
  try {
    $migrations = drush_migrate_get_migrations($args);
    foreach ($migrations as $name => $migration) {
      drush_print("\n" . dt('@migration Source Fields', array('@migration' => $name)) . "\n");

      $source = $migration->getSource();
      if (method_exists($source, 'fields')) {
        $table = array();
        foreach ($source->fields() as $machine_name => $description) {
          $table[] = array(strip_tags($description), $machine_name);
        }
        drush_print_table($table);
      }
      else {
        drush_print(dt('No fields were found.'));
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Display field mappings for a migration.
 */
function drush_migrate_mappings($args = NULL) {
  try {
    $full = drush_get_option('full');
    $migrations = drush_migrate_get_migrations($args);
    foreach ($migrations as $name => $migration) {
      drush_print("\n" . dt('@migration Mappings', array('@migration' => $name)) . "\n");

      // In verbose mode, we'll also get source and destination field descriptions
      if ($full) {
        $destination = $migration->getDestination();
        $dest_descriptions = array();
        if (method_exists($destination, 'fields')) {
          foreach ($destination->fields($migration) as $machine_name => $description) {
            if (is_array($description)) {
              $description = reset($description);
            }
            $dest_descriptions[$machine_name] = strip_tags($description);
          }
        }
        $source = $migration->getSource();
        $src_descriptions = array();
        if (method_exists($source, 'fields')) {
          foreach ($source->fields() as $machine_name => $description) {
            if (is_array($description)) {
              $description = reset($description);
            }
            $src_descriptions[$machine_name] = strip_tags($description);
          }
        }
      }

      if (method_exists($migration, 'getFieldMappings')) {
        // First group the mappings. We want "interesting" mappings first, so
        // put the boring Done and DNM mappings last.
        $descriptions = array();
        $done = array();
        $dnm = array();
        foreach ($migration->getFieldMappings() as $mapping) {
          $group = $mapping->getIssueGroup();
          $lowergroup = drupal_strtolower($group);
          if ($lowergroup == dt('done')) {
            $done[$group][] = $mapping;
          }
          elseif ($lowergroup == dt('dnm') || $lowergroup == dt('do not migrate')) {
            $dnm[$group][] = $mapping;
          }
          else {
            $descriptions[$group][] = $mapping;
          }
        }
        $descriptions = array_merge($descriptions, $done, $dnm);

        // Put out each group header
        $table = array();
        if ($full) {
          $table[] = array(
            dt('Destination'),
            dt(''),
            dt('Source'),
            dt(''),
            dt('Default'),
            dt('Description'),
          );
        }
        else {
          $table[] = array(
            dt('Destination'),
            dt('Source'),
            dt('Default'),
            dt('Description'),
          );
        }
        $first = TRUE;

        foreach ($descriptions as $group => $mappings) {
          if ($first) {
            $first = FALSE;
          }
          else {
            $table[] = array(' ');
          }
          // Attempt to highlight the group header a bit so it stands out
          $group_header = '--- ' . drupal_strtoupper($group) . ' ---';
          $table[] = array($group_header);
          foreach ($mappings as $mapping) {
            if (is_array($mapping->getDefaultValue())) {
              $default = implode(',', $mapping->getDefaultValue());
            }
            else {
              $default = $mapping->getDefaultValue();
            }
            $destination = $mapping->getDestinationField();
            $source = $mapping->getSourceField();
            if ($full) {
              if ($destination && $dest_descriptions[$destination]) {
                $dest_description = $dest_descriptions[$destination];
              }
              else {
                $dest_description = '';
              }
              if ($source && $src_descriptions[$source]) {
                $src_description = $src_descriptions[$source];
              }
              else {
                $src_description = '';
              }
              $table[] = array(
                $destination,
                $dest_description,
                $source,
                $src_description,
                $default,
                $mapping->getDescription(),
              );
            }
            else {
              $table[] = array(
                $destination,
                $source,
                $default,
                $mapping->getDescription(),
              );
            }
          }
        }
        if (drush_get_option('csv')) {
          foreach ($table as $row) {
            fputcsv(STDOUT, $row);
          }
        }
        else {
          drush_print_table($table, TRUE);
        }
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Display messages for a migration.
 */
function drush_migrate_messages($migration_name) {
  if (!trim($migration_name)) {
    drush_log(dt('You must specify a migration name'), 'status');
    return;
  }
  try {
    $migration = MigrationBase::getInstance($migration_name);
    if (is_a($migration, 'Migration')) {
      $map = $migration->getMap();
      $message_table = $map->getMessageTable();
      $result = db_select($message_table, 'msg', array('fetch' => PDO::FETCH_ASSOC))
        ->fields('msg')
        ->execute();
      $first = TRUE;
      $table = array();
      foreach ($result as $row) {
        unset($row['msgid']);
        unset($row['level']);
        if ($first) {
          $table[] = array_keys($row);
          $first = FALSE;
        }
        $table[] = $row;
      }
    }
    if (empty($table)) {
      drush_log(dt('No messages for this migration'), 'status');
    }
    else {
      if (drush_get_option('csv')) {
        foreach ($table as $row) {
          fputcsv(STDOUT, $row);
        }
      }
      else {
        $widths = array();
        foreach ($table[0] as $header) {
          $widths[] = strlen($header) + 1;
        }
        drush_print_table($table, TRUE, $widths);
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Analyze the source fields for any passed migrations.
 */
function drush_migrate_analyze($args = NULL) {
  $migrations = drush_migrate_get_migrations($args);
  foreach ($migrations as $name => $migration) {
    // "Migrations" derived from MigrationBase won't have an analyze method.
    if (method_exists($migration, 'analyze')) {
      drush_print("\n" . dt('Analyzing @migration', array('@migration' => $name)) . "\n");
      $analysis = $migration->analyze();
      if (!empty($analysis)) {
        foreach ($analysis as $field_name => $details) {
          if (!empty($details['description'])) {
            drush_print(dt('@name (@description):', array(
              '@name' => $field_name,
              '@description' => $details['description'],
            )));
          }
          else {
            drush_print(dt('@name:', array('@name' => $field_name)));
          }
          // Special handling in degenerate cases
          if (count($details['distinct_values']) == 1) {
            $value = trim(reset(array_keys($details['distinct_values'])));
            if ($value === '') {
              drush_print('  ' . dt('The field is always empty'));
            }
            else {
              drush_print('  ' . dt('Only one value present: @value',
                  array('@value' => $value)));
            }
          }
          else {
            if ($details['is_numeric']) {
              drush_print('  ' . dt('Numeric field with a range of @min to @max',
                  array(
                    '@min' => $details['min_numeric'],
                    '@max' => $details['max_numeric'],
                  )));
            }
            else {
              drush_print('  ' . dt('String field with a length ranging from @min to @max',
                  array(
                    '@min' => $details['min_strlen'],
                    '@max' => $details['max_strlen'],
                  )));
            }
            $values = array();
            $header = NULL;
            // If the max of 10 tracked distinct values was reached, we assume
            // there are many values and treat them as samples. Under 10 this
            // may be an enumerated field, show all values with their counts.
            if (count($details['distinct_values']) < 10) {
              drush_print('  ' . dt('Distinct values:'));
              $header = array('', dt('Value'), dt('Count'));
              $values[] = $header;
            }
            else {
              drush_print('  ' . dt('Sample values:'));
            }
            ksort($details['distinct_values']);
            foreach ($details['distinct_values'] as $value => $count) {
              // Truncate long strings
              $value = substr($value, 0, 60);
              if (strlen($value) == 60) {
                $value .= dt('...');
              }
              $row = array('    ', $value);
              if (count($details['distinct_values']) < 10) {
                $row[] = $count;
              }
              $values[] = $row;
            }
            // No header for sample values.
            drush_print_table($values, !is_null($header));
          }
        }
      }
    }
  }
}

/**
 * Display field mappings for a migration.
 */
function drush_migrate_audit($args = NULL) {
  try {
    $problem_descriptions = array(
      'wtf' => dt("Probably an incomplete migration:"),
      'noted_issues' => dt("Noted as an issue:"),
      // I wish drush had dformat_plural().
      'sources_unmapped' => dt("Source(s) not used in a mapping:"),
      'sources_missing' => dt("Used as source field in mapping but not in source field list:"),
      'destinations_unmapped' => dt("Destination(s) not used in a mapping:"),
      'destinations_missing' => dt("Used as destination field in mapping but not in destination field list:"),
    );

    drush_print("Auditing migrations");

    $migrations = drush_migrate_get_migrations($args);
    foreach ($migrations as $name => $migration) {
      $problems = array();
      foreach ($problem_descriptions as $key => $description) {
        $problems[$key] = array();
      }

      drush_print("\n" . dt('@migration', array('@migration' => $name)) . "\n");

      if (!method_exists($migration, 'getSource') || !($source = $migration->getSource())) {
        $problems['wtf'][] = dt('Missing a source');
        $source_fields = array();
      }
      else {
        $source_fields = $source->fields();
      }

      if (!method_exists($migration, 'getDestination') || !($destination = $migration->getDestination())) {
        $problems['wtf'][] = dt('Missing a destination');
        $destination_fields = array();
      }
      else {
        $destination_fields = $destination->fields($migration);
      }

      if (!method_exists($migration, 'getFieldMappings')) {
        $problems['wtf'][] = dt('Missing field mappings');
        $field_mappings = array();
      }
      else {
        $field_mappings = $migration->getFieldMappings();
      }

      $used_sources = array();
      $used_destinations = array();
      foreach ($field_mappings as $mapping) {
        $source_field = $mapping->getSourceField();
        $destination_field = $mapping->getDestinationField();

        $used_sources[$source_field] = TRUE;
        $used_destinations[$destination_field] = TRUE;

        $issue_priority = $mapping->getIssuePriority();
        if (!is_null($issue_priority) && $issue_priority != MigrateFieldMapping::ISSUE_PRIORITY_OK) {
          $problems['noted_issues'][] = array(
            dt('Source') => $source_field,
            dt('Destination') => $destination_field,
            dt('Priority') => MigrateFieldMapping::$priorities[$issue_priority],
            dt('Description') => $mapping->getDescription(),
          );
        }

        // Validate source and destination fields actually exist
        if (!is_null($source_field) && !isset($source_fields[$source_field])) {
          $problems['sources_missing'][] = $source_field;
        }
        if (!is_null($destination_field) && !isset($destination_fields[$destination_field])) {
          $problems['destinations_missing'][] = $destination_field;
        }
      }

      foreach (array_diff_key($source_fields, $used_sources) as $name => $description) {
        $problems['sources_unmapped'][] = array(
          'Field' => $name,
          'Description' => $description,
        );
      }
      foreach (array_diff_key($destination_fields, $used_destinations) as $name => $description) {
        $problems['destinations_unmapped'][] = array(
          'Field' => $name,
          'Description' => $description,
        );
      }

      $problems = array_filter($problems);
      if (empty($problems)) {
        drush_print(dt('No problems found.') . "\n", 1);
      }
      else {
        foreach ($problems as $type => $some_problems) {
          drush_print($problem_descriptions[$type]);
          // If the contents of each row are arrays print it as a table.
          if (is_array($some_problems[0])) {
            $table = array_merge(array(array_keys($some_problems[0])), $some_problems);
            drush_print_table($table, TRUE);
          }
          else {
            foreach ($some_problems as $problem) {
              drush_print($problem, 1);
            }
            // Add an extra new line to keep the spacing consistent with the
            // tables.
            drush_print();
          }
        }
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Roll back one specified migration
 */
function drush_migrate_rollback($args = NULL) {
  try {
    if (drush_get_option('notify', FALSE)) {
      // Capture non-informational output for mailing
      ob_start();
      ob_implicit_flush(FALSE);
    }

    $migrations = drush_migrate_get_migrations($args);

    // Rollback in reverse order
    $migrations = array_reverse($migrations, TRUE);

    $options = array();
    if ($idlist = drush_get_option('idlist', FALSE)) {
      $options['idlist'] = $idlist;
    }
    if (drush_get_option('force', FALSE) == 1) {
      $options['force'] = TRUE;
    }
    $limit = drush_get_option('limit');
    if ($limit) {
      $parts = explode(' ', $limit);
      $options['limit']['value'] = $parts[0];
      $options['limit']['unit'] = $parts[1];
      if (!$options['limit']['unit']) {
        $options['limit']['unit'] = 'items';
      }
      elseif ($options['limit']['unit'] != 'seconds' &&
        $options['limit']['unit'] != 'second' &&
        $options['limit']['unit'] != 'items' &&
        $options['limit']['unit'] != 'item') {
        drush_set_error(NULL, dt("Invalid limit unit '!unit'",
          array('!unit' => $options['limit']['unit'])));
        return;
      }
    }

    $feedback = drush_get_option('feedback');
    if ($feedback) {
      $parts = explode(' ', $feedback);
      $options['feedback']['value'] = $parts[0];
      $options['feedback']['unit'] = $parts[1];
      if ($options['feedback']['unit'] != 'seconds' &&
        $options['feedback']['unit'] != 'second' &&
        $options['feedback']['unit'] != 'items' &&
        $options['feedback']['unit'] != 'item') {
        drush_set_error(NULL, dt("Invalid feedback frequency unit '!unit'",
          array('!unit' => $options['feedback']['unit'])));
        return;
      }
    }

    $instrument = drush_get_option('instrument');
    global $_migrate_track_memory, $_migrate_track_timer;
    switch ($instrument) {
      case 'timer':
        $_migrate_track_timer = TRUE;
        break;
      case 'memory':
        $_migrate_track_memory = TRUE;
        break;
      case 'all':
        $_migrate_track_timer = TRUE;
        $_migrate_track_memory = TRUE;
        break;
    }

    foreach ($migrations as $migration) {
      drush_log(dt("Rolling back '!description' migration",
        array('!description' => $migration->getMachineName())));
      $return = $migration->processRollback($options);
      // If it couldn't finish (presumably because it was appraoching memory_limit),
      // continue in a subprocess
      if ($return == MigrationBase::RESULT_INCOMPLETE) {
        drush_migrate_invoke_process();
      }
      // If stopped, don't process any further
      elseif ($return == MigrationBase::RESULT_STOPPED) {
        break;
      }
      elseif ($return == MigrationBase::RESULT_SKIPPED) {
        drush_log(dt("Skipping migration !name due to unfulfilled dependencies, use the --force option to run it anyway.",
          array('!name' => $migration->getMachineName())),
          'warning');
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
  if ($_migrate_track_memory) {
    drush_migrate_print_memory();
  }
  if ($_migrate_track_timer && !drush_get_context('DRUSH_DEBUG')) {
    drush_print_timers();
  }

  // Notify user
  if (drush_get_option('notify')) {
    _drush_migrate_notify();
  }
}

/**
 * Send email notification to the user running the operation.
 */
function _drush_migrate_notify() {
  global $user;
  if ($user->uid) {
    $uid = $user->uid;
  }
  else {
    $uid = 1;
  }
  $account = user_load($uid);
  $params['account'] = $account;
  $params['output'] = ob_get_contents();
  drush_print_r(ob_get_status());
  ob_end_flush();
  drupal_mail('migrate_ui', 'import_complete', $account->mail,
    user_preferred_language($account), $params);
}

function drush_migrate_get_migrations($args) {
  $migration_objects = migrate_migrations();

  if ($start = drush_get_option('all')) {
    // Handle custom first migration when --all=foo is supplied.
    $seen = $start === TRUE ? TRUE : FALSE;

    foreach ($migration_objects as $name => $migration) {
      if (!$seen && (drupal_strtolower($start) == drupal_strtolower($name))) {
        // We found our starting migration. $seen is always TRUE now.
        $seen = TRUE;
      }
      if (!$migration->getEnabled() || !$seen) {
        // This migration is disabled or is before our starting migration.
        unset($migration_objects[$name]);
      }
    }
  }
  elseif ($group = drush_get_option('group')) {
    foreach ($migration_objects as $name => $migration) {
      if (drupal_strtolower($group) !=
        drupal_strtolower($migration->getGroup()->getName()) ||
        !$migration->getEnabled()) {
        unset($migration_objects[$name]);
      }
    }
  }
  elseif ($wildcard = drush_get_option('wildcard')) {
    foreach ($migration_objects as $name => $migration) {
      if (!fnmatch(drupal_strtolower($wildcard), drupal_strtolower($name)) ||
        !$migration->getEnabled()) {
        unset($migration_objects[$name]);
      }
    }
  }
  else {
    $named_migrations = array();
    foreach (explode(',', $args) as $name) {
      $found = FALSE;
      foreach ($migration_objects as $machine_name => $migration) {
        if (drupal_strtolower($name) == drupal_strtolower($machine_name)) {
          if ($migration->getEnabled()) {
            $named_migrations[$name] = $migration;
            $found = TRUE;
            break;
          }
          else {
            drush_log(dt('Migration !name is disabled', array('!name' => $name)), 'warning');
          }
        }
      }
      if (!$found) {
        drush_log(dt('No migration with machine name !name found', array('!name' => $name)), 'error');
      }
    }
    $migration_objects = $named_migrations;
  }
  return $migration_objects;
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_fields_destination_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_fields_source_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_mappings_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_analyze_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_audit_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_import_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_stop_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_reset_status_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

// Implement drush_hook_COMMAND_validate().
function drush_migrate_rollback_validate($args = NULL) {
  return drush_migrate_validate_common($args);
}

function drush_migrate_validate_common($args) {
  if (drush_get_option('all')) {
    if (!empty($args) || drush_get_option('group') || drush_get_option('wildcard')) {
      return drush_set_error(NULL, dt('You must specify exactly one of a migration name, --all, or --group, or --wildcard'));
    }
  }
  elseif (drush_get_option('group')) {
    if (!empty($args) || drush_get_option('all') || drush_get_option('wildcard')) {
      return drush_set_error(NULL, dt('You must specify exactly one of a migration name, --all, or --group, or --wildcard'));
    }
  }
  elseif (drush_get_option('wildcard')) {
    if (!empty($args) || drush_get_option('all') || drush_get_option('group')) {
      return drush_set_error(NULL, dt('You must specify exactly one of a migration name, --all, or --group, or --wildcard'));
    }
  }
  else {
    if (empty($args)) {
      return drush_set_error(NULL, dt('You must specify exactly one of a migration name, --all, or --group, or --wildcard'));
    }
    $machine_names = explode(',', $args);

    foreach ($machine_names as $machine_name) {
      $machine_name = trim($machine_name);
      $class_name = db_select('migrate_status', 'ms')
        ->fields('ms', array('class_name'))
        ->condition('machine_name', $machine_name)
        ->execute()
        ->fetchField();
      if (!$class_name || !class_exists($class_name)) {
        drush_set_error(dt('Unrecognized migration: !name', array('!name' => $machine_name)));
      }
    }
  }

  $feedback = drush_get_option('feedback');
  if ($feedback) {
    $parts = explode(' ', $feedback);
    $options['feedback']['value'] = $parts[0];
    $options['feedback']['unit'] = $parts[1];
    if ($options['feedback']['unit'] != 'seconds' &&
      $options['feedback']['unit'] != 'second' &&
      $options['feedback']['unit'] != 'items' &&
      $options['feedback']['unit'] != 'item') {
      drush_set_error(NULL, dt("Invalid feedback frequency unit '!unit'",
        array('!unit' => $options['feedback']['unit'])));
      return;
    }
  }
}

/*
 * A 'pre' callback for migrate-import command.
 * Call migrate-stop and migrate-rollback commands if requested.
 */
function drush_migrate_pre_migrate_import($args = NULL) {
  if (drush_get_option('stop')) {
    drush_unset_option('stop');
    try {
      /** @var Migration[] $migrations */
      $migrations = drush_migrate_get_migrations($args);
      foreach ($migrations as $migration) {
        $status = $migration->getStatus();
        if ($status == MigrationBase::STATUS_IMPORTING ||
          $status == MigrationBase::STATUS_ROLLING_BACK) {
          drush_log(dt("Stopping '!description' migration", array('!description' => $migration->getMachineName())));
          $migration->stopProcess();
          // Give the process a chance to stop.
          $count = 0;
          while ($migration->getStatus() != MigrationBase::STATUS_IDLE
            && $count++ < 5) {
            sleep(1);
          }
        }
      }
    } catch (MigrateException $e) {
      drush_print($e->getMessage());
      exit;
    }
  }
  if (drush_get_option('rollback')) {
    drush_unset_option('rollback');
    drush_invoke('migrate-rollback', $args);
  }
}

/**
 * Perform import on one or more migrations.
 *
 * @param $machine_names
 *  A comma delimited list of machine names, or the special name 'all'
 */
function drush_migrate_import($args = NULL) {
  $status = NULL;

  try {
    if (drush_get_option('notify', FALSE)) {
      // Capture non-informational output for mailing
      ob_start();
      ob_implicit_flush(FALSE);
    }
    $migrations = drush_migrate_get_migrations($args);
    $options = array();
    if ($idlist = drush_get_option('idlist', FALSE)) {
      $options['idlist'] = $idlist;
    }
    if ($file_function = drush_get_option('file_function', '')) {
      $options['file_function'] = $file_function;
    }
    if (drush_get_option('force', FALSE) == 1) {
      $options['force'] = TRUE;
    }
    $limit = drush_get_option('limit');
    if ($limit) {
      $parts = explode(' ', $limit);
      $options['limit']['value'] = $parts[0];
      // Default unit.
      if (!isset($parts[1])) {
        $parts[1] = 'items';
      }
      $options['limit']['unit'] = $parts[1];
      // Validation.
      if (!in_array($options['limit']['unit'], array(
        'seconds',
        'second',
        'items',
        'item',
      ))) {
        drush_set_error(NULL, dt("Invalid limit unit '!unit'",
          array('!unit' => $options['limit']['unit'])));
        return;
      }
    }
    $feedback = drush_get_option('feedback');
    if ($feedback) {
      $parts = explode(' ', $feedback);
      $options['feedback']['value'] = $parts[0];
      $options['feedback']['unit'] = $parts[1];
      if ($options['feedback']['unit'] != 'seconds' &&
        $options['feedback']['unit'] != 'second' &&
        $options['feedback']['unit'] != 'items' &&
        $options['feedback']['unit'] != 'item') {
        drush_set_error(NULL, dt("Invalid feedback frequency unit '!unit'",
          array('!unit' => $options['feedback']['unit'])));
        return;
      }
    }
    $instrument = drush_get_option('instrument');
    global $_migrate_track_memory, $_migrate_track_timer;
    switch ($instrument) {
      case 'timer':
        $_migrate_track_timer = TRUE;
        break;
      case 'memory':
        $_migrate_track_memory = TRUE;
        break;
      case 'all':
        $_migrate_track_timer = TRUE;
        $_migrate_track_memory = TRUE;
        break;
    }

    $stop = FALSE;
    foreach ($migrations as $machine_name => $migration) {
      drush_log(dt("Importing '!description' migration",
        array('!description' => $machine_name)));
      if (drush_get_option('update') && !$idlist) {
        $migration->prepareUpdate();

        if (drush_get_option('ignore-highwater')) {
          $migration->setHighwaterField(array());
        }
      }
      if (drush_get_option('needs-update')) {
        $map_rows = $migration->getMap()->getRowsNeedingUpdate(10000);
        $idlist = array();
        foreach ($map_rows as $row) {
          $idlist[] = $row->sourceid1;
        }
        $options['idlist'] = implode(',', $idlist);
      }
      // The goal here is to do one migration in the parent process and then
      // spawn subshells as needed when memory is depleted. We show feedback
      // after each subshell depletes itself. Best we can do in PHP.
      if (!drush_get_context('DRUSH_BACKEND')) {
        // Our first pass and in the parent process. Run a migration right here.
        $status = $migration->processImport($options);
        if ($status == MigrationBase::RESULT_SKIPPED) {
          drush_log(dt("Skipping migration !name due to unfulfilled dependencies:\n  !depends\nUse the --force option to run it anyway.",
            array(
              '!name' => $machine_name,
              '!depends' => implode("\n  ", $migration->incompleteDependencies()),
            )),
            'warning');
        }
        elseif ($status == MigrationBase::RESULT_STOPPED) {
          break;
        }
        elseif ($status == MigrationBase::RESULT_INCOMPLETE) {
          $stop = TRUE;
        }

        // Subsequent run in the parent process. Spawn subshells ad infinitum.
        $migration_string = implode(',', array_keys($migrations));
        while ($status == MigrationBase::RESULT_INCOMPLETE) {
          $return = drush_migrate_invoke_process($migration_string);
          // 'object' holds the return code we care about.
          $status = $return['object']['status'];
          $migration_string = $return['object']['migrations'];
          if ($status == MigrationBase::RESULT_SKIPPED) {
            drush_log(dt("Skipping migration !name due to unfulfilled dependencies:\n  !depends\nUse the --force option to run it anyway.",
              array(
                '!name' => $machine_name,
                '!depends' => implode("\n  ", $migration->incompleteDependencies()),
              )),
              'warning');
          }
          elseif ($status == MigrationBase::RESULT_STOPPED) {
            $stop = TRUE;
            break;
          }
        }
      }
      else {
        // I'm in a subshell. Import then set return value so parent process can respawn or move on.
        $status = $migration->processImport($options);
        if ($status == MigrationBase::RESULT_SKIPPED) {
          drush_log(dt("Skipping migration !name due to unfulfilled dependencies:\n  !depends\n",
            array(
              '!name' => $machine_name,
              '!depends' => implode("\n  ", $migration->incompleteDependencies()),
            )),
            'warning');
        }
        elseif ($status == MigrationBase::RESULT_INCOMPLETE) {
          $stop = TRUE;
        }
        drush_backend_set_result(array(
          'status' => $status,
          'migrations' => implode(',', array_keys($migrations)),
        ));
      }

      if ($stop) {
        break;
      }
      unset($migrations[$machine_name]);
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }

  if ($_migrate_track_memory) {
    drush_migrate_print_memory();
  }

  if ($_migrate_track_timer && !drush_get_context('DRUSH_DEBUG')) {
    drush_print_timers();
  }

  // Notify user
  if (drush_get_option('notify')) {
    _drush_migrate_notify();
  }

  return $status;
}

/**
 * Stop clearing or importing a given content set.
 *
 * @param $content_set
 *  The name of the Migration
 */
function drush_migrate_stop($args = NULL) {
  try {
    $migrations = drush_migrate_get_migrations($args);
    foreach ($migrations as $migration) {
      drush_log(dt("Stopping '!description' migration", array('!description' => $migration->getMachineName())));
      $migration->stopProcess();
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Reset the status of a given migration.
 */
function drush_migrate_reset_status($args = NULL) {
  try {
    $migrations = drush_migrate_get_migrations($args);
    foreach ($migrations as $migration) {
      drush_log(dt("Resetting '!description' migration",
        array('!description' => $migration->getMachineName())));
      $migration->resetStatus();
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Deregister a given migration, migration group, or all orphaned migrations.
 * Note that the migration might no longer "exist" (the class implementation
 * might be gone), so we can't count on being able to instantiate it, or use
 * migrate_migrations().
 */
function drush_migrate_deregister($args = NULL) {
  try {
    $orphans = drush_get_option('orphans');
    $group = drush_get_option('group');
    if ($group) {
      MigrateGroup::deregister($group);
      drush_log(dt("Deregistered group '!description' and all its migrations",
        array('!description' => $group)), 'success');
    }
    else {
      if ($orphans) {
        $migrations = array();
        $result = db_select('migrate_status', 'ms')
          ->fields('ms', array('class_name', 'machine_name'))
          ->execute();
        foreach ($result as $row) {
          if (!class_exists($row->class_name)) {
            $migrations[] = $row->machine_name;
          }
        }
      }
      else {
        $migrations = explode(',', $args);
      }
      foreach ($migrations as $machine_name) {
        drush_migrate_deregister_migration(drupal_strtolower($machine_name));
        drush_log(dt("Deregistered '!description' migration",
          array('!description' => $machine_name)), 'success');
      }
    }
  } catch (MigrateException $e) {
    drush_print($e->getMessage());
    exit;
  }
}

/**
 * Given a migration machine name, remove its tracking from the database.
 *
 * @param $machine_name
 */
function drush_migrate_deregister_migration($machine_name) {
  // The class is gone, so we'll manually clear migrate_status, and make
  // the default assumptions about the map/message tables.
  db_drop_table('migrate_map_' . $machine_name);
  db_drop_table('migrate_message_' . $machine_name);
  db_delete('migrate_status')
    ->condition('machine_name', $machine_name)
    ->execute();
  db_delete('migrate_field_mapping')
    ->condition('machine_name', $machine_name)
    ->execute();
}

/**
 * Auto-registration is no longer supported. This command should be removed
 * entirely in a future point release.
 *
 * @deprecated
 */
function drush_migrate_auto_register($args = NULL) {
  drush_log(dt('The auto-registration feature has been removed. Migrations '
    . 'must now be explicitly registered.'), 'error');
}

/**
 * Register any migrations defined in hook_migrate_api().
 */
function drush_migrate_register($args = NULL) {
  migrate_static_registration();
  drush_log(dt('All statically defined migrations have been (re)registered.'), 'success');
}

/**
 * A drush command callback.
 */
function drush_migrate_wipe() {
  $types = func_get_args();
  $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', $types, 'IN')
    ->execute()
    ->fetchCol();
  $chunks = array_chunk($nids, 50);
  foreach ($chunks as $chunk) {
    node_delete_multiple($chunk);
  }
  migrate_instrument_stop('node_delete');
}

// Print all timers for the request.
function drush_migrate_print_memory() {
  global $_migrate_memory;
  $temparray = array();
  foreach ((array) $_migrate_memory as $name => $memoryrec) {
    // We have to use timer_read() for active timers, and check the record for others
    if (isset($memoryrec['start'])) {
      $temparray[$name] = migrate_memory_read($name);
    }
    else {
      $temparray[$name] = $memoryrec['bytes'];
    }
  }
  // Go no farther if there were no timers
  if (count($temparray) > 0) {
    // Put the highest cumulative times first
    arsort($temparray);
    $table = array();
    $table[] = array('Name', 'Cum (bytes)', 'Count', 'Avg (bytes)');
    foreach ($temparray as $name => $memory) {
      $count = $_migrate_memory[$name]['count'];
      if ($count > 0) {
        $avg = round($memory / $count, 0);
      }
      else {
        $avg = 'N/A';
      }
      $table[] = array($name, $memory, $count, $avg);
    }
    drush_print_table($table, TRUE);
  }
}

/**
 * Command argument complete callback.
 *
 * @return
 *   List of migrations.
 */
function migrate_migrate_status_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_import_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_rollback_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_fields_destination_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_fields_source_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_mappings_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_reset_status_complete() {
  return array('values' => drush_migrate_migrations());
}

function migrate_migrate_stop_complete() {
  return array('values' => drush_migrate_migrations());
}

function drush_migrate_migrations() {
  drush_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  $migrations = migrate_migrations();
  foreach ($migrations as $migration) {
    $values[] = $migration->getMachineName();
  }
  return $values;
}
