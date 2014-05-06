<?php

/**
 * @file
 * UI pages for revisions, similar with pages from node.pages.inc.
 */

/**
 * Generate an overview table of older revisions of a taxonomy term.
 *
 * @param $viewed_term
 *  The term for which the revisions are listed.
 */
function taxonomy_revision_overview($viewed_term) {
  if (!module_exists('diff')) {
    return _taxonomy_revision_taxonomy_revisions_simple_form($viewed_term);
  }
  else {
    return drupal_get_form('taxonomy_revision_taxonomy_revisions', $viewed_term);
  }
}

/**
 * Form builder for taxonomy term revision revert form.
 *
 * @ingroup forms
 * @see taxonomy_revision_revert_confirm_submit()
 */
function taxonomy_revision_revert_confirm($form, $form_state, $term_revision) {
  $form['#term_revision'] = $term_revision;
  return confirm_form($form, t('Are you sure you want to revert to the revision from %revision-date?', array('%revision-date' => format_date($term_revision->timestamp))), 'taxonomy/term/' . $term_revision->tid . '/revisions', '', t('Revert'), t('Cancel'));
}

/**
 * Submit handler for the taxonomy revision revert form. Reverts taxonomy term
 * to a previous revision.
 *
 * @see taxonomy_revision_revert_confirm()
 */
function taxonomy_revision_revert_confirm_submit($form, &$form_state) {
  $term_revision = $form['#term_revision'];
  $term_revision->revision = 1;
  $term_revision->log = t('Copy of the revision from %date.', array('%date' => format_date($term_revision->timestamp)));
  taxonomy_term_save($term_revision);
  watchdog('taxonomy_revision', 'reverted %title revision %revision.', array('%title' => $term_revision->name, '%revision' => $term_revision->revision_id));
  drupal_set_message(t('%title has been reverted back to the revision from %revision-date.', array('%title' => $term_revision->name, '%revision-date' => format_date($term_revision->timestamp))));
  $form_state['redirect'] = 'taxonomy/term/' . $term_revision->tid . '/revisions';
}

/**
 * Form builder for a revision confirm for term deletion.
 */
