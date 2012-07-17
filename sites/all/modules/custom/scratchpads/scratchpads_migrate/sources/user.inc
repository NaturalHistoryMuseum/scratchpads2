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
    $this->sourceFields = array(
      'role_names' => t('User role')
    );
    $this->addFieldMapping('role_names', 'role_names');
    // Add country (derived from associated location node)
    $this->sourceFields['location_country'] = t("Country (derived from associated location node)");
    $this->addFieldMapping('field_user_country', 'location_country');
    // Handle the profile (drupal core) fields 
    // Need to use source connection argument as we're using it before calling __construct
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
    // Not all fields have format & title so match unmigrated destinations against desination fields
    $this->unmigratedDesinations = array_intersect($this->unmigratedDesinations, array_keys($this->getDestination()->fields()));
    $this->unmigratedDesinations[] = 'group_audience';
    $this->addUnmigratedDestinations($this->unmigratedDesinations);
    // Path isn't used
    $this->removeFieldMapping('path');
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
    // Get the profile node
    $query = Database::getConnection('default', $this->sourceConnection)->select('node', 'n');
    $query->innerJoin('content_type_profile', 'p', 'p.vid = n.vid');
    $query->fields('p');
    $query->condition('n.uid', $row->uid);
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
    // Get the location node for the content profile - this has the country location
    $query = Database::getConnection('default', $this->sourceConnection)->select('location', 'l');
    $query->innerJoin('location_instance', 'li', 'l.lid = li.lid');
    $query->fields('l', array(
      'country'
    ));
    $query->condition('li.uid', $row->uid);
    $query->range(0, 1);
    $row->location_country = $query->execute()->fetchField();
    // Get the core profile fields
    $query = Database::getConnection('default', $this->sourceConnection)->select('profile_values', 'pv');
    $query->addField('pv', 'value');
    $query->innerJoin('profile_fields', 'pf', 'pf.fid = pv.fid');
    $query->addField('pf', 'name');
    $query->condition('pv.uid', $row->uid);
    $profile_fields = $query->execute();
    foreach($profile_fields as $profile_field){
      $row->{$profile_field->name} = $profile_field->value;
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
  }
}