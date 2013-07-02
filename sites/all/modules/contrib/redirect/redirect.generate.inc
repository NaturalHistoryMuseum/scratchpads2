<?php

/**
 * @file
 * Devel generate integration for the redirect module.
 */

function redirect_generate_form() {
  $form['count'] = array(
    '#type' => 'textfield',
    '#title' => t('How many URL redirects would you like to generate?'),
    '#default_value' => 50,
    '#size' => 4,
  );
  $form['delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete all URL redirects before generating new URL redirects.'),
    '#default_value' => FALSE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate'),
  );

  return $form;
}

function redirect_generate_form_submit(&$form, &$form_state) {
  // Run the batch.
  $batch = redirect_generate_redirects_batch_info($form_state['values']['count'], $form_state['values']['delete']);
  batch_set($batch);
}

function redirect_generate_redirects_batch_info($count, $delete = FALSE) {
  if ($delete) {
    $operations[] = array('redirect_generate_batch_delete', array());
  }

  $operations[] = array('redirect_generate_batch_generate', array($count));

  return array(
    'operations' => $operations,
    'finished' => 'redirect_generate_batch_finished',
    'file' => drupal_get_path('module', 'redirect') . '/redirect.generate.inc',
  );
}

function redirect_generate_batch_delete(array &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_rid'] = 0;
    $context['sandbox']['max'] = db_query('SELECT COUNT(DISTINCT rid) FROM {redirect}')->fetchField();
  }

  $limit = 20;
  $rids = db_query_range("SELECT rid FROM {redirect} WHERE rid > :rid ORDER BY rid", 0, $limit, array(':rid' => $context['sandbox']['current_rid']))->fetchCol();
  redirect_delete_multiple($rids);

  // Update our progress information.
  $context['sandbox']['progress'] += count($rids);
  $context['sandbox']['current_rid'] = end($rids);
  $context['message'] = t('Deleted URL redirect @rid.', array('@rid' => end($rids)));

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = ($context['sandbox']['progress'] >= $context['sandbox']['max']);
  }
}

function redirect_generate_batch_generate($num, array &$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $num;

    $query = db_select('node', 'n');
    $query->addField('n', 'nid');
    $query->condition('n.status', NODE_PUBLISHED);
    $query->addTag('node_access');
    $context['sandbox']['nids'] = $query->execute()->fetchAllKeyed(0, 0);
  }

  module_load_include('inc', 'devel_generate');

  $limit = 20;
  $types = array_keys(redirect_status_code_options());
  $languages = module_exists('locale') ? array_keys(locale_language_list('name')) : array();

  for ($i = 0; $i < min($limit, $context['sandbox']['max'] - $context['sandbox']['progress']); $i++) {
    $rand = mt_rand(0, 100);

    $redirect = new stdClass();
    redirect_object_prepare($redirect);
    $redirect->source = _redirect_generate_url();
    $redirect->devel_generate = TRUE;

    if ($context['sandbox']['nids'] && $rand >= 40) {
      $redirect->redirect = 'node/'. array_rand($context['sandbox']['nids']);
    }
    else {
      $redirect->redirect = _redirect_generate_url(TRUE);
      if ($rand <= 20) {
        $redirect->redirect_options['query'] = _redirect_generate_querystring();
      }
      if ($rand <= 5) {
        $redirect->redirect_options['fragment'] = devel_generate_word(mt_rand(4, 8));
      }
    }

    if ($rand <= 20) {
      $redirect->status_code = $types[array_rand($types)];
    }

    if ($languages && $rand <= 20) {
      $redirect->language = $languages[array_rand($languages)];
    }

    if ($rand <= 30) {
      $redirect->source_options['query'] = _redirect_generate_querystring();
    }

    redirect_save($redirect);

    if (mt_rand(0, 1)) {
      db_update('redirect')
        ->fields(array(
          'count' => mt_rand(1, 500),
          'access' => mt_rand(REQUEST_TIME - 31536000, REQUEST_TIME),
        ))
        ->condition('rid', $redirect->rid)
        ->execute();
    }

    $context['results'][] = $redirect->rid;
  }

  // Update our progress information.
  $context['sandbox']['progress'] += $limit;
  //$context['message'] = t('Deleted URL redirect @rid.', array('@rid' => end($rids)));

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = ($context['sandbox']['progress'] >= $context['sandbox']['max']);
  }
}

function redirect_generate_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(format_plural(count($results), 'One URL redirect created.', '@count URL redirects created.'));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE))));
  }
}

function _redirect_generate_url($external = FALSE, $max_levels = 2) {
  module_load_include('inc', 'devel_generate');

  $url = array();
  if ($external) {
    $tlds = array('com', 'net', 'org');
    $url[] = 'http://www.example.'. $tlds[array_rand($tlds)];
  }
  $max_levels = mt_rand($external ? 0 : 1, $max_levels);
  for ($i = 1; $i <= $max_levels; $i++) {
    $url[] = devel_generate_word(mt_rand(6 / $i, 8));
  }
  return implode('/', $url);
}

function _redirect_generate_querystring() {
  module_load_include('inc', 'devel_generate');

  $query = array(devel_generate_word(mt_rand(1, 3)) => devel_generate_word(mt_rand(2, 4)));
  return $query;
}
