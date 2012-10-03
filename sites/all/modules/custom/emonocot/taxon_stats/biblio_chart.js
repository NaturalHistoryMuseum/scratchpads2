/**
 * 
 */

google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawCharts);
      
      function drawCharts(){
        jQuery('.biblio_charts').each(
            function() { 
              drawChart(jQuery(this).attr('id'));
              } 
            );
        
      }
      
      function drawChart(elementID) {
        var data = new google.visualization.DataTable();
        data.addColumn('number', 'Year');
        data.addColumn('number', 'Number of Publications');
        //data.addColumn({type:'string', role:'tooltip'})
        var query = '#'+elementID+'_data';
        jQuery(query+' span').each(
            function() { 
              if (parseInt(jQuery(this).data('year')) != 0) {
                data.addRows([ [ parseInt(jQuery(this).data('year')), parseInt(jQuery(this).data('count'))/*, $tooltip*/]]);
              }
            } 
          );

        var options = {
          title: jQuery(query).data('title'),
          legend: {position: 'none'},
          hAxis: {format: '####'},
        };

        var chart = new google.visualization.ColumnChart(document.getElementById(elementID));
        chart.draw(data, options);
      }
