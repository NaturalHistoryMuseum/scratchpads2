<?php
/**
 * @file
 * Implementation for Fields and Linkit.
 */


/**
 * Return a list of allowed elements.
 * @TODO: Make this a hook.
 */
function linkit_allowed_elements() {
  $allowed_elements = array(
    'textfield',
    'textarea',
    'link_field',
  );
  drupal_alter('linkit_allowed_elements', $allowed_elements);
  return $allowed_elements;
}

/**
 * Return a list of allowed field types.
 * @TODO: Make this a hook.
 */
function linkit_allowed_field_types() {
  $allowed_field_types = array(
    'text',
    'text_long',
    'link_field',
  );
  drupal_alter('linkit_allowed_field_types', $allowed_field_types);
  return $allowed_field_types;
}

/**
 * Return a list of allowed widget types.
 * @TODO: Make this a hook.
 */
function linkit_allowed_widget_types() {
  $allowed_widget_types = array(
    'text_textfield',
    'text_textarea',
    'link_field',
  );
  drupal_alter('linkit_allowed_widget_types', $allowed_widget_types);
  return $allowed_widget_types;
}

/**
 * Process callback.
 */
function linkit_process_widget(&$element) {
  if (!isset($element['#entity_type'])) {
    return $element;
  }

  $field = field_info_field($element['#field_name']);
  $instance = field_info_instance($element['#entity_type'], $element['#field_name'], $element['#bundle']);

  if (isset($instance['settings']['linkit']['enable']) && $instance['settings']['linkit']['enable']) {
    // Add base settings.
    _linkit_add_settings('field');

    ctools_include('plugins');
    // Try to load the insert plugin we have chosen to use.
    $plugin = ctools_get_plugins('linkit', 'linkit_insert', $instance['settings']['linkit']['insert_plugin']);

    // Set the field ID.
    $field_id = $element['#id'];
    // Special treatment for link fields.
    if ($element['#type'] == 'link_field') {
      $field_id = $element['#id'] . '-url';
    }

    $field_js = array(
      'data' => array(
        'linkit' => array(
          'fields' => array(
            $field_id => array(
              'insert_plugin' => $instance['settings']['linkit']['insert_plugin'],
            ),
          ),
        ),
      ),
      'type' => 'setting',
    );

    // Spcial settings for link fields.
    if ($element['#type'] == 'link_field') {
      $field_js['data']['linkit']['fields'][$field_id]['no_slash'] = TRUE;
      // Link fields can have a title field.
      if (isset($instance['settings']['title']) && in_array($instance['settings']['title'], array('optional', 'required'))) {
        $field_js['data']['linkit']['fields'][$field_id]['title_field'] = $element['#id'] . '-title';
      }
    }

    // Attach js files and settings Linkit needs.
    $element += array(
      '#attached' => array(
        'js' => array(
          $plugin['javascript'],
          $field_js,
        ),
      ),
    );

    // Add fake button to the element suffix.
    $element['#field_suffix'] = '<a class="button linkit-field-button linkit-field-' . $field_id . '" href="#">' . t('Search') . '</a>';
  }

  return $element;
}

/**
 * Implements hook_form_FIELD_UI_FIELD_EDIT_FORM_alter().
 */
function linkit_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  $instance = $form['#instance'];

  // Get allowed field and widget types.
  $allowed_field_types = linkit_allowed_field_types();
  $allowed_field = in_array($form['#field']['type'], $allowed_field_types);
  $allowed_widget_types = linkit_allowed_widget_types();
  $allowed_widget = in_array($form['instance']['widget']['type']['#value'], $allowed_widget_types);

  // Add the linkit settings to the field instance form.
  if ($allowed_field  && $allowed_widget) {

    // Fieldset for Linkit settings on this field instance.
    $form['instance']['settings']['linkit'] = array(
      '#type' => 'fieldset',
      '#title' => t('Linkit field settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    // Enable Linkit on this field instance.
    $form['instance']['settings']['linkit']['enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Linkit support.'),
      '#default_value' => isset($instance['settings']['linkit']['enable'])
        ? $instance['settings']['linkit']['enable'] : 0,
      '#description' => t('Do not use this for CKeditor and TinyMCE fields.'),
    );

    ctools_include('plugins');
    // Load all insert plugins.
    $plugins = ctools_get_plugins('linkit', 'linkit_insert');
    foreach ($plugins as $name => $plugin) {
      $options[$name] = $plugin['name'];
    }

    // Settings for input plugins.
    $form['instance']['settings']['linkit']['insert_plugin'] = array(
      '#type' => 'select',
      '#title' => t('Insert plugin'),
      '#options' => $options,
      '#empty_option' => t('- Select an insert plugin -'),
      '#default_value' => isset($instance['settings']['linkit']['insert_plugin'])
        ? $instance['settings']['linkit']['insert_plugin'] : '',
      '#states' => array(
        'invisible' => array(
          'input[name="instance[settings][linkit][enable]"]' => array('checked' => FALSE),
        ),
        'required' => array(
          'input[name="instance[settings][linkit][enable]"]' => array('checked' => TRUE),
        ),
      ),
      '#element_validate' => array('linkit_field_insert_pluing_validate'),
    );
  }
}

/**
 * Validation callback; From The insert plugin field on fields.
 *
 * Only validate this field if linkit is enabled on the instance.
 */
function linkit_field_insert_pluing_validate($element, &$form_state, $form) {
  if (isset($form_state['values']['instance']['settings']['linkit']['enable'])
    && $form_state['values']['instance']['settings']['linkit']['enable']) {
    if (empty($element['#value'])) {
      form_error($element, t('You must select an insert plugin.'));
    }
  }
}