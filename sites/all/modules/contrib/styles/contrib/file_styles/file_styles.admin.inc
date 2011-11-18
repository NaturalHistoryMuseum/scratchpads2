<?php

/**
 *  @file
 *  Administrative page callbacks for the File: Styles module.
 */

/**
 * Menu callback; Listing of all current File styles.
 */
function file_style_list() {
  $page = array();

  $styles = file_styles();
  $page['file_style_list'] = array(
    '#markup' => theme('file_style_list', array('styles' => $styles)),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'file_styles') . '/file_styles.admin.css' => array('preprocess' => FALSE)),
    ),
  );

  return $page;
}

/**
 * Form builder; Form for adding a new file style.
 *
 * @ingroup forms
 * @see file_style_add_form_submit()
 * @see file_style_name_validate()
 */
function file_style_add_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => '64',
    '#title' => t('Style name'),
    '#default_value' => '',
    '#description' => t('The name is used in URLs for generated file. Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
    '#element_validate' => array('file_style_name_validate'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new style'),
  );

  return $form;
}

/**
 * Submit handler for adding a new file style.
 */
function file_style_add_form_submit($form, &$form_state) {
  $style = array('name' => $form_state['values']['name']);
  $style = file_style_save($style);
  drupal_set_message(t('Style %name was created.', array('%name' => $style['name'])));
  $form_state['redirect'] = 'admin/config/file/file-styles/edit/' . $style['name'];
}

/**
 * Element validate function to ensure unique, URL safe style names.
 */
function file_style_name_validate($element, $form_state) {
  // Check for duplicates.
  $styles = file_styles();
  if (isset($styles[$element['#value']]) && (!isset($form_state['file_style']['msid']) || $styles[$element['#value']]['msid'] != $form_state['file_style']['msid'])) {
    form_set_error($element['#name'], t('The file style name %name is already in use.', array('%name' => $element['#value'])));
  }

  // Check for illegal characters in file style names.
  if (preg_match('/[^0-9a-z_\-]/', $element['#value'])) {
    form_set_error($element['#name'], t('Please only use lowercase alphanumeric characters, underscores (_), and hyphens (-) for style names.'));
  }
}


/**
 * Form builder; Edit a file style name and effects order.
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 * @param $style
 *   An file style array.
 * @ingroup forms
 * @see file_style_form_submit()
 * @see file_style_name_validate()
 */
