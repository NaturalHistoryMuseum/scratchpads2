<?php

/**
 * Further work is required on this export of SDD.  For now, I will wait until
 * hearing from Sacha regarding the work that Trellon may have carried out.
 * 
 * Active tabs:
 * 
 * http://wiki.tdwg.org/twiki/bin/view/SDD/Primer/SddCharacters
 * http://monkey.nhm.ac.uk/character-editor-sdd/1
 * file:///home/simor/Desktop/Erythroneura.sdd11.xml
 * http://monkey.nhm.ac.uk/character-editor-project/1
 * 
 * Ensure the key.sql and Erythroneura.sdd11.xml are deleted before pushing.
 */

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
      case 'numeric':
      case 'text':
        $data_array[$row->taxonomy_term_data_relation_tid][$row->character_editor_character_relation_id];
        break;
      default:
        drupal_set_message($row->character_editor_character_relation_type);
        break;
    }
    $data_array[$row->taxonomy_term_data_relation_tid][$row->character_editor_character_relation_id] = $row->field_field_character_state_data[0]['rendered']['#markup'];
  }
  if(TRUE){
    drupal_add_http_header('Content-Type', 'text/xml; utf-8');
    echo _character_editor_create_sdd($taxonomy_array, $characters_array, $data_array);
    exit();
  }else if(function_exists('dpm')){
    dpm($vars['view']->result);
    dpm($taxonomy_array);
    dpm($characters_array);
    dpm($data_array);
    return;
  }else{
    return 'Devel not installed';
  }
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
  // Add the taxa.
  foreach($taxa as $key => $name){
    $taxonname = $taxonnames->appendChild($dom->createElement('TaxonName'));
    $taxonname->setAttribute('id', "taxa-$key");
    $representation = $taxonname->appendChild($dom->createElement('Representation'));
    $representation->appendChild($dom->createElement('Label', $name));
  }
  // Create a Characters element
  $characters = $dataset->appendChild($dom->createElement('Characters'));
  // Add the characters.
  foreach($chars as $key => $char){
    switch($char['type']){
      case 'numeric':
        $character = $characters->appendChild($dom->createElement('QuantitativeCharacter'));
        break;
      case 'text':
        // FIXME - We don't currently export text characters.  Does it make
        // much sense?  We perhaps need two types of export.
        break;
      case 'controlled':
        $character = $characters->appendChild($dom->createElement('CategoricalCharacter'));
        break;
    }
    $character->setAttribute('id', "char-$key");
    $representation = $character->appendChild($dom->createElement('Representation'));
    $representation->appendChild($dom->createElement('Label', $char['label']));
    // Add the states if controlled.
    if($char['type'] == 'controlled'){
      // Create the states.
      $states = $character->appendChild($dom->createElement('States'));
      foreach($char['states'] as $state_key => $state_text){
        $state = $states->appendChild($dom->createElement('StateDefinition'));
        $state->setAttribute('id', "state-$state_key");
        $representation = $state->appendChild($dom->createElement('Representation'));
        $representation->appendChild($dom->createElement('Label', $state_text));
      }
    }
  }
  // Add the data.
  // Create a CodedDescriptions element
  $codeddescs = $dataset->appendChild($dom->createElement('CodedDescriptions'));
  foreach($data as $taxonkey => $char_vals){
    $codeddesc = $codeddescs->appendChild($dom->createElement('CodedDescription'));
    $codeddesc->setAttribute('id', "codeddesc-$key");
    // Not adding a Representation value here - it will be identical to the taxa
    // representation.
    $scope = $codeddesc->appendChild($dom->createElement('Scope'));
    $taxonname = $scope->appendChild($dom->createElement('TaxonName'));
    $taxonname->setAttribute('id', "taxa-$taxonkey");
    // Add a SummaryData element.
    $summarydata = $codeddesc->appendChild($dom->createElement('SummaryData'));
    foreach($char_vals as $charkey => $statekey){
      switch($chars[$charkey]['type']){
        case 'numeric':
          $char = $summarydata->appendChild($dom->createElement('Quantitative'));
          $char->setAttribute('ref', "char-$charkey");
          $measure = $char->appendChild($dom->createElement('Measure'));
          $measure->setAttribute('value', $statekey);
          break;
        case 'text':
          $char = $summarydata->appendChild($dom->createElement('TextChar'));
          break;
        case 'controlled':
          $char = $summarydata->appendChild($dom->createElement('Categorical'));
          $char->setAttribute('ref', "char-$charkey");
          $state = $char->appendChild($dom->createElement('State'));
          $state->setAttribute('ref', "state-$statekey");
          break;
      }
    }
  }
  return $dom->saveXML();
}