<?php
/**
 * @file
 * Linkit theme functions.
 */

/**
 * Processes variables
 *
 * The $variables array contains the following arguments:
 * - $form
 */
function template_preprocess_linkit_profiles_export_ui_form(&$variables) {
  _linkit_preprocess_tabledrag_variables($variables, 'plugins');
  _linkit_preprocess_tabledrag_variables($variables, 'attributes');
}

/**
 * Returns HTML for the profile form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 */
function theme_linkit_profiles_export_ui_form($variables) {
  $form = &$variables['form'];

  $plugin_table = _linkit_theme_profile_form_table($variables, 'plugins');
  $form['data']['plugins_fieldset']['plugins']['#markup'] = $plugin_table;

  $attribute_table = _linkit_theme_profile_form_table($variables, 'attributes');
  $form['data']['attributes_fieldset']['attributes']['#markup'] = $attribute_table;

  $output = drupal_render_children($form);
  return $output;
}

/**
 * Preprocess tabledrag variables
 *
 * @param $variables
 * @param $type
 *   "plugin" or "attribute"
 */
function _linkit_preprocess_tabledrag_variables(&$variables, $type) {
  // Add each attribute in the form to the appropriate place in the attribute listing.
  foreach (element_children($variables['form']['data'][$type . '_fieldset'][$type]) as $i) {
    $element = &$variables['form']['data'][$type . '_fieldset'][$type][$i];

    // Set special classes needed for table drag and drop.
    $element['weight']['#attributes']['class'] = array('weight');

    $variables['linkit_' . $type . '_listing'][$i] = new stdClass();
    $variables['linkit_' . $type . '_listing'][$i]->title = drupal_render($element['name']);
    $variables['linkit_' . $type . '_listing'][$i]->enabled = drupal_render($element['enabled']);
    $variables['linkit_' . $type . '_listing'][$i]->weight_select = drupal_render($element['weight']);
    $variables['linkit_' . $type . '_listing'][$i]->printed = FALSE;

    // Add description if it extists.
    if (isset($element['description'])) {
      $variables['linkit_' . $type . '_listing'][$i]->description = drupal_render($element['description']);
    }
  }
}

/**
 * Preprocess tabledrag
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 */
function theme_linkit_profiles_reorder($variables) {
  $form = $variables['form'];
  $header = array(t('Profile'), t('Roles'), t('Weight'));
  $rows = array();

  if (isset($form['profiles'])) {
    foreach (element_children($form['profiles']) as $profile_name) {
      $rows[] = array(
        'data' => array(
          drupal_render($form['profiles'][$profile_name]['name']),
          drupal_render($form['profiles'][$profile_name]['roles']),
          drupal_render($form['profiles'][$profile_name]['weight'])
        ),
        'class' => array('draggable', 'tabledrag-leaf'),
      );
    }
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'linkit-profile-reorder'), 'sticky' => FALSE));
  $output .= drupal_render_children($form);
  drupal_add_tabledrag('linkit-profile-reorder', 'order', 'sibling', 'weight', NULL, NULL, FALSE);
  return $output;
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
 * Returns HTML for a reverse menu trail. (Based on theme_breadcrumb)
 *
 * @param $variables
 *   An associative array containing:
 *   - reverse_menu_trail: An array containing the menu trail item titles.
 */
function theme_linkit_reverse_menu_trail($variables) {
  $menu_trail = $variables['reverse_menu_trail'];

  if (!empty($menu_trail)) {
    $output = '<p>' . implode($variables['separator'], $menu_trail) . '</p>';
    return $output;
  }
}