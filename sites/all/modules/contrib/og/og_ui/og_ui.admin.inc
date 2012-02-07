<?php


/**
 * @file
 * Admin settings for Organic groups module.
 */

/**
 * Menu callback; Admin settings form.
 */
function og_ui_user_admin_settings($form_state) {
  $form = array();

  $form['og_group_manager_full_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Group manager full permissions'),
    '#description' => t('When enabled the group manager will have all the permissions in the group.'),
    '#default_value' => variable_get('og_group_manager_full_access', TRUE),
  );

  $form['og_node_access_strict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strict node access permissions'),
    '#description' => t('When enabled Organic groups will take full control of node access permission, and will deny access based on Organic groups access. Example: A content editor with the <em>Edit any page content</em> permission who is not a member of a group would be denied access to modifying page content in that group.'),
    '#default_value' => variable_get('og_node_access_strict', TRUE),
  );

  return system_settings_form($form);
}

/**
 * Provide an overview of the administrator menu items.
 *
 * @param $entity_type
 *   The entity type.
 * @param $etid
 *   The entity ID.
 */
function og_ui_group_admin_overview($entity_type, $etid) {
  og_set_breadcrumb($entity_type, $etid);

  $group = og_get_group($entity_type, $etid);
  $items = og_ui_get_group_admin($group->gid);

  foreach ($items as &$item) {
    // Re-format the URL.
    $item['href'] = "group/$entity_type/$etid/" . $item['href'];
    // Imitate the menu_get_item() content, so we can pass it to
    // theme_admin_block_content(), so we add the localized_options key.
    $item['localized_options'] = array();
  }

  $form['admin_block_content'] = array(
    '#theme' => 'admin_block_content',
    '#content' => $items,
  );

  return $form;
}

/**
 * Add users to group form.
 */
