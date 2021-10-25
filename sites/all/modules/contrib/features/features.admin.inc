<?php

/**
 * @file
 * Forms for Features admin screens.
 */

/**
 * Form builder for 'admin/structure/features/settings'.
 *
 * @param array $form
 *   Form array, as passed in from form API.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Form array.
 */
function features_settings_form($form, $form_state) {
  $form = array();

  $components = features_get_components();
  uasort($components, 'features_compare_component_name');
  $form['show_components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show components on create/edit feature form.'),
    '#description' => t('Components with no options will not be shown no matter the setting below. Disabled components cannot be used with admin form.'),
  );

  $form['lock_components'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lock components'),
    '#description' => t('Locked components will be prevented from ever being reverted. For example, if site builder updates a feature with new settings for a field instance, but field instance is locked, it will not update that field. If the item is purely in code, like a view, the view changed when the code is updated no matter these settings.'),
  );
  $form['features_lock_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Features lock mode'),
    '#options' => array(
      'rebuild' => t('Allow rebuild (prevent revert)'),
      'all' => t('Prevent rebuild and revert'),
    ),
    '#description' => t('Rebuild will allow the feature to be updated till the point features has detected that the item has changed deliberately on the site, e.g. is overriden.'),
    '#default_value' => variable_get('features_lock_mode', 'all'),
  );
  foreach ($components as $component => $info) {
    if (empty($info['feature_source']) && empty($info['features_source'])) {
      continue;
    }
    $form['show_components']['features_admin_show_component_' . $component] = array(
      '#title' => t('@name (@machine)', array('@name' => $info['name'], '@machine' => $component)),
      '#type' => 'checkbox',
      '#default_value' => variable_get('features_admin_show_component_' . $component, TRUE),
    );
    /* @see \hook_features_revert() */
    /* @see \hook_features_rebuild() */
    if (features_hook($component, 'features_revert') || features_hook($component, 'features_rebuild')) {
      $form['lock_components']['features_component_locked_' . $component] = array(
        '#title' => t('@name (@machine)', array('@name' => $info['name'], '@machine' => $component)),
        '#type' => 'checkbox',
        '#default_value' => variable_get('features_component_locked_' . $component, FALSE),
      );
    }
    if ($component == 'menu_links' && ($menus = menu_get_menus())) {
      $form['show_components']['features_admin_menu_links'] = array(
        '#title' => t('Advanced Menu Link Settings'),
        '#type' => 'fieldset',
        '#collapsed' => TRUE,
        '#collapsible' => TRUE,
        '#states' => array(
          'invisible' => array(
            'input[name="features_admin_show_component_menu_links"]' => array('checked' => FALSE),
          ),
        ),
      );
      $form['show_components']['features_admin_menu_links']['features_admin_menu_links_menus'] = array(
        '#title' => t('Allowed menus for menu links'),
        '#type' => 'checkboxes',
        '#options' => array_map('check_plain', $menus),
        '#default_value' => variable_get('features_admin_menu_links_menus', array_keys(menu_get_menus())),
      );
    }
  }

  $form['general'] = array(
    '#title' => t('General settings'),
    '#type' => 'fieldset',
  );
  $form['general']['features_default_export_path'] = array(
    '#title' => t('Default export path'),
    '#type' => 'textfield',
    '#default_value' => variable_get('features_default_export_path', FEATURES_DEFAULT_EXPORT_PATH),
    '#description' => t('All feature exports will be automatically saved to this path, unless overridden on the individual feature.'),
  );
  $form['general']['features_rebuild_on_flush'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild features on cache clear'),
    '#default_value' => variable_get('features_rebuild_on_flush', TRUE),
    '#description' => t('If you have a large site with many features, you may experience lag on full cache clear. If disabled, features will rebuild only when viewing the features list or saving the modules list.'),
  );
  $form['general']['features_rebuild_modules_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Rebuild features on accessing modules list page'),
    '#default_value' => variable_get('features_rebuild_modules_page', FALSE),
    '#description' => t('If you have a large site with many features, you may experience lag on accessing the modules administration page. If disabled, features will not rebuild when viewing the modules list.'),
  );

  return system_settings_form($form);
}

/**
 * Form builder for features export form.
 *
 * Used on:
 * - 'admin/structure/features/create'
 * - 'admin/structure/features/%feature/recreate'
 *
 * Acts as a router based on the form_state.
 *
 * @param array $form
 *   Form array, as passed in from form API.
 * @param array $form_state
 *   Form state.
 * @param object|null $feature
 *   The feature object, if available. NULL by default.
 *
 * @return array
 *   Form array.
 *
 * @see \features_export_build_form_submit()
 * @see \features_form_rebuild()
 * @ingroup forms
 */
