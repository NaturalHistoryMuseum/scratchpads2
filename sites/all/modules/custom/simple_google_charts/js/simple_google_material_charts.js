(function($){
  google.load('visualization', '1.1', {'packages': ['corechart','bar']});
  google.setOnLoadCallback(simpleGoogleChartsStatisticsDrawChart);
  function simpleGoogleChartsStatisticsDrawChart(){
    $('div.google_material_chart').once(function(){
      var chart_id = $(this).attr('id');
      var data = new google.visualization.arrayToDataTable($.makeArray(Drupal.settings.scratchpads_statistics[chart_id]['data']));
      var chart = new google.charts[Drupal.settings.scratchpads_statistics[chart_id]['type']]($('#'+chart_id).get(0));
      chart.draw(data, google.charts.Bar.convertOptions(Drupal.settings.scratchpads_statistics[chart_id]['options']));
    });
  }
  Drupal.behaviors.simple_google_charts = {attach: function(context, settings){
    simpleGoogleChartsStatisticsDrawChart();
  }};
})(jQuery);