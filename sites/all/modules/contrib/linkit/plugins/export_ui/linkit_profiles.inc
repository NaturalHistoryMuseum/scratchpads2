<?php
/**
 * @file
 * Ctools export ui plugin to handle Linkit profiles.
 */

$plugin = array(
  'schema' => 'linkit_profiles',
  'access' => 'administer linkit',

  'menu' => array(
    'menu prefix' => 'admin/config/content',
    'menu item' => 'linkit',
    'menu title' => 'Linkit Profiles',
    'menu description' => t('Manage internal linking and control what results are shown in the autocomplete search.'),
  ),

  'title singular' => t('Linkit profile'),
  'title plural' => t('Linkit profiles'),
  'title singular proper' => t('Linkit profile'),
  'title plural proper' => t('Linkit profiles'),

  'form' => array(
     'settings' => 'linkit_profiles_export_ui_form',
     'submit' => 'linkit_profiles_export_ui_form_submit',
  ),

  'handler' => 'linkit_profiles_ui',
);

/**
 * Generate a profile form.
 */
function linkit_profiles_export_ui_form(&$form, &$form_state) {
  // Get all plugins.
  $plugins = linkit_get_plugins();

  $profile = $form_state['item'];

  $form['#theme'] = 'linkit_profiles_export_ui_form';
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'linkit') . '/css/linkit.css',
  );

  $form['data']['#tree'] = TRUE;
  $form['info']['#weight'] = -100;

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $profile->weight,
    '#delta' => 10,
    '#weight' => -98,
    '#description' => t('If the user has multiple roles, the profile with lowest weight will then be assigned to the user. In general, the most permissive profile should be assigned the lowest weight.'),
  );

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles with access'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#weight' => -97,
    '#description' => t('Roles which may use this profile. Note that if <em>authenticated user</em> is ticked, all roles (except anonymous) will have permission to this profile.'),
  );

  // Append role form elements to the settings form.
  _linkit_build_role_associate_form_fields($form, $profile);

  // Append plugin form element to the setttings form.
  _linkit_build_plugin_form_fields($form, $profile, $plugins);
  foreach ($plugins as $plugin) {

    // Get an instance og the handler for this plugin.
    $handler = linkit_get_plugin_handler($plugin, $profile);
    $plugin_form = $handler->buildSettingsForm();

    if ($plugin_form) {
      // Add the handler settings form to the generic.
      $form['data'] += $plugin_form;
    }
  }

  if (module_invoke('imce', 'access')) {
    $form['data']['imce'] = array(
      '#type' => 'fieldset',
      '#title' => t('IMCE file picking'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['data']['imce']['use_imce'] = array(
      '#title' => t('Enable IMCE file picking'),
      '#type' => 'checkbox',
      '#default_value' => isset($profile->data['imce']['use_imce']) ? $profile->data['imce']['use_imce'] : '',
      '#description' => t('Note that only public files are supported.'),
    );
  }

  $form['data']['autocomplete'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autocomplete options'),
    '#description' => t('Linkit uses !bac which may be configured with focus on performance.', array('!bac' => l('Better Autocomplete', 'https://github.com/betamos/Better-Autocomplete'))),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#weight' => 100,
  );

  $form['data']['autocomplete']['charLimit'] = array(
    '#title' => t('Character limit'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['autocomplete']['charLimit']) ? $profile->data['autocomplete']['charLimit'] : '3',
    '#size' => 5,
    '#description' => t('Minimum number of characters to perform server polling. A higher value reduces server load. <em>Default: 3</em>'),
    '#element_validate' => array('linkit_isnumeric_validate'),
  );

  $form['data']['autocomplete']['wait'] = array(
    '#title' => t('Key press delay'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['autocomplete']['wait']) ? $profile->data['autocomplete']['wait'] : '350',
    '#size' => 5,
    '#description' => t('Time from last key press to poll the server. A higher value reduces server load. <em>Default: 350</em>'),
    '#field_suffix' => t('ms'),
    '#element_validate' => array('linkit_isnumeric_validate'),
  );

  $form['data']['autocomplete']['remoteTimeout'] = array(
    '#title' => t('Remote timeout'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['autocomplete']['remoteTimeout']) ? $profile->data['autocomplete']['remoteTimeout'] : '10000',
    '#size' => 5,
    '#description' => t('Client side timeout for a request to the server. <em>Default: 10000</em>'),
    '#field_suffix' => t('ms'),
    '#element_validate' => array('linkit_isnumeric_validate'),
  );

  // Append attributes form element to the setttings form.
  _linkit_build_attribute_form_fields($form, $profile);
}

/**
 * Element validate callback for fields that should be numeric.
 */
function linkit_isnumeric_validate($element, &$form_state, $form) {
  if (!empty($element['#value']) && !is_numeric($element['#value'])) {
    form_error($element, $element['#title'] . ' should only contains numbers.');
  }

  // Alter the value by typecasting into an integer.
  $form_element = &$form_state['values'];
  foreach ($element['#array_parents'] as $current) {
    $form_element = &$form_element[$current];
  }
  $form_element = (int) $form_element;
}

/**
 * Sort plugins by weight.
 */
function _linkit_sort_plugins_by_weight($a, $b) {
  return $a["weight"]['#default_value'] >= $b["weight"]['#default_value'];
}

/**
 * Sort attribute by weight.
 */
function _linkit_sort_attributes_by_weight($a, $b) {
  return $a["#weight"] >= $b["#weight"];
}

/**
 * Append plugin form element to the setttings form.
 */
function _linkit_build_plugin_form_fields(&$form, $profile, $plugins) {
  $form['data']['plugins_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Linkit plugins'),
    '#description' => t('Linkit is all about the plugins. They define what content Linkit will present in the autocomplete search field.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
    '#theme' => '_linkit_theme_profile_form_table',
  );

  $form['data']['plugins_fieldset']['plugins'] = array(
    '#markup' => '',
    '#tree' => TRUE,
    '#parents' => array('data', 'plugins'),
  );

  // Used to store plugin form elements temporary so we can use this to sort by weight.
  $tmp_plugins = array();

  foreach ($plugins AS $name => $plugin) {
    // Get the plugin handler.
    $handler = linkit_get_plugin_handler($plugin, $profile);

    $tmp_plugins[$name]['name'] = array('#markup' => $handler->ui_title());
    $tmp_plugins[$name]['description'] = array('#markup' => $handler->ui_description());
    $tmp_plugins[$name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $handler->ui_title())),
      '#title_display' => 'invisible',
      '#default_value' => isset($profile->data['plugins'][$name]['weight']) ? $profile->data['plugins'][$name]['weight'] : LINKIT_DEFAULT_WEIGHT,
    );
    $tmp_plugins[$name]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable @title', array('@title' => $handler->ui_title())),
      '#title_display' => 'invisible',
    );
    if (isset($profile->data['plugins'])) {
      $tmp_plugins[$name]['enabled']['#default_value'] = isset($profile->data['plugins'][$name]['enabled']) ? $profile->data['plugins'][$name]['enabled'] : FALSE;
    }
    // Default enabled plugins
    elseif (in_array($name, array('node', 'external', 'file'))) {
      $tmp_plugins[$name]['enabled']['#default_value'] = TRUE;
    }
  }

  // Sort by #weight.
  uasort($tmp_plugins, '_linkit_sort_plugins_by_weight');

  foreach ($tmp_plugins AS $name => $plugin) {
    $form['data']['plugins_fieldset']['plugins'][$name] = $plugin;
  }

}

