<?php
/**
 * @file
 * ecoint.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function ecoint_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'revision_id' => '33',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'ectoparasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '1d3098d3-d82a-4f58-8b5c-76661ca91e4c',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002633',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002633',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002632',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002632',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  $terms[] = array(
    'revision_id' => '34',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'intercellular endoparasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '215a14f6-6c86-4f2e-940b-10ff85dac3db',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002639',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002639',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002638',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002638',
        ),
      ),
    ),
    'parent' => array(
      0 => '9c1f2691-8ba5-40bf-9e1b-753020ee7858',
    ),
  );
  $terms[] = array(
    'revision_id' => '35',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'is vector for',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '2262df4b-d2ba-4dec-b35e-fc7ff96076b8',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002460',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002460',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002459',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002459',
        ),
      ),
    ),
  );
  $terms[] = array(
    'revision_id' => '50',
    'log' => NULL,
    'timestamp' => '1465294798',
    'uid' => '1',
    'name' => 'Naturalised',
    'description' => 'The organism reproduces naturally and forms part of the local ecology.',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '24bd8019-c7cd-4580-84d9-0f117c5e6b69',
    'vocabulary_machine_name' => 'presence_status',
  );
  $terms[] = array(
    'revision_id' => '46',
    'log' => NULL,
    'timestamp' => '1465294709',
    'uid' => '1',
    'name' => 'Native',
    'description' => 'The organism either evolved in this region or arrived by non-anthropogenic means.',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '2eed3452-78b1-48ad-8ed7-7417eebc84cd',
    'vocabulary_machine_name' => 'presence_status',
  );
  $terms[] = array(
    'revision_id' => '36',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'parasitoid of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '32c093c1-44cf-49f7-88f2-3ad292511a23',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002209',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002209',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002208',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002208',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  $terms[] = array(
    'revision_id' => '53',
    'log' => NULL,
    'timestamp' => '1465294843',
    'uid' => '1',
    'name' => 'Invasive',
    'description' => 'The organism is having a deleterious impact on another organism,multiple organisms or the ecosystem as a whole.',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '3d630c9d-bd28-40c4-9218-92a928849082',
    'vocabulary_machine_name' => 'presence_status',
  );
  $terms[] = array(
    'revision_id' => '37',
    'log' => '',
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'is parasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002445',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002445',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002444',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002444',
        ),
      ),
    ),
  );
  $terms[] = array(
    'revision_id' => '51',
    'log' => NULL,
    'timestamp' => '1465294800',
    'uid' => '1',
    'name' => 'Introduced',
    'description' => 'The organism arrived in the region via an anthropogenic mechanism or mechanisms.',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '4e8aab27-595e-4bae-a676-f4ea60ac92be',
    'vocabulary_machine_name' => 'presence_status',
  );
  $terms[] = array(
    'revision_id' => '57',
    'log' => NULL,
    'timestamp' => '1465294915',
    'uid' => '1',
    'name' => 'Captivity',
    'description' => 'The organism is kept in captivity.',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '4fff4f3d-ec9c-44af-8498-3c94c5e5133c',
    'vocabulary_machine_name' => 'presence_status',
  );
  $terms[] = array(
    'revision_id' => '38',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'obligate parasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '51ce252f-87b4-473c-9cb6-c63df6efd5d6',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002227',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002227',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  $terms[] = array(
    'revision_id' => '55',
    'log' => NULL,
    'timestamp' => '1465294895',
    'uid' => '1',
    'name' => 'Managed',
    'description' => 'The organism maintains its presence through intentional cultivation or husbandry.',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '73c47c83-5f38-431f-808b-39fd272dabe7',
    'vocabulary_machine_name' => 'presence_status',
  );
  $terms[] = array(
    'revision_id' => '39',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'hyperparasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '8f3826b3-3e60-4a4e-8a40-c9d3943ef97b',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002554',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002554',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002553',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002553',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  $terms[] = array(
    'revision_id' => '40',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'endoparasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => '9c1f2691-8ba5-40bf-9e1b-753020ee7858',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002635',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002635',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002634',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002634',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  $terms[] = array(
    'revision_id' => '41',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'pollinates',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => 'ba477f7e-6a5f-4019-8052-1868cb7a5ac5',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002455',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002455',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002455',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002455',
        ),
      ),
    ),
  );
  $terms[] = array(
    'revision_id' => '42',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'intracellular endoparasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => 'bf0ddcc2-309e-40bb-82c6-7876c6aa7eca',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002641',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002641',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002640',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002640',
        ),
      ),
    ),
    'parent' => array(
      0 => '9c1f2691-8ba5-40bf-9e1b-753020ee7858',
    ),
  );
  $terms[] = array(
    'revision_id' => '43',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'hemiparasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => 'c015ec91-4d97-4255-93b7-fc07fcd60eb7',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002237',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002237',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  $terms[] = array(
    'revision_id' => '44',
    'log' => NULL,
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'eats',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => 'd4c1b689-bf3f-4a25-a504-2d516877bce2',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002471',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002471',
        ),
      ),
    ),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002470',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002470',
        ),
      ),
    ),
  );
  $terms[] = array(
    'revision_id' => '45',
    'log' => '',
    'timestamp' => '1465294393',
    'uid' => '0',
    'name' => 'faculative parasite of',
    'description' => '',
    'format' => 'plain_text',
    'weight' => '0',
    'uuid' => 'e5ce4152-898a-4740-8afd-868a3d765708',
    'vocabulary_machine_name' => 'ecological_interactions',
    'field_ontology_term_url_reverse_' => array(),
    'field_ontology_url' => array(
      'und' => array(
        0 => array(
          'value' => 'http://purl.obolibrary.org/obo/RO_0002228',
          'format' => NULL,
          'safe_value' => 'http://purl.obolibrary.org/obo/RO_0002228',
        ),
      ),
    ),
    'parent' => array(
      0 => '46f4c73e-36e3-4acb-95f1-c397b6302484',
    ),
  );
  return $terms;
}
