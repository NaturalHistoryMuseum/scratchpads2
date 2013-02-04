<?php

function theme_character_editor_create_sdd($vars){
  $taxonomy_array = array();
  $characters_array = array();
  $data_array = array();
  foreach($vars['view']->result as $row){
    $taxonomy_array[$row->taxonomy_term_data_relation_tid] = $row->taxonomy_term_data_relation_name;
    $characters_array[$row->character_editor_character_relation_id] = array(
      'type' => $row->character_editor_character_relation_type,
      'label' => $row->character_editor_character_relation_title,
      'description' => $row->field_field_char_description[0]['rendered']['#markup']
    );
    switch($row->character_editor_character_relation_type){
      case 'controlled':
        $characters_array[$row->character_editor_character_relation_id]['states'] = array();
        foreach($row->field_field_char_states as $state){
          foreach($state['rendered']['entity']['field_collection_item'] as $key => $label){
            $characters_array[$row->character_editor_character_relation_id]['states'][$key] = $label['field_char_state_label']['#items'][0]['value'];
          }
        }
        break;
      case 'text':
        $data_array[$row->taxonomy_term_data_relation_tid][$row->character_editor_character_relation_id];
        break;
      default:
        drupal_set_message($row->character_editor_character_relation_type);
        break;
    }
    $data_array[$row->taxonomy_term_data_relation_tid][$row->character_editor_character_relation_id] = $row->field_field_character_state_data[0]['rendered']['#markup'];
  }
  if(FALSE){
    drupal_add_http_header('Content-Type', 'text/xml; utf-8');
    echo _character_editor_create_sdd($taxonomy_array, $characters_array, $data_array);
    exit();
  }
  dpm($vars['view']->result);
  dpm($taxonomy_array);
  dpm($characters_array);
  dpm($data_array);
  return '';
}

/**
 * Create SDD file from data.
 */
function _character_editor_create_sdd($taxa, $chars, $data){
  $dom = new DOMDocument();
  // Create the Datasets element.
  $datasets = $dom->createElement("Datasets");
  $datasets->setAttribute('xsi:schemaLocation', 'http://rs.tdwg.org/UBIF/2006/ http://rs.tdwg.org/UBIF/2006/Schema/1.1/SDD.xsd');
  $datasets->setAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
  $datasets->setAttribute('xmlns', 'http://rs.tdwg.org/UBIF/2006/');
  $datasets->setAttribute('xmlns:u', 'http://rs.tdwg.org/UBIF/2006/');
  $datasets = $dom->appendChild($datasets);
  // Create the TechnicalMetadata element.
  $technical_metadata = $datasets->appendChild($dom->createElement('TechnicalMetadata'));
  $technical_metadata->setAttribute('created', date('r'));
  $generator = $technical_metadata->appendChild($dom->createElement('Generator'));
  $generator->setAttribute('name', 'Scratchpads');
  $generator->setAttribute('version', '2.0');
  // Create the dataset element
  $dataset = $datasets->appendChild($dom->createElement('Dataset'));
  $dataset->setAttribute('xml:lang', 'en'); // Is this always going to be the case?
  // Create the representation and label elements
  $representation = $dataset->appendChild($dom->createElement('Representation'));
  $label = t('An interactive key to the taxa: @taxa', array(
    '@taxa' => implode('; ', $taxa)
  ));
  $label = $representation->appendChild($dom->createElement('Label', $label));
  $label->setAttribute('xml:lang', 'en'); // Is this always going to be the case?
  // Create a taxonnames element
  $taxonnames = $dataset->appendChild($dom->createElement('TaxonNames'));
  // Add the taxa/
  foreach($taxa as $key => $name){
    $taxonname = $taxonnames->appendChild($dom->createElement('TaxonName'));
    $taxonname->setAttribute('id', $key);
    $representation = $taxonname->appendChild($dom->createElement('Representation'));
    $representation->appendChild($dom->createElement('Label', $name));
  }
  return $dom->saveXML();
}