<?php

/**
 * Menu callback;
 * Replaces user_admin(list) function to allow for real user names / hiding dummy usernames
 */
function scratchpads_user_admin($callback_arg = ''){
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
        $build['user_admin_account'] = drupal_get_form('scratchpads_user_admin_account');
      }
  }
  return $build;
}

function scratchpads_user_admin_account_submit($form, &$form_state){
  module_load_include('admin.inc', 'user');
  return user_admin_account_submit($form, $form_state);
}

function scratchpads_user_admin_account(){
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
    t('blocked'),
    t('active')
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
      )) : t('never'),
      'operations' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => "user/$account->uid/edit",
          '#options' => array(
            'query' => $destination
          )
        )
      )
    );
  }
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