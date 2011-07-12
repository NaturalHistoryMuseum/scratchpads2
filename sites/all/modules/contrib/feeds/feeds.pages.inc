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
      if (!(user_access('import '. $importer->id .' feeds') || user_access('administer feeds'))) {
        continue;
      }
      if (empty($importer->config['content_type'])) {
        $link = 'import/'. $importer->id;
        $title = $importer->config['name'];
      }
      elseif (node_access('create', $importer->config['content_type'])) {
        $link = 'node/add/'. str_replace('_', '-', $importer->config['content_type']);
        $title = node_get_types('name', $importer->config['content_type']);
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
    drupal_set_message(t('There are no importers, go to <a href="@importers">Feed importers</a> to create one or enable an existing one.', array('@importers' => url('admin/build/feeds'))));
  }
  $header = array(
    t('Import'),
    t('Description'),
  );
  return theme('table', $header, $rows);
}

/**
 * Render a feeds import form on import/[config] pages.
 */
function feeds_import_form(&$form_state, $importer_id) {
  $source = feeds_source($importer_id, empty($form['nid']['#value']) ? 0 : $form['nid']['#value']);

  $form = array();
  $form['#importer_id'] = $importer_id;
  // @todo Move this into fetcher?
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['feeds'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );
  $form['feeds'] += $source->configForm($form_state);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
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
  $source->addConfig($form_state['values']['feeds']);
  $source->save();

  // Refresh feed if import on create is selected.
  if ($source->importer->config['import_on_create']) {
    feeds_batch_set(t('Importing'), 'import', $form['#importer_id']);
  }

  // Add to schedule, make sure importer is scheduled, too.
  $source->schedule();
  $source->importer->schedule();
}

/**
 * Render a feeds import form on node/id/import pages.
 */
function feeds_import_tab_form(&$form_state, $node) {
  $importer_id = feeds_get_importer_id($node->type);

  $form = array();
  $form['#feed_nid'] = $node->nid;
  $form['#importer_id'] = $importer_id;
  $form['#redirect'] = 'node/'. $node->nid;
  return confirm_form($form, t('Import all content from feed?'), 'node/'. $node->nid, '', t('Import'), t('Cancel'), 'confirm feeds update');
}

/**
 * Submit handler for feeds_import_tab_form().
 */
function feeds_import_tab_form_submit($form, &$form_state) {
  $form_state['redirect'] = $form['#redirect'];
  feeds_batch_set(t('Importing'), 'import', $form['#importer_id'], $form['#feed_nid']);
}

/**
 * Render a feeds delete form.
 *
 * Used on both node pages and configuration pages.
 * Therefore $node may be missing.
 */
function feeds_delete_tab_form(&$form_state, $importer_id, $node = NULL) {
  if (empty($node)) {
    $form['#redirect'] = 'import/'. $importer_id;
  }
  else {
    $importer_id = feeds_get_importer_id($node->type);
    $form['#feed_nid'] = $node->nid;
    $form['#redirect'] = 'node/'. $node->nid;
  }
  // Form cannot pass on feed object.
  $form['#importer_id'] = $importer_id;
  return confirm_form($form, t('Delete all items from feed?'), $form['#redirect'], '', t('Delete'), t('Cancel'), 'confirm feeds update');
}

/**
 * Submit handler for feeds_delete_tab_form().
 */
function feeds_delete_tab_form_submit($form, &$form_state) {
  $form_state['redirect'] = $form['#redirect'];
  $feed_nid = empty($form['#feed_nid']) ? 0 : $form['#feed_nid'];
  feeds_batch_set(t('Deleting'), 'clear', $form['#importer_id'], $feed_nid);
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
 * Theme upload widget.
 */
function theme_feeds_upload($element) {
  drupal_add_css(drupal_get_path('module', 'feeds') .'/feeds.css');
  _form_set_class($element, array('form-file'));
  $output = '';
  if (!empty($element['#file_info'])) {
    $info = $element['#file_info'];
    $output .= '<div class="file-info">';
    $output .= '<div class="file-name">';
    $output .= l(basename($info['path']), $info['path']);
    $output .= '</div>';
    $output .= '<div class="file-size">';
    $output .= format_size($info['size']);
    $output .= '</div>';
    if (isset($info['mime'])) {
      $output .= '<div class="file-mime">';
      $output .= check_plain($info['mime']);
      $output .= '</div>';
    }
    $output .= '</div>';
  }
  $output .= '<div class="file-upload">';
  $output .= '<input type="file" name="'. $element['#name'] .'"'. ($element['#attributes'] ? ' '. drupal_attributes($element['#attributes']) : '') .' id="'. $element['#id'] .'" size="'. $element['#size'] ."\" />\n";
  $output .= '</div>';
  return theme('form_element', $element, $output);
}
