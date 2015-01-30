<?php

/**
 * Simple theme function
 */
function theme_gm3_map($variables){
  $map = gm3_get_map($variables);
  return drupal_render($map);
}

/**
 * Function to return a renderable array for a map.
 */
function gm3_get_map($variables){
  // FIXME - This should be changed to dynamically calcuate what GM3 JS
  // libraries are dependent on others.
  $dependencies = array(
    'region' => 'polygon'
  );
  foreach($dependencies as $library => $dependent){
    if(isset($variables['map']['libraries'][$library]) && !isset($variables['map']['libraries'][$dependent])){
      $variables['map']['libraries'][$dependent] = array();
    }
  }
  // No need to use drupal_static here!
  static $info = array();
  if(!isset($variables['map']['id'])){
    // Could we get this dynamically from hook_theme?
    $variables['map']['id'] = 'gm3-map';
  }
  if(isset($variables['map']['libraries']['point']['convexhull']) && $variables['map']['libraries']['point']['convexhull']){
    // We need to add a convex hull polygon. Lets do so.
    if(gm3_load_geophp()){
      $points = array();
      foreach($variables['map']['libraries']['point']['points'] as $point){
        $points[] = new Point($point['latitude'], $point['longitude']);
      }
      $multipoint = new MultiPoint($points);
      $convexhull = json_decode(@$multipoint->convexHull()->out('json'));
      $poly = array();
      foreach($convexhull->coordinates[0] as $latlng){
        $poly[] = array(
          $latlng[1], // WHY OH WHY is this the reverse order of what we expect?
          $latlng[0]
        );
      }
      $variables['map']['libraries']['polygon']['polygons'][] = array(
        'polygon' => $poly,
        'editable' => FALSE,
        'content' => t('Convex hull for the points within it.')
      );
    }
  }
  if(isset($info[$variables['map']['id']])){
    // this map has already been initialized.
    // We can alter settings for the map, but won't be able to add any tools
    // to the map. This won't be a problem for viewing a map, but may be an
    // issue for editing.
    drupal_add_js(array(
      'gm3' => array(
        'maps' => array(
          $variables['map']['id'] => array(
            'libraries' => $variables['map']['libraries']
          )
        )
      )
    ), 'setting');
    return FALSE;
  }else{
    $info[$variables['map']['id']] = TRUE;
    gm3_add($variables['map']);
    $tools = '';
    if(isset($variables['map']['tools'])){
      $tools = '<div class="gm3-tools"><ul id="toolbar-' . $variables['map']['id'] . '"><li class="gm3-clicked"><div data-gm3-class="default" class="gm3-tools-button"><p>' . t('Move') . '</p></div></li><li>' . implode("</li><li>", $variables['map']['tools']) . '</li></ul></div>';
    }
    return array(
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'gm3') . '/css/gm3.css'
        )
      ),
      '#markup' => '<div class="gm3-container">' . $tools . '<div class="gm3-map-wrapper"><div id="' . $variables['map']['id'] . '"></div></div></div>'
    );
  }
}

/**
 * Polygon tools button
 */
function theme_gm3_polygon_button($variables){
  theme('gm3_beautytip', array(
    'selector' => '.gm3-tools-button[data-gm3-class="polygon"]',
    'text' => t('Click to enable adding/removing polygons.  Many sided polygons are simply added by clicking on the extreme points of the polygon.')
  ));
  return '<div data-gm3-class="polygon" class="gm3-tools-button"><p>+ ' . t('Polygon') . '</p></div>';
}

/**
 * Polyline tools button
 */
function theme_gm3_polyline_button($variables){
  theme('gm3_beautytip', array(
    'selector' => '.gm3-tools-button[data-gm3-class="polyline"]',
    'text' => t('Click to enable adding/removing lines.  Lines are added by clicking on the map once to start a line, and then on every point where there is an angle in the line.')
  ));
  return '<div data-gm3-class="polyline" class="gm3-tools-button"><p>+ ' . t('Line') . ' +</p></div>';
}

