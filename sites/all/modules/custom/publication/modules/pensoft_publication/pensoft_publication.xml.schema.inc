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
function _pensoft_publication_xml_process_salutation($context){
  $value = $context->value_to_insert;
  $map = array(
    'Prof.' => 'Prof',
    'Miss' => 'Ms'
  );
  if(isset($map[$value])){
    return array(
      'value' => $map[$value]
    );
  }else{
    return $value;
  }
}

/**
 * Loader function for the treatment/taxon description
 * 
 * We cannot use entity metadata wrappers with relations,
 * so this returns the node entity with fake fields
 * that can be accessed via process functions.
 */
function _pensoft_publication_xml_load_treatment($context, $value){
  $av = array_values($value['endpoints']);
  $endpoint = array_shift($av);
  list($type, $nid) = explode(':', $endpoint);
  $rid = $value['relation_id'];
  $relation = relation_load($rid);
  $node = node_load($nid);
  $relation_fields = array(
    'field_publication_treat_species',
    'field_publication_treat_authy',
    'field_publication_treat_fields',
    'field_publication_treat_holotype',
    'field_publication_treat_paratype',
    'field_publication_treat_type',
  );
  foreach($relation_fields as $relation_field){
    $node->{$relation_field} = field_get_items('relation', $relation, $relation_field);
  }
  return array(
    'entity_type' => 'node',
    'entity' => $node
  );
  //$tid = $node->field_taxonomic_name[LANGUAGE_NONE][0]['tid'];
  //$term = taxonomy_term_load($tid);
  //$combine = array_merge((array)$node, (array)$relation, (array)$term);
  //return ((object)$combine);
}

/**
 * Return a field value of the object
 */
function _pensoft_publication_xml_treatment_field($context, $field){
  return $context->entity->{$field};
}

/**
 * From a given taxonomic name, find the associated classification
 */
function _pensoft_publication_xml_process_classification($context){
  $term = $context->raw_value;
  $family = _publication_xml_load_parent_term($context, $term, 'Family');
  if ($family){
    return $family->name;
  } else {
    return '';
  }
}

/**
 * Process function for biblio values
 */
function _pensoft_publication_xml_process_biblio($context, $value_type){
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
  $values = array();
  $node = $context->entity;
  if (isset($type_map[$node->biblio_type])) {
    $values['type'] = $type_map[$node->biblio_type];
  } else {
    throw new Exception(t("Pensoft only supports the following reference types: %types. This was a problem for reference %title", array(
      '%types' => implode(',', array_unique($type_map)),
      '%title' => $node->title
    )));
  }
  // Biblio re-uses fields depending on the type, Pensoft doesn't -
  // so match fields depending on the type
  if ($node->biblio_type == 100) { // Book
    $values['number_of_pages'] = $node->biblio_pages;
    $values['book_title'] = $node->title;
    $values['city'] = $node->biblio_place_published;
  } else if ($node->biblio_type == 101) { // Book chapter
    $values['chapter_title'] = $node->title;
    $values['book_title'] = $node->biblio_secondary_title;
    $values['city'] = $node->biblio_place_published;
    $pages = explode(' ', trim(preg_replace('/[^0-9]+/', ' ', $node->biblio_pages)));
    if (count($pages) > 0) {
      $values['first_page'] = $pages[0];
    }
    if (count($pages) > 1) {
      $values['last_page'] = $pages[1];
    }
  } else if ($node->biblio_type == 102) { // Journal Article
    $values['article_title'] = $node->title;
    $values['journal_name'] = $node->biblio_secondary_title;
    $values['journal_volume'] = $node->biblio_volume;
  } else if ($node->biblio_type == 103) {  // Conference paper
    $values['conference_name'] = $node->biblio_secondary_title;
    $values['conference_location'] = $node->biblio_place_published;
  } else if ($node->biblio_type == 104) {  // Conference proceedings
    $values['conference_name'] = $node->biblio_secondary_title;
    $values['conference_location'] = $node->biblio_place_published;
  }
  if (isset($values[$value_type])){
    return $values[$value_type];
  } else {
    return NULL;
  }
}