function og_ui_add_users($form, &$form_state, $entity_type, $etid) {
  og_set_breadcrumb($entity_type, $etid, array(l(t('Group'), "$entity_type/$etid/group")));
  if ($group = og_get_group($entity_type, $etid)) {
    $label = og_label($group->gid);

    $form['group'] = array('#type' => 'value', '#value' => $group);

    $form['og_user'] = array(
      '#type' => 'fieldset',
      '#title' => t('Add a group member to %group', array('%group' => $label)),
    );
    $form['og_user']['og_add_user'] = array(
      '#type' => 'textfield',
      '#title' => t('User name'),
      '#autocomplete_path' => 'user/autocomplete',
      '#required' => TRUE,
    );
    $form['og_user']['og_add_user_state'] = array(
      '#type' => 'select',
      '#title' => t('Membership state'),
      '#options' => og_group_content_states(),
      '#description' => t('Select the state of the membership.'),
    );
    $options = array();
    foreach (entity_load('og_membership_type') as $type) {
      $options[$type->name] = check_plain($type->description);
    }

    $form['og_user']['og_add_user_membership_type'] = array(
      '#type' => 'select',
      '#title' => t('Membership type'),
      '#options' => $options,
      '#default_value' => 'OG_MEMBERSHIP_TYPE_DEFAULT',
      '#description' => t('Select the group membership type of the membership.'),
      '#ajax' => array(
        'callback' => 'og_ui_admin_add_user_ajax_callback',
        'wrapper' => 'og-ui-membership-type',
      ),
    );

    // Add group membership form.
    $type = !empty($form_state['values']['og_add_user_membership_type']) ? $form_state['values']['og_add_user_membership_type'] : OG_MEMBERSHIP_TYPE_DEFAULT;
    $dummy_entity = og_membership_create(0, 'user', 0, array('type' => $type));
    $form['membership_fields'] = array(
      '#prefix' => '<div id="og-ui-membership-type">',
      '#suffix' => '</div>',
      '#tree' => TRUE,
      '#parents' => array('membership_fields'),
    );
    field_attach_form('og_membership', $dummy_entity, $form['membership_fields'], $form_state);

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Add users'));

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
function og_ui_admin_add_user_ajax_callback(array $form, array &$form_state) {
  return $form['membership_fields'];
}

/**
 * Validate handler; Add users to group.
 */
function og_ui_add_users_validate($form, &$form_state) {
  $account = user_load_by_name($form_state['values']['og_add_user']);
  if (!$account) {
    form_set_error('og_add_user', t('You have entered an invalid user name.'));
  }
  // @see entity_form_field_validate().
  $dummy_entity = new stdClass();
  if (!empty($form_state['values']['membership_fields'])) {
    $dummy_entity = (object) $form_state['values']['membership_fields'];
  }
  $dummy_entity->type = $form_state['values']['og_add_user_membership_type'];

  field_attach_form_validate('og_membership', $dummy_entity, $form['membership_fields'], $form_state);
}

/**
 * Submit handler; Add users to group.
 */
function og_ui_add_users_submit($form, &$form_state) {
  $account = user_load_by_name($form_state['values']['og_add_user']);

  $form_state['values'] += array('membership_fields' => array());

  $values = array(
    // Entity type is user by default.
    'entity' => $account,
    'state' => $form_state['values']['og_add_user_state'],
    'membership type' => $form_state['values']['og_add_user_membership_type'],
    'membership fields' => $form_state['values']['membership_fields']
  );
  og_group($form_state['values']['group']->gid, $values);
  drupal_set_message(t('%user has been added to group.', array('%user' => $account->name)));
}

/**
 * Form builder; OG user administration page.
 *
 * @ingroup forms
 * @see og_ui_user_admin_account_validate().
 * @see og_ui_user_admin_account_submit().
 */
function og_ui_user_admin_account($form, $form_state, $entity_type, $etid) {
  og_set_breadcrumb($entity_type, $etid, array(l(t('Group'), "$entity_type/$etid/group")));
  $group = og_get_group($entity_type, $etid);
  // Get the group real entity, so we can check the user ID of the entity.
  $entity = $group->getEntity();

  $form['group'] = array('#type' => 'value', '#value' => $group);
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  foreach (module_implements('og_user_operations') as $module) {
    $result = call_user_func($module . '_og_user_operations', array(), array('group' => $group));
    foreach ($result as $operation => $array) {
      $options[$operation] = $array['label'];
    }
  }

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'unblock',
  );

  $options = array();
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $states = og_group_content_states();
  $roles = og_roles($group->gid);
  $header = array(
    'username' => array('data' => t('Username'), 'field' => 'u.name'),
    'state' => array('data' => t('State'), 'field' => 'ogm.state'),
    'roles' => array('data' => t('Roles')),
    'member_for' => array('data' => t('Member for'), 'field' => 'ogm.created', 'sort' => 'desc'),
  );

  $query = db_select('users', 'u');
  $query->innerJoin('og_membership', 'ogm', 'u.uid = ogm.etid');
  $query
    ->condition('ogm.gid', $group->gid, '=')
    ->condition('ogm.entity_type', 'user', '=');

  $count_query = clone $query;
  $count_query->addExpression('COUNT(u.uid)');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('u', array('uid', 'name'))
    ->fields('ogm', array('state', 'created'))
    ->limit(25)
    ->orderByHeader($header)
    ->setCountQuery($count_query);
  $result = $query->execute();

  foreach ($result as $row) {
    // Note that we allow editing of the group manager, but will make sure on
    // submit that they can not be removed from group.
    $users_roles = array();
    foreach (og_get_user_roles($group->gid, $row->uid) as $rid) {
      if (!in_array($roles[$rid], array(OG_ANONYMOUS_ROLE, OG_AUTHENTICATED_ROLE))) {
        // Show the user's roles, except of the authenticated role, that all
        // group members have, or anonymous that blocked members have.
        $users_roles[] = $roles[$rid];
      }
    }
    asort($users_roles);

    $account = user_load($row->uid);
    $options[$account->uid] = array(
      'username' => theme('username', array('account' => $account)),
      'state' =>  $states[$row->state],
      'roles' => theme('item_list', array('items' => $users_roles)),
      'member_for' => format_interval(REQUEST_TIME - $row->created),
    );
  }

  // Add group manager details.
  $form['group_manager'] = array(
    '#type' => 'item',
    '#title' => t('Group manager'),
    '#markup' => theme('username', array('account' => $entity)),
  );

  $form['accounts'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => !empty($entity->uid) ? t('No people available apart of the group manager.') : t('No people available.'),
  );

  $form['pager'] = array('#markup' => theme('pager', array('tags' => NULL)));

  return $form;
}


function og_ui_user_admin_account_validate($form, &$form_state) {
  $form_state['values']['accounts'] = array_filter($form_state['values']['accounts']);
  if (count($form_state['values']['accounts']) == 0) {
    form_set_error('', t('No users selected.'));
  }
}

/**
 * Submit the user administration update form.
 */
function og_ui_user_admin_account_submit($form, &$form_state) {
  $group = $form_state['values']['group'];

  $operations = module_invoke_all('og_user_operations', $form, $form_state + array('group' => $group));
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked accounts.
  $accounts = array_filter($form_state['values']['accounts']);

  // If the operation is unblock, block or deny, we make sure the group manager
  // isn't being processed, if they were selected.
  if (in_array($form_state['values']['operation'], array('unblock', 'deny', 'block'))) {
    // Get the entity from group.
    $entity = $group->getEntity();

    if (!empty($entity->uid) && in_array($entity->uid, $accounts)) {
      // Remove from the accounts array, and give a message.
      unset($accounts[$entity->uid]);
      drupal_set_message('This update can not be used on the group manager.');
    }
  }

  if ($accounts && $function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($accounts), $operation['callback arguments']);
    }
    else {
      $args = array($accounts);
    }
    call_user_func_array($function, array('gid' => $group->gid) + $args);

    drupal_set_message(t('The update has been performed.'));
  }
}

