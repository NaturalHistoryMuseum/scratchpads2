
AUTHORS
-------

 * Tj Holowaychuk <http://www.350designs.com/>
 * Jimmy Berry ("boombatower", http://drupal.org/user/214218)

DOCUMENATION
------------

 * http://code.google.com/p/drupal-chart-api/w/list
 * chart_build() in chart.module
 * chart.api.php

EXAMPLE USING THEME()
---------------------

$chart = array(
  '#chart_id' => 'test_chart',
  '#title' => t('Servings'),
  '#type' => CHART_TYPE_PIE_3D,
);

$chart['#data']['fruits'] = 3;
$chart['#data']['meats']  = 2;
$chart['#data']['dairy']  = 5;

echo theme('chart', array('chart' => $chart));

EXAMPLE USING RENDERABLE ARRAYS
-------------------------------
Render Arrays in Drupal 7: http://drupal.org/node/930760

$page['chart'] = array(
  '#theme' => 'chart',
  '#chart_id' => 'test_chart',
  '#title' => t('Servings'),
  '#type' => CHART_TYPE_PIE_3D,
  '#data' => array(
    'fruits' => 3,
    'meats' => 2,
    'dairy' => 5,
  );
);

