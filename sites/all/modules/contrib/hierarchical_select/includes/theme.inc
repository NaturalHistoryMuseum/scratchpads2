<?php

/**
 * @file
 * All theme functions for the Hierarchical Select module.
 */


/**
 * @ingroup themeable
 * @{
 */

/**
 * Return a themed Hierarchical Select form element.
 *
 * @param element
 *   An associative array containing the properties of the element.
 *   Properties used: title, description, id, required
 * @param $value
 *   The form element's data.
 * @return
 *   A string representing the form element.
 *
 * @ingroup themeable
 */
function theme_hierarchical_select_form_element($variables) {
  $element = $variables['element'];
  $value = $variables['value'];

  $output = '<div class="form-item hierarchical-select-wrapper-wrapper"';
  if (!empty($element['#id'])) {
    $output .= ' id="' . $element['#id'] . '-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . t('This field is required.') . '">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="' . $element['#id'] . '">' . t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
    else {
      $output .= ' <label>' . t('!title: !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
  }

  $output .= " $value\n";

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/**
 * Format a hierarchical select.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 * @return
 *   A themed HTML string representing the form element.
 */
function theme_hierarchical_select($variables) {
  $element = $variables['element'];
  $output = '';

  // Update $element['#attributes']['class'].
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = array();
  }
  $hsid = $element['hsid']['#value'];
  $level_labels_style = variable_get('hierarchical_select_level_labels_style', 'none');
  $classes = array(
   'hierarchical-select-wrapper',
   "hierarchical-select-level-labels-style-$level_labels_style",
   // Classes that make it possible to override the styling of specific
   // instances of Hierarchical Select, based on either the ID of the form
   // element or the config that it uses.
   'hierarchical-select-wrapper-for-name-' . $element['#id'],
   (isset($element['#config']['config_id'])) ? 'hierarchical-select-wrapper-for-config-' . $element['#config']['config_id'] : NULL,
  );
  array_merge($element['#attributes']['class'], $classes);
  $element['#attributes']['class'] .= ' ' . implode(' ', $classes);
  $element['#attributes']['id'] = "hierarchical-select-$hsid-wrapper";
  $element['#id'] = "hierarchical-select-$hsid-wrapper"; // This ensures the label's for attribute is correct.

  return '<div ' . drupal_attributes($element['#attributes']) . '>' . drupal_render_children($element) . '</div>';
}

/**
 * Format the container for all selects in the hierarchical select.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 * @return
 *   A themed HTML string representing the form element.
 */
function theme_hierarchical_select_selects_container($variables) {
  $element = $variables['element'];
  $output = '';
  $output .= '<div class="hierarchical-select clearfix">';
  $output .= drupal_render_children($element);
  $output .= '</div>';
  return $output;
}

/**
 * Format a select in the .hierarchial-select div: prevent it from being
 * wrapped in a div. This simplifies the CSS and JS code.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 * @return
 *   A themed HTML string representing the form element.
 */
function theme_hierarchical_select_select($variables) {  
  $element = $variables['element'];
  element_set_attributes($element, array('id', 'name', 'size'));
  _form_set_class($element, array('form-select'));

  return '<select' . drupal_attributes($element['#attributes']) . '>' . _hierarchical_select_options($element) . '</select>';
}

/**
 * Format an item separator (for use in a lineage).
 */
function theme_hierarchical_select_item_separator($variables) {
  $output = '';
  $output .= '<span class="hierarchical-select-item-separator">';
  $output .= '›';
  $output .= '</span>';
  return $output;
}

/**
 * Format a special option in a Hierarchical Select select. For example the
 * "none" option or the "create new item" option. This theme function allows
 * you to change how a special option is indicated textually.
 *
 * @param $option
 *   A special option.
 * @return
 *   A textually indicated special option.
 */
function theme_hierarchical_select_special_option($variables) {
  $option = $variables['option'];
  return '<' . $option . '>';
}

/**
 * Forms API theming callback for the dropbox. Renders the dropbox as a table.
 *
 * @param $element
 *   An element for which the #theme property was set to this function.
 * @return
 *   A themed HTML string.
 */
function theme_hierarchical_select_dropbox_table($variables) {
  $element = $variables['element'];
  $output = '';

  $class = 'dropbox';
  if (form_get_error($element) === '') {
    $class .= ' error';
  }

  $title     = $element['title']['#value'];
  $separator = $element['separator']['#value'];
  $is_empty  = $element['is_empty']['#value'];

  $separator_html = '<span class="hierarchical-select-item-separator">' . $separator . '</span>';

  $output .= '<div class="' . $class . '">';
  $output .= '<table>';
  $output .= '<caption class="dropbox-title">' . $title . '</caption>';
  $output .= '<tbody>';

  if (!$is_empty) {
    // Each lineage in the dropbox corresponds to an entry in the dropbox table.
    $lineage_count = count(element_children($element['lineages']));
    for ($x = 0; $x < $lineage_count; $x++) {
      $db_entry = $element['lineages']["lineage-$x"];
      $zebra = $db_entry['#zebra'];
      $first = $db_entry['#first'];
      $last  = $db_entry['#last'];
      // The deepest level is the number of child levels minus one. This "one"
      // is the element for the "Remove" checkbox.
      $deepest_level = count(element_children($db_entry)) - 1;

      $output .= '<tr class="dropbox-entry ' . $first . ' ' . $last . ' ' . $zebra . '">';
      $output .= '<td>';
      // Each item in a lineage is separated by the separator string.
      for ($depth = 0; $depth < $deepest_level; $depth++) {
        $output .= drupal_render($db_entry[$depth]);

        if ($depth < $deepest_level - 1) {
          $output .= $separator_html;
        }
      }
      $output .= '</td>';
      $output .= '<td class="dropbox-remove">' . drupal_render($db_entry['remove']) . '</td>';
      $output .= '</tr>';
    }
  }
  else {
    $output .= '<tr class="dropbox-entry first last dropbox-is-empty"><td>';
    $output .= t('Nothing has been selected.');
    $output .= '</td></tr>';
  }

  $output .= '</tbody>';
  $output .= '</table>';
  $output .= '</div>';

  return $output;
}

/**
 * Themeing function to render the level_labels settings as a table.
 */
// TODO: rename $form to $element for consistency (and update hook_theme() after that), make the comment consistent.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_hierarchical_select_common_config_form_level_labels($variables) {
  $form = $variables['form'];
  // Recover the stored strings.
  $strings = $form['#strings'];

  $output = '';
  $header = array(t('Level'), t('Label'));
  $rows = array();

  $output .= drupal_render($form['status']);

  $output .= '<div class="level-labels-settings">';
  if (isset($form['labels']) && count(element_children($form['labels']))) {
    foreach (element_children($form['labels']) as $depth) {
      $row = array();
      $row[]['data'] = ($depth == 0) ? t('Root level') : t('Sublevel !depth', array('!depth' => $depth));
      $row[]['data'] = drupal_render($form['labels'][$depth]);
      $rows[] = $row;
    }

    // Render the table.
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('style' => 'width: auto;')));
  }
  else {
    // No levels exist yet in the hierarchy!
    $output .= '<p><strong>';
    $output .= t('There are no levels yet in this !hierarchy!', array('!hierarchy' => $strings['hierarchy']));
    $output .= '</strong></p>';
  }
  $output .= '</div>';

  // Render the remaining form items.
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Themeing function to render the per-level editability settings as a table,
 * (these are the item_types and allowed_levels settings).
 */
