<?php
define('DWCARCHIVER_DATA_FILE_EXTENSION', 'txt');
define('DWCARCHIVER_ARCHIVE_DIRECTORY_NAME', 'dwca/');

function dwcarchiver_rebuild($dwcarchiver, $deliver = TRUE){
  if(user_access('scratchpads team')){
    timer_start('dwcarchiver_rebuild');
  }
  // Create the dwcarchiver directory.
  $dir = 'public://dwcarchiver';
  file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
  // Create the ZIP file, and add files directly to it.
  $zip_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
  $zip = new ZipArchive();
  if($zip->open($zip_filename, ZipArchive::CREATE) !== TRUE){
    drupal_set_message(t('Unable to rebuild the archive, please contact an administrator.'), 'error');
  }else{
    _dwcarchiver_create_xml_file($dwcarchiver, $zip);
    $zip->close();
    // Move the zip file to the dwcarchiver folder.
    file_unmanaged_move($zip_filename, 'public://dwcarchiver/' . $dwcarchiver->machine_name . '.zip', FILE_EXISTS_REPLACE);
    drupal_set_message(t('The DwC-A has been rebuilt'));
  }
  $timer = timer_stop('dwcarchiver_rebuild');
  if(user_access('scratchpads team')){
    drupal_set_message(t('Archive rebuilt in @time miliseconds', array(
      '@time' => $timer['time']
    )));
  }
  drupal_goto('admin/config/content/dwcarchiver');
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
      $query->condition('base.vid', $vid);
      break;
    default:
      $query->condition("base.{$info['entity keys']['bundle']}", $bundle);
      break;
  }
  $processors = dwcarchiver_get_processors();
  // Keep a track of the joins we've made - we only want to make a join once,
  // and not multiple times.
  $joins = array();
  foreach($maps as $map_id => $map){
    // Check to see if we need to join to another entity.
    $field = explode(':', $map->field);
    $table = 'base';
    if(count($field) > 1){
      if(empty($joins[$field[0]])){
        // We haven't already joined to this object, so we do so, and keep @author simor
        // record of the join!
        $joins[$field[0]] = _dwcarchiver_join_from_field($field[0], $query, $info, $entity_type);
      }
      $table = $joins[$field[0]];
      $field = $field[1];
    }else{
      $field = $field[0];
    }
    // Load the processor, and call it!
    $path = drupal_get_path('module', $processors[$map->processor]['module']);
    include_once ($path . '/' . $processors[$map->processor]['file']);
    if(function_exists($processors[$map->processor]['callback'])){
      call_user_func($processors[$map->processor]['callback'], $query, $field, $info, $map_id, $table);
    }
  }
  // Create the temporary file for holding the CSV data
  $csv_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
  $f = fopen($csv_filename, 'w');
  try{
    $results = $query->execute();
  }
  catch(Exception $e){
    echo (string)$query;
    print_r($e);
    exit();
  }
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
 * Helper function for joining to another entity by a field.
 */
function _dwcarchiver_join_from_field($field, SelectQuery $query, $info, $entity_type){
  switch($field){
    case 'parent':
      $query->leftJoin('taxonomy_term_hierarchy', $field, $field . ".tid = base.{$info['entity keys']['id']}");
      return $query->leftJoin('taxonomy_term_data', "{$field}_term_data", "{$field}_term_data.tid = {$field}.parent");
    case 'user':
      return $query->leftJoin('users', "{$field}", "{$field}.uid = base.uid");
    default:
      // No doubt have a field here, so we load it, and work out what we need
      // to join to.
      $field_info = field_info_field($field);
      if($field_info){
        switch($field_info['type']){
          case 'taxonomy_term_reference':
            $query->leftJoin('field_data_' . $field, "{$field}_field", "{$field}_field.entity_id = base.{$info['entity keys']['id']}");
            return $query->leftJoin('taxonomy_term_data', $field, "{$field}_field.{$field}_tid = {$field}.tid");
          case 'node_reference':
            $query->leftJoin('field_data_' . $field, "{$field}_field", "{$field}_field.entity_id = base.{$info['entity keys']['id']}");
            return $query->leftJoin('node', $field, "{$field}_field.{$field}_nid = {$field}.nid");
          default:
            print_r($field_info);
            exit();
        }
      }else{
        // We shouldn't be here, but we are - what has gone wrong?
      }
  }
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