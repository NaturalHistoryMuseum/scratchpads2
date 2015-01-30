<?php

/**
 * Implements hook_block_view().
 */
function taverna_overlay_page($tid){
  // FIXME - dynamically set the height of the block.
  return array(
    'subject' => t('Run an Ecological Niche Model via BioVeL'),
    'content' => array(
      'iframe' => array(
        '#markup' => '<iframe src="https://portal.biovel.eu/runs/new?embedded=true&version=1&workflow_id=107&input_url=' . url('taxonomy/term/' . arg(2) . '/enm.csv', array(
          'absolute' => TRUE
        )) . '" width="100%" height="800px"/>'
      )
    )
  );
}

/**
 * Menu callback.
 * 
 * View the output of a run.  Note, this only works with specific run types.  If
 * the type is unknown, we display an error message.
 */
function taverna_page_view_output($taverna){
  // If there is a menu link to this taverna, the link becomes the last part
  // of the active trail, and the link name becomes the page title.
  // Thus, we must explicitly set the page title to be the taverna title.
  drupal_set_title("Output for: {$taverna->label}");
  // Set the breadcrumb to include /admin/services in the trail.
  $bc = drupal_get_breadcrumb();
  $bc[] = l(t('Services'), 'admin/services');
  drupal_set_breadcrumb($bc);
  switch($taverna->type){
    case 'killerwhaledemography(scratchpad)':
      if($taverna->fid){
        @drupal_mkdir('public://taverna');
        @drupal_mkdir('public://taverna/' . $taverna->tvid);
        $file = file_load($taverna->fid);
        $zip = new ZipArchive();
        if($zip->open(drupal_realpath($file->uri))){
          if($zip->extractTo(drupal_realpath('public://taverna/' . $taverna->tvid . '/'))){
            $zip->close();
            $zip = new ZipArchive();
            $zip->open(drupal_realpath('public://taverna/' . $taverna->tvid . '/zipFile.zip'));
            if($zip->extractTo(drupal_realpath('public://taverna/' . $taverna->tvid . '/'))){
              drupal_unlink('public://taverna/' . $taverna->tvid . '/zipFile.zip');
              $content = array();
              foreach(scandir('public://taverna/' . $taverna->tvid . '/') as $file){
                if(strlen($file) > 2){
                  $file_extension = explode('.', $file);
                  switch($file_extension[1]){
                    default:
                    case 'pdf':
                      $content[] = array(
                        '#markup' => '<h2>' . $file . '</h2><p><a href="' . file_create_url('public://taverna/' . $taverna->tvid . '/' . $file) . '">Download PDF file</a>'
                      );
                      break;
                    case 'csv':
                      $f = fopen('public://taverna/' . $taverna->tvid . '/' . $file, 'r');
                      $header = fgetcsv($f);
                      if(count($header) > 100){
                        $content[] = array(
                          '#markup' => '<h2>' . $file . '</h2><p>Too big to display, please <a href="' . file_create_url('public://taverna/' . $taverna->tvid . '/' . $file) . '">download CSV file</a>.'
                        );
                      }else{
                        $rows = array();
                        while(($row = fgetcsv($f)) != FALSE){
                          $rows[] = $row;
                        }
                        $content[] = array(
                          'title' => array(
                            '#markup' => '<h2>' . $file . '</h2>'
                          ),
                          array(
                            '#theme' => 'table',
                            '#rows' => $rows,
                            '#header' => $header
                          )
                        );
                      }
                      break;
                    case 'txt':
                      $content[] = array(
                        '#markup' => '<h2>' . $file . '</h2><div style="border:solid 1px grey; background-color:#eee;padding:10px;margin-bottom:15px"><p>' . check_plain(file_get_contents('public://taverna/' . $taverna->tvid . '/' . $file)) . '</p></div>'
                      );
                      break;
                    case 'png':
                      $content[] = array(
                        '#markup' => '<h2>' . $file . '</h2><p><img src="' . file_create_url('public://taverna/' . $taverna->tvid . '/' . $file) . '"/></p>'
                      );
                      break;
                  }
                }
              }
              return $content;
            }
          }
        }
      }
      break;
  }
}

/**
 * Menu callback. 
 */
