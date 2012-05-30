<?php
/**
 * @file
 * Provides API documentation.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Alter a views chart before it is rendered.
 *
 * This hook is executed before chart_alter() so additional changes may be made
 * in chart_alter()
 *
 * @param $chart
 *   An associative array defining a chart.
 * @param $view
 *   The name of the view to which the chart belongs.
 * @param $display
 *   The name of the display to which the chart belongs.
 * @see chart_alter()
 */
function hook_chart_views_alter(&$chart, $view, $display) {
  $chart['#title'] .= ' (altered)';
}