// TODO: rename $form to $element for consistency (and update hook_theme() after that), make the comment consistent.
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_hierarchical_select_common_config_form_editability($variables) {
  $form = $variables['form'];
  // Recover the stored strings.
  $strings = $form['#strings'];

  $output = '';
  $header = array(t('Level'), t('Allow'), t('!item_type', array('!item_type' => drupal_ucfirst($strings['item_type']))));
  $rows = array();

  $output .= drupal_render($form['status']);

  $output .= '<div class="editability-per-level-settings">';
  if (isset($form['item_types']) && count(element_children($form['item_types']))) {
    foreach (element_children($form['item_types']) as $depth) {
      $row = array();
      $row[]['data'] = ($depth == 0) ? t('Root level') : t('Sublevel !depth', array('!depth' => $depth));
      $row[]['data'] = drupal_render($form['allowed_levels'][$depth]);
      $row[]['data'] = drupal_render($form['item_types'][$depth]);
      $rows[] = $row;
    }

    // Render the table and description.
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('style' => 'width: auto;'), 'caption' => '<em>' . t('Per-level settings for creating new !items.', array('!items' => $strings['items']))));
    $output .= '<div class="description">';
    $output .= t(
      'The %item_type you enter for each level is what will be used in
      each level to replace a "&lt;create new item&gt;" option with a
      "&lt;create new %item_type&gt;" option, which is often more
      intuitive.',
      array(
        '%item_type' => $strings['item_type'],
      )
    );
    $output .= '</div>';
  }
  else {
    // No levels exist yet in the hierarchy!
    $output .= '<p><strong>';
    $output .= t('There are no levels yet in this !hierarchy!', array('!hierarchy' => $strings['hierarchy']));
    $output .= '</strong></p>';
  }
  $output .= '</div>';

  // Render the remaining form items.
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Themeing function to render a selection (of items) according to a given
 * Hierarchical Select configuration as one or more lineages.
 *
 * @param $selection
 *   A selection of items of a hierarchy.
 * @param $config
 *   A config array with at least the following settings:
 *   - module
 *   - save_lineage
 *   - params
 */
