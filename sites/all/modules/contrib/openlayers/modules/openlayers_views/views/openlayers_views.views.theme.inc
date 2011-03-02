<?php

/**
 * @file
 * This file holds views theme function for style plugin.
 *
 * @ingroup openlayers
 */

/**
 * Implementation of theme function
 */
function theme_openlayers_views_map($args) {
  return (isset($args['title']) ? '<h3>' . $args['title'] . '</h3>' : '') . 
    '<div class="openlayers-views-map">'
    . $args['map']
    . '</div>';
}
