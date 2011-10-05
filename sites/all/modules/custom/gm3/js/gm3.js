(function($){
  Drupal.gm3 = Drupal.gm3 || {};
  Drupal.behaviors.gm3 = {attach: function(context, settings){
    for(map_id in Drupal.settings.gm3.maps) {
      if($('#' + map_id, context).length) {
        try {
          var map = Drupal.settings.gm3.maps[map_id];
          $('#' + map_id, context).height(map['settings']['height']);
          $('#' + map_id, context).width(map['settings']['width']);
          // MapTypeID
          map['settings']['mapTypeId'] = eval(map['settings']['mapTypeId']);
          // Center
          map['settings']['center'] = new google.maps.LatLng(map['settings']['center']['latitude'], map['settings']['center']['longitude']);
          // Map control
          if(map['settings']['mapTypeControlOptions'] && map['settings']['mapTypeControlOptions']['mapTypeIds']) {
            for(map_type in map['settings']['mapTypeControlOptions']['mapTypeIds']) {
              map['settings']['mapTypeControlOptions']['mapTypeIds'][map_type] = eval(map['settings']['mapTypeControlOptions']['mapTypeIds'][map_type]);
            }
          }
          if(map['settings']['mapTypeControlOptions'] && map['settings']['mapTypeControlOptions']['position'] && map['settings']['mapTypeControlOptions']['style']) {
            map['settings']['mapTypeControlOptions']['position'] = eval(map['settings']['mapTypeControlOptions']['position']);
            map['settings']['mapTypeControlOptions']['style'] = eval(map['settings']['mapTypeControlOptions']['style']);
          }
          // PanControlOptions
          if(map['settings']['panControlOptions'] && map['settings']['panControlOptions']['position']) {
            map['settings']['panControlOptions']['position'] = eval(map['settings']['panControlOptions']['position']);
          }
          // rotateControlOptions
          if(map['settings']['rotateControlOptions'] && map['settings']['rotateControlOptions']['position']) {
            map['settings']['rotateControlOptions']['position'] = eval(map['settings']['rotateControlOptions']['position']);
          }
          // scaleControlOptions
          if(map['settings']['scaleControlOptions'] && map['settings']['scaleControlOptions']['position'] && map['settings']['scaleControlOptions']['style']){
            map['settings']['scaleControlOptions']['position'] = eval(map['settings']['scaleControlOptions']['position']);
            map['settings']['scaleControlOptions']['style'] = eval(map['settings']['scaleControlOptions']['style']);
          }
          // streetViewControlOptions
          if(map['settings']['streetViewControlOptions'] && map['settings']['streetViewControlOptions']['position']){
            map['settings']['streetViewControlOptions']['position'] = eval(map['settings']['streetViewControlOptions']['position']);
          }
          // zoomControlOptions
          if(map['settings']['zoomControlOptions'] && map['settings']['zoomControlOptions']['position'] && map['settings']['zoomControlOptions']['style']){
            map['settings']['zoomControlOptions']['position'] = eval(map['settings']['zoomControlOptions']['position']);
            map['settings']['zoomControlOptions']['style'] = eval(map['settings']['zoomControlOptions']['style']);
          }          
          // Create the map
          map['google_map'] = new google.maps.Map(document.getElementById(map_id), map['settings']);
          // Add listeners if we need to
          for(event in map['events']){
            google.maps.event.addListener(map['google_map'], event, eval(map['events'][event]));
          }
        } catch(err) {
          $('#' + map_id, context).html(Drupal.t('There has been an error with your map. Please contact an administrator.'));
        }
      }
    }
  }};
})(jQuery);