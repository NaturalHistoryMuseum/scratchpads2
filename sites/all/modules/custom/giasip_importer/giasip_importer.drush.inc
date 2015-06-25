<?php

function giasip_importer_drush_command(){
  return array(
    'giasip' => array(
      'callback' => 'giasip_importer_do_import',
      'description' => 'Import data from the two DwC-A files'
    ),
    'giasip-delete' => array(
      'callback' => 'giasip_importer_do_delete',
      'description' => 'Delete data from a DwC-A.  Useful for trying to clean up a mess!'
    )
  );
}

function giasip_importer_do_delete($filename){
  // Load the DwCAIterator.class.php file
  require_once ('sites/all/libraries/DwC-A-Iterator/DwCAIterator.class.php');
  if(strpos($filename, '/') === FALSE){
    $filename = $_SERVER['PWD'] . '/' . $filename;
  }
  if(!file_exists($filename)){
    drush_log('ZIP file does not exist', 'error');
    return;
  }
  $core = new DwCACore($filename);
  foreach($core as $row){
    if(!empty($row['http://rs.tdwg.org/dwc/terms/scientificName'])){
      _giasip_importer_delete_term($row);
    }
    /*
    // Loop through the various extensions.
    if(isset($args['http://rs.gbif.org/terms/1.0/Distribution']) && !empty($row['http://rs.gbif.org/terms/1.0/Distribution'])){
      foreach($row['http://rs.gbif.org/terms/1.0/Distribution'] as $distribution_row){
        _giasip_importer_create_or_update_distribution($distribution_row, $tid, $resource_nid, $row['http://purl.org/dc/terms/references']);
      }
    }
    if(isset($args['http://rs.gbif.org/terms/1.0/Description']) && !empty($row['http://rs.gbif.org/terms/1.0/Description'])){
      foreach($row['http://rs.gbif.org/terms/1.0/Description'] as $description_row){
        _giasip_importer_create_or_update_description($description_row, $tid, $resource_nid, $row['http://purl.org/dc/terms/references']);
        // FIXME - Once we have pathway information, we shouldn't break here as
        // this will prevent importing multiple records for the same
        // taxon/species
        break;
      }
    }
    */
  }
}

function _giasip_importer_delete_term($row){
  // Deletes a term.  Note, this only deletes the term if it's parent TID is 0.
  $tid = db_select('taxonomy_term_data', 't')->fields('t', array(
    'tid'
  ))->condition('vid', 12)->condition('name', $row['http://rs.tdwg.org/dwc/terms/scientificName'])->execute()->fetchField();
  if($tid){
    $parents = taxonomy_get_parents($tid);
    if(!$parents){
      taxonomy_term_delete($tid);
      drush_log('Deleted: ' . $tid, 'ok');
    }
  }
}

function giasip_importer_do_import($filename, $create_terms = TRUE){
  ini_set('memory_limit', '1500M');
  if(is_string($create_terms) && strtolower(trim($create_terms)) == 'false'){
    $create_terms = FALSE;
  }
  $args = func_get_args();
  // Shift the filename and create_terms variables off the beginning of the array
  @array_shift($args);
  @array_shift($args);
  if(!count($args)){
    $args = array(
      'http://rs.gbif.org/terms/1.0/Distribution',
      'http://rs.gbif.org/terms/1.0/Description'
    );
  }
  $args = array_combine($args, $args);
  // Load the DwCAIterator.class.php file
  require_once ('sites/all/libraries/DwC-A-Iterator/DwCAIterator.class.php');
  if(strpos($filename, '/') === FALSE){
    $filename = $_SERVER['PWD'] . '/' . $filename;
  }
  if(!file_exists($filename)){
    drush_log('ZIP file does not exist', 'error');
    return;
  }
  $core = new DwCACore($filename);
  $eml = $core->get_eml();
  if(!$eml){
    drush_log('Archive does not contain an EML file', 'error');
    return;
  }
  $resource_nid = _giasip_importer_create_or_update_resource($eml);
  if(!$resource_nid){
    drush_log('Unable to create a resource node', 'error');
    return;
  }
  // Next, we loop through each record, creating a term for each one, and also
    // creating associated extension nodes for each extension row it links to.
  foreach($core as $row){
    if(!empty($row['http://rs.tdwg.org/dwc/terms/scientificName'])){
      $tid = _giasip_importer_create_or_update_term($row, $resource_nid, $create_terms);
      if($tid){
        // Loop through the various extensions.
        if(isset($args['http://rs.gbif.org/terms/1.0/Distribution']) && !empty($row['http://rs.gbif.org/terms/1.0/Distribution'])){
          foreach($row['http://rs.gbif.org/terms/1.0/Distribution'] as $distribution_row){
            _giasip_importer_create_or_update_distribution($distribution_row, $tid, $resource_nid, $row['http://purl.org/dc/terms/references']);
          }
        }
        if(isset($args['http://rs.gbif.org/terms/1.0/Description']) && !empty($row['http://rs.gbif.org/terms/1.0/Description'])){
          foreach($row['http://rs.gbif.org/terms/1.0/Description'] as $description_row){
            _giasip_importer_create_or_update_description($description_row, $tid, $resource_nid, $row['http://purl.org/dc/terms/references']);
            // FIXME - Once we have pathway information, we shouldn't break here as
    // this will prevent importing multiple records for the same
    // taxon/species
            break;
          }
        }
      }
    }
  }
}