/**
 * Menu callback: administer roles.
 *
 * @ingroup forms
 * @see og_user_admin_roles_validate()
 * @see og_user_admin_roles_submit()
 * @see theme_group_user_admin_new_role()
 */
function og_ui_user_admin_roles($form, $form_state, $entity_type = '', $etid = 0, $rid = 0) {
  $gid = 0;
  if ($entity_type && $group = og_get_group($entity_type, $etid)) {
    $gid = $group->gid;
  }

  $form['gid'] = array('#type' => 'value', '#value' => $gid);

  if ($rid) {
    if ($gid) {
      og_set_breadcrumb($entity_type, $etid, array(l(t('Group'), "$entity_type/$etid/group")), l(t('Roles'), "group/$entity_type/$etid/admin/people/roles"));
    }

    $group_roles = og_roles($gid);
    if (!in_array($rid, array_keys($group_roles))) {
      drupal_goto($gid ? "group/$entity_type, $etid/admin/people/roles" : 'admin/config/group/roles');
    }
    // Display the edit role form.
    $role = og_role_load($rid);
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Role name'),
      '#default_value' => $role->name,
      '#size' => 30,
      '#required' => TRUE,
      '#maxlength' => 64,
      '#description' => t('The name for this role. Example: "moderator", "editorial board", "site architect".'),
    );
    $form['rid'] = array(
      '#type' => 'value',
      '#value' => $rid,
    );
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save role'),
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete role'),
    );
  }
  else {
    // No role ID.
    if ($gid) {
      og_set_breadcrumb($entity_type, $etid, array(l(t('Group'), "$entity_type/$etid/group")));
    }
    if (!$gid || !og_is_group_default_access($group->gid)) {
      $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#title_display' => 'invisible',
        '#size' => 32,
        '#maxlength' => 64,
        '#required' => TRUE,
      );
      $form['add'] = array(
        '#type' => 'submit',
        '#value' => t('Add role'),
        '#validate' => array('og_ui_user_admin_roles_validate'),
        '#submit' => array('og_ui_user_admin_roles_submit'),
      );

      // Attach the CSS to the form.
      $form['#attached'] = array(
        'css' => array (
          'type' => 'file',
          'data' => drupal_get_path('module', 'og_ui') . '/css/og_ui.css',
        ),
      );
    }
  }

  return $form;
}

