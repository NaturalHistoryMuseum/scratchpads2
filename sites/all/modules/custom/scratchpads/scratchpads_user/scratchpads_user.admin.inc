<?php

/**
 * Menu callback;
 * Replaces user_admin(list) function to allow for real user names / hiding dummy usernames
 */
function scratchpads_user_admin($callback_arg = '', $status_filter = NULL){
  $op = isset($_POST['op']) ? $_POST['op'] : $callback_arg;
  switch($op){
    case t('Cancel'):
    case t('Create new account'):
    case 'create':
      $build['user_register'] = drupal_get_form('user_register_form');
      break;
    default:
      if(!empty($_POST['accounts']) && isset($_POST['operation']) && ($_POST['operation'] == 'cancel')){
        $build['user_multiple_cancel_confirm'] = drupal_get_form('user_multiple_cancel_confirm');
      }else{
        $build['user_filter_form'] = drupal_get_form('user_filter_form');
        $build['user_admin_account'] = drupal_get_form('scratchpads_user_admin_account', $status_filter);
      }
  }
  return $build;
}

function scratchpads_user_admin_account_submit($form, &$form_state){
  // If we're filtering, we redirect to a URL with the filters as $_GET params.
  if($form_state['values']['op'] == $form_state['buttons'][1]['#value']){
    $query = array();
    foreach(array(
      'username',
      'given_name',
      'family_name'
    ) as $key){
      if(trim($form_state['values']['filter_' . $key])){
        $query[$key] = trim($form_state['values']['filter_' . $key]);
      }
    }
    drupal_goto($_GET['q'], array(
      'query' => $query
    ));
  }else if($form_state['values']['op'] == $form_state['buttons'][2]['#value']){
    drupal_goto($_GET['q']);
  }
  module_load_include('admin.inc', 'user');
  return user_admin_account_submit($form, $form_state);
}

