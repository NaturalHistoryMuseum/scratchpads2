<?php

/**
 * This file contains the schema/map definitions for the pensoft XML export, as
 * defined in the _pensoft_xml_build_from_schema function. It also contains
 * schema array process functions that are specific to Pensoft.
 * 
 */

/**
 * Process a salutation to return it as Pensoft expects it.
 */
function _pensoft_publication_xml_process_salutation($value){
  $map = array(
    'Prof.' => 'Prof'
  );
  if(isset($map[$value['value']])){
    return array(
      'value' => $map[$value['value']]
    );
  }else{
    return $value;
  }
}

/**
 * Loader function for the treatment/taxon description
 */
function _pensoft_publication_xml_load_treatment($value){
  $endpoint = array_shift(array_values($value['endpoints']));
  list($type, $nid) = explode(':', $endpoint);
  $rid = $value['relation_id'];
  $relation = relation_load($rid);
  $node = node_load($nid);
  $tid = $node->field_taxonomic_name[LANGUAGE_NONE][0]['tid'];
  $term = taxonomy_term_load($tid);
  $combine = array_merge((array)$node, (array)$relation, (array)$term);
  return ((object)$combine);
}

/**
 * The document_info section
 */
function _pensoft_publication_xml_document_info_schema(){
  $document_info_schema = array(
    'document_type' => array(
      '#attributes' => array(
        'id' => 1 // Taxonomy paper
      )
    ),
    'journal_name' => array(
      '#attributes' => array(
        'id' => 1
      )
    )
  );
  return $document_info_schema;
}

/**
 * The article_metadata section
 */
function _pensoft_publication_xml_article_metadata_schema(){
  $author_schema_template = array(
    '#load' => '_publication_xml_load_user',
    '#value' => '',
    'fields' => array(
      'e-mail' => array(
        'value' => array(
          '#field' => 'mail'
        )
      ),
      'salutation' => array(
        'value' => array(
          '#field' => 'field_user_title',
          '#process' => '_pensoft_publication_xml_process_salutation'
        )
      ),
      'first_name' => array(
        'value' => array(
          '#field' => 'field_user_given_names'
        )
      ),
      //'middle_name' => array(
      //  '#value' => ''
      //),
      'last_name' => array(
        'value' => array(
          '#field' => 'field_user_family_name'
        )
      ),
      'rights' => array(
        'value' => array(
          '#value' => htmlspecialchars('Edit & comment') //"Edit & comment" or "Comment only"
        )
      )
    )
    // corresponding_author, rights, submitting_author
    ,
    'address' => array(
      'fields' => array(
        // affiliation, city
        'country' => array(
          'value' => array(
            '#field' => 'field_user_country',
            '#process' => '_publication_xml_process_country'
          )
        )
      )
    )
  );
  $title_and_authors_schema = array(
    'fields' => array(
      'title' => array(
        'value' => array(
          'title' => array(
            '#field' => 'field_publication_title'
          )
        )
      )
    ),
    'author' => array_merge_recursive($author_schema_template, array(
      '#field' => 'field_publication_submit_author',
      '#min_occurence' => 1,
      'fields' => array(
        'submitting_author' => array(
          'value' => array(
            '#value' => 1
          )
        )
      )
    )),
    'author#1' => array_merge_recursive($author_schema_template, array(
      '#field' => 'field_publication_authors',
      '#condition' => 'field_publication_authors'
    ))
  );
  $abstract_and_keywords = array(
    'fields' => array(
      'abstract' => array(
        'value' => array(
          '#field' => 'field_publication_abstract'
        )
      ),
      'keywords' => array(
        'value' => array(
          '#field' => 'field_publication_keywords'
        )
      )
    )
  );
  $contributors = array(
    'contributor' => array(
      '#field' => 'field_publication_contributors',
      '#load' => '_publication_xml_load_user',
      '#value' => '',
      'fields' => array(
        'e-mail' => array(
          'value' => array(
            '#field' => 'mail'
          )
        ),
        'salutation' => array(
          'value' => array(
            '#field' => 'field_user_title',
            '#process' => '_pensoft_publication_xml_process_salutation'
          )
        ),
        'first_name' => array(
          'value' => array(
            '#field' => 'field_user_given_names'
          )
        ),
        //'middle_name' => array(
        //  '#value' => ''
        //),
        'last_name' => array(
          'value' => array(
            '#field' => 'field_user_family_name'
          )
        )
      ),
      //'rights' => array(
      //  'value' => array(
      //    '#value' => htmlspecialchars('Edit & comment'), //"Edit & comment" or "Comment only"
      //  )
      // role => array(
      //  'value' => array(
      //    '#value' => 'Linguistic editor', 'Copy editor', 'Mentor', 'Potential reviewer', 'Colleague / Friend'
      //   )
      //  ),
      'address' => array(
        'fields' => array(
          // affiliation, city
          'country' => array(
            'value' => array(
              '#field' => 'field_user_country',
              '#process' => '_publication_xml_process_country'
            )
          )
        )
      )
    )
  );
  $article_metadata_schema = array(
    'title_and_authors' => $title_and_authors_schema,
    'abstract_and_keywords' => $abstract_and_keywords,
    'contributors' => $contributors
  //'nomenclature_metadata' => $nomenclature_metadata
  );
  return $article_metadata_schema;
}

