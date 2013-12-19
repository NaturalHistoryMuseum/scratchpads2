<?php

class MigrateDestinationScratchpadsProfile extends MigrateDestinationScratchpadsUser{

  public function fields($migration = NULL){
    $fields = array();
    $fields['name'] = t("Name");
    $fields['mail'] = t("Email");
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
    // Ensure we use ISO2 for country fields.
    $country_fields = array();
    $account->field_user_country = trim($account->field_user_country);
    if(property_exists($account, 'field_user_country') && !empty($account->field_user_country)){
      $country_fields[] = $account->field_user_country;
    }
    if(property_exists($row, 'profile_Country') && !empty($row->profile_Country)){
      $country_fields[] = $row->profile_Country;
    }
    $matched = FALSE;
    foreach($country_fields as $country_field_value){
      $known = array(
        'uk' => 'GB',
        'usa' => 'US',
        'us' => 'US',
        'sw' => 'SE',
        'alaska' => 'US',
        'unitedstats' => 'US',
        'unitedstates' => 'US',
        'chicago' => 'US',
        'alabama' => 'US',
        'vietnam' => 'VN',
        'america' => 'US',
        'london' => 'GB',
        'england' => 'GB',
        'dehli' => 'IN',
        'britain' => 'GB',
        'greatbritain' => 'GB',
        'nederland' => 'NL',
        'heredia' => 'CR',
        'lessoto' => 'LS',
        'newyork' => 'US',
        'unitedstate' => 'US',
        'floridausa' => 'US',
        'california' => 'US',
        'kent' => 'GB',
        'germanx' => 'DE',
        'jakrta' => 'ID',
        'xiamen' => 'CN',
        'korea' => 'KR', // Assuming south, no offence meant.
        'berlin' => 'DE',
        'unitedestate' => 'US',
        'kentucky' => 'US',
        'texas' => 'US',
        'philippiness' => 'PH',
        'indiana' => 'US',
        'indonesian' => 'ID',
        'united kingdomn' => 'GB',
        'united sates' => 'GB',
        'florida' => 'US',
        'unitrd states' => 'US',
        'pakistani' => 'PK',
        'ny' => 'US' // Thankfully no country code is 'ny'.
      );
      $stripped_value = preg_replace('/[^a-z]/', '', strtolower($country_field_value));
      if(isset($known[$stripped_value])){
        $matched = TRUE;
        $account->field_user_country = $known[$stripped_value];
        break;
      }
      $match = db_query("
        SELECT iso2
          FROM {countries_country}
         WHERE iso2 = :value
            OR iso3 = :value
            OR name = :value
            OR official_name = :value
      ", array(
        ':value' => $country_field_value
      ))->fetchField();
      if($match){
        $matched = TRUE;
        $account->field_user_country = $match;
        break;
      }
    }
    if(!$matched){
      $account->field_user_country = 'IM'; // As good as anywhere...
    }
    // When a user is created, the related profile has that user's UID
    // Stand-alone profiles created by a user also have the same UID
    // So the only way to identify which is the correct profile is to assume the first one created for a UID is the profile - retarded
    // As this query is ordered by NID, check to see if we've already updated a user
    // If we have remove the UID
    $users_with_profiles = variable_get('migrate_user_profiles', array());
    if($row->uid && !in_array($row->uid, $users_with_profiles) && $user = user_load($row->uid)){
      // Do not overwrite the user email address with one from the profile
      if(isset($user->mail) && !empty($user->mail)){
        unset($account->mail);
      }
      $account->original = $user;
      $account->uid = $user->uid;
      $account->name = $user->name;
      $account->status = $user->status;
      $account->pass = '';
    }else{
      $account->pass = NULL;
      unset($account->uid);
    }
    if(!isset($account->picture)){
      $account->picture = NULL; // Ensure we have value
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