<?php

/**
 * Callback that is the core of this module.  Here we'll decode the data sent to
 * us, check it, and then create a user (if required), and log the user in.  We
 * can also redirect to a set page if required.
 */
function uwho_site_callback(){
  // First, we need to get the key that has been set, if no key is set, we 
  // panic, redirect to the front page, and display an error.
  $key = variable_get('uwho_site_key', FALSE);
  $url = url('uwho', array(
    'absolute' => TRUE
  ));
  //echo substr($_GET['q'], 5);exit;
  // If one of the essential settings hasn't been set, or we're not logged in!
  if(!$key){
    drupal_set_message('No key has been set, you can not login.  Please contact this site\'s maintainer');
    drupal_goto();
  }
  // Load the class file
  module_load_include('inc', 'uwho_site', 'uwho.cryptastic');
  $cryptastic = new cryptastic();
  $key = $cryptastic->pbkdf2($key, $key . $url, 1000, 32);
  if(!isset($_POST['uwhodata']) && isset($_GET['uwhodata'])){
    // Most likely someone without JS clicked the link. We'll be nice, and log
    // them in if the data is in the request
    // Note, $_GET is already decoded, so no need to decode it.
    $decrypted = $cryptastic->decrypt($_GET['uwhodata'], $key);
  } else {
    $decrypted = $cryptastic->decrypt(urldecode($_POST['uwhodata']), $key);
  }
  if(!$decrypted){
    // We've failed to decrypt the stuff, doh!  Error, and redirect to the front
    // page.
    drupal_set_message('Failed to decrypt your data.  Please inform a maintainer', 'error');
    watchdog('uwho', 'Decryption failed');
    drupal_goto();
  }
  // Lets make sure this user hasn't been created before.  We'll use the guid
  // that has been sent to check this.  If they haven't been created, we'll 
  // create a new user.
  $uid = db_result(db_query("SELECT uid FROM {uwho} WHERE guid = '%s'", $decrypted['guid']));
  global $user;
  if($uid && $uid > 1){ // Sanity check to ensure this won't login a user as admin 
    $user = user_load(array('uid' => $uid));
  } else {
    $new_user = new stdClass();
    $new_user->name = $decrypted['user']['name'];
    $new_user->pass = md5(rand());
    $new_user->mail = $decrypted['user']['mail'];
    $new_user->status = 1;
    $new_user->url = $decrypted['user']['url'];
    if(db_result(db_query("SELECT COUNT(*) FROM {users} WHERE name = '%s'", $new_user->name))){
      // We've got a username conflict most likely.
      // Lets change the username and add a random int on the end.  We'll only 
      // try this 5 times, if it is still failing at that point, it's most 
      // likely we've got other issues.
      $i = 0;
      while(db_result(db_query("SELECT COUNT(*) FROM {users} WHERE name = '%s'", $new_user->name)) && $i < 5){
        $new_user->name .= rand(0, 9);
        $i ++;
      }
    }
    // Assume we're fine here (the odds are low that we've failed.
    $user = user_save($new_user, (array)$new_user);
    db_query("INSERT INTO {uwho} (guid, uid, data) VALUES ('%s', %d, '%s')", $decrypted['guid'], $user->uid, serialize($decrypted));
  }
  drupal_set_message(t('You have been logged in as "!user_name"', array('!user_name' => $user->name)));
  // If we're using Boost, we should run init again, as we've cheated a little
  // here, and we're now logged in.
  if(function_exists('boost_init')){
    boost_init();
  }
  if(isset($decrypted['redirect']) && $decrypted['redirect']){
    drupal_goto($decrypted['redirect']);
  }
  drupal_goto(variable_get('uwho_site_redirect_path', ''));
}