/**
 * The introduction section
 */
function _pensoft_publication_xml_introduction_schema(){
  $introduction = array(
    'fields' => array(
      'introduction' => array(
        'value' => array(
          '#field' => 'field_publication_introduction'
        )
      )
    )
  );
  return $introduction;
}

/**
 *  The material_and_methods section
 */
function _pensoft_publication_xml_material_and_methods_schema(){
  $material_and_methods = array(
    'fields' => array(
      'materials' => array(
        'value' => array(
          '#field' => 'field_publication_methods'
        )
      )
    )
  );
  return $material_and_methods;
}

/**
 * The systematics section
 */
function _pensoft_publication_xml_systematics_schema(){
  $distribution = array(
    'fields' => array(
      'distribution' => array(
        '#field' => 'field_distribution'
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_dispersal',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Dispersal'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_dispersal'
          )
        )
      )
    )
  );
  $ecology = array(
    'fields' => array(
      'ecology' => array(
        '#field' => 'field_ecology'
      )
    ),
    'subsection#1' => array(
      '#condition' => 'field_associations',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Associations'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_associations'
          )
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_cyclicity',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Cyclicity'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_cyclicity'
          )
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_habitat',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Habitat'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_habitat'
          )
        )
      )
    ),
    'subsection#4' => array(
      '#condition' => 'field_life_cycle',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Life cycle'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_life_cycle'
          )
        )
      )
    ),
    'subsection#5' => array(
      '#condition' => 'field_life_expectancy',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Life expectancy'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_life_expectancy'
          )
        )
      )
    ),
    'subsection#6' => array(
      '#condition' => 'field_migration',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Migration'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_migration'
          )
        )
      )
    ),
    'subsection#7' => array(
      '#condition' => 'field_population_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Population biology'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_population_biology'
          )
        )
      )
    ),
    'subsection#8' => array(
      '#condition' => 'field_reproduction',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Reproduction'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_reproduction'
          )
        )
      )
    ),
    'subsection#9' => array(
      '#condition' => 'field_trophic_strategy',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Trophic strategy'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_trophic_strategy'
          )
        )
      )
    )
  );
  $conservation = array(
    'fields' => array(
      'conservation' => array(
        'value' => array(
          '#field' => 'field_conservation_status'
        )
      )
    ),
    /* // XXX conservation does not allow sub sections.
    'subsection' => array(
      '#condition' => 'field_legislation',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Legislation'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_legislation'
          )
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_management',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Management'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_management'
          )
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_procedures',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Procedures'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_procedures'
          )
        )
      )
    ),
    'subsection#4' => array(
      '#condition' => 'field_threats',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Threats'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_threats'
          )
        )
      )
    ),
    'subsection#5' => array(
      '#condition' => 'field_trends',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Trends'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_trends'
          )
        )
      )
    )
    */
  );
  $description = array(
    'subsection' => array(
      '#condition' => 'field_behavious',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Behavious'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_behavious'
          )
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_cytology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Cytology'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_cytology'
          )
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_diagnostic_description',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Diagnostic description'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_diagnostic_description'
          )
        )
      )
    ),
    'subsection#4' => array(
      '#condition' => 'field_genetics',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Genetics'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_genetics'
          )
        )
      )
    ),
    'subsection#5' => array(
      '#condition' => 'field_growth',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Growth'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_growth'
          )
        )
      )
    ),
    'subsection#6' => array(
      '#condition' => 'field_look_alikes',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Look alikes'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_look_alikes'
          )
        )
      )
    ),
    'subsection#7' => array(
      '#condition' => 'field_molecular_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Molecular biology'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_molecular_biology'
          )
        )
      )
    ),
    'subsection#8' => array(
      '#condition' => 'field_morphology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Morphology'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_morphology'
          )
        )
      )
    ),
    'subsection#9' => array(
      '#condition' => 'field_physiology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Physiology'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_physiology'
          )
        )
      )
    ),
    'subsection#10' => array(
      '#condition' => 'field_size',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Size'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_size'
          )
        )
      )
    ),
    'subsection#11' => array(
      '#condition' => 'field_taxon_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Taxon biology'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_taxon_biology'
          )
        )
      )
    )
  );
  $evolution = array(
    'fields' => array(
      '#condition' => 'field_evolution',
      'title' => array(
        'value' => array(
          '#value' => 'Evolution'
        )
      ),
      'content' => array(
        'value' => array(
          '#field' => 'field_evolution'
        )
      )
    ),
    'subsection' => array(
      '#condition' => 'field_phylogeny',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Phylogeny'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_phylogeny'
          )
        )
      )
    )
  );
  $biology = array(
    'fields' => array(
      '#condition' => 'field_biology',
      'biology' => array(
        'value' => array(
          '#field' => 'field_biology' // Originally from "overview" group
        )
      )
    )
  );
  $relevance = array(
    'fields' => array(
      '#condition' => 'field_diseases',
      'content' => array(
        'value' => array(
          '#field' => 'field_diseases'
        )
      ),
      'title' => array(
        'value' => array(
          '#value' => 'Diseases'
        )
      )
    ),
    'subsection' => array(
      '#condition' => 'field_risk_statement',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Risk statement'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_risk_statement'
          )
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_risk_uses',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Uses'
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_uses'
          )
        )
      )
    )
  );
  $systematics = array(
    'fields' => array(
      'systematics' => array(
        'value' => array(
          '#field' => 'field_publication_systematics'
        )
      )
    ),
    'treatment' => array(
      '#load' => '_pensoft_publication_xml_load_treatment',
      '#field' => 'field_publication_treatments',
      '#value' => '',
      'fields' => array(
        'classification' => array(
          '#field' => 'field_taxonomic_name',
          '#load' => '_publication_xml_load_parent_term',
          '#load arguments' => array(
            'Family',
            (object)(array(
              'name' => ''
            ))
          ),
          '#value' => '',
          'value' => array(
            '#field' => 'name'
          )
        ),
        'rank' => array(
          'value' => array(
            '#field' => 'field_rank' // from the term, only 'Species' or 'Genus' allowed
          )
        ),
        'type_of_treatment' => array(
          'value' => array(
            '#field' => 'field_publication_treat_type'
          )
        ),
        'select_type' => array(
          'value' => array(
            '#value' => '' // 'extant' or 'fossil' or ''. This is part of Specimen for us, not taxon ?
          )
        ),
        'habitat' => array(
          '#condition' => 'field_habitat',
          'value' => array(
            '#field' => 'field_habitat' // 'terrestrial', 'marine', 'freshwater', 'N/A' or 'empty'
          )
        )
      ),
      'taxon_name_genus' => array(
        '#condition' => 'field_rank',
        '#condition arguments' => array(
          '=',
          'Species'
        ),
        '#load' => '_publication_xml_load_parent_term',
        '#load arguments' => array(
          'Genus'
        ),
        '#field' => 'field_taxonomic_name',
        '#value' => '',
        'fields' => array(
          'genus' => array( // Genus if we have selected a species (parent genus)
            'value' => array(
              '#field' => 'name'
            )
          ),
          // year
          'taxon_authors' => array(
            '#condition' => 'field_authors',
            'value' => array(
              '#field' => 'field_authors' // from the term
            )
          )
        )
      ),
      /*
      'type_species' => array(
        'fields' => array(
          'status' => array(
            '#value' => '', //empty or "new species described in this paper" or "previously described species"
          ),
          'genus' => array(
            '#value' => '',
          ),
          'species' => array(
            '#value' => '',
          ),
          'taxon_authors' => array(
            '#value' => '',
          ),
          'year' => array(
            '#value' => '',
          ),
        ), 
      ),
      */
      'materials' => _pensoft_publication_xml_systematics_materials_schema(),
      'description' => $description,
      'section' => $evolution,
      'section#2' => $relevance,
      'biology' => $biology,
      'ecology' => $ecology,
      'distribution' => $distribution,
      'conservation' => $conservation
    )
  );
  return $systematics;
}