function og_ui_user_admin_roles_validate($form, &$form_state) {
  if (!empty($form_state['values']['name'])) {
    $roles = og_roles($form_state['values']['gid']);
    $unique_name = TRUE;
    if (!empty($form_state['values']['rid'])) {
      $role = og_role_load($form_state['values']['rid']);
      $unique_name = $role->name != $form_state['values']['name'];
    }
    if ($unique_name && in_array($form_state['values']['name'], $roles)) {
      form_set_error('name', t('The role name %name already exists. Please choose another role name.', array('%name' => $form_state['values']['name'])));
    }
  }
}

function og_ui_user_admin_roles_submit($form, &$form_state) {
  $gid = $form_state['values']['gid'];
  if ($gid) {
    $group = og_load($gid);
  }
  $role = new stdClass();
  $role->gid = $gid;
  $role->name = $form_state['values']['name'];
  if (!empty($form_state['values']['rid'])) {
    $role->rid = $form_state['values']['rid'];
  }
  // Indicates if we need to save role.
  $save = FALSE;
  if ($form_state['values']['op'] == t('Save role')) {
    og_role_save($role);
    drupal_set_message(t('The role has been renamed.'));
  }
  elseif ($form_state['values']['op'] == t('Add role')) {
    og_role_save($role);
    drupal_set_message(t('The role has been added.'));
  }
  elseif ($form_state['values']['op'] == t('Delete role')) {
    og_role_delete($form_state['values']['rid']);
    drupal_set_message(t('The role has been deleted.'));
  }

  $form_state['redirect'] = $gid ? 'group/' . $group->entity_type . '/' . $group->etid . '/admin/people/roles' : 'admin/config/group/roles';
  return;
}

/**
 * Theme the new role form.
 *
 * @ingroup themeable
 */
function theme_og_ui_user_admin_new_role($variables) {
  $form = $variables['form'];
  $header = array(t('Name'), array('data' => t('Operations'), 'colspan' => 2));
  // The group entity.
  if ($gid = $form['gid']['#value']) {
    $group = og_get_group('group', $gid);
  }

  $default_access = $gid && og_is_group_default_access($gid);

  foreach (og_roles($gid) as $rid => $name) {
    $text = !$default_access ? t('edit permissions') : t('view permissions');
    $path = $gid ? 'group/' . $group->entity_type .'/' . $group->etid . '/admin/people' : 'admin/config/group';
    $permissions = l($text, $path . '/permissions/' . $rid);

    if (!$default_access && !in_array($name, array(OG_ANONYMOUS_ROLE, OG_AUTHENTICATED_ROLE))) {
      $rows[] = array($name, l(t('edit role'), $path . '/roles/edit/' . $rid), $permissions);
    }
    else {
      $rows[] = array($name, t('locked'), $permissions);
    }
  }

  $rows[] = array(array('data' => drupal_render($form['name']) . drupal_render($form['add']), 'colspan' => 4, 'class' => 'edit-name'));

  $output = drupal_render_children($form);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));

  return $output;
}


/**
 * Menu callback: administer permissions.
 *
 * @ingroup forms
 * @see og_ui_user_admin_permissions_submit()
 * @see theme_group_ui_user_admin_permissions()
 */
