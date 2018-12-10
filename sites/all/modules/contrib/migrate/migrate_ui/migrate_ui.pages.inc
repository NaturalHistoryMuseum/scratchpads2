<?php
/**
 * @file
 * Pages for managing migration processes.
 */

/**
 * Form for managing migration groups.
 */
function migrate_ui_migrate_dashboard($form, &$form_state) {
  drupal_set_title(t('Migrate dashboard'));

  $build = array();

  $build['overview'] = array(
    '#prefix' => '<div>',
    '#markup' => filter_xss_admin(migrate_overview()),
    '#suffix' => '</div>',
  );

  $header = array(
    'machinename' => array('data' => t('Group')),
    'status' => array('data' => t('Status')),
    'source_system' => array('data' => t('Source system')),
  );

  $result = db_select('migrate_group', 'mg')
    ->fields('mg', array('name', 'title', 'arguments'))
    ->execute();
  $rows = array();
  foreach ($result as $group_row) {
    $row = array();
    $migration_result = db_select('migrate_status', 'ms')
      ->fields('ms', array('machine_name', 'status', 'arguments'))
      ->condition('group_name', $group_row->name)
      ->execute();
    if (!$migration_result) {
      continue;
    }
    $status = t('Idle');
    $idle = TRUE;
    $machine_names = array();
    foreach ($migration_result as $migration_row) {
      switch ($migration_row->status) {
        case MigrationBase::STATUS_IMPORTING:
          $status = t('Importing');
          $idle = FALSE;
          break;
        case MigrationBase::STATUS_ROLLING_BACK:
          $status = t('Rolling back');
          $idle = FALSE;
          break;
        case MigrationBase::STATUS_STOPPING:
          $status = t('Stopping');
          $idle = FALSE;
          break;
      }
      $machine_names[] = $migration_row->machine_name;
    }

    // If we're not in the middle of an operaton, what's the status of the import?
    if ($idle) {
      $ready = TRUE;
      $complete = TRUE;
      foreach ($machine_names as $machine_name) {
        $migration = Migration::getInstance($machine_name);
        if (!$migration) {
          continue;
        }
        if (method_exists($migration, 'sourceCount') && method_exists($migration, 'processedCount')) {
          $source_count = $migration->sourceCount();
          $processed_count = $migration->processedCount();
          if ($processed_count > 0) {
            $ready = FALSE;
            if (!$complete) {
              break;
            }
          }
          if ($processed_count != $source_count) {
            $complete = FALSE;
            if (!$ready) {
              break;
            }
          }
        }
      }
      if ($ready) {
        $status = t('Ready to import');
      }
      elseif ($complete) {
        $status = t('Import complete');
      }
      else {
        $status = t('Import incomplete, not currently running');
      }
    }

    $row['status'] = $status;
    $row['machinename'] =
      l($group_row->title, 'admin/content/migrate/groups/' . $group_row->name);
    $arguments = unserialize($group_row->arguments);
    if (!empty($arguments['source_system'])) {
      $row['source_system'] = filter_xss_admin($arguments['source_system']);
    }
    else {
      $row['source_system'] = '';
    }
    $rows[$group_row->name] = $row;
  }

  $build['dashboard']['tasks'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#tree' => TRUE,
    '#empty' => t('No migration groups defined.'),
  );
  $build['operations'] = _migrate_ui_migrate_operations();

  return $build;
}

/**
 * Menu callback
 */
function migrate_ui_migrate_group($form, &$form_state, $group_name) {
  $group = MigrateGroup::getInstance($group_name);
  $build = array();

  $header = array(
    'machinename' => array('data' => t('Task')),
    'status' => array('data' => t('Status')),
    'importrows' => array('data' => t('Items')),
    'imported' => array('data' => t('Imported')),
    'unprocessed' => array('data' => t('Unprocessed')),
  );
  if (user_access(MIGRATE_ACCESS_ADVANCED)) {
    $header += array(
      'messages' => array('data' => t('Messages')),
      'lastthroughput' => array('data' => t('Throughput')),
      'lastimported' => array('data' => t('Last imported')),
    );
  }

  $migrations = migrate_migrations();

  $rows = array();
  foreach ($migrations as $migration) {
    $current_group = $migration->getGroup()->getName();
    if ($current_group != $group_name) {
      continue;
    }
    $row = array();
    $has_counts = TRUE;
    if (method_exists($migration, 'sourceCount')) {
      $total = $migration->sourceCount();
      if ($total < 0) {
        $has_counts = FALSE;
        $total = t('N/A');
      }
    }
    else {
      $has_counts = FALSE;
      $total = t('N/A');
    }
    if (method_exists($migration, 'importedCount')) {
      $imported = $migration->importedCount();
      $processed = $migration->processedCount();
    }
    else {
      $has_counts = FALSE;
      $imported = t('N/A');
    }
    if ($has_counts) {
      $unprocessed = $total - $processed;
    }
    else {
      $unprocessed = t('N/A');
    }
    $status = $migration->getStatus();
    switch ($status) {
      case MigrationBase::STATUS_IDLE:
        $status = t('Idle');
        break;
      case MigrationBase::STATUS_IMPORTING:
        $status = t('Importing');
        break;
      case MigrationBase::STATUS_ROLLING_BACK:
        $status = t('Rolling back');
        break;
      case MigrationBase::STATUS_STOPPING:
        $status = t('Stopping');
        break;
      case MigrationBase::STATUS_DISABLED:
        $status = t('Disabled');
        break;
      default:
        $status = t('Unknown');
        break;
    }

    $row['status'] = $status;
    $machine_name = $migration->getMachineName();
    $row['machinename'] =
      l($machine_name, "admin/content/migrate/groups/$group_name/$machine_name");
    $row['importrows'] = (int) $total;
    $row['imported'] = (int) $imported;
    $row['unprocessed'] = (int) $unprocessed;
    if (user_access(MIGRATE_ACCESS_ADVANCED)) {
      if (is_subclass_of($migration, 'Migration')) {
        $num_messages = $migration->messageCount();
        $row['messages'] = $num_messages ?
          l($num_messages, "admin/content/migrate/groups/$group_name/$machine_name/messages")
          : 0;
      }
      else {
        $row['messages'] = t('N/A');
      }
      if (method_exists($migration, 'getLastThroughput')) {
        $rate = $migration->getLastThroughput();
        if ($rate == '') {
          $row['lastthroughput'] = t('Unknown');
        }
        else {
          $row['lastthroughput'] = t('@rate/min', array('@rate' => $rate));
        }
      }
      else {
        $row['lastthroughput'] = t('N/A');
      }
      $row['lastimported'] = check_plain($migration->getLastImported());
    }
    $rows[$machine_name] = $row;
  }

  $build['dashboard']['migrations'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#tree' => TRUE,
    '#empty' => t('No tasks are defined for this migration group.'),
  );

  $build['operations'] = _migrate_ui_migrate_operations();

  return $build;
}

/**
 * @return array
 */