/**
 * The materials
 */
function _pensoft_publication_xml_systematics_materials_schema(){
  $type_template = array(
    '#load' => '_publication_xml_load_specimen',
    '#field' => 'field_publication_treat_holotype',
    '#value' => '',
    'fields' => array(
      'type_status' => array(
        'value' => array(
          '#value' => 'Holotype'
        )
      ),
      // occurrencedetails
      'catalognumber' => array(
        '#condition' => 'field_catalogue_number',
        'value' => array(
          '#field' => 'field_catalogue_number'
        )
      ),
      // occurrenceremarks
      'recordnumber' => array(
        '#condition' => 'field_number',
        'value' => array(
          '#field' => 'field_number'
        )
      ),
      'recordedby' => array(
        '#condition' => 'field_collector',
        '#load' => '_publication_xml_load_user',
        '#field' => 'field_collector',
        'value' => array(
          '#field' => 'processed_full_name'
        )
      ),
      //individualid - would that be the title ?
      'individualcount' => array(
        '#condition' => 'field_count',
        'value' => array(
          '#field' => 'field_count'
        )
      ),
      'sex' => array(
        '#condition' => 'field_sex',
        'value' => array(
          '#field' => 'field_sex'
        )
      ),
      'lifestage' => array(
        '#condition' => 'field_lifestage',
        'value' => array(
          '#field' => 'field_lifestage'
        )
      ),
      // reproductivecondition, behavior, establishmentmeans,
      // occurencestatus,
      // preparations, disposition
      'othercatalognumbers' => array(
        '#condition' => 'field_other_catalogue_number',
        'value' => array(
          '#field' => 'field_other_catalogue_number'
        )
      ),
      // XXX Should all these things be copied in from the taxonomic term ?
      // previousidentifications, associatedmedia, associatedreferences,associatedsequences
      //taxonid
      //scientificnameid, acceptednameusageid,parentnameusageid,originalnameusageid,nameaccordingtoid,
      //namepushedinid, taxonconceptid,scientificname,acceptednameusage,parentnameusage,originalnameusage,
      //nameaccordinto,namepublishedin,higherclassification,kingdom,phylum,class,order,family,genus,subgenus,
      //specificepithet,infraspecificepithet,taxonrank,verbatimtaxonrank,scientificnameauthorship,
      //vernacularname,nomenclaturalcode,
      //taxonomicstatus, nomenclaturalstatus,taxonremarks,
      /*** LOCATION ***/
      // locationid,highergeographid,higergeography
      'continent' => array(
        '#condition' => 'generated_continent',
        'value' => array(
          '#field' => 'generated_continent'
        )
      ),
      'waterbody' => array(
        '#condition' => 'generated_waterbody',
        'value' => array(
          '#field' => 'generated_waterbody'
        )
      ),
      'islandgroup' => array(
        '#condition' => 'field_island_group',
        'value' => array(
          '#field' => 'field_island_group'
        )
      ),
      'island' => array(
        '#condition' => 'field_island',
        'value' => array(
          '#field' => 'field_island'
        )
      ),
      'country' => array(
        '#condition' => 'field_country',
        'value' => array(
          '#field' => 'field_country',
          '#process' => '_publication_xml_process_country'
        )
      ),
      'countrycode' => array(
        '#condition' => 'field_country',
        'value' => array(
          '#field' => 'field_country',
          '#process' => '_publication_xml_process_country',
          '#process arguments' => array(
            'iso2'
          )
        )
      ),
      'stateprovince' => array(
        '#condition' => 'field_state_province',
        'value' => array(
          '#field' => 'field_state_province'
        )
      ),
      'county' => array(
        '#condition' => 'field_county',
        'value' => array(
          '#field' => 'field_county'
        )
      ),
      //municipality
      'locality' => array(
        '#condition' => 'field_locality',
        'value' => array(
          '#field' => 'field_locality'
        )
      ),
      'verbatimlocality' => array(
        '#condition' => 'field_locality',
        'value' => array(
          '#field' => 'field_locality'
        )
      ),
      //verbatimelevation
      'minimumelevationinmeters' => array(
        '#condition' => 'field_min_elevation',
        'value' => array(
          '#field' => 'field_min_elevation'
        )
      ),
      'maximumelevationinmeters' => array(
        '#condition' => 'field_max_elevation',
        'value' => array(
          '#field' => 'field_max_elevation'
        )
      ),
      //verbatimdepth
      'minimumdepthinmeters' => array(
        '#condition' => 'field_min_depth',
        'value' => array(
          '#field' => 'field_min_depth'
        )
      ),
      'maximumdepthinmeters' => array(
        '#condition' => 'field_max_depth',
        'value' => array(
          '#field' => 'field_max_depth'
        )
      ),
      //minimumdisanceabovesurfaceinmeters,maximumdistanceabovesurfaceinmeters,
      //locationaccordinto,locationremarks,
      //verbatimcoordinates
      'verbatimlatitude' => array(
        '#condition' => 'field_map',
        'value' => array(
          '#field' => 'field_map',
          '#process' => '_publication_xml_process_map',
          '#process arguments' => array(
            'latitude'
          )
        )
      ),
      'verbatimlongitude' => array(
        '#condition' => 'field_map',
        'value' => array(
          '#field' => 'field_map',
          '#process' => '_publication_xml_process_map',
          '#process arguments' => array(
            'longitude'
          )
        )
      ),
      'verbatimcoordinatesystem' => array(
        '#condition' => 'field_coordinate_system',
        'value' => array(
          '#field' => 'field_coordinate_system'
        )
      ),
      'decimallatitude' => array(
        '#condition' => 'field_map',
        'value' => array(
          '#field' => 'field_map',
          '#process' => '_publication_xml_process_map',
          '#process arguments' => array(
            'latitude'
          )
        )
      ),
      'decimallongitude' => array(
        '#condition' => 'field_map',
        'value' => array(
          '#field' => 'field_map',
          '#process' => '_publication_xml_process_map',
          '#process arguments' => array(
            'longitude'
          )
        )
      ),
      'geodeticdatum' => array(
        '#condition' => 'field_geodetic_datum',
        'value' => array(
          '#field' => 'field_geodetic_datum'
        )
      ),
      'coordinateuncertaintyinmeters' => array(
        '#condition' => 'field_coordinate_uncertainty',
        'value' => array(
          '#field' => 'field_coordinate_uncertainty'
        )
      ),
      //coordinateprecision, pointradiusspatialfit,footprintwkt,footprintsrs,
      //footprintspatialfit,georeferencedby,
      'georeferenceprotocol' => array(
        '#condition' => 'field_georeference_protocol',
        'value' => array(
          '#field' => 'field_georeference_protocol'
        )
      ),
      //georeferencesources, georeferenceverificationstatus,
      'georeferenceremarks' => array(
        '#condition' => 'field_georeference_remarks',
        'value' => array(
          '#field' => 'field_georeference_remarks'
        )
      ),
      /*** END OF LOCATION ? ***/
      'identificationid' => array(
        'value' => array(
          '#field' => 'title'
        )
      ),
      'identifiedby' => array(
        '#condition' => 'field_identified_by',
        '#load' => '_publication_xml_load_user',
        '#field' => 'field_identified_by',
        'value' => array(
          '#field' => 'processed_full_name'
        )
      ),
      'dateidentified' => array(
        '#condition' => 'field_date_identified',
        'value' => array(
          '#field' => 'field_date_identified'
        )
      ),
      //identificationreferences, 
      'identificationremarks' => array(
        '#condition' => 'field_remarks',
        'value' => array(
          '#field' => 'field_remarks'
        )
      ),
      'identificationqualifier' => array(
        '#condition' => 'field_identification_qualifier',
        'value' => array(
          '#field' => 'field_identification_qualifier'
        )
      ),
      //eventid, samplingprotocol,samplingeffort,eventdate,eventtime,
      //startdateofyear,enddayofyear,year,month,day,
      //verbatimeeventdate,
      // habitat
      'fieldnumber' => array(
        '#condition' => 'field_number',
        'value' => array(
          '#field' => 'field_number'
        )
      ),
      'fieldnotes' => array(
        '#condition' => 'field_notes',
        'value' => array(
          '#field' => 'field_notes'
        )
      ),
      //eventremarks, type, modified, language, rights, rightsholder, accessrights,
      //bibliographiccitation
      //insitutionid,collectionid,datasetid,
      'institutioncode' => array(
        '#condition' => 'field_institution_code',
        'value' => array(
          '#field' => 'field_institution_code'
        )
      ),
      'collectioncode' => array(
        '#condition' => 'field_collection_code',
        'value' => array(
          '#field' => 'field_collection_code'
        )
      ),
      //datasetname,ownerinstitutionocde,
      'basisofrecord' => array(
        '#condition' => 'field_basis_of_record',
        'value' => array(
          '#field' => 'field_basis_of_record'
        )
      )
    //informationwithheld,datageneralizations,dynamicproperties,source,
    //geolocgicalcontextid,earliestornorlowelestenothem,
    //latesteonorhighesteonothem, earlisesteraorlowesterathem,
    //latesteraorhighesterathem, ...
    // group, formation, member, bed
    )
  );
  $holotype = $type_template;
  $paratypes = $type_template;
  $paratypes['#field'] = 'field_publication_treat_paratype';
  $paratypes['fields']['type_status']['value']['#value'] = 'Paratype(s)';
  return array(
    'material' => $holotype,
    'material#1' => $paratypes
  );
}