function scratchpads_user_admin_account($form, &$form_state, $status_filter = NULL){
  $header = array(
    'username' => array(
      'data' => t('Username'),
      'field' => 'u.name'
    ),
    'given_names' => array(
      'data' => t('Given names'),
      'field' => 'fgn.field_user_given_names_value'
    ),
    'family_name' => array(
      'data' => t('Family name'),
      'field' => 'fn.field_user_family_name_value'
    ),
    'status' => array(
      'data' => t('Status'),
      'field' => 'u.status'
    ),
    'roles' => array(
      'data' => t('Roles')
    ),
    'member_for' => array(
      'data' => t('Member for'),
      'field' => 'u.created',
      'sort' => 'desc'
    ),
    'access' => array(
      'data' => t('Last access'),
      'field' => 'u.access'
    ),
    'operations' => array(
      'data' => t('Operations')
    )
  );
  $query = db_select('users', 'u');
  if(isset($status_filter)){
    $query->condition('u.status', $status_filter, '=');
  }
  $query->condition('u.uid', 0, '<>');
  $query->condition('u.uid', 1, '<>');
  user_build_filter_query($query);
  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.uid)');
  $query = $query->extend('PagerDefault')->extend('TableSort');
  // Note, the Joins must appear after the "extend('PagerDefault') code
  // http://drupal.stackexchange.com/questions/36907/pager-query-with-join
  $query->leftJoin('field_revision_field_user_family_name', 'fn', 'u.uid = fn.revision_id');
  $query->addField('fn', 'field_user_family_name_value', 'family_name');
  $query->leftJoin('field_revision_field_user_given_names', 'fgn', 'u.uid = fgn.revision_id');
  $query->addField('fgn', 'field_user_given_names_value', 'given_names');
  $query->fields('u', array(
    'uid',
    'name',
    'status',
    'created',
    'access',
    'pass'
  ));
  // Add filters if we have any
  if(isset($_GET['username'])){
    $query->condition('u.name', '%' . db_like($_GET['username']) . '%', 'LIKE');
    $query->condition('u.status', 0, '>');
    $count_query->condition('u.name', '%' . db_like($_GET['username']) . '%', 'LIKE');
    $count_query->condition('u.status', 0, '>');
  }
  if(isset($_GET['family_name'])){
    $query->condition('fn.field_user_family_name_value', '%' . db_like($_GET['family_name']) . '%', 'LIKE');
    $sub_query = db_select('field_revision_field_user_family_name', 'f')->fields('f', array(
      'entity_id'
    ))->condition('field_user_family_name_value', '%' . db_like($_GET['family_name']) . '%', 'LIKE');
    $count_query->condition('u.uid', $sub_query, 'IN');
  }
  if(isset($_GET['given_name'])){
    $query->condition('fgn.field_user_given_names_value', '%' . db_like($_GET['given_name']) . '%', 'LIKE');
    $sub_query = db_select('field_revision_field_user_given_names', 'f')->fields('f', array(
      'entity_id'
    ))->condition('field_user_given_names_value', '%' . db_like($_GET['given_name']) . '%', 'LIKE');
    $count_query->condition('u.uid', $sub_query, 'IN');
  }
  $query->limit(50);
  $query->orderByHeader($header);
  $query->setCountQuery($count_query);
  $result = $query->execute();
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array(
      'class' => array(
        'container-inline'
      )
    )
  );
  $options = array();
  foreach(module_invoke_all('user_operations') as $operation => $array){
    $options[$operation] = $array['label'];
  }
  // Remove various update options for non_users
  if(arg(2) == 'non_users'){
    unset($options['block']);
    unset($options['unblock']);
    unset($options['Add a role to the selected users']);
    unset($options['Remove a role from the selected users']);
    $options['cancel'] = 'Cancel the selected accounts';
    foreach($options as $key => $option){
      if(substr($key, 0, 24) == 'role_delegation_add_role'){
        unset($options[$key]);
      }elseif(substr($key, 0, 27) == 'role_delegation_remove_role'){
        unset($options[$key]);
      }
    }
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => $options,
    '#default_value' => 'unblock'
  );
  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update')
  );
  $destination = drupal_get_destination();
  $status = array(
    t('no login'),
    t('Active')
  );
  $roles = array_map('check_plain', user_roles(TRUE));
  $accounts = array();
  foreach($result as $account){
    $users_roles = array();
    $roles_result = db_query('SELECT rid FROM {users_roles} WHERE uid = :uid', array(
      ':uid' => $account->uid
    ));
    foreach($roles_result as $user_role){
      $users_roles[] = $roles[$user_role->rid];
    }
    asort($users_roles);
    if(empty($account->pass)){
      $username = '';
    }else{
      $username = theme('username', array(
        'account' => $account
      ));
    }
    $options[$account->uid] = array(
      'username' => $username,
      'given_names' => $account->given_names,
      'family_name' => $account->family_name,
      'status' => $status[$account->status],
      'roles' => theme('item_list', array(
        'items' => $users_roles
      )),
      'member_for' => format_interval(REQUEST_TIME - $account->created),
      'access' => $account->access ? t('@time ago', array(
        '@time' => format_interval(REQUEST_TIME - $account->access)
      )) : t('Never'),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('Edit'),
          '#href' => "user/$account->uid/edit",
          '#options' => array(
            'query' => $destination
          )
        )
      )
    );
  }
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#attributes' => array(
      'class' => array(
        'container-inline',
        'scratchpads-user-filter'
      )
    ),
    'filter_username' => array(
      '#type' => 'textfield',
      '#title' => '',
      '#description' => t('Filters the users displayed below on those whose username contains the string you enter.'),
      '#attributes' => array(
        'placeholder' => t('Username')
      ),
      '#default_value' => isset($_GET['username']) ? $_GET['username'] : ''
    ),
    'filter_given_name' => array(
      '#type' => 'textfield',
      '#title' => '',
      '#description' => t('Filters the users displayed below on those whose given name contains the string you enter.'),
      '#attributes' => array(
        'placeholder' => t('Given name')
      ),
      '#default_value' => isset($_GET['given_name']) ? $_GET['given_name'] : ''
    ),
    'filter_family_name' => array(
      '#type' => 'textfield',
      '#title' => '',
      '#description' => t('Filters the users displayed below on those whose family name contains the string you enter.'),
      '#attributes' => array(
        'placeholder' => t('Family name')
      ),
      '#default_value' => isset($_GET['family_name']) ? $_GET['family_name'] : ''
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Filter')
    ),
    'clear' => array(
      '#type' => 'submit',
      '#value' => t('Clear filter')
    )
  );
  // Add some CSS
  $form['#attached']['css'][] = drupal_get_path('module', 'scratchpads_user') . '/css/scratchpads_user.css';
  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No people available.')
  );
  $form['pager'] = array(
    '#markup' => theme('pager')
  );
  return $form;
}
