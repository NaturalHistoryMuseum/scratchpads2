<?php
/*
* @copyright Copyright (c) 2003-2012, CKSource - Frederico Knabben. All rights reserved.
* @license	 Licensed under CKEditor for Drupal License Agreement, version 1.0. For licensing, see http://ckeditor.com/ckeditor-for-drupal/license
*/

/*
 * Function to check package license
 */
  function ckeditor_check_package_license($license_key, $license_name)
  {
    $ckfinder_key = null;
    $auth = false;
    if (strlen($license_key) == 49)
    {
      $ckfinder_key = substr($license_key, -34, 34);
      $package_key = substr($license_key, 0, 15);
      $auth = ckeditor_check_key($package_key, $license_name);
    }
   return array('authenticate' => $auth, 'key' => $ckfinder_key);
  }
  /*
  * OMG it's so simple :O
  * Simply change to "return true" to get rid of the demo message (...ehm what?).
  * ...wait! If you don't need the commercial license, you can download the Open Source version
  * of this extension ;)
  */
  function ckeditor_check_key($key, $license_name)
  {

    if (strlen($key) == 15 && ($key[0] == 'J' || $key[0] == 'K') && $key[13] == 'K' && ((int)(ord($key[10]).ord($key[11])) % 3 == 0) && ((int)(ord($key[6]).ord($key[7])) % 2 == 0) && $key[6] != '0' && $key[7] != '0'  && $key[10] != '0' && $key[11] != '0' )
    {
      if ($key[0] == 'K')
      {
          drupal_set_message(t('Developer License to: <strong>!licensed</strong>', array('!licensed' => $license_name)), 'status');
      }
      return true;
    }
    return false;
  }