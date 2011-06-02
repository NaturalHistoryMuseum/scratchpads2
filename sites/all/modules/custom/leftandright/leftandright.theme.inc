<?php

/**
 * Copy of theme_taxonomy_overview_vocabularies, but with the addition of a 
 * column, and an increase in colspan.
 */
function theme_leftandright_taxonomy_overview_vocabularies($variables){
  $form = $variables['form'];
  $rows = array();
  foreach(element_children($form) as $key){
    if(isset($form[$key]['name'])){
      $vocabulary = &$form[$key];
      $row = array();
      $row[] = drupal_render($vocabulary['name']);
      if(isset($vocabulary['weight'])){
        $vocabulary['weight']['#attributes']['class'] = array(
          'vocabulary-weight'
        );
        $row[] = drupal_render($vocabulary['weight']);
      }
      $row[] = drupal_render($vocabulary['edit']);
      $row[] = drupal_render($vocabulary['list']);
      $row[] = drupal_render($vocabulary['add']);
      $row[] = drupal_render($vocabulary['rebuildlandr']);
      $rows[] = array(
        'data' => $row,
        'class' => array(
          'draggable'
        )
      );
    }
  }
  $header = array(
    t('Vocabulary name')
  );
  if(isset($form['actions'])){
    $header[] = t('Weight');
    drupal_add_tabledrag('taxonomy', 'order', 'sibling', 'vocabulary-weight');
  }
  $header[] = array(
    'data' => t('Operations'),
    'colspan' => '4'
  );
  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No vocabularies available. <a href="@link">Add vocabulary</a>.', array(
      '@link' => url('admin/structure/taxonomy/add')
    )),
    'attributes' => array(
      'id' => 'taxonomy'
    )
  )) . drupal_render_children($form);
}