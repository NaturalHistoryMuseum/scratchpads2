<?php

/**
 * Build select_importer of available configurations.
 */
function feeds_tamper_ui_select_importer_form($form, &$form_state) {
  $form = array();
  $importers = feeds_importer_load_all();
  if (!empty($importers)) {
    $options = array_keys($importers);
    $options = array_combine($options, $options);
    $form['importer'] = array(
      '#title' => t('Select Importer'),
      '#type' => 'select',
      '#options' => $options,
    );
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    );
    return $form;
  }
  $form['help'] = array(
    '#markup' => t('There are no enabled Feeds importers.'),
  );
  return $form;
}

function feeds_tamper_ui_select_importer_form_submit($form, &$form_state) {
  $importer_id = check_plain($form_state['values']['importer']);
  drupal_goto('admin/structure/feeds_tamper/edit/' . $importer_id);
}

function feeds_tamper_ui_edit_form($form, &$form_state, $importer) {
  $mappings = $importer->processor->config['mappings'];
  $instances = feeds_tamper_load_by_importer($importer->id, TRUE);

  $form = array();

  foreach ($mappings as $mapping) {
    $form['mappings'][$mapping['source']] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($mapping['source']) . ' -> ' . check_plain($mapping['target']),
      '#tree' => TRUE,
    );
    $form['mappings'][$mapping['source']]['table'] = array();
    if (isset($instances[$mapping['source']])) {
      foreach ($instances[$mapping['source']] as $instance) {
        $plugin = feeds_tamper_get_plugin($instance->plugin_id);
        if ($instance->export_type == EXPORT_IN_CODE) {
          $status = t('Default');
          $edit = t('Override');
          $delete = '';
        }
        else if ($instance->export_type == EXPORT_IN_DATABASE) {
          $status = t('Normal');
          $edit = t('Edit');
          $delete = t('Delete');
        }
        else if ($instance->export_type == (EXPORT_IN_CODE | EXPORT_IN_DATABASE)) {
          $status = t('Overridden');
          $edit = t('Edit');
          $delete = t('Revert');
        }
        $instance->disabled = !isset($instance->disabled) ? FALSE : $instance->disabled;
        $checkbox = array('#type' => 'checkbox', '#default_value' => !$instance->disabled);
        $form['mappings'][$mapping['source']]['table'][$instance->id]['enabled'] = $checkbox;
        $data = array(
          !empty($instance->description) ? check_plain($instance->description) : $instance->id,
          $plugin['name'],
          $status,
          l($edit, 'admin/structure/feeds_tamper/edit_plugin/' . $instance->id) .
          (empty($delete) ? '' :  ' | '. l($delete, 'admin/structure/feeds_tamper/delete/'. $instance->id)),
        );
        $form['mappings'][$mapping['source']]['table'][$instance->id]['data'] = array(
          '#type' => 'value',
          '#value' => $data,
        );
        $form['mappings'][$mapping['source']]['table'][$instance->id]['weight-' . $instance->id] = array(
          '#type'=>'textfield',
          '#size'=>5,
          '#default_value'=>$instance->weight,
          '#attributes' => array('class' => array('weight')),
        );
      }
    }
    $form['mappings'][$mapping['source']]['add_link']['#value'] =
    l(t('Add plugin'), 'admin/structure/feeds_tamper/edit/' . $importer->id . '/add/' . $mapping['source']);
  }
  $form['submit']=array(
    '#type'=>'submit',
    '#value'=>t('Save changes'),
  );
  return $form;
}

function feeds_tamper_ui_edit_form_submit($form, &$form_state) {
  $mappings = element_children($form['mappings']);
  $disabled = array();
  foreach ($mappings as $mapping) {
    if (isset($form_state['values'][$mapping])) {
      foreach ($form_state['values'][$mapping]['table'] as $id => $value) {
        $instance = feeds_tamper_load_instance($id);
        $weight = (int) $value['weight-' . $id];
        $disabled[$id] = !$value['enabled'];
        if ($instance->weight != $weight) {
          $instance->weight = $weight;
          ctools_export_crud_save('feeds_tamper', $instance);
        }
      }
    }
  }
  variable_set('default_feeds_tamper', $disabled);
}

