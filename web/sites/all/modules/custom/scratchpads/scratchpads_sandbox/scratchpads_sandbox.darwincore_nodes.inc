<?php
$location_node = (object)array(
  'uid' => '2',
  'title' => 'Paris, France - 1969-07-21',
  'log' => '',
  'status' => '1',
  'comment' => '1',
  'promote' => '0',
  'sticky' => '0',
  'type' => 'location',
  'language' => 'und',
  'tnid' => '0',
  'translate' => '0',
  'revision_uid' => '2',
  'field_continent_or_ocean' => array(
    'und' => array(
      0 => array(
        'value' => 'Europe'
      )
    )
  ),
  'field_coordinate_system' => array(),
  'field_coordinate_uncertainty' => array(),
  'field_country' => array(
    'und' => array(
      0 => array(
        'iso2' => 'FR'
      )
    )
  ),
  'field_county' => array(),
  'field_geodetic_datum' => array(),
  'field_georeference_protocol' => array(),
  'field_georeference_remarks' => array(),
  'field_island_group' => array(),
  'field_island' => array(),
  'field_locality' => array(
    'und' => array(
      0 => array(
        'value' => 'Down an alley.',
        'format' => NULL,
        'safe_value' => 'Down an alley.'
      )
    )
  ),
  'field_map' => array(
    'und' => array(
      0 => array(
        'latitude' => '48.80686346108517',
        'longitude' => '2.4609375'
      )
    )
  ),
  'field_max_depth' => array(),
  'field_max_elevation' => array(),
  'field_min_depth' => array(),
  'field_min_elevation' => array(),
  'field_state_province' => array(),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document'
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title'
      )
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded'
      )
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator'
      ),
      'type' => 'rel'
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name'
      )
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies'
      ),
      'datatype' => 'xsd:integer'
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    )
  ),
  'cid' => '0',
  'comment_count' => '0',
  'name' => 'username',
  'picture' => '0',
  'data' => 'a:1:{s:7:"contact";i:1;}'
);
$specimen_node = (object)array(
  'uid' => '2',
  'title' => 'MADE-UP-INSTITUTION - MADE-UP-COLLECTION - 12345678',
  'log' => '',
  'status' => '1',
  'comment' => '1',
  'promote' => '0',
  'sticky' => '0',
  'type' => 'specimen_observation',
  'language' => 'und',
  'tnid' => '0',
  'translate' => '0',
  'field_media' => array(),
  'field_basis_of_record' => array(
    'und' => array(
      0 => array(
        'value' => 'HumanObservation'
      )
    )
  ),
  'field_catalogue_number' => array(
    'und' => array(
      0 => array(
        'value' => '12345678',
        'format' => NULL,
        'safe_value' => '12345678'
      )
    )
  ),
  'field_collection_code' => array(
    'und' => array(
      0 => array(
        'value' => 'MADE-UP-COLLECTION',
        'format' => NULL,
        'safe_value' => 'MADE-UP-COLLECTION'
      )
    )
  ),
  'field_collector' => array(),
  'field_collector_number' => array(
    'und' => array(
      0 => array(
        'value' => '124124',
        'format' => NULL,
        'safe_value' => '124124'
      )
    )
  ),
  'field_count' => array(
    'und' => array(
      0 => array(
        'value' => '1'
      )
    )
  ),
  'field_date_collected' => array(
    'und' => array(
      0 => array(
        'value' => '1969-07-21T00:00:00',
        'value2' => '1969-07-21T00:00:00',
        'timezone' => 'UTC',
        'timezone_db' => 'UTC',
        'date_type' => 'date'
      )
    )
  ),
  'field_date_identified' => array(
    'und' => array(
      0 => array(
        'value' => '1969-07-21T00:00:00',
        'value2' => '1969-07-21T00:00:00',
        'timezone' => 'UTC',
        'timezone_db' => 'UTC',
        'date_type' => 'date'
      )
    )
  ),
  'field_genbank_number' => array(),
  'field_identification_qualifier' => array(),
  'field_identified_by' => array(),
  'field_institution_code' => array(
    'und' => array(
      0 => array(
        'value' => 'MADE-UP-INSTITUTION',
        'format' => NULL,
        'safe_value' => 'MADE-UP-INSTITUTION'
      )
    )
  ),
  'field_lifestage' => array(),
  'field_location' => array(
    'und' => array(
      0 => array(
        'nid' => '104'
      )
    )
  ),
  'field_notes' => array(),
  'field_number' => array(
    'und' => array(
      0 => array(
        'value' => '123123',
        'format' => NULL,
        'safe_value' => '123123'
      )
    )
  ),
  'field_other_catalogue_numbers' => array(),
  'field_remarks' => array(
    'und' => array(
      0 => array(
        'value' => 'Itchy little beast.',
        'format' => 'filtered_html',
        'safe_value' => 'Itchy little beast.'
      )
    )
  ),
  'field_sex' => array(
    'und' => array(
      0 => array(
        'value' => 'Unknown'
      )
    )
  ),
  'field_taxonomic_name' => array(
    'und' => array(
      0 => array(
        'tid' => '499'
      )
    )
  ),
  'field_type_status' => array(),
  'rdf_mapping' => array(
    'rdftype' => array(
      0 => 'sioc:Item',
      1 => 'foaf:Document'
    ),
    'title' => array(
      'predicates' => array(
        0 => 'dc:title'
      )
    ),
    'created' => array(
      'predicates' => array(
        0 => 'dc:date',
        1 => 'dc:created'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    ),
    'changed' => array(
      'predicates' => array(
        0 => 'dc:modified'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    ),
    'body' => array(
      'predicates' => array(
        0 => 'content:encoded'
      )
    ),
    'uid' => array(
      'predicates' => array(
        0 => 'sioc:has_creator'
      ),
      'type' => 'rel'
    ),
    'name' => array(
      'predicates' => array(
        0 => 'foaf:name'
      )
    ),
    'comment_count' => array(
      'predicates' => array(
        0 => 'sioc:num_replies'
      ),
      'datatype' => 'xsd:integer'
    ),
    'last_activity' => array(
      'predicates' => array(
        0 => 'sioc:last_activity_date'
      ),
      'datatype' => 'xsd:dateTime',
      'callback' => 'date_iso8601'
    )
  ),
  'cid' => '0',
  'comment_count' => '0',
  'name' => 'username',
  'picture' => '0',
  'data' => 'a:1:{s:7:"contact";i:1;}'
);