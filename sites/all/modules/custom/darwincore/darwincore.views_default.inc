<?php

/**
 * Implementation of hook_views_default_views().
 */
function darwincore_views_default_views(){
  $views = array();
  // Exported view: specimens_table
  $view = new view();
  $view->name = 'specimens_table';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Specimens table';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE;
  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_basis_of_record' => 'field_basis_of_record',
    'field_catalogue_number' => 'field_catalogue_number',
    'field_collection_code' => 'field_collection_code',
    'field_collector' => 'field_collector',
    'field_collector_number' => 'field_collector_number',
    'field_count' => 'field_count',
    'field_date_collected' => 'field_date_collected',
    'field_date_identified' => 'field_date_identified',
    'field_notes' => 'field_notes',
    'field_number' => 'field_number',
    'field_genbank_number' => 'field_genbank_number',
    'field_identification_qualifier' => 'field_identification_qualifier',
    'field_identified_by' => 'field_identified_by',
    'field_institution_code' => 'field_institution_code',
    'field_lifestage' => 'field_lifestage',
    'field_location' => 'field_location',
    'field_other_catalogue_numbers' => 'field_other_catalogue_numbers',
    'field_remarks' => 'field_remarks',
    'field_sex' => 'field_sex',
    'field_taxonomic_name' => 'field_taxonomic_name',
    'field_type_status' => 'field_type_status'
  );
  $handler->display->display_options['style_options']['default'] = 'field_date_collected';
  $handler->display->display_options['style_options']['info'] = array(
    'field_basis_of_record' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_catalogue_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_collection_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_collector' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_collector_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_count' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_date_collected' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => ''
    ),
    'field_date_identified' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_notes' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_genbank_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_identification_qualifier' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_identified_by' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_institution_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_lifestage' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_location' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_other_catalogue_numbers' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_remarks' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_sex' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_taxonomic_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    ),
    'field_type_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ''
    )
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 1;
  /* Relationship: Specimen/Location */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['ui_name'] = 'Specimen/Location';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = 0;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '-1';
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Specimen ID';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Fields: Basis of record */
  $handler->display->display_options['fields']['field_basis_of_record']['id'] = 'field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['table'] = 'field_data_field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['field'] = 'field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_basis_of_record']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_basis_of_record']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_basis_of_record']['field_api_classes'] = 0;
  /* Field: Fields: Catalogue number */
  $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
  /* Field: Fields: Collection code */
  $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
  /* Field: Fields: Collector */
  $handler->display->display_options['fields']['field_collector']['id'] = 'field_collector';
  $handler->display->display_options['fields']['field_collector']['table'] = 'field_data_field_collector';
  $handler->display->display_options['fields']['field_collector']['field'] = 'field_collector';
  /* Field: Fields: Collector number */
  $handler->display->display_options['fields']['field_collector_number']['id'] = 'field_collector_number';
  $handler->display->display_options['fields']['field_collector_number']['table'] = 'field_data_field_collector_number';
  $handler->display->display_options['fields']['field_collector_number']['field'] = 'field_collector_number';
  /* Field: Fields: Count */
  $handler->display->display_options['fields']['field_count']['id'] = 'field_count';
  $handler->display->display_options['fields']['field_count']['table'] = 'field_data_field_count';
  $handler->display->display_options['fields']['field_count']['field'] = 'field_count';
  /* Field: Fields: Date collected */
  $handler->display->display_options['fields']['field_date_collected']['id'] = 'field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['table'] = 'field_data_field_date_collected';
  $handler->display->display_options['fields']['field_date_collected']['field'] = 'field_date_collected';
  /* Field: Fields: Date identified */
  $handler->display->display_options['fields']['field_date_identified']['id'] = 'field_date_identified';
  $handler->display->display_options['fields']['field_date_identified']['table'] = 'field_data_field_date_identified';
  $handler->display->display_options['fields']['field_date_identified']['field'] = 'field_date_identified';
  /* Field: Fields: Field notes */
  $handler->display->display_options['fields']['field_notes']['id'] = 'field_notes';
  $handler->display->display_options['fields']['field_notes']['table'] = 'field_data_field_notes';
  $handler->display->display_options['fields']['field_notes']['field'] = 'field_notes';
  /* Field: Fields: Field number */
  $handler->display->display_options['fields']['field_number']['id'] = 'field_number';
  $handler->display->display_options['fields']['field_number']['table'] = 'field_data_field_number';
  $handler->display->display_options['fields']['field_number']['field'] = 'field_number';
  /* Field: Fields: GenBank number */
  $handler->display->display_options['fields']['field_genbank_number']['id'] = 'field_genbank_number';
  $handler->display->display_options['fields']['field_genbank_number']['table'] = 'field_data_field_genbank_number';
  $handler->display->display_options['fields']['field_genbank_number']['field'] = 'field_genbank_number';
  $handler->display->display_options['fields']['field_genbank_number']['type'] = 'darwincore_link_to_ncbi';
  /* Field: Fields: Identification qualifier */
  $handler->display->display_options['fields']['field_identification_qualifier']['id'] = 'field_identification_qualifier';
  $handler->display->display_options['fields']['field_identification_qualifier']['table'] = 'field_data_field_identification_qualifier';
  $handler->display->display_options['fields']['field_identification_qualifier']['field'] = 'field_identification_qualifier';
  /* Field: Fields: Identified by */
  $handler->display->display_options['fields']['field_identified_by']['id'] = 'field_identified_by';
  $handler->display->display_options['fields']['field_identified_by']['table'] = 'field_data_field_identified_by';
  $handler->display->display_options['fields']['field_identified_by']['field'] = 'field_identified_by';
  /* Field: Fields: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  /* Field: Fields: Lifestage */
  $handler->display->display_options['fields']['field_lifestage']['id'] = 'field_lifestage';
  $handler->display->display_options['fields']['field_lifestage']['table'] = 'field_data_field_lifestage';
  $handler->display->display_options['fields']['field_lifestage']['field'] = 'field_lifestage';
  /* Field: Fields: Other catalogue numbers */
  $handler->display->display_options['fields']['field_other_catalogue_numbers']['id'] = 'field_other_catalogue_numbers';
  $handler->display->display_options['fields']['field_other_catalogue_numbers']['table'] = 'field_data_field_other_catalogue_numbers';
  $handler->display->display_options['fields']['field_other_catalogue_numbers']['field'] = 'field_other_catalogue_numbers';
  /* Field: Fields: Remarks */
  $handler->display->display_options['fields']['field_remarks']['id'] = 'field_remarks';
  $handler->display->display_options['fields']['field_remarks']['table'] = 'field_data_field_remarks';
  $handler->display->display_options['fields']['field_remarks']['field'] = 'field_remarks';
  /* Field: Fields: Sex */
  $handler->display->display_options['fields']['field_sex']['id'] = 'field_sex';
  $handler->display->display_options['fields']['field_sex']['table'] = 'field_data_field_sex';
  $handler->display->display_options['fields']['field_sex']['field'] = 'field_sex';
  /* Field: Fields: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  /* Field: Fields: Type status */
  $handler->display->display_options['fields']['field_type_status']['id'] = 'field_type_status';
  $handler->display->display_options['fields']['field_type_status']['table'] = 'field_data_field_type_status';
  $handler->display->display_options['fields']['field_type_status']['field'] = 'field_type_status';
  /* Field: Fields: Location */
  $handler->display->display_options['fields']['field_location']['id'] = 'field_location';
  $handler->display->display_options['fields']['field_location']['table'] = 'field_data_field_location';
  $handler->display->display_options['fields']['field_location']['field'] = 'field_location';
  /* Field: Fields: Continent/Ocean */
  $handler->display->display_options['fields']['field_continent_or_ocean']['id'] = 'field_continent_or_ocean';
  $handler->display->display_options['fields']['field_continent_or_ocean']['table'] = 'field_data_field_continent_or_ocean';
  $handler->display->display_options['fields']['field_continent_or_ocean']['field'] = 'field_continent_or_ocean';
  $handler->display->display_options['fields']['field_continent_or_ocean']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_continent_or_ocean']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_continent_or_ocean']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_continent_or_ocean']['field_api_classes'] = 0;
  /* Field: Fields: Coordinate uncertainty in meters */
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['id'] = 'field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['table'] = 'field_data_field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['field'] = 'field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['field_api_classes'] = 0;
  /* Field: Fields: County */
  $handler->display->display_options['fields']['field_county']['id'] = 'field_county';
  $handler->display->display_options['fields']['field_county']['table'] = 'field_data_field_county';
  $handler->display->display_options['fields']['field_county']['field'] = 'field_county';
  $handler->display->display_options['fields']['field_county']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_county']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_county']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_county']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_county']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_county']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_county']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_county']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_county']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_county']['field_api_classes'] = 0;
  /* Field: Fields: Geodetic datum */
  $handler->display->display_options['fields']['field_geodetic_datum']['id'] = 'field_geodetic_datum';
  $handler->display->display_options['fields']['field_geodetic_datum']['table'] = 'field_data_field_geodetic_datum';
  $handler->display->display_options['fields']['field_geodetic_datum']['field'] = 'field_geodetic_datum';
  $handler->display->display_options['fields']['field_geodetic_datum']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_geodetic_datum']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_geodetic_datum']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_geodetic_datum']['field_api_classes'] = 0;
  /* Field: Fields: Georeference protocol */
  $handler->display->display_options['fields']['field_georeference_protocol']['id'] = 'field_georeference_protocol';
  $handler->display->display_options['fields']['field_georeference_protocol']['table'] = 'field_data_field_georeference_protocol';
  $handler->display->display_options['fields']['field_georeference_protocol']['field'] = 'field_georeference_protocol';
  $handler->display->display_options['fields']['field_georeference_protocol']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_georeference_protocol']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_georeference_protocol']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_georeference_protocol']['field_api_classes'] = 0;
  /* Field: Fields: Georeference remarks */
  $handler->display->display_options['fields']['field_georeference_remarks']['id'] = 'field_georeference_remarks';
  $handler->display->display_options['fields']['field_georeference_remarks']['table'] = 'field_data_field_georeference_remarks';
  $handler->display->display_options['fields']['field_georeference_remarks']['field'] = 'field_georeference_remarks';
  $handler->display->display_options['fields']['field_georeference_remarks']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_georeference_remarks']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_georeference_remarks']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_georeference_remarks']['field_api_classes'] = 0;
  /* Field: Fields: Island */
  $handler->display->display_options['fields']['field_island']['id'] = 'field_island';
  $handler->display->display_options['fields']['field_island']['table'] = 'field_data_field_island';
  $handler->display->display_options['fields']['field_island']['field'] = 'field_island';
  $handler->display->display_options['fields']['field_island']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_island']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_island']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_island']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_island']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_island']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_island']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_island']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_island']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_island']['field_api_classes'] = 0;
  /* Field: Fields: Locality */
  $handler->display->display_options['fields']['field_locality']['id'] = 'field_locality';
  $handler->display->display_options['fields']['field_locality']['table'] = 'field_data_field_locality';
  $handler->display->display_options['fields']['field_locality']['field'] = 'field_locality';
  $handler->display->display_options['fields']['field_locality']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_locality']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_locality']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_locality']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_locality']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_locality']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_locality']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_locality']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_locality']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_locality']['field_api_classes'] = 0;
  /* Field: Fields: Maximum depth */
  $handler->display->display_options['fields']['field_max_depth']['id'] = 'field_max_depth';
  $handler->display->display_options['fields']['field_max_depth']['table'] = 'field_data_field_max_depth';
  $handler->display->display_options['fields']['field_max_depth']['field'] = 'field_max_depth';
  $handler->display->display_options['fields']['field_max_depth']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_max_depth']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_max_depth']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_max_depth']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_max_depth']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_max_depth']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_max_depth']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  $handler->display->display_options['fields']['field_max_depth']['field_api_classes'] = 0;
  /* Field: Fields: Maximum elevation */
  $handler->display->display_options['fields']['field_max_elevation']['id'] = 'field_max_elevation';
  $handler->display->display_options['fields']['field_max_elevation']['table'] = 'field_data_field_max_elevation';
  $handler->display->display_options['fields']['field_max_elevation']['field'] = 'field_max_elevation';
  $handler->display->display_options['fields']['field_max_elevation']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_max_elevation']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_max_elevation']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_max_elevation']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_max_elevation']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  $handler->display->display_options['fields']['field_max_elevation']['field_api_classes'] = 0;
  /* Field: Fields: Minimum depth */
  $handler->display->display_options['fields']['field_min_depth']['id'] = 'field_min_depth';
  $handler->display->display_options['fields']['field_min_depth']['table'] = 'field_data_field_min_depth';
  $handler->display->display_options['fields']['field_min_depth']['field'] = 'field_min_depth';
  $handler->display->display_options['fields']['field_min_depth']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_min_depth']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_min_depth']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_min_depth']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_min_depth']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_min_depth']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_min_depth']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  $handler->display->display_options['fields']['field_min_depth']['field_api_classes'] = 0;
  /* Field: Fields: Minimum elevation */
  $handler->display->display_options['fields']['field_min_elevation']['id'] = 'field_min_elevation';
  $handler->display->display_options['fields']['field_min_elevation']['table'] = 'field_data_field_min_elevation';
  $handler->display->display_options['fields']['field_min_elevation']['field'] = 'field_min_elevation';
  $handler->display->display_options['fields']['field_min_elevation']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_min_elevation']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_min_elevation']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_min_elevation']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_min_elevation']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  $handler->display->display_options['fields']['field_min_elevation']['field_api_classes'] = 0;
  /* Field: Fields: State/Province */
  $handler->display->display_options['fields']['field_state_province']['id'] = 'field_state_province';
  $handler->display->display_options['fields']['field_state_province']['table'] = 'field_data_field_state_province';
  $handler->display->display_options['fields']['field_state_province']['field'] = 'field_state_province';
  $handler->display->display_options['fields']['field_state_province']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_state_province']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_state_province']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_state_province']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_state_province']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_state_province']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_state_province']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_state_province']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_state_province']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_state_province']['field_api_classes'] = 0;
  /* Field: Fields: Verbatim Coordinate System */
  $handler->display->display_options['fields']['field_coordinate_system']['id'] = 'field_coordinate_system';
  $handler->display->display_options['fields']['field_coordinate_system']['table'] = 'field_data_field_coordinate_system';
  $handler->display->display_options['fields']['field_coordinate_system']['field'] = 'field_coordinate_system';
  $handler->display->display_options['fields']['field_coordinate_system']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_coordinate_system']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_coordinate_system']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_coordinate_system']['field_api_classes'] = 0;
  /* Filter: Node: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'specimens';
  $translatables['specimens_table'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('field_location'),
    t('Specimen ID'),
    t('Basis of record'),
    t('Catalogue number'),
    t('Collection code'),
    t('Collector'),
    t('Collector number'),
    t('Count'),
    t('Date collected'),
    t('Date identified'),
    t('Field notes'),
    t('Field number'),
    t('GenBank number'),
    t('Identification qualifier'),
    t('Identified by'),
    t('Institution code'),
    t('Lifestage'),
    t('Other catalogue numbers'),
    t('Remarks'),
    t('Sex'),
    t('Taxonomic name'),
    t('Type status'),
    t('Location'),
    t('Continent/Ocean'),
    t('Coordinate uncertainty in meters'),
    t('County'),
    t('Geodetic datum'),
    t('Georeference protocol'),
    t('Georeference remarks'),
    t('Island'),
    t('Locality'),
    t('Maximum depth'),
    t('Maximum elevation'),
    t('Minimum depth'),
    t('Minimum elevation'),
    t('State/Province'),
    t('Verbatim Coordinate System'),
    t('Page')
  );
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'users_references';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Users references';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Family name */
  $handler->display->display_options['fields']['field_user_family_name']['id'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['table'] = 'field_data_field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['field'] = 'field_user_family_name';
  $handler->display->display_options['fields']['field_user_family_name']['label'] = '';
  $handler->display->display_options['fields']['field_user_family_name']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['text'] = '[field_user_family_name],';
  $handler->display->display_options['fields']['field_user_family_name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_family_name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['empty'] = 'Unknown,';
  $handler->display->display_options['fields']['field_user_family_name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_family_name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_user_family_name']['field_api_classes'] = 0;
  /* Field: User: Other/given name(s) */
  $handler->display->display_options['fields']['field_user_given_names']['id'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['table'] = 'field_data_field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['field'] = 'field_user_given_names';
  $handler->display->display_options['fields']['field_user_given_names']['label'] = '';
  $handler->display->display_options['fields']['field_user_given_names']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_user_given_names']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['empty'] = 'Unknown';
  $handler->display->display_options['fields']['field_user_given_names']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_user_given_names']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_user_given_names']['field_api_classes'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['name']['alter']['text'] = '([name])';
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Sort criterion: User: Family name (field_user_family_name) */
  $handler->display->display_options['sorts']['field_user_family_name_value']['id'] = 'field_user_family_name_value';
  $handler->display->display_options['sorts']['field_user_family_name_value']['table'] = 'field_data_field_user_family_name';
  $handler->display->display_options['sorts']['field_user_family_name_value']['field'] = 'field_user_family_name_value';
  /* Sort criterion: User: Other/given name(s) (field_user_given_names) */
  $handler->display->display_options['sorts']['field_user_given_names_value']['id'] = 'field_user_given_names_value';
  $handler->display->display_options['sorts']['field_user_given_names_value']['table'] = 'field_data_field_user_given_names';
  $handler->display->display_options['sorts']['field_user_given_names_value']['field'] = 'field_user_given_names_value';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['operator'] = 'not in';
  $handler->display->display_options['filters']['uid']['value'] = array(
    0 => 0,
    1 => '1'
  );
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  /* Display: References */
  $handler = $view->new_display('references', 'References', 'references_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'User references';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'references_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'references_fields';
  $handler->display->display_options['row_options']['separator'] = '';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'ecological_niche_modelling';
  $view->description = '';
  $view->tag = 'no-batch';
  $view->base_table = 'node';
  $view->human_name = 'Ecological Niche Modelling';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '0';
  /* Field: Content: Collection code */
  $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['exclude'] = TRUE;
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['exclude'] = TRUE;
  /* Field: Content: Catalogue number */
  $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'occurrenceID';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[field_institution_code]-[field_collection_code]-[field_catalogue_number]';
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['label'] = 'nameComplete';
  $handler->display->display_options['fields']['field_taxonomic_name']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map_1']['id'] = 'field_map_1';
  $handler->display->display_options['fields']['field_map_1']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map_1']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map_1']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map_1']['label'] = 'decimalLatitude';
  $handler->display->display_options['fields']['field_map_1']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map_1']['type'] = 'gm3_text_lat';
  $handler->display->display_options['fields']['field_map_1']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map']['id'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map']['label'] = 'decimalLongitude';
  $handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['type'] = 'gm3_text_lon';
  $handler->display->display_options['fields']['field_map']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'taxonomy/term/%/enm.csv';
  $views[$view->name] = $view;
  $view = new view();
  $view->name = 'geocat_csv';
  $view->description = '';
  $view->tag = 'no-batch';
  $view->base_table = 'node';
  $view->human_name = 'GeoCAT CSV';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE;
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Location (field_location) */
  $handler->display->display_options['relationships']['field_location_nid']['id'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['table'] = 'field_data_field_location';
  $handler->display->display_options['relationships']['field_location_nid']['field'] = 'field_location_nid';
  $handler->display->display_options['relationships']['field_location_nid']['required'] = TRUE;
  $handler->display->display_options['relationships']['field_location_nid']['delta'] = '0';
  /* Field: Content: Basis of record */
  $handler->display->display_options['fields']['field_basis_of_record']['id'] = 'field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['table'] = 'field_data_field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['field'] = 'field_basis_of_record';
  $handler->display->display_options['fields']['field_basis_of_record']['label'] = 'basisOfRecord ';
  /* Field: Content: Catalogue number */
  $handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
  $handler->display->display_options['fields']['field_catalogue_number']['label'] = 'catalogNumber';
  /* Field: Content revision: Updated date */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'node_revision';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['label'] = 'changed';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'long';
  /* Field: Content: Collection code */
  $handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
  $handler->display->display_options['fields']['field_collection_code']['label'] = 'collectionCode';
  /* Field: Content: Collector */
  $handler->display->display_options['fields']['field_collector']['id'] = 'field_collector';
  $handler->display->display_options['fields']['field_collector']['table'] = 'field_data_field_collector';
  $handler->display->display_options['fields']['field_collector']['field'] = 'field_collector';
  $handler->display->display_options['fields']['field_collector']['label'] = 'collector';
  $handler->display->display_options['fields']['field_collector']['type'] = 'scratchpads_user_full_name';
  $handler->display->display_options['fields']['field_collector']['settings'] = array(
    'show_name_as_link' => 0,
    'separator_option' => '',
    'element_option' => '- None -',
    'element_class' => ''
  );
  $handler->display->display_options['fields']['field_collector']['delta_offset'] = '0';
  /* Field: Content: Coordinate uncertainty in meters */
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['id'] = 'field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['table'] = 'field_data_field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['field'] = 'field_coordinate_uncertainty';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['label'] = 'coordinateuncertaintyinmeters';
  $handler->display->display_options['fields']['field_coordinate_uncertainty']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  /* Field: Content: Country */
  $handler->display->display_options['fields']['field_country']['id'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['table'] = 'field_data_field_country';
  $handler->display->display_options['fields']['field_country']['field'] = 'field_country';
  $handler->display->display_options['fields']['field_country']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_country']['label'] = 'country';
  $handler->display->display_options['fields']['field_country']['type'] = 'country_alpha_3';
  /* Field: Content: County */
  $handler->display->display_options['fields']['field_county']['id'] = 'field_county';
  $handler->display->display_options['fields']['field_county']['table'] = 'field_data_field_county';
  $handler->display->display_options['fields']['field_county']['field'] = 'field_county';
  $handler->display->display_options['fields']['field_county']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_county']['label'] = 'county';
  /* Field: Content: Identified by */
  $handler->display->display_options['fields']['field_identified_by']['id'] = 'field_identified_by';
  $handler->display->display_options['fields']['field_identified_by']['table'] = 'field_data_field_identified_by';
  $handler->display->display_options['fields']['field_identified_by']['field'] = 'field_identified_by';
  $handler->display->display_options['fields']['field_identified_by']['label'] = 'identifiedBy';
  $handler->display->display_options['fields']['field_identified_by']['type'] = 'scratchpads_user_full_name';
  $handler->display->display_options['fields']['field_identified_by']['settings'] = array(
    'show_name_as_link' => 1,
    'separator_option' => '',
    'element_option' => '- None -',
    'element_class' => ''
  );
  /* Field: Content: Institution code */
  $handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
  $handler->display->display_options['fields']['field_institution_code']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_institution_code']['label'] = 'institutionCode';
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map']['id'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map']['label'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map']['type'] = 'gm3_text_lat';
  $handler->display->display_options['fields']['field_map']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Field: Content: Map */
  $handler->display->display_options['fields']['field_map_1']['id'] = 'field_map_1';
  $handler->display->display_options['fields']['field_map_1']['table'] = 'field_data_field_map';
  $handler->display->display_options['fields']['field_map_1']['field'] = 'field_map';
  $handler->display->display_options['fields']['field_map_1']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_map_1']['label'] = 'longitude';
  $handler->display->display_options['fields']['field_map_1']['click_sort_column'] = 'latitude';
  $handler->display->display_options['fields']['field_map_1']['type'] = 'gm3_text_lon';
  $handler->display->display_options['fields']['field_map_1']['settings'] = array(
    'display_convex_hull' => FALSE
  );
  /* Field: Content: Locality */
  $handler->display->display_options['fields']['field_locality']['id'] = 'field_locality';
  $handler->display->display_options['fields']['field_locality']['table'] = 'field_data_field_locality';
  $handler->display->display_options['fields']['field_locality']['field'] = 'field_locality';
  $handler->display->display_options['fields']['field_locality']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_locality']['label'] = 'locality';
  /* Field: Content: Georeference remarks */
  $handler->display->display_options['fields']['field_georeference_remarks']['id'] = 'field_georeference_remarks';
  $handler->display->display_options['fields']['field_georeference_remarks']['table'] = 'field_data_field_georeference_remarks';
  $handler->display->display_options['fields']['field_georeference_remarks']['field'] = 'field_georeference_remarks';
  $handler->display->display_options['fields']['field_georeference_remarks']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_georeference_remarks']['label'] = 'occurrenceRemarks';
  /* Field: Field: Taxonomic name */
  $handler->display->display_options['fields']['field_taxonomic_name']['id'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['table'] = 'field_data_field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['field'] = 'field_taxonomic_name';
  $handler->display->display_options['fields']['field_taxonomic_name']['label'] = 'scientificname ';
  $handler->display->display_options['fields']['field_taxonomic_name']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_taxonomic_name']['delta_offset'] = '0';
  /* Field: Content: State/Province */
  $handler->display->display_options['fields']['field_state_province']['id'] = 'field_state_province';
  $handler->display->display_options['fields']['field_state_province']['table'] = 'field_data_field_state_province';
  $handler->display->display_options['fields']['field_state_province']['field'] = 'field_state_province';
  $handler->display->display_options['fields']['field_state_province']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_state_province']['label'] = 'stateProvince';
  /* Field: Content: Maximum elevation */
  $handler->display->display_options['fields']['field_max_elevation']['id'] = 'field_max_elevation';
  $handler->display->display_options['fields']['field_max_elevation']['table'] = 'field_data_field_max_elevation';
  $handler->display->display_options['fields']['field_max_elevation']['field'] = 'field_max_elevation';
  $handler->display->display_options['fields']['field_max_elevation']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_max_elevation']['label'] = 'maximumElevation';
  $handler->display->display_options['fields']['field_max_elevation']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1
  );
  /* Field: Content: Minimum elevation */
  $handler->display->display_options['fields']['field_min_elevation']['id'] = 'field_min_elevation';
  $handler->display->display_options['fields']['field_min_elevation']['table'] = 'field_data_field_min_elevation';
  $handler->display->display_options['fields']['field_min_elevation']['field'] = 'field_min_elevation';
  $handler->display->display_options['fields']['field_min_elevation']['relationship'] = 'field_location_nid';
  $handler->display->display_options['fields']['field_min_elevation']['label'] = 'minimumElevation';
  $handler->display->display_options['fields']['field_min_elevation']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Has taxonomy term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'taxonomy_index';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'specimen_observation' => 'specimen_observation'
  );
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'taxonomy/term/%/geocat.csv';
  $views[$view->name] = $view;
  return $views;
}
