<?php

/**
 * Implementation of hook_views_default_views().
 */
function darwincore_views_default_views() {
  $views = array();

  // Exported view: specimens_table
  $view = new view;
  $view->name = 'specimens_table';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Specimens table';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

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
    'field_type_status' => 'field_type_status',
  );
  $handler->display->display_options['style_options']['default'] = 'field_date_collected';
  $handler->display->display_options['style_options']['info'] = array(
    'field_basis_of_record' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_catalogue_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_collection_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_collector' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_collector_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_count' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_date_collected' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
    ),
    'field_date_identified' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_notes' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_genbank_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_identification_qualifier' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_identified_by' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_institution_code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_lifestage' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_location' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_other_catalogue_numbers' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_remarks' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_sex' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_taxonomic_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_type_status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
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
    'prefix_suffix' => 1,
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
    'prefix_suffix' => 1,
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
    'prefix_suffix' => 1,
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
    'prefix_suffix' => 1,
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
    'prefix_suffix' => 1,
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
    'specimen_observation' => 'specimen_observation',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'specimens';
  $translatables['specimens_table'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
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
    t('Page'),
  );

  $views[$view->name] = $view;

  return $views;
}
