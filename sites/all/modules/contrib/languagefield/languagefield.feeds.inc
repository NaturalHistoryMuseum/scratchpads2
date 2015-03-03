<?php
/**
 * @file
 * A feeds mapper for language_field field of languagefield.module.
 */

/**
 * Implements feeds_processor_target_alter().
 *
 * @param $targets array of target fields
 * @param $entity_type
 * @param $bundle_name
 */
function languagefield_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'language_field') {
      $targets[$name] = array(
        'name' => check_plain($instance['label']),
        'callback' => 'languagefield_feeds_set_target',
        'description' => t('The @label field of the node.', array('@label' => $instance['label'])),
      );
      // Avoid confusion with node/user/entity language, which is called 'Language' in the field list.
      $targets[$name]['name'] .= ' (' . t('language field') . ')';
    }
  }
}

/**
 * Implements feed_set_target().
 *
 * @param $source Field mapper source settings.
 * @param $entity The entity to map to.
 * @param $target The target key on $entity to map to.
 * @param $value  The value to map. MUST be an array.
 * @return entity
 */
function languagefield_feeds_set_target($source, $entity, $target, $value) {
  if (empty($value)) {
    return;
  }

  if (!is_array($value)) {
    $value = array($value);
  }

  $info = field_info_field($target);

  // Get lists of all languages and user languages.
  $languages_all = _languagefield_options(LANGUAGEFIELD_LANGUAGES_ALL);
  $languages_user = _languagefield_options(LANGUAGEFIELD_AUTHORS_DEFAULT);

  // Iterate over all values.
  $i = 0;
  $field = isset($entity->$target) ? $entity->$target : array();
  foreach ($value as $v) {
    if (empty($v)) {
      // No value;
    }
    elseif (isset ($languages_all[$v])) {
      $field[LANGUAGE_NONE][$i]['value'] = $v;
    }
    elseif (isset ($languages_user[$v])) {
      $field[LANGUAGE_NONE][$i]['value'] = $v;
    }
    else {
      // No valid language code, search for language name?
      $key = array_search($v, $languages_all);
      if (!$key) {
        $key = array_search($v, $languages_user);
      }

      if ($key) {
        // Found, return language code.
        $field[LANGUAGE_NONE][$i]['value'] = $key;
      }
      else {
//        // Illegal value, pass anyway.
//        $field[LANGUAGE_NONE][$i]['value'] = $v;
      }
    }

    if ($info['cardinality'] == 1) {
      break;
    }
    $i++;
  }
  $entity->{$target} = $field;

  return $entity;
}
