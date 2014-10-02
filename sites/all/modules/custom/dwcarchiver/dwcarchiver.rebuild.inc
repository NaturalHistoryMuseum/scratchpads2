<?php

function dwcarchiver_rebuild($dwcarchiver, $deliver = TRUE){
  try{
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
      _dwcarchiver_create_xml_and_data_files($dwcarchiver, $zip);
      $zip->close();
      // Move the zip file to the dwcarchiver folder.
      file_unmanaged_move($zip_filename, 'public://dwcarchiver/' . $dwcarchiver->machine_name . '.zip', FILE_EXISTS_REPLACE);
      if($deliver && !user_access('scratchpads team')){
        drupal_set_message(t('The DwC-A has been rebuilt'));
      }
    }
    $timer = timer_stop('dwcarchiver_rebuild');
    if($deliver){
      if(user_access('scratchpads team')){
        drupal_set_message(t('@time seconds: Archive rebuilt', array(
          '@time' => round($timer['time'] / 1000, 2)
        )));
      }
    }
    if($deliver){
      drupal_goto('admin/config/content/dwcarchiver');
    }
  }
  catch(Exception $e){
    $function = 'drupsl_set_message';
    if(function_exists('drush_log')){
      $function = 'drush_log';
    }
    $function(t('<strong>Unable to rebuild the archive. Please send the following error message to the Scratchpads team:</strong><p>@errormessage</p><pre>@errortrace</pre>', array(
      '@errormessage' => $e->getMessage(),
      '@errortrace' => (!empty($e->query_string) ? $e->query_string . "\n" : '') . $e->getTraceAsString()
    )), 'error');
    if($deliver){
      drupal_goto('admin/config/content/dwcarchiver');
    }
  }
}

/**
 * Helper function to create the XML file for an archive.
 */