function theme_hierarchical_select_selection_as_lineages($variables) {
  $selection = $variables['selection'];
  $config = $variables['config'];
  $output = '';

  $selection = (!is_array($selection)) ? array($selection) : $selection;

  // Generate a dropbox out of the selection. This will automatically
  // calculate all lineages for us.
  $selection = array_keys($selection);
  $dropbox = _hierarchical_select_dropbox_generate($config, $selection);

  // Actual formatting.
  foreach ($dropbox->lineages as $id => $lineage) {
    if ($id > 0) {
      $output .= '<br />';
    }

    $items = array();
    foreach ($lineage as $level => $item) {
      $items[] = $item['label'];
    }
    $output .= implode('<span class="hierarchical-select-item-separator">›</span>', $items);
  }

  // Add the CSS.
  drupal_add_css(drupal_get_path('module', 'hierarchical_select') . '/hierarchical_select.css');

  return $output;
}

/**
 * @} End of "ingroup themeable".
 */


//----------------------------------------------------------------------------
// Private functions.

/**
 * This is an altered clone of form_select_options(). The reason: I need to be
 * able to set a class on an option element if it contains a level label, to
 * allow for level label styles.
 * TODO: rename to _hierarchical_select_select_options().
 */
function _hierarchical_select_options($element) {
  if (!isset($choices)) {
    $choices = $element['#options'];
  }
  // array_key_exists() accommodates the rare event where $element['#value'] is NULL.
  // isset() fails in this situation.
  $value_valid = isset($element['#value']) || array_key_exists('#value', $element);
  $value_is_array = isset($element['#value']) && is_array($element['#value']);
  $options = '';
  foreach ($choices as $key => $choice) {
    $key = (string) $key;
    if ($value_valid && (!$value_is_array && (string) $element['#value'] === $key || ($value_is_array && in_array($key, $element['#value'])))) {
      $selected = ' selected="selected"';
    }
    else {
      $selected = '';
    }

    // If an option DOES NOT have child info, then it's a special option:
    // - label_\d+ (level label)
    // - none ("<none>")
    // - create_new_item ("<create new item>")
    // Only when it's a level label, we have to add a class to this option.
    if (!isset($element['#childinfo'][$key])) {
      $class = (preg_match('/label_\d+/', $key)) ? ' level-label' : '';
    }
    else {
      $class = ($element['#childinfo'][$key] == 0) ? 'has-no-children' : 'has-children';
    }

    $options .= '<option value="' . check_plain($key) . '" class="' . $class . '"' . $selected . '>' . check_plain($choice) . '</option>';
  }
  return $options;
}