/**
 * Helper function to create the description node
 */
function _giasip_importer_create_or_update_description($row, $tid, $resource_nid, $record_url){
  // FIXME - No data is currently being sent about the pathway.  So this needs
    // to be fixed.
  switch($row['http://purl.org/dc/terms/type']){
    case 'dispersal':
      // Try and load an existing node.
      $query = db_select('node', 'n')->fields('n', array(
        'nid'
      ));
      $query->innerJoin('field_data_field_taxon', 'ft', 'n.nid = ft.entity_id');
      $query->innerJoin('field_data_field_resource', 'fr', 'n.nid = fr.entity_id');
      $query->condition('ft.field_taxon_tid', $tid);
      $query->condition('fr.field_resource_nid', $resource_nid);
      $query->condition('n.type', 'taxon_pathway');
      $nid = $query->execute()->fetchField();
      if($nid){
        $node = node_load($nid, NULL, TRUE);
      }else{
        $node = entity_create('node', array(
          'uid' => 1,
          'type' => 'taxon_pathway'
        ));
      }
      $node->field_record_url = array(
        LANGUAGE_NONE => array(
          array(
            'title' => '',
            'url' => (isset($row['http://purl.org/dc/terms/source']) ? $row['http://purl.org/dc/terms/source'] : $record_url),
            'attributes' => array()
          )
        )
      );
      $node->field_taxon = array(
        LANGUAGE_NONE => array(
          array(
            'tid' => $tid
          )
        )
      );
      $node->field_resource = array(
        LANGUAGE_NONE => array(
          array(
            'nid' => $resource_nid
          )
        )
      );
      // Don't create a pretty URL for this node (it's never accessed directly)
      $node->path['pathauto'] = FALSE;
      node_save($node);
      break;
  }
}

/**
 * Helper function to create the distribution node
 */
