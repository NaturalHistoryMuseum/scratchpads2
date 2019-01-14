<?php

/**
 * @file
 * Set up for the beer (basic) example.
 */
function migrate_example_beer_schema() {
  $schema['migrate_example_beer_account'] = migrate_example_beer_schema_account();
  $schema['migrate_example_beer_node'] = migrate_example_beer_schema_node();
  $schema['migrate_example_beer_comment'] = migrate_example_beer_schema_comment();
  $schema['migrate_example_beer_topic'] = migrate_example_beer_schema_topic();
  $schema['migrate_example_beer_topic_node'] = migrate_example_beer_schema_topic_node();

  // These two tables are primarily for testing the table_copy plugin.
  // They do provide some guidance for uri redirection per uri_map_redirect.php
  $schema['migrate_example_beer_legacy_urls'] = migrate_example_beer_schema_legacy_urls();
  $schema['migrate_example_beer_copy_urls'] = migrate_example_beer_schema_legacy_urls();

  return $schema;
}

function migrate_example_beer_install() {
  migrate_example_beer_content_type();
  migrate_example_beer_tags();
  migrate_example_beer_image();
  migrate_example_beer_country();
  migrate_example_beer_gender();
  if (module_exists('node_reference')) {
    migrate_example_beer_favs();
  }

  // Populate our tables.
  migrate_example_beer_data_account();
  migrate_example_beer_data_node();
  migrate_example_beer_data_comment();
  migrate_example_beer_data_topic();
  migrate_example_beer_data_topic_node();
  migrate_example_beer_data_urls();
}

function migrate_example_beer_uninstall() {
  if ($vids = taxonomy_vocabulary_load_multiple(array(), array('machine_name' => 'migrate_example_beer_styles'))) {
    // Grab key of the first returned vocabulary.
    taxonomy_vocabulary_delete(key($vids));
  }
  migrate_example_beer_content_type_delete();
}

function migrate_example_beer_disable() {
  MigrateGroup::deregister('beer');
}

function migrate_example_beer_schema_node() {
  return array(
    'description' => 'Beers of the world.',
    'fields' => array(
      'bid' => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Beer ID.',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Full description of the beer.',
      ),
      'excerpt' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Abstract for this beer.',
      ),
      'countries' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Countries of origin. Multiple values, delimited by pipe',
      ),
      'aid' => array(
        'type' => 'int',
        'not null' => FALSE,
        'description' => 'Account Id of the author.',
      ),
      'image' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Image path',
      ),
      'image_alt' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Image ALT',
      ),
      'image_title' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Image title',
      ),
      'image_description' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Image description',
      ),
    ),
    'primary key' => array('bid'),
  );
}

function migrate_example_beer_schema_topic() {
  return array(
    'description' => 'Categories',
    'fields' => array(
      'style' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'details' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
      ),
      'style_parent' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Parent topic, if any',
      ),
      'region' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Region first associated with this style',
      ),
      'hoppiness' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Relative hoppiness of the beer',
      ),
    ),
    'primary key' => array('style'),
  );
}

function migrate_example_beer_schema_topic_node() {
  return array(
    'description' => 'Beers topic pairs.',
    'fields' => array(
      'bid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Beer ID.',
      ),
      'style' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Topic name',
      ),
    ),
    'primary key' => array('style', 'bid'),
  );
}

function migrate_example_beer_schema_comment() {
  return array(
    'description' => 'Beers comments.',
    'fields' => array(
      'cid' => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Comment ID.',
      ),
      'bid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Beer ID that is being commented upon',
      ),
      'cid_parent' => array(
        'type' => 'int',
        'not null' => FALSE,
        'description' => 'Parent comment ID in case of comment replies.',
      ),
      'subject' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment subject',
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment body',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment name (if anon)',
      ),
      'mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment email (if anon)',
      ),
      'aid' => array(
        'type' => 'int',
        'not null' => FALSE,
        'description' => 'Account ID (if any).',
      ),
    ),
    'primary key' => array('cid'),
  );
}

