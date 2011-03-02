<?php

/**
 * @file
 * Definition for Maki icons as OpenLayers 2.x styles
 */

function _mapbox_openlayers_styles() {
  global $base_url;

  $icon_templates = array(
    t('Airport') => 'airport-%s.png',
    t('Bank') => 'bank-%s.png',
    t('Building: Collapsed') => 'building-collapsed-%s.png',
    t('Building: Damaged') => 'building-damaged-%s.png',
    t('Bus') => 'bus-%s.png',
    t('Cafe') => 'cafe-%s.png',
    t('Campsite') => 'campsite-%s.png',
    t('Cemetery') => 'cemetery-%s.png',
    t('Cemetery: Christian') => 'cemetery-christian-%s.png',
    t('Cemetery: Jewish') => 'cemetery-jewish-%s.png',
    t('Cemetery: Muslim') => 'cemetery-muslim-%s.png',
    t('Cinema') => 'cinema-%s.png',
    t('Circle') => 'circle-7.png',
    t('Embassy') => 'embassy-%s.png',
    t('Fuel') => 'fuel-%s.png',
    t('Garden') => 'garden-%s.png',
    t('Grocery') => 'grocery-%s.png',
    t('Historic') => 'historic-%s.png',
    t('Infrastructure: Damaged') => 'infrastructure-damaged-%s.png',
    t('Infrastructure: Destroyed') => 'infrastructure-destroyed-%s.png',
    t('Landslide') => 'landslide-%s.png',
    t('Library') => 'library-%s.png',
    t('Lodging') => 'lodging-%s.png',
    t('Medical') => 'medical-%s.png',
    t('Metro') => 'metro-%s.png',
    t('Monument') => 'monument-%s.png',
    t('Museum') => 'museum-%s.png',
    t('Park') => 'park-%s.png',
    t('Parking garage') => 'parking-garage-%s.png',
    t('Parking log') => 'parking-lot-%s.png',
    t('Playground') => 'playground-%s.png',
    t('Port') => 'port-%s.png',
    t('Post') => 'post-%s.png',
    t('Religious: Christian') => 'religious-christian-%s.png',
    t('Religious: Jewish') => 'religious-jewish-%s.png',
    t('Religious: Muslim') => 'religious-muslim-%s.png',
    t('Restaurant') => 'restaurant-%s.png',
    t('School: College') => 'school-college-%s.png',
    t('School: Elementary') => 'school-elementary-%s.png',
    t('Settlement: Temporary') => 'settlement-temporary-%s.png',
    t('Shopping') => 'shopping-%s.png',
    t('Sports') => 'sports-%s.png',
    t('Tourism') => 'tourism-%s.png',
    t('Town Hall') => 'town-hall-%s.png');

  $styles = array();

  foreach ($icon_templates as $icon_title => $icon_template) {
    $icon_url = $base_url . '/' . drupal_get_path('module', 'mapbox') . 
      '/includes/maki/' . sprintf($icon_template, 15);
    $icon_name = strtr(basename($icon_template, '.png'),
      array('-%s' => ''));

    $style = new stdClass();
    $style->api_version = 1;
    $style->name = $icon_name; 
    $style->title = $icon_title;
    $style->description = t('Maki icon');
    $style->data = array(
      'fillOpacity' => 1,
      'externalGraphic' => $icon_url,
      'pointRadius' => 7.5
    );
    $styles[$style->name] = $style;
  }
  return $styles;
}
