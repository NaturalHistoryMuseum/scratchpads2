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
    'Prof.' => 'Prof',
    'Miss' => 'Ms'
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
 * Loader function for authors
 */
function _pensoft_publication_xml_load_author($value) {
  $collection = reset(entity_load('field_collection_item', array($value)));
  if (empty($collection->field_publication_author_user[LANGUAGE_NONE][0]['uid'])) {
    // It's possible to create a field collection without an author. If this was done,
    // just ignore it.
    return NULL;
  }
  $wrapper = entity_metadata_wrapper('field_collection_item', $collection);
  $author = _publication_xml_load_user($collection->field_publication_author_user[LANGUAGE_NONE][0]['uid']);
  if ($wrapper->field_publication_author_rights->value()) {
    $author->processed_rights = $wrapper->field_publication_author_rights->value();
  }
  if ($wrapper->field_publication_author_submit->value()) {
    $author->processed_submit = 1;
  } else {
    $author->processed_submit = 0;
  }

  return $author;
}

/**
 * Loader function for  contributors
 */
function _pensoft_publication_xml_load_contributor($value) {
  $collection = reset(entity_load('field_collection_item', array($value)));
  if (empty($collection->field_publication_cbtor_user[LANGUAGE_NONE][0]['uid'])) {
    // It's possible to create a field collection without an author. If this was done,
    // just ignore it.
    return NULL;
  }
  $wrapper = entity_metadata_wrapper('field_collection_item', $collection);
  $contributor = _publication_xml_load_user($collection->field_publication_cbtor_user[LANGUAGE_NONE][0]['uid']);
  if ($wrapper->field_publication_cbtor_rights->value()) {
    $contributor->processed_rights = $wrapper->field_publication_cbtor_rights->value();
  }
  if ($wrapper->field_publication_cbtor_role->value()) {
    $contributor->processed_role = $wrapper->field_publication_cbtor_role->value();
  }

  return $contributor;
}

/**
 * Loader function for figures
 */
function _pensoft_publication_xml_load_figure($value) {
  $collection = reset(entity_load('field_collection_item', array($value)));
  $collection->pensoft_figure_url = file_create_url($collection->field_publication_figure_figure[LANGUAGE_NONE][0]['uri']);
  return $collection;
}

/**
 * Loader function for tables
 */
function _pensoft_publication_xml_load_table($value) {
  $collection = reset(entity_load('field_collection_item', array($value)));
  return $collection;
}

/**
 * Loader function for biblio
 */
function _pensoft_publication_xml_load_biblio($value) {
  $type_map = array(
    100 => 'Book',
    101 => 'Book chapter',
    102 => 'Journal article',
    103 => 'Conference paper',
    104 => 'Conference proceedings',
    108 => 'Thesis',
    113 => 'Software',
    132 => 'Website',
    107 => 'Website' // Re-map Web Artcile to Website
  );

  $node = node_load($value);
  if (isset($type_map[$node->biblio_type])) {
    $node->pensoft_type = $type_map[$node->biblio_type];
  } else {
    throw new Exception(t("Pensoft only supports the following reference types: %types. This was a problem for reference %title", array(
      '%types' => implode(',', array_unique($type_map)),
      '%title' => $node->title
    )));
  }
  // Biblio re-uses fields depending on the type, Pensoft doesn't -
  // so match fields depending on the type
  if ($node->type == 100) {
    $node->pensoft_number_of_pages = $node->biblio_pages;
    $node->pensoft_book_title = $node->title;
    $node->pensoft_city = $node->biblio_place_published;
  } else if ($node->type == 101) {
    $node->pensoft_chapter_title = $node->title;
    $node->pensoft_book_title = $node->secondary_title;
    $node->pensoft_city = $node->biblio_place_published;
    $pages = explode(' ', trim(preg_replace('/[^0-9]+/', ' ', $node->pages)));
    if (count($pages) > 0) {
      $node->pensoft_first_page = $pages[0];
    }
    if (count($pages) > 1) {
      $node->pensoft_last_page = $pages[1];
    }
  } else if ($node->type == 102) {
    $node->pensoft_article_title = $node->title;
    $node->pensoft_journal_name = $node->biblio_secondary_title;
    $node->pensoft_journal_volume = $node->biblio_volume;
  } else if ($node->type == 103) {
    $node->pensoft_conference_name = $node->biblio_secondary_title;
    $node->conference_location = $node->biblio_place_published;
  } else if ($node->type == 104) {
    $node->pensoft_conference_name = $node->biblio_secondary_title;
    $node->conference_location = $node->biblio_place_published;
  }

  return $node;
}

