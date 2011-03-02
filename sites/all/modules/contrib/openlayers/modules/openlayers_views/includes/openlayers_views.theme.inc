<?php

/**
 * @file
 * This file holds the theme functions for openlayers_views module
 *
 * @ingroup openlayers
 */

/**
 * Theme function for openlayers_views_group_display_item
 *
 * Output of OpenLayers View diplay plugin for grouped items.
 */
function theme_openlayers_views_group_display_item($args) {
  // Build output
  return ((!empty($args['name'])) ?
    '<h2 class="point-title openlayers-views-group-feature-name">'
    . $args['name'] 
    . '</h2>' : '') .
    (!empty($args['description']) ? 
    '<div class="point-content openlayers-views-group-feature-description">'
    . $args['description']
    . '</div>' : '');
}

/**
 * Theme function for openlayers_views_render_feature
 */
function theme_openlayers_views_render_feature($args) {
  return $args['feature'];
}
