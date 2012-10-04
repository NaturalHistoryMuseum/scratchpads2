/**
 * 
 */

      google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawCharts);
      
      function drawCharts(){
        jQuery('.ts_charts').each(
            function() { 
              drawChart(jQuery(this).attr('id'));
              } 
            );
        
      }

      function drawChart(elementID) {
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Type');
        data.addColumn('number', 'Count');

        jQuery('#'+elementID+'-data span').each(
            function() { 
              if (parseInt(jQuery(this).data('year')) != 0) {
                data.addRows([ [ jQuery(this).data('type'), parseInt(jQuery(this).data('count'))/*, $tooltip*/]]);
              }
            } 
          );
        
        var options = {
           height: 600,
           //chartArea: {height: 380},
           title: jQuery('#'+elementID+'-data').data('title'),
        };

        var chart = new google.visualization.PieChart(document.getElementById(elementID));
        chart.draw(data, options);
      }