/**
 * Polyline tools button
 */
function theme_gm3_rectangle_button($variables){
  theme('gm3_beautytip', array(
    'selector' => '.gm3-tools-button[data-gm3-class="rectangle"]',
    'text' => t('Click to enable adding/removing rectangles.<br/>Add a rectangle by clicking on the map at the top left of a rectangle, and then at the bottom right.')
  ));
  return '<div data-gm3-class="rectangle" class="gm3-tools-button"><p>+ ' . t('Rectangle') . '</p></div>';
}

/**
 * Point tools button
 */
function theme_gm3_point_button($variables){
  theme('gm3_beautytip', array(
    'selector' => '.gm3-tools-button[data-gm3-class="point"]',
    'text' => t('Click to enable adding/removing points.<br/>Add points by left clicking the map.<br/>Remove a point by right clicking it.')
  ));
  return '<div data-gm3-class="point" class="gm3-tools-button"><p>+ ' . t('Point') . '</p></div>';
}

/**
 * Theme a GM3 beautytip.
 */
function theme_gm3_beautytip($variables){
  if(function_exists('beautytips_add_beautytips')){
    $options['bt_drupal_help_page'] = array(
      'cssSelect' => $variables['selector'],
      'text' => $variables['text'],
      'closeWhenOthersOpen' => TRUE,
      'shrinkToFit' => TRUE,
      'width' => '300px',
      'spikeLength' => '10',
      'overlap' => '3',
      'positions' => 'right',
      'fill' => '#eee',
      'cornerRadius' => '3',
      'shadow' => TRUE
    );
    beautytips_add_beautytips($options);
  }
}

/**
 * Callback to add a Google map to a specific element ID on the page.
 */
function gm3_add(&$map = array()){
  $map['settings'] = array_merge(gm3_settings(), isset($map['settings']) ? $map['settings'] : array());
  drupal_alter('gm3_map', $map);
  if(isset($map['libraries']) && is_array($map['libraries'])){
    foreach($map['libraries'] as $library => $settings){
      $module = 'gm3';
      if(is_array($settings)){
        $module = isset($settings['module']) ? $settings['module'] : $module;
      }else{
        $map['libraries'][$settings] = $settings;
        unset($map['libraries'][$library]);
        $library = $settings;
      }
      $library = ($module == 'gm3') ? "gm3.$library" : $library;
      drupal_add_library($module, $library);
    }
  }
  drupal_add_library('gm3', 'gm3');
  drupal_add_js(array(
    'gm3' => array(
      'maps' => array(
        $map['id'] => $map
      )
    )
  ), 'setting');
}

/**
 * Theme a GM3 view.
 */
