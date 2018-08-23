<?php

/**
 * Callback to list a site's Scratchpads Messages, and also to allow deletion
 * of them.
 */
function scratchpads_messages_list_form($form, $form_state){
  if(isset($form_state['values']['op']) && $form_state['values']['op'] == t('Delete')){return scratchpads_messages_multiple_delete_confirm($form_state, array_filter($form_state['values']['msgs']));}
  $form = array();
  $results = db_select('scratchpads_messages_message', 's')->fields('s')->execute();
  $msgs = array();
  $form['msgs'] = array();
  foreach($results as $msg){
    if ($msg->type == 3) {
      $form['msgs'][$msg->mid] = array("#disabled" => TRUE);
    }
    $msgs[$msg->mid] = '';
    $form['message'][$msg->mid] = array(
      '#markup' => $msg->message
    );
    $form['type'][$msg->mid] = array(
      '#markup' => scratchpads_messages_types($msg->type)
    );
    $form['created'][$msg->mid] = array(
      '#markup' => format_date($msg->created)
    );
    $query = db_select('scratchpads_messages_roles', 's');
    $query->innerJoin('role', 'r', 's.rid = r.rid');
    $roles = $query->fields('r', array(
      'name'
    ))->condition('mid', $msg->mid)->execute()->fetchAll(PDO::FETCH_COLUMN, 0);
    $form['roles'][$msg->mid] = array(
      '#markup' => count($roles) ? implode('<br/>', $roles) : t('All roles')
    );
    $form['expire'][$msg->mid] = array(
      '#markup' => $msg->expire ? format_date($msg->expire) : t('Never')
    );
  }
  $form['msgs']['#type'] = 'checkboxes';
  $form['msgs']['#options'] = $msgs;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array(
      'scratchpads_messages_list_messages_submit'
    )
  );
  $form['#theme'] = 'scratchpads_messages_list_messages';
  return $form;
}

/**
 *
 * @param unknown_type $form_state
 */
function scratchpads_messages_list_messages_submit($form, &$form_state){
  if(!empty($form_state['values']['msgs'])){
    $form_state['rebuild'] = TRUE;
  }
}

function scratchpads_messages_multiple_delete_confirm(&$form_state, $messages){
  $form['messages'] = array(
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
    '#tree' => TRUE
  );
  // array_filter returns only elements with TRUE values
  $results = db_select('scratchpads_messages_message', 's')->fields('s')->condition('mid', array_keys($messages))->execute();
  foreach($results as $row){
    $form['messages'][$row->mid] = array(
      '#type' => 'hidden',
      '#value' => $row->mid,
      '#prefix' => '<li>',
      '#suffix' => check_plain($row->message) . "</li>\n"
    );
  }
  $form['operation'] = array(
    '#type' => 'hidden',
    '#value' => 'delete'
  );
  $form['#submit'][] = 'scratchpads_messages_multiple_delete_confirm_submit';
  return confirm_form($form, t('Are you sure you want to delete these items?'), 'node', t('This action cannot be undone.'), t('Delete all'), t('Cancel'));
}

/**
 * Submit delete messages form
 */
function scratchpads_messages_multiple_delete_confirm_submit($form, $form_state){
  scratchpads_messages_delete_messages($form_state['values']['messages']);
}

/**
 * Delete messages by ID, and related records
 */
function scratchpads_messages_delete_messages($message_ids) {
  return array(
    "messages" => db_delete('scratchpads_"messages"_message')->condition('mid', $message_ids)->execute(),
    "viewed" => db_delete('scratchpads_messages_viewed')->condition('mid', $message_ids)->execute(),
    "roles" => db_delete('scratchpads_messages_roles')->condition('mid', $message_ids)->execute()
  );
}

/**
 * Theme the form
 */
