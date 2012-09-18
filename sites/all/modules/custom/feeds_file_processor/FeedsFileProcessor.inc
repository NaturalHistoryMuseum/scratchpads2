<?php

/**
 * @file
 * Class definition of FeedsNodeProcessor.
 */
/**
 * Creates nodes from feed items.
 */
class FeedsFileProcessor extends FeedsProcessor{

  /**
   * Define entity type.
   */
  public function entityType(){
    return 'file';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo(){
    $info = parent::entityInfo();
    $info['label plural'] = t('Files');
    return $info;
  }

  /**
   * Creates a new node in memory and returns it.
   */
  protected function newEntity(FeedsSource $source){
    $file = new stdClass();
    $file->type = $this->config['bundle'];
    $file->changed = REQUEST_TIME;
    $file->created = REQUEST_TIME;
    $file->language = LANGUAGE_NONE;
    // Populate properties that are set by node_object_prepare().
    $file->log = 'Created by FeedsNodeProcessor';
    $file->uid = $this->config['author'];
    return $file;
  }

  /**
   * Loads an existing node.
   *
   * If the update existing method is not FEEDS_UPDATE_EXISTING, only the node
   * table will be loaded, foregoing the node_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $fid){
    return entity_load_single('file', $fid);
  }

  /**
   * Save a node.
   */
  public function entitySave($entity){
    entity_save('file', $entity);
  }

  /**
   * Delete a series of nodes.
   */
  protected function entityDeleteMultiple($fids){
    file_delete_multiple($fids);
  }

  /**
   * Implement expire().
   * 
   * I am not sure whether or not this actually makes sense to even be here. 
   * Why would we expire file data?
   *
   * @todo: move to processor stage?
   */
  public function expire($time = NULL){
    if($time === NULL){
      $time = $this->expiryTime();
    }
    if($time == FEEDS_EXPIRE_NEVER){return;}
    $count = $this->getLimit();
    $files = db_query_range("SELECT f.fid FROM {file_managed} f JOIN {feeds_item} fi ON fi.entity_type = 'file' AND f.nid = fi.entity_id WHERE fi.id = :id AND f.timestamp < :created", 0, $count, array(
      ':id' => $this->id,
      ':created' => REQUEST_TIME - $time
    ));
    $fids = array();
    foreach($files as $file){
      $fids[$file->fid] = $file->fid;
    }
    $this->entityDeleteMultiple($fids);
    if(db_query_range("SELECT 1 FROM {file_managed} f JOIN {feeds_item} fi ON fi.entity_type = 'node' AND f.fid = fi.entity_id WHERE fi.id = :id AND n.timestamp < :created", 0, 1, array(
      ':id' => $this->id,
      ':created' => REQUEST_TIME - $time
    ))->fetchField()){return FEEDS_BATCH_ACTIVE;}
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Return expiry time.
   */
  public function expiryTime(){
    return $this->config['expire'];
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults(){
    return array(
      'bundle' => 'image',
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state){
    $info = entity_get_info('file');
    $types = array();
    foreach($info['bundles'] as $key => $value){
      $types[$key] = check_plain($value['label']);
    }
    $form = parent::configForm($form_state);
    unset($form['input_format']);
    $form['bundle'] = array(
      '#type' => 'select',
      '#title' => t('File type'),
      '#description' => t('Select the file type for the files to be created. <strong>Note:</strong> Users with "import !feed_id feeds" permissions will be able to <strong>import</strong> files of the type selected here regardless of the file level permissions. Further, users with "clear !feed_id permissions" will be able to <strong>delete</strong> imported files regardless of their file level permissions.', array(
        '!feed_id' => $this->id
      )),
      '#options' => $types,
      '#default_value' => $this->config['content_type']
    );
    $period = drupal_map_assoc(array(
      FEEDS_EXPIRE_NEVER,
      3600,
      10800,
      21600,
      43200,
      86400,
      259200,
      604800,
      2592000,
      2592000 * 3,
      2592000 * 6,
      31536000
    ), 'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire nodes'),
      '#options' => $period,
      '#description' => t('Select after how much time nodes should be deleted. The node\'s published date will be used for determining the node\'s age, see Mapping settings.'),
      '#default_value' => $this->config['expire']
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing files',
      FEEDS_REPLACE_EXISTING => 'Replace existing files',
      FEEDS_UPDATE_EXISTING => 'Update existing files (slower than replacing them)'
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values){
    if($author = user_load_by_name($values['author'])){
      $values['author'] = $author->uid;
    }else{
      $values['author'] = 0;
    }
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values){
    if($this->config['expire'] != $values['expire']){
      feeds_reschedule($this->id);
    }
    parent::configFormSubmit($values);
  }

  /**
   * Override setTargetElement to operate on a target item that is a node.
   * 
   * I have no idea what this function is actually doing.  Need to check if it
   * is actually required.
   * 
  public function setTargetElement(FeedsSource $source, $target_node, $target_element, $value){
    switch($target_element){
      case 'created':
        $target_node->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;
      case 'feeds_source':
        // Get the class of the feed node importer's fetcher and set the source
        // property. See feeds_node_update() how $file->feeds gets stored.
        if($id = feeds_get_importer_id($this->config['bundle'])){
          $class = get_class(feeds_importer($id)->fetcher);
          $target_node->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_node_insert().
          $target_node->feeds['suppress_import'] = TRUE;
        }
        break;
      default:
        parent::setTargetElement($source, $target_node, $target_element, $value);
        break;
    }
  }
   */
  /**
   * Return available mapping targets.
   */
  public function getMappingTargets(){
    $type = node_type_get_type($this->config['bundle']);
    $targets = parent::getMappingTargets();
    $targets['filename'] = array(
      'name' => t('Filename'),
      'description' => t('The title of the node.'),
      'optional_unique' => TRUE
    );
    $targets['fid'] = array(
      'name' => t('File ID'),
      'description' => t('The fid of the file. NOTE: use this feature with care, file ids are usually assigned by Drupal.'),
      'optional_unique' => TRUE
    );
    $targets['uid'] = array(
      'name' => t('User ID'),
      'description' => t('The Drupal user ID of the file author.')
    );
    $targets['status'] = array(
      'name' => t('Published status'),
      'description' => t('Whether a file is published or not. 1 stands for published, 0 for not published.')
    );
    $targets['timestamp'] = array(
      'name' => t('File timestamp'),
      'description' => t('The UNIX time when a file was created.')
    );
    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'file', $this->config['bundle']);
    return $targets;
  }

  /**
   * Get fid of an existing feed item file if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result){
    if($fid = parent::existingEntityId($source, $result)){return $fid;}
    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach($this->uniqueTargets($source, $result) as $target => $value){
      switch($target){
        case 'fid':
          $nid = db_query("SELECT fid FROM {file_managed} WHERE fid = :fid", array(
            ':fid' => $value
          ))->fetchField();
          break;
        case 'filename':
          $nid = db_query("SELECT fid FROM {file_managed} WHERE filename = :filename", array(
            ':filename' => $value
          ))->fetchField();
          break;
        case 'feeds_source':
          // FIXME Does this need updating?  I'm not sure!
          if($id = feeds_get_importer_id($this->config['bundle'])){
            $fid = db_query("SELECT fs.feed_nid FROM {node} n JOIN {feeds_source} fs ON n.nid = fs.feed_nid WHERE fs.id = :id AND fs.source = :source", array(
              ':id' => $id,
              ':source' => $value
            ))->fetchField();
          }
          break;
      }
      // Return the first fid found.
      if($fid){return $fid;}
    }
    return 0;
  }
}