function _dwcarchiver_create_xml_and_data_files($dwcarchiver, ZipArchive $zip){
  // Firstly, we start with the XML file.
  // No elegant way of adding namespaces to SimpleXMLElement elements, so we
  // simply read in the namespaces from a string.
  // Note, this is possible using DOMDocument!
  $archive = new SimpleXMLElement('<archive
    xmlns="http://rs.tdwg.org/dwc/text/"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://rs.tdwg.org/dwc/text/ http://rs.tdwg.org/dwc/text/tdwg_dwc_text.xsd"/>');
  //$archive->addAttribute('xmlns', 'http://rs.tdwg.org/dwc/text/');
  $core = $archive->addChild('core');
  _dwcarchiver_add_default_attributes($core);
  $core->addAttribute('rowType', $dwcarchiver->rowtype);
  $files = $core->addChild('files');
  $files->addChild('location', $dwcarchiver->machine_name . '.' . variable_get('dwcarchiver_data_file_extension', 'txt'));
  _dwcarchiver_write_xml_for_fields($core, $dwcarchiver->maps[0], TRUE);
  // Write the data for the core to the archive.
  _dwcarchiver_create_and_add_data_file($dwcarchiver, $dwcarchiver, $zip);
  if(user_access('scratchpads team')){
    drupal_set_message(t('@time seconds: Starting extensions', array(
      '@time' => round(timer_read('dwcarchiver_rebuild') / 1000, 2)
    )));
  }
  foreach($dwcarchiver->extensions as $extension){
    $extension_element = $archive->addChild('extension');
    $files = $extension_element->addChild('files');
    $files->addChild('location', $extension->machine_name . '.' . variable_get('dwcarchiver_data_file_extension', 'txt'));
    _dwcarchiver_add_default_attributes($extension_element);
    $extension_element->addAttribute('rowType', $extension->rowtype);
    _dwcarchiver_write_xml_for_fields($extension_element, $extension->maps[0]);
    // Write the data for the extension to the archive.
    _dwcarchiver_create_and_add_data_file($extension, $dwcarchiver, $zip);
    if(user_access('scratchpads team')){
      drupal_set_message(t('@time seconds: Completed @extension', array(
        '@time' => round(timer_read('dwcarchiver_rebuild') / 1000, 2),
        '@extension' => $extension->name
      )));
    }
  }
  $zip->addFromString(variable_get('dwcarchiver_folder_name', 'dwca') . '/meta.xml', $archive->saveXML());
}

/**
 * Write core/extension data to a temporary file, and then add the temporary
 * file to the zip.
 */
function _dwcarchiver_create_and_add_data_file($archive_or_extension, $dwcarchiver, $zip = FALSE, $limit = FALSE){
  //$entity, $bundle, $maps, ZipArchive $zip, $machine_name, $dwcarchiver){
  // We create an SQL query that will pull out all the data required for the map
  $info = entity_get_info($archive_or_extension->entity);
  $base_query = db_select($info['base table'], 'base');
  if($limit){
    $base_query->range(0, $limit);
  }
  switch($archive_or_extension->entity){
    case 'taxonomy_term':
      $vid = db_select('taxonomy_vocabulary', 'v')->fields('v', array(
        'vid'
      ))->condition('machine_name', $archive_or_extension->bundle)->execute()->fetchField();
      $base_query->condition('base.vid', $vid);
      break;
    default:
      $base_query->condition("base.{$info['entity keys']['bundle']}", $archive_or_extension->bundle);
      break;
  }
  $processors = dwcarchiver_get_processors();
  // Create the temporary file for holding the CSV data
  $csv_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
  $f = fopen($csv_filename, 'w');
  foreach(array_keys($archive_or_extension->maps) as $delta){
    // Keep a track of the joins we've made - we only want to make a join once,
    // and not multiple times.
    $joins = array();
    $query = clone $base_query;
    foreach($archive_or_extension->maps[$delta] as $map_id => $map){
      // Check to see if we need to join to another entity.
      $field = explode(':', $map->field);
      $table = 'base';
      if(count($field) > 1){
        if(empty($joins[$field[0]])){
          // We haven't already joined to this object, so we do so, and keep
          // record of the join!
          $joins[$field[0]] = _dwcarchiver_join_from_field($field[0], $query, $info, $archive_or_extension->entity);
        }
        $table = $joins[$field[0]];
        $field = $field[1];
      }else{
        $field = $field[0];
      }
      // Load the processor, and call it!
      $path = drupal_get_path('module', $processors[$map->processor]['module']);
      include_once ($path . '/' . $processors[$map->processor]['file']);
      if($map->uri == 'coreid'){
        module_load_include('processors.inc', 'dwcarchiver');
        dwcarchiver_processor_core_map($archive_or_extension, $dwcarchiver, $query);
      }else if(function_exists($processors[$map->processor]['callback'])){
        call_user_func($processors[$map->processor]['callback'], $query, $field, $archive_or_extension->entity, $map_id, $map, $table);
      }else{
        drupal_set_message(t('@function does not exist', array(
          '@function' => $processors[$map->processor]['callback']
        )));
      }
    }
    $results = $query->execute();
    while(($row = $results->fetch(PDO::FETCH_ASSOC)) != FALSE){
      fputcsv($f, $row, ',', '"');
    }
  }
  if($zip){
    $zip->addFile($csv_filename, variable_get('dwcarchiver_folder_name', 'dwca') . '/' . $archive_or_extension->machine_name . '.' . variable_get('dwcarchiver_data_file_extension', 'txt'));
    $zip_filename = $zip->filename;
    $zip->close();
    unlink($csv_filename);
    $zip->open($zip_filename);
  }else{
    fclose($f);
    $f = fopen($csv_filename, 'r');
    $rows = array();
    while(($row = fgetcsv($f)) != FALSE){
      $rows[] = $row;
    }
    unlink($csv_filename);
    return array(
      'rows' => $rows,
      'query' => (string)$query
    );
  }
}

/**
 * Helper function for joining to another entity by a field.
 */
function _dwcarchiver_join_from_field($field, SelectQuery $query, $info, $entity){
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
  $index = 0;
  foreach($fields as $field){
    $field_element = 'field';
    if($field->uri == 'coreid'){
      if($is_core){
        $field_element = 'id';
      }else{
        $field_element = 'coreid';
      }
    }
    $field_element = $element->addChild($field_element);
    $field_element->addAttribute('index', $index);
    if($field->uri != 'coreid'){
      $field_element->addAttribute('term', $field->uri);
    }
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
  $element->addAttribute('fieldsTerminatedBy', ',');
  $element->addAttribute('fieldsEnclosedBy', '"');
  $element->addAttribute('ignoreHeaderLines', 0);
}