/**
 * Append attributes form element to the setttings form.
 */
function _linkit_build_attribute_form_fields(&$form, $profile) {

  $form['data']['attributes_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML Attributes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
  );

  $form['data']['attributes_fieldset']['attributes'] = array(
    '#markup' => '',
    '#tree' => TRUE,
    '#parents' => array('data', 'attributes'),

  );

  $attrs = linkit_get_attributes($profile);

  // Sort by #weight.
  uasort($attrs, '_linkit_sort_attributes_by_weight');

  foreach ($attrs as $name => $attr) {
    $form['data']['attributes_fieldset']['attributes'][$name]['name'] = array('#markup' => check_plain($attr['#title']));

    $form['data']['attributes_fieldset']['attributes'][$name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $attr['#title'])),
      '#title_display' => 'invisible',
      '#default_value' => isset($profile->data['attributes'][$name]['weight']) ? $profile->data['attributes'][$name]['weight'] : LINKIT_DEFAULT_WEIGHT,
    );

    $form['data']['attributes_fieldset']['attributes'][$name]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable @title', array('@title' => $attr['#title'])),
      '#title_display' => 'invisible',
      '#default_value' => isset($profile->data['attributes'][$name]['enabled']) ? $profile->data['attributes'][$name]['enabled'] : FALSE,
    );
  }
}

