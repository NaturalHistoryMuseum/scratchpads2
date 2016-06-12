<?php
/**
 * @file
 * Class definition of FeedsFieldCollectionProcessor.
 */

/**
 * Creates field collection from feed items.
 */
class FeedsFieldCollectionProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'field_collection_item';
  }
  
  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info += array('label plural' => $info['label']);
    return $info;
  }

  /**
   * Creates a new field collection item in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    //$field_collection_item = entity_create('field_collection_item', array('field_name' => $this->config['field_name'], 'hostEntityType' => $this->config['host_entity_type']));
    //$field_collection_item->setHostEntity($entity_type, $entity, LANGUAGE_NONE, FALSE);
    $field_collection_item = new stdClass();
    $field_collection_item->field_name = $this->config['field_name'];
    $field_collection_item->hostEntityType = $this->config['host_entity_type'];

    return $field_collection_item;
  }

  /**
   * Loads an existing entity.
   *
   * If the update existing method is not FEEDS_UPDATE_EXISTING, only the entity
   * table will be loaded, foregoing the entity_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $id) {
    $result = entity_load($this->entityType(), array($id));
    return reset($result);
  }

  /**
   * Save a entity.
   */
  public function entitysave($entity) {

    $values = (array)$entity;
    $skip_host_save = FALSE;
    $field_collection_item = entity_create('field_collection_item', $values);
	  
    if (!empty($entity->hostEntityId)) {
      $hostEntity_array = entity_load($this->config['host_entity_type'], array($entity->hostEntityId));
      $hostEntity = reset($hostEntity_array);
      $field_collection_item->setHostEntity($this->config['host_entity_type'], $hostEntity);
      //update by set item_id
	 
      if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING && isset($field_collection_item->identifier_field) && isset($this->config['identifier_field_name'])) {
       
	   $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', 'field_collection_item');
        $query->entityCondition('bundle', $this->config['field_name']);
        $items  = isset($hostEntity->{$this->config['field_name']}[LANGUAGE_NONE]) ? $hostEntity->{$this->config['field_name']}[LANGUAGE_NONE] : array();
        $ids = field_collection_field_item_to_ids($items);
		if(!empty($ids)){
          $query->entityCondition('entity_id', $ids, 'IN');
		
          $query->fieldCondition($this->config['identifier_field_name'], 'value', $field_collection_item->identifier_field);
          $result = $query->execute();
		
          list($EntityType, $data) = each($result);
          $field_collection_item->item_id = $data ? key($data) : FALSE;
		}
        if (!empty($field_collection_item->item_id)) {
          $field_collection_item->is_new = NULL;
		  $old_field_collection_item = field_collection_item_load($field_collection_item->item_id);
		  $field_collection_item->revision_id = $old_field_collection_item->revision_id;
          $skip_host_save = TRUE;
        }
        //drupal_set_message('123:'.$field_collection_item->item_id);
      }
	  

    }
	
    $field_collection_item->save($skip_host_save);
  }

  /**
   * Delete a series of entities.
   */
  protected function entityDeleteMultiple($ids) {
    entity_delete_multiple($this->entityType(), $ids);
  }
  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'field_name' => NULL,
      'host_entity_type' => NULL,
      'is_field' => NULL,
      'guid_field_name' => '',
      'identifier_field_name' => '',
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);

    $field_names = array();
    foreach (field_read_fields(array('type' => 'field_collection')) as $field_name => $field) {
      $field_names[$field_name] = $field_name;
    }
    $form['field_name'] = array(
      '#type' => 'select',
      '#title' => t('Field name'),
      '#options' => $field_names,
      '#default_value' => isset($this->config['field_name']) ? $this->config['field_name'] : NULL,
      '#description' => t('The machine-readable name of the field collection field containing this item.'),
    );
    $entity_types = array();
    foreach (entity_get_info() as $entity_type => $entity_info) {
      $entity_types[$entity_type] = $entity_info['label'];
    }

    $form['host_entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Host entity type'),
      '#options' => $entity_types,
      '#default_value' => isset($this->config['host_entity_type']) ? $this->config['host_entity_type'] : NULL,
    );
    $form['is_field'] = array(
      '#type' => 'checkbox',
      '#title' => t('Is field'),
      '#description' => t('A flag that indicate that whether it is a property or field.'),
      '#default_value' => isset($this->config['is_field']) ? $this->config['is_field'] : NULL,
    );
    $form['guid_field_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Field/property name of Host entity GUID'),
      '#description' => t('Machine name of the field/property that used for Host Entity GUID.'),
      '#default_value' => isset($this->config['guid_field_name']) ? $this->config['guid_field_name'] : '',
    );
    $form['identifier_field_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Identifier field name'),
      '#description' => t('Machine name of the identifier field which is unique in host entity.'),
      '#default_value' => isset($this->config['identifier_field_name']) ? $this->config['identifier_field_name'] : '',
    );
    return $form;
  }

  /**
   * Override setTargetElement to operate on a target item that is a entity.
   */
  //public function setTargetElement(FeedsSource $source, $target_item, $target_element, $value, $mapping) {
  public function setTargetElement(FeedsSource $source, $target_item, $target_element, $value) {
    
    switch ($target_element) {
      case 'host_entity_guid':
        $query = new EntityFieldQuery();
        $query->entityCondition('entity_type', $this->config['host_entity_type']);
        if (!empty($this->config['is_field'])) {
          $query->fieldCondition($this->config['guid_field_name'], 'value', $value);
        }
        else{
          $query->propertyCondition($this->config['guid_field_name'], $value);
        }
        $result = $query->execute();
        list($hostEntityType, $data) = each($result);
        $target_item->hostEntityId = $data ? key($data) : FALSE;
        break;
      default:
        parent::setTargetElement($source, $target_item, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
     $targets = parent::getMappingTargets();

    // Add general GUID target
    $targets['guid'] = array(
      'name' => t('GUID'),
      'description' => t('The globally unique identifier of the item. E. g. the feed item GUID in the case of a syndication feed. May be unique.'),
      'optional_unique' => TRUE,
    );

    // Add hostEntity GUID target
    $targets['host_entity_guid'] = array(
      'name' => t('Host Entity GUID'),
      'description' => t('The globally unique identifier of the host entity. Must be unique. We used this value to fetch hostEntityId.'),
    );

    // Add identifier field target
    $targets['identifier_field'] = array(
      'name' => t('Identifier Field'),
      'description' => t("The field collection item's identifier field within host entity. We used this value to fetch item id."),
    );

    $targets['item_id'] = array(
      'name' => t('Item ID'),
      'description' => t('The item id of the field collection. NOTE: use this feature with care, field collection item ids are usually assigned by Drupal.'),
      'optional_unique' => TRUE,
    );

    // Let other modules expose mapping targets.
    self::loadMappers();
    $entity_type = $this->entityType();
    $bundle = $this->config['field_name'];
    drupal_alter('feeds_processor_targets', $targets, $entity_type, $bundle);

    return $targets;
  }
}