function features_export_form($form, $form_state, $feature = NULL) {
  module_load_include('inc', 'features', 'features.export');
  features_include();

  $feature_name = !empty($feature->name) ? $feature->name : '';
  $form = array(
    '#attributes' => array('class' => array('features-export-form', 'clearfix')),
    '#feature' => isset($feature) ? $feature : NULL,
  );
  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Information'),
    '#tree' => FALSE,
    '#weight' => 2,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => "<div id='features-export-info'>",
    '#suffix' => '</div>',
  );
  $form['info']['name'] = array(
    '#title' => t('Name'),
    '#description' => t('Example: Image gallery') . ' (' . t('Do not begin name with numbers.') . ')',
    '#type' => 'textfield',
    '#default_value' => !empty($feature->info['name']) ? $feature->info['name'] : '',
  );
  $form['info']['module_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine-readable name'),
    '#description' => t('Example: image_gallery') . '<br/>' . t('May only contain lowercase letters, numbers and underscores. <strong>Try to avoid conflicts with the names of existing Drupal projects.</strong>'),
    '#required' => TRUE,
    '#default_value' => $feature_name,
    '#machine_name' => array(
      /* @see \features_export_form_module_name_exists() */
      'exists' => 'features_export_form_module_name_exists',
      'source' => array('info', 'name'),
    ),
  );
  // If recreating this feature, disable machine name field to ensure the
  // machine name cannot be changed, unless user role has granted permission to
  // edit machine name of disabled features.
  if (isset($feature) && ($feature->status || !user_access('rename features'))) {
    $form['info']['module_name']['#value'] = $feature_name;
    $form['info']['module_name']['#disabled'] = TRUE;
  }
  $form['info']['description'] = array(
    '#title' => t('Description'),
    '#description' => t('Provide a short description of what users should expect when they enable your feature.'),
    '#type' => 'textfield',
    '#default_value' => !empty($feature->info['description']) ? $feature->info['description'] : '',
  );
  $form['info']['package'] = array(
    '#title' => t('Package'),
    '#description' => t('Organize your features in groups.'),
    '#type' => 'textfield',
    '#autocomplete_path' => 'features/autocomplete/packages',
    '#default_value' => !empty($feature->info['package']) ? $feature->info['package'] : 'Features',
  );
  $form['info']['version'] = array(
    '#title' => t('Version'),
    '#description' => t('Examples: 7.x-1.0, 7.x-1.0-beta1'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#default_value' => !empty($feature->info['version']) ? $feature->info['version'] : '',
    '#size' => 30,
    /* @see \features_export_form_validate_field() */
    '#element_validate' => array('features_export_form_validate_field'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Options'),
    '#tree' => FALSE,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 10,
    '#prefix' => "<div id='features-export-advanced'>",
    '#suffix' => '</div>',
  );
  $form['advanced']['project_status_url'] = array(
    '#title' => t('URL of update XML'),
    '#description' => t('URL of Feature Server.  For Example: http://mywebsite.com/fserver'),
    '#type' => 'textfield',
    '#required' => FALSE,
    '#default_value' => !empty($feature->info['project status url']) ? $feature->info['project status url'] : '',
    /* @see \features_export_form_validate_field() */
    '#element_validate' => array('features_export_form_validate_field'),
  );
  $directory = (!empty($feature->filename)) ? dirname($feature->filename) : variable_get('features_default_export_path', FEATURES_DEFAULT_EXPORT_PATH);
  if (!empty($feature_name) && substr_compare($directory, $feature_name, strlen($directory) - strlen($feature_name), strlen($feature_name)) === 0) {
    // If path ends with module_name, strip it.
    $directory = dirname($directory);
  }
  if (user_access('generate features')) {
    $form['advanced']['generate_path'] = array(
      '#title' => t('Path to Generate feature module'),
      '#description' => t('File path for feature module.  For Example: sites/all/modules/features or /tmp.  ' .
        t('Leave blank for <strong>@path</strong>', array('@path' => $directory))),
      '#type' => 'textfield',
      '#required' => FALSE,
      '#default_value' => !empty($feature->info['project path']) ? $feature->info['project path'] : '',
    );
    $form['advanced']['generate'] = array(
      '#type' => 'submit',
      '#value' => t('Generate feature'),
      /* @see \features_export_build_form_submit() */
      /* @see \features_form_rebuild() */
      '#submit' => array('features_export_build_form_submit', 'features_form_rebuild'),
    );
  }
  // Build the component listing panel on the right.
  _features_export_form_components($form, $form_state);

  $form['advanced']['info-preview'] = array(
    '#type' => 'button',
    '#value' => t('Preview .info file'),
    '#ajax' => array(
      /* @see \features_info_file_preview() */
      'callback' => 'features_info_file_preview',
      'wrapper' => 'features-export-wrapper',
    ),
  );
  // Info dialog.
  $form['advanced']['info-file'] = array(
    '#prefix' => '<div id="features-info-file" title="Export .info file preview">',
    'text'    => array(
      '#type' => 'textarea',
      '#default_value' => '',
      '#resizable' => FALSE,
    ),
    '#suffix' => '</div>',
  );

  $form['buttons'] = array(
    /* @see \theme_features_form_buttons() */
    '#theme' => 'features_form_buttons',
    '#tree' => FALSE,
    '#weight' => 99,
    '#prefix' => "<div id='features-export-buttons'>",
    '#suffix' => '</div>',
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Download feature'),
    '#weight' => 10,
    /* @see \features_export_build_form_submit() */
    /* @see \features_form_rebuild() */
    '#submit' => array('features_export_build_form_submit', 'features_form_rebuild'),
  );

  $form['#attached']['library'][] = array('system', 'ui.dialog');

  return $form;
}

/**
 * Machine name existence callback for the module name.
 *
 * @param string $value
 *   Module name to check.
 *
 * @return bool
 *   TRUE, if a module with this name already exists (enabled or not).
 */
function features_export_form_module_name_exists($value) {
  return (bool) features_get_info('module', $value);
}

/**
 * Adds form elements for component selection on the export form.
 *
 * @param array $form
 *   Form array to add the form elements to.
 * @param array $form_state
 *   The form state array coming from Drupal core.
 */
function _features_export_form_components(&$form, &$form_state) {
  global $features_ignore_conflicts;
  drupal_add_css(drupal_get_path('module', 'features') . '/features.css');
  drupal_add_js(drupal_get_path('module', 'features') . '/features.js');

  $feature = $form['#feature'];

  // Keep the allow_conflict variable around in the session.
  if (isset($form_state['values']['features_allow_conflicts'])) {
    $_SESSION['features_allow_conflicts'] = $form_state['values']['features_allow_conflicts'];
    $features_ignore_conflicts = $_SESSION['features_allow_conflicts'];
  }

  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Components'),
    '#description' => t('Expand each component section and select which items should be included in this feature export.'),
    '#tree' => FALSE,
    '#prefix' => "<div id='features-export-wrapper'>",
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => 1,
  );

  // Filter field used in javascript, so javascript will unhide it.
  $form['export']['features_filter_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#tree' => FALSE,
    '#prefix' => "<div id='features-filter' class='element-invisible'>",
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => -10,
  );
  $form['export']['features_filter_wrapper']['features_filter'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#hidden' => TRUE,
    '#default_value' => '',
    '#suffix' => "<span class='features-filter-clear'>" . t('Clear') . "</span>",
  );
  $form['export']['features_filter_wrapper']['checkall'] = array(
    '#type' => 'checkbox',
    '#default_value' => FALSE,
    '#hidden' => TRUE,
    '#title' => t('Select all'),
    '#attributes' => array(
      'class' => array('features-checkall'),
    ),
  );

  $form['advanced']['features_autodetect_wrapper'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#prefix' => "<div id='features-autodetect'>",
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['advanced']['features_autodetect_wrapper']['autodetect'] = array(
    '#title' => t('Add auto-detected dependencies'),
    '#type' => 'checkbox',
    '#default_value' => !empty($feature->info['no autodetect']) ? FALSE : TRUE,
  );

  // This refresh button will rebuild the form.
  // This button is hidden by javascript, since it is only needed when
  // javascript is not available.
  $form['advanced']['features_autodetect_wrapper']['features_refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh'),
    '#name' => 'features-refresh',
    '#attributes' => array(
      'title' => t("Refresh the list of auto-detected items."),
      'class' => array('features-refresh-button'),
    ),
    /* @see \features_export_form_rebuild() */
    '#submit' => array('features_export_form_rebuild'),
    '#prefix' => "<div class='features-refresh-wrapper'>",
    '#suffix' => "</div>",
    '#ajax' => array(
      /* @see \features_export_form_ajax() */
      'callback' => 'features_export_form_ajax',
      'wrapper' => 'features-export-wrapper',
    ),
  );

  // Generate the export array for the current feature and user selections.
  $export = _features_export_build($feature, $form_state);

  $form['advanced']['features_allow_conflicts'] = array(
    '#title' => t('Allow conflicts to be added'),
    '#type' => 'checkbox',
    '#default_value' => $features_ignore_conflicts,
    '#ajax' => array(
      /* @see \features_export_form_ajax() */
      'callback' => 'features_export_form_ajax',
      'wrapper' => 'features-export-wrapper',
    ),
  );

  if (isset($form_state['values']['op']) && ($form_state['values']['op'] == $form_state['values']['info-preview'])) {
    // Handle clicking the "Preview .info file" button.
    module_load_include('inc', 'features', 'features.export');

    $feature_export = _features_export_generate($export, $form_state, $feature);
    $feature_export = features_export_prepare($feature_export, $feature->name, TRUE);
    $info = features_export_info($feature_export);

    drupal_add_js(array('features' => array('info' => $info)), 'setting');
  }

  // Determine any components that are deprecated.
  $deprecated = features_get_deprecated($export['components']);

  $sections = array('included', 'detected', 'added');
  foreach ($export['components'] as $component => $component_info) {
    if (!variable_get('features_admin_show_component_' . $component, TRUE)) {
      continue;
    }

    $component_items_count = count($component_info['options']['sources']);
    $count_label = ' (<span class = "component-count">' . $component_items_count . '</span>)';
    $label = (isset($component_info['name']) ?
      $component_info['name'] . $count_label . " <span>(" . check_plain($component) . ")</span>"
      : check_plain($component) . $count_label);

    $count = 0;
    foreach ($sections as $section) {
      $count += count($component_info['options'][$section]);
    }
    $extra_class = ($count == 0) ? 'features-export-empty' : '';
    $component_name = str_replace('_', '-', check_plain($component));

    if ($count + $component_items_count > 0) {

      if (!empty($deprecated[$component])) {
        // Only show deprecated component if it has some exports.
        if (!empty($component_info['options']['included'])) {
          $form['export'][$component] = array(
            '#markup' => '',
            '#tree' => TRUE,
          );

          $form['export'][$component]['deprecated'] = array(
            '#type' => 'fieldset',
            '#title' => $label . "<span class='features-conflict'> (" . t('DEPRECATED') . ")</span>",
            '#tree' => TRUE,
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
            '#attributes' => array('class' => array('features-export-component')),
          );
          $list = ' ';
          foreach ($component_info['options']['included'] as $key) {
            $list .= "<span class='form-type-checkbox features-conflict'>$key</span>";
          }
          $form['export'][$component]['deprecated']['selected'] = array(
            '#prefix' => "<div class='component-detected'>",
            '#markup' => $list,
            '#suffix' => "</div>",
          );
        }
      }
      else {
        $form['export'][$component] = array(
          '#markup' => '',
          '#tree' => TRUE,
        );

        $form['export'][$component]['sources'] = array(
          '#type' => 'fieldset',
          '#title' => $label,
          '#tree' => TRUE,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#attributes' => array('class' => array('features-export-component')),
          '#prefix' => "<div class='features-export-parent component-$component'>",
        );
        $form['export'][$component]['sources']['selected'] = array(
          '#type' => 'checkboxes',
          '#id' => "edit-sources-$component_name",
          '#options' => features_dom_encode_options($component_info['options']['sources']),
          '#default_value' => features_dom_encode_options($component_info['selected']['sources'], FALSE),
          '#attributes' => array(
            'class' => array('component-select'),
          ),
        );

        foreach ($sections as $section) {
          $form['export'][$component][$section] = array(
            '#type' => 'checkboxes',
            '#options' => !empty($component_info['options'][$section]) ?
              features_dom_encode_options($component_info['options'][$section]) : array(),
            '#default_value' => !empty($component_info['selected'][$section]) ?
              features_dom_encode_options($component_info['selected'][$section], FALSE) : array(),
            '#attributes' => array('class' => array('component-' . $section)),
          );
        }
        $form['export'][$component][$sections[0]]['#prefix'] =
          "<div class='component-list features-export-list $extra_class'>";
        $form['export'][$component][$sections[count($sections) - 1]]['#suffix'] = '</div></div>';
      }
    }
  }
  $form['export']['features_legend'] = array(
    '#type' => 'fieldset',
    '#title' => t('Legend'),
    '#tree' => FALSE,
    '#prefix' => "<div id='features-legend'>",
    '#suffix' => '</div>',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['export']['features_legend']['legend'] = array(
    '#markup' =>
      "<span class='component-included'>Normal</span> " .
      "<span class='component-added'>Changed</span> " .
      "<span class='component-detected'>Auto detected</span> " .
      "<span class='features-conflict'>Conflict</span> ",
  );
}

/**
 * Return the full feature export array based upon user selections in form_state.
 *
 * NOTE: This routine gets a bit complex to handle all of the different possible
 * user checkbox selections and de-selections.
 * Cases to test:
 *   1a) uncheck Included item -> mark as Added but unchecked
 *   1b) re-check unchecked Added item -> return it to Included check item
 *   2a) check Sources item -> mark as Added and checked
 *   2b) uncheck Added item -> return it to Sources as unchecked
 *   3a) uncheck Included item that still exists as auto-detect -> mark as Detected but unchecked
 *   3b) re-check Detected item -> return it to Included and checked
 *   4a) check Sources item should also add any auto-detect items as Detected and checked
 *   4b) uncheck Sources item with auto-detect and auto-detect items should return to Sources and unchecked
 *   5a) uncheck a Detected item -> refreshing page should keep it as unchecked Detected
 *   6)  when nothing changes, refresh should not change any state
 *   7)  should never see an unchecked Included item.
 *
 * @param \stdClass $feature
 *   Feature info object.
 * @param array $form_state
 *   Optional form_state information for user selections.
 *   Can be updated to reflect new selection status.
 *
 * @return array
 *   New export array to be exported
 *   array['components'][$component_name] = $component_info
 *     $component_info['options'][$section] is list of available options
 *     $component_info['selected'][$section] is option state TRUE/FALSE
 *   $section = array('sources', included', 'detected', 'added')
 *     sources - options that are available to be added to the feature
 *     included - options that have been previously exported to the feature
 *     detected - options that have been auto-detected
 *     added - newly added options to the feature
 */
function _features_export_build($feature, &$form_state) {
  global $features_ignore_conflicts;
  // Set a global to effect features_get_component_map when building feature
  // hate to use a global, but it's just for an admin screen so probably ok.
  if (isset($_SESSION['features_allow_conflicts'])) {
    $features_ignore_conflicts = $_SESSION['features_allow_conflicts'];
  }

  $feature_name = isset($feature->name) ? $feature->name : NULL;
  $conflicts = _features_get_used($feature_name);
  $reset = FALSE;
  if (isset($form_state['triggering_element']['#name']) && ($form_state['triggering_element']['#name'] == 'features_allow_conflicts')) {
    // When the 'Allow conflicts to be added' checkbox gets checked, reset the
    // feature back to its original state.
    $reset = TRUE;
  }

  module_load_include('inc', 'features', 'features.export');
  features_include();

  $components = features_get_components();
  uasort($components, 'features_compare_component_name');

  // Assemble the combined component list.
  $stub = array();
  $sections = array('sources', 'included', 'detected', 'added');

  // Create a new feature "stub" to populate.
  $stub_count = array();
  foreach ($components as $component => $component_info) {
    if ($reset) {
      unset($form_state['values'][$component]);
    }
    if (!variable_get('features_admin_show_component_' . $component, TRUE)) {
      unset($components[$component]);
      continue;
    }
    // User-selected components take precedence.
    $stub[$component] = array();
    $stub_count[$component] = 0;
    // Add selected items from 'sources' checkboxes.
    if (!empty($form_state['values'][$component]['sources']['selected'])) {
      $stub[$component] = array_merge($stub[$component], features_dom_decode_options(array_filter($form_state['values'][$component]['sources']['selected'])));
      $stub_count[$component]++;
    }
    // Add selected items from already included and newly added checkboxes.
    foreach (array('included', 'added') as $section) {
      if (!empty($form_state['values'][$component][$section])) {
        $stub[$component] = array_merge($stub[$component], features_dom_decode_options(array_filter($form_state['values'][$component][$section])));
        $stub_count[$component]++;
      }
    }
    // Count any detected items.
    if (!empty($form_state['values'][$component]['detected'])) {
      $stub_count[$component]++;
    }
    // Only fallback to an existing feature's values if there are no export
    // options for the component.
    if ($component == 'dependencies') {
      if (($stub_count[$component] == 0) && !empty($feature->info['dependencies'])) {
        $stub[$component] = drupal_map_assoc($feature->info['dependencies']);
      }
    }
    elseif (($stub_count[$component] == 0) && !empty($feature->info['features'][$component])) {
      $stub[$component] = drupal_map_assoc($feature->info['features'][$component]);
    }
  }
  // Generate new populated feature.
  $export = features_populate(array('features' => $stub, 'dependencies' => $stub['dependencies']), $feature_name);

  // Components that are already exported to feature.
  $exported_features_info = !empty($feature->info['features']) ? $feature->info['features'] : array();
  $exported_features_info['dependencies'] = !empty($feature->info['dependencies']) ? $feature->info['dependencies'] : array();
  // Components that should be exported.
  $new_features_info = !empty($export['features']) ? $export['features'] : array();
  $new_features_info['dependencies'] = !empty($export['dependencies']) ? $export['dependencies'] : array();
  $excluded = !empty($feature->info['features_exclude']) ? $feature->info['features_exclude'] : array();

  // Now fill the $export with categorized sections of component options
  // based upon user selections and de-selections.
  foreach ($components as $component => $component_info) {
    $component_export = $component_info;
    foreach ($sections as $section) {
      $component_export['options'][$section] = array();
      $component_export['selected'][$section] = array();
    }
    /* @see \hook_features_export_options() */
    $options = features_invoke($component, 'features_export_options');
    /* @see \hook_features_export_options_alter() */
    drupal_alter('features_export_options', $options, $component);
    if (!empty($options)) {
      $exported_components = !empty($exported_features_info[$component]) ? $exported_features_info[$component] : array();
      $new_components = !empty($new_features_info[$component]) ? $new_features_info[$component] : array();

      // Find all default components that are not provided by this feature and
      // strip them out of the possible options.
      if ($map = features_get_default_map($component)) {
        foreach ($map as $k => $v) {
          if (isset($options[$k]) && (!isset($feature->name) || $v !== $feature->name)) {
            unset($options[$k]);
          }
        }
      }
      foreach ($options as $key => $value) {
        // Use the $clean_key when accessing $form_state.
        $clean_key = features_dom_encode($key);
        // If checkbox in 'sources' is checked, move it to the 'added' section.
        if (!empty($form_state['values'][$component]['sources']['selected'][$clean_key])) {
          unset($form_state['input'][$component]['sources']['selected'][$clean_key]);
          $form_state['values'][$component]['sources']['selected'][$clean_key] = FALSE;
          $form_state['values'][$component]['added'][$clean_key] = 1;
          $form_state['input'][$component]['added'][$clean_key] = $clean_key;
          $component_export['options']['added'][$key] = check_plain($value);
          $component_export['selected']['added'][$key] = $key;
        }
        elseif (in_array($key, $new_components)) {
          // Option is in the New exported array.
          if (in_array($key, $exported_components)) {
            // Option was already previously exported
            // so it's part of the Included checkboxes.
            $section = 'included';
            $default_value = $key;
            if ($reset) {
              // Leave it included.
            }
            // If Included item was un-selected (removed from export $stub)
            // but was re-detected in the $new_components
            // means it was an auto-detect that was previously part of the export
            // and is now de-selected in UI.
            elseif (!empty($form_state['values']) &&
                (isset($form_state['values'][$component]['included'][$clean_key]) ||
                 empty($form_state['values'][$component]['detected'][$clean_key])) &&
                empty($stub[$component][$key])) {
              $section = 'detected';
              $default_value = FALSE;
            }
            // Unless it's unchecked in the form, then move it to Newly disabled item.
            elseif (!empty($form_state['values']) &&
                empty($form_state['values'][$component]['added'][$clean_key]) &&
                empty($form_state['values'][$component]['detected'][$clean_key]) &&
                empty($form_state['values'][$component]['included'][$clean_key])) {
              $section = 'added';
              $default_value = FALSE;
            }
          }
          else {
            // Option was in New exported array, but NOT in already exported
            // so it's a user-selected or an auto-detect item.
            $section = 'detected';
            // Check for item explicity excluded.
            if (isset($excluded[$component][$key]) && !isset($form_state['values'][$component]['detected'][$clean_key])) {
              $default_value = FALSE;
            }
            else {
              $default_value = $key;
            }
            // If it's already checked in Added or Sources, leave it in Added as checked.
            if (!empty($form_state['values']) &&
              (!empty($form_state['values'][$component]['added'][$clean_key]) ||
               !empty($form_state['values'][$component]['sources']['selected'][$clean_key]))) {
              $section = 'added';
              $default_value = $key;
            }
            // If it's already been unchecked, leave it unchecked.
            elseif (!empty($form_state['values']) &&
              empty($form_state['values'][$component]['sources']['selected'][$clean_key]) &&
              empty($form_state['values'][$component]['detected'][$clean_key]) &&
              !isset($form_state['values'][$component]['added'][$clean_key])) {
              $section = 'detected';
              $default_value = FALSE;
            }
          }
          $component_export['options'][$section][$key] = check_plain($value);
          $component_export['selected'][$section][$key] = $default_value;
          // Save which dependencies are specifically excluded from auto-detection.
          if (($section == 'detected') && ($default_value === FALSE)) {
            $excluded[$component][$key] = $key;
            // Remove excluded item from export.
            if ($component == 'dependencies') {
              unset($export['dependencies'][$key]);
            }
            else {
              unset($export['features'][$component][$key]);
            }
          }
          else {
            unset($excluded[$component][$key]);
          }
          // Remove the 'input' and set the 'values' so Drupal stops looking at 'input'.
          if (isset($form_state['values'])) {
            if (!$default_value) {
              unset($form_state['input'][$component][$section][$clean_key]);
              $form_state['values'][$component][$section][$clean_key] = FALSE;
            }
            else {
              $form_state['input'][$component][$section][$clean_key] = $clean_key;
              $form_state['values'][$component][$section][$clean_key] = 1;
            }
          }
        }
        else {
          // Option was not part of the new export.
          $added = FALSE;
          foreach (array('included', 'added') as $section) {
            // Restore any user-selected checkboxes.
            if (!empty($form_state['values'][$component][$section][$clean_key])) {
              $component_export['options'][$section][$key] = check_plain($value);
              $component_export['selected'][$section][$key] = $key;
              $added = TRUE;
            }
          }
          if (!$added) {
            // If not Included or Added, then put it back in the unchecked Sources checkboxes.
            $component_export['options']['sources'][$key] = check_plain($value);
            $component_export['selected']['sources'][$key] = FALSE;
          }
        }
      }
    }
    $export['components'][$component] = $component_export;
  }
  $export['features_exclude'] = $excluded;

  // Make excluded list and conflicts available for javascript to pass to our ajax callback.
  drupal_add_js(array('features' => array(
    'excluded' => $excluded,
    'conflicts' => $conflicts,
  )), 'setting');

  return $export;
}

/**
 * AJAX callback for features_export_form.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Part of the form array to be delivered in the AJAX request.
 */
function features_export_form_ajax($form, &$form_state) {
  return $form['export'];
}

/**
 * Submit handler for 'Refresh' button in the export form.
 *
 * Tells the ajax form submission to rebuild form state.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state array to modify.
 */
function features_export_form_rebuild($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Page callback for 'features/ajaxcallback/%'.
 *
 * @param string $feature_name
 *   Path fragment specifying the feature being exported.
 */
function features_export_components_json($feature_name) {
  module_load_include('inc', 'features', 'features.export');
  $export = array('features' => array());
  if (!empty($_POST['items'])) {
    $excluded = (!empty($_POST['excluded'])) ? $_POST['excluded'] : array();
    $stub = array();
    foreach ($_POST['items'] as $key) {
      preg_match('/^([^\[]+)(\[.+\])?\[(.+)\]\[(.+)\]$/', $key, $matches);
      if (!empty($matches[1]) && !empty($matches[4])) {
        $component = $matches[1];
        $item = features_dom_decode($matches[4]);
        if (empty($stub[$component])) {
          $stub[$component] = array($item);
        }
        else {
          $stub[$component] = array_merge($stub[$component], array($item));
        }
      }
    }

    $stub['dependencies'] = isset($stub['dependencies']) ? $stub['dependencies'] : array();
    $export = features_populate(array('features' => $stub, 'dependencies' => $stub['dependencies']), $feature_name);
    $export['features']['dependencies'] = $export['dependencies'];

    // Uncheck any detected item that is in the excluded list.
    foreach ($export['features'] as $component => $value) {
      foreach ($value as $key => $item) {
        $clean_key = features_dom_encode($key);
        if ($key != $clean_key) {
          // Need to move key to a cleankey for javascript.
          $export['features'][$component][$clean_key] = $export['features'][$component][$key];
          unset($export['features'][$component][$key]);
        }
        if (isset($excluded[$component][$key])) {
          $export['features'][$component][$clean_key] = FALSE;
        }
      }
    }
  }
  print drupal_json_encode($export['features']);
}

/**
 * AJAX callback for 'Preview .info file' button.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Part of the form to be delivered with AJAX.
 */
function features_info_file_preview($form, &$form_state) {
  return $form['export'];
}

/**
 * Form API callback: Validates a project field.
 *
 * This function is assigned as an #element_validate callback in
 * features_export_form().
 *
 * @param array $element
 *   Form element with '#type' => 'textfield'.
 * @param array $form_state
 *   Form state.
 */
function features_export_form_validate_field($element, &$form_state) {
  switch ($element['#name']) {
    case 'project_status_url':
      if (!empty($element['#value']) && !valid_url($element['#value'])) {
        form_error($element, t('The URL %url is invalid. Please enter a fully-qualified URL, such as http://www.example.com/feed.xml.', array('%url' => $element['#value'])));
      }
      break;

    case 'version':
      preg_match('/^(?P<core>\d+\.x)-(?P<major>\d+)\.(?P<patch>\d+)-?(?P<extra>\w+)?$/', $element['#value'], $matches);
      if (!empty($element['#value']) && !isset($matches['core'], $matches['major'])) {
        form_error($element, t('Please enter a valid version with core and major version number. Example: @example', array('@example' => '7.x-1.0')));
      };
      break;
  }
}

/**
 * Return the $export array to be rendered for the feature export.
 *
 * @param array $export
 *   Stub version of the export array.
 * @param array $form_state
 *   Form state.
 * @param \stdClass|null $feature
 *   Feature module info object.
 *
 * @return array
 *   Export array.
 */
function _features_export_generate($export, $form_state, $feature = NULL) {
  // Remove the UI data that we are not saving to disk.
  unset($export['components']);

  $module_name = $form_state['values']['module_name'];
  // Directly copy the following attributes from $form_state.
  $attr = array('name', 'description', 'package', 'project path');
  foreach ($attr as $key) {
    $export[$key] = isset($form_state['values'][$key]) ? $form_state['values'][$key] : NULL;
  }
  // Directly copy the following attributes from the original feature.
  $attr = array('scripts' , 'stylesheets');
  foreach ($attr as $key) {
    $export[$key] = isset($feature->info[$key]) ? $feature->info[$key] : NULL;
  }
  // If either update status-related keys are provided, add a project key
  // corresponding to the module name.
  if (!empty($form_state['values']['version']) && !empty($form_state['values']['project_status_url'])) {
    $export['project'] = $form_state['values']['module_name'];
  }
  if (!empty($form_state['values']['version'])) {
    $export['version'] = $form_state['values']['version'];
  }
  if (!empty($form_state['values']['project_status_url'])) {
    $export['project status url'] = $form_state['values']['project_status_url'];
  }
  $export['no autodetect'] = empty($form_state['values']['autodetect']) ? 1 : NULL;
  $export['project path'] = !empty($form_state['values']['generate_path']) ? $form_state['values']['generate_path'] : NULL;
  return $export;
}

/**
 * First submit handler 'Generate feature' and 'Download feature' buttons.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @see features_export_form()
 */
function features_export_build_form_submit($form, &$form_state) {
  $feature = $form['#feature'];
  $export = _features_export_build($feature, $form_state);
  $export = _features_export_generate($export, $form_state, $feature);
  $generate = isset($form_state['values']['generate']) && ($form_state['values']['op'] == $form_state['values']['generate']);
  $module_name = $form_state['values']['module_name'];

  if ($generate && !user_access('generate features')) {
    drupal_set_message(t("No permission for generating features."));
    return;
  }

  // Generate download.
  if ($files = features_export_render($export, $module_name, TRUE)) {
    $filename = (!empty($export['version']) ? "{$module_name}-{$export['version']}" : $module_name) . '.tar';

    if ($generate) {
      $success = TRUE;
      $destination = variable_get('features_default_export_path', FEATURES_DEFAULT_EXPORT_PATH);
      $directory = (!empty($export['project path'])) ? $export['project path'] . '/' . $module_name :
        (isset($feature->filename) ? dirname($feature->filename) : $destination . '/' . $module_name);
      if (!is_dir($directory)) {
        if (mkdir($directory, 0777, TRUE) === FALSE) {
          $success = FALSE;
        }
      }
    }
    else {
      // Clear out output buffer to remove any garbage from tar output.
      if (ob_get_level()) {
        ob_end_clean();
      }

      drupal_add_http_header('Content-type', 'application/x-tar');
      drupal_add_http_header('Content-Disposition', 'attachment; filename="' . $filename . '"');
      drupal_send_headers();
    }

    $tar = array();
    $filenames = array();
    // Write additional files from $files['_files'], if present.
    if (!empty($files['_files'])) {
      foreach ($files['_files'] as $file_name => $file_info) {
        if ($generate) {
          // Create a subdirectory, if needed.
          // @todo Support nested subdirectories, see #3060408.
          if (strpos($file_name, '/')) {
            $file_directory = $directory . '/' . substr($file_name, 0, strrpos($file_name, '/'));
            if (!is_dir($file_directory)) {
              mkdir($file_directory);
            }
          }
          if (!empty($file_info['file_path'])) {
            file_unmanaged_copy($file_info['file_path'], "{$directory}/{$file_name}", FILE_EXISTS_REPLACE);
          }
          elseif ($file_info['file_content']) {
            file_put_contents("{$directory}/{$file_name}", $file_info['file_content']);
          }
        }
        else {
          if (!empty($file_info['file_path'])) {
            print features_tar_create("{$module_name}/{$file_name}", file_get_contents($file_info['file_path']));
          }
          elseif ($file_info['file_content']) {
            features_tar_create("{$directory}/{$file_name}", $file_info['file_content']);
          }
        }
      }
      unset($files['_files']);
    }
    foreach ($files as $extension => $file_contents) {
      if (!in_array($extension, array('module', 'info'))) {
        $extension .= '.inc';
      }
      $filenames[] = "{$module_name}.$extension";
      if ($generate) {
        if (file_put_contents("{$directory}/{$module_name}.$extension", $file_contents) === FALSE) {
          $success = FALSE;
        }
      }
      else {
        print features_tar_create("{$module_name}/{$module_name}.$extension", $file_contents);
      }
    }
    if (features_get_modules($module_name, TRUE)) {
      // Prevent deprecated component files from being included in download.
      $deprecated = features_get_deprecated();
      foreach ($deprecated as $component) {
        $info = features_get_components($component);
        $filename = isset($info['default_file']) && $info['default_file'] == FEATURES_DEFAULTS_CUSTOM ? $info['default_filename'] : "features.{$component}";
        $filename .= '.inc';
        $filenames[] = "{$module_name}.$filename";
      }
      $module_path = drupal_get_path('module', $module_name);
      // file_scan_directory() can throw warnings when using PHP 5.3, messing
      // up the output of our file stream. Suppress errors in this one case in
      // order to produce valid output.
      foreach (@file_scan_directory($module_path, '/.*/') as $file) {
        $filename = substr($file->uri, strlen($module_path) + 1);
        if (!in_array($filename, $filenames)) {
          // Add this file.
          $contents = file_get_contents($file->uri);
          if ($generate) {
            if (file_put_contents("{$directory}/{$filename}", $contents) === FALSE) {
              $success = FALSE;
            }
          }
          else {
            print features_tar_create("{$module_name}/{$filename}", $contents);
          }
          unset($contents);
        }
      }
    }
    if ($generate) {
      if ($success) {
        drupal_set_message(t("Module @name written to @directory",
          array('@name' => $export['name'], '@directory' => $directory)));
      }
      else {
        drupal_set_message(
          t("Could not write module to @path. ", array('@path' => $directory)) .
          t("Ensure your file permissions allow the web server to write to that directory."), "error");
      }
    }
    else {
      print pack("a1024", "");
      exit;
    }
  }
}

/**
 * Callback for array_filter() to exclude hidden modules.
 *
 * @param \stdClass $module
 *   Module info object.
 *
 * @return bool
 *   TRUE, if the module is NOT hidden.
 */
function features_filter_hidden($module) {
  return empty($module->info['hidden']);
}

/**
 * Form builder for 'admin/structure/features'.
 *
 * The form shows a list of feautures grouped into vertical tabs.
 *
 * @param array $form
 *   Form as passed in from form API.
 * @param array $form_state
 *   Form state.
 *
 * @return array
 *   Form.
 *
 * @throws \Exception
 */
function features_admin_form($form, $form_state) {
  $features = _features_get_features_list();
  $modules = array_filter(features_get_modules(), 'features_filter_hidden');
  $conflicts = features_get_conflicts();

  // Load export functions to use in comparison.
  module_load_include('inc', 'features', 'features.export');

  if (empty($features)) {
    $form['no_features'] = array(
      '#markup' => t('No Features were found. Please use the !create_link link to create
      a new Feature module, or upload an existing Feature to your modules directory.',
      array('!create_link' => l(t('Create Feature'), 'admin/structure/features/create'))),
    );
    return $form;
  }

  $form = array('#features' => $features);

  // Generate features form. Features are sorted by dependencies, resort alpha.
  ksort($features);
  foreach ($features as $name => $module) {
    $package_title = !empty($module->info['package']) ? $module->info['package'] : t('Other');
    $package = 'package_' . strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $package_title));

    // Set up package elements.
    if (!isset($form[$package])) {
      $form[$package] = array(
        '#tree' => FALSE,
        '#title' => check_plain($package_title),
        '#theme' => 'features_form_package',
        '#type' => 'fieldset',
        '#group' => 'packages',
      );
      $form[$package]['links'] =
      $form[$package]['version'] =
      $form[$package]['weight'] =
      $form[$package]['status'] =
      $form[$package]['action'] = array('#tree' => TRUE);
    }

    $disabled = FALSE;
    $description = isset($module->info['description']) ? check_plain($module->info['description']) : '';

    // Detect unmet dependencies.
    if (!empty($module->info['dependencies'])) {
      $unmet_dependencies = array();
      $dependencies = _features_export_maximize_dependencies($module->info['dependencies']);
      foreach ($dependencies as $dependency) {
        if (empty($modules[$dependency])) {
          $unmet_dependencies[] = theme('features_module_status', array('status' => FEATURES_MODULE_MISSING, 'module' => $dependency));
        }
      }
      if (!empty($unmet_dependencies)) {
        $description .= "<div class='dependencies'>" . t('Unmet dependencies: !dependencies', array('!dependencies' => implode(', ', $unmet_dependencies))) . "</div>";
        $disabled = TRUE;
      }
    }

    if (!empty($module->dependents)) {
      $disabled = TRUE;
      $description .= "<div class='requirements'>" . t('Required by: !dependents', array('!dependents' => implode(', ', $module->dependents))) . "</div>";
    }

    // Detect potential conflicts.
    if (!empty($conflicts[$name])) {
      $module_conflicts = array();
      foreach ($conflicts[$name] as $conflict => $components) {
        $component_strings = array();
        foreach ($components as $component => $component_conflicts) {
          $component_strings[] = t('@component [@items]', array('@component' => $component, '@items' => implode(', ', $component_conflicts)));
        }
        $component_strings = implode(', ', $component_strings);
        // If conflicting module is disabled, indicate so in feature listing.
        $status = !module_exists($conflict) ? FEATURES_MODULE_DISABLED : FEATURES_MODULE_CONFLICT;
        $module_conflicts[] = theme('features_module_status', array('status' => $status, 'module' => $conflict)) . t(' in ') . $component_strings;
        // Only disable modules with conflicts, if they are not already enabled.
        // If they are already enabled, somehow the user got themselves into a
        // bad situation and they need to be able to disable a conflicted module.
        if (module_exists($conflict) && !module_exists($name)) {
          $disabled = TRUE;
        }
      }
      $description .= "<div class='conflicts'>" . t('Conflicts with: !conflicts', array('!conflicts' => implode(', ', $module_conflicts))) . "</div>";
    }

    $href = "admin/structure/features/{$name}";
    $href_overridden = module_exists('diff') ? $href . '/diff' : $href;
    $module_name = (user_access('administer features')) ? l($module->info['name'], $href) : $module->info['name'];
    $form[$package]['status'][$name] = array(
      '#type' => 'checkbox',
      '#title' => $module_name,
      '#description' => $description,
      '#default_value' => $module->status,
      '#disabled' => $disabled,
    );

    if (!empty($module->info['project status url'])) {
      $uri = l(truncate_utf8($module->info['project status url'], 35, TRUE, TRUE), $module->info['project status url']);
    }
    elseif (isset($module->info['project'], $module->info['version'], $module->info['datestamp'])) {
      $uri = l('http://drupal.org', 'http://drupal.org/project/' . $module->info['project']);
    }
    else {
      $uri = t('Unavailable');
    }
    $version = !empty($module->info['version']) ? $module->info['version'] : '';
    $version = !empty($version) ? "<div class='description'>$version</div>" : '';
    $form[$package]['sign'][$name] = array('#markup' => "{$uri} {$version}");

    if (user_access('administer features')) {
      // Add status link.
      if ($module->status) {
        $state = theme('features_storage_link', array('storage' => FEATURES_CHECKING, 'path' => $href));
        $state .= l(t('Check'), "admin/structure/features/{$name}/status", array('attributes' => array('class' => array('admin-check'))));
        $state .= theme('features_storage_link', array('storage' => FEATURES_REBUILDING, 'path' => $href));
        $state .= theme('features_storage_link', array('storage' => FEATURES_NEEDS_REVIEW, 'path' => $href));
        $state .= theme('features_storage_link', array('storage' => FEATURES_OVERRIDDEN, 'path' => $href_overridden));
        $state .= theme('features_storage_link', array('storage' => FEATURES_DEFAULT, 'path' => $href));
      }
      elseif (!empty($conflicts[$name])) {
        $state = theme('features_storage_link', array('storage' => FEATURES_CONFLICT, 'path' => $href));
      }
      else {
        $state = theme('features_storage_link', array('storage' => FEATURES_DISABLED, 'path' => $href));
      }
      $form[$package]['state'][$name] = array(
        '#markup' => !empty($state) ? $state : '',
      );

      // Add in recreate link.
      $form[$package]['actions'][$name] = array(
        '#markup' => l(t('Recreate'), "admin/structure/features/{$name}/recreate", array('attributes' => array('class' => array('admin-update')))),
      );
    }
  }
  ksort($form);

  // As of 7.0 beta 2 it matters where the "vertical_tabs" element lives on the
  // the array. We add it late, but at the beginning of the array because that
  // keeps us away from trouble.
  $form = array_merge(array('packages' => array('#type' => 'vertical_tabs')), $form);

  $form['buttons'] = array(
    '#theme' => 'features_form_buttons',
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    /* @see \features_form_submit() */
    '#submit' => array('features_form_submit'),
    /* @see \features_form_validate() */
    '#validate' => array('features_form_validate'),
  );
  return $form;
}