/**
 * Append role form elements to the settings form.
 */
function _linkit_build_role_associate_form_fields(&$form, $profile) {
  $roles = user_roles();
  $assiged_roles = _linkit_get_associated_roles($profile);

  foreach ($roles as $role_id => $role_name) {
    $form['roles'][$role_id] = array(
      '#type' => 'checkbox',
      '#title' => $role_name,
      '#default_value' => isset($profile->role_rids[$role_id]) ? $profile->role_rids[$role_id] : FALSE,
    );

    // Disable roles thats already associated.
    if (in_array($role_id, array_keys($assiged_roles))) {
      $form['roles'][$role_id]['#disabled'] = TRUE;
      $form['roles'][$role_id]['#description'] = t('This role is already in use by the %name profile', array('%name' => $assiged_roles[$role_id]));
    }

  }

}

/**
 * Save callback for linkit_profiles_export_ui_form.
 *
 * @param array $form
 * @param array $form_state
 */
function linkit_profiles_export_ui_form_submit(&$form, &$form_state) {

  $form_state['item']->admin_title = trim($form_state['values']['admin_title']);
  $form_state['item']->name = $form_state['values']['name'];

  $form_state['item']->data = $form_state['values']['data'];
  $form_state['item']->role_rids = array_filter($form_state['values']['roles']);
  $form_state['item']->weight = $form_state['values']['weight'];

  // CTools will work on this variable alter and screw it up, so unset it.
  unset($form_state['values']['data']);
}

/**
 * Get all roles that is associated to profiles and make an array of them.
 *
 * @param object The profile object.
 */
function _linkit_get_associated_roles($profile) {
  $profiles = linkit_profile_load_all();
  // Unset the profile being edited.
  unset($profiles[$profile->name]);
  $roles = array();

  foreach ($profiles as $profile) {
    foreach ($profile->role_rids as $rid => $value) {
      $role = user_role_load($rid);
      $roles[$rid] = $profile->admin_title;
    }
  }

  return $roles;
}

/**
 * Form for reorder profiles.
 */
function linkit_profiles_reorder() {
  $profiles = linkit_profile_load_all();
  $form = array();
  $form['#tree'] = TRUE;
  // Sort by weight.
  uasort($profiles, '_linkit_sort_profiles_by_weight');

  foreach ($profiles as $profile_machine_name => $profile) {
    $form['profiles'][$profile_machine_name] = array(
      'name' => array('#markup' => $profile->admin_title),
      'roles' => array('#markup' => linkit_profiles_ui::buildRoleList($profile->role_rids)),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight for @title', array('@title' => $profile->admin_title)),
        '#title_display' => 'invisible',
        '#default_value' => isset($profile->weight) ? $profile->weight : LINKIT_DEFAULT_WEIGHT,
        '#attributes' => array(
          'class' => array('weight'),
        )
      )
    );
  }

  if (!$profiles) {
    $form['helptext'] = array('#markup' => t('No profiles found.'));
  }
  else {
    $form['helptext'] = array('#markup' => t('If a user have multiple roles, the profile weight will determine which profile to use. Lower weight will take precedence.'));

    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  return $form;
}

/**
 * Save function for the reorder form.
 */
function linkit_profiles_reorder_submit($form, &$form_state) {
  foreach ($form_state['values']['profiles'] as $profile_machine_name => $value) {
    $profile = linkit_profile_load($profile_machine_name);
    $profile->weight = $value['weight'];
    linkit_profile_save($profile);
  }
}
