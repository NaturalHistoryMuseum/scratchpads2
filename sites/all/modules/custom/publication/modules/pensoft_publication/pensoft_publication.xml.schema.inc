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
        'id' => 2
      ),
      '#value' => 'Research article'
    ),
    'journal_name' => array(
      '#attributes' => array(
        'id' => 1
      ),
      '#value' => 'ZooKeys'
    )
  );
  return $document_info_schema;
}

/**
 * The article_metadata section
 */
function _pensoft_publication_xml_article_metadata_schema(){
  $title_and_authors_schema = array(
    'fields' => array(
      'title' => array(
        '#field' => 'field_publication_title'
      )
    ),
    'author' => array(
      '#field' => 'field_publication_authors',
      '#load' => '_pensoft_publication_xml_load_user',
      '#load arguments' => array(
        'field_publication_authors'
      ),
      '#value' => '',
      'fields' => array(
        'e-mail' => array(
          '#field' => 'mail'
        ),
        'salutation' => array(
          '#field' => 'field_user_title'
        ),
        'first_name' => array(
          '#field' => 'field_user_given_names'
        ),
        'middle_name' => array(
          '#value' => ''
        ),
        'last_name' => array(
          '#field' => 'field_user_family_name'
        )
      ),
      'secondary_address' => array(
        'fields' => array(
          'country' => array(
            '#field' => 'field_user_country',
            '#process' => '_pensoft_publication_xml_process_country'
          )
        )
      )
    )
  );
  $abstract_and_keywords = array(
    'fields' => array(
      'abstract' => array(
        '#field' => 'field_publication_abstract'
      ),
      'keywords' => array(
        '#field' => 'field_publication_keywords'
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
          '#field' => 'mail'
        ),
        'salutation' => array(
          '#field' => 'field_user_title'
        ),
        'first_name' => array(
          '#field' => 'field_user_given_names'
        ),
        'last_name' => array(
          '#field' => 'field_user_family_name'
        )
      ),
      'author_address' => array(
        'fields' => array(
          'country' => array(
            '#field' => array(
              'field_user_country'
            ),
            '#process' => '_pensoft_publication_xml_process_country'
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
        '#field' => 'field_publication_introduction'
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
        '#field' => 'field_publication_methods'
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
          '#value' => 'Dispersal'
        ),
        'content' => array(
          '#field' => 'field_dispersal'
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
          '#value' => 'Associations'
        ),
        'content' => array(
          '#field' => 'field_associations'
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_cyclicity',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Cyclicity'
        ),
        'content' => array(
          '#field' => 'field_cyclicity'
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_habitat',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Habitat'
        ),
        'content' => array(
          '#field' => 'field_habitat'
        )
      )
    ),
    'subsection#4' => array(
      '#condition' => 'field_life_cycle',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Life cycle'
        ),
        'content' => array(
          '#field' => 'field_life_cycle'
        )
      )
    ),
    'subsection#5' => array(
      '#condition' => 'field_life_expectancy',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Life expectancy'
        ),
        'content' => array(
          '#field' => 'field_life_expectancy'
        )
      )
    ),
    'subsection#6' => array(
      '#condition' => 'field_migration',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Migration'
        ),
        'content' => array(
          '#field' => 'field_migration'
        )
      )
    ),
    'subsection#7' => array(
      '#condition' => 'field_population_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Population biology'
        ),
        'content' => array(
          '#field' => 'field_population_biology'
        )
      )
    ),
    'subsection#8' => array(
      '#condition' => 'field_reproduction',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Reproduction'
        ),
        'content' => array(
          '#field' => 'field_reproduction'
        )
      )
    ),
    'subsection#9' => array(
      '#condition' => 'field_trophic_strategy',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Trophic strategy'
        ),
        'content' => array(
          '#field' => 'field_trophic_strategy'
        )
      )
    )
  );
  $conservation = array(
    'fields' => array(
      'conservation' => array(
        '#field' => 'field_conservation_status'
      )
    ),
    'subsection' => array(
      '#condition' => 'field_legislation',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Legislation'
        ),
        'content' => array(
          '#field' => 'field_legislation'
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_management',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Management'
        ),
        'content' => array(
          '#field' => 'field_management'
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_procedures',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Procedures'
        ),
        'content' => array(
          '#field' => 'field_procedures'
        )
      )
    ),
    'subsection#4' => array(
      '#condition' => 'field_threats',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Threats'
        ),
        'content' => array(
          '#field' => 'field_threats'
        )
      )
    ),
    'subsection#5' => array(
      '#condition' => 'field_trends',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Trends'
        ),
        'content' => array(
          '#field' => 'field_trends'
        )
      )
    )
  );
  $description = array(
    'subsection' => array(
      '#condition' => 'field_behavious',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Behavious'
        ),
        'content' => array(
          '#field' => 'field_behavious'
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_cytology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Cytology'
        ),
        'content' => array(
          '#field' => 'field_cytology'
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_diagnostic_description',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Diagnostic description'
        ),
        'content' => array(
          '#field' => 'field_diagnostic_description'
        )
      )
    ),
    'subsection#4' => array(
      '#condition' => 'field_genetics',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Genetics'
        ),
        'content' => array(
          '#field' => 'field_genetics'
        )
      )
    ),
    'subsection#5' => array(
      '#condition' => 'field_growth',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Growth'
        ),
        'content' => array(
          '#field' => 'field_growth'
        )
      )
    ),
    'subsection#6' => array(
      '#condition' => 'field_look_alikes',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Look alikes'
        ),
        'content' => array(
          '#field' => 'field_look_alikes'
        )
      )
    ),
    'subsection#7' => array(
      '#condition' => 'field_molecular_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Molecular biology'
        ),
        'content' => array(
          '#field' => 'field_molecular_biology'
        )
      )
    ),
    'subsection#8' => array(
      '#condition' => 'field_morphology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Morphology'
        ),
        'content' => array(
          '#field' => 'field_morphology'
        )
      )
    ),
    'subsection#9' => array(
      '#condition' => 'field_physiology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Physiology'
        ),
        'content' => array(
          '#field' => 'field_physiology'
        )
      )
    ),
    'subsection#10' => array(
      '#condition' => 'field_size',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Size'
        ),
        'content' => array(
          '#field' => 'field_size'
        )
      )
    ),
    'subsection#11' => array(
      '#condition' => 'field_taxon_biology',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Taxon biology'
        ),
        'content' => array(
          '#field' => 'field_taxon_biology'
        )
      )
    ),
  );
  $evolution = array(
    'subsection' => array(
      '#condition' => 'field_evolution',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Evolution'
        ),
        'content' => array(
          '#field' => 'field_evolution'
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_phylogeny',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Phylogeny'
        ),
        'content' => array(
          '#field' => 'field_phylogeny'
        )
      )
    ),
  );
  $biology = array(
    'fields' => array(
      'biology' => array(
        '#field' => 'field_biology', // Originally from "overview" group
      ),      
    ),
  );
  $relevance = array(
    'subsection' => array(
      '#condition' => 'field_diseases',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Diseases'
        ),
        'content' => array(
          '#field' => 'field_diseases'
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_risk_statement',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Risk statement'
        ),
        'content' => array(
          '#field' => 'field_risk_statement'
        )
      )
    ),
    'subsection#3' => array(
      '#condition' => 'field_risk_uses',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          '#value' => 'Uses'
        ),
        'content' => array(
          '#field' => 'field_uses'
        )
      )
    ),
  );
  
  $systematics = array(
    'fields' => array(
      'systematics' => array(
        '#field' => 'field_publication_treat_desc'
       )
    ),
    'treatment' => array(
      '#load' => '_pensoft_publication_xml_load_treatment',
      '#field' => 'field_publication_treatments',
      '#value' => '',
      'fields' => array(
        'rank' => array(
          '#field' => 'field_rank', // from the term
        ),
        'habitat' => array(
          '#field' => 'field_habitat',
        ),
      ),
      'taxon_name' => array(
        'fields' => array(
          'taxon_authors' => array(
            '#field' => 'field_authors' // from the term
          ),
        ),
      ),
      'taxon_treatment_sections1' => array(
        'taxon_treatment_distribution_section' => $distribution,
        'taxon_treatment_ecology_section' => $ecology,
        'taxon_treatment_conservation_section' => $conservation,
        'taxon_treatment_biology_section' => $biology,
        'taxon_treatment_custom_section' => $description,
        'taxon_treatment_custom_section#2' => $evolution,
        'taxon_treatment_custom_section#3' => $relevance,
      )
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
        '#field' => 'field_pensoft_acknowledgments'
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
          '#field' => 'title'
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