/**
 * The acknowledgements section
 */
function _pensoft_publication_xml_acknowledgements_schema(){
  $ack = array(
    'fields' => array(
      'acknowledgements' => array(
        '#condition' => 'field_pensoft_acknowledgments',
        'value' => array(
          '#field' => 'field_pensoft_acknowledgments'
        )
      )
    )
  );
  return $ack;
}

/**
 * The suplementary files section
 */
function _pensoft_publication_xml_files_schema(){
  $f = array(
    'supplementary_file' => array(
      'fields' => array(
        '#field' => 'field_pensoft_files',
        '#value' => '',
        '#load' => '_publication_xml_load_file',
        'title' => array(
          'value' => array(
            '#field' => 'title'
          )
        ),
        'type' => array(
          '#field' => 'type'
        ),
        'brief_description' => array(
          '#field' => 'description'
        ),
        'file' => array(
          '#field' => 'url'
        )
      )
    )
  );
  return $f;
}

/**
 * The figures section
 * 
 */
function _pensoft_publication_xml_figures_schema(){
  $f = array(
    'figure' => array(
      '#attributes' => array(
        'id' => 'xxx',
        'is_plate' => 'xxx',
        'type' => 'xxx'
      ),
      'caption' => array(
        '#value' => 'xxx'
      ),
      'url' => array(
        '#attributes' => array(
          'id' => 'xxx'
        )
      ),
      'photo_description' => array(
        '#value' => 'xxx'
      )
    )
  );
  return $f;
}

/**
 * The tables section
 */
function _pensoft_publication_xml_tables_schema(){
  $f = array();
  return $f;
}