/**
 * Form builder for 'admin/structure/features/%feature'.
 *
 * The form shows an overview of components in the feature.
 *
 * @param array $form
 *   Form array, as passed in from form API.
 * @param array $form_state
 *   Form state.
 * @param \stdClass $feature
 *   Feature module info object.
 *
 * @return array
 *   Form array.
 *
 * @throws \Exception
 *   Exception that may theoretically be thrown in theme().
 */
function features_admin_components($form, $form_state, $feature) {
  // Breadcrumb navigation.
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('Administration'), 'admin');
  $breadcrumb[] = l(t('Structure'), 'admin/structure');
  $breadcrumb[] = l(t('Features'), 'admin/structure/features');
  drupal_set_breadcrumb($breadcrumb);

  module_load_include('inc', 'features', 'features.export');
  $form['#feature'] = $feature;

  // Store feature info for theme layer.
  $form['module'] = array('#type' => 'value', '#value' => $feature->name);
  $form['#info'] = $feature->info;
  $form['#dependencies'] = array();
  if (!empty($feature->info['dependencies'])) {
    foreach ($feature->info['dependencies'] as $dependency) {
      $parsed_dependency = drupal_parse_dependency($dependency);
      $dependency = $parsed_dependency['name'];
      $status = features_get_module_status($dependency);
      $form['#dependencies'][$dependency] = $status;
    }
  }

  $conflicts = features_get_conflicts();
  if (!module_exists($form['module']['#value']) && isset($form['module']['#value']) && !empty($conflicts[$form['module']['#value']])) {
    $module_conflicts = $conflicts[$form['module']['#value']];
    $conflicts = array();
    foreach ($module_conflicts as $conflict) {
      $conflicts = array_merge_recursive($conflict, $conflicts);
    }
  }
  else {
    $conflicts = array();
  }
  $form['#conflicts'] = $conflicts;

  $review = $revert = FALSE;

  // Iterate over components and retrieve status for display.
  $states = features_get_component_states(array($feature->name), FALSE);
  $form['revert']['#tree'] = TRUE;
  foreach ($feature->info['features'] as $component => $items) {
    if (user_access('administer features') && array_key_exists($component, $states[$feature->name]) && in_array($states[$feature->name][$component], array(FEATURES_OVERRIDDEN, FEATURES_NEEDS_REVIEW))) {
      switch ($states[$feature->name][$component]) {
        case FEATURES_OVERRIDDEN:
          $revert = TRUE;
          break;

        case FEATURES_NEEDS_REVIEW:
          $review = TRUE;
          break;
      }
      $form['revert'][$component] = array(
        '#type' => 'checkbox',
        '#default_value' => FALSE,
      );
    }
    if (module_exists('diff')) {
      $diffpath = "admin/structure/features/{$feature->name}/diff/{$component}";
      $item = menu_get_item($diffpath);
      $path = ($item && $item['access']) ? $diffpath : NULL;
    }
    else {
      $path = NULL;
    }

    $storage = FEATURES_DEFAULT;
    if (array_key_exists($component, $states[$feature->name])) {
      $storage = $states[$feature->name][$component];
    }
    elseif (array_key_exists($component, $conflicts)) {
      $storage = FEATURES_CONFLICT;
    }
    // This can be removed if the css is fixed so link doesn't move when
    // ajaxing and linke moved.
    $lock_link = '<span class="features-lock-empty"></span>';
    if (user_access('administer features') && (features_hook($component, 'features_revert') || features_hook($component, 'features_rebuild'))) {
      $lock_link = ' ' . theme('features_lock_link', array('feature' => $feature->name, 'component' => $component));
    }
    $form['components'][$component] = array(
      '#markup' => $lock_link . theme('features_storage_link', array('storage' => $storage, 'path' => $path)),
    );
  }

  if ($review || $revert) {
    $form['buttons'] = array('#theme' => 'features_form_buttons', '#tree' => TRUE);
    if ($revert || $review) {
      $form['buttons']['revert'] = array(
        '#type' => 'submit',
        '#value' => t('Revert components'),
        /* @see \features_admin_components_revert() */
        '#submit' => array('features_admin_components_revert'),
      );
    }
    if ($review) {
      $form['buttons']['review'] = array(
        '#type' => 'submit',
        '#value' => t('Mark as reviewed'),
        /* @see \features_admin_components_review() */
        '#submit' => array('features_admin_components_review'),
      );
    }
  }
  return $form;
}

