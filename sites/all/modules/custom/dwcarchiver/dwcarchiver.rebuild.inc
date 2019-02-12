<?php

function dwcarchiver_rebuild($dwcarchiver, $deliver = true)
{
  try {
    if (user_access('scratchpads team')) {
      timer_start('dwcarchiver_rebuild');
    }
    // Create the dwcarchiver directory.
    $dir = 'public://dwcarchiver';
    file_prepare_directory($dir, FILE_CREATE_DIRECTORY);
    // Create the ZIP file, and add files directly to it.
    $zip_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
    $zip = new ZipArchive();
    if ($zip->open($zip_filename, ZipArchive::CREATE) !== true) {
      drupal_set_message(t('Unable to rebuild the archive, please contact an administrator.'), 'error');
    } else {
      _dwcarchiver_create_xml_and_data_files($dwcarchiver, $zip);
      $zip->close();
      // Move the zip file to the dwcarchiver folder.
      file_unmanaged_move($zip_filename, 'public://dwcarchiver/' . $dwcarchiver->machine_name . '.zip', FILE_EXISTS_REPLACE);
      if ($deliver && !user_access('scratchpads team')) {
        drupal_set_message(t('The DwC-A has been rebuilt'));
      }
    }
    $timer = timer_stop('dwcarchiver_rebuild');
    if ($deliver) {
      if (user_access('scratchpads team')) {
        drupal_set_message(t('@time seconds: Archive rebuilt', array(
          '@time' => round($timer['time'] / 1000, 2)
        )));
      }
    }
    if ($deliver) {
      drupal_goto('admin/config/content/dwcarchiver');
    }
  } catch (Exception $e) {
    $function = 'drupsl_set_message';
    if (function_exists('drush_log')) {
      $function = 'drush_log';
    }
    $function(t('<strong>Unable to rebuild the archive. Please send the following error message to the Scratchpads team:</strong><p>@errormessage</p><pre>@errortrace</pre>', array(
      '@errormessage' => $e->getMessage(),
      '@errortrace' => (!empty($e->query_string) ? $e->query_string . "\n" : '') . $e->getTraceAsString()
    )), 'error');
    if ($deliver) {
      drupal_goto('admin/config/content/dwcarchiver');
    }
  }
}

/**
 * Helper function to create the XML file for an archive.
 */