function _migrate_ui_migrate_operations() {
  // Build the 'Update options' form.
  $operations = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
  );

  //
  switch (variable_get('migrate_import_method', 0)) {
    case 1:
      $options = array(
        'import_background' => t('Import'),
        'rollback_background' => t('Rollback'),
      );
      break;
    case 2:
      $options = array(
        'import_immediate' => t('Import immediately'),
        'import_background' => t('Import in background'),
        'rollback_immediate' => t('Rollback immediately'),
        'rollback_background' => t('Rollback in background'),
      );
      break;
    case 0:
    default:
      $options = array(
        'import_immediate' => t('Import'),
        'rollback_immediate' => t('Rollback'),
      );
      break;
  }

  $options += array(
    'stop' => t('Stop'),
    'reset' => t('Reset'),
    'deregister' => t('Remove migration settings'),
  );
  $operations['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
  );
  $operations['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Execute'),
    '#validate' => array('migrate_ui_migrate_validate'),
    '#submit' => array('migrate_ui_migrate_submit'),
  );
  $operations['description'] = array(
    '#prefix' => '<p>',
    '#markup' => t(
      'Choose an operation to run on all selections above:
       <ul>
         <li>Import<br /> Imports all previously unprocessed records from the source, plus
             any records marked for update, into destination Drupal objects.</li>
         <li>Rollback<br /> Deletes all Drupal objects created by the import.</li>
         <li>Stop<br /> Cleanly interrupts any import or rollback processes that may
             currently be running.</li>
         <li>Reset<br /> Sometimes a process may fail to stop cleanly, and be
             left stuck in an Importing or Rolling Back status. Choose Reset to clear
             the status and permit other operations to proceed.</li>
         <li>Remove migration settings<br /> Removes all information about a migration group
             or task, while preserving any content that has already been imported.</li>
       </ul>'
    ),
    '#postfix' => '</p>',
  );

  $operations['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access(MIGRATE_ACCESS_ADVANCED),
  );
  $operations['options']['update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update'),
    '#description' => t('Check this box to update all previously-imported content
      in addition to importing new content. Leave unchecked to only import
      new content'),
  );
  $operations['options']['force'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore dependencies'),
    '#description' => t('Check this box to ignore dependencies when running imports
      - all tasks will run whether or not their dependent tasks have
      completed.'),
  );
  $operations['options']['limit'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('container-inline')),
    'value' => array(
      '#type' => 'textfield',
      '#title' => t('Limit to:'),
      '#size' => 10,
    ),
    'unit' => array(
      '#type' => 'select',
      '#options' => array(
        'items' => t('items'),
        'seconds' => t('seconds'),
      ),
      '#description' => t('Set a limit of how many items to process for
        each migration task, or how long each should run.'),
    ),
  );

  return $operations;
}

/**
 * Validate callback for the dashboard form.
 */
function migrate_ui_migrate_validate($form, &$form_state) {
  $migrations = _migrate_ui_selected_migrations($form_state['values']);
  if (empty($migrations)) {
    if (isset($form_state['values']['migrations'])) {
      form_set_error('', t('No items selected.'));
    }
  }
}

/**
 * Determine what migrations are selected (whether directly on a group page, or
 * via group selections on the dashboard).
 *
 * @param $values
 *
 * @return array
 */
function _migrate_ui_selected_migrations($values) {
  if (isset($values['migrations'])) {
    // From the specific group page, just take them in order (they were already
    // sorted by dependencies).
    $machine_names = array_filter($values['migrations']);
  }
  else {
    // From the groups page, we need to use migrate_migrations to be sure we've
    // got the tasks for each group in dependency order.
    $tasks = array_filter($values['tasks']);
    $migrations = migrate_migrations();
    $machine_names = array();
    foreach ($migrations as $migration) {
      $group_name = $migration->getGroup()->getName();
      if (in_array($group_name, $tasks)) {
        $machine_names[] = $migration->getMachineName();
      }
    }
  }
  return $machine_names;
}

/**
 * Submit callback for the dashboard form.
 */
function migrate_ui_migrate_submit($form, &$form_state) {
  $values = $form_state['values'];
  $operation = $values['operation'];

  $limit = $values['limit'];

  if (isset($values['update'])) {
    $update = $values['update'];
  }
  else {
    $update = 0;
  }
  if (isset($values['force'])) {
    $force = $values['force'];
  }
  else {
    $force = 0;
  }
  $machine_names = _migrate_ui_selected_migrations($values);
  $operations = array();

  // Rollback in reverse order.
  if (in_array($operation, array('rollback_immediate', 'deregister'))) {
    $machine_names = array_reverse($machine_names);
  }

  // Special case: when deregistering a group, go through the group API
  if ($operation == 'deregister' && isset($values['tasks'])) {
    foreach ($values['tasks'] as $task) {
      MigrateGroup::deregister($task);
    }
    return;
  }

  $drush_arguments = array();
  foreach ($machine_names as $machine_name) {
    $migration = Migration::getInstance($machine_name);
    if ($migration) {
      switch ($operation) {
        case 'import_immediate':
          // Update (if necessary) once, before starting
          if ($update && method_exists($migration, 'prepareUpdate')) {
            $migration->prepareUpdate();
          }
          $operations[] = array(
            'migrate_ui_batch',
            array('import', $machine_name, $limit, $force),
          );
          break;
        case 'rollback_immediate':
          $operations[] = array(
            'migrate_ui_batch',
            array('rollback', $machine_name, $limit, $force),
          );
          break;
        case 'import_background':
        case 'rollback_background':
          $drush_arguments[] = $machine_name;
          break;
        case 'stop':
          $migration->stopProcess();
          break;
        case 'reset':
          $migration->resetStatus();
          break;
        case 'deregister':
          migrate_ui_deregister_migration($machine_name);
          break;
      }
    }
  }

  // Only immediate rollback and import operations will need to go through Batch API.
  if (count($operations) > 0) {
    $batch = array(
      'operations' => $operations,
      'title' => t('Import processing'),
      'file' => drupal_get_path('module', 'migrate_ui') . '/migrate_ui.pages.inc',
      'init_message' => t('Starting import process'),
      'progress_message' => t(''),
      'error_message' => t('An error occurred. Some or all of the import processing has failed.'),
      'finished' => 'migrate_ui_batch_finish',
    );
    batch_set($batch);
  }
  elseif (count($drush_arguments) > 0) {
    $drush_path = trim(variable_get('migrate_drush_path', ''));
    // Check that $drush_path works. See migrate_ui_configure_form().
    if (!is_executable($drush_path)) {
      $message = t('To enable running operations in the background with <a href="@drush">drush</a>, (which is <a href="@recommended">recommended</a>), some configuration must be done on the server. See the <a href="@config">documentation</a> on <a href="@dorg">drupal.org</a>.',
        array(
          '@drush' => 'http://drupal.org/project/drush',
          '@recommended' => 'http://drupal.org/node/1806824',
          '@config' => 'http://drupal.org/node/1958170',
          '@dorg' => 'http://drupal.org/',
        )
      );
      drupal_set_message($message);
      return;
    }
    $uri = $GLOBALS['base_url'];
    $uid = $GLOBALS['user']->uid;
    if ($operation == 'import_background') {
      $command = 'mi';
      $log_suffix = '.import.log';
    }
    else {
      $command = 'mr';
      $log_suffix = '.rollback.log';
    }
    $migrations = implode(',', $drush_arguments);
    $drush_command = "$drush_path $command $migrations --user=$uid --uri=$uri " .
      '--root=' . DRUPAL_ROOT;
    if ($force) {
      $drush_command .= ' --force';
    }
    if ($update) {
      $drush_command .= ' --update';
    }
    if (variable_get('migrate_drush_mail', 0)) {
      $drush_command .= ' --notify';
    }
    if (!empty($limit['value'])) {
      $limit = $limit['value'] . ' ' . $limit['unit'];
      $drush_command .= " --limit=\"$limit\"";
    }
    $log_file = drupal_realpath('temporary://' . $drush_arguments[0] . $log_suffix);
    $drush_command .= " >$log_file 2>&1 &";
    exec($drush_command, $output, $status);
    if (variable_get('migrate_drush_mail', 0)) {
      drupal_set_message('Your operation is running in the background. You will receive an email message when it is complete.');
    }
    else {
      drupal_set_message('Your operation is running in the background. You may ' .
        'refresh the dashboard page to check on its progress.');
    }
  }
}

