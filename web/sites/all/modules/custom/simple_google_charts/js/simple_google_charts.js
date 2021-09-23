(function ($) {
    function simpleGoogleChartsStatisticsDrawChart() {
        google.load('visualization', '1.0', {'packages': ['corechart']});
        google.setOnLoadCallback(drawChart);

        function drawChart() {
            $('div.google_chart').once(function () {
                var chart_id = $(this).attr('id');
                var data = new google.visualization.arrayToDataTable($.makeArray(Drupal.settings.scratchpads_statistics[chart_id]['data']));
                var chart = new google.visualization[Drupal.settings.scratchpads_statistics[chart_id]['type']]($('#' + chart_id).get(0));
                chart.draw(data, Drupal.settings.scratchpads_statistics[chart_id]['options']);
            });
        }
    }

    Drupal.behaviors.simple_google_charts = {
        attach: function (context, settings) {
            simpleGoogleChartsStatisticsDrawChart();
        }
    };
})(jQuery);
