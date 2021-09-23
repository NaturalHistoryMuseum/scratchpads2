<?php

/**
 * Callback to provide the subpermissions form
 * 
 * This is pretty much a straight copy from the user.admin.inc file, but with
 * the addition of the check against our own config settings.
 */
function subpermissions_admin_permissions($form, $form_state){
  // Retrieve role names for columns.
  $role_names = user_roles();
  // Fetch permissions for all roles or the one selected role.
  $role_permissions = user_role_permissions($role_names);
  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names
  );
  // Render role/permission overview:
  $options = array();
  $module_info = system_get_info('module');
  $hide_descriptions = system_admin_compact_mode();
  // Get a list of all the modules implementing a hook_permission() and sort by
  // display name.
  $modules = array();
  foreach(module_implements('permission') as $module){
    $modules[$module] = $module_info[$module]['name'];
  }
  asort($modules);
  // Get our permissions
  $subset = variable_get('subpermissions_config', array());
  if(count($subset)){
    foreach($modules as $module => $display_name){
      if($permissions = module_invoke($module, 'permission')){
        $form['permission'][] = array(
          '#markup' => $module_info[$module]['name'],
          '#id' => $module
        );
        $set_a_permission = FALSE;
        foreach($permissions as $perm => $perm_item){
          if(isset($subset[$perm]) && $subset[$perm]){
            // Fill in default values for the permission.
            $perm_item += array(
              'description' => '',
              'restrict access' => FALSE
            );
            if($perm_item['restrict access']){
              continue;
            }
            $set_a_permission = TRUE;
            $options[$perm] = '';
            $form['permission'][$perm] = array(
              '#type' => 'item',
              '#markup' => $perm_item['title'],
              '#description' => theme('user_permission_description', array(
                'permission_item' => $perm_item,
                'hide' => $hide_descriptions
              ))
            );
            foreach($role_names as $rid => $name){
              // Builds arrays for checked boxes for each role
              if(isset($role_permissions[$rid][$perm])){
                $status[$rid][] = $perm;
              }
            }
          }
        }
        if(!$set_a_permission){
          // There are no alterable permissions for this module, we should unset
          // the markup
          array_pop($form['permission']);
        }
      }
    }
    // Have to build checkboxes here after checkbox arrays are built
    foreach($role_names as $rid => $name){
      $form['checkboxes'][$rid] = array(
        '#type' => 'checkboxes',
        '#options' => $options,
        '#default_value' => isset($status[$rid]) ? $status[$rid] : array()
      );
      $form['role_names'][$rid] = array(
        '#markup' => check_plain($name),
        '#tree' => TRUE
      );
    }
    $form['actions'] = array(
      '#type' => 'actions'
    );
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save permissions')
    );
    $form['#attached']['js'][] = drupal_get_path('module', 'user') . '/user.permissions.js';
    $form['#theme'] = array(
      'user_admin_permissions'
    );
    $form['#submit'] = array(
      'user_admin_permissions_submit'
    );
  }else{
    drupal_set_message(t('There are currently no permissions available for you to set.'));
  }
  return $form;
}

/**
 * Theme function
 */
function theme_subpermissions_config_form($variables){
  foreach(element_children($variables['form']['permission']) as $key){
    $row = array();
    // Module name
    if(is_numeric($key)){
      $row[] = array(
        'data' => drupal_render($variables['form']['permission'][$key]),
        'class' => array(
          'module'
        ),
        'id' => 'module-' . $variables['form']['permission'][$key]['#id'],
        'colspan' => count($variables['form']['role_names']['#value']) + 1
      );
    }else{
      // Permission row.
      $row[] = array(
        'data' => drupal_render($variables['form']['permission'][$key]),
        'class' => array(
          'permission'
        )
      );
      $variables['form']['checkboxes'][0][$key]['#title'] = 'Add to Permissions subset: ' . $variables['form']['permission'][$key]['#markup'];
      $variables['form']['checkboxes'][0][$key]['#title_display'] = 'invisible';
      $row[] = array(
        'data' => drupal_render($variables['form']['checkboxes'][0][$key]),
        'class' => array(
          'checkbox'
        )
      );
    }
    $rows[] = $row;
  }
  $header = array(
    t('Permission'),
    t('Add to Permissions subset')
  );
  $output = theme('system_compact_link');
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(
      'id' => 'permissions'
    )
  ));
  return $output .= drupal_render_children($variables['form']);
}

/**
 * Subpermissions setting form submit
 */
function subpermissions_config_form_submit($form, &$form_state){
  variable_set('subpermissions_config', $form_state['values'][0]);
}

/**
 * Subpermissions settings form
 */
function subpermissions_config_form($form, $form_state){
  $role_names = array(
    0 => "Add to Permissions subset"
  );
  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names
  );
  // Render role/permission overview:
  $options = array();
  $module_info = system_get_info('module');
  $hide_descriptions = system_admin_compact_mode();
  // Get a list of all the modules implementing a hook_permission() and sort by
  // display name.
  $modules = array();
  foreach(module_implements('permission') as $module){
    $modules[$module] = $module_info[$module]['name'];
  }
  asort($modules);
  foreach($modules as $module => $display_name){
    if($permissions = module_invoke($module, 'permission')){
      $form['permission'][] = array(
        '#markup' => $module_info[$module]['name'],
        '#id' => $module
      );
      $set_a_permission = FALSE;
      foreach($permissions as $perm => $perm_item){
        // Fill in default values for the permission.
        $perm_item += array(
          'description' => '',
          'restrict access' => FALSE
        );
        if($perm_item['restrict access']){
          continue;
        }
        $set_a_permission = TRUE;
        $options[$perm] = '';
        $form['permission'][$perm] = array(
          '#type' => 'item',
          '#markup' => $perm_item['title'],
          '#description' => theme('user_permission_description', array(
            'permission_item' => $perm_item,
            'hide' => $hide_descriptions
          ))
        );
      }
      if(!$set_a_permission){
        // There are no alterable permissions for this module, we should unset
        // the markup
        array_pop($form['permission']);
      }
    }
  }
  // Have to build checkboxes here after checkbox arrays are built
  $form['checkboxes'][0] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('subpermissions_config', array())
  );
  $form['role_names'][0] = array(
    '#markup' => $role_names[0],
    '#tree' => TRUE
  );
  $form['actions'] = array(
    '#type' => 'actions'
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save permissions')
  );
  $form['#attached']['js'][] = drupal_get_path('module', 'user') . '/user.permissions.js';
  return $form;
}