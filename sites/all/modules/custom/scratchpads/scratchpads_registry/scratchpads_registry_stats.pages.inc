<?php

function scratchpads_registry_stats_statistics_page(){
  // Lets start with the basic stats first.
  $results = db_select('node', 'n')->condition('type', 'entity_data')->condition('created', time() - 86400, '>')->fields('n', array(
    'nid'
  ))->execute();
  $rows = array();
  $total = 0;
  foreach($results as $row){
    $node = node_load($row->nid);
    if(!isset($rows[$node->field_machine_name[LANGUAGE_NONE][0]['value']])){
      $rows[$node->field_machine_name[LANGUAGE_NONE][0]['value']] = array(
        '',
        0
      );
      $rows[$node->field_machine_name[LANGUAGE_NONE][0]['value']][0] = $node->field_label[LANGUAGE_NONE][0]['value'];
    }
    $total += $node->field_quantity[LANGUAGE_NONE][0]['value'];
    $rows[$node->field_machine_name[LANGUAGE_NONE][0]['value']][1] += $node->field_quantity[LANGUAGE_NONE][0]['value'];
  }
  uasort($rows, 'scratchpads_registry_stats_uasort');
  $rows[] = array(
    'data' => array(
      array(
        'data' => t('Total')
      ),
      array(
        'data' => $total
      )
    ),
    'style' => 'background-color:#bbb !important;color:black;'
  );
  return array(
    '#header' => array(
      t('Entity label'),
      t('Count')
    ),
    '#theme' => 'table',
    '#rows' => $rows
  );
}

/**
 * Sort the array.
 */
function scratchpads_registry_stats_uasort($a, $b){
  if($b[1] == $a[1]){return strcasecmp($b[0], $a[0]);}
  return $b[1] > $a[1];
}