/**
 * Submit handler for the 'Revert components' button.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 *
 * @see \features_admin_components()
 */
function features_admin_components_revert(&$form, &$form_state) {
  module_load_include('inc', 'features', 'features.export');
  features_include();
  $module = $form_state['values']['module'];
  $revert = array($module => array());
  foreach (array_filter($form_state['values']['revert']) as $component => $status) {
    $revert[$module][] = $component;
    drupal_set_message(t('Reverted all <strong>@component</strong> components for <strong>@module</strong>.', array('@component' => $component, '@module' => $module)));
  }
  if (empty($revert[$module])) {
    drupal_set_message(t('Please select which components to revert.'), 'warning');
  }
  features_revert($revert);
  $form_state['redirect'] = 'admin/structure/features/' . $module;
}

/**
 * Submit handler for the 'Mark as reviewed' button.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @see \features_admin_components()
 */
function features_admin_components_review(&$form, &$form_state) {
  module_load_include('inc', 'features', 'features.export');
  features_include();
  $module = $form_state['values']['module'];
  $revert = array();
  foreach (array_filter($form_state['values']['revert']) as $component => $status) {
    features_set_signature($module, $component, NULL, 'review');
    drupal_set_message(t('All <strong>@component</strong> components for <strong>@module</strong> reviewed.', array('@component' => $component, '@module' => $module)));
  }
  $form_state['redirect'] = 'admin/structure/features/' . $module;
}