/**
 * Implements callback_batch_operation().
 *
 * Process all enabled migration processes in a browser, using the Batch API
 * to break it into manageable chunks.
 *
 * @param $operation
 *  Operation to perform - 'import', 'rollback', 'stop', or 'reset'.
 * @param $machine_name
 *  Machine name of migration to process.
 * @param $limit
 *  An array indicating the number of items to import or rollback, or the
 *  number of seconds to process. Should include 'unit' (either 'items' or
 *  'seconds') and 'value'.
 * @param $context
 *  Batch API context structure
 */
function migrate_ui_batch($operation, $machine_name, $limit, $force = FALSE, &$context) {
  // If we got a stop message, skip everything else
  if (isset($context['results']['stopped'])) {
    $context['finished'] = 1;
    return;
  }

  $migration = Migration::getInstance($machine_name);
  if (!$migration) {
    $context['finished'] = 1;
    return;
  }

  // Messages generated by migration processes will be captured in this global
  global $_migrate_messages;
  $_migrate_messages = array();
  Migration::setDisplayFunction('migrate_ui_capture_message');

  // Try to allocate enough time to run the full operation.
  $migration->setBatchTimeLimit();

  // Perform the requested operation
  switch ($operation) {
    case 'import':
      $result = $migration->processImport(array(
        'limit' => $limit,
        'force' => $force,
      ));
      break;
    case 'rollback':
      $result = $migration->processRollback(array(
        'limit' => $limit,
        'force' => $force,
      ));
      break;
  }

  switch ($result) {
    case Migration::RESULT_INCOMPLETE:
      // Default to half-done, in case we can't get a more precise fix
      $context['finished'] = .5;
      if (method_exists($migration, 'sourceCount')) {
        $total = $migration->sourceCount();
        if ($total > 0 && method_exists($migration, 'importedCount')) {
          $processed = $migration->processedCount();
          switch ($operation) {
            case 'import':
              $to_update = $migration->updateCount();
              $context['finished'] = ($processed - $to_update) / $total;
              break;
            case 'rollback':
              $context['finished'] = ($total - $migration->importedCount()) / $total;
              break;
          }
        }
      }
      break;
    case MigrationBase::RESULT_SKIPPED:
      $_migrate_messages[] = array(
        'message' => t("Skipped !name due to unfulfilled dependencies: !depends",
          array(
            '!name' => $machine_name,
            '!depends' => implode(", ", $migration->incompleteDependencies()),
          )),
      );
      $context['finished'] = 1;
      break;
    case MigrationBase::RESULT_STOPPED:
      $context['finished'] = 1;
      // Skip any further operations
      $context['results']['stopped'] = TRUE;
      break;
    default:
      $context['finished'] = 1;
      break;
  }

  // Add any messages generated in this batch to the cumulative list
  foreach ($_migrate_messages as $message_data) {
    $context['results'][] = $message_data;
  }

  // While in progress, show the cumulative list of messages
  $full_message = '';
  foreach ($context['results'] as $message_data) {
    $full_message .= $message_data['message'] . '<br />';
  }
  $context['message'] = $full_message;
}

/**
 * Implements callback_batch_finished().
 *
 * Report results.
 *
 * @param $success
 *  Ignored
 * @param $results
 *  List of results from batch processing
 * @param $operations
 *  Ignored
 */
function migrate_ui_batch_finish($success, $results, $operations) {
  unset($results['stopped']);

  foreach ($results as $result) {
    // Migration::progressMessage() uses message levels that don't match what
    // drupal_set_message() expects.
    if (isset($result['level'])) {
      if ($result['level'] == 'completed') {
        $level = 'status';
      }
      elseif ($result['level'] == 'failed') {
        $level = 'error';
      }
      else {
        $level = $result['level'];
      }
    }
    else {
      $level = NULL;
    }

    drupal_set_message($result['message'], $level);
  }
}

/**
 * Store a message to be displayed later.
 *
 * Ignore the message if $level is set to 'debug'.
 *
 * @param string $message
 *   the message to be displayed
 * @param string $level
 *   the type of the message: 'debug', 'completed', 'failed', or a valid $type
 *   used by drupal_set_message()
 */
function migrate_ui_capture_message($message, $level) {
  if ($level != 'debug') {
    // Store each message as an array with keys 'message' and 'level'.
    global $_migrate_messages;
    $_migrate_messages[] = array(
      'message' => filter_xss_admin($message),
      'level' => check_plain($level),
    );
  }
}

/**
 * Menu callback function for migration view page.
 */
