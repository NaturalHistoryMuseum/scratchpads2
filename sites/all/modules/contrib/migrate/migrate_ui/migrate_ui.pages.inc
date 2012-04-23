<?php

/**
 * @file
 */

/**
 * Menu callback
 */
function migrate_ui_dashboard() {
  drupal_set_title(t('Migrate'));
  return drupal_get_form('migrate_ui_dashboard_form');
}

/**
 * Form for reviewing migrations.
 */
function migrate_ui_dashboard_form($form, &$form_state) {
  $build = array();

  $build['overview'] = array(
    '#prefix' => '<div>',
    '#markup' => migrate_overview(),
    '#suffix' => '</div>',
  );

  $header = array(
    'status' => array('data' => t('Status')),
    'machinename' => array('data' => t('Migration')),
    'importrows' => array('data' => t('Total rows')),
    'imported' => array('data' => t('Imported')),
    'unimported' => array('data' => t('Unimported')),
    'messages' => array('data' => t('Messages')),
    'lastthroughput' => array('data' => t('Throughput')),
    'lastimported' => array('data' => t('Last imported')),
  );

  $migrations = migrate_migrations();

  $rows = array();
  foreach ($migrations as $migration) {
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
      $unimported = $total - $processed;
    }
    else {
      $unimported = t('N/A');
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
      l($machine_name, 'admin/content/migrate/' . $machine_name);
    $row['importrows'] = $total;
    $row['imported'] = $imported;
    $row['unimported'] = $unimported;

    if (is_subclass_of($migration, 'Migration')) {
      $num_messages = $migration->messageCount();
      $row['messages'] = $num_messages ? l($num_messages, 'admin/content/migrate/messages/' . $machine_name) : 0;
    }
    else {
      $row['messages'] = t('N/A');
    }
    if (method_exists($migration, 'getLastThroughput')) {
      $rate = $migration->getLastThroughput();
      if ($rate == '') {
        $row['lastthroughput'] = t('Unknown');
      }
      elseif ($status == MigrationBase::STATUS_IDLE) {
        $row['lastthroughput'] = t('!rate/min', array('!rate' => $rate));
      }
      else {
        if ($rate > 0) {
          $row['lastthroughput'] = t('!rate/min, !time remaining', array('!rate' => $rate, '!time' => format_interval((60*$unimported) / $rate)));
        }
        else {
          $row['lastthroughput'] = t('!rate/min, unknown time remaining', array('!rate' => $rate));
        }
      }
    }
    else {
      $row['lastthroughput'] = t('N/A');
    }
    $row['lastimported'] = $migration->getLastImported();
    $rows[$machine_name] = $row;
  }

  $build['dashboard'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No migrations'),
  );

  // Build the 'Update options' form.
  $build['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#attributes' => array('class' => array('container-inline')),
  );
  $build['operations']['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
//    '#attributes' => array('class' => array('container-inline')),
  );
  $build['operations']['options']['update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update'),
    '#description' => t('Check this box to update all previously-migrated content
      in addition to importing new content. Leave unchecked to only import
      new content'),
  );
  $build['operations']['options']['limit'] = array(
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
        each migration, or how long each should run.'),
    ),
  );
  $options = array(
    'import' => t('Import'),
    'rollback' => t('Rollback'),
    'rollback_and_import' => t('Rollback and import'),
    'stop' => t('Stop'),
    'reset' => t('Reset'),
  );
  $build['operations']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
  );
  $build['operations']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Execute'),
    '#validate' => array('migrate_ui_dashboard_validate'),
    '#submit' => array('migrate_ui_dashboard_submit'),
  );
  $build['operations']['description'] = array(
    '#prefix' => '<p>',
    '#markup' => t(
      'Choose an operation to run on all migrations selected above:
       <ul>
         <li>Import - Imports all previously unimported records from the source, plus
             any records marked for update, into destination Drupal objects.</li>
         <li>Rollback - Deletes all Drupal objects created by the migration.</li>
         <li>Rollback and import - Performs the Rollback operation, immediately
             followed by the Import operation.</li>
         <li>Stop - Cleanly interrupts any import or rollback processes that may
             currently be running.</li>
         <li>Reset - Sometimes a migration process may fail to stop cleanly, and be
             left stuck in an Importing or Rolling Back status. Choose Reset to clear
             the status and permit other operations to proceed.</li>
       </ul>'
    ),
    '#postfix' => '</p>',
  );

  return $build;
}

