<?php

/**
 * Callback to display all the oboe oboes.
 */
function oboe_admin_jobs(){
  $admin_access = user_access('administer oboes');
  // Build the sortable table header.
  $header = array(
    'title' => array(
      'data' => t('Title'),
      'field' => 'o.label'
    ),
    'type' => array(
      'data' => t('Type'),
      'field' => 'o.type'
    ),
    'author' => t('User'),
    'status' => t('Status'),
    'changed' => array(
      'data' => t('Updated'),
      'field' => 'o.changed',
      'sort' => 'desc'
    ),
    'operations' => array(
      'data' => t('Operations')
    )
  );
  $query = db_select('oboe', 'o')->extend('PagerDefault')->extend('TableSort');
  $oids = $query->fields('o', array(
    'oid'
  ))->limit(50)->orderByHeader($header)->execute()->fetchCol();
  $oboes = oboe_load_multiple($oids);
  // Prepare the list of oboes.
  $destination = drupal_get_destination();
  $options = array();
  $job_types = OBOEService::get_job_types();
  foreach($oboes as $oboe){
    $options[$oboe->oid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $oboe->label,
          '#href' => 'oboe/' . $oboe->oid
        )
      ),
      'type' => check_plain($job_types[$oboe->type]),
      'author' => theme('username', array(
        'account' => user_load($oboe->uid)
      )),
      'status' => $oboe->data['status'],
      'changed' => format_date($oboe->changed, 'short')
    );
    // Build a list of all the accessible operations for the current oboe.
    $operations = array();
    if(oboe_access('delete', $oboe)){
      $operations['delete'] = array(
        'title' => t('Delete'),
        'href' => 'oboe/' . $oboe->oid . '/delete',
        'query' => $destination
      );
    }
    $options[$oboe->oid]['operations'] = array();
    if(count($operations) > 1){
      // Render an unordered list of operations links.
      $options[$oboe->oid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array(
            'class' => array(
              'links',
              'inline'
            )
          )
        )
      );
    }elseif(!empty($operations)){
      // Render the first and only operation as a link.
      $link = reset($operations);
      $options[$oboe->oid]['operations'] = array(
        'data' => array(
          '#type' => 'link',
          '#title' => $link['title'],
          '#href' => $link['href'],
          '#options' => array(
            'query' => $link['query']
          )
        )
      );
    }
  }
  // Only use a tableselect when the current user is able to perform any
  // operations.
  $form['oboes'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.')
  );
  $form['pager'] = array(
    '#markup' => theme('pager')
  );
  return $form;
}
