<?php

/**
 * @file
 * Provide field replacement information for core entities and type specific
 * callbacks.
 */

/**
 * Implements hook_entity_info().
 */
function title_entity_info() {
  $info = array();

  $field = array(
    'type' => 'text',
    'cardinality' => 1,
    'translatable' => TRUE,
  );

  $instance = array(
    'required' => TRUE,
    'settings' => array(
      'text_processing' => 0,
    ),
    'widget' => array(
      'weight' => -5,
    ),
  );

  $info['node'] = array(
    'field replacement' => array(
      'title' => array(
        'field' => $field,
        'instance' => array(
          'label' => t('Title'),
          'description' => t('A field replacing node title.'),
        ) + $instance,
      ),
    ),
  );

  if (module_exists('taxonomy')) {
    $info['taxonomy_term'] = array(
      'field replacement' => array(
        'name' => array(
          'field' => $field,
          'instance' => array(
            'label' => t('Name'),
            'description' => t('A field replacing taxonomy term name.'),
          ) + $instance,
        ),
        'description' => array(
          'field' => array(
            'type' => 'text_with_summary',
          ) + $field,
          'instance' => array(
            'required' => FALSE,
            'label' => t('Description'),
            'description' => t('A field replacing taxonomy term description.'),
            'settings' => array(
              'text_processing' => 1,
            ),
          ) + $instance,
          'callbacks' => array(
            'submit' => 'title_field_term_description_submit',
          ),
          'additional keys' => array(
            'format' => 'format',
          ),
        ),
      ),
    );
  }

  if (module_exists('comment')) {
    $info['comment'] = array(
      'field replacement' => array(
        'subject' => array(
          'field' => $field,
          'instance' => array(
            'label' => t('Subject'),
            'description' => t('A field replacing comment subject.'),
          ) + $instance,
        ),
      ),
    );
  }

  return $info;
}

/**
 * Submit callback for the taxonomy term description.
 */
function title_field_term_description_submit(&$values, $legacy_field, $info, $langcode) {
  $values['description'] = array();
  foreach (array('value', 'format') as $key) {
    if (isset($values[$info['field']['field_name']][$langcode][0][$key])) {
      $values['description'][$key] = $values[$info['field']['field_name']][$langcode][0][$key];
    }
  }
}

/**
 * Sync callback for the text field type.
 */
function title_field_text_sync_get($entity_type, $entity, $legacy_field, $info, $langcode) {
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $wrapper->language($langcode);
  return $wrapper->{$info['field']['field_name']}->raw();
}

/**
 * Sync back callback for the text field type.
 */
function title_field_text_sync_set($entity_type, $entity, $legacy_field, $info, $langcode) {
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $wrapper->language($langcode);
  $wrapper->{$info['field']['field_name']}->set($entity->{$legacy_field});
}

/**
 * Sync callback for the text with summary field type.
 */
function title_field_text_with_summary_sync_get($entity_type, $entity, $legacy_field, $info, $langcode) {
  $format_key = $info['additional keys']['format'];
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $wrapper->language($langcode);
  $field_name = $info['field']['field_name'];
  // Return values only if there is any available to process for the current
  // language.
  if (is_array($wrapper->{$field_name}->value())) {
    $entity->{$format_key} = $wrapper->{$field_name}->format->raw();
    return $wrapper->{$field_name}->value->raw();
  }
  return NULL;
}

/**
 * Sync back callback for the text with summary field type.
 */
function title_field_text_with_summary_sync_set($entity_type, $entity, $legacy_field, $info, $langcode) {
  $format_key = $info['additional keys']['format'];
  $value = array('value' => $entity->{$legacy_field}, 'format' => $entity->{$format_key});
  $wrapper = entity_metadata_wrapper($entity_type, $entity);
  $wrapper->language($langcode);
  $wrapper->{$info['field']['field_name']}->set($value);
}
