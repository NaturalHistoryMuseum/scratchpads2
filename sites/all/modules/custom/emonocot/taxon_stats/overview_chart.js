/**
 * 
 */

      google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Type');
        data.addColumn('number', 'Count');

        jQuery('#content-by-type-data span').each(
            function() { 
              if (parseInt(jQuery(this).data('year')) != 0) {
                data.addRows([ [ jQuery(this).data('type'), parseInt(jQuery(this).data('count'))/*, $tooltip*/]]);
              }
            } 
          );
        
        
        
        var options = {
           height: 400,
        };

        var chart = new google.visualization.PieChart(document.getElementById('content-by-type'));
        chart.draw(data, options);
      }