function theme_gm3_view_gm3($variables){
  $geo_fields = array();
  $none_geo_fields = array();
  foreach($variables['view']->field as $field_key => $field){
    if(isset($field->field_info['type']) && substr($field->field_info['type'], 0, 4) == 'gm3_'){
      $geo_fields[$field_key] = $field->field_info['type'];
    }else{
      $none_geo_fields[] = $field_key;
    }
  }
  $map = array(
    'libraries' => array()
  );
  // Because it's not possible to click on two points with the exact same
  // location, we group content that have the exact same location.
  $array_points = array();
  foreach($variables['view']->result as $index => $row){
    $content = $variables['rows'][$index];
    foreach($geo_fields as $key => $type){
      switch($type){
        case 'gm3_rectangle':
        case 'gm3_polygon':
        case 'gm3_polyline':
          foreach($row->{"field_$key"} as $item){
            module_load_include('functions.inc', 'gm3');
            if(!isset($map['libraries'][substr($type, 4)])){
              $map['libraries'][substr($type, 4)] = array(
                substr($type, 4) . 's' => array()
              );
            }
            $map['libraries'][substr($type, 4)][substr($type, 4) . 's'][] = array(
              substr($type, 4) => array_pop(gm3_convert_polygon_string($item['raw'][substr($type, 4)])),
              'editable' => FALSE,
              'content' => $content
            );
          }
          break;
        case 'gm3_point':
          if(!isset($map['libraries']['point'])){
            $map['libraries']['point'] = array(
              'points' => array()
            );
          }
          if(count($row->{"field_$key"})){
            foreach($row->{"field_$key"} as $item){
              $point = $item['raw'];
              if(isset($array_points[$point['latitude']][$point['longitude']])){
                if(!is_array($map['libraries']['point']['points'][$array_points[$point['latitude']][$point['longitude']]]['content'])){
                  $map['libraries']['point']['points'][$array_points[$point['latitude']][$point['longitude']]]['content'] = array(
                    array(
                      'title' => '1',
                      'content' => $map['libraries']['point']['points'][$array_points[$point['latitude']][$point['longitude']]]['content']
                    )
                  );
                }
                $map['libraries']['point']['points'][$array_points[$point['latitude']][$point['longitude']]]['content'][] = array(
                  'title' => count($map['libraries']['point']['points'][$array_points[$point['latitude']][$point['longitude']]]['content']) + 1,
                  'content' => $content
                );
              }else{
                $point['content'] = $content;
                $array_points[$point['latitude']][$point['longitude']] = count($map['libraries']['point']['points']);
                $map['libraries']['point']['points'][] = $point;
              }
            }
          }else{
            if(isset($row->{"field_data_{$key}_{$key}_latitude"})){
              $map['libraries']['point']['points'][] = array(
                'latitude' => $row->{"field_data_{$key}_{$key}_latitude"},
                'longitude' => $row->{"field_data_{$key}_{$key}_longitude"},
                'content' => $content
              );
            }
          }
          foreach(array_keys($map['libraries']['point']['points']) as $key){
            $map['libraries']['point']['points'][$key]['colour'] = 7;
          }
          break;
        case 'gm3_combination':
          foreach($row->_field_data as $id => $values){
            if(empty($values['entity']->{$key})){
              continue;
            }
            foreach(entity_get_info() as $entity_type => $values2){
              if($values2['base table'] == $variables['view']->base_table){
                break;
              }
            }
            $lang = field_language($entity_type, $values['entity'], $values['entity']->{$key});
            foreach($values['entity']->{$key}[$lang] as $item){
              switch($item['gm3_type']){
                case 'point':
                  if(!isset($map['libraries']['point'])){
                    $map['libraries']['point'] = array(
                      'points' => array()
                    );
                  }
                  $map['libraries']['point']['points'][] = array(
                    'latitude' => $item['latitude'],
                    'longitude' => $item['longitude'],
                    'content' => $content
                  );
                  break;
                case 'rectangle':
                case 'polyline':
                case 'polygon':
                  module_load_include('functions.inc', 'gm3');
                  if(!isset($map['libraries'][$item['gm3_type']])){
                    $map['libraries'][$item['gm3_type']] = array(
                      $item['gm3_type'] . 's' => array()
                    );
                  }
                  $map['libraries'][$item['gm3_type']][$item['gm3_type'] . 's'][] = array(
                    $item['gm3_type'] => array_pop(gm3_convert_polygon_string($item[$item['gm3_type']])),
                    'editable' => FALSE,
                    'content' => $content
                  );
                  break;
                default:
                  // The default - This field type is not defined by the gm3
                  // module,
                  // we therefore append call the map_alter function for this
                  // field
                  $function = "{$item['gm3_type']}_map_alter";
                  if(function_exists($function)){
                    $function($map, $item);
                  }
                  break;
              }
            }
          }
          break;
        default:
          // The default - This field type is not defined by the gm3 module,
          // we therefore append call the map_alter function for this
          // field
          foreach($row->{"field_$key"} as $item){
            $function = "{$item['gm3_type']}_map_alter";
            if(function_exists($function)){
              $function($map, $item);
            }
          }
          break;
      }
    }
  }
  return theme('gm3_map', array(
    'map' => $map
  ));
}