function migrate_example_beer_schema_account() {
  return array(
    'description' => 'Beers accounts.',
    'fields' => array(
      'aid' => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Account ID',
      ),
      'status' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Blocked_Allowed',
      ),
      'posted' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Registration date',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account name (for login)',
      ),
      'nickname' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account name (for display)',
      ),
      'password' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account password (raw)',
      ),
      'mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account email',
      ),
      'sex' => array(
        'type' => 'int',
        'not null' => FALSE,
        'description' => 'Gender',
      ),
      'beers' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Favorite Beers',
      ),
    ),
    'primary key' => array('aid'),
  );
}

function migrate_example_beer_schema_legacy_urls() {
  return array(
    'description' => 'Stores legacy paths and destination ids for redirection.',
    'fields' => array(
      'id' => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Primary Key: ID.',
      ),
      'migration_name' => array(
        'type' => 'varchar',
        'length' => 50,
        'not null' => TRUE,
        'default' => '',
      ),
      'source_id' => array(
        'type' => 'int',
        'not null' => FALSE,
      ),
      'source_uri' => array(
        'type' => 'varchar',
        'length' => 500,
        'not null' => FALSE,
      ),
      'modificationdatetime' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    ),
    'primary key' => array('ID'),
    'indexes' => array(
      'source_uri' => array(array('source_uri', 255)),
    ),
  );
}

function migrate_example_beer_content_type() {
  // This code based on from standard.profile.
  // Insert default user-defined node types into the database.
  $types = array(
    array(
      'type' => 'migrate_example_beer',
      'name' => st('Beer'),
      'base' => 'node_content',
      'description' => st("Beer is what we drink."),
      'custom' => 1,
      'modified' => 1,
      'locked' => 1,
    ),
  );

  foreach ($types as $type) {
    $type = node_type_set_defaults($type);
    node_type_save($type);
    node_add_body_field($type);
  }
}