/**
 * Validate callback for the dashboard form.
 */
function migrate_ui_dashboard_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['dashboard']) || !count(array_filter($form_state['values']['dashboard']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Submit callback for the dashboard form.
 */
function migrate_ui_dashboard_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  $limit = $form_state['values']['limit'];
  $update = $form_state['values']['update'];
  $machine_names = array_filter($form_state['values']['dashboard']);
  $operations = array();

  // Rollback in reverse order.
  if (in_array($operation, array('rollback', 'rollback_and_import'))) {
    $machine_names = array_reverse($machine_names);
    foreach ($machine_names as $machine_name) {
      $operations[] = array('migrate_ui_batch', array('rollback', $machine_name, $limit));
    }
    // Reset order of machines names in preparation for final operation.
    $machine_names = array_reverse($machine_names);
    $operation = $operation == 'rollback_and_import' ? 'import' : NULL;
  }

  // Perform non-rollback operation, if one exists.
  if ($operation) {
    foreach ($machine_names as $machine_name) {
      $migration = Migration::getInstance($machine_name);
      // Update (if necessary) once, before starting
      if ($update && method_exists($migration, 'prepareUpdate')) {
        $migration->prepareUpdate();
      }
      $operations[] = array('migrate_ui_batch', array($operation, $machine_name, $limit));
    }
  }

  $batch = array(
    'operations' => $operations,
    'title' => t('Migration processing'),
    'file' => drupal_get_path('module', 'migrate_ui') . '/migrate_ui.pages.inc',
    'init_message' => t('Starting migration process'),
    'progress_message' => t(''),
    'error_message' => t('An error occurred. Some or all of the migrate processing has failed.'),
    'finished' => 'migrate_ui_batch_finish',
  );
  batch_set($batch);
}

/**
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
function migrate_ui_batch($operation, $machine_name, $limit, &$context) {
  // If we got a stop message, skip everything else
  if (isset($context['results']['stopped'])) {
    $context['finished'] = 1;
    return;
  }

  $migration = Migration::getInstance($machine_name);
  
  // Messages generated by migration processes will be captured in this global
  global $_migrate_messages;
  $_migrate_messages = array();
  Migration::setDisplayFunction('migrate_ui_capture_message');

  // Perform the requested operation
  switch ($operation) {
    case 'import':
      $result = $migration->processImport(array('limit' => $limit));
      break;
    case 'rollback':
      $result = $migration->processRollback(array('limit' => $limit));
      break;
    case 'stop':
      $migration->stopProcess();
      $result = Migration::RESULT_COMPLETED;
      break;
    case 'reset':
      $migration->resetStatus();
      $result = Migration::RESULT_COMPLETED;
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
              $context['finished'] = ($processed-$to_update)/$total;
              break;
            case 'rollback':
              $context['finished'] = ($total-$processed)/$total;
              break;
          }
        }
      }
      break;
    case MigrationBase::RESULT_SKIPPED:
      $_migrate_messages[] = t("Skipped !name due to unfulfilled dependencies: !depends",
        array(
          '!name' => $machine_name,
          '!depends' => implode(", ", $migration->incompleteDependencies()),
        ));
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
  foreach ($_migrate_messages as $message) {
    $context['results'][] = $message;
  }

  // While in progress, show the cumulative list of messages
  $full_message = '';
  foreach ($context['results'] as $message) {
    $full_message .= $message . '<br />';
  }
  $context['message'] = $full_message;
}

/**
 * Batch API finished callback - report results
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
    drupal_set_message($result);
  }
}

function migrate_ui_capture_message($message, $level) {
  if ($level != 'debug') {
    global $_migrate_messages;
    $_migrate_messages[] = $message;
  }
}

/**
 * Menu callback for messages page
 */
function migrate_ui_messages($migration) {
  $build = $rows = array();

  $header = array(t('Source ID'), t('Level'), t('Message'));

  if (is_string($migration)) {
    $migration = migration_load($migration);
  }

  // TODO: need a general MigrateMap API
  $messages = $migration->getMap()->getConnection()
              ->select($migration->getMap()->getMessageTable(), 'msg')
              ->extend('PagerDefault')
              ->limit(500)
              ->fields('msg')
              ->execute();

  foreach ($messages as $message) {
    $classes[] = $message->level <= MigrationBase::MESSAGE_WARNING ? 'migrate-error' : '';
    $rows[] = array(
      array('data' => $message->sourceid1, 'class' => $classes), // TODO: deal with compound keys
      array('data' => $migration->getMessageLevelName($message->level), 'class' => $classes),
      array('data' => $message->message, 'class' => $classes),
    );
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
 * Menu callback function for migration view page.
 */
function migrate_migration_info($form, $form_state, $migration) {
  if (is_string($migration)) {
    $migration = migration_load($migration);
  }

  $has_mappings = method_exists($migration, 'getFieldMappings');
  $form = array();

  if ($has_mappings) {
    $field_mappings = $migration->getFieldMappings();
    // Identify what destination and source fields are mapped
    foreach ($field_mappings as $mapping) {
      $source_field = $mapping->getSourceField();
      $destination_field = $mapping->getDestinationField();
      $sourceFields[$source_field] = $source_field;
      $destinationFields[$destination_field] = $destination_field;
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

  $team = array();
  foreach ($migration->getTeam() as $member) {
    $email_address = $member->getEmailAddress();
    $team[$member->getGroup()][] =
      $member->getName() . ' <' . l($email_address, 'mailto:' .$email_address) . '>';
  }

  foreach ($team as $group => $list) {
    $form['overview'][$group] = array(
      '#type' => 'item',
      '#title' => $group,
      '#markup' => implode(', ', $list),
    );
  }

  $dependencies = $migration->getHardDependencies();
  if (count($dependencies) > 0) {
    $form['overview']['dependencies'] = array(
      '#title' => t('Dependencies') ,
      '#markup' => implode(', ', $dependencies),
      '#type' => 'item',
    );
  }
  $soft_dependencies = $migration->getSoftDependencies();
  if (count($soft_dependencies) > 0) {
    $form['overview']['soft_dependencies'] = array(
      '#title' => t('Soft Dependencies'),
      '#markup' => implode(', ', $soft_dependencies),
      '#type' => 'item',
    );
  }

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
      '#title' => t('System of record: '),
      '#markup' => $system_of_record,
    );
  }

  $form['overview']['description'] = array(
    '#title' => t('Description: '),
    '#markup' => $migration->getDescription(),
    '#type' => 'item',
  );

  if ($has_mappings) {
    // Destination field information
    $form['destination'] = array(
      '#type' => 'fieldset',
      '#title' => t('Destination'),
      '#group' => 'detail',
      '#description' =>
        t('<p>These are the fields available in the destination of this
           migration. The machine names listed here are those available to be used
           as the first parameter to $this->addFieldMapping() in your Migration
           class constructor. <span class="error">Unmapped fields are red</span>.</p>'),
    );
    $destination = $migration->getDestination();
    $form['destination']['type'] = array(
      '#type' => 'item',
      '#title' => t('Type'),
      '#markup' => (string)$destination,
    );
    $destKey = $destination->getKeySchema();
    $header = array(t('Machine name'), t('Description'));
    $rows = array();
    foreach ($destination->fields() as $machine_name => $description) {
      $classes = array();
      if (isset($destKey[$machine_name])) {
        // Identify primary key
        $machine_name .= t(' (PK)');
      }
      else {
        // Add class for mapped/unmapped. Used in summary.
        $classes[] = !isset($destinationFields[$machine_name]) ? 'migrate-error' : '';
      }
      $rows[] = array(array('data' => $machine_name, 'class' => $classes), array('data' => $description, 'class' => $classes));
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
        t('<p>These are the fields available from the source of this
           migration. The machine names listed here are those available to be used
           as the second parameter to $this->addFieldMapping() in your Migration
           class constructor. <span class="error">Unmapped fields are red</span>.</p>'),
    );
    $source = $migration->getSource();
    $form['source']['query'] = array(
      '#type' => 'item',
      '#title' => t('Query'),
      '#markup' => '<pre>' . $source . '</pre>',
    );
    $sourceKey = $migration->getMap()->getSourceKey();
    $header = array(t('Machine name'), t('Description'));
    $rows = array();
    foreach ($source->fields() as $machine_name => $description) {
      if (isset($sourceKey[$machine_name])) {
        // Identify primary key
        $machine_name .= t(' (PK)');
      }
      else {
        // Add class for mapped/unmapped. Used in summary.
        $classes = !isset($sourceFields[$machine_name]) ? 'migrate-error' : '';
      }
      $rows[] = array(array('data' => $machine_name, 'class' => $classes), array('data' => $description, 'class' => $classes));
    }
    $classes = array();

    $form['source']['fields'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('No fields'),
    );

    $header = array(t('Destination'), t('Source'), t('Default'), t('Description'), t('Priority'));

    // First group the mappings
    $descriptions = array();
    $source_fields = $source->fields();
    $destination_fields = $destination->fields();

    foreach ($field_mappings as $mapping) {
      // Validate source and destination fields actually exist
      $source_field = $mapping->getSourceField();
      $destination_field = $mapping->getDestinationField();
      if (!is_null($source_field) && !isset($source_fields[$source_field])) {
        drupal_set_message(t('"!source" was used as source field in the
          "!destination" mapping but is not in list of source fields', array(
            '!source' => $source_field,
            '!destination' => $destination_field
          )),
        'warning');
      }
      if (!is_null($destination_field) && !isset($destination_fields[$destination_field])) {
        drupal_set_message(t('"!destination" was used as destination field in
          "!source" mapping but is not in list of destination fields', array(
            '!source' => $source_field,
            '!destination' => $destination_field)),
        'warning');
      }
      $descriptions[$mapping->getIssueGroup()][] = $mapping;
    }

    // Put out each group header
    $rows = array();
    $count = 0;
    foreach ($descriptions as $group => $mappings) {
      $form[$group] = array(
        '#type' => 'fieldset',
        '#title' => t('Mapping: ') . $group,
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
          $classes[] = 'migrate-priority-' . $issue_priority;
          $priority = MigrateFieldMapping::$priorities[$issue_priority];
          $issue_pattern = $migration->getIssuePattern();
          $issue_number = $mapping->getIssueNumber();
          if (!is_null($issue_pattern) && !is_null($issue_number)) {
            $priority .= ' (' . l('#' . $issue_number, str_replace(':id:', $issue_number,
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
        $row = array(
          array('data' => $mapping->getDestinationField(), 'class' => $classes),
          array('data' => $mapping->getSourceField(), 'class' => $classes),
          array('data' => $default, 'class' => $classes),
          array('data' => $mapping->getDescription(),  'class' => $classes),
          array('data' => $priority, 'class' => $classes),
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
 * Menu callback
 */
function migrate_ui_configure() {
  drupal_set_title(t('Migrate configuration'));
  return drupal_get_form('migrate_ui_configure_form');
}

/**
 * Form for reviewing migrations.
 */
function migrate_ui_configure_form($form, &$form_state) {
  $build = array();
  $build['description'] = array(
    '#prefix' => '<div>',
    '#markup' => t('In some cases, such as when a handler for a contributed module is
      implemented in both migrate_extras and the module itself, you may need to disable
      a particular handler. In this case, you may uncheck the undesired handler below.'),
    '#suffix' => '</div>',
  );

  $build['destination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination handlers'),
    '#collapsible' => TRUE,
  );

  $header = array(
    'module' => array('data' => t('Module')),
    'class' => array('data' => t('Class')),
    'types' => array('data' => t('Destination types handled')),
  );

  $disabled = unserialize(variable_get('migrate_disabled_handlers', serialize(array())));
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
    $row['module'] = $module;
    $row['class'] = $class_name;
    $row['types'] = implode(', ', $handler->getTypesHandled());
    $default_values[$class_name] = !in_array($class_name, $disabled);
    $rows[$class_name] = $row;
  }
  $build['destination']['destination_handlers'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => $default_values,
    '#empty' => t('No destination handlers found'),
  );

  $build['field'] = array(
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
    $row['module'] = $module;
    $row['class'] = $class_name;
    $row['types'] = implode(', ', $handler->getTypesHandled());
    $default_values[$class_name] = !in_array($class_name, $disabled);
    $rows[$class_name] = $row;
  }
  $build['field']['field_handlers'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#default_value' => $default_values,
    '#empty' => t('No field handlers found'),
  );

  $build['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save handler statuses'),
    '#submit' => array('migrate_ui_configure_submit'),
  );

  return $build;
}

/**
 * Submit callback for the dashboard form.
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
    drupal_set_message(t('The following handler classes are disabled: !classes',
      array('!classes' => implode(', ', $disabled))));
  }
  else {
    drupal_set_message(t('No handler classes are currently disabled.'));
  }
}
