<?php

/**
 * @file
 *   Contains administration forms.
 */

/**
 * Administration form for each bundle.
 */
function auto_entitylabel_settings_form($form, $form_state, $entity_type, $bundle_arg) {
  $info = entity_get_info($entity_type);
  $bundle_name = is_object($bundle_arg) ? $bundle_arg->{$info['bundle keys']['bundle']} : $bundle_arg;
  $bundle_label = $info['bundles'][$bundle_name]['label'];
  $key = $entity_type . '_' . $bundle_name;
  $default_value = auto_entitylabel_get_setting($key);
  $form['auto_entitylabel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic label generation for @type', array('@type' => $bundle_label)),
    '#weight' => 0,
    /* '#attached' => array( // Not needed anymore at the moment.
      'js' => array(
        'auto-entitylabel' => drupal_get_path('module', 'auto_entitylabel') . '/auto_entitylabel.js',
      ),
    ),*/
  );
  $form['auto_entitylabel']['auto_entitylabel_' . $key] = array(
    '#type' => 'radios',
    '#default_value' => $default_value,
    '#options' => _auto_entitylabel_options($entity_type, $bundle_name),
  );
  $form['auto_entitylabel']['auto_entitylabel_pattern_' . $key] = array(
    '#type' => 'textarea',
    '#title' => t('Pattern for the title'),
    '#description' => t('Leave blank for using the per default generated title. Otherwise this string will be used as title. Use the syntax [token] if you want to insert a replacement pattern.'),
    '#default_value' => variable_get('auto_entitylabel_pattern_' . $key, ''),
  );
  // Don't allow editing of the pattern if PHP is used, but the users lacks
  // permission for PHP.
  if (variable_get('auto_entitylabel_php_' . $key, '') && !user_access('use PHP for label patterns')) {
    $form['auto_entitylabel']['auto_entitylabel_pattern_' . $key]['#disabled'] = TRUE;
    $form['auto_entitylabel']['auto_entitylabel_pattern_' . $key]['#description'] = t('You are not allow the configure the pattern for the title, as you lack the %permission permission.', array('%permission' => t('Use PHP for title patterns')));
  }
  // Display the list of available placeholders if token module is installed.
  if (module_exists('token')) {
    $form['auto_entitylabel']['token_help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array($info['token type']),
      '#dialog' => TRUE,
    );
  }

  $form['auto_entitylabel']['auto_entitylabel_php_' . $key] = array(
    '#access' => user_access('use PHP for label patterns'),
    '#type' => 'checkbox',
    '#title' => t('Evaluate PHP in pattern.'),
    '#description' => t('Put PHP code above that returns your string, but make sure you surround code in <code>&lt;?php</code> and <code>?&gt;</code>. Note that <code>$entity</code> and <code>$language</code> are available and can be used by your code.'),
    '#default_value' => variable_get('auto_entitylabel_php_' . $key, ''),
  );
  return system_settings_form($form);
}

/**
 * Constructs the list of options for the given bundle.
 */
function _auto_entitylabel_options($entity_type, $bundle_name) {
  $options = array(
    AUTO_ENTITYLABEL_DISABLED => t('Disabled'),
  );
  if (auto_entitylabel_entity_label_visible($entity_type, $bundle_name)) {
    $options += array(
      AUTO_ENTITYLABEL_ENABLED => t('Automatically generate the label and hide the label field'),
      AUTO_ENTITYLABEL_OPTIONAL => t('Automatically generate the label if the label field is left empty'),
    );
  }
  else {
    $options += array(
      AUTO_ENTITYLABEL_ENABLED => t('Automatically generate the label'),
    );
  }
  return $options;
}

/**
 * Check if given entity bundle has a visible label on the entity form.
 *
 * @param $entity_type
 *   The entity type.
 * @param $bundle_name
 *   The name of the bundle.
 *
 * @return
 *   TRUE if the label is rendered in the entity form, FALSE otherwise.
 *
 * @todo
 *   Find a generic way of determining the result of this function. This
 *   will probably require access to more information about entity forms
 *   (entity api module?).
 */
function auto_entitylabel_entity_label_visible($entity_type, $bundle_name) {
  $hidden = array(
    'profile2' => TRUE,
  );
  
  return empty($hidden[$entity_type]);
}