function _giasip_importer_create_or_update_distribution($row, $tid, $resource_nid, $record_url){
  $country = countries_country_lookup($row['http://rs.tdwg.org/dwc/terms/countryCode']);
  // We only go any further if we actually have a country, else we provide an
    // error.
  $status = '';
  switch($row['http://rs.tdwg.org/dwc/terms/establishmentMeans']){
    case 'native':
      $status = 'Native';
      break;
    case 'introduced':
      $status = 'Alien - introduced';
      break;
    case 'uncertain':
      $status = 'Cryptogenic';
      break;
  }
  $invasive = '';
  switch($row['http://rs.tdwg.org/dwc/terms/occurrenceRemarks']){
    case 'Invasive':
      $invasive = 'Yes';
      break;
    case 'Not invasive':
      $invasive = 'No';
      break;
  }
  if($country){
    // Try and load an existing node.
    $query = db_select('node', 'n')->fields('n', array(
      'nid'
    ));
    $query->innerJoin('field_data_field_taxon_country', 'ftc', 'n.nid = ftc.entity_id');
    $query->innerJoin('field_data_field_taxon', 'ft', 'n.nid = ft.entity_id');
    $query->innerJoin('field_data_field_resource', 'fr', 'n.nid = fr.entity_id');
    $query->condition('ftc.field_taxon_country_iso2', $country->iso2);
    $query->condition('ft.field_taxon_tid', $tid);
    $query->condition('fr.field_resource_nid', $resource_nid);
    $query->condition('n.type', 'taxon_country');
    $nid = $query->execute()->fetchField();
    if($nid){
      $node = node_load($nid, NULL, TRUE);
    }else{
      $node = entity_create('node', array(
        'uid' => 1,
        'type' => 'taxon_country'
      ));
    }
    $node->field_record_url = array(
      LANGUAGE_NONE => array(
        array(
          'title' => '',
          'url' => (isset($row['http://purl.org/dc/terms/source']) ? $row['http://purl.org/dc/terms/source'] : $record_url),
          'attributes' => array()
        )
      )
    );
    $node->field_taxon_country = array(
      LANGUAGE_NONE => array(
        array(
          'iso2' => $country->iso2
        )
      )
    );
    $node->field_taxon = array(
      LANGUAGE_NONE => array(
        array(
          'tid' => $tid
        )
      )
    );
    $node->field_status = array(
      LANGUAGE_NONE => array(
        array(
          'value' => $status
        )
      )
    );
    $node->field_invasive = array(
      LANGUAGE_NONE => array(
        array(
          'value' => $invasive
        )
      )
    );
    $node->field_resource = array(
      LANGUAGE_NONE => array(
        array(
          'nid' => $resource_nid
        )
      )
    );
    // Don't create a pretty URL for this node (it's never accessed directly)
    $node->path['pathauto'] = FALSE;
    node_save($node);
  }
}

/**
 * Helper function to return an ISO 639-1 code for a language name
 */
