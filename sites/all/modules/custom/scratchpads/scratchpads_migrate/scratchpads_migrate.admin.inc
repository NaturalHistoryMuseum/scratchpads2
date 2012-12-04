<?php

function scratchpads_migrate_tasks_form($form, &$form_state){
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'scratchpads_migrate') . '/css/scratchpads-migrate.css'
  );
  global $s1_domain;
  // For those sites created before I added the task loggins
  if(db_table_exists('scratchpads_migrate_task')){
    // Get tasks
    $tasks = db_query('SELECT * FROM {scratchpads_migrate_task}');
    if($tasks->rowCount()){
      $header = array(
        'source' => t('Source'),
        'error' => t('Error'),
        'action' => t('action'),
        'destination' => t('Destination')
      );
      //Build the rows.
      $options = array();
      $defaults = array();
      foreach($tasks as $task){
        $options[$task->tid] = array(
          '#attributes' => array(
            'class' => array(
              ($task->status ? 'error' : 'ok')
            )
          ),
          '#value' => $task->entity,
          'source' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => t("Source !entity", array(
                '!entity' => $task->entity
              )),
              '#href' => url($s1_domain . '/' . $task->entity . '/' . $task->entity_id)
            )
          ),
          'destination' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => t("Destination !entity", array(
                '!entity' => $task->entity
              )),
              '#href' => $task->entity . '/' . $task->entity_id
            )
          ),
          'error' => $task->error,
          'action' => $task->action
        );
        $defaults[$task->tid] = !$task->status;
      }
      $form['tasks'] = array(
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#default_value' => $defaults
      );
      $form['complete'] = array(
        '#type' => 'submit',
        '#value' => t('Mark as complete')
      );
    }
  }
  if(!isset($form['tasks'])){
    drupal_set_message(t('There are no migration tasks'));
  }
  return $form;
}

function scratchpads_migrate_tasks_form_submit($form, &$form_state){
  // Reset all to not completed
  db_update('scratchpads_migrate_task')->fields(array(
    'status' => 1
  ))->execute();
  $completed_tasks = array_filter($form_state['values']['tasks']);
  // Mark some as completed
  $num_updated = db_update('scratchpads_migrate_task')->fields(array(
    'status' => 0
  ))->condition('tid', $completed_tasks, 'IN')->execute();
  drupal_set_message(format_plural(count($completed_tasks), '1 task marked as completed', '@count tasks marked as completed'));
}
