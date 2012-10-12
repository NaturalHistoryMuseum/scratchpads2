/**
 * JavaScript to draw the charts in the taxon_stats module
 * 
 * This file should be included using taxon_stats_add_javascript() which will also
 * include the Google Charts JavScript library
 * 
 */

google.load("visualization", "1", {packages:["corechart"]});
google.setOnLoadCallback(drawCharts);
      
/**
 * Loops through all HTML elements with class="ts-charts" and draws the chart
 * 
 * Called on page Load
 * 
 */
function drawCharts(){
  jQuery('.ts-charts').each(
    function() {
      //Calls drawChart with the id of the target element
      drawChart(jQuery(this).attr('id'));
    } 
  );
        
      }

/**
 *  Draws a chart in the element passed as elementID
 *  
 *  This function assumes the data is stored in HTML as defined in ts_charts.txt
 *  
 * @param elementID
 *   The element in which to draw the chart
 */
function drawChart(elementID) {
  //This function makes a number of jQuery calls, these variables make things pretty
  var jq_data_element  = '#'+elementID+'-data';
        
  //Setup the DataTable
  var data = new google.visualization.DataTable();
  data.addColumn(jQuery(jq_data_element).data('chart-first-column-type'), 'Year');
  data.addColumn('number', 'Number of Publications');

  //Populate the DataTable
  jQuery(jq_data_element+' span').each(
    function() {
      //Need to handle things different if first column is a string or number
      switch(jQuery(jq_data_element).data('chart-first-column-type')){
        case 'number':
          data.addRows([ [ parseInt(jQuery(this).data('title')), parseInt(jQuery(this).data('value'))]]);
          break;
        case 'string':
          data.addRows([ [ jQuery(this).data('title'), parseInt(jQuery(this).data('value'))]]);
          break;
      }  
    } 
  );

  //Prepare the correct type of chart, and set options
  switch(jQuery(jq_data_element).data('chart-type')){
    case 'column':
      var chart = new google.visualization.ColumnChart(document.getElementById(elementID));
      var options = {
        title: jQuery(jq_data_element).data('chart-title'),
        legend: {position: 'none'},
        hAxis: {format: '####'},
      };
      break;
    case 'pie':
      var chart = new google.visualization.PieChart(document.getElementById(elementID));
      var options = {
        title: jQuery(jq_data_element).data('chart-title'),
        height: 600,
        sliceVisibilityThreshold: 0,
      };
      break;
  }
  
  chart.draw(data, options);
}