function _dwcarchiver_create_xml_and_data_files($dwcarchiver, ZipArchive $zip)
{
  // Firstly, we start with the XML file.
  // No elegant way of adding namespaces to SimpleXMLElement elements, so we
  // simply read in the namespaces from a string.
  // Note, this is possible using DOMDocument!
  $archive = new SimpleXMLElement('<archive
    metadata="eml.xml"
    xmlns="http://rs.tdwg.org/dwc/text/"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://rs.tdwg.org/dwc/text/ http://rs.tdwg.org/dwc/text/tdwg_dwc_text.xsd"/>');
  // $archive->addAttribute('xmlns', 'http://rs.tdwg.org/dwc/text/');
  $core = $archive->addChild('core');
  _dwcarchiver_add_default_attributes($core);
  $core->addAttribute('rowType', $dwcarchiver->rowtype);
  $files = $core->addChild('files');
  $files->addChild('location', $dwcarchiver->machine_name . '.' . variable_get('dwcarchiver_data_file_extension', 'csv'));
  _dwcarchiver_write_xml_for_fields($core, $dwcarchiver->maps[0], true);
  // Write the data for the core to the archive.
  _dwcarchiver_create_and_add_data_file($dwcarchiver, $dwcarchiver, $zip);
  if (user_access('scratchpads team')) {
    drupal_set_message(t('@time seconds: Starting extensions', array(
      '@time' => round(timer_read('dwcarchiver_rebuild') / 1000, 2)
    )));
  }
  foreach ($dwcarchiver->extensions as $extension) {
    $extension_element = $archive->addChild('extension');
    $files = $extension_element->addChild('files');
    $files->addChild('location', $extension->machine_name . '.' . variable_get('dwcarchiver_data_file_extension', 'csv'));
    _dwcarchiver_add_default_attributes($extension_element);
    $extension_element->addAttribute('rowType', $extension->rowtype);
    _dwcarchiver_write_xml_for_fields($extension_element, $extension->maps[0]);
    // Write the data for the extension to the archive.
    _dwcarchiver_create_and_add_data_file($extension, $dwcarchiver, $zip);
    if (user_access('scratchpads team')) {
      drupal_set_message(t('@time seconds: Completed @extension', array(
        '@time' => round(timer_read('dwcarchiver_rebuild') / 1000, 2),
        '@extension' => $extension->name
      )));
    }
  }
  $zip->addFromString(variable_get('dwcarchiver_folder_name', 'dwca') . '/meta.xml', $archive->saveXML());
  $zip->addFromString(variable_get('dwcarchiver_folder_name', 'dwca') . '/eml.xml', _dwcarchiver_get_eml($dwcarchiver));
}

/**
 *
 * @return XML string
 */
function _dwcarchiver_get_eml($dwcarchiver)
{
  $eml = new SimpleXMLElement('<eml:eml
    xmlns:eml="eml://ecoinformatics.org/eml-2.1.1"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:dc="http://purl.org/dc/terms/"
    xsi:schemaLocation="eml://ecoinformatics.org/eml-2.1.1 eml.xsd"
    xml:lang="en"
    packageId="619a4b95-1a82-4006-be6a-7dbe3c9b33c5/v7" system="http://gbif.org" scope="system"/>');
  $dataset = $eml->addChild('dataset', null, '');
  $title = $dataset->addChild('title', variable_get('site_name', 'Scratchpad'));
  $title->addAttribute('xml:lang', 'en', 'xml');
  $add_to = $dataset->addChild('creator');
  $contact = $dataset->addChild('contact');
  foreach (array(
    $contact,
    $add_to
  ) as $add_to) {
    $individualName = $add_to->addChild('individualName');
    $primary_author = _dwcarchiver_get_primary_author();
    $individualName->addChild('givenName', $primary_author->field_user_given_names[LANGUAGE_NONE][0]['value']);
    $individualName->addChild('surName', $primary_author->field_user_family_name[LANGUAGE_NONE][0]['value']);
    if (!empty($primary_author->field_user_institution[LANGUAGE_NONE][0]['value'])) {
      $add_to->addChild('organizationName', $primary_author->field_user_institution[LANGUAGE_NONE][0]['value']);
    }
    $add_to->addChild('electronicMailAddress', $primary_author->mail);
    if (!empty($primary_author->field_homepage[LANGUAGE_NONE][0]['url'])) {
      $add_to->addChild('onlineUrl', $primary_author->field_homepage[LANGUAGE_NONE][0]['url']);
    }
  }
  $metadataProvider = $dataset->addChild('metadataProvider');
  $individualName = $metadataProvider->addChild('individualName');
  $individualName->addChild('givenName', 'Scratchpads');
  $individualName->addChild('surName', 'Team');
  $address = $metadataProvider->addChild('address');
  $address->addChild('deliveryPoint', 'Natural History Museum, Cromwell Road');
  $address->addChild('city', 'London');
  $address->addChild('administrativeArea', 'London');
  $address->addChild('postalCode', 'SW7 5BD');
  $address->addChild('country', 'UK');
  $metadataProvider->addChild('electronicMailAddress', 'scratchpads@nhm.ac.uk');
  $metadataProvider->addChild('onlineUrl', 'http://scratchpads.eu/');
  foreach (_dwcarchiver_get_maintainers() as $other_maintainer) {
    if ($other_maintainer->uid == $primary_author->uid) {
      continue;
    }
    $associatedParty = $dataset->addChild('associatedParty');
    $individualName = $associatedParty->addChild('individualName');
    $individualName->addChild('givenName', $other_maintainer->field_user_given_names[LANGUAGE_NONE][0]['value']);
    $individualName->addChild('surName', $other_maintainer->field_user_family_name[LANGUAGE_NONE][0]['value']);
    $associatedParty->addChild('electronicMailAddress', $other_maintainer->mail);
    $associatedParty->addChild('role', 'pointOfContact');
  }
  $dataset->addChild('language', 'en');
  $dataset->addChild('pubData', date('Y-m-d'));
  $distribution = $dataset->addChild('distribution');
  $distribution->addAttribute('scope', 'document');
  $online = $distribution->addChild('online');
  $biological_vids = variable_get('biological_vids', array());
  $vids = array_filter($biological_vids);
  $vids = array_keys($biological_vids);
  $vid = array_shift($vids);
  $url = $online->addChild('url', url('classification/' . $vid, array(
    'absolute' => true
  )));
  $url->addAttribute('function', 'information');
  $coverage = $dataset->addChild('coverage');
  $taxonomicCoverage = $coverage->addChild('taxonomicCoverage');
  $root_terms = taxonomy_get_tree(4, 0, 1);
  foreach ($root_terms as $term) {
    $taxonomicClassification = $taxonomicCoverage->addChild('taxonomicClassification');
    $taxonomicClassification->addChild('taxonRankValue', $term->name);
  }
  $project = $dataset->addChild('project');
  $project->addChild('title', variable_get('site_name', 'Scratchpad'));
  $additionalMetadata = $eml->addChild('additionalMetadata', null, '');
  $metadata = $additionalMetadata->addChild('metadata');
  $gbif = $metadata->addChild('gbif');
  $gbif->addChild('dateStamp', date('Y-m-d'));
  $gbif->addChild('hierarchyLevel', 'dataset');
  $citation = $gbif->addChild('citation', variable_get('dwcarchiver_alternative_citation', url('', array(
    'absolute' => true
  )) . ' - ' . variable_get('site_name', 'Scratchpad')));
  if (($identifier = variable_get('dwcarchiver_alternative_identifier', false)) != false) {
    $citation->addAttribute('identifier', $identifier);
  }
  $intrights = $dataset->addChild('intellectualRights');
  $licence = _dwcarchiver_get_licence($dwcarchiver);
  $cc_markup = creative_commons_display($licence, variable_get('creative_commons_block_jurisdiction', ''), CC_URL);
  $para = $intrights->addChild('para', 'This work is licensed under a Creative Commons 3.0 License - https:' . $cc_markup['#markup']);
  $ulink = $para->addChild('ulink');
  $ulink->addAttribute('url', 'https:' . $cc_markup['#markup']);
  return $eml->saveXML();
  // Due to the slightly silly structure of the para element, we need to use the
  // DOM functions to manipulate the XML.
  /*
   * $dom = new DOMDocument(); $dom->loadXML($eml->asXML()); $para = new
   * DOMDocument(); $cc_markup =
   * creative_commons_display(variable_get('creative_commons_block_licence_type',
   * CC_BY), variable_get('creative_commons_block_jurisdiction', ''), CC_URL);
   * $para->loadXML('<intellectualRights><para>This work is licensed under a
   * <ulink url="https:' . array_pop($cc_markup) . '"><citetitle>Creative
   * Commons 3.0 License</citetitle></ulink>.</para></intellectualRights>');
   * $node = $para->getElementsByTagName('intellectualRights')->item(0); $para =
   * $dom->importNode($node, TRUE); // print_r($dom);exit; $node =
   * $dom->getElementsByTagName('dataset')->item(0); $node->appendChild($para);
   * return $dom->saveXML();
   */
}

/**
 * Get other maintainers.
 */
function _dwcarchiver_get_maintainers()
{
  $query = db_select('users', 'u');
  $query->fields('u', array(
    'uid'
  ));
  $query->innerJoin('users_roles', 'ur', 'u.uid = ur.uid');
  $query->innerJoin('role', 'r', 'r.rid = ur.rid');
  $query->innerJoin('field_data_field_user_family_name', 'fn', 'fn.entity_id = u.uid');
  $query->innerJoin('field_data_field_user_given_names', 'gn', 'gn.entity_id = u.uid');
  $query->condition('r.name', 'maintainer');
  $query->condition('u.status', '1');
  $query->orderBy('u.uid');
  // Limit to 10 maintainers. If we have more than that, then our users have
  // misunderstood the role of maintainer.
  $query->range(0, 10);
  $uids = $query->execute()->fetchCol();
  return user_load_multiple($uids);
}

/**
 * Get the licence for this DwC-A
 * If GBIF needs to use scratchpads_gbif_registry_client_registration_licence
 */
function _dwcarchiver_get_licence($dwcarchiver)

{
  switch ($dwcarchiver->machine_name) {
    case 'gbif-dwca':
      $licence = variable_get('scratchpads_gbif_registry_client_registration_licence', CC_0);
      break;
    default:
      $licence = variable_get('creative_commons_block_licence_type', CC_BY);
      break;
  }
  return $licence;
}

/**
 * Get first author from variable_get('dwcarchiver_author')
 * Or default to the first maintainer.
 */
function _dwcarchiver_get_primary_author()
{
  $dwcarchiver_author_uid = variable_get('dwcarchiver_author', null);
  if (!$dwcarchiver_author_uid) {
    $query = db_select('users', 'u');
    $query->fields('u', array(
      'uid'
    ));
    $query->innerJoin('users_roles', 'ur', 'u.uid = ur.uid');
    $query->innerJoin('role', 'r', 'r.rid = ur.rid');
    $query->innerJoin('field_data_field_user_family_name', 'fn', 'fn.entity_id = u.uid');
    $query->innerJoin('field_data_field_user_given_names', 'gn', 'gn.entity_id = u.uid');
    $query->condition('r.name', 'maintainer');
    $query->condition('u.status', '1');
    $query->orderBy('u.uid');
    $query->range(0, 1);
    $uid = $query->execute()->fetchAssoc();
    $dwcarchiver_author_uid = $uid['uid'];
  }
  return user_load($dwcarchiver_author_uid);
}

/**
 * Write core/extension data to a temporary file, and then add the temporary
 * file to the zip.
 */
function _dwcarchiver_create_and_add_data_file($archive_or_extension, $dwcarchiver, $zip = false, $limit = false)
{
  // $entity, $bundle, $maps, ZipArchive $zip, $machine_name, $dwcarchiver){
  // We create an SQL query that will pull out all the data required for the map
  $info = entity_get_info($archive_or_extension->entity);
  $base_query = db_select($info['base table'], 'base');
  if ($limit) {
    $base_query->range(0, $limit);
  }
  switch ($archive_or_extension->entity) {
    case 'taxonomy_term':
      $vids = db_select('taxonomy_vocabulary', 'v')->fields('v', array(
        'vid'
      ))->condition('machine_name', $archive_or_extension->bundles)->execute()->fetchCol();
      $base_query->condition('base.vid', $vids);
      break;
    case 'user':
      // Prevent returning of Scratchpads Team, and Anonymous
      $base_query->condition('base.uid', 1, '>');
      break;
    default:
      $base_query->condition("base.{$info['entity keys']['bundle']}", $archive_or_extension->bundles[0]);
      break;
  }
  $processors = dwcarchiver_get_processors();
  // Create the temporary file for holding the CSV data
  $csv_filename = drupal_tempnam(file_directory_temp(), 'dwcarchiver');
  $f = fopen($csv_filename, 'w');
  foreach (array_keys($archive_or_extension->maps) as $delta) {
    // Keep a track of the joins we've made - we only want to make a join once,
    // and not multiple times.
    $joins = array();
    $query = clone $base_query;
    foreach ($archive_or_extension->maps[$delta] as $map_id => $map) {
      // Check to see if we need to join to another entity.
      $field = explode(':', $map->field);
      $table = 'base';
      if (count($field) > 1) {
        if (empty($joins[$field[0]])) {
          // We haven't already joined to this object, so we do so, and keep
          // record of the join!
          $joins[$field[0]] = _dwcarchiver_join_from_field($field[0], $query, $info, $archive_or_extension->entity);
        }
        $table = $joins[$field[0]];
        $field = $field[1];
      } else {
        $field = $field[0];
      }
      // Load the processor, and call it!
      $path = drupal_get_path('module', $processors[$map->processor]['module']);
      include_once($path . '/' . $processors[$map->processor]['file']);
      if ($map->uri == 'coreid' && $map->processor != 'dwcarchiver_value') {
        module_load_include('processors.inc', 'dwcarchiver');
        dwcarchiver_processor_core_map($archive_or_extension, $dwcarchiver, $query);
      } else if (function_exists($processors[$map->processor]['callback'])) {
        call_user_func($processors[$map->processor]['callback'], $query, $field, $archive_or_extension->entity, $map_id, $map, $table);
      } else {
        drupal_set_message(t('@function does not exist', array(
          '@function' => $processors[$map->processor]['callback']
        )));
      }
    }
    $results = $query->execute();
    while (($row = $results->fetch(PDO::FETCH_ASSOC)) != false) {
      /*
       * foreach($row as $key => $value){ $row[$key] = str_replace("\r", "",
       * $value); // TEMPORARY UNTIL EOL FIX THEIR DwC-A parser. // // EOL are
       * not currently able to parse line breaks WITHIN values (i.e. // those
       * within the defined value enclosure for the CSV file. For this //
       * reason, we need to escape the new lines to prevent them from causing //
       * issues. $row[$key] = str_replace("\n", '\\n', $row[$key]); }
       */
      fputcsv($f, $row, ',', '"');
    }
  }
  if ($zip) {
    $zip->addFile($csv_filename, variable_get('dwcarchiver_folder_name', 'dwca') . '/' . $archive_or_extension->machine_name . '.' . variable_get('dwcarchiver_data_file_extension', 'csv'));
    $zip_filename = $zip->filename;
    $zip->close();
    unlink($csv_filename);
    $zip->open($zip_filename);
  } else {
    fclose($f);
    $f = fopen($csv_filename, 'r');
    $rows = array();
    while (($row = fgetcsv($f)) != false) {
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
function _dwcarchiver_join_from_field($field, SelectQuery $query, $info, $entity)
{
  switch ($field) {
    case 'feeds_item':
      $alias = $query->leftJoin('feeds_item', $field, $field . ".entity_id = base.{$info['entity keys']['id']}");
      $or = db_or()->condition($alias . '.entity_type', $entity)->condition($alias . '.entity_type', null);
      $query->condition($or);
      return $alias;
    case 'parent':
      $query->leftJoin('taxonomy_term_hierarchy', $field, $field . ".tid = base.{$info['entity keys']['id']}");
      return $query->leftJoin('taxonomy_term_data', "{$field}_term_data", "{$field}_term_data.tid = {$field}.parent");
    case 'special_biblio_back_reference':
      $query->leftJoin('field_data_field_taxonomic_name', $field, "({$field}.field_taxonomic_name_tid = base.{$info['entity keys']['id']} AND {$field}.entity_type = 'node')");
      $alias = $query->leftJoin('node', "special_biblio_back_reference_node", "(special_biblio_back_reference_node.nid = {$field}.entity_id AND 'special_biblio_back_reference_node.type' = 'biblio')");
      return $alias;
    case 'user':
      return $query->leftJoin('users', "{$field}", "{$field}.uid = base.uid");
    default:
      // No doubt have a field here, so we load it, and work out what we need
      // to join to.
      $field_info = field_info_field($field);
      if ($field_info) {
        switch ($field_info['type']) {
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
      } else {
        // We shouldn't be here, but we are - what has gone wrong?
      }
  }
}

/**
 * Helper function to write out the fields to an XML element
 */
function _dwcarchiver_write_xml_for_fields(SimpleXMLElement $element, $fields, $is_core = false)
{
  $index = 0;
  foreach ($fields as $field) {
    $field_element = 'field';
    if ($field->uri == 'coreid') {
      if ($is_core) {
        $field_element = 'id';
      } else {
        $field_element = 'coreid';
      }
    }
    $field_element = $element->addChild($field_element);
    $field_element->addAttribute('index', $index);
    if ($field->uri != 'coreid') {
      $field_element->addAttribute('term', $field->uri);
    }
    $index++;
  }
}

/**
 * Add default attributes for meta.xml
 */
function _dwcarchiver_add_default_attributes(SimpleXMLElement $element)
{
  // encoding="UTF-8" linesTerminatedBy="\r\n" fieldsTerminatedBy=","
  // fieldsEnclosedBy="&quot;" ignoreHeaderLines="0"
  // rowType="http://rs.tdwg.org/dwc/terms/Taxon"
  $element->addAttribute('encoding', 'UTF-8');
  $element->addAttribute('linesTerminatedBy', '\n');
  $element->addAttribute('fieldsTerminatedBy', ',');
  $element->addAttribute('fieldsEnclosedBy', '"');
  $element->addAttribute('ignoreHeaderLines', 0);
}