function og_ui_user_admin_permissions($form, $form_state, $entity_type = NULL, $etid = NULL, $rid = NULL) {
  // If no node entity is provided then the node ID is 0, which means this
  // is the default permissions settings.
  $group = !empty($etid) ? og_get_group($entity_type, $etid) : array();
  $form['group'] = array('#type' => 'value', '#value' => $group);

  if (!empty($group)) {
    $gid =  $group->gid;
    og_set_breadcrumb($entity_type, $etid, array(l(t('Group'), "$entity_type/$etid/group")));
  }
  else {
    $gid = 0;
  }

  // Retrieve role names for columns.
  $role_names = og_roles($gid);
  if (!empty($rid)) {
    $role_names = array($rid => $role_names[$rid]);
  }
  // Fetch permissions for all roles or the one selected role.
  $role_permissions = og_role_permissions($role_names);

  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names,
  );

  // Render role/permission overview:
  $options = array();
  $module_info = system_get_info('module');

  $module = '';
  foreach (og_get_permissions() as $perm => $perm_item) {
    if ($perm_item['module'] != $module) {
      $module = $perm_item['module'];
      $form['permission'][] = array('#markup' => $module_info[$module]['name'], '#id' => $module);
    }
    // Initialize values.
    $perm_item += array('description' => '');
    $options[$perm] = '';
    $form['permission'][$perm] = array(
      '#type' => 'item',
      '#markup' => $perm_item['title'],
      '#description' => $perm_item['description'],
    );

    // TODO: Move this to theme function.
    // @see theme_user_admin_permissions().
    if (!empty($perm_item['restrict access'])) {
      $form['permission'][$perm]['#description'] .= '</br>';
      $form['permission'][$perm]['#description'] .= '<em class="permission-warning">' . t('Warning: Give to trusted roles only; this permission has security implications.') . '</em>';
    }
    foreach ($role_names as $rid => $name) {
      // Builds arrays for checked boxes for each role
      if (isset($role_permissions[$rid][$perm])) {
        $status[$rid][] = $perm;
      }
    }
  }

  // Have to build checkboxes here after checkbox arrays are built.
  foreach ($role_names as $rid => $name) {
    $form['checkboxes'][$rid] = array('#type' => 'checkboxes', '#options' => $options, '#default_value' => isset($status[$rid]) ? $status[$rid] : array());
    $form['role_names'][$rid] = array('#markup' => $name, '#tree' => TRUE);
  }

  if (empty($group)) {
    $form['#theme'] = array('og_ui_user_admin_permissions');
  }
  if ($gid == 0 || !og_is_group_default_access($group->gid)) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $gid == 0 ? t('Save global permissions') : t('Save permissions'),
      '#submit' => array('og_ui_user_admin_permissions_submit'),
    );
  }

  $form['#after_build'][] = 'og_ui_user_admin_permissions_after_build';

  return $form;
}

/**
 * Helper function to disable the permissions checkboxes.
 *
 * Every permission can declare to which roles it applies, as some don't make
 * sense if applied to anonymous or authenticated user (e.g. subscribe to group
 * should appear only to anonymous members).
 * A second case for disabling is when using deafult roles and permissions, and
 * the user is browsing a group's permissions page.
 */
function og_ui_user_admin_permissions_after_build($element) {
  $gid = !empty($element['group']['#value']->gid) ? $element['group']['#value']->gid : 0;

  // Indicate if all checkbes should be disabled.
  $disable = $gid && og_is_group_default_access($gid);

  // Authenticated roles.
  $auth_rids = array_flip(og_roles($gid));

  // Remove the anonymous member from the authenticated roles.
  $anon_rid = array_shift($auth_rids);

  foreach (og_get_permissions() as $key => $perm) {
    $perm['roles'] = drupal_map_assoc($perm['roles']);

    if ($disable || (empty($perm['roles']) || empty($perm['roles'][OG_ANONYMOUS_ROLE]))) {
      $element['checkboxes'][$anon_rid][$key]['#attributes']['disabled'] = TRUE;
    }
    if ($disable || (empty($perm['roles']) || empty($perm['roles'][OG_AUTHENTICATED_ROLE]))) {
      foreach ($auth_rids as $auth_rid) {
        $element['checkboxes'][$auth_rid][$key]['#attributes']['disabled'] = TRUE;
      }
    }
  }

  return $element;
}
/**
 * Save permissions selected on the administer permissions page.
 *
 * @see og_user_admin_permissions()
 */