function migrate_migration_info($form, $form_state, $group_name, $migration_name) {
  $migration = Migration::getInstance($migration_name);

  $has_mappings = $migration && method_exists($migration, 'getFieldMappings');
  $form = array();

  if ($has_mappings) {
    $field_mappings = $migration->getFieldMappings();
    // Identify what destination and source fields are mapped
    foreach ($field_mappings as $mapping) {
      $source_field = $mapping->getSourceField();
      $destination_field = $mapping->getDestinationField();
      $source_fields[$source_field] = $source_field;
      $destination_fields[$destination_field] = $destination_field;
    }

    $form['detail'] = array(
      '#type' => 'vertical_tabs',
      '#attached' => array(
        'js' => array(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.js'),
        'css' => array(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.css'),
      ),
    );
  }
  else {
    $form['detail'] = array(
      '#type' => 'fieldset',
    );
  }

  $form['overview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overview'),
    '#group' => 'detail',
  );

  if (!$migration) {
    return $form;
  }

  $team = array();
  foreach ($migration->getTeam() as $member) {
    $email_address = $member->getEmailAddress();
    $team[$member->getGroup()][] =
      $member->getName() . ' <' . l($email_address, 'mailto:' . $email_address) . '>';
  }

  foreach ($team as $group => $list) {
    $form['overview'][$group] = array(
      '#type' => 'item',
      '#title' => filter_xss_admin($group),
      '#markup' => filter_xss_admin(implode(', ', $list)),
    );
  }

  $dependencies = $migration->getHardDependencies();
  if (count($dependencies) > 0) {
    $form['overview']['dependencies'] = array(
      '#title' => t('Dependencies'),
      '#markup' => filter_xss_admin(implode(', ', $dependencies)),
      '#type' => 'item',
    );
  }
  $soft_dependencies = $migration->getSoftDependencies();
  if (count($soft_dependencies) > 0) {
    $form['overview']['soft_dependencies'] = array(
      '#title' => t('Soft Dependencies'),
      '#markup' => filter_xss_admin(implode(', ', $soft_dependencies)),
      '#type' => 'item',
    );
  }

  $form['overview']['group'] = array(
    '#title' => t('Group:'),
    '#markup' => filter_xss_admin($migration->getGroup()->getTitle()),
    '#type' => 'item',
  );

  if ($has_mappings) {
    switch ($migration->getSystemOfRecord()) {
      case Migration::SOURCE:
        $system_of_record = t('Source data');
        break;
      case Migration::DESTINATION:
        $system_of_record = t('Destination data');
        break;
      default:
        $system_of_record = t('Unknown');
        break;
    }
    $form['overview']['system_of_record'] = array(
      '#type' => 'item',
      '#title' => t('System of record:'),
      '#markup' => $system_of_record,
    );
  }

  $form['overview']['description'] = array(
    '#title' => t('Description:'),
    '#markup' => filter_xss_admin($migration->getDescription()),
    '#type' => 'item',
  );

  if ($has_mappings) {
    // Destination field information
    $form['destination'] = array(
      '#type' => 'fieldset',
      '#title' => t('Destination'),
      '#group' => 'detail',
      '#description' =>
        t('<p>These are the fields available in the destination of this migration
           task. The machine names listed here are those available to be used
           as the first parameter to $this->addFieldMapping() in your Migration
           class constructor. <span class="error">Unmapped fields are red</span>.</p>'),
    );
    $destination = $migration->getDestination();
    $form['destination']['type'] = array(
      '#type' => 'item',
      '#title' => t('Type'),
      '#markup' => filter_xss_admin((string) $destination),
    );
    $dest_key = $destination->getKeySchema();
    $header = array(t('Machine name'), t('Description'));
    $rows = array();
    foreach ($destination->fields($migration) as $machine_name => $description) {
      $classes = array();
      if (isset($dest_key[$machine_name])) {
        // Identify primary key
        $machine_name .= ' ' . t('(PK)');
      }
      else {
        // Add class for mapped/unmapped. Used in summary.
        $classes[] = !isset($destination_fields[$machine_name]) ? 'migrate-error' : '';
      }
      $rows[] = array(
        array('data' => check_plain($machine_name), 'class' => $classes),
        array('data' => filter_xss_admin($description), 'class' => $classes),
      );
    }
    $classes = array();

    $form['destination']['fields'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No fields'),
    );

    // TODO: Get source_fields from arguments
    $form['source'] = array(
      '#type' => 'fieldset',
      '#title' => t('Source'),
      '#group' => 'detail',
      '#description' =>
        t('<p>These are the fields available from the source of this migration
           task. The machine names listed here are those available to be used
           as the second parameter to $this->addFieldMapping() in your Migration
           class constructor. <span class="error">Unmapped fields are red</span>.</p>'),
    );
    $source = $migration->getSource();
    $form['source']['query'] = array(
      '#type' => 'item',
      '#title' => t('Query'),
      '#markup' => '<pre>' . filter_xss_admin($source) . '</pre>',
    );
    $source_key = $migration->getMap()->getSourceKey();
    $header = array(t('Machine name'), t('Description'));
    $rows = array();
    foreach ($source->fields() as $machine_name => $description) {
      if (isset($source_key[$machine_name])) {
        // Identify primary key
        $machine_name .= ' ' . t('(PK)');
      }
      else {
        // Add class for mapped/unmapped. Used in summary.
        $classes = !isset($source_fields[$machine_name]) ? 'migrate-error' : '';
      }
      $rows[] = array(
        array('data' => check_plain($machine_name), 'class' => $classes),
        array('data' => filter_xss_admin($description), 'class' => $classes),
      );
    }
    $classes = array();

    $form['source']['fields'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No fields'),
    );

    $header = array(
      t('Destination'),
      t('Source'),
      t('Default'),
      t('Description'),
      t('Priority'),
    );

    // First group the mappings
    $descriptions = array();
    $source_fields = $source->fields();
    $destination_fields = $destination->fields($migration);

    foreach ($field_mappings as $mapping) {
      // Validate source and destination fields actually exist
      $source_field = $mapping->getSourceField();
      $destination_field = $mapping->getDestinationField();
      if (!is_null($source_field) && !isset($source_fields[$source_field])) {
        drupal_set_message(t('"!source" was used as source field in the
          "!destination" mapping but is not in list of source fields', array(
          '!source' => filter_xss_admin($source_field),
          '!destination' => filter_xss_admin($destination_field),
        )),
          'warning');
      }
      if (!is_null($destination_field) && !isset($destination_fields[$destination_field])) {
        drupal_set_message(t('"!destination" was used as destination field in
          "!source" mapping but is not in list of destination fields', array(
          '!source' => filter_xss_admin($source_field),
          '!destination' => filter_xss_admin($destination_field),
        )),
          'warning');
      }
      $descriptions[$mapping->getIssueGroup()][] = $mapping;
    }

    // Put out each group header
    foreach ($descriptions as $group => $mappings) {
      $form[$group] = array(
        '#type' => 'fieldset',
        '#title' => t('Mapping: !group', array('!group' => filter_xss_admin($group))),
        '#group' => 'detail',
        '#attributes' => array('class' => array('migrate-mapping')),
      );
      $rows = array();
      foreach ($mappings as $mapping) {
        $default = $mapping->getDefaultValue();
        if (is_array($default)) {
          $default = implode(',', $default);
        }
        $issue_priority = $mapping->getIssuePriority();
        if (!is_null($issue_priority)) {
          $classes[] = 'migrate-priority-' . drupal_html_class($issue_priority);
          $priority = MigrateFieldMapping::$priorities[$issue_priority];
          $issue_pattern = $migration->getIssuePattern();
          $issue_number = $mapping->getIssueNumber();
          if (!is_null($issue_pattern) && !is_null($issue_number)) {
            $priority .= ' (' . l(t('#') . $issue_number, str_replace(':id:', $issue_number,
                $issue_pattern)) . ')';
          }
          if ($issue_priority != MigrateFieldMapping::ISSUE_PRIORITY_OK) {
            $classes[] = 'migrate-error';
          }
        }
        else {
          $priority = t('OK');
          $classes[] = 'migrate-priority-' . 1;
        }
        $destination_field = $mapping->getDestinationField();
        $source_field = $mapping->getSourceField();
        // Highlight any mappings overridden in the database.
        if ($mapping->getMappingSource() == MigrateFieldMapping::MAPPING_SOURCE_DB) {
          $destination_field = "<em>$destination_field</em>";
          $source_field = "<em>$source_field</em>";
        }
        $row = array(
          array(
            'data' => filter_xss_admin($destination_field),
            'class' => $classes,
          ),
          array('data' => filter_xss_admin($source_field), 'class' => $classes),
          array('data' => filter_xss_admin($default), 'class' => $classes),
          array(
            'data' => filter_xss_admin($mapping->getDescription()),
            'class' => $classes,
          ),
          array('data' => filter_xss_admin($priority), 'class' => $classes),
        );
        $rows[] = $row;
        $classes = array();
      }
      $form[$group]['table'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
      );
    }
  }
  return $form;
}

/**
 * Page callback to edit field mappings for a given migration.
 *
 * @param $form
 * @param $form_state
 * @param $group_name
 * @param $migration_name
 *
 * @return array
 */
function migrate_ui_edit_mappings($form, $form_state, $group_name,
                                  $migration_name) {
  drupal_set_title(t('Edit !migration', array('!migration' => filter_xss_admin($migration_name))));

  $form = array();
  $form['#tree'] = TRUE;

  $form['machine_name'] = array(
    '#type' => 'value',
    '#value' => $migration_name,
  );

  $migration = Migration::getInstance($migration_name);
  if (!$migration) {
    return $form;
  }

  $source_migration_options = array('-1' => t(''),);
  $all_dependencies = array();
  foreach (migrate_migrations() as $machine_name => $migration_info) {
    $source_migration_options[$machine_name] = $machine_name;
    $dependencies = $migration_info->getDependencies();
    if (!empty($dependencies)) {
      $all_dependencies[$machine_name] = $dependencies;
    }
  }

  if (is_a($migration, 'Migration')) {
    $source_fields = $migration->getSource()->fields();
    $dest_fields = $migration->getDestination()->fields($migration);
    $dest_key = $migration->getDestination()->getKeySchema();
    $field_mappings = $migration->getFieldMappings();

    $form['field_mappings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field mappings'),
      '#collapsible' => TRUE,
      '#description' => t('For each field available in your Drupal destination, select the source field used to populate it. You can enter a default value to be applied to the destination when there is no source field, or the source field is empty in a given source item. Check the DNM (Do Not Migrate) box for destination fields you do not want populated by migration. Note that any changes you make here override any field mappings defined by the underlying migration module. Clicking the Revert button will remove all such overrides.'),
      '#theme' => array('migrate_ui_field_mapping_form'),
      '#prefix' => '<div id="field-mappings">',
      '#suffix' => '</div>',
    );

    // So the theme function knows whether to include the xpath column.
    $form['field_mappings']['#is_xml_migration'] = is_a($migration, 'XMLMigration');

    $form['source_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Source fields'),
      '#collapsible' => TRUE,
      '#description' => t('Check off any source fields that are not being mapped to destination fields. They will be removed from the select lists above.'),
    );

    $base_options = array(
      '-1' => t(''),
    );

    $field_options = array();
    foreach ($source_fields as $name => $description) {
      // Clean up the source field description
      if (is_array($description)) {
        $description = reset($description);
      }
      $description = strip_tags($description);
      // Limit the description length
      if (strlen($description) > 50) {
        $short_description = substr($description, 0, 50) . '...';
      }
      else {
        $short_description = $description;
      }
      if (user_access(MIGRATE_ACCESS_ADVANCED)) {
        $label_format = '!description [!source_field]';
      }
      else {
        $label_format = '!description';
      }
      $label = t($label_format, array(
        '!source_field' => filter_xss_admin($name),
        '!description' => filter_xss_admin($description),
      ));
      $short_label = t($label_format, array(
        '!source_field' => filter_xss_admin($name),
        '!description' => filter_xss_admin($short_description),
      ));

      $dnm_value = 0;

      // Check for a click of the DNM box...
      if (isset($form_state['values']) &&
        $form_state['values']['source_fields'][$name] == 1) {
        $dnm_value = 1;
      }
      else {
        // ... or a source-only mapping with the DNM issue group.
        foreach ($field_mappings as $mapping) {
          if ($mapping->getSourceField() == $name &&
            $mapping->getIssueGroup() == t('DNM')) {
            $dnm_value = 1;
          }
        }
      }

      // So, if this field is not marked DNM, include it in possible source
      // options in the field mappings.
      if (!$dnm_value) {
        $field_options[$name] = $short_label;
      }

      $form['source_fields'][$name] = array(
        '#type' => 'checkbox',
        '#title' => $label,
        '#default_value' => $dnm_value,
      );
    }

    if (isset($field_mappings['is_new'])) {
      $default_is_new = $field_mappings['is_new']->getDefaultValue();
    }
    else {
      $default_is_new = 0;
    }
    foreach ($dest_fields as $name => $description) {
      // Don't map the destination key unless is_new = 1 (ie, TRUE)
      if (isset($dest_key[$name]) && $default_is_new == 0) {
        continue;
      }

      if (is_array($description)) {
        $description = reset($description);
      }

      $options = $base_options + $field_options;

      $default_mapping = '-1';
      $default_value = '';
      if (isset($field_mappings[$name])) {
        $mapping = $field_mappings[$name];
      }
      else {
        $mapping = NULL;
      }

      // If the DNM box has been clicked, make sure we clear the mapping and
      // default value fields.
      if (isset($form_state['values']) &&
        $form_state['values']['field_mappings'][$name]['issue_group'] == 1) {
        $dnm_value = 1;
      }
      else {
        // Determine the default field mapping - if we have an existing one, use
        // that, otherwise try to match on name.
        if (isset($field_mappings[$name])) {
          if ($mapping->getSourceField()) {
            $default_mapping = $mapping->getSourceField();
          }
          $default_value = $mapping->getDefaultValue();
          $dnm_value = ($mapping->getIssueGroup() == t('DNM'));
        }
        else {
          $dnm_value = 0;
        }
      }

      // Only show the machine name to advanced users.
      if (user_access(MIGRATE_ACCESS_ADVANCED)) {
        $label = '!description [!field_name]';
      }
      else {
        $label = '!description';
      }

      // Indent subfields and options to show their relationship to the parent.
      if (strpos($name, ':') > 0) {
        $description = '&nbsp;&nbsp;' . $description;
      }

      $form['field_mappings'][$name]['issue_group'] = array(
        '#type' => 'checkbox',
        '#default_value' => $dnm_value,
      );

      $form['field_mappings'][$name]['mapping'] = array(
        '#type' => 'select',
        '#title' => t($label,
          array('!description' => $description, '!field_name' => $name)),
        '#options' => $options,
        '#default_value' => $default_mapping,
      );

      $form['field_mappings'][$name]['default_value'] = array(
        '#type' => 'textfield',
        '#default_value' => $default_value,
        '#size' => 20,
      );

      $source_migration = NULL;
      if ($mapping) {
        $source_migration = $mapping->getSourceMigration();
      }
      if (!$source_migration) {
        $source_migration = '-1';
      }
      $form['field_mappings'][$name]['source_migration'] = array(
        '#type' => 'select',
        '#options' => $source_migration_options,
        '#default_value' => $source_migration,
      );

      if (is_a($mapping, 'MigrateXMLFieldMapping')) {
        /** @var MigrateXMLFieldMapping $mapping */
        $form['field_mappings'][$name]['xpath'] = array(
          '#type' => 'textfield',
          '#default_value' => $mapping->getXpath(),
          '#size' => 20,
        );
      }

    }
  }

  unset($source_migration_options[-1]);
  unset($source_migration_options[$migration_name]);

  $form['dependencies'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dependencies'),
    '#collapsible' => TRUE,
    '#collapsed' => is_a($migration, 'Migration'),
    '#theme' => 'migrate_ui_field_mapping_dependencies',
    '#description' => t('Set any dependencies on other migrations here. A <em>hard dependency</em> means that this migration is not allowed to run until the dependent migration has completed (without forcing it). A <em>soft dependency</em> places no such requirement - it simply affects the default order of migration. Note that any migrations that would lead to circular dependencies are not listed here.'),
  );

  // Get the list of possible dependencies - it's the same as the list of
  // possible source migrations, minus the empty choice and ourselves.

  $dependency_options = array(
    0 => t('None'),
    1 => t('Hard'),
    2 => t('Soft'),
  );

  // Remove any migrations depending on us, directly or indirectly. First, get
  // a list of such migrations.
  $descendent_migrations = _migrate_ui_get_descendents($migration_name,
    $all_dependencies);
  $source_migration_options = array_diff_key($source_migration_options,
    $descendent_migrations);
  foreach ($source_migration_options as $machine_name) {
    if (in_array($machine_name, $migration->getHardDependencies())) {
      $default_value = 1;
    }
    elseif (in_array($machine_name, $migration->getSoftDependencies())) {
      $default_value = 2;
    }
    else {
      $default_value = 0;
    }
    $form['dependencies'][$machine_name] = array(
      '#type' => 'select',
      '#title' => check_plain($machine_name),
      '#default_value' => $default_value,
      '#options' => $dependency_options,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Apply changes'),
  );

  $form['revert'] = array(
    '#type' => 'submit',
    '#value' => t('Revert'),
    '#submit' => array('migrate_ui_edit_mappings_revert'),
  );

  return $form;
}

