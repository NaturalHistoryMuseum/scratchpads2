<?php
/**
 * @file
 * Provides API documentation.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Alter a chart before it is rendered.
 *
 * @param $chart
 *   An associative array defining a chart.
 * @see chart_build()
 */
function hook_chart_alter(&$chart) {
  $chart['#title'] .= ' (altered)';
}

/**
 * Define chart color schemes.
 *
 * @return
 *   An associative array of color schemes keyed by the scheme name and
 *   containing an array of colors.
 */
function hook_chart_color_schemes() {
  return array(
    'my_scheme' => array(
      'FF8000',
      'FFFFFF',
    ),
  );
}

/**
 * Alter chart color schemes.
 *
 * @param $schemes
 *   An associative array of color schemes, see hook_chart_color_schemes().
 */
function hook_chart_color_schemes_alter(&$schemes) {
  $schemes['default'] = array(
    'FFFFFF',
    '000000',
  );
}
