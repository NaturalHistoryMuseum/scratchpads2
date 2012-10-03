/**
 * 
 */

google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = [
                     ['Year', 'Publications'],
                   ];
        jQuery('#biblio_chart_data span').each(
            function() { 
              data.push([parseInt(jQuery(this).data('year')), parseInt(jQuery(this).data('count'))]);
              } 
            );
        var data = google.visualization.arrayToDataTable(data);

        var options = {
          title: 'Publications per year',
          legend: {position: 'none'},
          hAxis: {format: '####'},
        };

        var chart = new google.visualization.ColumnChart(document.getElementById('biblio_chart'));
        chart.draw(data, options);
      }
