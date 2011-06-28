/**
 * @file
 * OpenLayers JS test file, utilizing QUnit.
 */
(function ($) {
  $(document).ready(function() {
    module('Utility Functions');
  
    // Test the relate path function
    test('Testing Drupal.openlayers.relatePath correctness', function() {
      var root_path = '/this/is/path.js';
      var domain_path = 'http://www.test.com/this/is/path.js';
      var relative_path = 'this/is/path.js';
      equals(root_path, 
        Drupal.openlayers.relatePath(root_path, 'foo'), 'Root path is correct' );
      equals(domain_path, 
        Drupal.openlayers.relatePath(domain_path, 'foo'), 'Domain path is correct' );
      equals('http://foo.com/this/is/path.js', 
        Drupal.openlayers.relatePath(relative_path, 'http://foo.com/'), 'Relative path is correct' )});

    // Test the object from feature function
    test('Testing object_from_feature correctness', function() {
      var latlonobject = {lat: 5, lon: 10};
      var llobj = Drupal.openlayers.objectFromFeature(latlonobject);
      equals(10, llobj.geometry.x, 'Latitude is correct');
      equals(5,  llobj.geometry.y, 'Latitude is correct');
  
      var wktobject = {wkt: 'POINT(50 40)'};
      var wktobj = Drupal.openlayers.objectFromFeature(wktobject);
  
      equals(50, wktobj.geometry.x, 'Latitude is correct');
      equals(40,  wktobj.geometry.y, 'Latitude is correct');
    });
    
    module('Rendering');
    test('Testing basic rendering', function() {
      // Remove stop_render
      $('.openlayers-map:not(.openlayers-processed)').each(function() {
        var map_id = $(this).attr('id');
        Drupal.settings.openlayers.maps[map_id].stop_render = false;
        Drupal.attachBehaviors($('body'), Drupal.settings);
        ok($('#' + map_id).children().hasClass('olMapViewport'), 'Map ' + map_id + ' rendered');
      });
    });
  });
})(jQuery);