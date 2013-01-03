<?php
/**
 * @file
 * Provide views data and handlers for twitter.module
 */

/**
 * @defgroup views_twitter_module twitter.module handlers
 *
 * Includes the ability to create views of just the twitter table.
 * @{
 */


/**
 * Implements hook_views_data().
 */
function twitter_views_data() {
  // Basic table information.

  $data['twitter']['table']['group']  = t('Twitter');

  // Advertise this table as a possible base table
  $data['twitter']['table']['base'] = array(
    'field' => 'twitter_id',
    'title' => t('Twitter message'),
    'help' => t('Twitter status messages.'),
    'weight' => 10,
  );

  // Tweet ID
  $data['twitter']['twitter_id'] = array(
    'title' => t('Twitter status message ID'),
    'help' => t('The ID of the Twitter status message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Twitter reply-to-status id
  $data['twitter']['in_reply_to_status_id'] = array(
    'title' => t('In reply to status ID'),
    'help' => t('The ID of the Twitter status this message is replying to.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter reply-to-status screen name
  $data['twitter']['in_reply_to_screen_name'] = array(
    'title' => t('In reply to user name'),
    'help' => t('The login account of the Twitter user this message is replying to.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Twitter screen name
  $data['twitter']['screen_name'] = array(
    'title' => t('Login name'),
    'help' => t('The login account of the Twitter user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Twitter message timestamp
  $data['twitter']['created_time'] = array(
    'title' => t('Created time'),
    'help' => t('The time the Twitter message was posted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Twitter text
  $data['twitter']['text'] = array(
    'title' => t('Message text'),
    'help' => t('The text of the Twitter message.'),
    'field' => array(
      'handler' => 'twitter_views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Twitter source
  $data['twitter']['source'] = array(
    'title' => t('Source'),
    'help' => t('The name of the application that posted the Twitter message.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );




  $data['twitter_account']['table']['group']  = t('Twitter');

  $data['twitter_account']['table']['join'] = array(
    'twitter' => array(
      'left_field' => 'screen_name',
      'field' => 'screen_name',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      //      'left_table' => 'twitter_user',
    ),
  );

  // Twitter screen name
  $data['twitter_account']['screen_name'] = array(
    'title' => t('Login name'),
    'help' => t('The login account of the Twitter user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );


  // Twitter account full name
  $data['twitter_account']['name'] = array(
    'title' => t('Full name'),
    'help' => t('The full name Twitter account user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Twitter account description
  $data['twitter_account']['description'] = array(
    'title' => t('Description'),
    'help' => t('The description of the Twitter account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter account location
  $data['twitter_account']['location'] = array(
    'title' => t('Location'),
    'help' => t('The location of the Twitter account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter account description
  $data['twitter_account']['followers_count'] = array(
    'title' => t('Followers'),
    'help' => t('The number of users following this Twitter account.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter account profile image
  $data['twitter_account']['profile_image_url'] = array(
    'title' => t('Profile image'),
    'help' => t('The image used by the Twitter account.'),
    'field' => array(
      'handler' => 'twitter_views_handler_field_profile_image',
      'click sortable' => TRUE,
    ),
  );

  // Twitter account url
  $data['twitter_account']['url'] = array(
    'title' => t('URL'),
    'help' => t('The URL given by the Twitter account user.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter account protected
  $data['twitter_account']['protected'] = array(
    'title' => t('Protected status'),
    'help' => t('Whether posts from this Twitter account should be visible to the general public.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter message timestamp
  $data['twitter_account']['last_refresh'] = array(
    'title' => t('Last refresh'),
    'help' => t('The time the Twitter account statuses were retrieved.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Twitter account description
  $data['twitter_account']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The UID of the Twitter account.'),
  );
  // Twitter account protected
  $data['twitter_account']['import'] = array(
    'title' => t('Import status'),
    'help' => t('Whether posts from this Twitter account should be imported automatically.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function twitter_views_data_alter(&$data) {
  $data['users']['table']['join']['twitter'] = array(
    'left_table' => 'twitter_account',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  $data['users']['table']['join']['twitter_account'] = array(
    //    'left_table' => 'twitter_user',
    'left_field' => 'uid',
    'field' => 'uid',
  );
}

/**
 * @}
 */
