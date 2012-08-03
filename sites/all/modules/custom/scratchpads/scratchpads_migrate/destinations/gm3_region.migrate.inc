<?php

/**
 * @file
 * Base integration with the Migrate API class.
 */
/**
 * Custom extended MigrateFieldHandler class.
 */
class MigrateGM3CombinationFieldHandler extends MigrateFieldHandler{

  public function __construct(){
    $this->registerTypes(array(
      'gm3_combination'
    ));
  }

  public function prepare(stdClass $entity, array $field_info, array $instance, array $values){
    $arguments = (isset($values['arguments'])) ? $values['arguments'] : array();
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $delta = 0;
    foreach($values as $value){
      $return[$language][$delta]['map']['children']['gm3_region'][$value] = $value;
    }
    return isset($return) ? $return : NULL;
  }
}