function taxonomy_revision_delete_confirm($form, $form_state, $term_revision) {
  $form['#term_revision'] = $term_revision;
  return confirm_form($form, t('Are you sure you want to delete the revision from %revision-date?', array('%revision-date' => format_date($term_revision->timestamp))), 'taxonomy/term/' . $term_revision->tid . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Submit handler for the taxonomy revision delete form. Deletes a taxonomy term
 * revision.
 *
 * @see taxonomy_revision_delete_confirm()
 */
function taxonomy_revision_delete_confirm_submit($form, &$form_state) {
  $term_revision = $form['#term_revision'];
  taxonomy_revision_delete($term_revision);

  watchdog('taxonomy_revision', 'deleted %title revision %revision.', array('%title' => $term_revision->name, '%revision' => $term_revision->revision_id));
  drupal_set_message(t('Revision from %revision-date %title has been deleted.', array('%revision-date' => format_date($term_revision->timestamp), '%title' => $term_revision->name)));
  $form_state['redirect'] = 'taxonomy/term/' . $term_revision->tid;
  if (db_query('SELECT COUNT(revision_id) FROM {taxonomy_term_data_revision} WHERE tid = :tid', array(':tid' => $term_revision->tid))->fetchField() > 1) {
    $form_state['redirect'] .= '/revisions';
  }
}

/**
 * Create a comparison for the term between versions 'old_revision_id' and 'new_revision_id'.
 *
 * @param object $term
 *   Term on which to perform comparison
 * @param integer $old_revision_id
 *   Version ID of the old revision.
 * @param integer $new_revision_id
 *   Version ID of the new revision.
 */
function taxonomy_revision_diffs_show($term, $old_revision_id, $new_revision_id, $state = NULL) {
  // Attaches the CSS.
  $build['#attached'] = diff_build_attachments();

  // The default state is the raw state.
  $default_state = 'raw';
  if (empty($state)) {
    $state = $default_state;
  }
  $state = str_replace('-', '_', $state);
  if (!array_key_exists($state, diff_available_states())) {
    $state = $default_state;
  }

  // Same title as the 'Revisions' tab. Blocked by non-page requests.
  drupal_set_title(t('Revisions for %title', array('%title' => $term->name)), PASS_THROUGH);
  $term_revisions = taxonomy_revision_list($term);

  $old_term = taxonomy_revision_term_load($term->tid, $old_revision_id);
  $new_term = taxonomy_revision_term_load($term->tid, $new_revision_id);

  // Generate table header (date, username, log message).
  $old_header = t('!date by !username', array(
    '!date' => l(format_date($old_term->timestamp), "taxonomy/term/$term->tid/revisions/$old_term->revision_id/view", array('absolute' => 1)),
    '!username' => theme('username', array('account' => $term_revisions[$old_revision_id])),
  ));
  $new_header = t('!date by !username', array(
    '!date' => l(format_date($new_term->timestamp), "taxonomy_term/$term->tid/revisions/$new_term->revision_id/view", array('absolute' => 1)),
    '!username' => theme('username', array('account' => $term_revisions[$new_revision_id])),
  ));

  $old_log = $old_term->log != '' ? '<p class="revision-log">' . filter_xss($old_term->log) . '</p>' : '';
  $new_log = $new_term->log != '' ? '<p class="revision-log">' . filter_xss($new_term->log) . '</p>' : '';

  // Generate previous diff/next diff links.
  $nav_suffix = ($default_state != $state) ? '/' . str_replace('_', '-', $state) : '';
  $next_revision_id = _taxonomy_revision_diff_get_next_revision_id($term_revisions, $new_revision_id);
  if ($next_revision_id) {
    $next_link = l(t('Next difference >'), 'taxonomy/term/' . $term->tid . '/revisions/view/' . $new_revision_id . '/' . $next_revision_id . $nav_suffix, array('absolute' => 1));
  }
  else {
    $next_link = '';
  }
  $prev_revision_id = _taxonomy_revision_diff_get_previous_revision_id($term_revisions, $old_revision_id);
  if ($prev_revision_id) {
    $prev_link = l(t('< Previous difference'), 'taxonomy/term/' . $term->tid . '/revisions/view/' . $prev_revision_id . '/' . $old_revision_id . $nav_suffix, array('absolute' => 1));
  }
  else {
    $prev_link = '';
  }

  $header = _taxonomy_revision_diff_default_header($old_header, $new_header);
  $rows = array();
  if ($old_log || $new_log) {
    $rows['logs'] = array(
      array(
        'data' => $old_log,
        'colspan' => 2,
      ),
      array(
        'data' => $new_log,
        'colspan' => 2,
      ),
    );
  }
  $rows['navigation'] = array(
    array(
      'data' => $prev_link,
      'class' => array('diff-prevlink'),
      'colspan' => 2,
    ),
    array(
      'data' => $next_link,
      'class' => array('diff-nextlink'),
      'colspan' => 2,
    ),
  );

  $links = array();
  foreach (diff_available_states('taxonomy_term') as $alternative_state => $label) {
    if ($alternative_state == $state) {
      // @todo: from the diff module: Should we show both or just alternatives?
    }
    $links[$alternative_state] = array(
      'title' => $label,
      'href' => "taxonomy/term/{$term->tid}/revisions/view/{$old_revision_id}/{$new_revision_id}" . ($alternative_state == $default_state ? '' : '/' . str_replace('_', '-', $alternative_state)),
    );
  }
  if (count($links) > 1) {
    $state_links = theme('links', array(
      'links' => $links,
      'attributes' => array('class' => array('links', 'inline')),
    ));
    $rows['states'] = array(
      array(
        'data' => $state_links,
        'class' => 'diff-links',
        'colspan' => 4,
      ),
    );
  }
  $rows = array_merge($rows, _taxonomy_revision_diff_body_rows($old_term, $new_term, $state));

  $build['diff_table'] = array(
    '#theme' => 'table__diff__standard',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('class' => array('diff')),
    '#colgroups' => _diff_default_cols(),
    '#sticky' => FALSE,
  );

  return $build;
}

/**
 * Helper function to create default 'header' array for diff table.
 */
function _taxonomy_revision_diff_default_header($old_header = '', $new_header = '') {
  return array(
    array(
      'data' => $old_header,
      'colspan' => 2,
    ),
    array(
      'data' => $new_header,
      'colspan' => 2,
    ),
  );
}

/**
 * Get the entry in the revisions list after $revision_id.
 *
 * @param array $term_revisions
 *   Array of term revision IDs in descending order.
 * @param int $revision_id
 *   Version ID to look for.
 *
 * @return boolean|integer
 *   Returns FALSE if $revision_id is the last entry.
 */
function _taxonomy_revision_diff_get_next_revision_id($term_revisions, $revision_id) {
  $previous = NULL;
  foreach ($term_revisions as $revision) {
    if ($revision->revision_id == $revision_id) {
      return ($previous ? $previous->revision_id : FALSE);
    }
    $previous = $revision;
  }
  return FALSE;
}

/**
 * Get the entry in the revision list before $revision_id.
 *
 * @param array $term_revisions
 *   Array of term revision IDs in descending order.
 * @param integer $revision_id
 *   Version ID to look for.
 *
 * @return boolean|integer
 *   Returns FALSE if $revision_id is the first entry.
 */
function _taxonomy_revision_diff_get_previous_revision_id($term_revisions, $revision_id) {
  $previous = NULL;
  foreach ($term_revisions as $revision) {
    if ($previous && $previous->revision_id == $revision_id) {
      return $revision->revision_id;
    }
    $previous = $revision;
  }
  return FALSE;
}

/**
 * Creates an array of rows which represent the difference between terms.
 *
 * @param object $old_term
 *   Term for comparison which will be displayed on the left side.
 * @param object $new_term
 *   Term for comparison which will be displayed on the right side.
 * @param boolean $state
 *   The state to render for the diff.
 */
function _taxonomy_revision_diff_body_rows($old_term, $new_term, $state = 'raw') {
  // This is an unique index only, so no need for drupal_static().
  static $table_row_counter = 0;

  if ($theme = variable_get('diff_theme', 'default')) {
    drupal_add_css(drupal_get_path('module', 'diff') . "/css/diff.{$theme}.css");
  }
  module_load_include('inc', 'diff', 'includes/taxonomy');

  $rows = array();
  $any_visible_change = FALSE;
  $context = array(
    'entity_type' => 'taxonomy_term',
    'states' => array($state),
    'view_mode' => 'diff_standard',
  );

  module_load_include('inc', 'diff', 'diff.pages');
  $term_diffs = diff_compare_entities($old_term, $new_term, $context);

  // Track line numbers between multiple diffs.
  $line_stats = array(
    'counter' => array('x' => 0, 'y' => 0),
    'offset' => array('x' => 0, 'y' => 0),
  );

  // Render diffs for each.
  foreach ($term_diffs as $term_diff) {
    $show_header = !empty($term_diff['#name']);
    // These are field level settings.
    if ($show_header && isset($term_diff['#settings']['show_header'])) {
      $show_header = $show_header && $term_diff['#settings']['show_header'];
    }

    // Line counting and line header options.
    if (empty($term_diff['#settings']['line_counter'])) {
      $line_counter = FALSE;
    }
    else {
      $line_counter = $term_diff['#settings']['line_counter'];
    }
    // Every call to 'line' resets the counters.
    if ($line_counter) {
      $line_stats['counter']['x'] = 0;
      $line_stats['counter']['y'] = 0;
      if ($line_counter == 'line' && 0) {
        $line_stats['offset']['x'] = 0;
        $line_stats['offset']['y'] = 0;
      }
      $line_stats_ref = $line_stats;
    }
    else {
      $line_stats_ref = NULL;
    }

    list($old, $new) = diff_extract_state($term_diff, $state);
    if ($term_diff_rows = diff_get_rows($old, $new, $line_counter && $line_counter != 'hidden', $line_stats_ref)) {
      if ($line_counter && $line_counter != 'line') {
        $line_stats['offset']['x'] += $line_stats_ref['counter']['x'];
        $line_stats['offset']['y'] += $line_stats_ref['counter']['y'];
      }
      if ($show_header) {
        $rows['diff-header-' . $table_row_counter++] = array(
          array(
            'data' => t('Changes to %name', array('%name' => $term_diff['#name'])),
            'class' => 'diff-section-title',
            'colspan' => 4,
          ),
        );
      }
      // To avoid passing counter to the Diff engine, index rows manually here
      // to allow modules to interact with the table. i.e. no array_merge().
      foreach ($term_diff_rows as $row) {
        $rows['diff-row-' . $table_row_counter++] = $row;
      }
      $any_visible_change = TRUE;
    }
  }
  if (!$any_visible_change) {
    $rows['diff-empty-' . $table_row_counter++] = array(
      array(
        'data' => t('No visible changes'),
        'class' => 'diff-section-title',
        'colspan' => 4,
      ),
    );
    // @todo: revise this.
    // Needed to keep safari happy.
    $rows['diff-empty-' . $table_row_counter++] = array(
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
      array('data' => ''),
    );
  }

  return $rows;
}

/**
 * Private function which returns a simple form when diff module is not active.
 *
 * @param $viewed_term
 * @return mixed
 */

function _taxonomy_revision_taxonomy_revisions_simple_form($viewed_term) {
  drupal_set_title(t('Revisions for %title', array('%title' => $viewed_term->name)), PASS_THROUGH);

  $header = array('revision' => t('Revision'), 'operations' => array('data' => t('Operations'), 'colspan' => 2));

  $revisions = taxonomy_revision_list($viewed_term);
  $rows = array();

  $revert_permission = FALSE;
  $delete_permission = FALSE;
  if (user_access('revert taxonomy term revisions') || user_access('administer taxonomy')) {
    $revert_permission = TRUE;
  }
  if (user_access('delete taxonomy term revisions') || user_access('administer taxonomy')) {
    $delete_permission = TRUE;
  }

  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();
    if ($revision->revision_id == $viewed_term->revision_id) {
      $row[] = array(
        'data' => t('!date by !username', array(
            '!date' => l(format_date($revision->timestamp, 'short'), "taxonomy/term/$viewed_term->tid"),
            '!username' => theme('username', array('account' => user_load($revision->uid))),
          )) . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : ''),
        'class' => array('revision-current'),
      );
      $operations[] = array(
        'data' => drupal_placeholder(t('current revision')),
        'class' => array('revision-current'),
        'colspan' => 2,
      );
    }
    else {
      $row[] = t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'short'), "taxonomy/term/$viewed_term->tid/revisions/$revision->revision_id/view"),
          '!username' => theme('username', array('account' => user_load($revision->uid))),
        )) . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : '');
      if ($revert_permission) {
        $operations[] = l(t('revert'), "taxonomy/term/$viewed_term->tid/revisions/$revision->revision_id/revert");
      }
      else {
        // Empty row for theming purposes.
        $operations[] = "";
      }
      if ($delete_permission) {
        $operations[] = l(t('delete'), "taxonomy/term/$viewed_term->tid/revisions/$revision->revision_id/delete");
      }
      else {
        // Empty row for theming purposes.
        $operations[] = "";
      }
    }
    $rows[] = array_merge($row, $operations);
  }
  $build['taxonomy_term_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );
  return $build;
}