function _giasip_6391_magic($language){
  $languageCodes = array(
    "aa" => "Afar",
    "ab" => "Abkhazian",
    "ae" => "Avestan",
    "af" => "Afrikaans",
    "ak" => "Akan",
    "am" => "Amharic",
    "an" => "Aragonese",
    "ar" => "Arabic",
    "as" => "Assamese",
    "av" => "Avaric",
    "ay" => "Aymara",
    "az" => "Azerbaijani",
    "ba" => "Bashkir",
    "be" => "Belarusian",
    "bg" => "Bulgarian",
    "bh" => "Bihari",
    "bi" => "Bislama",
    "bm" => "Bambara",
    "bn" => "Bengali",
    "bo" => "Tibetan",
    "br" => "Breton",
    "bs" => "Bosnian",
    "ca" => "Catalan",
    "ce" => "Chechen",
    "ch" => "Chamorro",
    "co" => "Corsican",
    "cr" => "Cree",
    "cs" => "Czech",
    "cu" => "Church Slavic",
    "cv" => "Chuvash",
    "cy" => "Welsh",
    "da" => "Danish",
    "de" => "German",
    "dv" => "Divehi",
    "dz" => "Dzongkha",
    "ee" => "Ewe",
    "el" => "Greek",
    "en" => "English",
    "eo" => "Esperanto",
    "es" => "Spanish",
    "et" => "Estonian",
    "eu" => "Basque",
    "fa" => "Persian",
    "ff" => "Fulah",
    "fi" => "Finnish",
    "fj" => "Fijian",
    "fo" => "Faroese",
    "fr" => "French",
    "fy" => "Western Frisian",
    "ga" => "Irish",
    "gd" => "Scottish Gaelic",
    "gl" => "Galician",
    "gn" => "Guarani",
    "gu" => "Gujarati",
    "gv" => "Manx",
    "ha" => "Hausa",
    "he" => "Hebrew",
    "hi" => "Hindi",
    "ho" => "Hiri Motu",
    "hr" => "Croatian",
    "ht" => "Haitian",
    "hu" => "Hungarian",
    "hy" => "Armenian",
    "hz" => "Herero",
    "ia" => "Interlingua (International Auxiliary Language Association)",
    "id" => "Indonesian",
    "ie" => "Interlingue",
    "ig" => "Igbo",
    "ii" => "Sichuan Yi",
    "ik" => "Inupiaq",
    "io" => "Ido",
    "is" => "Icelandic",
    "it" => "Italian",
    "iu" => "Inuktitut",
    "ja" => "Japanese",
    "jv" => "Javanese",
    "ka" => "Georgian",
    "kg" => "Kongo",
    "ki" => "Kikuyu",
    "kj" => "Kwanyama",
    "kk" => "Kazakh",
    "kl" => "Kalaallisut",
    "km" => "Khmer",
    "kn" => "Kannada",
    "ko" => "Korean",
    "kr" => "Kanuri",
    "ks" => "Kashmiri",
    "ku" => "Kurdish",
    "kv" => "Komi",
    "kw" => "Cornish",
    "ky" => "Kirghiz",
    "la" => "Latin",
    "lb" => "Luxembourgish",
    "lg" => "Ganda",
    "li" => "Limburgish",
    "ln" => "Lingala",
    "lo" => "Lao",
    "lt" => "Lithuanian",
    "lu" => "Luba-Katanga",
    "lv" => "Latvian",
    "mg" => "Malagasy",
    "mh" => "Marshallese",
    "mi" => "Maori",
    "mk" => "Macedonian",
    "ml" => "Malayalam",
    "mn" => "Mongolian",
    "mr" => "Marathi",
    "ms" => "Malay",
    "mt" => "Maltese",
    "my" => "Burmese",
    "na" => "Nauru",
    "nb" => "Norwegian Bokmal",
    "nd" => "North Ndebele",
    "ne" => "Nepali",
    "ng" => "Ndonga",
    "nl" => "Dutch",
    "nn" => "Norwegian Nynorsk",
    "no" => "Norwegian",
    "nr" => "South Ndebele",
    "nv" => "Navajo",
    "ny" => "Chichewa",
    "oc" => "Occitan",
    "oj" => "Ojibwa",
    "om" => "Oromo",
    "or" => "Oriya",
    "os" => "Ossetian",
    "pa" => "Panjabi",
    "pi" => "Pali",
    "pl" => "Polish",
    "ps" => "Pashto",
    "pt" => "Portuguese",
    "qu" => "Quechua",
    "rm" => "Raeto-Romance",
    "rn" => "Kirundi",
    "ro" => "Romanian",
    "ru" => "Russian",
    "rw" => "Kinyarwanda",
    "sa" => "Sanskrit",
    "sc" => "Sardinian",
    "sd" => "Sindhi",
    "se" => "Northern Sami",
    "sg" => "Sango",
    "si" => "Sinhala",
    "sk" => "Slovak",
    "sl" => "Slovenian",
    "sm" => "Samoan",
    "sn" => "Shona",
    "so" => "Somali",
    "sq" => "Albanian",
    "sr" => "Serbian",
    "ss" => "Swati",
    "st" => "Southern Sotho",
    "su" => "Sundanese",
    "sv" => "Swedish",
    "sw" => "Swahili",
    "ta" => "Tamil",
    "te" => "Telugu",
    "tg" => "Tajik",
    "th" => "Thai",
    "ti" => "Tigrinya",
    "tk" => "Turkmen",
    "tl" => "Tagalog",
    "tn" => "Tswana",
    "to" => "Tonga",
    "tr" => "Turkish",
    "ts" => "Tsonga",
    "tt" => "Tatar",
    "tw" => "Twi",
    "ty" => "Tahitian",
    "ug" => "Uighur",
    "uk" => "Ukrainian",
    "ur" => "Urdu",
    "uz" => "Uzbek",
    "ve" => "Venda",
    "vi" => "Vietnamese",
    "vo" => "Volapuk",
    "wa" => "Walloon",
    "wo" => "Wolof",
    "xh" => "Xhosa",
    "yi" => "Yiddish",
    "yo" => "Yoruba",
    "za" => "Zhuang",
    "zh" => "Chinese",
    "zu" => "Zulu"
  );
  return array_search($language, $languageCodes);
}

/**
 * Helper function to create/update a term based on a core row from the DwC-A
 */