/**
 * Loader function for biblio author.
 * We get passed an already populated object, we just need to filter otu
 * unwanted ones.
 */
function _pensoft_publication_xml_load_biblio_author($context, $value) {
  $biblio_author = array(1,2,3,4,5);
  if (in_array($value['auth_type'], $biblio_author)) {
    return (object)$value;
  } else {
    return NULL;
  }
}

/**
 * Loader function for biblio editor.
 * We get passed an already populated object, we just need to filter otu
 * unwanted ones.
 */
function _pensoft_publication_xml_load_biblio_editor($context, $value) {
  $biblio_editor = array(10, 14);
  if (in_array($value['auth_type'], $biblio_editor)) {
    return (object)$value;
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
        'id' => 9 // Taxonomy paper
      )
    ),
    'journal_name' => array(
      '#attributes' => array(
        'id' => 1 // BDJ
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
    '#field' => 'field_publication_authors',
    '#value' => '',
    '#child_entity' => array(
      '_publication_xml_load_entity',
      'field_collection_item'
    ),
    '#error_info' => 'Authors',
    'fields' => array(
      'e-mail' => array(
        '#entity' => array(
          '_publication_xml_load_entity_from_field',
          'field_publication_author_user',
          'user'
        ),
        'value' => array(
          '#field' => 'mail',
          '#error_info' => 'mail (author)'
        )
      ),
      'salutation' => array(
        '#entity' => array(
          '_publication_xml_load_entity_from_field',
          'field_publication_author_user',
          'user'
        ),
        'value' => array(
          '#error_info' => 'Title for user %',
          '#error_field' => 'name',
          '#field' => 'field_user_title',
          '#process' => '_pensoft_publication_xml_process_salutation'
        )
      ),
      'first_name' => array(
        '#entity' => array(
          '_publication_xml_load_entity_from_field',
          'field_publication_author_user',
          'user'
        ),
        'value' => array(
          '#field' => 'field_user_given_names',
          '#process' => array(
            '_publication_xml_process_user',
            'first'
          )
        )
      ),
      'middle_name' => array(
        '#entity' => array(
          '_publication_xml_load_entity_from_field',
          'field_publication_author_user',
          'user'
        ),
        '#condition' => array(
          '_publication_xml_process_user',
          'middle'
        ),
        'value' => array(
          '#field' => 'field_user_given_names',
          '#process' => array(
            '_publication_xml_process_user',
            'middle'
          )
        )
      ),
      'last_name' => array(
        '#entity' => array(
          '_publication_xml_load_entity_from_field',
          'field_publication_author_user',
          'user'
        ),
        'value' => array(
          '#field' => 'field_user_family_name'
        )
      ),
      // corresponding_author
      'rights' => array(
        'value' => array(
          '#field' => 'field_publication_author_rights',  //"Edit & comment" or "Comment only"
          '#comment' => '"Edit & comment" or "Comment only"',
          '#error_info' => 'Author rights for author %',
          '#error_field' => 'field_user_family_name'
        )
      ),
      'submitting_author' => array(
        'value' => array(
          '#field' => 'field_publication_author_submit',
          '#process' => '_publication_xml_boolean_to_int'
        )
      )
    ),
    'address' => array(
      '#entity' => array(
        '_publication_xml_load_entity_from_field',
        'field_publication_author_user',
        'user'
      ),
      'fields' => array(
        'affiliation' => array(
          '#condition' => 'field_user_institution',
          'value' => array(
            '#field' => 'field_user_institution',
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
          //'title' => array(
            '#field' => 'field_publication_title',
            '#raw' => TRUE,
          //)
        )
      )
    ),
    'author' => $author_schema_template
  );
  $abstract_and_keywords = array(
    'fields' => array(
      'abstract' => array(
        'value' => array(
          '#field' => 'field_publication_abstract',
          '#raw' => TRUE
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
      '#value' => '',
      '#child_entity' => array(
        '_publication_xml_load_entity',
        'field_collection_item'
      ),
      'fields' => array(
        'e-mail' => array(
          '#entity' => array(
            '_publication_xml_load_entity_from_field',
            'field_publication_cbtor_user',
            'user'
          ),
          'value' => array(
            '#field' => 'mail'
          )
        ),
        'salutation' => array(
          '#entity' => array(
            '_publication_xml_load_entity_from_field',
            'field_publication_cbtor_user',
            'user'
          ),
          'value' => array(
            '#error_info' => 'Title for user %',
            '#error_field' => 'name',
            '#field' => 'field_user_title',
            '#process' => '_pensoft_publication_xml_process_salutation'
          )
        ),
        'first_name' => array(
          '#entity' => array(
            '_publication_xml_load_entity_from_field',
            'field_publication_cbtor_user',
            'user'
          ),
          'value' => array(
            '#field' => 'field_user_given_names',
            '#process' => array(
              '_publication_xml_process_user',
              'first'
            )
          )
        ),
        'middle_name' => array(
          '#entity' => array(
            '_publication_xml_load_entity_from_field',
            'field_publication_cbtor_user',
            'user'
          ),
          '#condition' => array(
            '_publication_xml_process_user',
            'middle'
          ),
          'value' => array(
            '#field' => 'field_user_given_names',
            '#process' => array(
              '_publication_xml_process_user',
              'middle'
            )
          ),
        ),
        'last_name' => array(
          '#entity' => array(
            '_publication_xml_load_entity_from_field',
            'field_publication_cbtor_user',
            'user'
          ),
          'value' => array(
            '#field' => 'field_user_family_name'
          )
        ),
        'rights' => array(
          '#condition' => 'field_publication_cbtor_rights',
          'value' => array(
            '#field' => 'field_publication_cbtor_rights'
          )
        ),
        'role' => array(
          '#condition' => 'field_publication_cbtor_role',
          'value' => array(
            '#field' => 'field_publication_cbtor_role'
          )
        )
      ),
      'address' => array(
        '#entity' => array(
          '_publication_xml_load_entity_from_field',
          'field_publication_cbtor_user',
          'user'
        ),
        'fields' => array(
          'affiliation' => array(
            '#condition' => 'field_user_institution',
            'value' => array(
              '#field' => 'field_user_institution',
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
          '#field' => 'field_publication_introduction',
          '#raw' => TRUE,
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
          '#field' => 'field_publication_methods',
          '#raw' => TRUE
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
          '#raw' => TRUE,
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
          '#raw' => TRUE,
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
          '#field' => 'field_distribution',
          '#raw' => TRUE
        )
      )
    ),
    'subsection#2' => array(
      '#condition' => 'field_dispersal',
      'fields' => array(
        'enter_title_of_this_subsection' => array(
          'value' => array(
            '#value' => 'Dispersal',
          )
        ),
        'content' => array(
          'value' => array(
            '#field' => 'field_dispersal',
            '#raw' => TRUE
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
          '#field' => 'field_ecology',
          '#raw' => TRUE
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
            '#field' => 'field_associations',
            '#raw' => TRUE
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
            '#field' => 'field_cyclicity',
            '#raw' => TRUE
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
            '#field' => 'field_habitat',
            '#raw' => TRUE
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
            '#field' => 'field_life_cycle',
            '#raw' => TRUE
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
            '#field' => 'field_life_expectancy',
            '#raw' => TRUE
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
            '#field' => 'field_migration',
            '#raw' => TRUE
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
            '#field' => 'field_population_biology',
            '#raw' => TRUE
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
            '#field' => 'field_reproduction',
            '#raw' => TRUE
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
            '#field' => 'field_trophic_strategy',
            '#raw' => TRUE
          )
        )
      )
    )
  );
  $conservation = array(
    'fields' => array(
      'conservation' => array(
        '#condition' => 'field_conservation_status',
        'value' => array(
          '#field' => 'field_conservation_status',
          '#raw' => TRUE
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
          '#raw' => TRUE
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
            '#field' => 'field_behavious',
            '#raw' => TRUE
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
            '#field' => 'field_cytology',
            '#raw' => TRUE
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
            '#field' => 'field_diagnostic_description',
            '#raw' => TRUE
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
            '#field' => 'field_genetics',
            '#raw' => TRUE
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
            '#field' => 'field_growth',
            '#raw' => TRUE
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
            '#field' => 'field_look_alikes',
            '#raw' => TRUE
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
            '#field' => 'field_molecular_biology',
            '#raw' => TRUE
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
            '#field' => 'field_morphology',
            '#raw' => TRUE
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
            '#field' => 'field_physiology',
            '#raw' => TRUE
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
            '#field' => 'field_size',
            '#raw' => TRUE
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
            '#field' => 'field_taxon_biology',
            '#raw' => TRUE
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
          '#field' => 'field_evolution',
          '#raw' => TRUE
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
            '#field' => 'field_phylogeny',
            '#raw' => TRUE
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
          '#field' => 'field_biology',
          '#raw' => TRUE
        )
      )
    )
  );
  $relevance = array(
    'fields' => array(
      '#condition' => 'field_diseases',
      'content' => array(
        'value' => array(
          '#field' => 'field_diseases',
          '#raw' => TRUE
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
            '#field' => 'field_risk_statement',
            '#raw' => TRUE
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
            '#field' => 'field_uses',
            '#raw' => TRUE
          )
        )
      )
    )
  );
  $taxon_name_species = array(
    '#condition' => function($context) {
      return $context->child_object->field_rank[LANGUAGE_NONE][0]['value'] == 'Species';
    },
    'fields' => array(
      'genus' => array(
        '#condition' => 'field_publication_treat_genus',
        'value' => array(
          '#field' => 'field_publication_treat_genus',
        )
      ),
      'species' => array(
        '#condition' => 'field_publication_treat_species',
        'value' => array(
          '#field' => 'field_publication_treat_species',
        )
      ),
      'taxon_authors' => array(
        '#condition' => 'field_publication_treat_authy',
        'value' => array(
          '#field' => 'field_publication_treat_authy',
        )
      )
    )
  );
  $systematics = array(
    'treatment' => array(
      '#field' => 'field_publication_treatments',
      '#value' => '',
      '#child_entity' => '_pensoft_publication_xml_load_treatment',
      'fields' => array(
        'classification' => array(
          'value' => array(
            '#field' => 'field_taxonomic_name',
            '#process' => array(
              '_pensoft_publication_xml_process_classification'
            )
          )
        ),
        'rank' => array(
          'value' => array(
            '#entity' => '_publication_xml_load_taxonomic_name',
            '#field' => 'field_rank',
            '#comment' => "from the term, only 'Species' or 'Genus' allowed"
          )
        ),
        'type_of_treatment' => array(
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_pensoft_publication_xml_treatment_field',
              'field_publication_treat_type'
            )
          )
        ),
        'select_type' => array(
          '#entity' => '_publication_xml_load_taxonomic_name',
          '#condition' => function($context) {
            return $context->wrapper->field_rank->value() == 'Species';
          },
          'value' => array(
            '#value' => '', 
            '#comment' => "'extant' or 'fossil' or ''. This is part of Specimen for us, not taxon ?"
          )
        ),
        'habitat' => array(
          '#condition' => function($context) {
            $hab_value = $context->wrapper->field_habitat->value();
            if (empty($hab_value)){
              return FALSE;
            }
            $term = entity_metadata_wrapper('taxonomy_term', $context->wrapper->field_taxonomic_name->value());
            return $term->field_rank->value() == 'Species';
          },
          'value' => array(
            '#field' => 'field_habitat',
            '#comment' => "'terrestrial', 'marine', 'freshwater', 'N/A' or 'empty'"
          )
        )
        // parasite_of, classification_root_id, taxon_treatment_host_of, taxon_treatment_simbiotic_with, taxon_treatment_feeds_on
      ),
      'taxon_name_genus' => array(
        '#condition' => function($context) {
          $term = $context->wrapper->field_taxonomic_name->value();
          if ($context->wrapper->field_taxonomic_name->type() == 'list<taxonomy_term>'){
            $term = reset($term);
          }
          $term = entity_metadata_wrapper('taxonomy_term', $term);
          return $term->field_rank->value() == 'Genus';
        },
        'fields' => array(
          '#condition' => array(
            '_pensoft_publication_xml_treatment_field',
            'field_publication_treat_genus',
          ),
          'genus' => array(
            'value' => array(
              '#field' => 'nid',
              '#process' => array(
                '_pensoft_publication_xml_treatment_field',
                'field_publication_treat_genus',
              )
            )
          ),
          'taxon_authors' => array(
            '#condition' => array(
              '_pensoft_publication_xml_treatment_field',
              'field_publication_treat_authy'
            ),
            'value' => array(
              '#field' => 'nid',
              '#process' => array(
                '_pensoft_publication_xml_treatment_field',
                'field_publication_treat_authy',
              )
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
      /*
      'materials' => _pensoft_publication_xml_systematics_materials_schema(),
      'description' => $description,
      'section' => $evolution,
      'section#2' => $relevance,
      'biology' => $biology,
      'ecology' => $ecology,
      'distribution' => $distribution,
      'conservation' => $conservation,
      // taxon_discussion
      'taxon_name_species' => $taxon_name_species,
      // type_species_treatment, synonyms
       * */
    )
  );
  return $systematics;
}

/**
 * The materials
 */
function _pensoft_publication_xml_systematics_materials_schema(){
  $type_template = array(
    '#field' => 'field_publication_treat_holotype',
    '#value' => '',
    '#child_entity' => '_publication_xml_load_entity',
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
        'value' => array(
          '#field' => 'field_collector',
          '#merge' => '_publication_xml_merge_user'
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
        '#entity' => '_publication_xml_load_location',
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
          '#process' => array(
            '_publication_xml_process_country',
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
          '#process' => array(
            '_publication_xml_process_map',
            'latitude'
          )
        )
      ),
      'verbatimlongitude' => array(
        '#condition' => 'field_map',
        'value' => array(
          '#field' => 'field_map',
          '#process' => array(
            '_publication_xml_process_map',
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
          '#process' => array(
            '_publication_xml_process_map',
            'latitude'
          )
        )
      ),
      'decimallongitude' => array(
        '#condition' => 'field_map',
        'value' => array(
          '#field' => 'field_map',
          '#process' => array(
            '_publication_xml_process_map',
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
        '#field' => 'field_identified_by',
        '#value' => '',
        '#child_entity' => array(
          '_publication_xml_load_entity',
          'user'
        ),
        'value' => array(
          '#field' => 'field_user_family_name',
          '#process' => '_publication_xml_process_full_name'
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
  $paratypes['fields']['type_status']['value']['#value'] = 'Paratype';
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
          '#raw' => TRUE
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
          '#field' => 'field_pensoft_acknowledgments',
          '#raw' => TRUE
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
      '#attributes' => array(
        'id' => function($context) {
          return $context->raw_value['nid'];
        }
      ),
      '#field' => 'field_publication_references',
      '#value' => '',
      '#child_entity' => '_publication_xml_load_entity',
      'fields' => array(
        'reference_type' => array(
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'type'
            )
          )
        ),
        'year_of_publication' => array(
          '#condition' => 'biblio_year',
          'value' => array(
            '#field' => 'biblio_year',
          ),
        ),
        'book_title' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'book_title'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'book_title'
            )
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
          '#condition' => array(
            '_publication_xml_process_biblio',
            'city'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'city'
            )
          )
        ),
        'number_of_pages' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'number_of_pages'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'number_of_pages'
            )
          )
        ),
        // publication_language
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
          '#condition' => array(
            '_publication_xml_process_biblio',
            'chapter_title'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'chapter_title'
            )
          )
        ),
        'article_title' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'article_title'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'article_title'
            )
          )
        ),
        'journal_name' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'journal_name'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'journal_name'
            )
          )
        ),
        'issue' => array(
          '#condition' => 'biblio_issue',
          'value' => array(
            '#field' => 'biblio_issue',
          )
        ),
        'first_page' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'first_page'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'first_page'
            )
          )
        ),
        'last_page' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'last_page'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'last_page'
            )
          )
        ),
        'title' => array(
          'value' => array(
            '#field' => 'title',
          )
        ),
        'conference_name' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'conference_name'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'conference_name'
            )
          )
        ),
        'conference_location' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'conference_location'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'conference_location'
            )
          )
        ),
        //'conference_date' => array(
        //),
        'journal_volume' => array(
          '#condition' => array(
            '_publication_xml_process_biblio',
            'journal_volume'
          ),
          'value' => array(
            '#field' => 'nid',
            '#process' => array(
              '_publication_xml_process_biblio',
              'journal_volume'
            )
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
        '#value' => '',
        '#child_entity' => '_pensoft_publication_xml_load_biblio_author',
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
        '#value' => '',
        '#child_entity' => '_pensoft_publication_xml_load_biblio_editor',
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
 * XXX we don't supply file URL yet !
 */
function _pensoft_publication_xml_files_schema(){
  $f = array(
    'supplementary_file' => array(
      'fields' => array(
        '#field' => 'field_pensoft_files',
        '#value' => '',
        '#child_entity' => '_publication_xml_load_file',
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
        //  '#field' => ''
        //)
      )
    )
  );
  return $f;
}

/**
 * The figures section
 * 
 * 
 */
function _pensoft_publication_xml_figures_schema(){
  $f = array(
    'figure' => array(
      '#field' => 'field_publication_figures',
      '#value' => '',
      '#child_entity' => array(
        '_publication_xml_load_entity',
        'field_collection_item'
      ),
      '#attributes' => array(
        'id' => function($context) {
           $collection = reset(entity_load('field_collection_item', array($context->raw_value['value'])));
           $wrapper = entity_metadata_wrapper('field_collection_item', $collection);
           return $wrapper->field_publication_figure_figure->file->fid->value();
       }
      ),
      'caption' => array(
        '#field' => 'field_publication_figure_title'
      ),
      'url' => array(
        '#field' => 'field_publication_figure_figure',
        '#process' => '_publication_xml_process_get_file_url'
        //'#attributes' => array(
        //  'id' => ''
        //)
      ),
      'photo_description' => array(
        '#condition' => 'field_publication_figure_desc',
        'value' => array(
          '#field' => 'field_publication_figure_desc',
          '#raw' => TRUE
        )
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
      '#child_entity' => array(
        '_publication_xml_load_entity',
        'field_collection_item'
      ),
      '#attributes' => array(
        'id' => function($context) {
           $collection = reset(entity_load('field_collection_item', array($context->raw_value['value'])));
           $wrapper = entity_metadata_wrapper('field_collection_item', $collection);
           return $wrapper->field_publication_table_id->value();
        }
      ),
      'caption' => array(
          '#field' => 'field_publication_table_title'
      ),
      'content' => array(
        'value' => array(
          '#field' => 'field_publication_table',
          '#raw' => TRUE
        )
      )
    )
  );
  return $f;
}
