<?php

/**
 * 
 */
function theme_taxonomystatistics_page($variables){
  $content = array(
    'title' => array(
      '#markup' => '<h2>' . $variables['name'] . '</h2><p>' . t('There is a total of !total terms, !leaf of which have no children.', array(
        '!total' => $variables['allterms'],
        '!leaf' => $variables['leafterms']
      )) . '</p>'
    )
  );
  foreach($variables['fields'] as $key => $field){
    // Add the titles
    array_unshift($field['values'], array(
      t('Value'),
      t('Count')
    ));
    // Add the chart!
    $content[$key] = array(
      'chart' => array(
        '#theme' => 'simple_google_material_chart',
        '#type' => 'Bar',
        '#options' => array(
          'legend' => array(
            'position' => 'none'
          ),
          'bars' => 'horizontal',
          'chart' => array(
            'title' => $field['label'],
            'subtitle' => $field['description']
          )
        ),
        '#data' => $field['values']
      )
    );
  }
  return drupal_render($content);
}