<?php

class MigrateDestinationFieldBackreference extends MigrateDestination{

  /**
   * Constructor
   */
  function __construct($source){
    parent::__construct();
    $this->sourceConnection = $source;
  }

  /**
   * Implement __toString
   */
  public function __toString(){
    return t('Create backreference fields');
  }

  /**
   * The schema for the mapping table
   */
  static public function getKeySchema(){
    return array(
      'field_name' => array(
        'type' => 'text'
      )
    );
  }

  /**
   * Implement import
   */
  public function import(stdClass $object, stdClass $row){
    if(empty($row->global_settings)){return FALSE;}
    // Ensure the viewfield module is enabled
    if(!module_exists('viewfield')){
      module_enable(array(
        'viewfield'
      ));
      // Ensure field type cache is reset
      _field_info_collate_types(TRUE);
    }
    // Read the settings
    $settings = unserialize($row->global_settings);
    if(isset($settings['referrer_fields'])){
      list($referrer_types, $referrer_fields) = $this->_get_nodereferrer_types($row->type_name, $settings);
      $referrer_type = reset($referrer_types);
    }else if(isset($settings['referrer_field'])){
      $referrer_fields = array(
        $settings['referrer_field']
      );
      $referrer_type = $settings['referrer_type'];
    }
    $backref_type = 'content';
    if($row->type == 'nodereferrer'){
      $backref_type = 'list';
    }
    // Actual S1 Backref behaviour would require a field per referrer_field/referrer_type combo. In practice this hasn't
    // been necessary, so we just do this for each referrer_field.
    $view = $this->get_backreference_view();
    $result = array();
    foreach($referrer_fields as $referrer_field){
      $field_name = MigrateDestinationFieldBackreference::generate_field_name($referrer_field, $row->type);
      $object->field_name = $field_name;
      // Get/create the view and the display on the view
      $display_name = $this->get_backreference_display($view, $referrer_field, $backref_type);
      // Create the field and the field instance
      $this->ensure_backreference_field_exists($field_name, $row->type_name);
      $this->create_field_instance($field_name, $view->name, $display_name, $referrer_type, $row);
      $result[] = $field_name;
    }
    return array(
      implode(':', $result)
    );
  }

  /**
   * Implement rollback
   */
  public function rollback(array $ids){
    $key = $ids['destid1'];
    $fields = explode(':', $key);
    foreach($fields as $field_name){
      $info = field_info_field($field_name);
      if($info){
        field_delete_field($field_name);
      }
    }
  }

  /**
   * Return the name of the fields on the destination. We mostly
   * bypass the migration module for creating those, so only return the
   * one we want as key
   */
  public function fields(){
    return array(
      'field_name' => t('Field name')
    );
  }

  /**
   * Generate a field name depending on a referrer field and a field type
   */
  static function generate_field_name($referrer_field, $field_type){
    $name_parts = array(
      'backref',
      'br'
    ); // Long and short prefix for generated field name
    if($field_type == 'nodereferrer'){
      $name_parts = array(
        'backnr',
        'nr'
      );
    }
    $has_prefix = preg_match('/^field_/', $referrer_field);
    if(strlen($referrer_field) <= (32 - strlen($name_parts[0]) - 1)){
      if($has_prefix){
        $field_name = preg_replace('/^field_/', 'field_' . $name_parts[0] . '_', $referrer_field);
      }else{
        $field_name = $name_parts[0] . '_' . $referrer_field;
      }
    }else if(strlen($referrer_field) <= (32 - strlen($name_parts[1]) - 1)){
      // Hopefully collision are still sufficiently unlikely.
      if($has_prefix){
        $field_name = preg_replace('/^field_/', 'field_' . $name_parts[1] . '_', $referrer_field);
      }else{
        $field_name = $name_parts[1] . '_' . $referrer_field;
      }
    }else{
      // XXX Should we raise an error instead ?
      $field_name = "field_" . $name_parts[1] . '_' . substr(md5($name_parts[0] . $referrer_field), 0, 23);
    }
    return $field_name;
  }