/**
 * Generate a list of all migrations dependent on a given migration.
 *
 * @param $migration_name
 * @param array $all_dependencies
 *
 * @return array
 */
function _migrate_ui_get_descendents($migration_name, array $all_dependencies) {
  $descendents = array();
  foreach ($all_dependencies as $machine_name => $dependencies) {
    if (in_array($migration_name, $dependencies)) {
      $descendents[$machine_name] = $machine_name;
      $descendents += _migrate_ui_get_descendents($machine_name,
        $all_dependencies);
    }
  }
  return $descendents;
}

/**
 * Submit callback for the edit mappings form. Save any choices made in the
 * UI which override mappings made in code.
 *
 * @param $form
 * @param $form_state
 */
function migrate_ui_edit_mappings_submit(&$form, &$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  $row = db_select('migrate_status', 'ms')
    ->fields('ms', array('arguments', 'class_name', 'group_name'))
    ->condition('machine_name', $machine_name)
    ->execute()
    ->fetchObject();
  $class_name = $row->class_name;
  $group_name = $row->group_name;
  $arguments = unserialize($row->arguments);
  $arguments['field_mappings'] = array();
  $arguments['group_name'] = $group_name;
  $field_mappings = array();
  $default_values = array();
  $issue_group_values = array();
  $xpaths = array();

  $migration = Migration::getInstance($machine_name);
  if (is_a($migration, 'Migration')) {
    $xml = is_a($migration, 'XMLMigration') ? TRUE : FALSE;
    $existing_mappings = $migration->getFieldMappings();
    $coded_mappings = $migration->getCodedFieldMappings();
    foreach ($form_state['values']['field_mappings'] as $destination_field => $info) {
      // Treat empty strings as NULL.
      if ($info['default_value'] === '') {
        $info['default_value'] = NULL;
      }
      if ($xml && $info['xpath'] === '') {
        $info['xpath'] = NULL;
      }

      // If this mapping matches a coded mapping but not a stored mapping, remove
      // it entirely (don't store it in the database) so the coded mapping is not
      // overwritten.
      if (isset($coded_mappings[$destination_field])) {
        $coded_source_field =
          $coded_mappings[$destination_field]->getSourceField();
        $coded_default_value =
          $coded_mappings[$destination_field]->getDefaultValue();
        $coded_source_migration =
          $coded_mappings[$destination_field]->getSourceMigration();
        if ($xml) {
          $coded_xpath =
            $coded_mappings[$destination_field]->getXpath();
        }
        if ($info['mapping'] == '-1') {
          $info['mapping'] = NULL;
        }
        if ($info['source_migration'] == '-1') {
          $info['source_migration'] = NULL;
        }
        if ($info['issue_group'] == 0 && $coded_mappings[$destination_field]->getIssueGroup() != t('DNM') ||
          $info['issue_group'] == 1 && $coded_mappings[$destination_field]->getIssueGroup() == t('DNM')) {
          $dnm_matches = TRUE;
        }
        else {
          $dnm_matches = FALSE;
        }
        if ($info['mapping'] == $coded_source_field &&
          $info['default_value'] == $coded_default_value &&
          $info['source_migration'] == $coded_source_migration &&
          (!$xml || ($xml && ($info['xpath'] == $coded_xpath))) &&
          $dnm_matches) {
          continue;
        }
      }
      // This is not a match for a coded mapping, so we will want to save it.
      $field_mappings[$destination_field] = $info['mapping'];
      $default_values[$destination_field] = $info['default_value'];
      $source_migrations[$destination_field] = $info['source_migration'];
      $issue_group_values[$destination_field] = $info['issue_group'];
      if ($xml) {
        $xpaths[$destination_field] = $info['xpath'];
      }
    }

    foreach ($field_mappings as $destination_field => $source_field) {
      if ($source_field == -1) {
        $source_field = NULL;
      }

      if ($issue_group_values[$destination_field]) {
        $source_field = NULL;
        $default = NULL;
      }
      else {
        $default = $default_values[$destination_field];
      }

      // Until we can provide editing of all the options that go along with
      // field mappings, we want to avoid overwriting pre-existing mappings and
      // losing important bits.
      $mapping = NULL;
      if (isset($existing_mappings[$destination_field]) &&
        $issue_group_values[$destination_field] != 0) {
        /** @var MigrateFieldMapping $old_mapping */
        $old_mapping = $existing_mappings[$destination_field];
        if ($source_field == $old_mapping->getSourceField() &&
          $default_values[$destination_field] == $old_mapping->getDefaultValue() &&
          $source_migrations[$destination_field] == $old_mapping->getSourceMigration() &&
          (!$xml || ($xml && ($xpaths[$destination_field] == $old_mapping->getXpath())))) {
          // First, if this mapping matches a previously-stored mapping, we want to
          // preserve it as it was originally stored.
          if ($old_mapping->getMappingSource() ==
            MigrateFieldMapping::MAPPING_SOURCE_DB) {
            $mapping = $old_mapping;
          }
          // If it matches a coded mapping, then we don't want to save it at all.
          else {
            continue;
          }
        }
      }

      // We're not skipping this mapping, or preserving an old one, so create the
      // new mapping.
      if (!$mapping) {
        $mapping = _migrate_ui_get_mapping_object($migration, $destination_field, $source_field);
        $mapping->defaultValue($default);
        if ($xml && $xpaths[$destination_field]) {
          $mapping->xpath($xpaths[$destination_field]);
        }
      }

      if ($issue_group_values[$destination_field]) {
        $mapping->issueGroup(t('DNM'));
      }
      else {
        $mapping->issueGroup(NULL);
      }

      if ($source_migrations[$destination_field] &&
        $source_migrations[$destination_field] != '-1') {
        $mapping->sourceMigration($source_migrations[$destination_field]);
      }

      $arguments['field_mappings'][] = $mapping;
    }

    // Handle any source fields marked DNM.
    foreach ($form_state['values']['source_fields'] as $source_field => $value) {
      // Is this field ignored in the code?
      $code_ignored = FALSE;
      foreach ($coded_mappings as $destination_field => $mapping) {
        if (is_numeric($destination_field) &&
          $mapping->getSourceField() == $source_field) {
          $code_ignored = TRUE;
        }
      }
      // If it is marked DNM in the UI, but is not ignored in the code,
      // generate a DNM mapping.
      if ($value && !$code_ignored) {
        $mapping = _migrate_ui_get_mapping_object($migration, NULL, $source_field);
        $mapping->issueGroup(t('DNM'));
        $arguments['field_mappings'][] = $mapping;
      }
      // If it is marked DNM in the code, but is unchecked in the UI, see if
      // it's mapped to a destination field through the UI. If not, generate
      // an empty mapping without the DNM so it's available to be mapped.
      elseif (!$value && $code_ignored) {
        $mapping_found = FALSE;
        foreach ($field_mappings as $destination_field => $mapped_source_field) {
          if ($source_field == $mapped_source_field) {
            $mapping_found = TRUE;
            break;
          }
        }
        if (!$mapping_found) {
          $mapping = _migrate_ui_get_mapping_object($migration, NULL, $source_field);
          $arguments['field_mappings'][] = $mapping;
        }
      }
    }
  }

  $arguments['dependencies'] = $arguments['soft_dependencies'] = array();
  if (isset($form_state['values']['dependencies'])) {
    foreach ($form_state['values']['dependencies'] as $dependency => $value) {
      if ($value == 1) {
        $arguments['dependencies'][] = $dependency;
      }
      elseif ($value == 2) {
        $arguments['soft_dependencies'][] = $dependency;
      }
    }
  }

  Migration::registerMigration($class_name, $machine_name, $arguments);

  drupal_set_message(t('Migration changes applied.'));
  $form_state['redirect'] =
    "admin/content/migrate/groups/$group_name/$machine_name";
}