/**
 * Loader function for biblio author.
 * We get passed an already populated object, we just need to filter otu
 * unwanted ones.
 */
function _pensoft_publication_xml_load_biblio_author($value) {
  $biblio_author = array(1,2,3,4,5);
  if (in_array($value->auth_type, $biblio_author)) {
    return $value;
  } else {
    return NULL;
  }
}

/**
 * Loader function for biblio editor.
 * We get passed an already populated object, we just need to filter otu
 * unwanted ones.
 */
function _pensoft_publication_xml_load_biblio_editor($value) {
  $biblio_editor = array(10, 14);
  if (in_array($value->auth_type, $biblio_editor)) {
    return $value;
  } else {
    return NULL;
  }
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
    '#load' => '_pensoft_publication_xml_load_author',
    '#field' => 'field_publication_authors',
    '#value' => '',
    'fields' => array(
      'e-mail' => array(
        'value' => array(
          '#field' => 'mail'
        )
      ),
      'salutation' => array(
        'value' => array(
          '#error_info' => 'Title for user %',
          '#error_field' => 'name',
          '#field' => 'field_user_title',
          '#process' => '_pensoft_publication_xml_process_salutation'
        )
      ),
      'first_name' => array(
        'value' => array(
          '#field' => 'processed_first_name'
        )
      ),
      'middle_name' => array(
        '#condition' => 'processed_middle_names',
        'value' => array(
          '#field' => 'processed_middle_names',
        )
      ),
      'last_name' => array(
        'value' => array(
          '#field' => 'field_user_family_name'
        )
      ),
      // corresponding_author
      'rights' => array(
        '#condition' => 'processed_rights',
        'value' => array(
          '#field' => 'processed_rights'  //"Edit & comment" or "Comment only"
        )
      ),
      'submitting_author' => array(
        'value' => array(
          '#field' => 'processed_submit'
        )
      )
    ),
    'address' => array(
      'fields' => array(
        'affiliation' => array(
          '#condition' => 'field_institution',
          'value' => array(
            '#field' => 'field_institution',
          )
        ),
        // city
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
    'author' => $author_schema_template
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
      '#load' => '_pensoft_publication_xml_load_contributor',
      '#value' => '',
      'fields' => array(
        'e-mail' => array(
          'value' => array(
            '#field' => 'mail'
          )
        ),
        'salutation' => array(
          'value' => array(
            '#error_info' => 'Title for user %',
            '#error_field' => 'name',
            '#field' => 'field_user_title',
            '#process' => '_pensoft_publication_xml_process_salutation'
          )
        ),
        'first_name' => array(
          'value' => array(
            '#field' => 'processed_first_name'
          )
        ),
        'middle_name' => array(
          '#condition' => 'processed_middle_names',
          'value' => array(
            '#field' => 'processed_middle_names',
          ),
        ),
        'last_name' => array(
          'value' => array(
            '#field' => 'field_user_family_name'
          )
        ),
        'rights' => array(
          '#condition' => 'processed_rights',
          'value' => array(
            '#field' => 'processed_rights'
          )
        ),
        'role' => array(
          '#condition' => 'processed_role',
          'value' => array(
            '#field' => 'processed_role'
          )
        )
      ),
      'address' => array(
        'fields' => array(
          'affiliation' => array(
            '#condition' => 'field_institution',
            'value' => array(
              '#field' => 'field_institution',
            )
          ),
          // city
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
  /*
   'classification' => array(
     'taxon_classification' => array(
       'value' => array(
       ),
     ),
     'subject_classification' => array(
       'value' => array(
       ),
     ),
     'chronological_classification' => array(
       'value' => array(
       ),
     ),
     'geographical_classification' => array(
       'value' => array(
       ),
     )
   );
  */
  $article_metadata_schema = array(
    'title_and_authors' => $title_and_authors_schema,
    'abstract_and_keywords' => $abstract_and_keywords,
    'contributors' => $contributors
    // classifications, funding_agencies
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
      '#condition' => 'field_publication_methods',
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
 * The data resources section
 */
function _pensoft_publication_xml_data_resources_schema() {
  $data_resources = array(
    'fields' => array(
      '#condition' => 'field_publication_data_resources',
      'data_resources' => array(
        'value' => array(
          '#field' => 'field_publication_data_resources',
        ),
      ),
    )
  );

  return $data_resources;
}

/**
 * The results section
 */
function _pensoft_publication_xml_results_schema() {
  $results = array(
    'fields' => array(
      '#condition' => 'field_publication_results',
      'results' => array(
        'value' => array(
          '#field' => 'field_publication_results',
        ),
      ),
    )
  );

  return $results;
}

/**
 * The systematics section
 */
function _pensoft_publication_xml_systematics_schema(){
  $distribution = array(
    'fields' => array(
      'distribution' => array(
        '#condition' => 'field_distribution',
        'value' => array(
          '#field' => 'field_distribution'
        )
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
        '#condition' => 'field_ecology',
        'value' => array(
          '#field' => 'field_ecology'
        )
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
    'fields' => array(
      'description' => array(
        '#condition' => 'field_general_description',
        'value' => array(
          '#field' => 'field_general_description',
        )
      )
    ),
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
      '#condition' => 'field_publication_systematics',
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
 * The discussion section
 */
function _pensoft_publication_xml_discussion_schema() {
  $results = array(
    'fields' => array(
      '#condition' => 'field_publication_discussion',
      'discussions' => array(
        'value' => array(
          '#field' => 'field_publication_discussion',
        ),
      ),
    )
  );

  return $results;
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
 * The references section
 */
function _pensoft_publication_xml_references_schema() {
  $references = array(
    'reference' => array(
      '#field' => 'field_publication_references',
      '#load' => '_pensoft_publication_xml_load_biblio',
      '#value' => '',
      'fields' => array(
        'reference_type' => array(
          'value' => array(
            '#field' => 'pensoft_type',
          )
        ),
        'year_of_publication' => array(
          '#condition' => 'biblio_year',
          'value' => array(
            '#field' => 'biblio_year',
          ),
        ),
        'book_title' => array(
          '#condition' => 'pensoft_book_title',
          'value' => array(
            '#field' => 'pensoft_book_title',
          )
        ),
        'edition' => array(
          '#condition' => 'biblio_edition',
          'value' => array(
            '#field' => 'biblio_edition',
          )
        ),
        'volume' => array(
          '#condition' => 'biblio_volume',
          'value' => array(
            '#field' => 'biblio_volume',
          )
        ),
        'publisher' => array(
          '#condition' => 'biblio_publisher',
          'value' => array(
            '#field' => 'biblio_publisher',
          )
        ),
        'city' => array(
          '#condition' => 'pensoft_city',
          'value' => array(
            '#field' => 'pensoft_city',
          )
        ),
        'number_of_pages' => array(
          '#condition' => 'pensoft_number_of_pages',
          'value' => array(
            '#field' => 'pensoft_number_of_pages',
          )
        ),
        'publication_language' => array(
          '#condition' => 'biblio_lang',
          'value' => array(
            '#field' => 'biblio_lang',
          )
        ),
        'url' => array(
          '#condition' => 'biblio_url',
          'value' => array(
            '#field' => 'biblio_url',
          )
        ),
        'isbn' => array(
          '#condition' => 'biblio_isbn',
          'value' => array(
            '#field' => 'biblio_isbn',
          )
        ),
        'doi' => array(
          '#condition' => 'biblio_doi',
          'value' => array(
            '#field' => 'biblio_doi',
          )
        ),
        'translated_title' => array(
          '#condition' => 'biblio_translated_title',
          'value' => array(
            '#field' => 'biblio_translated_title',
          )
        ),
        'chapter_title' => array(
          '#condition' => 'pensoft_chapter_title',
          'value' => array(
            '#field' => 'pensoft_chapter_title',
          )
        ),
        'article_title' => array(
          '#condition' => 'pensoft_article_title',
          'value' => array(
            '#field' => 'pensoft_article_title',
          )
        ),
        'journal_name' => array(
          '#condition' => 'pensoft_journal_name',
          'value' => array(
            '#field' => 'pensoft_journal_name',
          )
        ),
        'issue' => array(
          '#condition' => 'biblio_issue',
          'value' => array(
            '#field' => 'biblio_issue',
          )
        ),
        'first_page' => array(
          '#condition' => 'pensoft_first_page',
          'value' => array(
            '#field' => 'pensoft_first_page',
          )
        ),
        'last_page' => array(
          '#condition' => 'pensoft_last_page',
          'value' => array(
            '#field' => 'pensoft_last_page',
          )
        ),
        'title' => array(
          'value' => array(
            '#field' => 'title',
          )
        ),
        'conference_name' => array(
          '#condition' => 'pensoft_conference_name',
          'value' => array(
            '#field' => 'pensoft_conference_name',
          )
        ),
        'conference_location' => array(
          '#condition' => 'pensoft_conference_location',
          'value' => array(
            '#field' => 'pensoft_conference_location',
          )
        ),
        //'conference_date' => array(
        //),
        'journal_volume' => array(
          '#condition' => 'pensoft_journal_volume',
          'value' => array(
            '#field' => 'pensoft_journal_volume',
          )
        ),
        //'version' => array(
        //),
        //'release_date' => array(
        //),
        //'access_date' => array(
        //),
      ),
      'reference_author' => array(
        '#field' => 'biblio_contributors',
        '#load' => '_pensoft_xml_load_biblio_author',
        'fields' => array(
          'name' => array(
            '#condition' => 'name',
            'value' => array(
              '#field' => 'name',
            ),
          ),
          'first_name' => array(
            '#condition' => 'firstname',
            'value' => array(
              '#field' => 'firstname',
            ),
          ),
          'last_name' => array(
            '#condition' => 'lastname',
            'value' => array(
              '#field' => 'lastname',
            ),
          ),
          'middle_name' => array(
            '#condition' => 'initials',
            'value' => array(
              '#field' => 'initials',
            ),
          ),
        ),
      ),
      'reference_editor' => array(
        '#field' => 'biblio_contributors',
        '#load' => '_pensoft_xml_load_biblio_editor',
        'fields' => array(
          'name' => array(
            '#condition' => 'name',
            'value' => array(
              '#field' => 'name',
            ),
          ),
          'first_name' => array(
            '#condition' => 'firstname',
            'value' => array(
              '#field' => 'firstname',
            ),
          ),
          'last_name' => array(
            '#condition' => 'lastname',
            'value' => array(
              '#field' => 'lastname',
            ),
          ),
          'middle_name' => array(
            '#condition' => 'initials',
            'value' => array(
              '#field' => 'initials',
            ),
          ),
        ),
      ),
    ),
  );

  return $references;
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
        // authors
        'type' => array(
          '#field' => 'type'
        ),
        'brief_description' => array(
          '#field' => 'description'
        ),
        //'file' => array(
        //  '#field' => 'url'
        //)
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
      '#field' => 'field_publication_figures',
      '#value' => '',
      '#load' => '_pensoft_publication_xml_load_figure',
      'caption' => array(
        '#field' => 'field_publication_figure_title'
      ),
      'url' => array(
        '#field' => 'pensoft_figure_url',
        //'#attributes' => array(
        //  'id' => ''
        //)
      ),
      'photo_description' => array(
        '#field' => 'field_publication_figure_desc'
      ),
      /*'#attributes' => array(
        'id' => '',
        'is_plate' => '',
        'type' => ''
      ),
      */
    )
  );
  return $f;
}

/**
 * The tables section.
 *
 * At the time of writting, pensoft haven't actually defined the
 * format for this, so I'm making it similar to the figures schema.
 *
 */
function _pensoft_publication_xml_tables_schema(){
  $f = array(
    'table' => array(
      '#min_occurence' => 0,
      '#max_occurence' => -1,
      '#field' => 'field_publication_tables',
      '#value' => '',
      '#load' => '_pensoft_publication_xml_load_table',
      'title' => array(
          '#field' => 'field_publication_table_title'
      ),
      'body' => array(
        '#field' => 'field_publication_table'
      )
    )
  );
  return $f;
}