function og_ui_user_admin_permissions_submit($form, &$form_state) {
  foreach ($form_state['values']['role_names'] as $rid => $name) {
    og_role_change_permissions($rid, $form_state['values'][$rid]);
  }

  drupal_set_message(t('The changes have been saved.'));
}


/**
 * Theme the administer permissions page.
 *
 * @ingroup themeable
 */
function theme_og_ui_user_admin_permissions($variables) {
  $form = $variables['form'];
  $gid = !empty($form['group']['#value']->gid) ? $form['group']['#value']->gid : 0;
  $roles = og_roles($gid);

  foreach (element_children($form['permission']) as $key) {
    $row = array();
    // Module name.
    if (is_numeric($key)) {
      $row[] = array('data' => drupal_render($form['permission'][$key]), 'class' => array('module'), 'id' => 'module-' . $form['permission'][$key]['#id'], 'colspan' => count($form['role_names']['#value']) + 1);
    }
    else {
      // Permission row.
      $row[] = array(
        'data' => drupal_render($form['permission'][$key]),
        'class' => array('permission'),
      );
      foreach (element_children($form['checkboxes']) as $rid) {
        $row[] = array('data' => drupal_render($form['checkboxes'][$rid][$key]), 'class' => array('checkbox'), 'title' => $roles[$rid] . ' : ' . t($key));
      }
    }
    $rows[] = $row;
  }
  $header[] = (t('Permission'));
  foreach (element_children($form['role_names']) as $rid) {
    $header[] = array('data' => drupal_render($form['role_names'][$rid]), 'class' => array('checkbox'));
  }
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'permissions')));
  $output .= drupal_render_children($form);
  return $output;
}


/**
 * Groups permissions on default settings form.
 *
 * Allow site admin to add or remove group fields from fieldable enteies.
 */
