<?php
/**
 * @file
 * OpenLayers Map Content Type. Displays a Map.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  // And this is just the administrative title.
  // All our callbacks are named according to the standard pattern and can be deduced.
  'title' => t('Openlayers Map'),
  'content type' => 'openlayers_openlayers_map_content_type_content_types',
  'render callback' => 'openlayers_map_content_type_render',
);

/**
 * Return all content types available.
 */
function openlayers_openlayers_map_content_type_content_types($plugin) {
  $types = array();
  $maps = openlayers_maps();

  foreach ($maps as $map) {
    $types[$map->name] = array (
      'map' => $map->name,
      'title' => $map->title,
      'description' => $map->description,
      'category' => t('OpenLayers Maps'),
    );
  }

  return $types;
}

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function openlayers_map_content_type_render($subtype, $conf, $args, $context) {
  $map = openlayers_map_load($subtype);

  $block = new stdClass();
  $block->title = $map->title;
  $block->content = openlayers_render_map($map);
  
  return $block;
}

/**
 * Empty form so we can have the default override title.
 */
function openlayers_openlayers_map_content_type_edit_form($form, &$form_state) {
  // Does nothing!
  return $form;
}
