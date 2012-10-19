<?php

class MigrateDestinationScratchpadsUser extends MigrateDestinationUser{

  public function import(stdClass $account, stdClass $row){
    scratchpads_migrate_unset_empty_fields($account, array(
      'legal_accept',
      'pass',
      'picture'
    ));
    // Max account name length 60 chars
    $account->name = substr($account->name, 0, 60);
    if(!isset($account->original)){
      $account->original = $account;
      $account->is_new = true;
    }
    return parent::import($account, $row);
  }

  public function fields($migration = NULL){
    $fields = array();
    // First the core (users table) properties
    $fields['uid'] = t('User: <a href="@doc">Existing user ID</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#uid'
    ));
    $fields['mail'] = t('User: <a href="@doc">Email address</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#mail'
    ));
    $fields['name'] = t('User: <a href="@doc">Username</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#name'
    ));
    $fields['pass'] = t('User: <a href="@doc">Password (plain text)</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#pass'
    ));
    $fields['status'] = t('User: <a href="@doc">Status</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#status'
    ));
    $fields['created'] = t('User: <a href="@doc">Registered timestamp</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#created'
    ));
    $fields['access'] = t('User: <a href="@doc">Last access timestamp</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#access'
    ));
    $fields['login'] = t('User: <a href="@doc">Last login timestamp</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#login'
    ));
    $fields['roles'] = t('User: <a href="@doc">Role IDs</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#roles'
    ));
    $fields['role_names'] = t('User: <a href="@doc">Role Names</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#role_names'
    ));
    $fields['picture'] = t('User: <a href="@doc">Picture</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#picture'
    ));
    $fields['signature'] = t('User: <a href="@doc">Signature</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#signature'
    ));
    $fields['signature_format'] = t('User: <a href="@doc">Signature format</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#signature_format'
    ));
    $fields['timezone'] = t('User: <a href="@doc">Timezone</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#timezone'
    ));
    $fields['language'] = t('User: <a href="@doc">Language</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#language'
    ));
    $fields['theme'] = t('User: <a href="@doc">Default theme</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#theme'
    ));
    $fields['init'] = t('User: <a href="@doc">Init</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#init'
    ));
    $fields['is_new'] = t('Option: <a href="@doc">Indicates a new user with the specified uid should be created</a>', array(
      '@doc' => 'http://drupal.org/node/1349632#is_new'
    ));
    $fields['legal_accept'] = t("Legal accept");
    $fields['data'] = t("Data");
    // This is just a basic user being created
    // Fields are added in the profile migration so the file dependecies can be resolved
    return $fields;
  }
}