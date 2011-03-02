<?php

/**
 * @file
 * This file contains styles implementations
 *
 * @ingroup openlayers
 */

/**
 * Style Implementation
 *
 * Internal callback for openlayers style implementation.
 *
 * @return
 *   Array of styles for an OpenLayers map
 */
function _openlayers_openlayers_styles() {
  $styles = array();

  $style = new stdClass();
  $style->api_version = 1;
  $style->name = 'default';
  $style->title = t('Default style');
  $style->description = t('Basic default style.');
  $style->data = array(
    'pointRadius' => '5',
    'fillColor' => '#FFCC66',
    'strokeColor' => '#FF9933',
    'strokeWidth' => '4',
    'fillOpacity' => '0.5'
  );
  $styles[$style->name] = $style;

  $style = new stdClass();
  $style->api_version = 1;
  $style->name = 'invisible';
  $style->title = t('Invisible style');
  $style->description = t('Invisible default style.');
  $style->data = array(
    'pointRadius' => '0',
    'strokeWidth' => '0',
    'fillOpacity' => '0'
  );
  $styles[$style->name] = $style;

  $style = new stdClass();
  $style->api_version = 1;
  $style->name = 'default_select';
  $style->title = t('Default select style');
  $style->description = t('Default style for selected geometries');
  $style->data = array(
    'pointRadius' => '5',
    'fillColor' => '#66CCFF',
    'strokeColor' => '#3399FF',
    'strokeWidth' => '4',
    'fillOpacity' => '0.5'
  );
  $styles[$style->name] = $style;

  return $styles;
}
