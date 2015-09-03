(function ($) {
  jQuery(document).ready(function () {
    // moet een each worden, meerdere views op 1 pagina
    
    element = $('.has-inbetween-exposed-filter');
    while(!element.hasClass('view') && element.length){
      element = element.parent();
    }
    element = element.find('form');
    element.hide();

    $('div.flot-with-zoom').each(function() {
      var $mainplot = jQuery(this);
        var mainplot = $mainplot.attr("id").replace(/-/g, '_');
      var $zoomplot = jQuery('#' + $mainplot.attr("id") + '-zoom');
        var zoomplot = $zoomplot.attr("id").replace(/-/g, '_');
      
      // set current selection on zoom, only run once!
      mainaxes = Drupal.flot[mainplot]['flot'].getAxes();
      zoomaxes = Drupal.flot[zoomplot]['flot'].getAxes();
      
      if (mainaxes.xaxis.min != zoomaxes.xaxis.min && mainaxes.xaxis.max != zoomaxes.xaxis.max) {
        zoomrange = {
          xaxis: {
            from: mainaxes.xaxis.min,
            to: mainaxes.xaxis.max
          },
          yaxis: {
            from: zoomaxes.yaxis.min,
            to: zoomaxes.yaxis.max
          }
        }
        Drupal.flot[zoomplot]['flot'].setSelection(zoomrange, true);
      }

      divmin = 'div.form-item-' + Drupal.settings.flot.field_name + '-min input';
      divmax = 'div.form-item-' + Drupal.settings.flot.field_name + '-max input';
      
      $mainplot.bind("plotselected", function (event, ranges) {
        // no hard coding of names, element
        if(typeof(ranges.xaxis) != 'undefined'){
          element.find(divmin).val(ranges.xaxis.from / 1000);
          element.find(divmax).val(ranges.xaxis.to / 1000);
          element.submit();
        }
      });
      
      $zoomplot.bind("plotselected", function (event, ranges) {
        // no hard coding of names, element
        element.find(divmin).val(ranges.xaxis.from / 1000);
        element.find(divmax).val(ranges.xaxis.to / 1000);
        element.submit();
      });
    });

  });
})(jQuery);