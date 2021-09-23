<?php

/**
 *  @file
 *  Administrative page callbacks for the Styles UI module.
 */

/**
 *  Page callback for various styles overview listing pages.
 */
function styles_ui_containers_overview($field_type) {
  // Get the full list of styles.
  $styles = styles_default_styles();

  // Get the containers for this field type.
  $styles_containers = styles_default_containers();
  $field_containers = $styles_containers[$field_type];

  // Build our table listing of styles.
  $header = array(t('Styles'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();

  foreach ($styles[$field_type]['styles'] as $style_name => $style) {
    // What's the path to edit this style?
    $edit_path = $field_containers['admin']['path'] . '/edit/' . $style_name;
    $row = array(l(t('@style', array('@style' => $style_name)), $edit_path));

    // Add our ops.
    if ($style['storage'] == STYLES_STORAGE_NORMAL) {
      $row[] = array('data' => l(t('edit'), $edit_path));
      $row[] = array('data' => l(t('delete'), $field_containers['admin']['path'] . '/delete/' . $style_name));
    }
    else if ($style['storage'] == STYLES_STORAGE_OVERRIDE) {
      $row[] = array('data' => l(t('edit'), $edit_path));
      $row[] = array('data' => l(t('revert'), $field_containers['admin']['path'] . '/delete/' . $style_name));
    }
    else {
      $row[] = array('data' => l(t('edit'), $edit_path), 'colspan' => 2);
    }
    $rows[] = $row;
  }

  // Add a new style link to the empty table.
  // Note that the  link at the top of the page is placed in hook_menu.
  $field_info = field_info_field_types($field_type);
  $field_label = $field_info['label'];
  $title = 'Add ' . $field_label . ' style';

  $build['styles_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No styles available. <a href="@link">@add</a>.', array('@add' => t($title), '@link' => url($field_containers['admin']['path'] . '/add'))),
  );

  return $build;
}

/**
 *  Page callback for adding a style.
 */
function styles_ui_style_add_form($form, $form_state, $field_type) {
  $form = array();
  $form['field_type'] = array(
    '#type' => 'value',
    '#value' => $field_type,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Style name'),
    '#description' => t('Enter the name of your desired style, which must be a unique name containing only alphanumeric characters and underscores.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 *  Validation handler for the add style form.
 */
function styles_ui_style_add_form_validate($form, $form_state) {
  $field_type = $form_state['values']['field_type'];
  $style_name = $form_state['values']['name'];
  $styles = styles_default_styles();
  if (!preg_match('!^[a-z0-9_]+$!', $style_name)) {
    form_set_error('style_name', t('The machine-readable style name must contain only lowercase letters, numbers, and underscores.'));
  }
  else if (!empty($styles[$field_type]['styles'][$style_name])) {
    form_set_error('style_name', t('The machine-readable style name %style_name is already taken.', array('%style_name' => $style_name)));
  }
}

/**
 *  Submission handler for the add style form.
 */
function styles_ui_style_add_form_submit(&$form, &$form_state) {
  $field_type = $form_state['values']['field_type'];
  $style_name = $form_state['values']['name'];
  $style = array(
    'field_type' => $field_type,
    'name' => $style_name,
    'description' => '',
  );
  // Save the new style.
  styles_style_save($style);

  // Get the containers for this field type.
  $styles_containers = styles_default_containers();
  $field_containers = $styles_containers[$field_type];

  // Redirect to this style's edit page.
  $form_state['redirect'] = $field_containers['admin']['path'] . '/edit/' . $style_name;
}

/**
 *  Callback for the style edit form.
 */
function styles_ui_style_edit_form($form, $form_state, $field_type, $style_name) {
  $style = styles_style_load($field_type, $style_name);

  $form['field_type'] = array(
    '#type' => 'value',
    '#value' => $field_type,
  );
  $form['old_style_name'] = array(
    '#type' => 'value',
    '#value' => $style['name'],
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The name of this style, which must be a unique name containing only alphanumeric characters and underscores.'),
    '#default_value' => $style['name'],
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('The description of the style.'),
    '#default_value' => $style['description'],
  );

  $form['containers'] = array(
    '#type' => 'vertical_tabs',
    '#tree' => TRUE,
  );
  $containers = styles_default_containers($field_type);
  $presets = styles_default_presets($field_type);
  foreach ($containers['containers'] as $container_name => $container) {
    $style = $presets['containers'][$container_name]['styles'][$style_name];
    $container_label = isset($container['label']) ? $container['label'] : $container_name;
    $form['containers'][$container_name] = array(
      '#type' => 'fieldset',
      '#title' => t('@container', array('@container' => $container_label)),
    );
    $options = array();
    foreach ($presets['containers'][$container_name]['presets'] as $preset_name => $preset) {
      $options[$preset_name] = t('@preset', array('@preset' => $preset_name));
    }
    $default_value = isset($presets['containers'][$container_name]['styles'][$style_name]['preset']) ? $presets['containers'][$container_name]['styles'][$style_name]['preset'] : (isset($presets['containers'][$container_name]['styles'][$style_name]['default preset']) ? $presets['containers'][$container_name]['styles'][$style_name]['default preset'] : $presets['containers'][$container_name]['default preset']);
    $form['containers'][$container_name]['preset'] = array(
      '#type' => 'select',
      '#title' => t('Preset'),
      '#options' => $options,
      '#default_value' => $default_value,
    );
    $form['containers'][$container_name]['default_preset'] = array(
      '#type' => 'item',
      '#title' => t('Default preset'),
      '#markup' => t('%preset', array('%preset' => $style['default preset'])),
    );
    if (isset($container['preview'])) {
      $form['containers'][$container_name]['preview'] = array(
        '#type' => 'item',
        '#title' => t('Preview'),
        '#markup' => theme($container['preview'], array('style_name' => $style_name)),
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
//   $field_info = field_info_field_types($field_type);
//   $presets = styles_default_presets();
//   $preset = $presets[$field_type]['containers'][$preset_name];
//   $styles_containers = styles_default_containers();
//   $containers = $styles_containers[$field_type]['containers'];
//   drupal_set_title(t('Edit @field_type style preset: @preset', array('@field_type' => $field_info['label'], '@preset' => $preset_name)));
//   $form = array();
//   $form['containers'] = array(
//     '#type' => 'vertical_tabs',
//   );
//   $styles = styles_default_styles($field_type);
//   dpm($styles);
//
//   // Begin the settings array to send to jQuery.
//   $settings = array(
//     'stylesUI' => array(
//       'url' => url('styles-ui/preview'),
//       'fieldType' => check_plain($field_type),
//     ),
//   );
//   foreach ($styles['styles'] as $style_name => $style) {
//     $form['containers'][$style_name] = array(
//       '#type' => 'fieldset',
//       '#title' => $style['label'],
//     );
//     if (isset($preset['styles'][$style_name]['current preset'])) {
//       $this_preset = $preset['styles'][$style_name]['current preset'];
//     }
//     else {
//       $this_preset = $preset['styles'][$style_name]['default preset'];
//     }
//     $options = array();
//     foreach ($preset['presets'] as $preset_style_name => $effects) {
//       $options[$preset_style_name] = $preset_style_name;
//     }
//
//     // Store the container in the 'rel' attribute for later AJAX previews.
//     $rel = check_plain($style_name);
//     $form['containers'][$style_name]['preset_' . $style_name] = array(
//       '#type' => 'radios',
//       '#title' => t('Style effects preset'),
//       '#default_value' => $this_preset,
//       '#options' => $options,
//       '#attributes' => array('class' => array('styles-ui-preset'), 'rel' => $rel),
//     );
//     // Add a preview.
//     if (isset($styles_containers[$field_type]['themes']) && isset($styles_containers[$field_type]['themes']['preview'])) {
//       $preview = '<div id="styles-ui-preview-wrapper-' . $rel . '" class="styles-ui-preview-wrapper">' . theme($styles_containers[$field_type]['themes']['preview'], array('field_type' => $field_type, 'container_name' => $style_name, 'style_name' => $this_preset)) . '</div>';
//       $form['containers'][$style_name]['preview_' . $style_name] = array(
//         '#type' => 'item',
//         '#title' => t('Style preview'),
//         '#markup' => $preview,
//       );
//     }
//   }
//   // Add the javascript for live previews on radio select.
//   $form['#attached'] = array(
//     'js' => array(drupal_get_path('module', 'styles_ui') . '/styles_ui.js'),
//   );
//   drupal_add_js($settings, array('type' => 'setting'));
//   return $form;
}

/**
 *  Validation handler for the edit style form.
 */
function styles_ui_style_edit_form_validate($form, $form_state) {
  $field_type = $form_state['values']['field_type'];
  $style_name = $form_state['values']['name'];
  $old_style_name = $form_state['values']['old_style_name'];
  $styles = styles_default_styles();
  if (!preg_match('!^[a-z0-9_]+$!', $style_name)) {
    form_set_error('style_name', t('The machine-readable style name must contain only lowercase letters, numbers, and underscores.'));
  }
  else if (($style_name != $old_style_name) && !empty($styles[$field_type]['styles'][$style_name])) {
    form_set_error('style_name', t('The machine-readable style name %style_name is already taken.', array('%style_name' => $style_name)));
  }
}

/**
 *  Submission handler for the add style form.
 */
function styles_ui_style_edit_form_submit(&$form, &$form_state) {
  $field_type = $form_state['values']['field_type'];
  $style_name = $form_state['values']['name'];
  $style = array(
    'field_type' => $field_type,
    'name' => $style_name,
    'description' => $form_state['values']['description'],
  );
  // Save the new style.
  styles_style_save($style);

  // Get the containers for this field type.
  $styles = styles_default_presets($field_type);
  $containers = styles_default_containers($field_type);

  foreach ($form_state['values']['containers'] as $container_name => $container) {
    if (is_array($container)) {
      $default_preset = isset($styles['containers'][$container_name]['styles'][$style_name]['default preset']) ? $styles['containers'][$container_name]['styles'][$style_name]['default preset'] : $styles['containers'][$container_name]['default preset'];
      $delete_only = ($default_preset == $container['preset']);
      styles_style_save_preset($field_type, $container_name, $style_name, $container['preset'], $delete_only);
    }
  }

  // Redirect to this style's edit page.
  $form_state['redirect'] = $containers['admin']['path'] . '/edit/' . $style_name;
}

function styles_ui_preview_ajax($field_type, $container_name, $preset_name) {
  $styles_containers = styles_default_containers();
  $containers = $styles_containers[$field_type]['containers'];
  drupal_json_output(array(
    'id' => '#styles-ui-preview-wrapper-' . check_plain($container_name),
    'preview' => theme($containers[$container_name]['themes']['preview'], array('field_type' => $field_type, 'container_name' => $container_name, 'style_name' => $preset_name)),
  ));
  die();
}

/**
 * Menu callback; delete a style.
 */
function styles_ui_delete_confirm($form, &$form_state, $field_type, $style_name) {
  $form['field_type'] = array(
    '#type' => 'value',
    '#value' => $field_type,
  );
  $form['style_name'] = array(
    '#type' => 'value',
    '#value' => $style_name,
  );

  $styles = styles_default_styles($field_type);
  $style = $styles['styles'][$style_name];
  if ($style['storage'] == STYLES_STORAGE_OVERRIDE) {
    $delete = 'revert';
  }
  else {
    $delete = 'delete';
  }

  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  $containers = styles_default_containers($field_type);
  $cancel_path = $containers['admin']['path'];
  $form['containers'] = array(
    '#type' => 'value',
    '#value' => $containers,
  );

  $message = t('Are you sure you want to @delete the %field_type style %style_name?', array('@delete' => $delete, '%field_type' => $field_type, '%style_name' => $style_name));

  return confirm_form($form, $message, $cancel_path, $caption, t('@delete', array('@delete' => ucfirst($delete))));
}

/**
 * Process style delete confirm submissions.
 */
function styles_ui_delete_confirm_submit($form, &$form_state) {
  $field_type = $form_state['values']['field_type'];
  $style_name = $form_state['values']['style_name'];
  $containers = styles_default_containers($field_type);
  styles_style_delete($field_type, $style_name);
  // Redirect to this style's edit page.
  $form_state['redirect'] = $containers['admin']['path'];
}
