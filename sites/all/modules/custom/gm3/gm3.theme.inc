<?php

/**
 * Simple theme function
 */
function theme_gm3_map($variables){
  // No need to use drupal_static here!
  static $info = array();
  if(!isset($variables['map']['id'])){
    // Could we get this dynamically from hook_theme?
    $variables['map']['id'] = 'gm3-map';
  }
  if(isset($variables['map']['libraries']['point']['convexhull']) && $variables['map']['libraries']['point']['convexhull']){
    // We need to add a convex hull polygon.  Lets do so.
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
    // to the map.  This won't be a problem for viewing a map, but may be an
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
  return '<div data-gm3-class="polygon" class="gm3-tools-button"><p>' . t('Shape') . ' +</p></div>';
}

/**
 * Polyline tools button
 */
function theme_gm3_polyline_button($variables){
  return '<div data-gm3-class="polyline" class="gm3-tools-button"><p>' . t('Line') . ' +</p></div>';
}

/**
 * Polyline tools button
 */
function theme_gm3_rectangle_button($variables){
  return '<div data-gm3-class="rectangle" class="gm3-tools-button"><p>' . t('Rectangle') . ' +</p></div>';
}

/**
 * Point tools button
 */
function theme_gm3_point_button($variables){
  return '<div data-gm3-class="point" class="gm3-tools-button"><p>' . t('Point') . ' +</p></div>';
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
  //foreach($variables['view']->result as $index => $row){
  foreach($variables['rows'] as $index => $row){
    $content = '';
    foreach($none_geo_fields as $key){
      if($key == 'title'){
        // Need a way of setting the title on all entity types.
      }else{
        if(isset($row->{"field_$key"})){
          foreach($row->{"field_$key"} as $key => $value){
            if(is_array($value['rendered'])){
              $content .= drupal_render($value['rendered']);
            }else{
              $content .= $value['rendered'];
            }
          }
        }
      }
    }
    foreach($geo_fields as $key => $type){
      switch($type){
        case 'gm3_polygon':
        case 'gm3_polyline':
          break;
        case 'gm3_point':
          if(!isset($map['libraries']['point'])){
            $map['libraries']['point'] = array(
              'points' => array()
            );
          }
          foreach($row->{"field_$key"} as $item){
            $point = $item['raw'];
            $point['content'] = $content;
            $map['libraries']['point']['points'][] = $point;
          }
          break;
        case 'gm3_combination':
          foreach($row->{"field_$key"} as $item){
            switch($item['raw']['gm3_type']){
              case 'point':
                if(!isset($map['libraries']['point'])){
                  $map['libraries']['point'] = array(
                    'points' => array()
                  );
                }
                $map['libraries']['point']['points'][] = array(
                  'latitude' => $item['raw']['latitude'],
                  'longitude' => $item['raw']['longitude'],
                  'content' => $content
                );
                break;
              case 'polyline':
              case 'polygon':
                module_load_include('functions.inc', 'gm3');
                //gm3_convert_polygon_string
                if(!isset($map['libraries'][$item['raw']['gm3_type']])){
                  $map['libraries'][$item['raw']['gm3_type']] = array(
                    $item['raw']['gm3_type'] . 's' => array()
                  );
                }
                $map['libraries'][$item['raw']['gm3_type']][$item['raw']['gm3_type'] . 's'][] = array(
                  $item['raw']['gm3_type'] => array_pop(gm3_convert_polygon_string($item['raw'][$item['raw']['gm3_type']])),
                  'editable' => FALSE,
                  'content' => $content
                );
                break;
              default:
                // The default - This field type is not defined by the gm3 module,
                // we therefore append call the map_alter function for this
                // field
                $function = "{$item['raw']['gm3_type']}_map_alter";
                if(function_exists($function)){
                  $function($map, $item['raw']);
                }
                break;
            }
          }
          break;
        default:
          // The default - This field type is not defined by the gm3 module,
          // we therefore append call the map_alter function for this
          // field
          $function = "{$item['raw']['gm3_type']}_map_alter";
          if(function_exists($function)){
            $function($map, $item['raw']);
          }
          break;
      }
    }
  }
  return drupal_render(theme('gm3_map', array(
    'map' => $map
  )));
}