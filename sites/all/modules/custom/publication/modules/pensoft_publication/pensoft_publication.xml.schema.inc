<?php

/**
 * This file contains the schema/map definitions for the pensoft XML export, as
 * defined in the _pensoft_xml_build_from_schema function.
 * 
 */
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
    '#load' => '_pensoft_publication_xml_load_user',
    '#value' => '',
    'fields' => array(
      'e-mail' => array(
        'value' => array(
          '#field' => 'mail'
        ),
      ),
      'salutation' => array(
        'value' => array(
          '#field' => 'field_user_title',
          '#process' => '_pensoft_publication_xml_process_salutation'
        ),
      ),
      'first_name' => array(
        'value' => array(
          '#field' => 'field_user_given_names'
        ),
      ),
      //'middle_name' => array(
      //  '#value' => ''
      //),
      'last_name' => array(
        'value' => array(
          '#field' => 'field_user_family_name'
        ),
      ),
      'rights' => array(
        'value' => array(
          '#value' => htmlspecialchars('Edit & comment'), //"Edit & comment" or "Comment only"
        )
      ),
      // corresponding_author, rights, submitting_author
    ),
    'address' => array(
      'fields' => array(
        // affiliation, city
        'country' => array(
          'value' => array(
            '#field' => 'field_user_country',
            '#process' => '_pensoft_publication_xml_process_country'
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
          ),
        ),
      )
    ),
    'author' => array_merge_recursive($author_schema_template, array(
      '#field' => 'field_publication_submit_author',
      '#min_occurence' => 1,
      'fields' => array(
        'submitting_author' => array(
          'value' => array(
            '#value' => 1
          ),
        ),
      ),
    )),
    'author#1' => array_merge_recursive($author_schema_template, array(
      '#field' => 'field_publication_authors',
    ))
  );
  $abstract_and_keywords = array(
    'fields' => array(
      'abstract' => array(
        'value' => array(
          '#field' => 'field_publication_abstract'
        ),
      ),
      'keywords' => array(
        'value' => array(
          '#field' => 'field_publication_keywords',
        ),
      )
    )
  );
  $contributors = array(
    'contributor' => array(
      '#field' => 'field_publication_contributors',
      '#load' => '_pensoft_publication_xml_load_user',
      '#value' => '',
      'fields' => array(
        'e-mail' => array(
          'value' => array(
            '#field' => 'mail'
          ),
        ),
        'salutation' => array(
          'value' => array(
            '#field' => 'field_user_title',
            '#process' => '_pensoft_publication_xml_process_salutation'
          ),
        ),
        'first_name' => array(
          'value' => array(
            '#field' => 'field_user_given_names'
          ),
        ),
        //'middle_name' => array(
        //  '#value' => ''
        //),
        'last_name' => array(
          'value' => array(
            '#field' => 'field_user_family_name'
          ),
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
      ),
      'address' => array(
        'fields' => array(
          // affiliation, city
          'country' => array(
            'value' => array(
              '#field' => 'field_user_country',
              '#process' => '_pensoft_publication_xml_process_country'
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
        ),
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
          ),
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
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_cyclicity'
          ),
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_habitat',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Habitat'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_habitat',
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
          ),
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
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_life_expectancy'
          ),
        )
      )
    ),
    'subsection#6' => array(
      '#condition' => 'field_migration',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Migration'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_migration'
          ),
        )
      )
    ),
    'subsection#7' => array(
      '#condition' => 'field_population_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Population biology'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_population_biology'
          ),
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
        ),
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
          ),
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
          ),
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
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_look_alikes'
          ),
        )
      )
    ),
    'subsection#7' => array(
      '#condition' => 'field_molecular_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Molecular biology'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_molecular_biology'
          ),
        )
      )
    ),
    'subsection#8' => array(
      '#condition' => 'field_morphology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Morphology'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_morphology'
          ),
        )
      )
    ),
    'subsection#9' => array(
      '#condition' => 'field_physiology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Physiology'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_physiology'
          ),
        )
      )
    ),
    'subsection#10' => array(
      '#condition' => 'field_size',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Size'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_size'
          ),
        )
      )
    ),
    'subsection#11' => array(
      '#condition' => 'field_taxon_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Taxon biology'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_taxon_biology'
          ),
        )
      )
    ),
  );
  $evolution = array(
    'fields' => array(
      '#condition' => 'field_evolution',
      'title' => array(
        'value' => array(        
          '#value' => 'Evolution'
        ),
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
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_phylogeny'
          ),
        )
      )
    ),
  );
  $biology = array(
    'fields' => array(
      'biology' => array(
        'value' => array(
          '#field' => 'field_biology', // Originally from "overview" group
        ),
      ),      
    ),
  );
  $relevance = array(
    'fields' => array(
      '#condition' => 'field_diseases',
      'content' => array(
        'value' => array(
          '#field' => 'field_diseases'
        ),
      ),
      'title' => array(
        'value' => array(
          '#value' => 'Diseases'
        ),
      ),
    ),
    'subsection' => array(
      '#condition' => 'field_risk_statement',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Risk statement'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_risk_statement'
          ),
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_risk_uses',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Uses'
          ),
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_uses'
          ),
        )
      )
    ),
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
        /*
        'classification' => array(
          'value' => array(
            '#field' => 'field_classification', // Parent term ?
          ),
        ),
        */
        'rank' => array(
          'value' => array(
            '#field' => 'field_rank', // from the term, only 'Species' or 'Genus' allowed
          )
        ),
        'type_of_treatment' => array(
          'value' => array(
            '#value' => 'New taxon', // 'New taxon' or 'Redescription'
          ),
        ),
        'select_type' => array(
          'value' => array(
            '#value' => '', // 'extant' or 'fossil' or ''. This is part of Specimen for us, not taxon ?
          ),
        ),        
        'habitat' => array(
          '#condition' => 'field_habitat',
          'value' => array(
            '#field' => 'field_habitat', // 'terrestrial', 'marine', 'freshwater', 'N/A' or 'empty'
          ),
        ),
      ),
      'taxon_name_genus' => array(
        'fields' => array(
          /*
          'genus' => array(
           ),
          'year' => array(
          ),
          */
          'taxon_authors' => array(
            '#condition' => 'field_authors',
            'value' => array(
              '#field' => 'field_authors' // from the term
            ),
          ),
        ),
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
      'materials' => array(
        'material' => array(
          'fields' => array(
            .....
          )
        )
      ),
      */
      'description' => $description,
      'section' => $evolution,
      'section#2' => $relevance,
      'biology' => $biology,
      'ecology' => $ecology,
      'distribution' => $distribution,
      'conservation' => $conservation,
    )
  );
  return $systematics;
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
        '#load' => '_pensoft_publication_xml_load_file',
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
function _pensoft_publication_xml_figures_schema() {
  $f = array(
    'figure' => array(
      '#attributes' => array('id' => 'xxx', 'is_plate' => 'xxx', 'type' => 'xxx'),
      'caption' => array(
        '#value' => 'xxx',
      ),
      'url' => array(
        '#attributes' => array('id' => 'xxx'),
      ),
      'photo_description' => array(
        '#value' => 'xxx',
      ),
    ),
  );
  
  return $f;
}

/**
 * The tables section
 */
function _pensoft_publication_xml_tables_schema() {
  $f = array(
  );

  return $f;
}
