<?php

/**
 * @file
 * This file is a fork of the default mappers for fields in Feeds module.
 * 
 * Problem occured when Feeds module called the function _field_feeds_set_target()
 *   and hardcoded the 'und' language for each field. Thus we ought to fork these functions
 *   and alter the callbacks for targets to become as below. 
 */

/**
 * Callback for mapping numerics.
 *
 * Ensure that $value is a numeric to avoid database errors.
 */
function _feeds_et_field_feeds_set_target_numeric($source, $entity, $target, $value) {
  if (!is_array($value)) {
    $value = array($value);
  }
  foreach ($value as $k => $v) {
    if (!is_numeric($v)) {
      unset($value[$k]);
    }
  }
  // Process the target name and language
  $target_parts = explode(':', $target);
  $language_code = end($target_parts);
  $target = $target_parts[0];
  _field_feeds_set_target($source, $entity, $target, $value, FALSE, $language_code);
}

/**
 * Callback for mapping text fields.
 */
function _feeds_et_field_feeds_set_target_text($source, $entity, $target, $value) {
  if (!is_array($value) && $value !== '') {
    $value = array($value);
  }
  // Process the target name and language
  $target_parts = explode(':', $target);
  $language_code = end($target_parts);
  $target = $target_parts[0];
  _feeds_et_field_feeds_set_target($source, $entity, $target, $value, TRUE, $language_code);
}

/**
 * Helper for mapping.
 *
 * When the callback is invoked, $target contains the name of the field the
 * user has decided to map to and $value contains the value of the feed item
 * element the user has picked as a source.
 *
 * @param $source
 *   A FeedsSource object.
 * @param $entity
 *   The entity to map to.
 * @param $target
 *   The target key on $entity to map to.
 * @param $value
 *   The value to map. MUST be an array.
 * @param $input_format
 *   TRUE if an input format should be applied.
 * @param $language_code
 *   The language code for the field data to be saved.
 */
function _feeds_et_field_feeds_set_target($source, $entity, $target, $value, $input_format = FALSE, $language_code = LANGUAGE_NONE) {
  if (empty($value)) {
    return;
  }

  if ($input_format) {
    if (isset($source->importer->processor->config['input_format'])) {
      $format = $source->importer->processor->config['input_format'];
    }
  }

  $info = field_info_field($target);

  // Iterate over all values.
  $i = 0;
  $field = isset($entity->$target) ? $entity->$target : array();
  foreach ($value as $v) {
    if (!is_array($v) && !is_object($v)) {
      $field[$language_code][$i]['value'] = $v;
    }
    if ($input_format) {
      if (isset($format)) {
        $field[$language_code][$i]['format'] = $format;
      }
    }
    if ($info['cardinality'] == 1) {
      break;
    }
    $i++;
  }

  /**
   * @todo: make default node language from settings form.
   */
  global $language;
  $default_language = isset($language->language) ? $language->language : 'en';

  // Assign default language if it language was undefined
  if (!isset($entity->language) || $entity->language == LANGUAGE_NONE) {
    $entity->language = $default_language;
  }
  // Or set the default language to the original node language
  else {
    $default_language = $entity->language;
  }

  // Now add translations
  if ($language_code != $default_language) {
    $handler = entity_translation_get_handler($entity->feeds_item->entity_type, $entity);
    $translation = array(
      'translate' => 0,
      'status' => 1,
      'language' => $language_code,
      'source' => $default_language,
    );
    $handler->setTranslation($translation, $entity);
  }

  $entity->{$target} = $field;
}
