<?php
/**
 * Created by JetBrains PhpStorm.
 * User: rflueras
 * Date: 11/22/13
 * Time: 6:33 PM
 * To change this template use File | Settings | File Templates.
 */

/**
 * Theme function to display the revisions formular.
 * Test Test
 */
function theme_taxonomy_revision_taxonomy_revisions($vars) {
  $form = $vars['form'];
  $output = '';

  // Overview table:
  $header = array(
    t('Revision'),
    array('data' => drupal_render($form['submit']), 'colspan' => 2),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  if (isset($form['info']) && is_array($form['info'])) {
    foreach (element_children($form['info']) as $key) {
      $row = array();
      if (isset($form['operations'][$key][0])) {
        // Note: even if the commands for revert and delete are not permitted,
        // the array is not empty since we set a dummy in this case.
        $row[] = drupal_render($form['info'][$key]);
        $row[] = drupal_render($form['diff']['old'][$key]);
        $row[] = drupal_render($form['diff']['new'][$key]);
        $row[] = drupal_render($form['operations'][$key][0]);
        $row[] = drupal_render($form['operations'][$key][1]);
        $rows[] = array(
          'data' => $row,
          'class' => array('diff-revision'),
        );
      }
      else {
        // The current revision (no commands to revert or delete).
        $row[] = array(
          'data' => drupal_render($form['info'][$key]),
          'class' => array('revision-current'),
        );
        $row[] = array(
          'data' => drupal_render($form['diff']['old'][$key]),
          'class' => array('revision-current'),
        );
        $row[] = array(
          'data' => drupal_render($form['diff']['new'][$key]),
          'class' => array('revision-current'),
        );
        $row[] = array(
          'data' => t('current revision'),
          'class' => array('revision-current'),
          'colspan' => '2',
        );
        $rows[] = array(
          'data' => $row,
          'class' => array('error diff-revision'),
        );
      }
    }
  }
  $output .= theme('table__diff__revisions', array(
    'header' => $header,
    'rows' => $rows,
    'sticky' => FALSE,
    'attributes' => array('class' => 'diff-revisions'),
  ));

  $output .= drupal_render_children($form);
  return $output;
}