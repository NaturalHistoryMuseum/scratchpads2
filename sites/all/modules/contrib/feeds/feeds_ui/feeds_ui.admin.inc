<?php

/**
 * @file
 * Contains all page callbacks, forms and theming functions for Feeds
 * administrative pages.
 */

/**
 * Introductory help for admin/build/feeds/edit/config page
 */
function feeds_ui_edit_help() {
  return t('
    <p>
    You can create as many Feeds importer configurations as you would like to. Each can have a distinct purpose like letting your users aggregate RSS feeds or importing a CSV file for content migration. Here are a couple of things that are important to understand in order to get started with Feeds:
    </p>
    <ul>
    <li>
    Every importer configuration consists of basic settings, a fetcher, a parser and a processor and their settings.
    </li>
    <li>
    The <strong>basic settings</strong> define the general behavior of the importer. <strong>Fetchers</strong> are responsible for loading data, <strong>parsers</strong> for organizing it and <strong>processors</strong> for "doing stuff" with it, usually storing it.
    </li>
    <li>
    In Basic settings, you can <strong>attach an importer configuration to a content type</strong>. This is useful when many imports of a kind should be created, for example in an RSS aggregation scenario. If you don\'t attach a configuration to a content type, you can use it on the !import page.
    </li>
    <li>
    Imports can be <strong>refreshed periodically</strong> - see the minimum refresh period in the Basic settings.
    </li>
    <li>
    Processors can have <strong>mappings</strong> in addition to settings. Mappings allow you to define what elements of a data feed should be mapped to what content fields on a granular level. For instance, you can specify that a feed item\'s author should be mapped to a node\'s body.
    </li>
    </ul>
    ', array('!import' => l(t('Import'), 'import')));
}

/**
 * Help text for mapping.
 */
function feeds_ui_mapping_help() {
  return t('
  <p>
  Define which elements of a single item of a feed (= <em>Sources</em>) map to which content pieces in Drupal (= <em>Targets</em>). Make sure that at least one definition has a <em>Unique target</em>. A unique target means that a value for a target can only occur once. E. g. only one item with the URL <em>http://example.com/story/1</em> can exist.
  </p>
  ');
}

/**
 * Build overview of available configurations.
 */
function feeds_ui_overview_form(&$form_status) {

  $form = $form['enabled'] = $form['disabled'] = array();

  $form['#header'] = array(
    t('Name'),
    t('Description'),
    t('Attached to'),
    t('Status'),
    t('Operations'),
    t('Enabled'),
  );
  foreach (feeds_importer_load_all(TRUE) as $importer) {
    $importer_form = array();
    $importer_form['name']['#value'] = check_plain($importer->config['name']);
    $importer_form['description']['#value'] = check_plain($importer->config['description']);
    if (empty($importer->config['content_type'])) {
      $importer_form['attached']['#value'] = '[none]';
    }
    else {
      if (!$importer->disabled) {
        $importer_form['attached']['#value'] = l(node_get_types('name', $importer->config['content_type']), 'node/add/'. str_replace('_', '-', $importer->config['content_type']));
      }
      else {
        $importer_form['attached']['#value'] = check_plain(node_get_types('name', $importer->config['content_type']));
      }
    }

    if ($importer->export_type == EXPORT_IN_CODE) {
      $status = t('Default');
      $edit = t('Override');
      $delete = '';
    }
    elseif ($importer->export_type == EXPORT_IN_DATABASE) {
      $status = t('Normal');
      $edit = t('Edit');
      $delete = t('Delete');
    }
    elseif ($importer->export_type == (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
      $status = t('Overridden');
      $edit = t('Edit');
      $delete = t('Revert');
    }
    $importer_form['status'] = array(
      '#value' => check_plain($status),
    );
    if (!$importer->disabled) {
      $importer_form['operations'] = array(
        '#value' =>
          l($edit, 'admin/build/feeds/edit/'. $importer->id) .' | '.
          l(t('Export'), 'admin/build/feeds/export/'. $importer->id) .' | '.
          l(t('Clone'), 'admin/build/feeds/clone/'. $importer->id) .
          (empty($delete) ? '' :  ' | '. l($delete, 'admin/build/feeds/delete/'. $importer->id)),
      );
    }
    else {
      $importer_form['operations']['#value'] = '&nbsp;';
    }

    $importer_form[$importer->id] = array(
      '#type' => 'checkbox',
      '#default_value' => !$importer->disabled,
      '#attributes' => array('class' => 'feeds-ui-trigger-submit'),
    );

    if ($importer->disabled) {
      $form['disabled'][$importer->id] = $importer_form;
    }
    else {
      $form['enabled'][$importer->id] = $importer_form;
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => 'feeds-ui-hidden-submit'),
  );
  return $form;
}

/**
 * Submit handler for feeds_ui_overview_form().
 */
function feeds_ui_overview_form_submit($form, &$form_state) {

  $disabled = array();
  foreach (feeds_importer_load_all(TRUE) as $importer) {
    $disabled[$importer->id] = !$form_state['values'][$importer->id];
  }
  variable_set('default_feeds_importer', $disabled);
  feeds_cache_clear();
}

/**
 * Create a new configuration.
 *
 * @param $form_state
 *  Form API form state array.
 * @param $from_importer
 *   FeedsImporter object. If given, form will create a new importer as a copy
 *   of $from_importer.
 */
function feeds_ui_create_form(&$form_state, $from_importer = NULL) {
  drupal_add_js(drupal_get_path('module', 'feeds_ui') .'/feeds_ui.js');
  $form = array();
  $form['#from_importer'] = $from_importer;
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A natural name for this configuration. Example: RSS Feed. You can always change this name later.'),
    '#required' => TRUE,
    '#maxlength' => 128,
    '#attributes' => array('class' => 'feed-name'),
  );
  $form['id'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('A unique identifier for this configuration. Example: rss_feed. Must only contain lower case characters, numbers and underscores.'),
    '#required' => TRUE,
    '#maxlength' => 128,
    '#attributes' => array('class' => 'feed-id'),
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('A description of this configuration.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create'),
  );
  return $form;
}

/**
 * Validation handler for feeds_build_create_form().
 */
function feeds_ui_create_form_validate($form, &$form_state) {
  ctools_include('export');
  $importer = feeds_importer($form_state['values']['id']);
  if (ctools_export_load_object('feeds_importer', 'conditions', array('id' => $form_state['values']['id']))) {
    form_set_error('id', t('Id is taken.'));
  }
  $importer->configFormValidate($form_state['values']);
}

/**
 * Submit handler for feeds_build_create_form().
 */
function feeds_ui_create_form_submit($form, &$form_state) {
  // Create feed.
  $importer = feeds_importer($form_state['values']['id']);
  // If from_importer is given, copy its configuration.
  if (!empty($form['#from_importer'])) {
    $importer->copy($form['#from_importer']);
  }
  // In any case, we want to set this configuration's title and description.
  $importer->addConfig($form_state['values']);
  $importer->save();

  // Set a message and redirect to settings form.
  if (empty($form['#from_importer'])) {
    drupal_set_message(t('Your configuration has been created with default settings. If they do not fit your use case you can adjust them here.'));
  }
  else {
    drupal_set_message(t('A clone of the @name configuration has been created.', array('@name' => $form['#from_importer']->config['name'])));
  }
  $form_state['redirect'] = 'admin/build/feeds/edit/'. $importer->id;
  feeds_cache_clear();
}

/**
 * Delete configuration form.
 */
function feeds_ui_delete_form(&$form_state, $importer) {
  $form['#redirect'] = 'admin/build/feeds';
  $form['#importer'] = $importer;
  if ($importer->export_type & EXPORT_IN_CODE) {
    $title = t('Would you really like to revert the importer @importer?', array('@importer' => $importer->config['name']));
    $button_label = t('Revert');
  }
  else {
    $title = t('Would you really like to delete the importer @importer?', array('@importer' => $importer->config['name']));
    $button_label = t('Delete');
  }
  return confirm_form($form,
    $title,
    $form['#redirect'],
    t('This action cannot be undone.'),
    $button_label
  );
}

/**
 * Submit handler for feeds_ui_delete_form().
 */
function feeds_ui_delete_form_submit($form, &$form_state) {
  // Remove importer.
  $form['#importer']->delete();

  // Clear cache, deleting a configuration may have an affect on menu tree.
  feeds_cache_clear();
}

/**
 * Export a feed configuration.
 */
function feeds_ui_export_form(&$form_state, $importer) {
  $code = feeds_export($importer->id);

  $form['export'] = array(
    '#title' => t('Export feed configuration'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => substr_count($code, "\n"),
  );
  return $form;
}

/**
 * Edit feed configuration.
 */
function feeds_ui_edit_page($importer, $active = 'help', $plugin_key = '') {

  // Get plugins and configuration.
  $plugins = feeds_get_plugins();
  $config = $importer->config;
  // Base path for changing the active container.
  $path = 'admin/build/feeds/edit/'. $importer->id;

  $active_container = array(
    'class' => 'active-container',
    'actions' => array(l(t('Help'), $path)),
  );
  switch ($active) {
    case 'help':
      $active_container['title'] = t('Getting started');
      $active_container['body'] = '<div class="help feeds-admin-ui">'. feeds_ui_edit_help() .'</div>';
      unset($active_container['actions']);
      break;
    case 'fetcher':
    case 'parser':
    case 'processor':
      $active_container['title'] = t('Select a !plugin_type', array('!plugin_type' => $active));
      $active_container['body'] = drupal_get_form('feeds_ui_plugin_form', $importer, $active);
      break;
    case 'settings':
      drupal_add_js(drupal_get_path('module', 'ctools') .'/js/dependent.js');
      ctools_include('dependent');
      if (empty($plugin_key)) {
        $active_container['title'] = t('Basic settings');
        $active_container['body'] = feeds_get_form($importer, 'configForm');
      }
      // feeds_plugin_instance() returns a correct result because feed has been
      // instantiated previously.
      elseif (in_array($plugin_key, array_keys($plugins)) && $plugin = feeds_plugin_instance($plugin_key, $importer->id)) {
        $active_container['title'] = t('Settings for !plugin', array('!plugin' => $plugins[$plugin_key]['name']));
        $active_container['body'] = feeds_get_form($plugin, 'configForm');
      }
      break;
    case 'mapping':
      $active_container['title'] = t('Mapping for !processor', array('!processor' => $plugins[$config['processor']['plugin_key']]['name']));
      $active_container['body'] = drupal_get_form('feeds_ui_mapping_form', $importer);
      break;
  }

  // Build config info.
  $config_info = $info = array();
  $info['class'] = 'config-set';

  // Basic information.
  $items = array();
  $items[] = t('Attached to: !type', array('!type' => $importer->config['content_type'] ? node_get_types('name', $importer->config['content_type']) : t('[none]')));
  if ($importer->config['import_period'] == FEEDS_SCHEDULE_NEVER) {
    $import_period = t('never');
  }
  elseif ($importer->config['import_period'] == 0) {
    $import_period = t('as often as possible');
  }
  else {
    $import_period = t('every !interval', array('!interval' => format_interval($importer->config['import_period'])));
  }
  $items[] = t('Refresh: !import_period', array('!import_period' => $import_period));
  $items[] = $importer->config['import_on_create'] ? t('Import on create') : t('Do not import on create');

  $info['title'] = t('Basic settings');
  $info['body'] = array(
    array(
      'body' => theme('item_list', $items),
      'actions' => array(l(t('Settings'), $path .'/settings')),
    ),
  );
  $config_info[] = $info;

  // Fetcher.
  $fetcher = $plugins[$config['fetcher']['plugin_key']];
  $actions = array();
  if (feeds_get_form($importer->fetcher, 'configForm')) {
    $actions = array(l(t('Settings'), $path .'/settings/'. $config['fetcher']['plugin_key']));
  }
  $info['title'] = t('Fetcher');
  $info['body'] = array(
    array(
      'title' => $fetcher['name'],
      'body' => $fetcher['description'],
      'actions' => $actions,
    ),
  );
  $info['actions'] = array(l(t('Change'), $path .'/fetcher'));
  $config_info[] = $info;

  // Parser.
  $parser = $plugins[$config['parser']['plugin_key']];
  $actions = array();
  if (feeds_get_form($importer->parser, 'configForm')) {
    $actions = array(l(t('Settings'), $path .'/settings/'. $config['parser']['plugin_key']));
  }
  $info['title'] = t('Parser');
  $info['body'] = array(
    array(
      'title' => $parser['name'],
      'body' => $parser['description'],
      'actions' => $actions,
    )
  );
  $info['actions'] = array(l(t('Change'), $path .'/parser'));
  $config_info[] = $info;

  // Processor.
  $processor = $plugins[$config['processor']['plugin_key']];
  $actions = array();
  if (feeds_get_form($importer->processor, 'configForm')) {
    $actions[] = l(t('Settings'), $path .'/settings/'. $config['processor']['plugin_key']);
  }
  $actions[] = l(t('Mapping'), $path .'/mapping');
  $info['title'] = t('Processor');
  $info['body'] = array(
    array(
      'title' => $processor['name'],
      'body' => $processor['description'],
      'actions' => $actions,
    )
  );
  $info['actions'] = array(l(t('Change'), $path .'/processor'));
  $config_info[] = $info;

  return theme('feeds_ui_edit_page', $config_info, $active_container);
}

/**
 * Build a form of plugins to pick from.
 *
 * @param $form_state
 *   Form API form state array.
 * @param $importer
 *   FeedsImporter object.
 * @param $type
 *   Plugin type. One of 'fetcher', 'parser', 'processor'.
 *
 * @return
 *   A Form API form definition.
 */
function feeds_ui_plugin_form(&$form_state, $importer, $type) {
  $plugins = feeds_get_plugins_by_type($type);

  $form = array();
  $form['#importer'] = $importer;
  $form['#plugin_type'] = $type;

  foreach ($plugins as $key => $plugin) {
    $form['plugin_key'][$key] = array(
      '#type' => 'radio',
      '#parents' => array('plugin_key'),
      '#title' => check_plain($plugin['name']),
      '#description' => filter_xss(isset($plugin['help']) ? $plugin['help'] : $plugin['description']),
      '#return_value' => $key,
      '#default_value' => ($plugin['handler']['class'] == get_class($importer->$type)) ? $key : '',
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => 'feeds-ui-hidden-submit'),
  );
  return $form;
}

/**
 * Submit handler for feeds_ui_plugin_form().
 */
function feeds_ui_plugin_form_submit($form, &$form_state) {
  // Set the plugin and save feed.
  $form['#importer']->setPlugin($form_state['values']['plugin_key']);
  $form['#importer']->save();
  drupal_set_message(t('Changed @type plugin.', array('@type' => $form['#plugin_type'])));
}

/**
 * Theme feeds_ui_plugin_form().
 */
function theme_feeds_ui_plugin_form($form) {
  drupal_add_js(drupal_get_path('module', 'feeds_ui') .'/feeds_ui.js');
  $output = '';

  foreach (element_children($form['plugin_key']) as $key) {

    // Assemble container, render form elements.
    $container = array(
      'title' => $form['plugin_key'][$key]['#title'],
      'body' => $form['plugin_key'][$key]['#description'],
    );
    $form['plugin_key'][$key]['#title'] = t('Select');
    $form['plugin_key'][$key]['#attributes']['class'] = 'feeds-ui-radio-link';
    unset($form['plugin_key'][$key]['#description']);
    $container['actions'] = array(drupal_render($form['plugin_key'][$key]));

    $output .= theme_feeds_ui_container($container);
  }

  $output .= drupal_render($form);
  return $output;
}

/**
 * Edit mapping.
 *
 * @todo Completely merge this into config form handling. This is just a
 *   shared form of configuration, most of the common functionality can live in
 *   FeedsProcessor, a flag can tell whether mapping is supported or not.
 */
function feeds_ui_mapping_form(&$form_state, $importer) {
  drupal_add_js(drupal_get_path('module', 'feeds_ui') .'/feeds_ui.js');

  $form = array();
  $form['#importer'] = $importer;
  $form['#mappings'] = $mappings = $importer->processor->getMappings();
  $form['help']['#value'] = feeds_ui_mapping_help();

  // Get mapping sources from parsers and targets from processor, format them
  // for output.
  // Some parsers do not define mapping sources but let them define on the fly.
  if ($sources = $importer->parser->getMappingSources()) {
    $source_options = _feeds_ui_format_options($sources);
    foreach ($sources as $k => $source) {
      $legend['sources'][$k]['name']['#value'] = empty($source['name']) ? $k : $source['name'];
      $legend['sources'][$k]['description']['#value'] = empty($source['description']) ? '' : $source['description'];
    }
  }
  else {
    $legend['sources']['#value'] = t('This parser supports free source definitions. Enter the name of the source field in lower case into the Source text field above.');
  }
  $targets = $importer->processor->getMappingTargets();
  $target_options = _feeds_ui_format_options($targets);
  foreach ($targets as $k => $target) {
    $legend['targets'][$k]['name']['#value'] = empty($target['name']) ? $k : $target['name'];
    $legend['targets'][$k]['description']['#value'] = empty($target['description']) ? '' : $target['description'];
  }

  // Legend explaining source and target elements.
  $form['legendset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Legend'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['legendset']['legend'] = $legend;

  // Add unique and remove forms to mappings.
  $form['unique_flags'] = $form['remove_flags'] = array(
    '#tree' => TRUE,
  );
  if (is_array($mappings)) {
    foreach ($mappings as $i => $mapping) {
      $param = array(
        'processor' => $importer->processor,
        'mapping' => $mapping,
      );
      if (isset($targets[$mapping['target']]['optional_unique']) && $targets[$mapping['target']]['optional_unique'] === TRUE) {
        $form['unique_flags'][$i] = array(
          '#type' => 'checkbox',
          '#default_value' => !empty($mapping['unique']),
          '#attributes' => array('class' => 'feeds-ui-trigger-submit'),
        );
      }
      $form['remove_flags'][$i] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove'),
        '#prefix' => '<div class="feeds-ui-checkbox-link">',
        '#suffix' => '</div>',
      );
    }
  }

  if (isset($source_options)) {
    $form['source'] = array(
      '#type' => 'select',
      '#options' => array('' => t('Select a source')) + $source_options,
    );
  }
  else {
    $form['source'] = array(
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => t('Name of source field'),
      '#attributes' => array('class' => 'hide-text-on-focus'),
    );
  }
  $form['target'] = array(
    '#type' => 'select',
    '#options' => array('' => t('Select a target')) + $target_options,
  );
  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => array('feeds_ui_mapping_form_add_submit'),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#attributes' => array('class' => 'feeds-ui-hidden-submit'),
  );
  return $form;
}

/**
 * Submit handler for add button on feeds_ui_mapping_form().
 */
function feeds_ui_mapping_form_add_submit($form, &$form_state) {
  $importer = $form['#importer'];
  try {
    $mappings = $form['#mappings'];
    $mappings[] = array(
      'source' => $form_state['values']['source'],
      'target' => $form_state['values']['target'],
      'unique' => FALSE,
    );
    $importer->processor->addConfig(array('mappings' => $mappings));
    $importer->processor->save();
    drupal_set_message(t('Mapping has been added.'));
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }
}

/**
 * Submit handler for save button on feeds_ui_mapping_form().
 */
function feeds_ui_mapping_form_submit($form, &$form_state) {
  $processor = $form['#importer']->processor;
  // We may set some unique flags to mappings that we remove in the subsequent
  // step, that's fine.
  $mappings = $form['#mappings'];
  if (isset($form_state['values']['unique_flags'])) {
    foreach ($form_state['values']['unique_flags'] as $k => $v) {
      $mappings[$k]['unique'] = $v;
    }
  }

  if (!empty($form_state['values']['remove_flags'])) {
    foreach ($form_state['values']['remove_flags'] as $k => $v) {
      if ($v) {
        unset($mappings[$k]);
      }
    }
    // Keep our keys clean.
    $mappings = array_values($mappings);
  }
  $processor->addConfig(array('mappings' => $mappings));
  $processor->save();
  drupal_set_message(t('Your changes have been saved.'));
}

/**
 * Walk the result of FeedsParser::getMappingSources() or
 * FeedsProcessor::getMappingTargets() and format them into
 * a Form API options array.
 */
function _feeds_ui_format_options($options) {
  $result = array();
  foreach ($options as $k => $v) {
    if (is_array($v) && !empty($v['name'])) {
      $result[$k] = $v['name'];
    }
    elseif (is_array($v)) {
      $result[$k] = $k;
    }
    else {
      $result[$k] = $v;
    }
  }
  return $result;
}

/**
 * Theme feeds_ui_overview_form().
 */
function theme_feeds_ui_overview_form($form) {
  drupal_add_js(drupal_get_path('module', 'feeds_ui') .'/feeds_ui.js');
  drupal_add_css(drupal_get_path('module', 'feeds_ui') .'/feeds_ui.css');
  // Iterate through all importers and build a table.
  $rows = array();
  foreach (array('enabled', 'disabled') as $type) {
    if (isset($form[$type])) {
      foreach (element_children($form[$type]) as $id) {
        $row = array();
        foreach (element_children($form[$type][$id]) as $col) {
          $row[$col] = array(
            'data' => drupal_render($form[$type][$id][$col]),
            'class' => $type,
          );
        }
        $rows[] = array(
          'data' => $row,
          'class' => $type,
        );
      }
    }
  }
  $output = '';
  if (count($rows)) {
    $output .= theme('table', $form['#header'], $rows, array('class' => 'feeds-admin-importers'));
  }
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme feeds_ui_edit_page().
 */
function theme_feeds_ui_edit_page($config_info, $active_container) {
  drupal_add_css(drupal_get_path('module', 'feeds_ui') .'/feeds_ui.css');

  // Outer wrapper.
  $output = '<div class="feeds-settings clear-block">';

  // Build left bar.
  $output .= '<div class="left-bar">';
  foreach ($config_info as $info) {
    $output .= theme('feeds_ui_container', $info);
  }
  $output .= '</div>';

  // Build configuration space.
  $output .= '<div class="configuration">';
  $output .= '<div class="configuration-squeeze">';
  $output .= theme('feeds_ui_container', $active_container);
  $output .= '</div>';
  $output .= '</div>';

  $output .= '</div>'; // ''<div class="feeds-settings">';

  return $output;
}

/**
 * Render a simple container. A container can have a title, a description and
 * one or more actions. Recursive.
 *
 * @todo Replace with theme_fieldset or a wrapper to theme_fieldset?
 *
 * @param $container
 *   An array that describes the container. All elements are optional:
 *   array(
 *     'title' => 'the title',
 *     'body' => 'the body of the container, may also be an array of more
 *                containers.',
 *     'class' => 'the class of the container.',
 *     'id' => 'the id of the container',
 *   );
 */
function theme_feeds_ui_container($container) {

  $class = empty($container['class']) ? ' plain': " {$container['class']}";
  $id = empty($container['id']) ? '': ' id="'. $container['id'] .'"';
  $output = '<div class="feeds-container'. $class .'"'. $id .'>';

  if (isset($container['actions']) && count($container['actions'])) {
    $output .= '<ul class="container-actions">';
    foreach ($container['actions'] as $action) {
      $output .= '<li>'. $action .'</li>';
    }
    $output .= '</ul>';
  }

  if (!empty($container['title'])) {
    $output .= '<h4 class="feeds-container-title">';
    $output .= $container['title'];
    $output .= '</h4>';
  }

  if (!empty($container['body'])) {
    $output .= '<div class="feeds-container-body">';
    if (is_array($container['body'])) {
      foreach ($container['body'] as $c) {
        $output .= theme('feeds_ui_container', $c);
      }
    }
    else {
      $output .= $container['body'];
    }
    $output .= '</div>';
  }

  $output .= '</div>';
  return $output;
}

/**
 * Theme function for feeds_ui_mapping_form().
 */
function theme_feeds_ui_mapping_form($form) {

  // Build the actual mapping table.
  $header = array(
    t('Source'),
    t('Target'),
    t('Unique target'),
    '&nbsp;',
  );
  $rows = array();
  if (is_array($form['#mappings'])) {
    foreach ($form['#mappings'] as $i => $mapping) {
      // Some parsers do not define source options.
      $source = isset($form['source']['#options'][$mapping['source']]) ? $form['source']['#options'][$mapping['source']] : $mapping['source'];
      $rows[] = array(
        check_plain($source),
        check_plain($form['target']['#options'][$mapping['target']]),
        drupal_render($form['unique_flags'][$i]),
        drupal_render($form['remove_flags'][$i]),
      );
    }
  }
  if (!count($rows)) {
    $rows[] = array(
      array(
        'colspan' => 4,
        'data' => t('No mappings defined.'),
      ),
    );
  }
  $rows[] = array(
    drupal_render($form['source']),
    drupal_render($form['target']),
    '',
    drupal_render($form['add']),
  );
  $output = '<div class="help feeds-admin-ui""'. drupal_render($form['help']) .'</div>';
  $output .= theme('table', $header, $rows);

  // Build the help table that explains available sources.
  $legend = '';
  $rows = array();
  foreach (element_children($form['legendset']['legend']['sources']) as $k) {
    $rows[] = array(
      check_plain(drupal_render($form['legendset']['legend']['sources'][$k]['name'])),
      check_plain(drupal_render($form['legendset']['legend']['sources'][$k]['description'])),
    );
  }
  if (count($rows)) {
    $legend .= '<h4>'. t('Sources') .'</h4>';
    $legend .= theme('table', array(t('Name'), t('Description')), $rows);
  }

  // Build the help table that explains available targets.
  $rows = array();
  foreach (element_children($form['legendset']['legend']['targets']) as $k) {
    $rows[] = array(
      check_plain(drupal_render($form['legendset']['legend']['targets'][$k]['name'])),
      check_plain(drupal_render($form['legendset']['legend']['targets'][$k]['description'])),
    );
  }
  $legend .= '<h4>'. t('Targets') .'</h4>';
  $legend .= theme('table', array(t('Name'), t('Description')), $rows);

  // Stick tables into collapsible fieldset.
  $form['legendset']['legend'] = array(
    '#value' => '<div>'. filter_xss($legend) .'</div>',
  );

  $output .= drupal_render($form['legendset']);

  $output .= drupal_render($form);
  return $output;
}
