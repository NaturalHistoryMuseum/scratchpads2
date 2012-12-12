(function($){
  google.load('visualization', '1.0', {'packages': ['corechart']});
  google.setOnLoadCallback(scratchpadsStatisticsDrawChart);
  function scratchpadsStatisticsDrawChart(){
    $('div.google_chart_pie').once(function(){
      var chart_id = $(this).attr('id');
      var chart_data = Drupal.settings.scratchpads_statistics[chart_id];
      var data = new google.visualization.DataTable();
      data.addColumn('string', chart_data['column_name']);
      data.addColumn('number', chart_data['value_name']);
      for( var key in chart_data.rows) {
        data.addRow([key, Number(chart_data.rows[key])]);
      }
      var options = {'title': chart_data.title, 'width': chart_data.width, 'height': chart_data.height, 'is3D': true};
      var chart = new google.visualization.PieChart(document.getElementById(chart_id));
      chart.draw(data, options);
    });
    $('div.google_chart_bar').once(function(){
      console.log(this);
      var chart_id = $(this).attr('id');
      console.log($.makeArray(Drupal.settings.scratchpads_statistics[chart_id]['data']));
      console.log(Drupal.settings.scratchpads_statistics[chart_id]);
      var data = new google.visualization.arrayToDataTable($.makeArray(Drupal.settings.scratchpads_statistics[chart_id]['data']));
      //var options = {'title': chart_data.title, 'width': chart_data.width, 'height': chart_data.height, 'is3D': true};
      var options = {};
      var chart = new google.visualization.BarChart(document.getElementById(chart_id));
      chart.draw(data, options);
    });
  }
})(jQuery);