function _giasip_importer_create_or_update_term($row, $resource_nid, $create_terms){
  static $ids = array();
  // Ensure we don't have any double spaces
  $row['http://rs.tdwg.org/dwc/terms/scientificName'] = preg_replace('/\s\s+/', ' ', $row['http://rs.tdwg.org/dwc/terms/scientificName']);
  if(isset($row['http://rs.tdwg.org/dwc/terms/scientificNameAuthorship'])){
    $row['http://rs.tdwg.org/dwc/terms/scientificName'] = trim(str_replace($row['http://rs.tdwg.org/dwc/terms/scientificNameAuthorship'], '', $row['http://rs.tdwg.org/dwc/terms/scientificName']));
  }
  $tid = db_select('taxonomy_term_data', 't')->fields('t', array(
    'tid'
  ))->condition('vid', 12)->condition('name', $row['http://rs.tdwg.org/dwc/terms/scientificName'], 'LIKE')->execute()->fetchField();
  if($tid){
    $term = taxonomy_term_load($tid);
  }else{
    if(!$create_terms){return FALSE;}
    $term = entity_create('taxonomy_term', array(
      'vid' => 12,
      'name' => $row['http://rs.tdwg.org/dwc/terms/scientificName']
    ));
  }
  if($create_terms){
    if(!empty($row['http://rs.tdwg.org/dwc/terms/scientificNameAuthorship'])){
      $term->field_authors = array(
        LANGUAGE_NONE => array(
          array(
            'value' => $row['http://rs.tdwg.org/dwc/terms/scientificNameAuthorship'],
            'format' => 'plain_text',
            'safe_value' => ''
          )
        )
      );
    }
    if(!empty($row['http://rs.tdwg.org/dwc/terms/taxonomicStatus'])){
      $term->field_usage = array(
        LANGUAGE_NONE => array(
          array(
            'value' => $row['http://rs.tdwg.org/dwc/terms/scientificNameAuthorship']
          )
        )
      );
    }
    // We don't update the alias for the term, as it will result in 
    // taxonomy_get_tree() being called on a large tree.
    if(!isset($term->path)){
      $term->path = array();
    }
    $term->path['pathauto'] = FALSE;
    taxonomy_term_save($term);
  }
  // We then add the resource (if it hasn't already been added)
    // We check for the presence of the $resource_nid, and add it if it hasn't
    // already been added.
  $resource_not_found = TRUE;
  if(!empty($term->field_resource_collection[LANGUAGE_NONE])){
    foreach($term->field_resource_collection[LANGUAGE_NONE] as $term_resource_collection){
      $collection_item = entity_load('field_collection_item', array(
        $term_resource_collection['value']
      ));
      if(@isset($collection_item[$term_resource_collection['value']]->field_resource[LANGUAGE_NONE][0]['nid']) && $collection_item[$term_resource_collection['value']]->field_resource[LANGUAGE_NONE][0]['nid'] == $resource_nid){
        if(isset($row['http://purl.org/dc/terms/references']) && @isset($collection_item[$term_resource_collection['value']]->field_taxon_url[LANGUAGE_NONE][0])){
          if($row['http://purl.org/dc/terms/references'] == $collection_item){
            $resource_not_found = FALSE;
          }
        }else{
          $resource_not_found = FALSE;
        }
        $resource_not_found = FALSE;
      }
      if(!$resource_not_found){
        break;
      }
    }
  }
  if(!empty($row['http://purl.org/dc/terms/references'])){
    if(empty($term->field_resource_collection[LANGUAGE_NONE]) || $resource_not_found){
      $field_collection_item = entity_create('field_collection_item', array(
        'field_name' => 'field_resource_collection'
      ));
      $field_collection_item->setHostEntity('taxonomy_term', $term, LANGUAGE_NONE, FALSE);
      $field_collection_item->field_resource = array(
        LANGUAGE_NONE => array(
          array(
            'nid' => $resource_nid
          )
        )
      );
      $field_collection_item->field_taxon_url = array(
        LANGUAGE_NONE => array(
          array(
            'url' => $row['http://purl.org/dc/terms/references'],
            'title' => '',
            'attributes' => array()
          )
        )
      );
      $field_collection_item->save();
    }
  }
  if(isset($row['http://rs.gbif.org/terms/1.0/VernacularName'])){
    foreach($row['http://rs.gbif.org/terms/1.0/VernacularName'] as $vernacular_row){
      $vernacular_not_found = TRUE;
      if(isset($term->field_vernacular_name_collection[LANGUAGE_NONE])){
        foreach($term->field_vernacular_name_collection[LANGUAGE_NONE] as $term_vernacular_collection){
          $collection_item = entity_load('field_collection_item', array(
            $term_vernacular_collection['value']
          ));
          if($collection_item[$term_vernacular_collection['value']]->field_vernacular_name[LANGUAGE_NONE][0]['value'] == $vernacular_row['http://rs.tdwg.org/dwc/terms/vernacularName']){
            if(!empty($vernacular_row['http://rs.tdwg.org/dwc/terms/countryCode'])){
              if($collection_item[$term_vernacular_collection['value']]->field_vernacular_locality[LANGUAGE_NONE][0]['value'] == $vernacular_row['http://rs.tdwg.org/dwc/terms/vernacularName']){
                $vernacular_not_found = FALSE;
              }
            }else{
              $vernacular_not_found = FALSE;
            }
            if(!$vernacular_not_found){
              break;
            }
          }
        }
      }
      if($vernacular_not_found){
        $field_collection_item = entity_create('field_collection_item', array(
          'field_name' => 'field_vernacular_name_collection'
        ));
        $field_collection_item->setHostEntity('taxonomy_term', $term, LANGUAGE_NONE, FALSE);
        $field_collection_item->field_vernacular_name = array(
          LANGUAGE_NONE => array(
            array(
              'value' => $vernacular_row['http://rs.tdwg.org/dwc/terms/vernacularName']
            )
          )
        );
        if(($lang_code = _giasip_6391_magic($vernacular_row['http://purl.org/dc/terms/language'])) != FALSE){
          $field_collection_item->field_language = array(
            LANGUAGE_NONE => array(
              array(
                'value' => $lang_code
              )
            )
          );
        }
        if(!empty($vernacular_row['http://rs.tdwg.org/dwc/terms/countryCode'])){
          $field_collection_item->field_vernacular_locality = array(
            LANGUAGE_NONE => array(
              array(
                'value' => $vernacular_row['http://rs.tdwg.org/dwc/terms/countryCode'],
                'format' => 'plain_text'
              )
            )
          );
        }
        $field_collection_item->save();
      }
    }
  }
  $ids[$row['id']] = $term->tid;
  if($create_terms){
    // We try and set the parent.
    if(isset($row['http://rs.tdwg.org/dwc/terms/acceptedNameUsageID']) && isset($ids[$row['http://rs.tdwg.org/dwc/terms/acceptedNameUsageID']])){
      db_delete('taxonomy_term_hierarchy')->condition('tid', $term->tid)->execute();
      db_insert('taxonomy_term_hierarchy')->fields(array(
        'tid' => $term->tid,
        'parent' => $ids[$row['http://rs.tdwg.org/dwc/terms/acceptedNameUsageID']]
      ))->execute();
    }else{
      $parent_tid = FALSE;
      if(!empty($row['http://rs.tdwg.org/dwc/terms/genus'])){
        $parent_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
          'tid'
        ))->condition('name', $row['http://rs.tdwg.org/dwc/terms/genus'])->execute()->fetchField();
        if(!$parent_tid && !empty($row['http://rs.tdwg.org/dwc/terms/family'])){
          // We couldn't find the genus name, so we try and find the family
          $family_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
            'tid'
          ))->condition('name', $row['http://rs.tdwg.org/dwc/terms/family'])->execute()->fetchField();
          if(!$family_tid){
            $order_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
              'tid'
            ))->condition('name', $row['http://rs.tdwg.org/dwc/terms/order'])->execute()->fetchField();
            if(!$order_tid){
              $class_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
                'tid'
              ))->condition('name', $row['http://rs.tdwg.org/dwc/terms/class'])->execute()->fetchField();
              if(!$class_tid){
                $phylum_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
                  'tid'
                ))->condition('name', $row['http://rs.tdwg.org/dwc/terms/phylum'])->execute()->fetchField();
                if(!$phylum_tid){
                  $kingdom_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
                    'tid'
                  ))->condition('name', $row['http://rs.tdwg.org/dwc/terms/kingdom'])->execute()->fetchField();
                  if($kingdom_tid){
                    $phylum = entity_create('taxonomy_term', array(
                      'vid' => 12,
                      'name' => $row['http://rs.tdwg.org/dwc/terms/phylum'],
                      'parent' => array(
                        $kingdom_tid
                      )
                    ));
                    taxonomy_term_save($phylum);
                    $phylum_tid = $phylum->tid;
                  }else{
                    drush_log('Unable to find kingdom: ' . $row['http://rs.tdwg.org/dwc/terms/kingdom'], 'error');
                  }
                }
                if($phylum_tid){
                  $class = entity_create('taxonomy_term', array(
                    'vid' => 12,
                    'name' => $row['http://rs.tdwg.org/dwc/terms/class'],
                    'parent' => array(
                      $phylum_tid
                    )
                  ));
                  taxonomy_term_save($class);
                  $class_tid = $class->tid;
                }
              }
              if($class_tid){
                $order = entity_create('taxonomy_term', array(
                  'vid' => 12,
                  'name' => $row['http://rs.tdwg.org/dwc/terms/order'],
                  'parent' => array(
                    $class_tid
                  )
                ));
                taxonomy_term_save($order);
                $order_tid = $order->tid;
              }
            }
            if($order_tid){
              $family = entity_create('taxonomy_term', array(
                'vid' => 12,
                'name' => $row['http://rs.tdwg.org/dwc/terms/family'],
                'parent' => array(
                  $order_tid
                )
              ));
              taxonomy_term_save($family);
              $family_tid = $family->tid;
            }
          }
          if($family_tid){
            $genus = entity_create('taxonomy_term', array(
              'vid' => 12,
              'name' => $row['http://rs.tdwg.org/dwc/terms/genus'],
              'parent' => array(
                $family_tid
              )
            ));
            taxonomy_term_save($genus);
            $parent_tid = $genus->tid;
          }
        }
      }else{
        // We try to get a parent TID based on the Genus name.
        $name_parts = explode(' ', $term->name);
        array_pop($name_parts);
        for($i = 1; $i <= count($name_parts); $i++){
          $poss_genus = array();
          for($j = 0; $j < $i; $j++){
            $poss_genus[] = $name_parts[$j];
          }
          $poss_genus = implode(' ', $poss_genus);
          $parent_tid = db_select('taxonomy_term_data', 't')->fields('t', array(
            'tid'
          ))->condition('name', $poss_genus)->execute()->fetchField();
          if($parent_tid){
            break;
          }
        }
      }
      if($parent_tid){
        db_delete('taxonomy_term_hierarchy')->condition('tid', $term->tid)->execute();
        db_insert('taxonomy_term_hierarchy')->fields(array(
          'tid' => $term->tid,
          'parent' => $parent_tid
        ))->execute();
      }
    }
  }
  drush_log('Term: ' . $term->name, 'ok');
  return $term->tid;
}

/**
 * Helper function to create/update a resource node based on eml.
 */
function _giasip_importer_create_or_update_resource(SimpleXMLElement $eml){
  // This code assumes that the title of the dataset is persistent.
  $nid = db_select('node', 'n')->fields('n', array(
    'nid'
  ))->condition('type', 'resource')->condition('title', (string)$eml->dataset->title)->execute()->fetchField();
  if($nid){
    $node = node_load($nid, NULL, TRUE);
  }else{
    // Create or get the NID of a resource which we can use for this DwC-A.
    $node = entity_create('node', array(
      'uid' => 1,
      'title' => (string)$eml->dataset->title,
      'status' => 1,
      'comment' => 0,
      'sticky' => 0,
      'type' => 'resource',
      'body' => array(
        LANGUAGE_NONE => array(
          array(
            'value' => (string)$eml->dataset->abstract->para,
            'format' => 'filtered_html'
          )
        )
      )
    ));
  }
  node_save($node);
  return $node->nid;
}
