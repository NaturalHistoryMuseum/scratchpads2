<?php

/**
 * Common mappings for the Drupal 6 node migrations.
 */
class ScratchpadsUserMigration extends DrupalUser6Migration{

  public $unmigratedDesinations = array();

  public $profileFields = array();

  public function __construct(array $arguments){
    // Get profile fields
    $this->profileFields = array_merge(DrupalFieldContentProfileMigration::getFieldNameMappings(), DrupalFieldProfileMigration::getFieldNameMappings());
    // Add role names
    $this->sourceFields['role_names'] = t('User role');
    $this->sourceFields['body'] = t('Body');
    $this->addFieldMapping('role_names', 'role_names');
    // Add country (derived from associated location node)
    $this->sourceFields['location_country'] = t("Country (derived from associated location node)");
    $this->addFieldMapping('field_user_country', 'location_country');
    // Handle the profile (drupal core) fields
    // Need to use source connection argument as we're using it before calling
    // __construct
    // This needs to go before profile fields so the field re-writing works
    if(Database::getConnection('default', $arguments['source_connection'])->schema()->tableExists('profile_fields')){
      $query = Database::getConnection('default', $arguments['source_connection'])->select('profile_fields', 'pf');
      $query->addField('pf', 'name', 'field_name');
      $query->addField('pf', 'title', 'label');
      $this->mapFields($query->execute());
    }
    // Handle the CCK profile fields
    $query = Database::getConnection('default', $arguments['source_connection'])->select('content_node_field_instance', 'i');
    $query->fields('i', array(
      'field_name',
      'label'
    ));
    $query->condition('i.widget_active', 1);
    $query->condition('i.type_name', 'profile');
    $this->mapFields($query->execute());
    parent::__construct($arguments);
    // Change the destination to our modified user destination
    $this->destination = new MigrateDestinationScratchpadsUser(array(
      'md5_passwords' => TRUE
    ));
    $this->removeFieldMapping('path');
    if(array_key_exists('field_body', $this->getDestination()->fields())){
      $this->addFieldMapping('field_body', 'body');
      $this->addFieldMapping('field_body:format')->defaultValue('filtered_html');
      $this->addUnmigratedDestinations(array(
        'field_body:summary',
        'field_body:language'
      ));
    }
    // Not all fields have format & title so match unmigrated destinations
    // against desination fields
    $this->unmigratedDesinations = array_intersect($this->unmigratedDesinations, array_keys($this->getDestination()->fields()));
    $this->unmigratedDesinations[] = 'group_audience';
    $this->unmigratedDesinations[] = 'path';
    $this->addUnmigratedDestinations($this->unmigratedDesinations);
    // Path isn't used
    // Add legal accept
    $this->addFieldMapping('legal_accept')->defaultValue(1);
    // Map any fields created
  }

  /**
   * Get a new field name
   */
  protected function getDestinationField($name){
    if(array_key_exists($name, $this->profileFields)){
      return $this->profileFields[$name];
    }else{
      return false;
    }
  }

  /**
   * Map fields from a query result
   */
  protected function mapFields($result){
    foreach($result as $row){
      // Add this to the list of source fields
      $this->sourceFields[$row->field_name] = $row->label;
      // Add a field mapping if we know the new name
      if($field = $this->getDestinationField($row->field_name)){
        // Allow for duplicate destination fields
        $field_mappings = $this->getFieldMappings();
        // Have we already mapped this field?
        // Special use case for email field - do not map
        if(array_key_exists($field, $field_mappings) || $row->field_name == 'field_email'){
          // Already mapped so add as an unmigrated source
          $this->addUnmigratedSources(array(
            $row->field_name
          ));
        }else{
          $this->addFieldMapping($field, $row->field_name);
          $this->unmigratedDesinations[] = $field . ':format';
          $this->unmigratedDesinations[] = $field . ':language';
        }
      }elseif($row->field_name == 'field_imagefield'){
        $this->addFieldMapping('field_media', $row->field_name);
        $this->addFieldMapping('field_media:file_class')->defaultValue('MigrateFileFid');
        $this->addUnmigratedDestinations(array(
          'field_media:language',
          'field_media:description',
          'field_media:display'
        ));
      }else{
        if(substr($row->field_name, 0, 5) == 'field'){
          $field = $row->field_name;
        }else{
          $field = 'field_' . $row->field_name;
        }
        $this->addFieldMapping($field, $row->field_name);
        $this->unmigratedDesinations[] = $field . ':format';
        $this->unmigratedDesinations[] = $field . ':language';
      }
    }
  }

