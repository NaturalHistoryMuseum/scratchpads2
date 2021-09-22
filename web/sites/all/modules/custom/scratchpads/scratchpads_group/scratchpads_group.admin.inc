<?php

/**
 * Form callback to allow the addition of multiple users.
 */
function scratchpads_group_add_users($form, &$form_state, $entity_type, $etid){
  og_set_breadcrumb($entity_type, $etid, array(
    l(t('Group'), "$entity_type/$etid/group")
  ));
  if(($group = og_get_group($entity_type, $etid)) != FALSE){
    $label = og_label($group->gid);
    $form['group'] = array(
      '#type' => 'value',
      '#value' => $group
    );
    $form['og_user'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add a group member to %group', array(
        '%group' => $label
      ))
    );
    $form['og_user']['og_add_user'] = array(
      '#type' => 'textarea',
      '#title' => t('User names'),
      '#required' => TRUE,
      '#description' => t('A list of usernames or email addresses (one per line) of the users you would like adding to this group.')
    );
    $form['og_user']['og_add_user_state'] = array(
      '#type' => 'select',
      '#title' => t('Membership state'),
      '#options' => og_group_content_states(),
      '#description' => t('Select the state of the membership.')
    );
    $options = array();
    foreach(entity_load('og_membership_type') as $type){
      $options[$type->name] = check_plain($type->description);
    }
    $form['og_user']['og_add_user_membership_type'] = array(
      '#type' => 'select',
      '#title' => t('Membership type'),
      '#options' => $options,
      '#default_value' => 'OG_MEMBERSHIP_TYPE_DEFAULT',
      '#description' => t('Select the group membership type of the membership.'),
      '#ajax' => array(
        'callback' => 'scratchpads_group_admin_add_user_ajax_callback',
        'wrapper' => 'og-ui-membership-type'
      )
    );
    $form['actions'] = array(
      '#type' => 'actions'
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add users')
    );
    return $form;
  }
  // Not a valid group node.
  drupal_not_found();
}

/**
 * AJAX callback to attach the group membership type fields to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function scratchpads_group_admin_add_user_ajax_callback(array $form, array &$form_state){
  return $form['membership_fields'];
}

/**
 * Validate handler; Add users to group.
 */
function scratchpads_group_add_users_validate($form, &$form_state){
  $lines = preg_split('/[\n\r]+/', trim($form_state['values']['og_add_user']));
  foreach($lines as $line){
    if(valid_email_address($line)){
      $account = user_load_by_mail($line);
      if(!$account){
        form_set_error('og_add_user', t('@email is not the email of any user on this site.', array(
          '@email' => $line
        )));
        ;
      }
    }else{
      $account = user_load_by_name($line);
      if(!$account){
        form_set_error('og_add_user', t('@name is not the username of any user on this site.', array(
          '@name' => $line
        )));
      }
    }
  }
}

/**
 * Submit handler; Add users to group.
 */
function scratchpads_group_add_users_submit($form, &$form_state){
  $lines = preg_split('/[\n\r]+/', trim($form_state['values']['og_add_user']));
  foreach($lines as $line){
    if(valid_email_address($line)){
      $account = user_load_by_mail($line);
    }else{
      $account = user_load_by_name($line);
    }
    $values = array(
      // Entity type is user by default.
      'entity' => $account,
      'state' => $form_state['values']['og_add_user_state'],
      'membership type' => $form_state['values']['og_add_user_membership_type'],
      'membership fields' => $form_state['values']['membership_fields']
    );
    og_group($form_state['values']['group']->gid, $values);
    drupal_set_message(t('%user has been added to group.', array(
      '%user' => $account->name
    )));
  }
}