function migrate_example_beer_tags() {
  // Create a vocabulary named "Migrate Example Beer Styles", enabled for the 'migrate_example_beer' content type.
  $description = st('Use tags to group beers on similar topics into categories.');
  $help = st('Enter a comma-separated list of words to describe your content.');
  $vocabulary = (object) array(
    'name' => 'Migrate Example Beer Styles',
    'description' => $description,
    'machine_name' => 'migrate_example_beer_styles',
    'help' => $help,

  );
  taxonomy_vocabulary_save($vocabulary);

  if (!field_info_field('migrate_example_beer_styles')) {
    $field = array(
      'field_name' => $vocabulary->machine_name,
      'type' => 'taxonomy_term_reference',
      // Set cardinality to unlimited for tagging.
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'settings' => array(
        'allowed_values' => array(
          array(
            'vocabulary' => $vocabulary->machine_name,
            'vid' => $vocabulary->vid,
            'parent' => 0,
          ),
        ),
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'migrate_example_beer_styles', 'migrate_example_beer')) {
    $instance = array(
      'field_name' => $vocabulary->machine_name,
      'entity_type' => 'node',
      'label' => $vocabulary->name,
      'bundle' => 'migrate_example_beer',
      'description' => $vocabulary->help,
      'widget' => array(
        'type' => 'taxonomy_autocomplete',
      ),
    );
    field_create_instance($instance);
  }
}

// Create an image field named "Migrate Example Image", enabled for the 'Beer' content type.
function migrate_example_beer_image() {
  if (!field_info_field('field_migrate_example_image')) {
    $field = array(
      'field_name' => 'field_migrate_example_image',
      'type' => 'image',
      'cardinality' => 1,
      'translatable' => TRUE,
      'indexes' => array('fid' => array('fid')),
      'settings' => array(
        'uri_scheme' => 'public',
        'default_image' => FALSE,
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_migrate_example_image', 'migrate_example_beer')) {
    $instance = array(
      'field_name' => 'field_migrate_example_image',
      'entity_type' => 'node',
      'label' => 'Image',
      'bundle' => 'migrate_example_beer',
      'description' => 'Upload an image to go with this beer.',
      'settings' => array(
        'file_directory' => 'field/migrate_example/image',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'alt_field' => TRUE,
        'title_field' => '',
      ),

      'widget' => array(
        'type' => 'image_image',
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
        'weight' => -1,
      ),

      'display' => array(
        'full' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'type' => 'image_link_content__medium',
          'settings' => array(),
          'weight' => -1,
        ),
        'rss' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_index' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_results' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
      ),
    );
    field_create_instance($instance);
  }
}

function migrate_example_beer_favs() {
  if (!field_info_field('field_migrate_example_favbeers')) {
    $field = array(
      'field_name' => 'field_migrate_example_favbeers',
      'type' => 'node_reference',
      'cardinality' => -1,
      'settings' => array(
        'referenceable_types' => array('migrate_example_beer'),
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('user', 'field_migrate_example_favbeers', 'user')) {
    $instance = array(
      'field_name' => 'field_migrate_example_favbeers',
      'entity_type' => 'user',
      'label' => 'Favorite Beers',
      'bundle' => 'user',
      'widget' => array(
        'type' => 'node_reference_autocomplete',
      ),
    );
    field_create_instance($instance);
  }
}

// Create Gender list field on User entity.
function migrate_example_beer_gender() {
  if (!field_info_field('field_migrate_example_gender')) {
    $field = array(
      'field_name' => 'field_migrate_example_gender',
      'type' => 'list_integer',
      'settings' => array(
        'allowed_values' => array(
          0 => 'Male',
          1 => 'Female'
        ),
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('user', 'field_migrate_example_gender', 'user')) {
    $instance = array(
      'field_name' => 'field_migrate_example_gender',
      'entity_type' => 'user',
      'label' => 'Gender',
      'bundle' => 'user',
      'widget' => array(
        'type' => 'options_select',
      ),
    );
    field_create_instance($instance);
  }
}

// Create a text field named "Countries", enabled for the 'Beer' content type.
function migrate_example_beer_country() {
  if (!field_info_field('field_migrate_example_country')) {
    $field = array(
      'field_name' => 'field_migrate_example_country',
      'type' => 'text',
      'cardinality' => -1,
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_migrate_example_country', 'migrate_example_beer')) {
    $instance = array(
      'field_name' => 'field_migrate_example_country',
      'entity_type' => 'node',
      'label' => 'Countries',
      'bundle' => 'migrate_example_beer',
      'description' => 'Beer country.',

      'widget' => array(
        'type' => 'text_textfield',
      ),
    );
    field_create_instance($instance);
  }
}

function migrate_example_beer_content_type_delete() {
  $bundle = 'migrate_example_beer';
  $field_names = array(
    'migrate_example_beer_styles',
    'field_migrate_example_image',
    'field_migrate_example_country',
  );
  foreach ($field_names as $field_name) {
    $instance = field_info_instance('node', $field_name, $bundle);
    field_delete_instance($instance);
    field_delete_field($field_name);
  }
  node_type_delete($bundle);

  $bundle = 'user';
  $field_names = array('field_migrate_example_gender');
  if (module_exists('node_reference')) {
    $field_names[] = 'field_migrate_example_favbeers';
  }
  foreach ($field_names as $field_name) {
    $instance = field_info_instance('user', $field_name, $bundle);
    field_delete_instance($instance);
    field_delete_field($field_name);
  }
}

function migrate_example_beer_data_node() {
  $fields = array(
    'bid',
    'name',
    'body',
    'excerpt',
    'countries',
    'aid',
    'image',
    'image_alt',
    'image_title',
    'image_description',
  );
  $query = db_insert('migrate_example_beer_node')
    ->fields($fields);
  // Use high bid numbers to avoid overwriting an existing node id.
  $data = array(
    array(
      99999999,
      'Heineken',
      'Blab Blah Blah Green',
      'Green',
      'Netherlands|Belgium',
      0,
      'heineken.jpg',
      'Heinekin alt',
      'Heinekin title',
      'Heinekin description',
    ), // comes with migrate_example project.
    array(
      99999998,
      'Miller Lite',
      'We love Miller Brewing',
      'Tasteless',
      'USA|Canada',
      1,
      NULL,
      NULL,
      NULL,
      NULL,
    ),
    array(
      99999997,
      'Boddington',
      'English occassionally get something right',
      'A treat',
      'United Kingdom',
      1,
      NULL,
      NULL,
      NULL,
      NULL,
    ),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

// Note that alice has duplicate username. Exercies dedupe() method.
// @TODO duplicate email also.
function migrate_example_beer_data_account() {
  $fields = array(
    'status',
    'posted',
    'name',
    'nickname',
    'password',
    'mail',
    'sex',
    'beers',
  );
  $query = db_insert('migrate_example_beer_account')
    ->fields($fields);
  $data = array(
    array(
      1,
      '2010-03-30 10:31:05',
      'alice',
      'alice hot pants',
      'alicepass',
      'alice@example.com',
      '1',
      '99999999|99999998|99999997',
    ),
    array(
      1,
      '2010-04-04 10:31:05',
      'alice',
      'alice dupe pants',
      'alicepass',
      'alice2@example.com',
      '1',
      '99999999|99999998|99999997',
    ),
    array(
      0,
      '2007-03-15 10:31:05',
      'bob',
      'rebob',
      'bobpass',
      'bob@example.com',
      '1',
      '99999999|99999997',
    ),
    array(
      1,
      '2004-02-29 10:31:05',
      'charlie',
      'charlie chocolate',
      'mykids',
      'charlie@example.com',
      '0',
      '99999999|99999998',
    ),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_beer_data_comment() {
  $fields = array(
    'bid',
    'cid_parent',
    'subject',
    'body',
    'name',
    'mail',
    'aid',
  );
  $query = db_insert('migrate_example_beer_comment')
    ->fields($fields);
  $data = array(
    array(
      99999998,
      NULL,
      'im first',
      'hot body',
      'alice',
      'alice@example.com',
      0,
    ),
    array(
      99999998,
      NULL,
      'im second',
      'hot body',
      'alice',
      'alice@example.com',
      0,
    ),
    array(
      99999999,
      NULL,
      'im parent',
      'hot body',
      'alice',
      'alice@example.com',
      0,
    ),
    array(99999999, 1, 'im child', 'cold body', 'bob', NULL, 1),
    array(
      99999999,
      2,
      'im grandchild',
      'bitter body',
      'charlie@example.com',
      NULL,
      1,
    ),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_beer_data_topic() {
  $fields = array('style', 'details', 'style_parent', 'region', 'hoppiness');
  $query = db_insert('migrate_example_beer_topic')
    ->fields($fields);
  $data = array(
    array('ale', 'traditional', NULL, 'Medieval British Isles', 'Medium'),
    array('red ale', 'colorful', 'ale', NULL, NULL),
    array(
      'pilsner',
      'refreshing',
      NULL,
      'Pilsen, Bohemia (now Czech Republic)',
      'Low',
    ),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_beer_data_topic_node() {
  $fields = array('bid', 'style');
  $query = db_insert('migrate_example_beer_topic_node')
    ->fields($fields);
  $data = array(
    array(99999999, 'pilsner'),
    array(99999999, 'red ale'),
    array(99999998, 'red ale'),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}

function migrate_example_beer_data_urls() {
  $fields = array(
    'id',
    'migration_name',
    'source_id',
    'source_uri',
    'modificationdatetime',
  );
  $query = db_insert('migrate_example_beer_legacy_urls')
    ->fields($fields);
  $data = array(
    array(
      1,
      'BeerNode',
      99999997,
      'the_boddington/main',
      strtotime('2010-04-12 08:32:06'),
    ),
    array(
      2,
      'BeerNode',
      99999998,
      'Miller Lite taste',
      strtotime('2010-04-12 08:32:05'),
    ),
    array(
      3,
      'BeerNode',
      99999999,
      'green wonder',
      strtotime('2010-04-12 08:32:03'),
    ),
  );
  foreach ($data as $row) {
    $query->values(array_combine($fields, $row));
  }
  $query->execute();
}