/**
 * Validate handler for the 'manage features' form.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @see \features_admin_form()
 */
function features_form_validate(&$form, &$form_state) {
  include_once './includes/install.inc';
  $conflicts = features_get_conflicts();
  foreach ($form_state['values']['status'] as $module => $status) {
    if ($status) {
      if (!empty($conflicts[$module])) {
        foreach (array_keys($conflicts[$module]) as $conflict) {
          if (!empty($form_state['values']['status'][$conflict])) {
            form_set_error('status', t('The feature @module cannot be enabled because it conflicts with @conflict.', array('@module' => $module, '@conflict' => $conflict)));
          }
        }
      }
      if (!drupal_check_module($module)) {
        form_set_error('status', t('The feature @module cannot be enabled because it has unmet requirements.', array('@module' => $module)));
      }
    }
  }
}

/**
 * Submit handler for the 'manage features' form.
 *
 * @param array $form
 *   Form.
 * @param array $form_state
 *   Form state.
 *
 * @see \features_admin_form()
 */
function features_form_submit(&$form, &$form_state) {
  // Clear drupal caches after enabling a feature. We do this in a separate
  // page callback rather than as part of the submit handler as some modules
  // have includes/other directives of importance in hooks that have already
  // been called in this page load.
  $form_state['redirect'] = array('admin/structure/features/cleanup', array('query' => array('token' => drupal_get_token())));

  $features = $form['#features'];
  if (!empty($features)) {
    $status = $form_state['values']['status'];
    $install = array_keys(array_filter($status));
    $disable = array_diff(array_keys($status), $install);

    // Disable first. If there are any features that are disabled that are
    // dependencies of features that have been queued for install, they will
    // be re-enabled.
    module_disable($disable);
    features_install_modules($install);
  }
}

