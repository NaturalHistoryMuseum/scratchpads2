<?php

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 * dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 * the default.
 */
class ScratchpadsNodeMigration extends DrupalNode6Migration{

  /**
   * Allow for multiple biological classifications which need to be merged into
   * the taxon name field
   *
   * @var array
   */
  protected $biologicalClassifications = array();

  public function getBiologicalClassifications(){
    return $this->biologicalClassifications;
  }

  /**
   * Track fields that have been converted to image fields
   *
   * @var array
   */
  protected $extraImageFields = array();

  /**
   * Long text fields we'll need to parse files etc.,
   *
   * @var array
   */
  protected $longTextFields = array();

  public function getLongTextFields(){
    return $this->longTextFields;
  }

  protected $manualFieldMapping = array();

  public function getManualFieldMapping($field_name = null){
    if($field_name){
      if(isset($this->manualFieldMapping[$field_name])){
        return $this->manualFieldMapping[$field_name];
      }else{
        // Check if a lower case version of the field mapping exists
        foreach(array_keys($this->manualFieldMapping) as $field_mapping){
          if($field_name == strtolower($field_mapping)){return $this->manualFieldMapping[$field_mapping];}
        }
        // Manual mapping without using a class
        $type = $this->getFieldType($field_name);
        $final = DrupalFieldMigration::getCustomMapping($field_name, $type, $this->sourceConnection);
        if($final != $field_name){return $final;}
        return false;
      }
    }else{
      return $this->manualFieldMapping;
    }
  }

  public function sourceFieldList(){
    $fields = parent::sourceFieldList();
    $fields['file_attachment'] = t('Attached files');
    $fields['menu'] = t('Menu');
    $fields['node_weight'] = t('Node weight');
    return $fields;
  }

