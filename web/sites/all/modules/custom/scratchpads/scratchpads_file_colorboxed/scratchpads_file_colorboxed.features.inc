<?php

/**
 * Implements hook_ctools_plugin_api().
 */
function scratchpads_file_colorboxed_ctools_plugin_api(){
  list($module, $api) = func_get_args();
  if($module == "file_entity" && $api == "file_default_displays"){
    return array(
      "version" => "1"
    );
  }else if($module == "field_group" && $api == "field_group"){return array(
      "version" => "1"
    );}
}

/**
 * Implements hook_styles_default_styles().
 */
function scratchpads_file_colorboxed_styles_default_styles(){
  $styles = array();
  // Exported style: colorbox
  $styles['file']['styles']['colorbox'] = array(
    'label' => 'colorbox',
    'description' => '',
    'preset_info' => array(
      'image' => array(
        'colorbox' => array(
          'default preset' => 'original',
          'preset' => 'large'
        )
      ),
      'audio' => array(
        'colorbox' => array(
          'default preset' => 'original',
          'preset' => 'large'
        )
      ),
      'video' => array(
        'colorbox' => array(
          'default preset' => 'original',
          'preset' => 'large'
        )
      ),
      'default' => array(
        'colorbox' => array(
          'default preset' => 'original',
          'preset' => 'large'
        )
      )
    )
  );
  return $styles;
}

/**
 * Implements hook_styles_default_presets_alter().
 */
function scratchpads_file_colorboxed_styles_default_presets_alter(&$presets){
  $styles = styles_default_styles();
  if($styles['file']['styles']['colorbox']['storage'] == STYLES_STORAGE_DEFAULT){
    $presets['file']['containers']['image']['styles']['colorbox'] = array(
      'default preset' => 'original',
      'preset' => 'large'
    );
    $presets['file']['containers']['audio']['styles']['colorbox'] = array(
      'default preset' => 'original'
    );
    $presets['file']['containers']['video']['styles']['colorbox'] = array(
      'default preset' => 'original'
    );
    $presets['file']['containers']['default']['styles']['colorbox'] = array(
      'default preset' => 'original'
    );
  }
}
