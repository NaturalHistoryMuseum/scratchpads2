<?php

/**
 * Output the schema array of the given XSD file
 *
 */
//module_load_include('class.php', 'schemaxml', 'SchemaXMLBuilder');

$f1 =  'file://' . drupal_realpath(drush_get_option('f1'));
$f2 =  'file://' . drupal_realpath(drush_get_option('f2'));

if (!is_file($f1) || !is_file($f2)) {
  echo "Could not access $f1 or $f2\n";
  return;
}

$parser1 = new SchemaXSDParser($f1);
$schema1 = $parser1->get_schema_array();
//var_dump($schema1);
//return;
$parser2 = new SchemaXSDParser($f2);
$schema2 = $parser2->get_schema_array();

$description = array();
if (!SchemaXSDParser::compare_schemas($schema1, $schema2, $description)) {
  echo "Schemas differ:\n";
  echo implode("\n", $description) . "\n";
} else {
  echo "Schemas are the same.\n";
}