function theme_scratchpads_messages_list_messages($vars){
  $form = $vars['scratchpads_messages_list'];
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $has_posts = isset($form['message']) && is_array($form['message']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';
  $header = array(
    $select_header,
    t('Message'),
    t('Roles'),
    t('Type'),
    t('Created'),
    t('Expire')
  );
  $output = '';
  if($has_posts){
    foreach(element_children($form['message']) as $key){
      $row = array();
      $row[] = drupal_render($form['msgs'][$key]);
      $row[] = drupal_render($form['message'][$key]);
      $row[] = drupal_render($form['roles'][$key]);
      $row[] = drupal_render($form['type'][$key]);
      $row[] = drupal_render($form['created'][$key]);
      $row[] = drupal_render($form['expire'][$key]);
      $rows[] = $row;
    }
  }else{
    $rows[] = array(
      array(
        'data' => t('No messages available.'),
        'colspan' => '5'
      )
    );
  }
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows
  ));
  foreach(element_children($form, TRUE) as $child_key){
    $output .= drupal_render($form[$child_key]);
  }
  return $output;
}

/**
 * Add a message
 */
function scratchpads_messages_add_form($form, $form_state){
  $date = date('Y-m-d G:i');
  $expire = date('Y-m-d G:i', time() + 172800);
  return array(
    'message' => array(
      '#title' => t('Message'),
      '#type' => 'textarea',
      '#required' => TRUE
    ),
    'created' => array(
      '#title' => t('Created'),
      '#type' => 'textfield',
      '#default_value' => $date,
      '#description' => t('Ensure the date format is kept <em>exactly</em> like !example_date', array(
        '!example_date' => $date
      )),
      '#required' => TRUE
    ),
    'expire' => array(
      '#title' => t('Expire'),
      '#type' => 'textfield',
      '#default_value' => $expire,
      '#description' => t('Note, the default expiry date is two days hence.') . '<br/>' . t('Ensure the date format is kept <em>exactly</em> like !example_date, or blank to never expire.', array(
        '!example_date' => $expire
      ))
    ),
    'type' => array(
      '#title' => t('Type'),
      '#type' => 'select',
      '#options' => scratchpads_messages_types(),
      '#required' => TRUE,
      '#default_value' => 1
    ),
    'roles' => array(
      '#title' => t('Roles'),
      '#type' => 'select',
      '#options' => user_roles(),
      '#multiple' => TRUE,
      '#default_value' => FALSE,
      '#description' => t('Leave blank to display to all roles')
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save')
    )
  );
}

/**
 * Validate the string format of the dates and times.
 */
function scratchpads_messages_add_form_validate(&$form, &$form_state){
  $created = strtotime($form_state['values']['created']);
  if(!$created){
    form_set_error('created', t('Ensure you keep the time format.'));
    unset($form['#redirect']);
  }
  if(strlen($form_state['values']['expire'])){
    $expire = strtotime($form_state['values']['expire']);
    if(!$expire){
      form_set_error('expire', t('Ensure you keep the time format.'));
      unset($form['#redirect']);
    }
  }
}

/**
 * Save a message
 */
function scratchpads_messages_add_message($values) {
  $created = strtotime($values['created']);
  $expire = strtotime($values['expire']);
  $mid = db_insert('scratchpads_messages_message')->fields(array(
    'message' => $values['message'],
    'created' => $created,
    'expire' => $expire ? $expire : 0,
    'type' => $values['type']
  ))->execute();
  if($mid){
    $query = db_insert('scratchpads_messages_roles')->fields(array(
      'mid',
      'rid'
    ));
    if(!count($values['roles'])){
      $query->values(array(
        'mid' => $mid,
        'rid' => 0
      ));
    }else{
      foreach($values['roles'] as $rid){
        $query->values(array(
          'mid' => $mid,
          'rid' => $rid
        ));
      }
    }
    $query->execute();

    return $mid;
  }

  return false;
}

/**
 * Save new message and redirect
 */
function scratchpads_messages_add_form_submit(&$form, &$form_state){
  scratchpads_messages_add_message($form_state['values']);

  $form_state['redirect'] = 'admin/config/system/scratchpads-messages';
}