/**
 * Create a field mapping object of the appropriate class.
 *
 * @param $migration
 *
 * @return MigrateFieldMapping
 */
function _migrate_ui_get_mapping_object($migration, $destination, $source) {
  if (is_a($migration, 'XMLMigration')) {
    return new MigrateXMLFieldMapping($destination, $source);
  }
  else {
    return new MigrateFieldMapping($destination, $source);
  }
}

/**
 * Revert callback for the edit mappings form. Remove any field mappings that
 * were defined through the UI.
 *
 * @param $form
 * @param $form_state
 */
function migrate_ui_edit_mappings_revert(&$form, &$form_state) {
  $machine_name = $form_state['values']['machine_name'];
  db_delete('migrate_field_mapping')
    ->condition('machine_name', $machine_name)
    ->execute();
  // Note that not all field mappings in the database came from the UI - they
  // may also have been passed through a hook_migrate_api registration, so
  // reregister the migration to restore those mappings.
  migrate_static_registration(array($machine_name));
}

/**
 * Theme function to layout field mappings in a table.
 *
 * @param array $variables
 *
 * @return string
 *  Rendered markup.
 */
function theme_migrate_ui_field_mapping_form($variables) {
  $output = '';
  $form = $variables['field_mappings'];
  $elements = element_children($form);
  if (!empty($elements)) {
    $header = array(
      t('DNM'),
      t('Destination field'),
      t('Source field'),
      t('Default value'),
      t('Source migration'),
    );
    if (!empty($form['#is_xml_migration'])) {
      $header[] = t('Xpath');
    }
    $rows = array();
    foreach ($elements as $mapping_key) {
      $row = array();
      $title = $form[$mapping_key]['mapping']['#title'];
      unset($form[$mapping_key]['mapping']['#title']);
      $row[] = drupal_render($form[$mapping_key]['issue_group']);
      $row[] = $title;
      $row[] = drupal_render($form[$mapping_key]['mapping']);
      $row[] = drupal_render($form[$mapping_key]['default_value']);
      $row[] = drupal_render($form[$mapping_key]['source_migration']);
      if (!empty($form['#is_xml_migration'])) {
        $row[] = drupal_render($form[$mapping_key]['xpath']);
      }
      $rows[] = $row;
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Theme function to layout dependencies in a table.
 *
 * @param array $variables
 *
 * @return string
 *  Rendered markup.
 */
function theme_migrate_ui_field_mapping_dependencies($variables) {
  $output = '';
  $form = $variables['dependencies'];
  $header = array(t('Migration'), t('Dependency'));
  $rows = array();
  $elements = element_children($form);
  foreach ($elements as $mapping_key) {
    $row = array();
    $title = $form[$mapping_key]['#title'];
    unset($form[$mapping_key]['#title']);
    $row[] = $title;
    $row[] = drupal_render($form[$mapping_key]);
    $rows[] = $row;
  }
  $output .= theme('table', array(
    'rows' => $rows,
    'header' => $header,
    'empty' => t('No other migrations were found.'),
  ));

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Menu callback for messages page
 */
function migrate_ui_messages($group_name, $migration_name) {
  drupal_set_title(t('Import messages for !migration',
    array('!migration' => $migration_name)));

  $build = $rows = array();

  $migration = Migration::getInstance($migration_name);
  if (!$migration) {
    return $build;
  }

  $source_key = $migration->getMap()->getSourceKey();
  $source_key_map = $migration->getMap()->getSourceKeyMap();
  $source_key_map_flipped = array_flip($source_key_map);

  $header = array();
  // Add a table header for each source key in the migration's map.
  foreach ($source_key as $key => $map_info) {
    $header[] = array(
      'data' => isset($map_info['description']) ? filter_xss_admin($map_info['description']) : t('Source ID'),
      'field' => $source_key_map[$key],
      'sort' => 'asc',
    );
  }

  $header[] = array('data' => t('Level'), 'field' => 'level');
  $header[] = array('data' => t('Message'), 'field' => 'message');

  // TODO: need a general MigrateMap API
  $messages = $migration->getMap()->getConnection()
    ->select($migration->getMap()->getMessageTable(), 'msg')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->orderByHeader($header)
    ->limit(500)
    ->fields('msg')
    ->execute();

  foreach ($messages as $message) {
    $classes[] = $message->level <= MigrationBase::MESSAGE_WARNING ? 'migrate-error' : '';
    $row = array();
    // Add a table column for each source key.
    foreach ($source_key_map_flipped as $source_key => $source_field) {
      $row[] = array(
        'data' => filter_xss_admin($message->{$source_key}),
        'class' => $classes,
      );
    }
    $row[] = array(
      'data' => filter_xss_admin($migration->getMessageLevelName($message->level)),
      'class' => $classes,
    );
    $row[] = array(
      'data' => filter_xss_admin($message->message),
      'class' => $classes,
    );

    $rows[] = $row;

    unset($classes);
  }

  $build['messages'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No messages'),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.css'),
    ),
  );
  $build['migrate_ui_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Given a migration machine name, remove its tracking from the database.
 *
 * @param $machine_name
 */
function migrate_ui_deregister_migration($machine_name) {
  // The class is gone, so we'll manually clear migrate_status, and make
  // the default assumptions about the map/message tables.
  db_drop_table('migrate_map_' . strtolower($machine_name));
  db_drop_table('migrate_message_' . strtolower($machine_name));
  db_delete('migrate_status')
    ->condition('machine_name', $machine_name)
    ->execute();
  db_delete('migrate_field_mapping')
    ->condition('machine_name', $machine_name)
    ->execute();
  drupal_set_message(t("Deregistered '!description' task",
    array('!description' => $machine_name)));
}

/**
 * Menu callback
 */
function migrate_ui_configure() {
  drupal_set_title(t('Configure migration settings'));
  return drupal_get_form('migrate_ui_configure_form');
}

/**
 * Form for reviewing migrations.
 */
function migrate_ui_configure_form($form, &$form_state) {
  $build = array();

  $description = t('To register (or reregister with updated arguments) any
    migrations defined in hook_migrate_api(), click the <strong>Register</strong>
    button below.');

  $build['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration'),
    '#description' => $description,
  );

  $build['registration']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Register statically defined classes'),
    '#submit' => array('migrate_ui_configure_register_submit'),
  );

  $migrations = array();
  $result = db_select('migrate_status', 'ms')
    ->fields('ms', array('class_name', 'machine_name'))
    ->execute();
  $migration_list = '';
  foreach ($result as $row) {
    if (!class_exists($row->class_name)) {
      $migrations[] = $row->machine_name;
      $migration_list .= '<li>' . t('!migration (class !class)',
          array(
            '!migration' => filter_xss_admin($row->machine_name),
            '!class' => filter_xss_admin($row->class_name),
          )) . "</li>\n";
    }
  }

  if (!empty($migrations)) {
    $description = t('No class currently exists for the following migrations: <ul>!list</ul>',
      array('!list' => $migration_list));
    $build['deregistration'] = array(
      '#type' => 'fieldset',
      '#title' => t('Orphaned migration tasks'),
      '#description' => $description,
    );
    $build['deregistration']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Deregister orphans'),
      '#submit' => array('migrate_ui_configure_deregister_submit'),
    );
  }

  $build['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $build['settings']['deprecation_warnings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Warn on usage of deprecated patterns'),
    '#default_value' => variable_get('migrate_deprecation_warnings', 1),
    '#description' => t('When checked, you will receive warnings when using methods and patterns that are deprecated as of Migrate 2.6.'),
  );
  $build['settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('migrate_ui_configure_settings_submit'),
  );

  // Configure background imports if the drush command has been set.
  $drush_path = trim(variable_get('migrate_drush_path', ''));
  $drush_validated = FALSE;
  if ($drush_path && is_executable($drush_path)) {
    // Try running a drush status command to verify it's properly configured.
    $uri = $GLOBALS['base_url'];
    $uid = $GLOBALS['user']->uid;
    $command = "$drush_path status --user=$uid --uri=$uri --root=" . DRUPAL_ROOT;
    exec($command, $output, $status);
    if ($status == 0) {
      $version = '';
      foreach ($output as $line) {
        if (preg_match('|Drush version +: +(.*)|', $line, $matches)) {
          $version = trim($matches[1]);
        }
        elseif (preg_match('|Drupal bootstrap +: +Successful *|', $line, $matches)) {
          $drush_validated = TRUE;
        }
      }
    }
    if ($drush_validated) {
      $description = t('Configure the use of <a href="@drush">drush</a> version @version to run import and rollback operations in the background.',
        array(
          '@drush' => 'http://drupal.org/project/drush',
          '@version' => $version,
        )
      );
    }
    else {
      $description = t('<a href="@drush">Drush</a> is misconfigured on the server. Please review the <a href="@config">documentation</a> on <a href="@dorg">drupal.org</a>, and make sure everything is set up correctly.',
        array(
          '@drush' => 'http://drupal.org/project/drush',
          '@config' => 'http://drupal.org/node/1958170',
        )
      );
    }
  }
  else {
    $description = t('To enable running operations in the background with <a href="@drush">drush</a>, (which is <a href="@recommended">recommended</a>), some configuration must be done on the server. See the <a href="@config">documentation</a> on <a href="@dorg">drupal.org</a>.',
      array(
        '@drush' => 'http://drupal.org/project/drush',
        '@recommended' => 'http://drupal.org/node/1806824',
        '@config' => 'http://drupal.org/node/1958170',
        '@dorg' => 'http://drupal.org/',
      )
    );
  }

  $build['drush'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Background operations'),
    '#description' => $description,
  );

  if ($drush_validated) {
    $options = array(
      0 => t('Immediate operation only'),
      1 => t('Background operation only'),
      2 => t('Allows both immediate and background operations'),
    );
    $build['drush']['migrate_import_method'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => variable_get('migrate_import_method', 0),
      '#title' => t('Import method'),
      '#description' => t('Choose whether the dashboard will perform operations immediately, in the background via drush, or offer a choice.'),
    );

    $build['drush']['migrate_drush_mail'] = array(
      '#type' => 'checkbox',
      '#default_value' => variable_get('migrate_drush_mail', 0),
      '#title' => t('Send email notification when a background operation completes.'),
    );

    $build['drush']['migrate_drush_mail_subject'] = array(
      '#type' => 'textfield',
      '#default_value' => variable_get('migrate_drush_mail_subject',
        t('Migration operation completed')),
      '#title' => t('Subject'),
      '#states' => array(
        'visible' => array(
          ':input[name="migrate_drush_mail"]' => array('checked' => TRUE),
        ),
      ),
    );

    $build['drush']['migrate_drush_mail_body'] = array(
      '#type' => 'textarea',
      '#rows' => 10,
      '#default_value' => variable_get('migrate_drush_mail_body',
        t('The migration operation is complete. Please review the results on your dashboard')),
      '#title' => t('Body'),
      '#states' => array(
        'visible' => array(
          ':input[name="migrate_drush_mail"]' => array('checked' => TRUE),
        ),
      ),
    );

    $build['drush']['save_drush_config'] = array(
      '#type' => 'submit',
      '#value' => t('Save background configuration'),
      '#submit' => array('migrate_ui_configure_drush_submit'),
    );
  }

  $build['handlers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Handlers'),
    '#description' => t('In some cases, such as when a field handler for a contributed module is implemented in both migrate_extras and the module itself, you may need to disable a particular handler. In this case, you may uncheck the undesired handler below.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $build['handlers']['destination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination handlers'),
    '#collapsible' => TRUE,
  );

  $header = array(
    'module' => array('data' => t('Module')),
    'class' => array('data' => t('Class')),
    'types' => array('data' => t('Destination types handled')),
  );

  $disabled = unserialize(variable_get('migrate_disabled_handlers',
    serialize(array())));
  $class_list = _migrate_class_list('MigrateDestinationHandler');
  $rows = array();
  $default_values = array();
  foreach ($class_list as $class_name => $handler) {
    $row = array();
    $module = db_select('registry', 'r')
      ->fields('r', array('module'))
      ->condition('name', $class_name)
      ->condition('type', 'class')
      ->execute()
      ->fetchField();
    $row['module'] = check_plain($module);
    $row['class'] = check_plain($class_name);
    $row['types'] = filter_xss_admin(implode(', ', $handler->getTypesHandled()));
    $default_values[$class_name] = !in_array($class_name, $disabled);
    $rows[$class_name] = $row;
  }
  $build['handlers']['destination']['destination_handlers'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => $default_values,
    '#empty' => t('No destination handlers found'),
  );

  $build['handlers']['field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field handlers'),
    '#collapsible' => TRUE,
  );

  $header = array(
    'module' => array('data' => t('Module')),
    'class' => array('data' => t('Class')),
    'types' => array('data' => t('Field types handled')),
  );

  $class_list = _migrate_class_list('MigrateFieldHandler');
  $rows = array();
  $default_values = array();
  foreach ($class_list as $class_name => $handler) {
    $row = array();
    $module = db_select('registry', 'r')
      ->fields('r', array('module'))
      ->condition('name', $class_name)
      ->condition('type', 'class')
      ->execute()
      ->fetchField();
    $row['module'] = check_plain($module);
    $row['class'] = check_plain($class_name);
    $row['types'] = filter_xss_admin(implode(', ', $handler->getTypesHandled()));
    $default_values[$class_name] = !in_array($class_name, $disabled);
    $rows[$class_name] = $row;
  }
  $build['handlers']['field']['field_handlers'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => $default_values,
    '#empty' => t('No field handlers found'),
  );

  $build['handlers']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save handler statuses'),
    '#submit' => array('migrate_ui_configure_submit'),
  );

  return $build;
}

