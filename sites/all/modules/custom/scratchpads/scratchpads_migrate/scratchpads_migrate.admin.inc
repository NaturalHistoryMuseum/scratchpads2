<?php

function scratchpads_migrate_overview_form($form, &$form_state){
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'scratchpads_migrate') . '/css/scratchpads-migrate.css'
  );
  $form['#prefix'] = t("<h4>Please complete the following tasks to get your migrated site ready to go live.</h4><br /><p>Once all the tasks have been completed, we will receive an email and will make arrangements to make your new site live.</p><br /><br />");
  $migration_status = variable_get('scratchpads_migration_status', array());
  $header = array(
    'task' => t('Task'),
    'description' => t('Description')
  );
  $options['check_content'] = array(
    '#attributes' => array(
      'class' => array(
        (in_array('check_content', $migration_status) ? 'ok' : 'error')
      )
    ),
    '#value' => 'check_content',
    'task' => t('Check content'),
    'description' => t('<p>Please check your new site to ensure all your content has correctly migrated across.</p><p>It is <em>very important</em> you do this before adding or editing content to the new site. If we need to re-migrate your site all your changes will be lost</p>')
  );
  // If tasks
  $options['home_page'] = array(
    '#attributes' => array(
      'class' => array(
        (in_array('home_page', $migration_status) ? 'ok' : 'error')
      )
    ),
    '#value' => 'home_page',
    'task' => t('Home page'),
    'description' => t('Please select images for your home page - you can do this by editing an image and selecting <em>Promote to home page</em>')
  );
  $options['structure'] = array(
    '#attributes' => array(
      'class' => array(
        (in_array('structure', $migration_status) ? 'ok' : 'error')
      )
    ),
    '#value' => 'structure',
    'task' => t('Site strucutre'),
    'description' => t('<p>Scratchpads 2.0 have a completely different structure. To help people organise their sites, page nodes now need to be placed in the site menu.</p><p>We recommed at this point, you edit your pages and organise them into the new site structure.</p>')
  );
  $options['custom_views'] = array(
    '#attributes' => array(
      'class' => array(
        (in_array('custom_views', $migration_status) ? 'ok' : 'error')
      )
    ),
    '#value' => 'custom_views',
    'task' => t('Custom views'),
    'description' => t('<p>If you had custom views in Scratchpads 1.0 you will need to recreate them in your new site.</p><p>We have tried to create many more default views in 2.0 though, so hopefully this shouldn\'t be necessary. Please get in touch if you need to add many, as this is something we can probably help with.</p>')
  );
  $options['blocks'] = array(
    '#attributes' => array(
      'class' => array(
        (in_array('blocks', $migration_status) ? 'ok' : 'error')
      )
    ),
    '#value' => 'blocks',
    'task' => t('Blocks'),
    'description' => t('<p>The page structure of the new sites has changed - for exmaple, there is no right hand column any more.<p><p>We have placed all of your custom blocks in the left hand sidebar - but please check they look OK</p><p>You can change your blocks by going to <em>Structure > Blocks</em> section of your new site</p>')
  );
  // Are there any tasks
  $error_count = scratchpads_migrate_get_error_count();
  $options['errors'] = array(
    '#attributes' => array(
      'class' => array(
        ($error_count > 0 ? 'error' : 'ok'),
        'scratchpads-migrate-errors'
      )
    ),
    '#value' => 'errors',
    'task' => t('Migration errors'),
    'description' => t('<p>There <a href="/admin/config/content/scratchpads-migrate/error">!errors</a>. Please mark as complete when they have been resolved.</p>', array(
      '!errors' => format_plural($error_count, 'is 1 migration error which requires your attention', 'are @count migration errors which require your attention')
    ))
  );
  $form['tasks'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#default_value' => array_fill_keys($migration_status, true)
  );
  $form['complete'] = array(
    '#type' => 'submit',
    '#value' => t('Mark as complete')
  );
  return $form;
}

function scratchpads_migrate_tasks_complete(){
  $migration_status = variable_get('scratchpads_migration_status', array());
  $form = scratchpads_migrate_overview_form();
  $completed = false;
  if(!scratchpads_migrate_get_error_count()){
    $migration_status[] = 'errors';
    $completed = !count(array_diff(array_keys($form['tasks']['#options']), $migration_status));
  }
  return $completed;
}

function scratchpads_migrate_get_error_count(){
  if(db_table_exists('scratchpads_migrate_task')){
    $count = db_query('SELECT COUNT(*) FROM {scratchpads_migrate_task} WHERE status=1')->fetchField();
  }else{
    $count = 0;
  }
  return $count;
}

function scratchpads_migrate_overview_form_submit($form, &$form_state){
  $migration_status = array_keys(array_filter($form_state['values']['tasks']));
  variable_set('scratchpads_migration_status', $migration_status);
  if(scratchpads_migrate_tasks_complete()){
    $message = t('Thank you! All tasks have been completed and we will now arrange to get your new site live.');
    scratchpads_migrate_site_live_notification();
  }else{
    $message = t('Thank you! Migration tasks updated.');
  }
  drupal_set_message($message);
}

function scratchpads_migrate_error_form($form, &$form_state){
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

function scratchpads_migrate_error_form_submit($form, &$form_state){
  // Reset all to not completed
  db_update('scratchpads_migrate_task')->fields(array(
    'status' => 1
  ))->execute();
  $completed_tasks = array_filter($form_state['values']['tasks']);
  // Mark some as completed
  $num_updated = db_update('scratchpads_migrate_task')->fields(array(
    'status' => 0
  ))->condition('tid', $completed_tasks, 'IN')->execute();
  if(scratchpads_migrate_tasks_complete()){
    $message = t('Thank you! All tasks & migration errors have been completed and we will now arrange to get your new site live.');
    scratchpads_migrate_site_live_notification();
  }else{
    $message = t('Migration errors updated.');
  }
  drupal_set_message($message);
}

/**
 * Send email notification site is ready to go live
 */
function scratchpads_migrate_site_live_notification(){
  global $base_url;
  global $user;
  $from = $user->mail;
  $language = 'en';
  drupal_mail('scratchpads_migrate', 'scratchpads-migrate-mail', SCRATCHPADS_MIGRATE_NOTIFICATIONS_EMAIL, $language, array(
    'subject' => t('Make !site live', array(
      '!site' => parse_url($base_url, PHP_URL_HOST)
    )),
    'body' => array(
      t('!base_url has been checked and all migration tasks completed. Please make it live.', array(
        '!base_url' => $base_url
      ))
    )
  ), $from, TRUE);
}

