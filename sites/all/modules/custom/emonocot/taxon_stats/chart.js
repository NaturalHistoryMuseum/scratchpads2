/**
 * 
 */
      google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawCharts);
      
      function drawCharts(){
        jQuery('#taxon_stats_table .ts_chart').each(
            function() { 
              drawChart(jQuery(this).attr('id'), 
                        parseInt(jQuery(this).data('numerator')), 
                        parseInt(jQuery(this).data('denominator')));
              } 
            );
        
      }
      
      
      function drawChart(elementID, done, notdone) {
        var data = google.visualization.arrayToDataTable([
          ['Done', 'Not Done'],
          ['Done',     done],
          ['No content',      notdone]
        ]);

        var options = {
          legend: {position: 'none'},
          enableInteractivity: false,
          width:20,
          height:20,
          slices: {0:{color: 'grey'}, 1:{color: 'white'}},
          pieSliceBorderColor: 'grey',
          pieSliceText: 'none',
        };

        var chart = new google.visualization.PieChart(document.getElementById(elementID));
        chart.draw(data, options);
      }