/**
 * Submit callback for the configuration form registration fieldset.
 */
function migrate_ui_configure_register_submit($form, &$form_state) {
  migrate_static_registration();
  drupal_set_message(t('All statically defined migrations have been (re)registered.'));
  $form_state['redirect'] = 'admin/content/migrate';
}

/**
 * Submit callback for the configuration form deregistration fieldset.
 */
function migrate_ui_configure_deregister_submit($form, &$form_state) {
  $result = db_select('migrate_status', 'ms')
    ->fields('ms', array('class_name', 'machine_name'))
    ->execute();
  foreach ($result as $row) {
    if (!class_exists($row->class_name)) {
      migrate_ui_deregister_migration($row->machine_name);
    }
  }
  $form_state['redirect'] = 'admin/content/migrate';
}

/**
 * Submit callback for the configuration form settings fieldset.
 */
function migrate_ui_configure_settings_submit($form, &$form_state) {
  variable_set('migrate_deprecation_warnings',
    $form_state['values']['deprecation_warnings']);
  drupal_set_message(t('Migration settings saved.'));
}

/**
 * Submit callback for the drush configuration form handler fieldset.
 */
function migrate_ui_configure_drush_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('migrate_import_method', $values['migrate_import_method']);
  variable_set('migrate_drush_mail', $values['migrate_drush_mail']);
  variable_set('migrate_drush_mail_subject', $values['migrate_drush_mail_subject']);
  variable_set('migrate_drush_mail_body', $values['migrate_drush_mail_body']);
}

/**
 * Submit callback for the handler configuration form handler fieldset.
 */
function migrate_ui_configure_submit($form, &$form_state) {
  $disabled = array();
  foreach ($form_state['values']['destination_handlers'] as $class => $value) {
    if (!$value) {
      $disabled[] = $class;
    }
  }
  foreach ($form_state['values']['field_handlers'] as $class => $value) {
    if (!$value) {
      $disabled[] = $class;
    }
  }
  variable_set('migrate_disabled_handlers', serialize($disabled));
  if (!empty($disabled)) {
    drupal_set_message(t('The following handler classes are disabled: @classes',
      array('@classes' => implode(', ', $disabled))));
  }
  else {
    drupal_set_message(t('No handler classes are currently disabled.'));
  }
}
