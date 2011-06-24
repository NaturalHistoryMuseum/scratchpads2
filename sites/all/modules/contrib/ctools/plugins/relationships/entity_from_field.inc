<?php

/**
 * @file
 * Plugin to provide an relationship handler for an entity from a field.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Entity'),
  'description' => t('Creates an entity context from a foreign key on a field.'),
  'context' => 'ctools_entity_from_field_context',
  'get child' => 'ctools_entity_from_field_get_child',
  'get children' => 'ctools_entity_from_field_get_children',
);

function ctools_entity_from_field_get_child($plugin, $parent, $child) {
  $plugins = ctools_entity_from_field_get_children($plugin, $parent);
  return $plugins[$parent . ':' . $child];
}

function ctools_entity_from_field_get_children($parent_plugin, $parent) {
  ctools_include('fields');
  $entities = entity_get_info();
  $plugins = array();
  $context_types = array();

  // Get the schema information for every field.
  $fields_info = field_info_instances();
  foreach ($fields_info as $from_entity => $entity_info) {
    foreach ($entity_info as $bundle => $fields) {
      // There might be fields attached to bundles that are disabled (e.g. a
      // module that declared a node's content type, is now disabled), but the
      // field instance is still shown.
      if (!empty($entities[$from_entity]['bundles'][$bundle])) {
        foreach ($fields as $field_name => $field) {
          $field = field_info_field($field_name);
          $foreign_keys = NULL;

          if (!empty($field['foreign keys'])) {
            $foreign_keys = $field['foreign keys'];
          }
          else {
            // try to fetch foreign keys from schema, as not everything
            // stores foreign keys properly in the field info.
            $module = $field['module'];
            module_load_install($module);
            $schema = module_invoke($module, 'field_schema', $field);
            if (!empty($schema['foreign keys'])) {
              $foreign_keys = $schema['foreign keys'];
            }
          }

          if (is_array($foreign_keys)) {
            foreach ($foreign_keys as $key => $info) {
              if (isset($info['table'])) {
                foreach ($entities as $to_entity => $to_entity_info) {
                  $from_entity_info = $entities[$from_entity];
                  if (isset($to_entity_info['base table']) && $to_entity_info['base table'] == $info['table'] && isset($info['columns'][$to_entity_info['entity keys']['id']])) {
                    $name = $field_name . '-' . $from_entity . '-' . $to_entity;
                    $plugin_id = $parent . ':' . $name;

                    // Record the bundle for later.
                    $context_types[$plugin_id]['types'][$bundle] = $from_entity_info['bundles'][$bundle]['label'];

                    // We check for every bundle; this plugin may already have
                    // been created, so don't recreate it.
                    if (!isset($plugins[$plugin_id])) {
                      $plugin = $parent_plugin;
                      $replacements = array(
                        '@to_entity' => $to_entity_info['label'],
                        '@from_entity' => $from_entity_info['label'],
                        '@field_name' => ctools_field_label($field_name),
                      );
                      $plugin['title'] = t('@to_entity from @from_entity (on @from_entity: @field_name)', $replacements);
                      $plugin['keyword'] = $to_entity;
                      $plugin['context name'] = $name;
                      $plugin['name'] = $plugin_id;
                      $plugin['description'] = t('Creates a @to_entity context from @from_entity using the @field_name field on @from_entity.', $replacements);
                      $plugin['from entity'] = $from_entity;
                      $plugin['to entity'] = $to_entity;
                      $plugin['field name'] = $field_name;
                      $plugin['parent'] = $parent;

                      $plugins[$plugin_id] = $plugin;

/*
 -- commented out until I figure out how to actually load the reverse properly.
                      // Build the reverse
                      $plugin = $parent_plugin;
                      $name = $field_name . '-' . $from_entity . '-' . $to_entity . '-reverse';
                      $plugin_id = $parent . ':' . $name;

                      $plugin['title'] = t('@from_entity from @to_entity (on @from_entity: @field_name)', $replacements);
                      $plugin['keyword'] = $to_entity;
                      $plugin['context name'] = $name;
                      $plugin['name'] = $plugin_id;
                      $plugin['description'] = t('Creates a @from_entity context from @to_entity using the @field_name field on @from_entity.', $replacements);

                      $plugin['from entity'] = $from_entity;
                      $plugin['to entity'] = $to_entity;
                      $plugin['field name'] = $field_name;
                      $plugin['reverse'] = TRUE;
                      $plugin['parent'] = $parent;

                      // Since we can't apply restrictions on the reverse relationship
                      // we just add the required context here.
                      $plugin['required context'] = new ctools_context_required($to_entity_info['label'], $to_entity);

                      $plugin_entities = array(
                        'to' => array($from_entity => $from_entity_info),
                        'from' => array($to_entity => $to_entity_info)
                      );
                      drupal_alter('ctools_entity_context', $plugin, $plugin_entities, $plugin_id);

                      $plugins[$plugin_id] = $plugin;
*/
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  foreach ($context_types as $key => $context) {
    list($parent, $plugin_name) = explode(':', $key);
    list($field_name, $from_entity, $to_entity) = explode('-', $plugin_name);

    $from_entity_info = $entities[$from_entity];
    $to_entity_info = $entities[$to_entity];

    $plugins[$key]['required context'] = new ctools_context_required($from_entity_info['label'], $from_entity, array('type' => array_keys($context['types'])));

    $plugin_entities = array(
      'to' => array($to_entity => $to_entity_info),
      'from' => array($from_entity => $from_entity_info)
    );
    drupal_alter('ctools_entity_context', $plugins[$key], $plugin_entities, $key);
  }
  drupal_alter('ctools_entity_contexts', $plugins);

  return $plugins;
}

/**
 * Return a new context based on an existing context.
 */
function ctools_entity_from_field_context($context, $conf) {
  $plugin = $conf['name'];
  list($plugin, $plugin_name) = explode(':', $plugin);
  list($field_name, $from_entity, $to_entity_info) = explode('-', $plugin_name);
  // If unset it wants a generic, unfilled context, which is just NULL.
  $entity_info = entity_get_info($from_entity);
  if (empty($context->data) || !isset($context->data->{$entity_info['entity keys']['id']})) {
    return ctools_context_create_empty('entity:' . $to_entity_info, NULL);
  }

  if (isset($context->data->{$entity_info['entity keys']['id']})) {
    // Load the entity.
    $id = $context->data->{$entity_info['entity keys']['id']};
    $entity = entity_load($from_entity, array($id));
    $entity = $entity[$id];
    if (isset($entity->$field_name)) {
      $language = field_language($from_entity, $entity, $field_name);
      $to_entity_info_info = entity_get_info($to_entity_info);
      $to_entity_info_id = $entity->{$field_name}[$language][0][$to_entity_info_info['entity keys']['id']];

      // Send it to ctools.
      return ctools_context_create('entity:' . $to_entity_info, $to_entity_info_id);
    }
  }
}