  /**
   * Given a nodereferrer settings array and a type name,
   * work out exactly which are the possible referring fields and
   * referring types.
   */
  private function _get_nodereferrer_types($type_name, $settings){
    $referrer_types = array_filter($settings['referrer_types']);
    $referrer_fields = array_filter($settings['referrer_fields']);
    if(empty($referrer_fields)){
      $referrer_fields = array_keys($settings['referrer_fields']);
    }
    if(!empty($referrer_types)){
      // Only keep the referrer fields that are used on the given referrer types
      $referrer_query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
      $referrer_query->fields('i', array(
        'field_name'
      ));
      $referrer_query->condition('i.type_name', $referrer_types, 'IN');
      $referrer_fields = array_intersect($referrer_fields, $referrer_query->execute()->fetchCol());
    }else{
      $referrer_types = array_keys($settings['referrer_types']);
    }
    // In practice people kept the settings wider than they need to be. To avoid generating useless fields, we'll
    // filter this additionally to only keep fields for which there is actual content
    $potential_fields = array();
    foreach($referrer_fields as $referrer_field){
      $r_table = "content_" . $referrer_field;
      if(Database::getConnection('default', $this->sourceConnection)->schema()->tableExists($r_table)){
        $r_field = $referrer_field . "_nid";
        $referrer_query = Database::getConnection('default', $this->sourceConnection)->select($r_table, 'f');
        $referrer_query->fields('f', array(
          'nid'
        ));
        $referrer_query->addJoin('INNER', 'node', 'n', $r_field . '=n.nid');
        $referrer_query->condition('n.type', $type_name);
        $count = $referrer_query->countQuery()->execute()->fetchField();
        if($count > 0){
          $potential_fields[] = $referrer_field;
        }
      }else{
        $potential_fields[] = $referrer_field;
      }
    }
    $referrer_fields = $potential_fields;
    // Only keep the referrer types that include the given referrer fields
    $referrer_query = Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'i');
    $referrer_query->fields('i', array(
      'type_name'
    ));
    $referrer_query->condition('i.field_name', $referrer_fields, 'IN');
    $referrer_types = array_intersect($referrer_types, $referrer_query->execute()->fetchCol());
    return array(
      $referrer_types,
      $referrer_fields
    );
  }

  /**
   * Ensure the given backreference field exists.
   */
  private function ensure_backreference_field_exists($field_name, $type_name){
    // Check if the field already exists
    $info = field_info_field($field_name);
    if($info){return;}
    // Create the field
    $field = array(
      'translatable' => '0',
      'entity_types' => array(),
      'settings' => array(),
      'storage' => array(
        'type' => 'field_sql_storage',
        'settings' => array(),
        'module' => 'field_sql_storage',
        'active' => '1',
        'details' => array(
          'sql' => array(
            'FIELD_LOAD_CURRENT' => array(
              'field_data_' . $field_name => array(
                'vname' => $field_name . '_vname',
                'vargs' => $field_name . '_vargs'
              )
            ),
            'FIELD_LOAD_REVISION' => array(
              'field_revision_' . $field_name => array(
                'vname' => $field_name . '_vname',
                'vargs' => $field_name . '_vargs'
              )
            )
          )
        )
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'field_name' => $field_name,
      'type' => 'viewfield',
      'module' => 'viewfield',
      'active' => 1, /* We want to re-use this field when possible, so it must be active */
      'locked' => FALSE, /* Don't want this locked, regardless of original setting */
      'cardinality' => '1', /* Only '1' makes sense, so ignore what comes from the input */
      'deleted' => '0',
      'columns' => array(
        'vname' => array(
          'type' => 'varchar',
          'not null' => false,
          'length' => 128
        ),
        'vargs' => array(
          'type' => 'varchar',
          'not null' => false,
          'length' => 255
        )
      ),
      'bundles' => array(
        'node' => array(
          0 => $type_name
        )
      )
    );
    field_create_field($field);
  }

  /**
   * Create the new field instance
   */
  private function create_field_instance($field_name, $view_name, $display_name, $referrer_type, $row){
    // Check if the instance already exists
    $info = field_info_instance('node', $field_name, $row->type_name);
    if($info){return;}
    // Get the human name of the referrer type for the label
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_type', 't');
    $query->fields('t', array(
      'name'
    ));
    $query->condition('t.type', $referrer_type);
    $result = $query->execute();
    if($result){
      $label = $result->fetchField();
    }else{
      $label = 'Backreference';
    }
    // Create the field instance
    $display_settings = unserialize($row->display_settings);
    $label = isset($display_settings['label']['format']) ? $display_settings['label']['format'] : 'above';
    $default_type = isset($display_settings['full']['format']) ? $display_settings['full']['format'] : 'default';
    $teaser_type = isset($display_settings['teaser']['format']) ? $display_settings['teaser']['format'] : 'default';
    if($default_type != 'hidden'){
      $default_type = 'viewfield_default';
    }
    if($teaser_type != 'hidden'){
      $teaser_type = 'viewfield_default';
    }
    $instance = array(
      'label' => $referrer_type ? $referrer_type : 'Backreference',
      'widget' => array(
        'weight' => $row->weight,
        'type' => 'viewfield_select',
        'module' => 'viewfield',
        'active' => 0, /* We don't want a widget */
        'settings' => array()
      ),
      'settings' => array(
        'force_default' => 1,
        'allowed_views' => array(
          $view_name => $view_name
        ),
        'user_register_form' => false
      ),
      'display' => array(
        'default' => array(
          'label' => $label,
          'type' => $default_type,
          'settings' => array(),
          'module' => 'viewfield',
          'weight' => $row->weight
        ),
        'teaser' => array(
          'type' => $teaser_type,
          'label' => $label,
          'settings' => array(),
          'weight' => $row->weight
        ),
        'linked_node' => array(
          'type' => 'hidden',
          'label' => $label,
          'settings' => array(),
          'weight' => $row->weight
        )
      ),
      'required' => 0, /* No widget, so never required */
      'description' => '',
      'default_value' => array(
        0 => array(
          'vname' => $view_name . '|' . $display_name,
          'vargs' => '[node:nid]'
        )
      ),
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $row->type_name,
      'deleted' => '0'
    );
    field_create_instance($instance);
  }

  /**
   * Returns the backreference view, and creates it if needed
   */
  private function get_backreference_view(){
    $view = views_get_view('scratchpads_backreference', TRUE);
    if($view){return $view;}
    // This view has not been created yet. Create it now.
    $view = new view();
    $view->name = 'scratchpads_backreference';
    $view->description = '';
    $view->tag = 'default';
    $view->base_table = 'node';
    $view->human_name = 'Scratchpads Backreference';
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
    // Display: Master. This is the default display settings, which displays a single node in
    // full content.
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = 'Scratchpads Backreference';
    $handler->display->display_options['access']['type'] = 'perm';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'mini';
    $handler->display->display_options['pager']['options']['items_per_page'] = '1';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['pager']['options']['id'] = '0';
    $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
    $handler->display->display_options['style_plugin'] = 'default';
    $handler->display->display_options['row_plugin'] = 'entity';
    $handler->display->display_options['row_options']['relationship'] = 'reverse_field_location_node';
    /* Sort criterion: Content: Post date */
    $handler->display->display_options['sorts']['created']['id'] = 'created';
    $handler->display->display_options['sorts']['created']['table'] = 'node';
    $handler->display->display_options['sorts']['created']['field'] = 'created';
    $handler->display->display_options['sorts']['created']['order'] = 'DESC';
    /* Contextual filter: Content: Nid */
    $handler->display->display_options['arguments']['nid']['id'] = 'nid';
    $handler->display->display_options['arguments']['nid']['table'] = 'node';
    $handler->display->display_options['arguments']['nid']['field'] = 'nid';
    $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
    $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
    $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
    $handler->display->display_options['arguments']['nid']['not'] = 0;
    /* Filter criterion: Content: Published */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'node';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = 1;
    $handler->display->display_options['filters']['status']['group'] = 1;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    $view->save();
    return $view;
  }

  /**
   * Given a reference field, return the associate backreference display id on the
   * given view. Create the backreference display if it does not exist.
   */
  private function get_backreference_display($view, $referrer_field_name, $type = 'content'){
    // XXX Not an exact migration - the S1 Backreference behaviour would match both
    // the field_name and the type name. In practice (for the tested site) the second
    // test is redundant, but might need to be introduced later (as a filter on the
    // display returned here).
    $display_name = $referrer_field_name . '_' . $type; // strlen(field_name) <= 32, so this will always be <= 64
    $reverse_name = 'reverse_' . $referrer_field_name . '_node';
    // Look for an existing display
    $view->init_display();
    if(!isset($view->display[$display_name])){
      /* Display: field_pcrprimers */
      $handler = $view->new_display('block', $display_name, $display_name);
      $handler->display->display_options['defaults']['pager'] = FALSE;
      $handler->display->display_options['pager']['type'] = 'none';
      $handler->display->display_options['pager']['options']['offset'] = '0';
      $handler->display->display_options['defaults']['style_plugin'] = FALSE;
      $handler->display->display_options['style_plugin'] = 'grid';
      $handler->display->display_options['style_options']['fill_single_line'] = 1;
      $handler->display->display_options['defaults']['style_options'] = FALSE;
      $handler->display->display_options['defaults']['row_plugin'] = FALSE;
      $handler->display->display_options['row_plugin'] = 'fields';
      $handler->display->display_options['row_options']['hide_empty'] = 0;
      $handler->display->display_options['row_options']['default_field_elements'] = 1;
      $handler->display->display_options['defaults']['row_options'] = FALSE;
      $handler->display->display_options['defaults']['relationships'] = FALSE;
      /* Relationship - reverse */
      $handler->display->display_options['relationships'][$reverse_name]['id'] = $reverse_name;
      $handler->display->display_options['relationships'][$reverse_name]['table'] = 'node';
      $handler->display->display_options['relationships'][$reverse_name]['field'] = $reverse_name;
      $handler->display->display_options['relationships'][$reverse_name]['label'] = $display_name;
      $handler->display->display_options['relationships'][$reverse_name]['required'] = 0;
      $handler->display->display_options['defaults']['fields'] = FALSE;
      /* Field: Content: Title */
      $handler->display->display_options['fields']['title']['id'] = 'title';
      $handler->display->display_options['fields']['title']['table'] = 'node';
      $handler->display->display_options['fields']['title']['field'] = 'title';
      $handler->display->display_options['fields']['title']['relationship'] = $reverse_name;
      $handler->display->display_options['fields']['title']['label'] = '';
      $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
      $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
      $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
      $handler->display->display_options['fields']['title']['alter']['external'] = 0;
      $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
      $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
      $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
      $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
      $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
      $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
      $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
      $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
      $handler->display->display_options['fields']['title']['alter']['html'] = 0;
      $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
      $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
      $handler->display->display_options['fields']['title']['hide_empty'] = 0;
      $handler->display->display_options['fields']['title']['empty_zero'] = 0;
      $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
      $handler->display->display_options['fields']['title']['link_to_node'] = 1;
      $view->save();
    }
    return $display_name;
  }
}