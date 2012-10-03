/**
 * 
 */

google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = new google.visualization.DataTable();
        data.addColumn('number', 'Year');
        data.addColumn('number', 'Number of Publications');
        jQuery('#biblio_chart_data span').each(
            function() { 
              if (parseInt(jQuery(this).data('year')) != 0) {
                data.addRows([ [ parseInt(jQuery(this).data('year')), parseInt(jQuery(this).data('count'))]]);
              }
            } 
          );


        var options = {
          title: 'Publications per year',
          legend: {position: 'none'},
          hAxis: {format: '####'},
        };

        var chart = new google.visualization.ColumnChart(document.getElementById('biblio_chart'));
        chart.draw(data, options);
      }
