<?php
// $ID:  $
/**
 * @file
 *   Drupal Module
 * 
 * Created on Jun 9, 2010
 *
 * @author 'dman' Dan Morrison http://coders.co.nz/
 */

/**
 * Page callback. Displays an unordered list of all available vocabularies for
 * export
 *
 * @return
 *   An unordered HTML list
 */
function taxonomy_xml_export() {
  // return the list of vocabularies
  $output = '';
  $vocabularies = taxonomy_get_vocabularies();
  $format_infos = taxonomy_xml_format_info();

  if (empty($vocabularies)) {
    $output .= t('There are no vocabularies present');
  }
  else {
    $export_table = array();
    foreach ($vocabularies as $vocabulary) {
      $row = array();
      $vocabcount = db_query("SELECT count(*) FROM {taxonomy_term_data} WHERE vid = :vid", array(':vid' => $vocabulary->vid))->fetchField();
      $row['name'] = $vocabulary->name . ' ('. $vocabcount . ' ' . t('terms') .')';
      
      foreach ($format_infos as $format_id => $format_info) {
        if (!empty($format_info['create'])) {
          $link = "taxonomy/vocabulary/" . $vocabulary->vid . "/" . $format_id;
          $link = TAXONOMY_XML_VOCAB_PATH . '/' . $vocabulary->machine_name . '/' . $format_id;
          $description = @$format_info['description'];
          $extra = array('attributes' => array('title' => $description));
          $row[$format_id] = l($format_info['name'], $link, $extra);
        }
      }

      $export_table[$vocabulary->vid] = $row;
    }
    $output = theme('table', array('rows' => $export_table));
  }
  return $output;
}


/**
 * Return a flat file representation of the requested vocab
 *
 * Default format is the original custom Drupal XML file.
 * 
 * Constructs and prints result to the screen, with appropriate headers.
 */
function taxonomy_xml_export_vocabulary($vocabulary, $format = 'xml') {
  $vocabulary = is_numeric($vocabulary) ? taxonomy_vocabulary_load($vocabulary) : $vocabulary;
  $vid = $vocabulary->vid;
  
  // Retrieving Vocabulary name
  $vname = drupal_strtolower(str_replace(' ', '_', trim($vocabulary->name)));

  // Load the appropriate library, guess at the format name file
  module_load_include('inc', 'taxonomy_xml', $format . '_format');
  $create_funcname = "taxonomy_xml_{$format}_create";
  $file = $create_funcname($vid);
dpm($file);
return 'done';
  if (!empty($_SERVER['HTTP_USER_AGENT']) && (strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE 5.5') || strpos($_SERVER['HTTP_USER_AGENT'], 'Opera'))) {
    header('Content-Type: application/dummy');
  }
  else {
    header('Content-Type: text/xml; charset=UTF-8');
  }
  if (headers_sent()) {
    echo 'Some data has already been output to browser, can\'t send file';
  }
  header('Content-Length: ' . drupal_strlen($file));
  header("Content-Disposition: attachment; filename=taxonomy_$vname.$format.xml");
  echo $file;
}

/**
 * Return a representation of the requested term
 *
 * Constructs and prints result to the screen, with appropriate headers.
 * 
 * @todo UNFINISHED, currently a stub copy from full vocab export
 */
function taxonomy_xml_export_term($term, $format = 'rdf') {
  $term = is_numeric($term) ? taxonomy_term_load($term) : $term;
  
  if (empty($term)) {
    trigger_error("NULL term loaded");
    dpm(func_get_args());
    return "Failed";
  }
  

  module_load_include('inc', 'taxonomy_xml', $format . '_format');
  $create_funcname = "taxonomy_xml_{$format}_create_term";
  if (! function_exists($create_funcname)) {
    trigger_error("The $format format does not currently support exporting individual terms");
    return "Failed to export term '{$term->name}' ({$term->tid}) as $format";
  }
  $file = $create_funcname($term);

dpm($file);
return 'done';

  if (!empty($_SERVER['HTTP_USER_AGENT']) && (strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE 5.5') || strpos($_SERVER['HTTP_USER_AGENT'], 'Opera'))) {
    header('Content-Type: application/dummy');
  }
  else {
    header('Content-Type: text/xml; charset=UTF-8');
  }
  if (headers_sent()) {
    echo 'Some data has already been output to browser, can\'t send file';
  }
  header('Content-Length: ' . drupal_strlen($file));
  header("Content-Disposition: attachment; filename=taxonomy_$vname.$format.xml");
  echo $file;
}

