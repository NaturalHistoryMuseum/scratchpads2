(function ($) {
  jQuery(document).ready(function(){
  jQuery('div.flot-with-zoom').each(function() {
    var $mainplot = jQuery(this);
    var $zoomplot = jQuery('#' + $mainplot.attr("id") + '-zoom');

    $mainplot.bind("plotselected", function (event, ranges) {
        mainplot = idToFlotName($mainplot.attr("id"));
        var data = Drupal.flot[mainplot]['data'];
        var plotoptions = Drupal.flot[mainplot]['options'];
        // do the zooming
        if(typeof(ranges.xaxis) != 'undefined'){
          Drupal.flot[mainplot]['flot'] = jQuery.plot($mainplot, data,
                        jQuery.extend(true, {}, plotoptions, {
                            xaxis: { min: ranges.xaxis.from, max: ranges.xaxis.to }
                        }));
        }
        // don't fire event on the overview to prevent eternal loop
        Drupal.flot[idToFlotName($zoomplot.attr("id"))]['flot'].setSelection(ranges, true);
    });

    $zoomplot.bind("plotselected", function (event, ranges) {
        Drupal.flot[idToFlotName($mainplot.attr("id"))]['flot'].setSelection(ranges);
    });

  });

  function idToFlotName (id) {
    return id.replace(/-/g, '_');
  }

  
});
})(jQuery);