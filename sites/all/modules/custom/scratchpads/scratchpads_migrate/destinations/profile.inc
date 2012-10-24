<?php

class MigrateDestinationScratchpadsProfile extends MigrateDestinationScratchpadsUser{

  public function fields($migration = NULL){
    $fields = array();
    $fields['name'] = t("Name");
    $fields['uid'] = t("User ID");
    $fields['created'] = t("Created");
    $fields['legal_accept'] = t("Legal accept");
    $fields['picture'] = t('User: <a href="@doc">Picture</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#picture'
    ));
    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Entity', 'fields', $this->entityType, $this->bundle, $migration);
    $fields += migrate_handler_invoke_all('User', 'fields', $this->entityType, $this->bundle, $migration);
    return $fields;
  }

  public function import(stdClass $account, stdClass $row){
    // Map some of the duplicate fields
    // Content profile fields always take precedence so overwite existing content 
    foreach(DrupalFieldContentProfileMigration::getFieldNameMappings() as $d6_field => $d7_field){
      if(!empty($row->{$d6_field})){
        $account->{$d7_field} = $row->{$d6_field};
      }
    }
    // When a user is created, the related profile has that user's UID
    // Stand-alone profiles created by a user also have the same UID
    // So the only way to identify which is the correct profile is to assume the first one created for a UID is the profile - retarded
    // As this query is ordered by NID, check to see if we've already updated a user
    // If we have remove the UID
    $users_with_profiles = variable_get('migrate_user_profiles', array());
    if($row->uid && !in_array($row->uid, $users_with_profiles) && $user = user_load($row->uid)){
      $account->original = $user;
      $account->uid = $user->uid;
      $account->name = $user->name;
      $account->status = $user->status;
      $account->pass = '';
    }else{
      $account->pass = NULL;
      unset($account->uid);
    }
    $ret = parent::import($account, $row);
    // Update the var setting we have update this user's profile
    // All subsequent profiles with the same UID will be added as standalone profiles
    if($row->uid){
      $users_with_profiles = variable_get('migrate_user_profiles', array());
      if(!in_array($row->uid, $users_with_profiles)){
        $users_with_profiles[] = $row->uid;
      }
      variable_set('migrate_user_profiles', $users_with_profiles);
    }
    if(isset($row->url_alias)){
      foreach($row->url_alias as $url_alias){
        $path = array(
          'alias' => $url_alias,
          'source' => 'user/' . reset($ret),
          'language' => isset($node->language) ? $node->language : LANGUAGE_NONE
        );
        path_save($path);
      }
    }
    return $ret;
  }

  public function bulkRollback(array $uids){
    // Only delete users who are profile only users
    $users = variable_get('migrate_user_profiles', array());
    $uids = array_diff($uids, $users);
    migrate_instrument_start('user_delete_multiple');
    user_delete_multiple($uids);
    foreach($uids as $uid){
      path_delete(array(
        'source' => 'user/' . $uid
      ));
    }
    migrate_instrument_stop('user_delete_multiple');
    variable_del('migrate_user_profiles');
  }
}