/**
 * Second submit handler for 'Generate feature' and 'Download feature' buttons.
 *
 * @see \features_export_form()
 */
function features_form_rebuild() {
  cache_clear_all('features:features_list', 'cache');
}

/**
 * Page callback for 'admin/structure/features/cleanup'.
 *
 * Callback for clearing cache after enabling a feature.
 *
 * @return int|void
 *   If the token is missing or invalid, a MENU_NOT_FOUND.
 *   If the token is valid, the process will exit here.
 */
function features_cleanup() {
  if (!empty($_GET['token']) && drupal_valid_token($_GET['token'])) {
    drupal_flush_all_caches();
    // The following functions need to be run because drupal_flush_all_caches()
    // runs rebuilds in the wrong order. The node type cache is rebuilt *after*
    // the menu is rebuilt, meaning that the menu tree is stale in certain
    // circumstances after drupal_flush_all_caches(). We rebuild again.
    menu_rebuild();
    drupal_goto('admin/structure/features');
  }
  return MENU_NOT_FOUND;
}

/**
 * Page callback for 'admin/structure/features/%feature/diff'.
 *
 * Shows the differences between what's in code and what is in the db.
 *
 * @param \stdClass $feature
 *   A loaded feature object to display differences for.
 * @param string $component
 *   (optional) Specific component to display differences for. If excluded, all
 *   components are used.
 *
 * @return array
 *   Render element showing the difference between code and db.
 *
 * @throws \Exception
 *   Exception theoretically thrown in theme().
 */
