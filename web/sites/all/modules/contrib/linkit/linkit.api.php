<?php
/**
 * @file
 * Hooks and alters provided by Linkit.
 */

/**
 * Extend Linkit with new plugins.
 *
 * Since 7.x-2.2 Linkit uses ctools for the plugins.
 * Linkit supports all entities that have an "uri callback" defined out of the
 * box. See "hook_entity_info" for more info.
 *
 * If you would like to extend the methods in the entity plugin, you can extands
 * the regular LinkitPluginEntity class. See the linkit-plugin-node.class.php as
 * an example.
 */

/**
 * If you will implement new Linkit plugins, you'll need to tell ctools that you
 * have linkit plugins. This is done by using hook_ctools_plugin_directory.
 * See the ctools documentation for for more info about this hook.
 */
function hook_ctools_plugin_directory($module, $plugin) {
  if ($module == 'linkit' && $plugin == 'linkit_plugin') {
    return 'plugins/linkit_plugin';
  }
}


/**
 * Defines one or more attributes to use with Linkit.
 *
 * All attributes is defined as form elements, and it used both in the Linkit
 * profile form and in the Linkit dashboard.
 *
 * See Drupal FAPI for more info.
 * http://api.drupal.org/api/drupal/developer--topics--forms_api_reference.html
 *
 * @param array $profile
 *   The profile settings the user calling this function has.
 * @return
 *   An associative array with form elements with the key being the HTML
 *   attribute name (my_attribute = <a my_attribute="value"></a>).
 */
function hook_linkit_attributes($profile) {
  $attributes['my_attribute'] = array(
    '#type' => 'textfield',
    '#title' => t('My attribute'),
    '#maxlength' => 255,
    '#size' => 40,
    '#default_value' => '',
    '#weight' => isset($profile->data['attributes']['my_attribute']['weight']) ? $profile->data['attributes']['my_attribute']['weight'] : 0,
  );

  return $attributes;
}

/**
 * Alter an attribute before it has been processed.
 *
 * This hook is useful for altering the attribute form array that will be used
 * in both the Linkit profile form and in the Linkit dashboard.
 *
 * @param $attributes
 *   An associative array with form elements defining attributes.
 *
 * @see hook_linkit_attributes()
 */
function hook_linkit_attributes_alter(&$attributes) {
  $attributes['rel']['#type'] = 'select';
  $attributes['rel']['#title'] = t('Rel select');
  $attributes['rel']['#options'] = array(
    '' => t('None'),
    'now-follow' => t('No follow'),
    'other-rel' => t('Other rel'),
  );
}
