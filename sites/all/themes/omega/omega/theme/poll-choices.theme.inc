<?php

/**
 * @file
 * Contains the theme function override for 'poll_choices'.
 */

/**
 * Returns HTML for an admin poll form for choices.
 *
 * @ingroup themeable
 */
function omega_poll_choices($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('poll-choice-table', 'order', 'sibling', 'poll-weight');

  $is_admin = user_access('administer nodes');
  $delta = 0;
  $rows = array();
  $headers = array('', t('Choice'));
  if ($is_admin) {
    $headers[] = t('Vote count');
  }
  $headers[] = t('Weight');

  foreach (element_children($form) as $key) {
    $delta++;
    // Set special classes for drag and drop updating.
    $form[$key]['weight']['#attributes']['class'] = array('poll-weight');

    // Build the table row.
    $row = array(
      'data' => array(
        array('class' => array('choice-flag')),
        drupal_render($form[$key]['chtext']),
      ),
      'class' => array('draggable'),
    );
    if ($is_admin) {
      $row['data'][] = drupal_render($form[$key]['chvotes']);
    }
    $row['data'][] = drupal_render($form[$key]['weight']);

    // Add any additional classes set on the row.
    if (!empty($form[$key]['#attributes']['class'])) {
      $row['class'] = array_merge($row['class'], $form[$key]['#attributes']['class']);
    }

    $rows[] = $row;
  }

  $output = theme('table', array(
    'header' => $headers,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'poll-choice-table', 'class' => array('poll-choice-table'),
    ),
  ));
  $output .= drupal_render_children($form);
  return $output;
}