  /**
   * Implementation of Migration::prepareRow().
   */
  public function prepareRow($row){
    if(parent::prepareRow($row) === FALSE){return FALSE;}
    // Add the profiel node
    $content_profile = $this->getProfileNode($row, array(
      'n.uid' => $row->uid
    ));
    if($content_profile){
      $this->updateProfileNids($content_profile['nid']);
    }
    // Get the location node for the content profile - this has the country
    // location
    $query = Database::getConnection('default', $this->sourceConnection)->select('location', 'l');
    $query->innerJoin('location_instance', 'li', 'l.lid = li.lid');
    $query->fields('l', array(
      'country'
    ));
    $query->condition('li.uid', $row->uid);
    $query->range(0, 1);
    if($location = $query->execute()->fetchField()){
      $row->location_country = strtoupper($location);
    }
    // Get the core profile fields
    if(Database::getConnection('default', $this->sourceConnection)->schema()->tableExists('profile_values')){
      $query = Database::getConnection('default', $this->sourceConnection)->select('profile_values', 'pv');
      $query->addField('pv', 'value');
      $query->innerJoin('profile_fields', 'pf', 'pf.fid = pv.fid');
      $query->addField('pf', 'name');
      $query->condition('pv.uid', $row->uid);
      $profile_fields = $query->execute();
      foreach($profile_fields as $profile_field){
        $row->{$profile_field->name} = $profile_field->value;
      }
    }
    // Get the user roles
    $query = Database::getConnection('default', $this->sourceConnection)->select('users_roles', 'ur');
    $query->innerJoin('role', 'r', 'r.rid = ur.rid');
    $query->condition('ur.uid', $row->uid);
    $query->fields('r', array(
      'rid',
      'name'
    ));
    $row->role_names = $query->execute()->fetchAllKeyed();
    if(isset($row->body)){
      $row->body = ScratchpadsNodeMigration::parseImages($row->body);
    }
  }

  public static function getProfileNode(&$row, $conditions){
    // Get the profile node
    $query = Database::getConnection('default', 'scratchpad_1')->select('node', 'n');
    $query->innerJoin('node_revisions', 'nr', 'nr.vid = n.vid');
    $query->addField('nr', 'body');
    $query->innerJoin('content_type_profile', 'p', 'p.vid = n.vid');
    $query->fields('p');
    foreach($conditions as $field => $condition){
      $query->condition($field, $condition);
    }
    $query->range(0, 1);
    $content_profile = $query->execute()->fetchAssoc();
    if($content_profile){
      foreach($content_profile as $field => $value){
        if(!empty($value)){
          $field = str_replace('_value', '', $field);
          // Tidy up some of the field values
          switch($field){
            case 'field_homepage':
              // Homepage link is actually a textarea?
              $value = strip_tags($value);
              break;
            case 'field_title':
              if($value == 'Dr.'){
                $value = 'Dr';
              }
              break;
          }
          $row->{$field} = $value;
        }
      }
    }
    return $content_profile;
  }

  public function updateProfileNids($nid){
    $profile_nids = variable_get('profile_nids', array());
    $profile_nids[] = $nid;
    variable_set('profile_nids', $profile_nids);
  }
}