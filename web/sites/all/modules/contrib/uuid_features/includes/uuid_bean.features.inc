<?php
/**
 * @file
 * Features UUID integration for BEAN instances.
 */

/**
 * Implements hook_features_export_options().
 */
function uuid_bean_features_export_options() {
  $options = array();
  $types = variable_get('uuid_features_entity_bean', FALSE);
  if (module_exists("bean_uuid") && !empty($types)) {
    $query = db_select('bean', 'n');
    $query->fields('n', array('bid', 'delta', 'type', 'uuid'))
      ->condition('type', $types)
      ->orderBy('type')
      ->orderBy('delta', 'ASC');
    $beans = $query->execute()->fetchAll();
    foreach ($beans as $bean) {
      $options[$bean->uuid] = t('@type: @delta', array(
        '@type' => $bean->type,
        '@delta' => $bean->delta,
      ));
    }
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function uuid_bean_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['bean'] = 'bean';
  $export['dependencies']['bean_uuid'] = 'bean_uuid';
  $export['dependencies']['uuid_features'] = 'uuid_features';

  uuid_features_load_module_includes();

  $bids = entity_get_id_by_uuid('bean', $data);
  foreach ($bids as $uuid => $bid) {
    // Load the bean matching the $bid.
    $query = new EntityFieldQuery();
    $bean = $query
      // We just want one bean: the one matching
      // the current $bid.
      ->entityCondition('entity_type', 'bean')
      ->propertyCondition('bid', $bid)
      ->range(0, 1)
      ->execute();

    $export['features']['uuid_bean'][$uuid] = $uuid;
    $pipe['bean'][$bean['bean'][$bid]->type] = $bean['bean'][$bid]->type;

    // Check for additional bean plugin module dependencies.
    foreach (module_implements('bean_types') as $module) {
      $result = call_user_func_array($module . '_bean_types', array());
      if (isset($result) && is_array($result)) {
        foreach ($result as $type => $values) {
          if ($type == $bean['bean'][$bid]->type) {
            $export['dependencies'][$module] = $module;
          }
        }
      }
    }

    // drupal_alter() normally supports just one byref parameter. Using
    // the __drupal_alter_by_ref key, we can store any additional parameters
    // that need to be altered, and they'll be split out into additional params
    // for the hook_*_alter() implementations.  The hook_alter signature is
    // hook_uuid_bean_features_export_alter(&$export, &$pipe, $bean).
    $data = &$export;
    $data['__drupal_alter_by_ref'] = array(&$pipe);
    drupal_alter('uuid_bean_features_export', $data, $bean);
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function uuid_bean_features_export_render($module, $data) {
  $translatables = $code = array();

  uuid_features_load_module_includes();

  $code[] = '  $beans = array();';
  $code[] = '';
  foreach ($data as $uuid) {
    $beans = entity_uuid_load('bean', array($uuid), array(), TRUE);
    if (!count($beans)) {
      continue;
    }
    $first_bean = reset($beans);
    $export = clone $first_bean;

    // Unset some things that dont need to be exported.
    unset($export->bid);
    unset($export->rdf_mapping);
    unset($export->changed);
    unset($export->vid);

    // Enable file field exports.
    uuid_features_file_field_export($export, 'bean');

    // Allow other modules to alter the export rendering.
    // The hook_alter signature is:
    // hook_uuid_bean_features_export_render_alter(&$export, $bean, $module);
    drupal_alter('uuid_bean_features_export_render', $export, $bean, $module);

    $json = json_encode($export);
    $export_array = json_decode($json, TRUE);

    $code[] = '  $beans[] = ' . features_var_export($export_array, '  ') . ';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '  return $beans;';
  $code = implode("\n", $code);

  return array('uuid_features_default_beans' => $code);
}

/**
 * Implements hook_features_revert().
 */
function uuid_bean_features_revert($module) {
  uuid_bean_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds beans based on UUID from code defaults.
 */
function uuid_bean_features_rebuild($module) {
  $beans = features_get_default('uuid_bean', $module);

  if (!empty($beans)) {
    // Get info about current beans types available in system.
    $entity_info = bean_entity_info();
    // Loop through the export.
    foreach ($beans as $data) {
      // Double-check that bean can be created/reverted.
      if (!isset($entity_info['bean']['bundles'][$data['type']])) {
        drupal_set_message('Bundle not found for bean of type ' . $data['type'] . '. Bean was not created/reverted.', 'warning');
      }
      else {
        // If this is an update, there will be a by-UUID matching bean.
        $existing = entity_get_id_by_uuid('bean', array($data['uuid']));
        if (!empty($existing)) {
          $bean = entity_load_single('bean', $existing[$data['uuid']]);
          foreach ($data as $key => $value) {
            $bean->$key = $value;
          }
        }
        else {
          // Create a new bean.
          $bean = entity_create('bean', $data);
        }
        // Import file fields.
        uuid_features_file_field_import($bean, 'bean');
        if (!$bean->save()) {
          drupal_set_message('Failed to create ' . $data['type'] . ' bean ' . $data['label'], 'error');
        }
      }
    }
  }
}
