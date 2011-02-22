<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function features_test_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_image_default_styles().
 */
function features_test_image_default_styles() {
  $styles = array();

  // Exported image style: features_test
  $styles['features_test'] = array(
    'name' => 'features_test',
    'effects' => array(
      '2' => array(
        'label' => 'Scale',
        'help' => 'Scaling will maintain the aspect-ratio of the original image. If only a single dimension is specified, the other dimension will be calculated.',
        'effect callback' => 'image_scale_effect',
        'form callback' => 'image_scale_form',
        'summary theme' => 'image_scale_summary',
        'module' => 'image',
        'name' => 'image_scale',
        'data' => array(
          'width' => '100',
          'height' => '100',
          'upscale' => 0,
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implementation of hook_node_info().
 */
function features_test_node_info() {
  $items = array(
    'features_test' => array(
      'name' => t('Testing: Features'),
      'base' => 'node_content',
      'description' => t('Content type provided for Features tests.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function features_test_views_api() {
  return array(
    'api' => '3.0-alpha1',
  );
}
