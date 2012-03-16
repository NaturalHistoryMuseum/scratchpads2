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
  $profile = $form_state['item'];

  $form = array();
  $form['#theme'] = 'linkit_profiles_export_ui_form';
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'linkit') . '/css/linkit.css',
  );

  $form['data']['#tree'] = TRUE;

  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($profile->admin_title) ? $profile->admin_title : t('Untitled profile'),
    '#required' => TRUE,
    '#weight' => -100,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#default_value' => isset($profile->name) ? $profile->name : '',
    '#maxlength' => 255,
    '#machine_name' => array(
      'exists' => 'linkit_profile_exists',
      'source' => array('admin_title'),
    ),
    '#disabled' => !is_null($profile->name),
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $profile->weight,
    '#delta' => 10,
    '#weight' => -98,
    '#description' => t('If the user has multiple roles, the profile with lowest weight will then be assigned to the user. ' .
    										'In general, the most permissive profile should be assigned the lowest weight.'),
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
  _linkit_build_plugin_form_fields($form, $profile);

  $form['data']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node plugin'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#states' => array(
      'invisible' => array(
        'input[name="plugins[node][enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['data']['node']['result_description'] = array(
    '#title' => t('Result format'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['node']['result_description']) ? $profile->data['node']['result_description'] : 'by [node:author] · [node:created:short]',
    '#size' => 120,
    '#maxlength' => 255,
    '#description' => t('Available tokens: %tokens.', array('%tokens' => '[node:nid], [node:vid], [node:tnid], [node:type], [node:type-name], [node:title], [node:language], [node:created], [node:changed], [node:author]')),
  );

  $form['data']['node']['content_types'] = array(
    '#title' => t('Content type filter'),
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => isset($profile->data['node']['content_types']) ? $profile->data['node']['content_types'] : array(),
    '#description' => t('If left blank, all content types will appear in autocomplete results.'),
  );

  $form['data']['node']['include_unpublished'] = array(
    '#title' => t('Include unpublished nodes'),
    '#type' => 'checkbox',
    '#default_value' => isset($profile->data['node']['include_unpublished']) ? $profile->data['node']['include_unpublished'] : 0,
  );

  $form['data']['node']['group_by_content_type'] = array(
    '#title' => t('Group nodes by content type'),
    '#type' => 'checkbox',
    '#default_value' => isset($profile->data['node']['group_by_content_type']) ? $profile->data['node']['group_by_content_type'] : 1,
  );

  $form['data']['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User plugin'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#states' => array(
      'invisible' => array(
        'input[name="plugins[user][enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['data']['user']['result_description'] = array(
    '#title' => t('Result format'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['user']['result_description']) ? $profile->data['user']['result_description'] : '',
    '#size' => 120,
    '#maxlength' => 255,
    '#description' => t('Available tokens: %tokens.', array('%tokens' => '[user:uid], [user:name], [user:mail], [user:last-login], [user:created]')),
  );

  $form['data']['taxonomy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy plugin'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#states' => array(
      'invisible' => array(
        'input[name="plugins[taxonomy][enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['data']['taxonomy']['result_description'] = array(
    '#title' => t('Result format'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['taxonomy']['result_description']) ? $profile->data['taxonomy']['result_description'] : '[term:vocabulary]',
    '#size' => 120,
    '#maxlength' => 255,
    '#description' => t('Available tokens: %tokens.', array('%tokens' => '[term:tid], [term:name], [term:vocabulary], [term:parent]')),
  );

  $form['data']['taxonomy']['group_by_vocabulary'] = array(
    '#title' => t('Group terms by vocabulary'),
    '#type' => 'checkbox',
    '#default_value' => isset($profile->data['taxonomy']['group_by_vocabulary']) ? $profile->data['taxonomy']['group_by_vocabulary'] : 0,
  );

  $form['data']['file'] = array(
    '#type' => 'fieldset',
    '#title' => t('File plugin'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
    '#states' => array(
      'invisible' => array(
        'input[name="plugins[file][enabled]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['data']['file']['result_description'] = array(
    '#title' => t('Result format'),
    '#type' => 'textfield',
    '#default_value' => isset($profile->data['file']['result_description']) ? $profile->data['file']['result_description'] : '[file:mime] · [file:size]',
    '#size' => 120,
    '#maxlength' => 255,
    '#description' => t('Available tokens: %tokens.', array('%tokens' => '[file:fid], [file:name], [file:mime], [file:size], [file:timestamp], [file:owner]')),
  );

  $form['data']['file']['show_scheme'] = array(
    '#title' => t('Show file scheme'),
    '#type' => 'checkbox',
    '#default_value' => isset($profile->data['file']['show_scheme']) ? $profile->data['file']['show_scheme'] : array()
  );

  $form['data']['file']['group_by_scheme'] = array(
    '#title' => t('Group files by scheme'),
    '#type' => 'checkbox',
    '#default_value' => isset($profile->data['file']['group_by_scheme']) ? $profile->data['file']['group_by_scheme'] : array(),
  );

  $image_extra_info_options = array(
    // Coder is saying "$text argument to l() should be enclosed within t() so that it is translatable." but image styles is not translatable.
    'thumbnail' => t('Show thumbnails <em>(using the image style !linkit_thumb_link)</em>', array('!linkit_thumb_link' => l('linkit_thumb', 'admin/config/media/image-styles/edit/linkit_thumb'))),
    'dimensions' => t('Show pixel dimensions'),
  );

  $form['data']['file']['image_extra_info'] = array(
    '#title' => t('Images'),
    '#type' => 'checkboxes',
    '#options' => $image_extra_info_options,
    '#default_value' => isset($profile->data['file']['image_extra_info']) ? $profile->data['file']['image_extra_info'] : array('thumbnail', 'dimensions'),
  );

  if (module_invoke('imce', 'access')) {
    $form['data']['imce'] = array(
      '#type' => 'fieldset',
      '#title' => t('IMCE file picking'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['data']['imce']['use_imce'] = array(
      '#title' => t('Enable !imce file picking', array('!imce' => l(t('IMCE'), 'admin/config/media/imce'))),
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
function _linkit_build_plugin_form_fields(&$form, $profile) {
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

  // Get all plugins.
  $plugins = linkit_get_plugins();

  // Used to store plugin form elements temporary so we can use this to sort by weight.
  $tmp_plugins = array();

  foreach ($plugins AS $name => $plugin) {
    $tmp_plugins[$name]['name'] = array('#markup' => $plugin['title']);
    $tmp_plugins[$name]['description'] = array('#markup' => $plugin['description']);
    $tmp_plugins[$name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $plugin['title'])),
      '#title_display' => 'invisible',
      '#default_value' => isset($profile->data['plugins'][$name]['weight']) ? $profile->data['plugins'][$name]['weight'] : LINKIT_DEFAULT_WEIGHT,
    );
    $tmp_plugins[$name]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable @title', array('@title' => $plugin['title'])),
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
 * Helper function to render settings tables.
 *
 * @param $variables.
 * @param $type
 *   "plugin" or "attribute".
 */
function _linkit_theme_profile_form_table($variables, $type) {
  $rows = array();
  $has_description = FALSE;

  // Build table rows.
  foreach ($variables['linkit_' . $type . '_listing'] as $delta => $element) {
    $fields = array(
      $element->title,
      $element->weight_select,
      $element->enabled
    );

    if (isset($element->description)) {
      $has_description = TRUE;
      $fields[] = $element->description;
    }

    $rows[$delta]['data'] = $fields;
    $rows[$delta]['class'] = array('draggable', 'tabledrag-leaf');
  }

  drupal_add_tabledrag('linkit-' . $type, 'order', 'sibling', 'weight');

  $header = array(
    t('Name'),
    t('Weight'),
    t('Enabled'),
  );

  if ($has_description) {
    $header[] = t('Description');
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'linkit-' . $type), 'sticky' => FALSE));
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

  $form_state['item']->data = serialize($form_state['values']['data']);
  $form_state['item']->role_rids = serialize(array_filter($form_state['values']['roles']));
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

  $form['helptext'] = array('#markup' => t('If a user have multiple roles, the profile weight will determine which profile to use. Lower weight will take precedence.'));

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Save function for the reorder form.
 */
function linkit_profiles_reorder_submit($form, &$form_state) {
  foreach($form_state['values']['profiles'] as $profile_machine_name => $value) {
    $profile = linkit_profile_load($profile_machine_name);
    $profile->weight = $value['weight'];
    linkit_profile_save($profile);
  }
}
