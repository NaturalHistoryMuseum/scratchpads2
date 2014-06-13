<?php

function _character_editor_export_sdd($entity){
  $entity_w = entity_metadata_wrapper('character_editor_project', $entity);
  $xml = new SimpleXMLElement('<Datasets></Datasets>');
  $dataset = $xml->addChild('Dataset');
  // List of IDs to load
  $ids = array();
  // Add the taxonomy terms to the dataset element.
  $taxon_names = $dataset->addChild('TaxonNames');
  foreach($entity_w->field_taxonomic_name->value() as $term){
    $ids[] = 'taxonomy_term:' . $term->tid;
    $taxon_name = $taxon_names->addChild('TaxonName');
    /*$path = entity_uri('taxonomy_term', $term);
    $taxon_name->addAttribute('id', url($path['path'], array(
      'absolute' => TRUE
    )));*/
    $taxon_name->addAttribute('id', 'taxon' . $term->tid);
    $representation = $taxon_name->addChild('Representation');
    $representation->addChild('Label', $term->name);
  }
  // Add the specimens to the dataset element.
  $specimens = $dataset->addChild('Specimens');
  foreach($entity_w->field_char_proj_specimen->value() as $node){
    $ids[] = 'node:' . $node->nid;
    $specimen = $specimens->addChild('Specimen');
    /*$path = entity_uri('node', $node);
    $specimen->addAttribute('id', url($path['path'], array(
      'absolute' => TRUE
    )));*/
    $specimen->addAttribute('id', 'specimen' . $node->nid);
    $representation = $specimen->addChild('Representation');
    $representation->addChild('Label', $node->title);
  }
  // Add the characters and the mappings
  $characters = $dataset->addChild('Characters');
  $descriptive_concepts = $dataset->addChild('DescriptiveConcepts');
  $character_tree = $dataset->addChild('CharacterTrees');
  $character_tree = $character_tree->addChild('CharacterTree');
  $nodes = $character_tree->addChild('Nodes');
  $tree = character_editor_get_tree($entity_w);
  $character_ids_and_types = array();
  foreach($tree as $character){
    // TODO - Add media.
    switch($character['wrapper']->type->value()){
      case 'group':
        $character_xml = $descriptive_concepts->addChild('DescriptiveConcept');
        $character_xml->addAttribute('id', 'character' . $character['wrapper']->id->value());
        $representation = $character_xml->addChild('Representation');
        $representation->addChild('Label', $character['wrapper']->title->value());
        break;
      case 'numeric':
        $character_xml = $characters->addChild('QuantitativeCharacter');
        $character_xml->addAttribute('id', 'character' . $character['wrapper']->id->value());
        $representation = $character_xml->addChild('Representation');
        $representation->addChild('Label', $character['wrapper']->title->value());
        if($character['wrapper']->field_char_unit->value()){
          $measurement_unit = $character_xml->addChild('MeasurementUnit');
          $measurement_unit->addChild('Label', $character['wrapper']->field_char_unit->value());
        }
        break;
      case 'controlled':
        $character_xml = $characters->addChild('CategoricalCharacter');
        $character_xml->addAttribute('id', 'character' . $character['wrapper']->id->value());
        $representation = $character_xml->addChild('Representation');
        $representation->addChild('Label', $character['wrapper']->title->value());
        $states = $character_xml->addChild('States');
        foreach($character['wrapper']->field_char_states->value() as $state){
          $state_definition = $states->addChild('StateDefinition');
          $state_w = entity_metadata_wrapper('field_collection_item', $state);
          $state_definition->addAttribute('id', 'state' . $state_w->item_id->value());
          $representation = $state_definition->addChild('Representation');
          $representation->addChild('Label', $state_w->field_char_state_label->value());
        }
        break;
      case 'text':
        $character_xml = $characters->addChild('TextCharacter');
        $character_xml->addAttribute('id', 'character' . $character['wrapper']->id->value());
        $representation = $character_xml->addChild('Representation');
        $representation->addChild('Label', $character['wrapper']->title->value());
        break;
    }
    switch($character['wrapper']->type->value()){
      case 'group':
        $node_xml = $nodes->addChild('Node');
        $node_xml = $node_xml->addChild('DescriptiveConcept');
        $node_xml->addAttribute('ref', 'character' . $character['wrapper']->id->value());
        break;
      default:
        $character_ids_and_types[$character['wrapper']->id->value()] = $character['wrapper']->type->value();
        $node_xml = $nodes->addChild('CharNode');
        $node_xml = $node_xml->addChild('Character');
        $node_xml->addAttribute('ref', 'character' . $character['wrapper']->id->value());
    }
    if($character['parent_id']){
      $parent = $node_xml->addChild('Parent');
      $parent_id_parts = explode(':', $character['parent_id']);
      $parent->addAttribute('ref', 'character' . $parent_id_parts[1]);
    }
  }
  // Finally, add all the mappings.
  $project_model = new CharacterProjectModel('character_editor_project', $entity_w->id->value());
  // Get the character tree
  $tree = $project_model->getTree();
  $flags = $project_model->getFlags();
  // Get the row values
  $return = array();
  foreach($ids as $key => $id){
    $ids[$key] = new stdClass();
    $ids[$key]->id = $id;
  }
  $rows = $project_model->getRowValues($ids, 0, count($ids));
  $coded_descriptions = $dataset->addChild('CodedDescriptions');
  foreach($rows as $row){
    $coded_description = $coded_descriptions->addChild('CodedDescription');
    $scope = $coded_description->addChild('Scope');
    $id_parts = explode(':', $row->id);
    switch($id_parts[0]){
      case 'taxonomy_term':
        $taxon_name = $scope->addChild('TaxonName');
        $taxon_name->addAttribute('ref', 'taxon' . $id_parts[1]);
        break;
      case 'node':
        break;
    }
    $summary_data = FALSE;
    foreach($row as $id => $data){
      foreach($character_ids_and_types as $key => $type){
        if(substr($id, 0, strlen($key) + 11) == 'character_' . $key . '_'){
          switch($type){
            case 'numeric':
              if(($data = json_decode($data, TRUE)) != FALSE && isset($data['data']) && strlen($data['data'])){
                if(!$summary_data){
                  $summary_data = $coded_description->addChild('SummaryData');
                }
                $quantitative = $summary_data->addChild('Quantitative');
                $quantitative->addAttribute('ref', 'character' . $key);
                $measure = $quantitative->addChild('Measure');
                $measure->addAttribute('value', $data['data']);
                break;
              }
            case 'text':
              if(($data = json_decode($data, TRUE)) != FALSE && isset($data['data']) && strlen($data['data'])){
                if(!$summary_data){
                  $summary_data = $coded_description->addChild('SummaryData');
                }
                $text = $summary_data->addChild('Text');
                $text->addAttribute('ref', 'character' . $key);
                $content = $text->addChild('Content', $data['data']);
                break;
              }
              break;
            case 'controlled':
              if(($data = json_decode($data, TRUE)) != FALSE){
                if(isset($data['value'])){
                  if(!$summary_data){
                    $summary_data = $coded_description->addChild('SummaryData');
                  }
                  $categorical = $summary_data->addChild('Categorical');
                  $categorical->addAttribute('ref', 'character' . $key);
                  foreach(explode(',', $data['value']) as $data_value){
                    $state = $categorical->addChild('State');
                    $state->addAttribute('ref', 'state' . $data_value);
                  }
                }
              }
              break;
          }
        }
      }
    }
  }
  drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');
  echo $xml->saveXML();
  drupal_exit();
}