function features_feature_diff($feature, $component = NULL) {
  drupal_add_css(drupal_get_path('module', 'features') . '/features.css');
  module_load_include('inc', 'features', 'features.export');
  drupal_set_title($feature->info['name']);

  $overrides = features_detect_overrides($feature);

  $output = '';
  if (!empty($overrides)) {
    // Filter overrides down to specified component.
    if (isset($component) && isset($overrides[$component])) {
      $overrides = array($component => $overrides[$component]);
    }

    module_load_include('inc', 'diff', 'diff.engine');
    $formatter = new DrupalDiffFormatter();

    $rows = array();
    foreach ($overrides as $component => $items) {
      $rows[] = array(array('data' => $component, 'colspan' => 4, 'header' => TRUE));
      $diff = new Diff(explode("\n", $items['default']), explode("\n", $items['normal']));
      $rows = array_merge($rows, $formatter->format($diff));
    }
    $header = array(
      array('data' => t('Default'), 'colspan' => 2),
      array('data' => t('Overrides'), 'colspan' => 2),
    );
    $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('diff', 'features-diff'))));
  }
  else {
    $output = "<div class='features-empty'>" . t('No changes have been made to this feature.') . "</div>";
  }
  $output = array('page' => array('#markup' => "<div class='features-comparison'>{$output}</div>"));
  return $output;
}

/**
 * Page callback to lock or unlock a component.
 *
 * Paths:
 *   - 'admin/structure/features/%feature/lock' (official router path)
 *   - 'admin/structure/features/%feature/lock/nojs/' . $component
 *   - 'admin/structure/features/%feature/lock/ajax/' . $component.
 *
 * @param \stdClass $feature
 *   Loaded feature object to be processed for component locking.
 * @param string $type
 *   (optional) Path fragment, which may be 'nojs' or 'ajax'.
 * @param string|null $component
 *   (optional) Path fragment, specifying the component to lock.
 *   If NULL, all components will be locked/unlocked.
 *
 * @return array|void
 *   Render element to display on the page, with a confirm form to confirm
 *   locking or unlocking the component.
 *   In case of 'ajax', nothing is returned, and the component is locked or
 *   unlocked without further confirmation, if the url is signed with a token.
 *
 * @throws \Exception
 *   In theme() if called too early in the request.
 */
function features_admin_lock($feature, $type = 'ajax', $component = NULL) {
  if ($type == 'ajax' && !empty($_GET['token']) && drupal_valid_token($_GET['token'], 'features/' . $feature->name . '/' . ($component ? $component : '')) == $_GET['token']) {
    if (features_feature_is_locked($feature->name, $component, FALSE)) {
      features_feature_unlock($feature->name, $component);
    }
    else {
      features_feature_lock($feature->name, $component);
    }
    $commands = array();
    $new_link = theme('features_lock_link', array('feature' => $feature->name, 'component' => $component));
    $commands[] = ajax_command_replace('#features-lock-link-' . $feature->name . ($component ? '-' . $component : ''), $new_link);
    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  }
  else {
    /* @see \features_feature_lock_confirm_form() */
    return drupal_get_form('features_feature_lock_confirm_form', $feature, $component);
  }
}

/**
 * Form builder for 'admin/structure/features/%feature/lock'.
 *
 * Shows a "Confirm" button that will lock or unlock the feature.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 * @param \stdClass $feature
 *   Loaded feature object.
 * @param string|null $component
 *   Path fragment, specifying the component to lock. If empty, all components.
 *
 * @return array
 *   Form array.
 *
 * @see \features_feature_lock_confirm_form_submit()
 */
