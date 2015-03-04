<?php

/**
 * 
 */
function theme_taxonomystatistics_page($variables){
  $content = '<h2>' . t('Statistics for %vocabulary with !total terms, !leaf of which have no children', array(
    '%vocabulary' => $variables['name'],
    '!total' => $variables['allterms'],
    '!leaf' => $variables['leafterms']
  )) . '</h2>';
  foreach($variables['fields'] as $field){
    $content .= '<h2>' . check_plain($field['label']) . '</h2>' . theme('table', array(
      'header' => array(
        t('Value'),
        t('Count'),
        t('Percentage of all terms')
      ),
      'rows' => $field['values']
    ));
  }
  return $content;
}