function theme_feeds_tamper_ui_edit_form($variables) {
  $form = $variables['form'];
  $header = array(t('Description'), t('Plugin'), t('Status'), t('Operations'), t('Enabled'), t('Weight'));
  $output = '';
  foreach (element_children($form['mappings']) as $source) {
    $table_rows = array();
    $children = element_children($form['mappings'][$source]['table']);

    if (!empty($children)) {
      foreach ($children as $id) {
        $this_row = $form['mappings'][$source]['table'][$id]['data']['#value'];
        $this_row[] = drupal_render($form['mappings'][$source]['table'][$id]['enabled']);
        $this_row[] = drupal_render($form['mappings'][$source]['table'][$id]['weight-' . $id]);
        $table_rows[] = array('data' => $this_row, 'class' => array('draggable'));
      }
      $table_id = preg_replace('/[^a-z0-9-]/', '',
        preg_replace('/_+/', '-',
          strtolower($source . '-table')
        )
      );
      $table = theme_table(array(
        'header' => $header,
        'rows' => $table_rows,
        'attributes' => array('id' => $table_id),
        'caption' => '',
        'colgroups' => array(),
        'sticky' => FALSE,
        'empty' => '',
      ));
      $form['mappings'][$source]['#value'] = $table;
      drupal_add_tabledrag($table_id, 'order', 'sibling', 'weight');
    }
    $form['mappings'][$source]['#value'] .= $form['mappings'][$source]['add_link']['#value'];
    $output .= drupal_render($form['mappings'][$source]);
  }
  $output .= drupal_render_children($form);
  return $output;
}

function feeds_tamper_ui_add_plugin_form($form, &$form_state, $importer, $source) {
  $crumb = drupal_get_breadcrumb();
  $crumb[4] = l(t('Edit ') . $importer->id, 'admin/structure/feeds_tamper/edit/' . $importer->id);
  drupal_set_breadcrumb($crumb);
  $form_state['importer'] = $importer;
  $form_state['source'] = $source;
  $form = array();
  $feeds_tamper_plugins = feeds_tamper_get_plugins();

  $plugins = array();
  foreach ($feeds_tamper_plugins as $plugin_id => $plugin) {
    $plugins[$plugin['category']][$plugin_id] = $plugin['name'];
  }
  ksort($plugins);
  foreach ($plugins as &$p) {
    asort($p);
  }
  $form['plugin_id'] = array(
    '#title' => 'The plugin to add',
    '#type' => 'select',
    '#options' => $plugins,
    '#default_value' => '',
    '#tree' => TRUE,
  );

  foreach ($feeds_tamper_plugins as $machine_name => $plugin) {
    $i = str_replace('_', '-', $machine_name);

    $form['plugin_forms'][$machine_name] = array(
      '#title' => t('Configure ' . $plugin['name']),
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#states' => array(
        'visible' => array(
          ':input[name="plugin_id"]' => array(
            'value' => $machine_name,
          ),
        ),
      ),
    );
    if (!$plugin['machine_name']) {
      $form['plugin_forms'][$machine_name]['id'] = array(
        '#title' => t('Machine Name'),
        '#type' => 'textfield',
        '#description' => t('A unique identifier for this configuration. Example: make_uppercase. Must only contain lower case characters, numbers and underscores.')
      );
    }
    if (!$plugin['description']) {
      $form['plugin_forms'][$machine_name]['description'] = array(
        '#title' => t('Description'),
        '#type' => 'textfield',
        '#description' => t('A useful description of what this plugin is doing.')
      );
    }
    foreach ($plugin['form']($importer, $source, array()) as $key => $form_item) {
      $form['plugin_forms'][$machine_name][$key] = $form_item;
    }
  }
  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  return $form;
}

function feeds_tamper_ui_add_plugin_form_submit($form, &$form_state) {
  $obj = new stdClass();
  $obj->plugin_id = $form_state['values']['plugin_id'];
  $obj->settings = $form_state['values'][$obj->plugin_id];
  $obj->importer = $form_state['importer']->id;
  $obj->source = $form_state['source'];
  $obj->export_type = NULL;
  $obj->description = $form_state['storage']['description'];
  $obj->id = $form_state['storage']['id'];
  $plugin = feeds_tamper_get_plugin($obj->plugin_id);

  ctools_export_crud_save('feeds_tamper', $obj);
  drupal_goto('admin/structure/feeds_tamper/edit/' . $obj->importer);
}

