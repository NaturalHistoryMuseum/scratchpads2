<?php

function theme_formflow_ui_steps_form($variables){
  $form = $variables['form'];
  $output = '<div class="column" id="available-flows">';
  $output .= drupal_render($form['available_forms']);
  $output .= '</div>';
  $output .= '<div class="column" id="enabled-flows">';
  if(isset($form['enabled_forms']) && count(element_children($form['enabled_forms']))){
    $header = array(
      t('Forms'),
      t('Weight'),
      t('Operations')
    );
    $rows = array();
    foreach(element_children($form['enabled_forms']) as $delta){
      $row = array();
      $row[] = drupal_render($form['enabled_forms'][$delta]['title']) . drupal_render($form['enabled_forms'][$delta]['path']);
      $form['enabled_forms'][$delta]['weight']['#attributes']['class'] = array(
        'weight'
      );
      $row[] = drupal_render($form['enabled_forms'][$delta]['weight']);
      $row[] = drupal_render($form['enabled_forms'][$delta]['remove']);
      $rows[] = array(
        'data' => $row,
        'class' => array(
          'draggable'
        )
      );
    }
    $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
        'id' => 'formflow-forms',
        'class' => array(
          'num-forms-' . count($rows)
        )
      )
    ));
  }else{
  	$output .= t('No forms enabled. Please start selecting a from from the right hand side.');
  }
  $output .= '</div>';
  $output .= drupal_render_children($form);
  return $output;
}

function formflow_ui_preprocess_formflow_ui_steps_form($variables){
  drupal_add_tabledrag('formflow-forms', 'order', 'sibling', 'weight');
  drupal_add_css(drupal_get_path('module', 'formflow') . '/css/formflow_ui.css');
}