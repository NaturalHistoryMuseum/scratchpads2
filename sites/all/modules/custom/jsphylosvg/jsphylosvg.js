(function($){
  Drupal.behaviors.jsphylosvg = {attach: function(context, settings){
    $('.jsphylosvg', context).each(function(){
      var height = 800;
      var width = 800;
      if(Drupal.settings.jsphylosvg[$(this).attr('id')].type != 'circular') {
        height = Drupal.settings.jsphylosvg[$(this).attr('id')].count * 17;
      }
      var phylocanvas = new Smits.PhyloCanvas(Drupal.settings.jsphylosvg[$(this).attr('id')].data, $(this).attr('id'), width, height, Drupal.settings.jsphylosvg[$(this).attr('id')].type);
    });
  }}
})(jQuery);