  public function __construct(array $arguments){
    parent::__construct($arguments);
    // SO versioning still works, we're going to use VID, not NID
    $this->map = new MigrateSQLMap($this->machineName, array(
      'vid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Source node VID',
        'alias' => 'n'
      )
    ), ScratchpadsMigrateDestinationNode::getKeySchema());
    $this->fieldSuffixes['link'] = 'url';
    $this->fieldSuffixes['content_taxonomy'] = 'value';
    $this->fieldSuffixes['number_float'] = 'value';
    $this->fieldSuffixes['taxonomy_term_reference'] = 'tid';
    $this->fieldSuffixes['userreference'] = 'uid';
    $this->destination = new ScratchpadsMigrateDestinationNode($this->destinationType);
    $destination_fields = $this->destination->fields();
    $fields = $this->sourceFieldList();
    // Add the manual field mappings to the fields array
    // We can't just use $fields += $this->getManualFieldMapping(); here
    // As SPM has uppercase field name problems
    foreach($this->getManualFieldMapping() as $delta => $field_mapping){
      $lc_delta = strtolower($delta);
      if(!array_key_exists($lc_delta, $fields)){
        $fields[$lc_delta] = $field_mapping;
      }
    }
    foreach($fields as $delta => $field){
      // Numeric keys are vocabulary fields
      if(is_numeric($delta)){
        $vocabulary = taxonomy_vocabulary_load($delta);
        $vids = array_keys(variable_get('biological_vids', array()));
        if($vocabulary){
          if($vocabulary->module != 'forum'){
            // Is this a biological classification
            if(in_array($delta, $vids)){
              $field_name = 'field_taxonomic_name';
            }else{
              $field_name = DrupalFieldTaxonomyMigration::getFieldName($vocabulary->name, $this->sourceConnection);
            }
          }else{
            $field_name = 'taxonomy_forums';
          }
          $sourceMigration = scratchpads_migrate_get_machine_name('Term', $vocabulary->name);
          // Handle multiple biological classifications
          if($field_name == 'field_taxonomic_name'){
            $this->biologicalClassifications[] = $delta;
            // If we already have a mapped biological classification, add as
            // unmapped source, we'll merge in perpareRow
            if(count($this->biologicalClassifications) > 1){
              $this->addUnmigratedSources(array(
                $delta
              ));
              continue;
            }
          }
          // Remove any existing mapping
          $this->removeFieldMapping($field_name);
          $this->removeFieldMapping($field_name . ':source_type');
          $this->removeFieldMapping($field_name . ':create_term');
          $this->addFieldMapping($field_name, $delta);
          $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
          $this->addFieldMapping($field_name . ':create_term')->defaultValue(false);
        }else{
          // Do nothing with Image Galleries vocab
          $this->addUnmigratedSources(array(
            $delta
          ));
        }
      }elseif($delta == 'file_attachment'){
        // Does this node type have a file_field?
        if(array_key_exists('field_file', $destination_fields)){
          // Does have field file mapping so add a simple mapping.
          // We'll get the files in prepareRow
          $this->addFieldMapping('field_file', $delta)->sourceMigration('File');
          $this->addFieldMapping('field_file:file_class')->defaultValue('MigrateFileFid');
          $this->addFieldMapping('field_file:file_replace')->defaultValue(FILE_EXISTS_REPLACE);
          $this->addUnmigratedDestinations(array(
            'field_file:language',
            'field_file:description',
            'field_file:display',
            'field_file:destination_dir',
            'field_file:destination_file',
            'field_file:preserve_files',
            'field_file:source_dir'
          ));
        }else{
          $this->addUnmigratedSources(array(
            'file_attachment'
          ));
        }
      }elseif($delta == 'node_weight'){
        $this->addFieldMapping('weight_weight', $delta);
      }else{ // Other field types
        // Load the field
        $source_type = $this->getFieldType($delta);
        $mapped_delta_source_type = DrupalFieldMigration::getCustomMapping($delta, $source_type, $this->sourceConnection);
        if($mapped_delta_source_type){
          $field = field_info_field($mapped_delta_source_type);
        }
        if(!$mapped_delta_source_type || !$field){
          $field = field_info_field($delta);
        }
        // If we don't have a field with the same name, check if it's a media
        // field
        if($field){
          $type = $field['type'];
        }else{
          $type = $this->getFieldType($delta);
        }
        if(isset($type)){
          switch($type){
            case 'image':
            case 'file':
            case 'filefield':
              // D6 doesn't seperate type of file fields, so chceck the
              // widget here
              $widget_type = $this->getFieldWidgetType($delta);
              $file_destination_field = DrupalFieldMigration::getCustomMapping($delta, $type, $this->sourceConnection);
              if($widget_type == 'nodereference_select'){
                $file_destination_field = $delta;
                $this->extraImageFields[] = $delta;
              }
              if($widget_type == 'imagefield_widget' || $widget_type == 'nodereference_select'){
                $this->addFieldMapping($file_destination_field, $delta)->sourceMigration('Image');
              }else{
                $this->addFieldMapping($file_destination_field, $delta)->sourceMigration('File');
              }
              $this->addFieldMapping($file_destination_field . ':file_class')->defaultValue('MigrateFileFid');
              $this->addFieldMapping($file_destination_field . ':file_replace')->defaultValue(FILE_EXISTS_REPLACE);
              $this->addUnmigratedDestinations(array(
                $file_destination_field . ':language',
                $file_destination_field . ':description',
                $file_destination_field . ':display',
                $file_destination_field . ':destination_dir',
                $file_destination_field . ':destination_file',
                $file_destination_field . ':preserve_files',
                $file_destination_field . ':source_dir',
                $file_destination_field . ':alt',
                $file_destination_field . ':title'
              ));
              break;
            case 'node_reference':
              if(!$destination_field = $this->getManualFieldMapping($delta)){
                $destination_field = $delta;
              }
              // Get the source migration
              $referenceable_types = array_filter($field['settings']['referenceable_types']);
              $sourceMigration = ucfirst(current($referenceable_types));
              // We don't care about the node type - just need to specify any node migration to use to create a stub
              $this->addFieldMapping($destination_field, $delta)->sourceMigration(array(
                $sourceMigration
              ));
              break;
            case 'noderelationships_backref':
            case 'nodereferrer':
              // These fields don't actually contain content, we don't want them migrated
              $this->removeFieldMapping($delta);
              $this->addUnmigratedSources(array(
                $delta
              ));
              break;
            default:
              // Special use case for taxonomicname field - (darwincore)
              // Need to add taxonomicname field to biologicalClassifications array
              if($delta == 'taxonomicname'){
                $this->biologicalClassifications[] = $delta;
                $field_name = 'field_taxonomic_name';
                // If we already have a mapped biological classification, add as
                // unmapped source, we'll merge in perpareRow
                if(count($this->biologicalClassifications) > 1){
                  $this->addUnmigratedSources(array(
                    $delta
                  ));
                }else{
                  $this->addFieldMapping($field_name, $delta);
                  $this->addFieldMapping($field_name . ':source_type')->defaultValue('tid');
                  $this->addFieldMapping($field_name . ':create_term')->defaultValue(false);
                }
                continue;
              }
              if($type == 'text_long'){
                // Keep a record of long text fields, as we need to extract
                // files
                $this->longTextFields[] = $delta;
              }
              if(!$destination_field = $this->getManualFieldMapping($delta)){
                $destination_field = $delta;
              }
              $this->addFieldMapping($destination_field, $delta);
              foreach(array(
                'format' => 'filtered_html',
                'language' => NULL,
                'source_type' => 'tid',
                'create_term' => FALSE
              ) as $subfield_type => $default_value){
                $subfield = $destination_field . ':' . $subfield_type;
                if(array_key_exists($subfield, $this->destination->fields())){
                  $this->addFieldMapping($subfield)->defaultValue($default_value);
                }
              }
              break;
          }
        }
      }
    }
    // In D6 sticky is actually node weight
    $this->removeFieldMapping('sticky');
    $this->addFieldMapping('sticky', 'sticky')->defaultValue(0);
    // Use english as the default language
    $this->removeFieldMapping('language');
    $this->addFieldMapping('language', 'language')->defaultValue('en');
    $this->addFieldMapping('nid', 'nid');
    $this->addFieldMapping('vid', 'vid');
    $this->removeFieldMapping('is_new');
    $this->addFieldMapping('is_new', NULL)->defaultValue(true);
    // Revisioning
    $this->removeFieldMapping('changed');
    $this->addFieldMapping('changed', 'timestamp');
    $this->addFieldMapping('group_audience', 'group_nid'); // Remap this in
    // prepare row -
    // needs to use OG id
    $this->addFieldMapping('group_content_access', 'og_public')->defaultValue(0);
    // Does this node have a body field?
    if(field_read_instance('node', 'body', $this->destinationType)){
      $this->longTextFields[] = 'body';
      $this->addUnmigratedDestinations(array(
        'body:summary',
        'body:format',
        'body:language'
      ));
    }else{
      $this->removeFieldMapping('body');
      $this->addUnmigratedSources(array(
        'body'
      ));
    }
    // Overide auto node title - we want to use the old titles
    $this->addFieldMapping('auto_nodetitle_applied')->defaultValue(true);
    $fields['path'] = t('Node: Path alias');
    $this->source = new MigrateSourceSQL($this->nodeQuery(), $fields, $this->countQuery(), $this->sourceOptions);
    // New fields added?
    $this->addUnmigratedDestinations(array(
      'totalcount',
      'daycount',
      'timestamp'
    ));
  }

  protected function nodeQuery(){
    // Get organic groups info
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_revisions', 'nr')->fields('nr', array(
      'vid',
      'body',
      'teaser',
      'format',
      'uid',
      'timestamp'
    ));
    $query->condition('type', $this->sourceType);
    $query->leftJoin('node', 'n', 'n.nid=nr.nid');
    $query->fields('n', array(
      'nid',
      'language',
      'title',
      'status',
      'created',
      'comment',
      'promote',
      'moderate',
      'sticky'
    ));
    // NOTE: OG works with NID, not VID
    if(Database::getConnection('default', $this->sourceConnection)->schema()->tableExists('og_ancestry')){
      $query->leftJoin('og_ancestry', 'oga', 'oga.nid = n.nid');
      $query->addField('oga', 'group_nid', 'group_nid');
    }else{
      // Simulate an empty left join.
      $query->addExpression(':null', 'group_nid', array(
        ':null' => NULL
      ));
    }
    if(Database::getConnection('default', $this->sourceConnection)->schema()->tableExists('og_access_post')){
      $query->leftJoin('og_access_post', 'ogap', 'ogap.nid = n.nid');
      $query->addField('ogap', 'og_public', 'og_public');
    }else{
      // Simulate an empty left join.
      $query->addExpression(':null', 'og_public', array(
        ':null' => NULL
      ));
    }
    $query->condition('n.title', 'Taskguide', '!=');
    $query->orderBy('nr.vid');
    return $query;
  }
  // Allow overiding count query
  protected function countQuery(){
    $query = Database::getConnection('default', $this->sourceConnection)->select('node_revisions', 'nr');
    $query->addExpression('COUNT (nr.vid)');
    $query->innerJoin('node', 'n', 'n.nid=nr.nid');
    $query->condition('n.type', $this->sourceType);
    $query->condition('n.title', 'Taskguide', '!=');
    return $query;
  }

  protected function getFieldType($field_name){
    return Database::getConnection('default', $this->sourceConnection)->select('content_node_field', 'cnf')->fields('cnf', array(
      'type'
    ))->condition('field_name', $field_name)->execute()->fetchField();
  }

  protected function getFieldWidgetType($field_name){
    return Database::getConnection('default', $this->sourceConnection)->select('content_node_field_instance', 'cnfi')->fields('cnfi', array(
      'widget_type'
    ))->condition('field_name', $field_name)->distinct()->execute()->fetchField();
  }

  public function prepareRow($row){
    // Delete all migration error messages for this node
    // That way, only errors relating to the most recent migration will be displayed to the users
    scratchpads_migrate_task_delete('node', $row->nid);
    parent::prepareRow($row);
    // Doesn't seem to use the default value, unless it's unset
    if(empty($row->language)){
      unset($row->language);
    }
    // Get the group ID
    if(!empty($row->group_nid)){
      // Get the group type
      $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
      $query->addField('n', 'type');
      $query->condition('n.nid', $row->group_nid);
      $group_type = $query->execute()->fetchField();
      // Skip publication group types. May need to add other ones here too
      if($group_type != 'publication'){
        $group_id = db_query('SELECT gid FROM {og} WHERE og.etid = :group_nid', array(
          ':group_nid' => $row->group_nid
        ))->fetchField();
        if($group_id){
          $row->group_nid = $group_id;
        }else{
          watchdog('scratchpads_migrate', 'Could not find group id for group node %group_nid', array(
            '%group_nid' => $row->group_nid
          ), WATCHDOG_ERROR);
          unset($row->og_public);
          unset($row->group_nid); // Avoid this getting associated to the wrong group
        }
      }else{ // This is part of a publication group so unset it
        unset($row->og_public);
        unset($row->group_nid);
      }
    }
    if(isset($row->og_public)){
      switch($row->og_public){
        case 0:
          $row->og_public = 2; // Private
          break;
        case 1:
          break;
        default:
          $row->og_public = 0; // Group defaults
          break;
      }
    }
    // Add the url alias
    $query = Database::getConnection('default', $this->sourceConnection)->select('url_alias', 'a');
    $query->addField('a', 'dst');
    $query->condition('a.src', 'node/' . $row->nid);
    // We're not using path, as SP1 nodes have multiple aliases
    $row->url_alias = $query->execute()->fetchCol();
    // Does this node have files attached?
    $query = Database::getConnection('default', $this->sourceConnection)->select('upload', 'u');
    $query->addField('u', 'fid');
    $query->condition('u.vid', $row->vid);
    $row->file_attachment = $query->execute()->fetchCol();
    // If this row has a nodereference field that references an image field,load the fid
    foreach($this->extraImageFields as $source_field){
      $query = Database::getConnection('default', $this->sourceConnection)->select('image', 'i');
      $query->addField('i', 'fid');
      $query->condition('i.image_size', '_original');
      $query->innerJoin('content_' . $source_field, 'f', 'f.' . $source_field . '_nid=i.nid');
      $query->condition('f.vid', $row->vid);
      $row->{$source_field} = $query->execute()->fetchCol();
    }
    // Unset null rows, necessary for dwc location
    $properties = array_keys((array)$row);
    foreach($properties as $property){
      if(empty($row->{$property}) && $row->{$property} != 0){
        unset($row->{$property});
      }
    }
    // Extract any files
    foreach($this->getLongTextFields() as $longTextField){
      if(isset($row->{$longTextField})){
        if(!is_array($row->{$longTextField})){
          $long_text_data = array(
            $row->{$longTextField}
          );
        }else{
          $long_text_data = $row->{$longTextField};
        }
        $long_text_result = array();
        foreach($long_text_data as $long_text_key => $long_text_html){
          $long_text_html = ScratchpadsNodeMigration::lowercaseHtml($long_text_html);
          $long_text_html = ScratchpadsNodeMigration::parseImages($long_text_html, $row);
          $long_text_html = ScratchpadsNodeMigration::parseStyles($long_text_html);
          $long_text_html = preg_replace('/\[(\/?)no-glossary\]/', '[$1no-lexicon]', $long_text_html);
          $long_text_result[$long_text_key] = $long_text_html;
        }
        if(!is_array($row->{$longTextField})){
          $row->{$longTextField} = reset($long_text_result);
        }else{
          $row->{$longTextField} = $long_text_result;
        }
      }
    }
    // Is there a menu item for this node?
    $query = Database::getConnection('default', $this->sourceConnection)->select('menu_links', 'ml');
    $query->addField('ml', 'link_title');
    $query->condition('link_path', 'node/' . $row->nid);
    $menu_title = $query->execute()->fetchField();
    // All we want to get is the menu title
    // We'll add this to the top level main menu and re-organise by hand
    // afterwards
    if($menu_title){
      $row->menu = array(
        'menu_name' => PRIMARY_MENU_NAME,
        'module' => 'menu',
        'link_title' => $menu_title,
        'weight' => 1,
        'parent' => PRIMARY_MENU_NAME . ':0',
        'parent_depth_limit' => 0
      );
    }
    // Are there multiple biological classifcations?
    if(count($this->biologicalClassifications) > 1){
      // First item biological classification is the one we're using as the base VID
      $mapped_classification = reset($this->biologicalClassifications);
      if(!isset($row->{$mapped_classification})){
        $row->{$mapped_classification} = array();
      }
      foreach($this->biologicalClassifications as $unmapped_classifcation){
        if(isset($row->{$unmapped_classifcation})){
          $row->{$mapped_classification} = array_merge($row->{$mapped_classification}, (array)$row->{$unmapped_classifcation});
          // Dedupe
          $row->{$mapped_classification} = array_unique($row->{$mapped_classification});
        }
      }
    }
    // Row weight
    $this->weightDecode($row);
    // Remove node teaser
    unset($row->teaser);
  }

  public static function parseImages($html, $row){
    if(strpos($html, '<img') !== false){
      try{
        $old_html = $html;
        $html = mb_convert_encoding($html, 'html-entities', 'utf-8');
        $dom = new DOMDocument();
        $dom->strictErrorChecking = false;
        @$dom->loadHTML($html);
        $dom->normalizeDocument();
        $items = $dom->getElementsByTagName('img');
        for($i = 0; $i < $items->length; $i++){
          // Does this img have a parent anchor tag
          $parent_node = $items->item($i)->parentNode;
          if($parent_node->tagName == 'a'){
            $href = $parent_node->getAttribute('href');
            if(strpos($href, 'files') !== false){
              if($new_href = ScratchpadsNodeMigration::saveFile($href, $row->uid, $row)){
                $new_href .= '?width=600&height=600';
                $parent_node->removeAttribute('href');
                $parent_node->setAttribute('href', $new_href);
                $parent_node->setAttribute('class', 'colorbox-inline');
              }
            }
          }
          $src = $items->item($i)->getAttribute('src');
          if($new_href = ScratchpadsNodeMigration::saveFile($src, $row->uid, $row)){
            $items->item($i)->removeAttribute('src');
            $items->item($i)->setAttribute('src', $new_href);
          }
        }
        $dom->encoding = 'UTF-8';
        $html = $dom->saveHTML();
      }
      catch(Exception $e){
        scratchpads_migrate_task_insert('node', $row->nid, t('The html in this node is malformed, so we cannot extract the images.'), t('If the node has images they will need to be manually copied to the new site.'));
        watchdog('scratchpads_migrate', t('Error !error for node %nid', array(
          '!error' => $e->getMessage(),
          '%nid' => $row->nid
        )), WATCHDOG_ERROR);
      }
    }
    return $html;
  }

  /**
   * Copy across a file
   */
  public static function saveFile($src, $uid, $row){
    global $s1_domain, $user;
    $parsed_url = parse_url($src);
    // If this is an external file, leave it alone
    if(isset($parsed_url['host']) && $parsed_url['host'] != $s1_domain){return;}
    $file_name = basename($parsed_url['path']);
    // Resolve any ../ in the path
    if(strpos($parsed_url['path'], '../') !== false){
      $parsed_url['path'] = str_replace('../', '', $parsed_url['path']);
    }
    $source = 'http://' . $s1_domain . '/' . ltrim(str_replace(' ', '%20', $parsed_url['path']), '/');
    $destination = file_default_scheme() . '://' . rawurldecode($file_name);
    if(file_exists($destination)){
      $html_img_src = $destination;
    }else{
      // Aaargh nasty hackery so the created file inherits the UID of the person who created the node
      $user->uid = $uid;
      // Is this a data file?
      if(ScratchpadsNodeMigration::isDataImage($src)){
        // Get the extenion
        $fp = fopen($src, 'r');
        $meta = stream_get_meta_data($fp);
        $extension = str_replace('image/', '', $meta['mediatype']);
        // Generate a unique ID for the file name
        $destination = file_default_scheme() . '://data-' . uniqid() . '.' . $extension;
        $file = file_save_data($fp, $destination, FILE_EXISTS_REPLACE);
        $html_img_src = $file->uri;
      }else if($file_contents = file_get_contents($source)){
        if($file = file_save_data($file_contents, $destination, FILE_EXISTS_REPLACE)){
          $html_img_src = $file->uri;
        }
      }
      // Change back to user 1
      $user->uid = 1;
    }
    if(isset($html_img_src)){
      return ScratchpadsNodeMigration::fileCreateUrl($html_img_src);
    }else{
      scratchpads_migrate_task_insert('node', $row->nid, t('The <a href="!source">file</a>could not be saved, probably because the image is missing on the source site.', array(
        '!source' => $source
      )), t('Check the old site and update the new node accordingly.'));
      watchdog('scratchpads_migrate', t('Could not save file for !source'), array(
        '!source' => $source
      ), WATCHDOG_ERROR);
    }
  }

  /**
   * Is this a data image
   */
  protected function isDataImage($href){
    return strpos($href, 'data:image') !== false;
  }

  /**
   * Create a full path URL for a file (rewrite public:// etc.,)
   * Suitable for an html img src
   */
  public static function fileCreateUrl($file){
    $file = drupal_encode_path($file);
    $url = '/files/' . basename($file);
    return $url;
  }

  /**
   * Over-write DrupalNode6Migration::handle_type_filefield()
   * We want to use the fid, not filename
   */
  public function handle_type_filefield($fid){
    return $fid;
  }

  /**
   * Change style to classes
   * @param string $html
   */
  public static function parseStyles($html){
    // If the text contains no tags, leave it as it is
    if(!preg_match('/<.+?>/', $html)){return $html;}
    $dom = new DomDocument();
    @$dom->loadHTML($html);
    $xpath = new DOMXpath($dom);
    $body_list = $dom->getElementsByTagName('body');
    if($body_list->length == 0){return $html;}
    $body = $body_list->item(0);
    ScratchpadsNodeMigration::parseStylesHelper($dom, $xpath, $body);
    $html = $dom->saveHTML();
    return preg_replace('/(^.*<body>|<\/body>.*$)/s', '', $html);
  }

  public static function parseStylesHelper($dom, $xpath, $xmlElem){
    // Replace styles
    $style = $xmlElem->getAttribute('style');
    if(preg_match('/text-align\s*:\s*center/i', $style)){
      $classes = $xmlElem->getAttribute('class');
      $classes .= ' rtecenter';
      $xmlElem->setAttribute('class', $classes);
    }
    if(preg_match('/float\s*:\s*left/i', $style)){
      $xmlElem->setAttribute('align', 'left');
    }else if(preg_match('/float\s*:\s*right/i', $style)){
      $xmlElem->setAttribute('align', 'right');
    }
    if($xmlElem->nodeName == 'span' && preg_match('/font-style\s*:\s*italic/i', $style)){
      $xmlElem = ScratchpadsNodeMigration::replaceXmlTag($dom, $xmlElem, 'em');
    }else if($xmlElem->nodeName == 'span' && preg_match('/font-weight\s*:\s*bold/i', $style)){
      $xmlElem = ScratchpadsNodeMigration::replaceXmlTag($dom, $xmlElem, 'strong');
    }else if($xmlElem->nodeName == 'h2'){
      if($xpath->query('.//a', $xmlElem)->length > 0){
        $xmlElem = ScratchpadsNodeMigration::replaceXmlTag($dom, $xmlElem, 'h3');
      }
    }else if($xmlElem->nodeName == 'p'){
      $images = $xpath->query('img', $xmlElem);
      if($images->length == 1){
        $image = $images->item(0);
        $image_style = $image->getAttribute('style');
        if(preg_match('/margin-right\s*:\s*auto/', $image_style) && preg_match('/margin-left\s*:\s*auto/', $image_style)){
          $image->removeAttribute('style');
          $classes = $xmlElem->getAttribute('class');
          $classes .= ' rtecenter';
          $xmlElem->setAttribute('class', $classes);
        }
      }
    }
    $xmlElem->removeAttribute('style');
    // Recurse
    for($i = 0; $i < $xmlElem->childNodes->length; $i++){
      $child = $xmlElem->childNodes->item($i);
      if($child->nodeType == 1){
        ScratchpadsNodeMigration::parseStylesHelper($dom, $xpath, $child);
      }
    }
  }

  public static function replaceXmlTag($dom, $elem, $new_tag){
    // We need to copy the item and it's children to replace the tag
    $new_elem = $dom->createElement($new_tag);
    $elem->parentNode->replaceChild($new_elem, $elem);
    for($i = 0; $i < $elem->attributes->length; $i++){
      $attribute = $elem->attributes->item($i);
      $new_elem->setAttribute($attribute->name, $attribute->value);
    }
    $children = array();
    for($i = 0; $i < $elem->childNodes->length; $i++){
      $children[] = $elem->childNodes->item($i);
    }
    foreach($children as $child){
      $new_elem->appendChild($elem->removeChild($child));
    }
    return $new_elem;
  }

  /**
   * Ensures HTML tags are lowercase
   */
  public static function lowercaseHtml($html){
    return preg_replace_callback('/<(\/?)([a-zA-Z]+)(.*?)>/s', array(
      'ScratchpadsNodeMigration',
      '_lowercaseHtmlCallback'
    ), $html);
  }

  public static function _lowercaseHtmlCallback($matches){
    return '<' . $matches[1] . strtolower($matches[2]) . $matches[3] . '>';
  }

  /**
   * Build a basic stub node
   */
  protected function buildStubNode($migration, $nid){
    // We're using VIDs as primary identifiers, need to get the vid for this stub node
    if($vid = scratchpads_migrate_source_get_vid($nid)){
      $node = new stdClass();
      $node->title = t('Stub for !source_id', array(
        '!source_id' => $nid
      ));
      $node->type = $this->destination->getBundle();
      $node->uid = 0;
      $node->status = 0;
      $node->nid = $nid;
      $node->vid = $vid;
      $node->is_new = true;
      // OG requires the original uid
      if(!isset($node->original)){
        $node->original = new StdClass();
      }
      $node->original->uid = $node->uid;
      return $node;
    }
    return false;
  }

  /**
   * Create stubs (for node reference fields)
   */
  protected function createStub($migration){
    // Get source_id from args
    $args = func_get_args();
    $nid = $args[1][0];
    // Check the node doesn't already exist - otherwise as we're passing in nid
    // and create new, we'll get integrity constraint violations
    $node = node_load($nid);
    if($node){
      return array(
        $node->vid
      );
    }else{
      // Check the node exists in the source database
      // There's a lot of nodes still referenced by NIDs which have been deleted
      if(scratchpads_migrate_source_node_exists($nid)){
        $node = $this->buildStubNode($migration, $nid);
        if($node){
          node_save($node);
          // We use VID for the primary identifiers now
          if(isset($node->vid)){return array(
              $node->vid
            );}
        }
      }
      return FALSE;
    }
  }

  /**
   * Override the handleSourceMigration - we want to check against the VID not NID
   */
  protected function handleSourceMigration($source_migrations, $source_keys, $default = NULL, $migration = NULL){
    $is_node = FALSE;
    $test_source_migration = $source_migrations;
    if(is_array($test_source_migration)){
      $test_source_migration = reset($test_source_migration);
    }
    $migration_instance = Migration::getInstance($test_source_migration);
    if(method_exists($migration_instance->destination, 'getEntityType')){
      $is_node = $migration_instance->destination->getEntityType() == 'node';
    }
    if(!$is_node){
      return parent::handleSourceMigration($source_migrations, $source_keys, $default, $migration);
    }else{
      // Because we're mapping to VID not NID, get the VID to perform lookup against
      if(is_array($source_keys)){
        $vid_source_keys = array();
        foreach($source_keys as $source_key){
          // Failsafe: in case this migration source did not map to vids.
          $val = scratchpads_migrate_source_get_vid($source_key);
          $vid_source_keys[] = $val ? $val : $source_key;
        }
      }else{
        $vid_source_keys = scratchpads_migrate_source_get_vid($source_keys);
        $vid_source_keys = $vid_source_keys ? $vid_source_keys : $source_keys;
      }
      parent::handleSourceMigration($source_migrations, $vid_source_keys, $default, $migration);
      // We can safaely return the original source keys - these are NIDs to use in the node reference
      // We use the same NIDs anyway
      return $source_keys;
    }
  }

  /**
   * Override the create stubwrapper - we want to use the VID but this uses the NID
   */
  protected function createStubWrapper(array $source_key, $migration = NULL){
    if(method_exists($this, 'createStub')){
      $destids = $this->createStub($migration, $source_key);
      if($destids){
        $data_row = new stdClass();
        // Use the same VID of the item we've created - this will be the same
        $data_row->vid = reset($destids);
        $this->map->saveIDMapping($data_row, $destids, MigrateMap::STATUS_NEEDS_UPDATE);
      }
    }else{
      $destids = NULL;
    }
    return $destids;
  }

  /**
   * Decode the weight to get stickiness
   */
  public static function weightDecode(&$row){
    if($row->sticky == 0 || $row->sticky == 1){
      $row->node_weight = 0;
      return;
    }
    if($row->sticky > 0){
      $row->node_weight = 100 - $row->sticky;
      $row->sticky = 1;
    }else{
      $row->node_weight = -($row->sticky + 100);
      $row->sticky = 0;
    }
  }
}
