<?php

/**
 * @file
 * Menu callbacks, form callbacks and helpers.
 */

/**
 * Render a page of available importers.
 */
function feeds_page() {
  $rows = array();
  if ($importers = feeds_importer_load_all()) {
    foreach ($importers as $importer) {
      if ($importer->disabled) {
        continue;
      }
      if (!(user_access('import ' . $importer->id . ' feeds') || user_access('administer feeds'))) {
        continue;
      }
      if (empty($importer->config['content_type'])) {
        $link = 'import/' . $importer->id;
        $title = $importer->config['name'];
      }
      elseif (node_access('create', $importer->config['content_type'])) {
        $link = 'node/add/' . str_replace('_', '-', $importer->config['content_type']);
        $title = node_type_get_name($importer->config['content_type']);
      }
      else {
        continue;
      }
      $rows[] = array(
        l($title, $link),
        check_plain($importer->config['description']),
      );
    }
  }
  if (empty($rows)) {
    // The feeds_ui module is enabled.
    if (module_exists('feeds_ui') && user_access('administer feeds')) {
      drupal_set_message(t('There are no importers, go to <a href="@importers">Feed importers</a> to create one or enable an existing one.', array('@importers' => url('admin/structure/feeds'))));
    }
    else {
      // The feeds_ui module is not enabled but the current user has access to
      // Modules to enable it.
      if (user_access('administer modules')) {
        drupal_set_message(t('The Feeds UI Admin module is not enabled and there are no importers, go to <a href="@modules">Modules</a> and enable Feeds Admin UI. Then go to <a href="@importers">Feed importers</a> to create one or enable an existing one.', array('@modules' => url('admin/modules'), '@importers' => url('admin/structure/feeds'))));
      }
      else {
        // The feeds_ui module is not enabled and the current user cannot
        // enable it.
        drupal_set_message(t("The Feeds UI Admin module is not enabled. Please contact the Administrator for your site and ask them to enable it."));
      }
    }
  }
  $header = array(
    t('Import'),
    t('Description'),
  );
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Render a feeds import form on import/[config] pages.
 */
function feeds_import_form(array $form, array &$form_state, FeedsImporter $importer) {
  $source = feeds_source($importer->id);

  $form['#importer_id'] = $importer->id;
  // @todo Move this into fetcher?
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['source_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
    '#tree' => TRUE,
    '#value' => feeds_source_status($source),
  );

  $source_form = $source->configForm($form_state);
  if (!empty($source_form)) {
    $form['feeds'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import'),
      '#tree' => TRUE,
    ) + $source_form;
  }

  // Set submit button label based on settings.
  if ($source->importer->config['import_on_create']) {
    $submit = t('Import');
    if ($source->importer->config['process_in_background']) {
      // When processing the import in background, the import job is put in the
      // queue.
      $submit = t('Schedule import');
    }
  }
  elseif ($source->importer->config['import_period'] != FEEDS_SCHEDULE_NEVER) {
    // The import would be scheduled according to the periodic import setting.
    $submit = t('Schedule import');
  }
  else {
    drupal_set_message(t('For this importer both "@import_period" and "@import_on_create" are turned off. It is possible that Feeds will not import the provided source.', array(
      '@import_period' => t('Periodic import'),
      '@import_on_create' => t('Import on submission'),
    )), 'warning', FALSE);
    $submit = t('Save');
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit,
  );

  // Disable submit button if import is initiated.
  $progress = $source->progressImporting();
  if ($progress !== FEEDS_BATCH_COMPLETE) {
    $form['actions']['submit']['#disabled'] = TRUE;
    $form['actions']['submit']['#value'] = t('Importing (@progress %)', array(
      '@progress' => number_format(100 * $progress, 0),
    ));

    // Check if import task is queued.
    if ($source->isQueued()) {
      $form['source_status']['#value'] .= t('Run cron to continue the import.');
    }
  }

  return $form;
}

/**
 * Validation handler for node forms and feeds_import_form().
 */
function feeds_import_form_validate($form, &$form_state) {
  // @todo This may be a problem here, as we don't have a feed_nid at this point.
  feeds_source($form['#importer_id'])->configFormValidate($form_state['values']['feeds']);
}

/**
 * Submit handler for feeds_import_form().
 */
function feeds_import_form_submit($form, &$form_state) {
  // Save source and import.
  $source = feeds_source($form['#importer_id']);

  if (!empty($form_state['values']['feeds']) && is_array($form_state['values']['feeds'])) {
    $source->addConfig($form_state['values']['feeds']);
    $source->save();
  }

  // Refresh feed if import on create is selected.
  if ($source->importer->config['import_on_create']) {
    $source->startImport();
    if ($source->importer->config['process_in_background']) {
      drupal_set_message(t('Import scheduled.'));
    }
  }

  // Add to schedule, make sure importer is scheduled, too.
  $source->ensureSchedule();

  // If an import is only triggered by periodic import, check if it is about to
  // be rescheduled so there is at least a message.
  if (!$source->importer->config['import_on_create']) {
    // Check if the importer is about to be rescheduled.
    $importers = feeds_reschedule();
    if (isset($importers[$form['#importer_id']])) {
      drupal_set_message(t('Rescheduling the import will happen on the next cron run.'), 'status');
    }
  }
}

/**
 * Render a feeds import form on node/id/import pages.
 */
function feeds_import_tab_form($form, &$form_state, $node) {
  $importer_id = feeds_get_importer_id($node->type);
  $source = feeds_source($importer_id, $node->nid);

  $form = array();
  $form['#feed_nid'] = $node->nid;
  $form['#importer_id'] = $importer_id;
  $form['#redirect'] = 'node/' . $node->nid;
  $form['source_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
    '#tree' => TRUE,
    '#value' => feeds_source_status($source),
  );
  $form = confirm_form($form, t('Import all content from source?'), 'node/' . $node->nid, '', t('Import'), t('Cancel'), 'confirm feeds update');

  // Change submit button label if processing in background.
  if ($source->importer->config['process_in_background']) {
    $form['actions']['submit']['#value'] = t('Schedule import');
  }

  // Disable submit button if import is initiated.
  $progress = $source->progressImporting();
  if ($progress !== FEEDS_BATCH_COMPLETE) {
    $form['actions']['submit']['#disabled'] = TRUE;
    $form['actions']['submit']['#value'] = t('Importing (@progress %)', array(
      '@progress' => number_format(100 * $progress, 0),
    ));

    // Check if import task is queued.
    if ($source->isQueued()) {
      $form['source_status']['#value'] .= t('Run cron to continue the import.');
    }
  }

  return $form;
}

/**
 * Submit handler for feeds_import_tab_form().
 */
function feeds_import_tab_form_submit($form, &$form_state) {
  $form_state['redirect'] = $form['#redirect'];
  $source = feeds_source($form['#importer_id'], $form['#feed_nid']);
  $source->startImport();
  $source->ensureSchedule();

  if ($source->importer->config['process_in_background']) {
    drupal_set_message(t('Import scheduled.'));
  }
}

/**
 * Render a feeds delete form.
 *
 * Used on both node pages and configuration pages.
 * Therefore $node may be missing.
 */
function feeds_delete_tab_form(array $form, array &$form_state, FeedsImporter $importer = NULL, $node = NULL) {
  if (empty($node)) {
    $source = feeds_source($importer->id);
    $form['#redirect'] = 'import/' . $source->id;
  }
  else {
    $importer_id = feeds_get_importer_id($node->type);
    $source = feeds_source($importer_id, $node->nid);
    $form['#redirect'] = 'node/' . $source->feed_nid;
  }
  // Form cannot pass on source object.
  $form['#importer_id'] = $source->id;
  $form['#feed_nid'] = $source->feed_nid;
  $form['source_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
    '#tree' => TRUE,
    '#value' => feeds_source_status($source),
  );
  $form = confirm_form($form, t('Delete all items from source?'), $form['#redirect'], '', t('Delete'), t('Cancel'), 'confirm feeds update');

  // Change submit button label if processing in background.
  if ($source->importer->config['process_in_background']) {
    $form['actions']['submit']['#value'] = t('Schedule delete');
  }

  // Disable submit button if clearing is initiated.
  $progress = $source->progressClearing();
  if ($progress !== FEEDS_BATCH_COMPLETE) {
    $form['actions']['submit']['#disabled'] = TRUE;
    $form['actions']['submit']['#value'] = t('Deleting (@progress %)', array(
      '@progress' => number_format(100 * $progress, 0),
    ));
    $form['source_status']['#value'] .= t('Run cron to continue the deletion of items.');
  }

  return $form;
}

/**
 * Submit handler for feeds_delete_tab_form().
 */
function feeds_delete_tab_form_submit($form, &$form_state) {
  $form_state['redirect'] = $form['#redirect'];
  $feed_nid = empty($form['#feed_nid']) ? 0 : $form['#feed_nid'];
  $source = feeds_source($form['#importer_id'], $feed_nid);
  $source->startClear();
  $source->ensureSchedule();

  if ($source->importer->config['process_in_background']) {
    drupal_set_message(t('Deletion of items scheduled.'));
  }
}

/**
 * Render a feeds unlock form.
 *
 * Used on both node pages and configuration pages.
 * Therefore $node may be missing.
 */
function feeds_unlock_tab_form($form, &$form_state, FeedsImporter $importer = NULL, $node = NULL) {
  if (empty($node)) {
    $source = feeds_source($importer->id);
    $form['#redirect'] = 'import/' . $source->id;
  }
  else {
    $importer_id = feeds_get_importer_id($node->type);
    $source = feeds_source($importer_id, $node->nid);
    $form['#redirect'] = 'node/' . $source->feed_nid;
  }
  // Form cannot pass on source object.
  $form['#importer_id'] = $source->id;
  $form['#feed_nid'] = $source->feed_nid;
  $form['source_status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
    '#tree' => TRUE,
    '#value' => feeds_source_status($source),
  );
  $form = confirm_form($form, t('Unlock this importer?'), $form['#redirect'], '', t('Delete'), t('Cancel'), 'confirm feeds update');
  if ($source->progressImporting() == FEEDS_BATCH_COMPLETE && $source->progressClearing() == FEEDS_BATCH_COMPLETE) {
    $form['source_locked'] = array(
      '#type' => 'markup',
      '#title' => t('Not Locked'),
      '#tree' => TRUE,
      '#markup' => t('This importer is not locked, therefore it cannot be unlocked.'),
    );
    $form['actions']['submit']['#disabled'] = TRUE;
    $form['actions']['submit']['#value'] = t('Unlock (disabled)');
  }
  else {
    $form['actions']['submit']['#value'] = t('Unlock');
  }
  return $form;
}

/**
 * Form submit handler. Resets all feeds state.
 */
function feeds_unlock_tab_form_submit($form, &$form_state) {
  $form_state['redirect'] = $form['#redirect'];
  $feed_nid = empty($form['#feed_nid']) ? 0 : $form['#feed_nid'];
  $importer_id = $form['#importer_id'];

  feeds_source($importer_id, $feed_nid)
    ->unlock();
  drupal_set_message(t('Importer unlocked.'));
}

/**
 * Handle a fetcher callback.
 */
function feeds_fetcher_callback($importer, $feed_nid = 0) {
  if ($importer instanceof FeedsImporter) {
    try {
      return $importer->fetcher->request($feed_nid);
    }
    catch (Exception $e) {
      // Do nothing.
    }
  }
  drupal_access_denied();
}

/**
 * Template generation.
 */
function feeds_importer_template(FeedsImporter $importer) {
  if ($importer->parser instanceof FeedsCSVParser) {
    return $importer->parser->getTemplate();
  }
  return drupal_not_found();
}

/**
 * Renders a status display for a source.
 */
function feeds_source_status($source) {
  $progress_importing = $source->progressImporting();
  $v = array();
  if ($progress_importing != FEEDS_BATCH_COMPLETE) {
    $v['progress_importing'] = $progress_importing;
  }
  $progress_clearing = $source->progressClearing();
  if ($progress_clearing != FEEDS_BATCH_COMPLETE) {
    $v['progress_clearing'] = $progress_clearing;
  }
  $v['imported'] = $source->imported;
  $v['count'] = $source->itemCount();
  $v['next'] = $source->getNextImportTimeDetails();
  if (!empty($v)) {
    return theme('feeds_source_status', $v);
  }
}

/**
 * Themes a status display for a source.
 */
function theme_feeds_source_status($v) {
  $output = '<div class="info-box feeds-source-status">';
  $items = array();
  if ($v['progress_importing']) {
    $progress = number_format(100.0 * $v['progress_importing'], 0);
    $items[] = t('Importing - @progress % complete.', array(
      '@progress' => $progress,
    ));
  }
  if ($v['progress_clearing']) {
    $progress = number_format(100.0 * $v['progress_clearing'], 0);
    $items[] = t('Deleting items - @progress % complete.', array(
      '@progress' => $progress,
    ));
  }
  if (!count($items)) {
    if ($v['count']) {
      if ($v['imported']) {
        $items[] = t('Last import: @ago ago.', array(
          '@ago' => format_interval(REQUEST_TIME - $v['imported'], 1),
        ));
      }
      $items[] = t('@count imported items total.', array(
        '@count' => $v['count'],
      ));
    }
    else {
      $items[] = t('No imported items.');
    }
  }

  if ($v['next']) {
    // Check if medium date format contains hours/minutes.
    $date_format = variable_get('date_format_medium');
    $use_custom_date_format = $date_format && !strpos($date_format, 'H:i');

    if (!empty($v['next']['message'])) {
      $items[] = t('Next import: @message.', array(
        '@message' => $v['next']['message'],
      ));
    }
    elseif ($v['next']['time'] > REQUEST_TIME) {
      $items[] = t('Next import: @date (via @method)', array(
        '@date' => $use_custom_date_format ? format_date($v['next']['time'], 'custom', 'Y/m/d H:i:s') : format_date($v['next']['time']),
        '@method' => $v['next']['method'],
      ));
    }
    else {
      $items[] = t('Next import: on next cron run (via @method).', array(
        '@method' => $v['next']['method'],
      ));
    }
  }
  else {
    $items[] = t('Next import: not scheduled.');
  }
  $output .= theme('item_list', array('items' => $items));
  $output .= '</div>';
  return $output;
}

/**
 * Theme upload widget.
 */
function theme_feeds_upload($variables) {
  $element = $variables['element'];
  drupal_add_css(drupal_get_path('module', 'feeds') . '/feeds.css');
  _form_set_class($element, array('form-file'));
  $summary = '';
  if (!empty($element['#file_info'])) {
    $file = $element['#file_info'];
    $wrapper = file_stream_wrapper_get_instance_by_uri($file->uri);
    $summary .= '<div class="feeds-file-info">';
    $summary .= '<div class="feeds-file-name">';
    if ($wrapper) {
      $summary .= l($file->filename, $wrapper->getExternalUrl());
    }
    else {
      $summary .= t('URI scheme %scheme not available.', array(
        '%scheme' => file_uri_scheme($uri),
      ));
    }
    $summary .= '</div>';
    $summary .= '<div class="file-size">';
    $summary .= format_size($file->filesize);
    $summary .= '</div>';
    $summary .= '<div class="feeds-file-mime">';
    $summary .= check_plain($file->filemime);
    $summary .= '</div>';
    $summary .= '</div>';
  }
  // Prepend the summary to the form field.
  $element['#children'] = '<div class="feeds-file">' . $summary . '<div class="feeds-file-upload">' . $element['#children'];
  // Render file upload field using theme_form_element().
  $output = theme('form_element', $element);
  // Close "feeds-file" and "feeds-file-upload" divs.
  $output .= '</div></div>';

  return $output;
}