function file_style_form($form, &$form_state, $file_style) {
  $title = t('Edit %name style', array('%name' => $file_style['name']));
  drupal_set_title($title, PASS_THROUGH);

  // Adjust this form for styles that must be overridden to edit.
  $editable = (bool) ($file_style['storage'] & IMAGE_STORAGE_EDITABLE);

  if (!$editable && empty($form_state['input'])) {
    drupal_set_message(t('This file style is currently being provided by a module. Click the "Override defaults" button to change its settings.'), 'warning');
  }

  $form_state['file_style'] = $file_style;
  $form['#tree'] = TRUE;
  $form['#attached']['css'][drupal_get_path('module', 'file') . '/file.admin.css'] = array('preprocess' => FALSE);
  $form['#attached']['css'][drupal_get_path('module', 'image') . '/image.admin.css'] = array('preprocess' => FALSE);

//   // Show the thumbnail preview.
//   $form['preview'] = array(
//     '#type' => 'item',
//     '#title' => t('Preview'),
//     '#markup' => theme('file_style_preview', array('style' => $style)),
//   );

  // Allow the name of the style to be changed, unless this style is
  // provided by a module's hook_default_file_styles().
  if ($file_style['storage'] & IMAGE_STORAGE_MODULE) {
    $form['name'] = array(
      '#type' => 'item',
      '#title' => t('File style name'),
      '#markup' => $file_style['name'],
      '#description' => t('This file style is being provided by %module module and may not be renamed.', array('%module' => $style['module'])),
    );
  }
  else {
    $form['name'] = array(
      '#type' => 'textfield',
      '#size' => '64',
      '#title' => t('File style name'),
      '#default_value' => $file_style['name'],
      '#description' => t('The name is used in URLs for generated file. Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
      '#element_validate' => array('file_style_name_validate'),
      '#required' => TRUE,
    );
  }

  // Build the list of existing file effects for this file style.
  $form['effects'] = array(
    '#theme' => 'file_style_effects',
  );
//   foreach ($style['effects'] as $meid => $effect) {
//     $form['effects'][$meid]['#weight'] = isset($form_state['input']['effects']) ? $form_state['input']['effects'][$meid]['weight'] : NULL;
//     $form['effects'][$meid]['label'] = array(
//       '#markup' => $effect['label'],
//     );
//     $form['effects'][$meid]['summary'] = array(
//       '#markup' => isset($effect['summary theme']) ? theme($effect['summary theme'], array('data' => $effect['data'])) : '',
//     );
//     $form['effects'][$meid]['weight'] = array(
//       '#type' => 'weight',
//       '#default_value' => $effect['weight'],
//       '#access' => $editable,
//     );
//     $form['effects'][$meid]['configure'] = array(
//       '#type' => 'link',
//       '#title' => t('edit'),
//       '#href' => 'admin/config/file/file-styles/edit/' . $style['name'] . '/effects/' . $effect['meid'],
//       '#access' => $editable && isset($effect['form callback']),
//     );
//     $form['effects'][$meid]['remove'] = array(
//       '#type' => 'link',
//       '#title' => t('delete'),
//       '#href' => 'admin/config/file/file-styles/edit/' . $style['name'] . '/effects/' . $effect['meid'] . '/delete',
//       '#access' => $editable,
//     );
//   }

  $form['effects']['tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  // Build the new file effect addition form and add it to the effect list.
  foreach (file_effect_definitions() as $definition_name => $definition) {
    $form['effects']['tabs'][$definition_name] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => check_plain($definition['label']),
      '#description' => t('@label will be applied to the following:<br />Streams: %streams<br />File types: %types', array('@label' => $definition['label'], '%streams' => implode(', ', $definition['streams']), '%types' => implode(', ', $definition['mimetypes']))),
    );
    $new_effect_options = array(' ' => t('Select a file style to apply'));
    foreach ($definition['styles'] as $style_name => $style) {
      $effects = array();
      foreach ($style['effects'] as $effect) {
        $data = array();
        foreach ($effect['data'] as $type => $value) {
          $data[] = t('@type = @value', array('@type' => $type, '@value' => $value));
        }
        $effects[] = t('@effect: (@data)', array('@effect' => $effect['label'], '@data' => implode(', ', $data)));
      }
      $new_effect_options[$style_name] = t('@name - %effects', array('@name' => $style['name'], '%effects' => implode('; ', $effects))); // check_plain($style['name'] . ' ' . implode(', ', $effects));
    }
    $form['effects']['tabs'][$definition_name]['new'] = array(
      '#type' => 'radios',
      '#title' => t('Please select a style from the following options:'),
      '#options' => $new_effect_options,
      '#default_value' => isset($file_style['styles'][$definition_name]) ? $file_style['styles'][$definition_name] : ' ',
    );
    if (isset($definition['add-new-link'])) {
      $form['effects']['tabs'][$definition_name]['new']['#description'] = filter_xss($definition['add-new-link']);
    }
    if (isset($definition['preview-theme'])) {
      // theme_image_style_preview is not properly registered.
      module_load_include('inc', 'image', 'image.admin');
      // Show the thumbnail preview.
      $form['effects']['tabs'][$definition_name]['preview'] = array(
        '#type' => 'item',
        '#title' => t('Preview'),
        '#markup' => theme($definition['preview-theme'], array('style' => $definition['styles']['thumbnail'])),// $file_style['styles'][$definition_name])),
      );
    }
//     $form['effects']['tabs'][$definition_name]['weight'] = array(
//       '#type' => 'weight',
//       '#default_value' => count($form['effects']['tabs'][$definition_name]) - 1,
//     );
  }

//   $new_effect_options = array('' => t('Select a new effect'));
//   foreach (file_effect_definitions() as $effect => $definition) {
//     $new_effect_options[$effect] = check_plain($definition['name']);
//   }
//   $form['effects']['new'] = array(
//     '#tree' => FALSE,
//     '#weight' => isset($form_state['input']['weight']) ? $form_state['input']['weight'] : NULL,
//     '#access' => $editable,
//   );
//   $form['effects']['new']['new'] = array(
//     '#type' => 'radios',
//     '#options' => $new_effect_options,
//   );

//   $form['effects']['new']['add'] = array(
//     '#type' => 'submit',
//     '#value' => t('Add'),
//     '#validate' => array('file_style_form_add_validate'),
//     '#submit' => array('file_style_form_submit', 'file_style_form_add_submit'),
//   );

  // Show the Override or Submit button for this style.
  $form['override'] = array(
    '#type' => 'submit',
    '#value' => t('Override defaults'),
    '#validate' => array(),
    '#submit' => array('file_style_form_override_submit'),
    '#access' => !$editable,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update style'),
    '#access' => $editable,
  );

  return $form;
}

/**
 * Validate handler for adding a new file effect to a file style.
 */
function file_style_form_add_validate($form, &$form_state) {
  if (!$form_state['values']['new']) {
    form_error($form['effects']['new']['new'], t('Select an effect to add.'));
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Submit handler for adding a new file effect to a file style.
 */
function file_style_form_add_submit($form, &$form_state) {
  $style = $form_state['file_style'];
  // Check if this field has any configuration options.
  $effect = file_effect_definition_load($form_state['values']['new']);

  // Load the configuration form for this option.
  if (isset($effect['form callback'])) {
    $path = 'admin/config/file/file-styles/edit/' . $form_state['file_style']['name'] . '/add/' . $form_state['values']['new'];
    $form_state['redirect'] = array($path, array('query' => array('weight' => $form_state['values']['weight'])));
  }
  // If there's no form, imfiletely add the file effect.
  else {
    $effect['isid'] = $style['isid'];
    $effect['weight'] = $form_state['values']['weight'];
    file_effect_save($effect);
    drupal_set_message(t('The file effect was successfully applied.'));
  }
}

/**
 * Submit handler for overriding a module-defined style.
 */
function file_style_form_override_submit($form, &$form_state) {
  drupal_set_message(t('The %style style has been overridden, allowing you to change its settings.', array('%style' => $form_state['file_style']['name'])));
  file_default_style_save($form_state['file_style']);
}

/**
 * Submit handler for saving a file style.
 */
function file_style_form_submit($form, &$form_state) {
  // Update the file style name if it has changed.
  $style = $form_state['file_style'];
  if (isset($form_state['values']['name']) && $style['name'] != $form_state['values']['name']) {
    $style['name'] = $form_state['values']['name'];
  }

  // Update file effect weights.
  if (!empty($form_state['values']['effects'])) {
    foreach ($form_state['values']['effects'] as $meid => $effect_data) {
      if (isset($style['effects'][$meid])) {
        $effect = $style['effects'][$meid];
        $effect['weight'] = $effect_data['weight'];
        file_effect_save($effect);
      }
    }
  }

  file_style_save($style);
  if ($form_state['values']['op'] == t('Update style')) {
    drupal_set_message('Changes to the style have been saved.');
  }
  $form_state['redirect'] = 'admin/config/file/file-styles/edit/' . $style['name'];
}

function file_style_delete_form() {
}

function file_style_revert_form() {
}

function file_effect_form() {
}

function file_effect_delete_form() {
}

// function file_effect_form() {
// }

/**
 * Display the page containing the list of file styles.
 *
 * @param $variables
 *   An associative array containing:
 *   - styles: An array of all the file styles returned by file_get_styles().
 *
 * @see file_get_styles()
 * @ingroup themeable
 */
function theme_file_style_list($variables) {
  $styles = $variables['styles'];

  $header = array(t('Style name'), t('Settings'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();
  foreach ($styles as $style) {
    $row = array();
    $row[] = l($style['name'], 'admin/config/file/file-styles/edit/' . $style['name']);
    $link_attributes = array(
      'attributes' => array(
        'class' => array('file-style-link'),
      ),
    );
    if ($style['storage'] == IMAGE_STORAGE_NORMAL) {
      $row[] = t('Custom');
      $row[] = l(t('edit'), 'admin/config/file/file-styles/edit/' . $style['name'], $link_attributes);
      $row[] = l(t('delete'), 'admin/config/file/file-styles/delete/' . $style['name'], $link_attributes);
    }
    elseif ($style['storage'] == IMAGE_STORAGE_OVERRIDE) {
      $row[] = t('Overridden');
      $row[] = l(t('edit'), 'admin/config/file/file-styles/edit/' . $style['name'], $link_attributes);
      $row[] = l(t('revert'), 'admin/config/file/file-styles/revert/' . $style['name'], $link_attributes);
    }
    else {
      $row[] = t('Default');
      $row[] = l(t('edit'), 'admin/config/file/file-styles/edit/' . $style['name'], $link_attributes);
      $row[] = '';
    }
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 4,
      'data' => t('There are currently no styles. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/file/file-styles/add'))),
    ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}