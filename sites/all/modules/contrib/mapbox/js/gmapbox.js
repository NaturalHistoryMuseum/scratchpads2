Drupal.gmap.addHandler('gmap',function(elem) {
  var obj = this;
  obj.bind("bootstrap_options", function() {
    var opts = obj.opts;
    var layers = obj.vars.baselayers;
    var maps = Drupal.settings.gmapbox.mapdefs;
    // Use .each to avoid scope issues in CustomTilesUrl
    $.each(layers, function(k, val) {    
      if (k in maps) {
        var minZoom = maps[k].minzoom;
        var maxZoom = maps[k].maxzoom;
        var name = maps[k].title;
    	opts.mapTypes.push(GMapBox(k, name, {minZoom: minZoom, maxZoom: maxZoom}));
    	opts.mapTypeNames.push(k); 
      }
    });
  });
});
