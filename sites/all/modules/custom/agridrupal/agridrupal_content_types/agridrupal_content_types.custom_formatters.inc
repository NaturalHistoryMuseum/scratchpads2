<?php
/**
 * @file
 * agridrupal_content_types.custom_formatters.inc
 */

/**
 * Implements hook_custom_formatters_defaults().
 */
function agridrupal_content_types_custom_formatters_defaults() {
  $export = array();

  $formatter = new stdClass();
  $formatter->disabled = FALSE; /* Edit this to true to make a default formatter disabled initially */
  $formatter->api_version = 2;
  $formatter->name = 'terms_names_and_descriptions';
  $formatter->label = 'Terms names and descriptions';
  $formatter->description = 'It displays referenced terms as name - description, separated by semicolons';
  $formatter->mode = 'php';
  $formatter->field_types = 'taxonomy_term_reference';
  $formatter->code = '$output = "";
foreach ($variables[\'#items\'] as $item) {
  $term = taxonomy_term_load($item[\'tid\']);
  //$term_page =  taxonomy_term_view($term, $view_mode = \'full\');
  //$term_url = taxonomy_term_uri($term);
  //$descr=$term_page[\'#term\']->field_description; 
  if ($output != "")
    $output .= "; ";
  $link = l($term->name . " - " . $term->description, "ag_dlios-by-subjects/" . $term->tid . "/" . $term->description);
  $output .= $link;
}
return $output;';
  $formatter->fapi = '';
  $export['terms_names_and_descriptions'] = $formatter;

  return $export;
}
