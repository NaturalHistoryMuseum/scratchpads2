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
        var query = '#'+elementID+'_data';
        var data = new google.visualization.DataTable();
        data.addColumn(jQuery(query).data('first-column-type'), 'Year');
        data.addColumn('number', 'Number of Publications');
        //data.addColumn({type:'string', role:'tooltip'})
        jQuery(query+' span').each(
            function() { 
              switch(jQuery(query).data('first-column-type')){
                case 'number':
                  if (parseInt(jQuery(this).data('year')) != 0) {
                    data.addRows([ [ parseInt(jQuery(this).data('year')), parseInt(jQuery(this).data('count'))/*, $tooltip*/]]);
                  }
                  break;
                case 'string':
                  data.addRows([ [ jQuery(this).data('year'), parseInt(jQuery(this).data('count'))/*, $tooltip*/]]);
                  break;
              }
              
            } 
          );

        

        switch(jQuery(query).data('chart-type')){
          case 'column':
            var chart = new google.visualization.ColumnChart(document.getElementById(elementID));
            var options = {
                title: jQuery(query).data('title'),
                legend: {position: 'none'},
                hAxis: {format: '####'},
              };
            break;
          case 'pie':
            var chart = new google.visualization.PieChart(document.getElementById(elementID));
            var options = {
                title: jQuery(query).data('title'),
            };
            break;
        }
        chart.draw(data, options);
      }
