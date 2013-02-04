<?php

class MigrateDestinationVariable extends MigrateDestination{

  public function __construct(){
    // No options.
  }

  static public function getKeySchema(){
    return array(
      'name' => array(
        'type' => 'varchar',
        'length' => 255
      )
    );
  }

  public function __toString(){
    return t('Create variables/settings.');
  }

  public function import(stdClass $object, stdClass $row){
    global $s1_domain;
    $map = VariableMigration::variableNames();
    // Is this a theme setting?
    if(strpos($row->name, 'theme') !== false){
      $theme_settings = unserialize($row->value);
      // Is there a logo?
      if(isset($theme_settings['logo_path'])){
        $destination = 'public://' . basename($theme_settings['logo_path']);
        $source = 'http://' . $s1_domain . '/' . ltrim($theme_settings['logo_path'], '/');
        if($data = @file_get_contents($source)){
          $file = file_unmanaged_save_data($data, $destination);
          // Update the theme settings variable
          if($file){
            $sp2_theme_settings = variable_get('theme_settings', array());
            $sp2_theme_settings['logo_path'] = $file;
            variable_set('theme_settings', $sp2_theme_settings);
          }
        }
      }
    }elseif($map[$row->name] == 'lexicon_vids'){
      $unserialized_value = unserialize($row->value);
      if(!empty($unserialized_value)){
        $lexicon_vids = variable_get($row->name, array());
        $lexicon_vids += $unserialized_value;
        variable_set($map[$row->name], $lexicon_vids);
      }
    }else{
      variable_set($map[$row->name], unserialize($row->value));
    }
    $this->numCreated++;
    return array(
      $object->name
    );
  }

  public function fields(){
    return array(
      'name' => t('Name'),
      'value' => t('Value')
    );
  }

  public function rollback(array $ids){
    // We don't do anything here.  Once variables are set, they're set and have
    // overridden the previous value that may or may not have been set.  We can
    // not simply delete the variable.
  }
}