function og_ui_field_settings($form, &$form_state) {
  $form = array();
  $options = array();

  $options = array();
  foreach (entity_get_info() as $entity_type => $entity) {
    if (!empty($entity['fieldable']) && $entity_type != 'group') {
      foreach($entity['bundles'] as $bundle_name => $bundle) {
        // Prefix the bundle name with the entity type.
        $options[$entity_type][$entity_type . '__' .$bundle_name] = check_plain($bundle['label']);
      }
    }
  }

  $form['bundles'] = array(
    '#title' => t('Bundles'),
    '#type' => 'select',
    '#options' => $options,
  );

  $options = array();
  foreach (og_fields_info() as $field_name => $field) {
    foreach ($field['type'] as $type) {
      $options[$type][$field_name] = filter_xss($field['instance']['label']);
    }
  }

  $form['fields'] = array(
    '#title' => t('Fields'),
    '#type' => 'select',
    '#options' => $options,
  );

  $field_enabled = array();

  $group_fields = og_fields_info();

  $group_fields_name = array_keys($group_fields);

  $entity_info = entity_get_info();

  // Get the fields that exist in the bundle.
  foreach (field_info_fields() as $field_name => $field) {
    if (in_array($field_name, $group_fields_name) && !empty($field['bundles'])) {
      foreach ($field['bundles'] as $entity_type => $bundles) {
        foreach ($bundles as $bundle) {
          $field_enabled[$entity_type][$bundle][] = $field_name;
        }
      }
    }
  }

  if ($field_enabled) {
    $form['group_fields'] = array(
      '#type' => 'vertical_tabs',
      '#weight' => 99,
    );

    // Show all the group fields of each bundle.
    foreach ($field_enabled as $entity_type => $bundles) {
      foreach ($bundles as $bundle => $fields) {
        $options = array();
        $bundles = field_info_bundles($entity_type);
        $form['group_fields_' . $entity_type . '_' . $bundle] = array(
          '#type' => 'fieldset',
          '#title' => t('@bundle - @entity entity', array('@bundle' => $bundles[$bundle]['label'], '@entity' => $entity_info[$entity_type]['label'])),
          '#collapsible' => TRUE,
          '#group' => 'group_fields',
        );
        foreach ($fields as $field_name) {
          $options[] = array(
            check_plain($group_fields[$field_name]['instance']['label']),
            filter_xss($group_fields[$field_name]['description']),
            l(t('Delete'), "admin/config/group/fields/$entity_type/$bundle/$field_name/delete"),
          );
        }

        $header = array(t('Field'), t('Description'), t('Operations'));
        $form['group_fields_' . $entity_type . '_' . $bundle]['fields'] = array(
          '#markup' => theme('table', array('header' => $header, 'rows' => $options)),
        );

      }
    }
  }
  else {
    $form['group_fields'] = array(
      '#markup' => t('There are no Group fields attached to any bundle yet.'),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add field'),
  );

  return $form;
}

function og_ui_field_settings_validate($form, &$form_state) {
  $bundle = preg_replace('/^.*__/i', '', $form_state['values']['bundles']);
  $entity_type = str_replace('__' . $bundle, '', $form_state['values']['bundles']);

  // Check if field can be attached to entity.
  $group_field = og_fields_info($form_state['values']['fields']);
  $bundles = field_info_bundles($entity_type);
  $entity_info = entity_get_info($entity_type);

  $params = array(
    '%field' => $group_field['instance']['label'],
    '%bundle' => $bundles[$bundle]['label'],
    '%entity' => $entity_info['label'],
  );

  if ($form_state['values']['fields'] == OG_GROUP_FIELD && $entity_type == 'group') {
    form_set_error('bundles', t('It is not allowed to add the group field to the group entity.'));
  }

  // Check if field doesn't exist already.
  if (($field = field_info_field($form_state['values']['fields'])) && !empty($field['bundles'][$entity_type]) && in_array($bundle, drupal_map_assoc($field['bundles'][$entity_type]))) {
    form_set_error('bundles', t('Field %field already exists in %bundle.', $params));
  }

  // Check field can be attached to entity type.
  if (!empty($group_field['entity']) && !in_array($entity_type, $group_field['entity'])) {
    $items = array();
    foreach ($group_field['entity'] as $entity_type) {
      $info = entity_get_info($entity_type);
      $items[] = $info['label'];
    }
    form_set_error('bundles', t('Field %field can only be attached to %entities entity bundles.', $params + array('%entities' => implode(', ', $items))));
  }

}

function og_ui_field_settings_submit($form, &$form_state) {
  $bundle = preg_replace('/^.*__/i', '', $form_state['values']['bundles']);
  $entity_type = str_replace('__' . $bundle, '', $form_state['values']['bundles']);
  $field_name = $form_state['values']['fields'];

  og_create_field($field_name, $entity_type, $bundle);

  $group_field = og_fields_info($field_name);
  $bundles = field_info_bundles($entity_type);

  $params = array(
    '%field' => $group_field['instance']['label'],
    '%bundle' => $bundles[$bundle]['label'],
  );

  drupal_set_message(t('Added field %field to %bundle.', $params));
}


/**
 * Menu callback; present a form for removing a field instance from a bundle.
 */
function og_ui_field_delete_form($form, &$form_state, $instance) {
  module_load_include('inc', 'field_ui', 'field_ui.admin');
  $output = field_ui_field_delete_form($form, $form_state, $instance);

  $output['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/config/group/fields'),
  );

  return $output;
}


function og_ui_field_delete_form_submit($form, &$form_state) {
  module_load_include('inc', 'field_ui', 'field_ui.admin');
  field_ui_field_delete_form_submit($form, $form_state);
  $form_state['redirect'] = 'admin/config/group/fields';
}