/**
 * Function which return a more complex form when diff module is activated.
 *
 * @param $form
 * @param $form_state
 * @param $viewed_term
 * @return mixed
 */

function taxonomy_revision_taxonomy_revisions($form, $form_state, $viewed_term) {
  drupal_set_title(t('Revisions for %title', array('%title' => $viewed_term->name)), PASS_THROUGH);

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $viewed_term->revision_id,
  );

  $revision_list = taxonomy_revision_list($viewed_term);

  if (count($revision_list) > REVISION_LIST_SIZE) {
    // If the list of revisions is longer than the number shown on one page
    // split the array.
    $page = isset($_GET['page']) ? $_GET['page'] : '0';
    $revision_chunks = array_chunk(taxonomy_revision_list($viewed_term), REVISION_LIST_SIZE);
    $revisions = $revision_chunks[$page];
    // Set up global pager variables as would 'pager_query' do.
    // These variables are then used in the theme('pager') call later.
    global $pager_page_array, $pager_total, $pager_total_items;
    $pager_total_items[0] = count($revision_list);
    $pager_total[0] = ceil(count($revision_list) / REVISION_LIST_SIZE);
    $pager_page_array[0] = max(0, min($page, ((int) $pager_total[0]) - 1));
  }
  else {
    $revisions = $revision_list;
  }

  $delete_permission = FALSE;
  $revert_permission = FALSE;
  if (user_access('revert taxonomy term revisions') || user_access('administer taxonomy')) {
    $revert_permission = TRUE;
  }
  if (user_access('delete taxonomy term revisions') || user_access('administer taxonomy')) {
    $delete_permission = TRUE;
  }

  // Take all of the revisions.
  foreach ($revisions as $revision) {
    $operations = array();
    $revision_ids[$revision->revision_id] = '';

    $revision_log = ($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : '';
    if ($revision->current_revision_id > 0) {
      $form['info'][$revision->revision_id] = array(
        '#markup' => t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'small'), "taxonomy/term/$viewed_term->tid"),
          '!username' => theme('username', array('account' => $revision)))) . $revision_log,
      );
    }
    else {
      $diff_date = l(format_date($revision->timestamp, 'small'), "taxonomy/term/$viewed_term->tid/revisions/$revision->revision_id/view");
      $form['info'][$revision->revision_id] = array(
        '#markup' => t('!date by !username', array(
            '!date' => $diff_date,
            '!username' => theme('username', array('account' => $revision)))
        ) . $revision_log,
      );
      if ($revert_permission) {
        $operations[] = array(
          '#markup' => l(t('Revert'), "taxonomy/term/$viewed_term->tid/revisions/$revision->revision_id/revert"),
        );
      }
      if ($delete_permission) {
        $operations[] = array(
          '#markup' => l(t('Delete'), "taxonomy/term/$viewed_term->tid/revisions/$revision->revision_id/delete"),
        );
      }
      // Set a dummy, even if the user has no permission for the other
      // operations, so that we can check if the operations array is
      // empty to know if the row denotes the current revision.
      $operations[] = array();
    }
    $form['operations'][$revision->revision_id] = $operations;

  }
  $new_vid = key($revision_ids);
  next($revision_ids);
  $old_vid = key($revision_ids);
  $form['diff']['old'] = array(
    '#type' => 'radios',
    '#options' => $revision_ids,
    '#default_value' => $old_vid,
  );
  $form['diff']['new'] = array(
    '#type' => 'radios',
    '#options' => $revision_ids,
    '#default_value' => $new_vid,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Compare'));

  if (count($revision_list) > REVISION_LIST_SIZE) {
    $form['#suffix'] = theme('pager');
  }
  $attachments['css'] = array(
    drupal_get_path('module', 'diff') . "/css/diff.default.css",
  );
  $attachments['js'] = array(
    drupal_get_path('module', 'diff') . "/js/diff.js",
    array(
      'data' => array('diffRevisionRadios' => 'simple'),
      'type' => 'setting',
    ),
  );
  $form['#attached'] = $attachments;
  return $form;
}

/**
 * Submit code for input form to select two revisions.
 */
function taxonomy_revision_taxonomy_revisions_submit($form, &$form_state) {
  // The ids are ordered so the old revision is always on the left.
  $old_vid = min($form_state['values']['old'], $form_state['values']['new']);
  $new_vid = max($form_state['values']['old'], $form_state['values']['new']);
  $form_state['redirect'] = 'taxonomy/term/' . $form_state['build_info']['args'][0]->tid . '/revisions/view/' . $old_vid . '/' . $new_vid;
}

/**
 * Validation for input form to select two revisions.
 */
function taxonomy_revision_taxonomy_revisions_validate($form, &$form_state) {
  $old_vid = $form_state['values']['old'];
  $new_vid = $form_state['values']['new'];
  if ($old_vid == $new_vid || !$old_vid || !$new_vid) {
    form_set_error('diff', t('Select different revisions to compare.'));
  }
}
