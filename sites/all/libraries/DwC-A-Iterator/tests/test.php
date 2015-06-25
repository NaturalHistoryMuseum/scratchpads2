<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN">
<html>
<head>
<title>Tests</title>
<style type="text/css">
body {
  margin: 0;
  padding: 0;
  background-color: #ccc;
  color: #333;
  font-size: 80%;
  font-family: helvetica, arial, sans;
}

div {
  border: solid 1px black;
  margin: 10px;
  padding: 10px;
  background-color: #eee;
}

p {
  text-align: justify;
}

p.signature {
  text-align: right;
  color: #999;
}
</style>
</head>
<body>
	<div>
<?php
//start_stop_xhprof();
// Load the class
require_once ('../DwCAIterator.class.php');
$tests = array(
  array(
    'func' => 'count_all_core_rows',
    'args' => array(
      'examples/solanaceacesource-small.zip'
    ),
    'result' => 100
  ),
  array(
    'func' => 'get_an_array_of_extensions',
    'args' => array(
      'examples/solanaceacesource-small.zip'
    ),
    'result' => array(
      'http://rs.gbif.org/terms/1.0/TypesAndSpecimen',
      'http://rs.gbif.org/terms/1.0/Reference',
      'http://rs.gbif.org/terms/1.0/Distribution',
      'http://rs.gbif.org/terms/1.0/Image',
      'http://rs.gbif.org/terms/1.0/Description',
      'http://rs.gbif.org/terms/1.0/VernacularName'
    )
  ),
  array(
    'func' => 'count_all_extension_rows',
    'args' => array(
      'examples/solanaceacesource-small.zip'
    ),
    'result' => 500
  ),
  array(
    'func' => 'return_all_related_extension_rows',
    'args' => array(
      'examples/solanaceacesource-small.zip'
    ),
    'result' => 166
  ),
  array(
    'func' => 'count_all_core_rows',
    'args' => array(
      'examples/solanaceacesource.zip'
    ),
    'result' => 14089
  ),
  array(
    'func' => 'count_all_extension_rows',
    'args' => array(
      'examples/solanaceacesource.zip'
    ),
    'result' => 100448
  ),
  /*array(
    'func' => 'return_all_related_extension_rows',
    'args' => array(
      'examples/solanaceacesource.zip'
    ),
    'result' => 166
  )*/
);
$start = microtime(TRUE);
foreach($tests as $test){
  $test_start = microtime(TRUE);
  $result = call_user_func_array($test['func'], $test['args']);
  $func = ucwords(str_replace('_', ' ', $test['func']));
  $time_taken = round(microtime(TRUE) - $test_start, 5);
  echo "<h2>{$func} (<span style=\"font-size:80%\">" . implode(', ', $test['args']) . "</span>)</h2><h3>Time taken: $time_taken seconds</h3>";
  if(serialize($result) == serialize($test['result'])){
    echo "<p>PASSED: <pre>" . print_r($result, 1) . "</pre> == <pre>" . print_r($test['result'], 1) . "</pre></p>";
  }else{
    echo "<p style=\"color:red\">FAILED: <pre>" . print_r($result, 1) . "</pre> != <pre>" . print_r($test['result'], 1) . "</pre></p>";
  }
}
echo "<h2>Total time: " . (round(microtime(TRUE) - $start, 3)) . " seconds</h2>";
//start_stop_xhprof();
/**
 * Count all of the core rows.
 */
function count_all_core_rows($uri){
  // Basic test to iterate over the core file.  This should output xxxxxxx records
  // and should not display ANY errors.
  $dwca = new DwCACore($uri);
  $count = 0;
  foreach($dwca as $key => $record){
    $count++;
  }
  return $count;
}

/**
 * Count how many extensions we have.
 */
function get_an_array_of_extensions($uri){
  $dwca = new DwCACore($uri);
  $extensions = array();
  foreach($dwca->get_extensions() as $type => $extension){
    $extensions[] = $type;
  }
  return $extensions;
}

/**
 * Count ALL rows in each extension file.
 */
function count_all_extension_rows($uri){
  // Basic test to iterate over the core file.  This should output xxxxxxx records
  // and should not display ANY errors.
  $dwca = new DwCACore($uri);
  $count = 0;
  foreach($dwca->get_extensions() as $type => $extension){
    foreach($extension as $row){
      $count++;
    }
  }
  return $count;
}

/**
 * Test that all extension rows are returned for each core item.
 */
function return_all_related_extension_rows($uri){
  $dwca = new DwCACore($uri);
  $count = 0;
  foreach($dwca as $core_key => $core_row){
    foreach($core_row as $column){
      if(!is_string($column)){
        foreach($column as $extension_key => $extension_row){
          $count++;
        }
      }
    }
  }
  return $count;
}

function start_stop_xhprof(){
  static $started = FALSE;
  if(!$started){
    // Enable xhprof
    if(extension_loaded('xhprof')){
      require_once '/usr/share/php/xhprof_lib/utils/xhprof_lib.php';
      require_once '/usr/share/php/xhprof_lib/utils/xhprof_runs.php';
      xhprof_enable(XHPROF_FLAGS_CPU + XHPROF_FLAGS_MEMORY);
      $started = TRUE;
    }
  }else{
    // Close xhprof
    if(extension_loaded('xhprof')){
      $xhprof_data = xhprof_disable();
      $xhprof_runs = new XHProfRuns_Default();
      $xhprof_runs->save_run($xhprof_data, 'dwca-tests');
    }
  }
}
?></div>
</body>
</html>