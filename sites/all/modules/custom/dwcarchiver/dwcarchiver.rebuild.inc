<?php
define('DWCARCHIVER_DATA_FILE_EXTENSION', 'txt');
define('DWCARCHIVER_ARCHIVE_DIRECTORY_NAME', 'dwca/');

function dwcarchiver_rebuild($dwcarchiver, $quiet = TRUE){
  // Create the ZIP file, and add files directly to it.
  $zip_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
  $zip = new ZipArchive();
  if($zip->open($zip_filename, ZipArchive::CREATE) !== TRUE){
    // We've not been able to create the zip, return an error.
    _dwcarchiver_return_error_on_rebuild();
  }
  _dwcarchiver_create_xml_file($dwcarchiver, $zip);
  $zip->close();
  if(!$quiet){return array(
      'message' => array(
        '#markup' => t('Rebuilding the @name archive', array(
          '@name' => $dwcarchiver->name
        ))
      )
    );}
}

/**
 * Simple function that returns a 503 error
 */
function _dwcarchiver_return_error_on_rebuild(){
  // 503 error!
}

/**
 * Helper function to create the XML file for an archive.
 */
function _dwcarchiver_create_xml_file($dwcarchiver, ZipArchive $zip){
  // Firstly, we start with the XML file.
  $archive = new SimpleXMLElement('<archive/>');
  $archive->addAttribute('xmlns', 'http://rs.tdwg.org/dwc/text/');
  $core = $archive->addChild('core');
  _dwcarchiver_add_default_attributes($core);
  $core->addAttribute('rowType', $dwcarchiver->rowtype);
  $files = $core->addChild('files');
  $files->addChild('location', $dwcarchiver->machine_name . '.' . DWCARCHIVER_DATA_FILE_EXTENSION);
  _dwcarchiver_write_xml_for_fields($core, $dwcarchiver->maps, TRUE);
  // Write the data for the core to the archive.
  _dwcarchiver_create_and_add_data_file($dwcarchiver->core_entity, $dwcarchiver->core_bundle, $dwcarchiver->maps, $zip, $dwcarchiver->machine_name);
  foreach($dwcarchiver->extensions as $extension){
    $extension_element = $archive->addChild('extension');
    $files = $extension_element->addChild('files');
    $files->addChild('location', $extension->machine_name . '.' . DWCARCHIVER_DATA_FILE_EXTENSION);
    _dwcarchiver_add_default_attributes($extension_element);
    $extension_element->addAttribute('rowType', $extension->rowtype);
    _dwcarchiver_write_xml_for_fields($extension_element, $extension->maps);
    // Write the data for the extension to the archive.
    _dwcarchiver_create_and_add_data_file($extension->extension_entity, $extension->extension_bundle, $extension->maps, $zip, $extension->machine_name);
  }
  $zip->addFromString(DWCARCHIVER_ARCHIVE_DIRECTORY_NAME . 'meta.xml', $archive->saveXML());
}

/**
 * Write core/extension data to a temporary file, and then add the temporary
 * file to the zip.
 */
function _dwcarchiver_create_and_add_data_file($entity_type, $bundle, $maps, ZipArchive $zip, $machine_name){
  // We create an SQL query that will pull out all the data required for the map
  $info = entity_get_info($entity_type);
  $query = db_select($info['base table'], 'base');
  switch($entity_type){
    case 'taxonomy_term':
      $vid = db_select('taxonomy_vocabulary', 'v')->fields('v', array(
        'vid'
      ))->condition('machine_name', $bundle)->execute()->fetchField();
      $query->condition('vid', $vid);
      break;
    default:
      $query->condition($info['entity keys']['bundle'], $bundle);
      break;
  }
  $processors = dwcarchiver_get_processors();
  foreach($maps as $map_id => $map){
    if(strpos($map->field, 'field_') === 0){
      // Load the processor, and call it!
      $path = drupal_get_path('module', $processors[$map->processor]['module']);
      include_once ($path . '/' . $processors[$map->processor]['file']);
      if(function_exists($processors[$map->processor]['callback'])){
        call_user_func($processors[$map->processor]['callback'], $query, $map, $info, $map_id);
      }
    }else{
      $query->addExpression('base.' . $map->field);
    }
  }
  // Create the temporary file for holding the CSV data
  $csv_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
  $f = fopen($csv_filename, 'w');
  $results = $query->execute();
  while(($row = $results->fetch(PDO::FETCH_ASSOC)) != FALSE){
    fputcsv($f, $row, ',', '"');
  }
  $zip->addFile($csv_filename, DWCARCHIVER_ARCHIVE_DIRECTORY_NAME . $machine_name . '.' . DWCARCHIVER_DATA_FILE_EXTENSION);
  $zip_filename = $zip->filename;
  $zip->close();
  unlink($csv_filename);
  $zip->open($zip_filename);
}

/**
 * Helper function to write out the fields to an XML element
 */
function _dwcarchiver_write_xml_for_fields(SimpleXMLElement $element, $fields, $is_core = FALSE){
  $id_name = 'coreid';
  if($is_core){
    $id_name = 'id';
  }
  $id_element = $element->addChild($id_name);
  $id_element->addAttribute('index', 0);
  $index = 1;
  foreach($fields as $field){
    $field_element = $element->addChild('field');
    $field_element->addAttribute('index', $index);
    $field_element->addAttribute('term', $field->uri);
    $index++;
  }
}

/**
 * Add default attributes for meta.xml
 */
function _dwcarchiver_add_default_attributes(SimpleXMLElement $element){
  // encoding="UTF-8" linesTerminatedBy="\r\n" fieldsTerminatedBy="," fieldsEnclosedBy="&quot;" ignoreHeaderLines="0" rowType="http://rs.tdwg.org/dwc/terms/Taxon"
  $element->addAttribute('encoding', 'UTF-8');
  $element->addAttribute('linesTerminatedBy', '\n');
  $element->addAttribute('fieldsEnclosedBy', '"');
  $element->addAttribute('ignoreHeaderLines', 0);
}