function taverna_add_run(){
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  // Bypass the node/add listing if only one content type is available.
  if(count($content) == 1){
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  return theme('node_add_list', array(
    'content' => $content
  ));
}

/**
 * Confirm delete an taverna entity.
 */
function taverna_delete_confirm($form, &$form_state, $taverna){
  $form['#taverna'] = $taverna;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['tvid'] = array(
    '#type' => 'value',
    '#value' => $taverna->tvid
  );
  return confirm_form($form, t('Are you sure you want to delete %title?', array(
    '%title' => $taverna->label
  )), 'taverna/' . $taverna->tvid, t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Execute taverna deletion
 */
function taverna_delete_confirm_submit($form, &$form_state){
  if($form_state['values']['confirm']){
    taverna_delete($form_state['values']['tvid']);
  }
  $form_state['redirect'] = '<front>';
}

/**
 * Returns an taverna submission form.
 */
function taverna_add($type){
  $types = TavernaService::get_run_types(FALSE, variable_get('taverna_limit_run_types', FALSE));
  global $user;
  $taverna = (object)array(
    'uid' => $user->uid,
    'type' => $type
  );
  drupal_set_title(t('Create @name run', array(
    '@name' => $types[$type]['title']
  )), PASS_THROUGH);
  $output = drupal_get_form($type . '_taverna_form', $taverna);
  return $output;
}

/**
 * Function to submit an interaction
 */
function taverna_interaction($taverna){
  $taverna_service = new TavernaService($taverna->data['id']);
  $taverna_service->interact();
}

/**
 * Callback defined in hook_forms().
 */
function taverna_run_form($form, &$form_state, $taverna){
  // Firstly, we need to get the parameters for this taverna type.
  $run_type = TavernaService::get_run_types($taverna->type);
  if(isset($run_type['inputs_attributes'])){
    foreach($run_type['inputs_attributes'] as $parameter){
      $form[$parameter['name']] = array(
        '#type' => 'textfield',
        '#default_value' => $parameter['value'],
        '#title' => $parameter['name']
      );
    }
  }
  $form['run_description'] = array(
    '#type' => 'hidden',
    '#value' => $run_type['name'] . ' (' . format_date(time()) . ')'
  );
  $form['run_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE
  );
  $form['#taverna'] = $taverna;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create')
  );
  return $form;
}

/**
 * Submit for the above function
 */
function taverna_run_form_submit(&$form, &$form_state){
  try{
    $taverna_service = new TavernaService();
    $taverna_service->set_type($form['#taverna']->type);
    $parameters = $form_state['values'];
    unset($parameters['op'], $parameters['form_id'], $parameters['form_token'], $parameters['form_build_id'], $parameters['submit']);
    foreach($parameters as $key => $value){
      if($value == ''){
        unset($parameters[$key]);
      }
    }
    $taverna_service->set_parameters($parameters);
    if($taverna_service->run()){
      // The run has run correctly, we can create the entity for this run.
      $taverna = $form['#taverna'];
      $taverna->label = $taverna_service->get_run_name();
      $taverna->description = $taverna_service->get_run_description();
      $taverna->data = $taverna_service->get_response();
      $taverna->created = time();
      $taverna->changed = time();
      entity_save('taverna', $taverna);
      $form_state['redirect'] = 'taverna/' . $taverna->tvid;
    }else{
      drupal_set_message('Your submission failed.  Please try again later.  If you continue to experience problems, then please consult the user support.', 'error');
    }
  }
  catch(TavernaServiceException $e){
    watchdog('taverna_error', print_r($e, 1));
    drupal_set_message('Your submission failed.  Please try again later.  If you continue to experience problems, then please consult the user support.', 'error');
  }
}

/**
 * Callback to display all the taverna tavernas.
*/
function taverna_admin_runs(){
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
  $query = db_select('taverna', 'o')->extend('PagerDefault')->extend('TableSort');
  $tvids = $query->fields('o', array(
    'tvid'
  ))->limit(50)->orderByHeader($header)->execute()->fetchCol();
  $tavernas = taverna_load_multiple($tvids);
  // Prepare the list of tavernas.
  $destination = drupal_get_destination();
  $options = array();
  $run_types = tavernaService::get_run_types(FALSE, variable_get('taverna_limit_run_types', FALSE));
  foreach($tavernas as $taverna){
    $options[$taverna->tvid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $taverna->label,
          '#href' => 'taverna/' . $taverna->tvid
        )
      ),
      'type' => check_plain($run_types[$taverna->type]['title']),
      'author' => theme('username', array(
        'account' => user_load($taverna->uid)
      )),
      'status' => isset($taverna->data['state']) ? ucfirst($taverna->data['state']) : '',
      'changed' => format_date($taverna->changed, 'short')
    );
    // Build a list of all the accessible operations for the current taverna.
    $operations = array();
    if(taverna_access('delete', $taverna)){
      $operations['delete'] = array(
        'title' => t('Delete'),
        'href' => 'taverna/' . $taverna->tvid . '/delete',
        'query' => $destination
      );
    }
    $options[$taverna->tvid]['operations'] = array();
    if(count($operations) > 1){
      // Render an unordered list of operations links.
      $options[$taverna->tvid]['operations'] = array(
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
      $options[$taverna->tvid]['operations'] = array(
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
  $form['tavernas'] = array(
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
