<?php

function scratchpads_registry_stats_statistics_page(){
  // Entity counts ------------------------------------------------------------
  $results = db_select('node', 'n')->condition('type', 'entity_data')->condition('created', time() - 86400, '>')->fields('n', array(
    'nid'
  ))->execute();
  $rows = array();
  $total = 0;
  $total_users = array();
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
    if($node->field_machine_name[LANGUAGE_NONE][0]['value'] == 'user'){
      $total_users[$node->field_scratchpad[LANGUAGE_NONE][0]['nid']] = $node->field_quantity[LANGUAGE_NONE][0]['value'];
    }
  }
  uasort($rows, 'scratchpads_registry_stats_uasort');
  $rows[] = array(
    'data' => array(
      t('Total'),
      $total
    ),
    'style' => 'background-color:#bbb !important;color:black;'
  );
  $content = array(
    'entity_counts' => array(
      '#prefix' => '<h3>' . t('Entity counts') . '</h3>',
      '#header' => array(
        t('Entity label'),
        t('Count')
      ),
      '#theme' => 'table',
      '#rows' => $rows
    )
  );
  // User counts --------------------------------------------------------------
  $results = db_select('node', 'n')->condition('type', 'user_data')->condition('created', time() - 86400, '>')->fields('n', array(
    'nid'
  ))->execute();
  $rows = array();
  $total = array(
    0,
    0,
    0
  );
  foreach($results as $row){
    $node = node_load($row->nid);
    $total[0] += $total_users[$node->field_scratchpad[LANGUAGE_NONE][0]['nid']];
    $total[1] += $node->field_active_in_last_7_days[LANGUAGE_NONE][0]['value'];
    $total[2] += $node->field_active_in_last_28_days[LANGUAGE_NONE][0]['value'];
    $scratchpad_node = node_load($node->field_scratchpad[LANGUAGE_NONE][0]['nid']);
    $rows[] = array(
      l($scratchpad_node->title, 'node/' . $scratchpad_node->nid),
      $total_users[$node->field_scratchpad[LANGUAGE_NONE][0]['nid']],
      $node->field_active_in_last_7_days[LANGUAGE_NONE][0]['value'],
      $node->field_active_in_last_28_days[LANGUAGE_NONE][0]['value']
    );
  }
  uasort($rows, 'scratchpads_registry_stats_uasort');
  $rows[] = array(
    'data' => array(
      t('Totals'),
      $total[0],
      $total[1],
      $total[2]
    ),
    'style' => 'background-color:#bbb !important;color:black;'
  );
  $content['user_counts'] = array(
    '#prefix' => '<h3>' . t('User counts') . '</h3>',
    '#header' => array(
      t('Scratchpad'),
      t('Number of users'),
      t('Number of users active in last 7 days'),
      t('Number of users active in last 28 days')
    ),
    '#theme' => 'table',
    '#rows' => $rows
  );
  return $content;
}

/**
 * Sort the array.
 */
function scratchpads_registry_stats_uasort($a, $b){
  if($b[1] == $a[1]){return strcasecmp($b[0], $a[0]);}
  return $b[1] > $a[1];
}