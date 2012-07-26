<?php

/**
 * @file
 * Support for migrate module
 */

class MigrateLinkFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('link_field'));
  }

  static function arguments($title = NULL, $attributes = NULL, $language = NULL) {
    $arguments = array();
    if (!is_null($title)) {
      $arguments['title'] = $title;
    }
    if (!is_null($attributes)) {
      $arguments['attributes'] = $attributes;
    }
    if (!is_null($language)) {
      $arguments['language'] = $language;
    }
    return $arguments;
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    foreach($values as $delta => $value) {
      $item = array();
      if (isset($arguments['title'])) {
        if (!is_array($arguments['title'])) {
          $item['title'] = $arguments['title'];
        }
        else if (isset($arguments['title'][$delta])) {
          $item['title'] = $arguments['title'][$delta];
        }
      }
      if (isset($arguments['attributes'])) {
        $item['attributes'] = $arguments['attributes'];
      }
      $item['url'] = $value;
      $return[$language][$delta] = $item;
      $delta++;
    }

    return isset($return) ? $return : NULL;
  }
}
