<?php

/**
 * Implements hook_dwcarchiver_archives
 */
function dwcarchiver_dwcarchiver_archives(){
  // Get all of the "taxonomy" vocabularies, and create an archive for each.
  // Note, we may want to be able to allow multiple vocabularies for a single
  // archive.
  $bio_vids = variable_get('biological_vids');
  $archives = array();
  foreach($bio_vids as $vid => $type){
    if($type){
      $vocabulary = taxonomy_vocabulary_load($vid);
      $archives[] = array(
        'name' => 'GBIF DwCA',
        'machine_name' => 'gbif_dwca',
        'entity' => 'taxonomy_term',
        'bundle' => $vocabulary->machine_name,
        'rowtype' => 'http://rs.tdwg.org/dwc/terms/Taxon',
        'extensions' => array(
          array(
            'name' => 'Specimen',
            'machine_name' => 'specimen',
            'rowtype' => 'http://rs.gbif.org/terms/1.0/TypesAndSpecimen',
            'entity' => 'node',
            'bundle' => 'specimen_observation',
            'field_name' => 'field_taxonomic_name',
            'maps' => array(
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_type_status',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/typeStatus',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_institution_code',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/institutionCode',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_location:field_map',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/decimalLongitude',
                  'required' => '0',
                  'processor' => 'gm3_longitude'
                ),
                array(
                  'field' => 'field_location:field_map',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/decimalLatitude',
                  'required' => '0',
                  'processor' => 'gm3_latitude'
                ),
                array(
                  'field' => 'field_collection_code',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/collectionCode',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_catalogue_number',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/catalogNumber',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                )
              )
            )
          ),
          array(
            'name' => 'Reference',
            'machine_name' => 'reference',
            'rowtype' => 'http://rs.gbif.org/terms/1.0/Reference',
            'entity' => 'node',
            'bundle' => 'biblio',
            'field_name' => 'field_taxonomic_name',
            'maps' => array(
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'biblio_issn',
                  'uri' => 'http://purl.org/ontology/bibo/issn',
                  'required' => '0',
                  'processor' => 'dwcarchiver_biblio'
                ),
                array(
                  'field' => 'biblio_isbn',
                  'uri' => 'http://purl.org/ontology/bibo/isbn',
                  'required' => '0',
                  'processor' => 'dwcarchiver_biblio'
                ),
                array(
                  'field' => 'biblio_doi',
                  'uri' => 'http://purl.org/ontology/bibo/doi',
                  'required' => '0',
                  'processor' => 'dwcarchiver_biblio'
                ),
                array(
                  'field' => 'biblio_type',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_biblio'
                ),
                array(
                  'field' => 'title',
                  'uri' => 'http://purl.org/dc/terms/title',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'changed',
                  'uri' => 'http://purl.org/dc/terms/modified',
                  'required' => '0',
                  'processor' => 'dwcarchiver_timestamp'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'biblio_date',
                  'uri' => 'http://purl.org/dc/terms/date',
                  'required' => '0',
                  'processor' => 'dwcarchiver_biblio'
                ),
                array(
                  'field' => 'biblio_authors_field',
                  'uri' => 'http://purl.org/dc/terms/creator',
                  'required' => '0',
                  'processor' => 'dwcarchiver_biblio'
                ),
                array(
                  'field' => 'created',
                  'uri' => 'http://purl.org/dc/terms/created',
                  'required' => '0',
                  'processor' => 'dwcarchiver_timestamp'
                )
              )
            )
          ),
          array(
            'name' => 'Distribution',
            'machine_name' => 'distribution',
            'rowtype' => 'http://rs.gbif.org/terms/1.0/Distribution',
            'entity' => 'node',
            'bundle' => 'specimen_observation',
            'field_name' => 'field_taxonomic_name',
            'maps' => array(
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://rs.tdwg.org/dwc/terms/locationID',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                )
              )
            )
          ),
          array(
            'name' => 'Image',
            'machine_name' => 'image',
            'rowtype' => 'http://rs.gbif.org/terms/1.0/Image',
            'entity' => 'file',
            'bundle' => 'image',
            'field_name' => 'field_taxonomic_name',
            'maps' => array(
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'file_url',
                  'uri' => 'http://rs.tdwg.org/ac/terms/accessURI',
                  'required' => '0',
                  'processor' => 'dwcarchiver_file_url'
                ),
                array(
                  'field' => 'filename',
                  'uri' => 'http://purl.org/dc/terms/title',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'filemime',
                  'uri' => 'http://purl.org/dc/terms/format',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_cc_licence',
                  'uri' => 'http://ns.adobe.com/xap/1.0/rights/UsageTerms',
                  'required' => '0',
                  'processor' => 'dwcarchiver_creative_commons_url'
                )
              )
            )
          ),
          array(
            'name' => 'Description',
            'machine_name' => 'description',
            'rowtype' => 'http://rs.gbif.org/terms/1.0/Description',
            'entity' => 'node',
            'bundle' => 'spm',
            'field_name' => 'field_taxonomic_name',
            'maps' => array(
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_0',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value',
                  'value' => 'general description'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/source',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_1',
                  'uri' => 'http://purl.org/dc/terms/license',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_general_description',
                  'uri' => 'http://purl.org/dc/terms/description',
                  'required' => '1',
                  'processor' => 'dwcarchiver_raw_text'
                )
              ),
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_0',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/source',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_1',
                  'uri' => 'http://purl.org/dc/terms/license',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_biology',
                  'uri' => 'http://purl.org/dc/terms/description',
                  'required' => '1',
                  'processor' => 'dwcarchiver_raw_text'
                )
              ),
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_0',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/source',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_1',
                  'uri' => 'http://purl.org/dc/terms/license',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_conservation_status',
                  'uri' => 'http://purl.org/dc/terms/description',
                  'required' => '1',
                  'processor' => 'dwcarchiver_raw_text'
                )
              ),
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_0',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/source',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_1',
                  'uri' => 'http://purl.org/dc/terms/license',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_distribution',
                  'uri' => 'http://purl.org/dc/terms/description',
                  'required' => '1',
                  'processor' => 'dwcarchiver_raw_text'
                )
              ),
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_0',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/source',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_1',
                  'uri' => 'http://purl.org/dc/terms/license',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_genetics',
                  'uri' => 'http://purl.org/dc/terms/description',
                  'required' => '1',
                  'processor' => 'dwcarchiver_raw_text'
                )
              ),
              array(
                array(
                  'field' => 'uuid',
                  'uri' => 'coreid',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_0',
                  'uri' => 'http://purl.org/dc/terms/type',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_url',
                  'uri' => 'http://purl.org/dc/terms/source',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_url'
                ),
                array(
                  'field' => 'dwcarchiver_text_entry_1',
                  'uri' => 'http://purl.org/dc/terms/license',
                  'required' => '0',
                  'processor' => 'dwcarchiver_value'
                ),
                array(
                  'field' => 'content_language',
                  'uri' => 'http://purl.org/dc/terms/language',
                  'required' => '0',
                  'processor' => 'dwcarchiver_content_language'
                ),
                array(
                  'field' => 'uuid',
                  'uri' => 'http://purl.org/dc/terms/identifier',
                  'required' => '0',
                  'processor' => 'dwcarchiver_raw_text'
                ),
                array(
                  'field' => 'field_uses',
                  'uri' => 'http://purl.org/dc/terms/description',
                  'required' => '1',
                  'processor' => 'dwcarchiver_raw_text'
                )
              )
            )
          )
        ),
        'maps' => array(
          array(
            array(
              'field' => 'uuid',
              'uri' => 'coreid',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'field_rank',
              'uri' => 'http://rs.tdwg.org/dwc/terms/taxonRank',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'field_unacceptability_reason',
              'uri' => 'http://rs.tdwg.org/dwc/terms/taxonomicStatus',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'field_authors',
              'uri' => 'http://rs.tdwg.org/dwc/terms/scientificNameAuthorship',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'name',
              'uri' => 'http://rs.tdwg.org/dwc/terms/scientificName',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'parent:uuid',
              'uri' => 'http://rs.tdwg.org/dwc/terms/parentNameUsageID',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'field_unacceptability_reason',
              'uri' => 'http://rs.tdwg.org/dwc/terms/nomenclaturalStatus',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'field_reference:content_url',
              'uri' => 'http://rs.tdwg.org/dwc/terms/namePublishedInID',
              'required' => '0',
              'processor' => 'dwcarchiver_content_url'
            ),
            array(
              'field' => 'field_reference:title',
              'uri' => 'http://rs.tdwg.org/dwc/terms/namePublishedIn',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            ),
            array(
              'field' => 'field_aan_5:uuid',
              'uri' => 'http://rs.tdwg.org/dwc/terms/acceptedNameUsageID',
              'required' => '0',
              'processor' => 'dwcarchiver_raw_text'
            )
          )
        )
      );
    }
  }
  return $archives;
}