function features_feature_lock_confirm_form($form, $form_state, $feature, $component) {
  $form['#feature'] = $feature;
  $form['#component'] = $component;
  $is_locked = features_feature_is_locked($feature->name, $component, FALSE);
  $args = array(
    '@name' => $feature->name,
    '@component' => $component ? $component : t('all'),
    '!action' => $is_locked ? t('unlock') : t('lock'),
  );
  $question = t('Are you sure you want to !action this Feature @name (component @component)?', $args);
  return confirm_form($form, $question, 'admin/structure/features/' . $feature->name);
}

/**
 * Submit callback to lock components of a feature.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state.
 */
function features_feature_lock_confirm_form_submit($form, &$form_state) {
  $feature = $form['#feature']->name;
  $component = $form['#component'];
  if (features_feature_is_locked($feature, $component, FALSE)) {
    features_feature_unlock($feature, $component);
    drupal_set_message(t('Feature @name (component @component) has been unlocked.', array('@name' => $feature, '@component' => $component ? $component : t('all'))));
  }
  else {
    features_feature_lock($feature, $component);
    drupal_set_message(t('Feature @name (component @component) has been locked.', array('@name' => $feature, '@component' => $component ? $component : t('all'))));
  }
  $form_state['redirect'] = 'admin/structure/features/' . $feature;
}

/**
 * Compare the component names. Used to sort alphabetically.
 *
 * @param array $a
 *   Component info array.
 * @param array $b
 *   Component info array.
 *
 * @return int
 *   Negative, postive or zero, depending on the comparison.
 */
function features_compare_component_name($a, $b) {
  return strcasecmp($a['name'], $b['name']);
}

/**
 * Page callback. Sends a json response with the status of a feature.
 *
 * @param \stdClass $feature
 *   Feature module info object.
 */
function features_feature_status($feature) {
  module_load_include('inc', 'features', 'features.export');
  return drupal_json_output(array('storage' => features_get_storage($feature->name)));
}

/**
 * Make a Drupal options array safe for usage with jQuery DOM selectors.
 *
 * Encodes known bad characters into __[ordinal]__ so that they may be
 * safely referenced by JS behaviors.
 *
 * @param string[] $options
 *   Format: $[$key] = $value
 *   Raw options array.
 * @param bool $keys_only
 *   If TRUE, only the keys will be encoded, while values remain unchanged.
 *   If FALSE, both keys and values are encoded.
 *
 * @return string[]
 *   Format, if $keys_only: $[$key_encoded] = $value
 *   Format, if !$keys_only: $[$key_encoded] = $value_encoded
 *   Encoded version of the options array.
 *
 * @see \features_dom_decode_options()
 */
function features_dom_encode_options($options = array(), $keys_only = TRUE) {
  $replacements = features_dom_encode_map();
  $encoded = array();
  foreach ($options as $key => $value) {
    $encoded[strtr($key, $replacements)] = $keys_only ? $value : strtr($value, $replacements);
  }
  return $encoded;
}

/**
 * Encodes a string, replacing characters that cause problems for jQuery.
 *
 * @param string $key
 *   String to encode.
 *
 * @return string
 *   Encoded string.
 *
 * @see \features_dom_decode()
 */
function features_dom_encode($key) {
  $replacements = features_dom_encode_map();
  return strtr($key, $replacements);
}

/**
 * Decodes a string that was encoded with features_dom_encode().
 *
 * @param string $key
 *   Encoded string.
 *
 * @return string
 *   Decoded string.
 *
 * @see \features_dom_encode()
 */
function features_dom_decode($key) {
  $replacements = array_flip(features_dom_encode_map());
  return strtr($key, $replacements);
}

/**
 * Decodes an options array that was encoded by features_dom_encode_options().
 *
 * @param string[] $options
 *   Format: $[$key] = $value
 *   Encoded options.
 * @param bool $keys_only
 *   If TRUE, only the array keys will be decoded.
 *   If FALSE, both array keys and values will be decoded.
 *
 * @return string[]
 *   Format, if $keys_only: $[$key_decoded] = $value
 *   Format, if !$keys_only: $[$key_decoded] = $value_decoded
 *
 * @see \features_dom_encode_options()
 */
function features_dom_decode_options($options, $keys_only = FALSE) {
  $replacements = array_flip(features_dom_encode_map());
  $encoded = array();
  foreach ($options as $key => $value) {
    $encoded[strtr($key, $replacements)] = $keys_only ? $value : strtr($value, $replacements);
  }
  return $encoded;
}

/**
 * Returns encoding map for decode and encode options.
 *
 * @return string[]
 *   Format: $[$char] = $char_encoded
 */
function features_dom_encode_map() {
  return array(
    ':' => '__' . ord(':') . '__',
    '/' => '__' . ord('/') . '__',
    ',' => '__' . ord(',') . '__',
    '.' => '__' . ord('.') . '__',
    '<' => '__' . ord('<') . '__',
    '>' => '__' . ord('>') . '__',
    '%' => '__' . ord('%') . '__',
    ')' => '__' . ord(')') . '__',
    '(' => '__' . ord('(') . '__',
  );
}

/**
 * Page callback: Autocomplete field for features package.
 *
 * @param string $search_string
 *   The char or string that user have written in autocomplete field,
 *   this is the string this function uses for filter.
 *
 * @see features_menu()
 */
function features_autocomplete_packages($search_string) {
  $matched_packages = array();
  // Collect matching package names from existing features.
  foreach (features_get_features(NULL, TRUE) as $value) {
    // @todo More efficient: Collect all package names, then preg_grep() later.
    if (preg_match('/' . $search_string . '/i', $value->info['package'])) {
      $matched_packages[$value->info['package']] = $value->info['package'];
    }
  }
  // Remove duplicate package names.
  // @todo This is not necessary, the array keys already guarantee uniqueness.
  $matched_packages = array_unique($matched_packages);
  drupal_json_output($matched_packages);
}

/**
 * Gets a list of component items already provided by (other) enabled modules.
 *
 * This list can be used to prevent adding/exporting component items to a
 * feature which are already exported elsewhere.
 *
 * @param string|null $module_name
 *   Module to omit in the returned list.
 *
 * @return string[][][]
 *   Format: $[$module][$component][] = $name
 *   E.g. $['myfeature']['node'][] = 'article'
 *   Object names that are already exported in (other) enabled feature modules.
 */
function _features_get_used($module_name = NULL) {

  global $features_ignore_conflicts;
  // Make sure we turn off the ignore_conflicts global to get full list of used components
  // hate to use global, but since this is just for an admin screen it's not a real problem.
  // @todo Rethink this in #3077012.
  $old_value = $features_ignore_conflicts;
  $features_ignore_conflicts = FALSE;

  $conflicts = array();
  $component_info = features_get_components();
  $map = features_get_component_map();

  foreach ($map as $type => $components) {
    // Only check conflicts for components we know about.
    if (isset($component_info[$type])) {
      foreach ($components as $component => $modules) {
        foreach ($modules as $module) {
          // Only for enabled modules.
          if (module_exists($module) && (empty($module_name) || ($module_name != $module))) {
            if (!isset($conflicts[$module])) {
              $conflicts[$module] = array();
            }
            $conflicts[$module][$type][] = $component;
          }
        }
      }
    }
  }

  // Restore previous value of global.
  $features_ignore_conflicts = $old_value;
  return $conflicts;
}

/**
 * Retrieves the array of features as expected on the Manage Features form.
 *
 * Uses caching for performance reasons if caching is enabled.
 *
 * @internal - This function might return cached result with outdated data,
 * use with caution.
 *
 * @return \stdClass[]
 *   Format: $[$module] = $module_info_object
 *   $[$module]->dependents[$dependent_module] = $dependent_module_human_name
 *   Feature objects with added ->dependents property for reverse dependency
 *   lookup.
 */
function _features_get_features_list() {
  $features = array();

  $cache = cache_get('features:features_list');
  if ($cache) {
    $features = $cache->data;
  }

  if (empty($features)) {
    // Clear and rebuild relevant caches.
    features_get_info(NULL, NULL, TRUE);
    features_rebuild();

    $modules = array_filter(features_get_modules(), 'features_filter_hidden');
    $features = array_filter(features_get_features(), 'features_filter_hidden');

    foreach ($modules as $key => $module) {
      if ($module->status && !empty($module->info['dependencies'])) {
        foreach ($module->info['dependencies'] as $dependent) {
          if (isset($features[$dependent])) {
            $features[$dependent]->dependents[$key] = $module->info['name'];
          }
        }
      }
    }

    cache_set('features:features_list', $features);
  }

  return $features;
}