function feeds_tamper_ui_add_plugin_form_validate($form, &$form_state) {
  $plugin_id = $form_state['values']['plugin_id'];
  $plugin = feeds_tamper_get_plugin($plugin_id);
  if ($plugin['validate']) {
    $plugin['validate']($form_state['values'][$plugin_id]);
  }
  if ($plugin['description']) {
    $form_state['storage']['description'] = $plugin['description']($form_state['values'][$plugin_id]);
  }
  else {
    $form_state['storage']['description'] = trim($form_state['values'][$plugin_id]['description']);
    if (!empty($form_state['storage']['description'])) {
      unset($form_state['values'][$plugin_id]['description']);
    }
  }
  if ($plugin['machine_name']) {
    $form_state['storage']['id'] = $form_state['importer']->id .
    '_' . feeds_tamper_make_machine($form_state['source']) .
    '_' . $plugin['machine_name']($form_state['values'][$plugin_id]);
  }
  else {
    $form_state['storage']['id'] = trim($form_state['values'][$plugin_id]['id']);
    if (empty($form_state['storage']['id'])) {
      form_set_error($plugin_id . '][id', t('Machine name is required.'));
    }
    else {
      unset($form_state['values'][$plugin_id]['id']);
    }
  }
}

function feeds_tamper_ui_edit_plugin_form($form, &$form_state, $instance) {
  $importer = l(t('Edit ') . $instance->importer, 'admin/structure/feeds_tamper/edit/' . $instance->importer);
  $crumb = drupal_get_breadcrumb();
  $crumb[] = $importer;
  drupal_set_breadcrumb($crumb);
  $form_state['instance'] = $instance;
  $plugin = feeds_tamper_get_plugin($instance->plugin_id);
  $form = array();
  $form['#tree'] = TRUE;

  $importer = feeds_importer($instance->importer);
  if (!$plugin['description']) {
    $form['description'] = array(
      '#title' => t('Description'),
      '#type' => 'textfield',
      '#description' => t('A useful description of what this plugin is doing.'),
      '#default_value' => $instance->description,
    );
  }
  foreach ($plugin['form']($importer, $instance->source, $instance->settings) as $key => $form_item) {
    $form[$instance->plugin_id][$key] = $form_item;
  }

  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function feeds_tamper_ui_edit_plugin_form_validate($form, &$form_state) {
  $plugin_id = $form_state['instance']->plugin_id;
  $plugin = feeds_tamper_get_plugin($plugin_id);
  if ($plugin['validate']) {
    $plugin['validate']($form_state['values'][$plugin_id]);
  }
  if ($plugin['description']) {
    $form_state['storage']['description'] = $plugin['description']($form_state['values'][$plugin_id]);
  }
  else {
    $form_state['storage']['description'] = trim($form_state['values']['description']);
  }
}

function feeds_tamper_ui_edit_plugin_form_submit($form, &$form_state) {
  $instance = $form_state['instance'];
  $instance->settings = $form_state['values'][$instance->plugin_id];
  $instance->description = $form_state['storage']['description'];
  $plugin = feeds_tamper_get_plugin($instance->plugin_id);

  ctools_export_crud_save('feeds_tamper', $instance);
  drupal_goto('admin/structure/feeds_tamper/edit/' . $instance->importer);
}

function feeds_tamper_ui_delete_form($form, &$form_state, $instance) {
  $importer = l(t('Edit ') . $instance->importer, 'admin/structure/feeds_tamper/edit/' . $instance->importer);
  $crumb = drupal_get_breadcrumb();
  $crumb[] = $importer;
  drupal_set_breadcrumb($crumb);
  $form_state['instance'] = $instance;
  $form = array();
  $form['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  return $form;
}

function feeds_tamper_ui_delete_form_submit($form, &$form_state) {
  ctools_export_crud_delete('feeds_tamper', $form_state['instance']);
  drupal_goto('admin/structure/feeds_tamper/edit/' . $form_state['instance']->importer);
}
