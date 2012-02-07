<?php

/**
 * @file
 * Administration pages for taxonomy_breadcrumb
 */

/**
 * Settings page for module.
 */
function taxonomy_breadcrumb_admin_settings($form, &$form_state) {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic settings'),
    '#collapsible' => TRUE,
  );

  $form['settings']['taxonomy_breadcrumb_home'] = array(
    '#type' => 'textfield',
    '#title' => t('Home breadcrumb text'),
    '#default_value' => variable_get('taxonomy_breadcrumb_home', t('Home')),
    '#description' => t('Text to display at top of breadcrumb trail. Typically home or your site name. Leave blank to have no home breadcrumb.'),
  );

  $form['settings']['taxonomy_breadcrumb_show_current_term'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show current term in breadcrumb trail'),
    '#default_value' => variable_get('taxonomy_breadcrumb_show_current_term', TRUE),
    '#description' => t('If enabled, the lightest term associated with the node is shown as the last item in the breadcrumb trail. If disabled, the only terms shown in the breadcrumb trail are parent terms (if any parents exist). The recommended setting is enabled.'),
  );

/*
  // TODO - this functionality has been removed from DRUPAL 7.
  // TODO - is there still a way to do this?
  $form['settings']['taxonomy_breadcrumb_use_synonym'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the first term synonym instead of term name'),
    '#default_value' => variable_get('taxonomy_breadcrumb_use_synonym', FALSE),
    '#description' => t("If enabled and if viewing a node, the term's first synonym will be used in the breadcrumb, instead of the term name."),
  );
*/

  $form['settings']['taxonomy_breadcrumb_include_node_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show current node title in breadcrumb trail'),
    '#default_value' => variable_get('taxonomy_breadcrumb_include_node_title', FALSE),
    '#description' => t("If enabled and if viewing a node, the node's title will be shown as the last item in the breadcrumb trail."),
    '#weight' => 30,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#description' => t('Use these advanced settings to control which node types the taxonomy-based breadcrumbs will be generated for.  This allows the taxonomy breadcrumb module to peacefully coexist with modules that define their own breadcrumbs, such as the book module.'),
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['taxonomy_breadcrumb_include_nodes'] = array(
    '#type' => 'radios',
    '#title' => t('Include or exclude the following node types'),
    '#default_value' => variable_get('taxonomy_breadcrumb_include_nodes', 0),
    '#options' => array(
      1 => t('Include'),
      0 => t('Exclude'),
    ),
    '#weight' => 10,
  );

  $tb_types = (array) variable_get('taxonomy_breadcrumb_node_types', TAXONOMY_BREADCRUMB_NODE_TYPES_DEFAULT);
  $default = array();
  foreach ($tb_types as $index => $value) {
    if ($value) {
      $default[] = $index;
    }
  }

  $form['advanced']['taxonomy_breadcrumb_node_types'] = array(
    '#type' => 'checkboxes',
    '#multiple' => TRUE,
    '#title' => t('Node types to include or exclude'),
    '#default_value' => $default,
    '#options' => array_map('check_plain', node_type_get_names()),
    '#description' => t('A list of node types to include or exclude when applying taxonomy-based breadcrumbs.'),
    '#weight' => 20,
  );

  return system_settings_form($form);
}
