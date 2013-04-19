<?php

/**
 * @file
 * Hooks provided by the Simplenews module.
 */

/**
 * @mainpage Simplenews API documentation.
 *
 * Simplenews builds on the following basic concepts.
 *
 * @link subscriber Subscribers @endlink subscribe to @link newsletter
 * newsletters (categories) @endlink. That connection is called
 * a @link subscription subscription @endlink. Nodes of enabled content types
 * are @link issue newsletter issues @endlink. These are then sent to the
 * subscribers of the newsletter the issue is attached to.
 *
 * Sending is done by first adding a row for each subscriber to the @link spool
 * mail spool @endlink.
 * Then they are processed either immediatly or during cron runs. The actual
 * sending happens through a @link source source instance @endlink, which is
 * first instanciated based on the mail spool and then used to generated the
 * actual mail content.
 *
 */

/**
 * @defgroup subscriber Subscriber
 *
 * @todo
 */

/**
 * @defgroup newsletter Newsletter (category)
 *
 * @todo
 */

/**
 * @defgroup subscription Subscription
 *
 * @todo
 */

/**
 * @defgroup issue Newsletter issue
 *
 * @todo
 */

/**
 * @defgroup spool Mail spool
 *
 * @todo
 */

/**
 * @defgroup source Source
 *
 * @todo
 */

/**
 * Return operations to be applied to newsletter issues.
 *
 * @ingroup issue
 */
function hook_simplenews_issue_operations() {
  $operations = array(
    'activate' => array(
      'label' => t('Send'),
      'callback' => 'simplenews_issue_send',
    ),
  );
  return $operations;
}

/**
 * Return operations to be applied to subscriptions.
 *
 * @ingroup issue
 */
function hook_simplenews_subscription_operations() {
  $operations = array(
    'activate' => array(
      'label' => t('Activate'),
      'callback' => 'simplenews_subscription_activate',
      'callback arguments' => array(SIMPLENEWS_SUBSCRIPTION_ACTIVE),
    ),
    'inactivate' => array(
      'label' => t('Inactivate'),
      'callback' => 'simplenews_subscription_activate',
      'callback arguments' => array(SIMPLENEWS_SUBSCRIPTION_INACTIVE),
    ),
    'delete' => array(
      'label' => t('Delete'),
      'callback' => 'simplenews_subscription_delete_multiple',
    ),
  );
  return $operations;
}

/**
 * Act after a newsletter has been spooled.
 *
 * @param $node
 *   The node that has just been spooled.
 */
function hook_simplenews_spooled($node) {

}

/**
 * Act after a newsletter category has been saved.
 *
 * @ingroup newsletter
 */
function hook_simplenews_category_update($category) {

}

/**
 * Act after a newsletter category has been deleted.
 *
 * @ingroup newsletter
 */
function hook_simplenews_category_delete($category) {

}

/**
 * Act after a subscriber is updated.
 *
 * @ingroup subscriber
 */
function hook_simplenews_subscriber_update($subscriber) {

}

/**
 * Act after a new subscriber has been created.
 *
 * @ingroup subscriber
 */
function hook_simplenews_subscriber_insert($subscriber) {

}

/**
 * Act after a subscriber has been deleted.
 *
 * @ingroup subscriber
 */
function hook_simplenews_subscriber_delete($subscriber) {

}

/**
 * Invoked if a user is subscribed to a newsletter.
 *
 * @param $subscriber
 *   The subscriber object including all subscriptions of this user.
 *
 * @param $subscription
 *   The subscription object for this specific subscribe action.
 *
 * @ingroup subscriber
 */
function hook_simplenews_subscribe_user($subscriber, $subscription) {

}

/**
 * Invoked if a user is unsubscribed from a newsletter.
 *
 * @param $subscriber
 *   The subscriber object including all subscriptions of this user.
 *
 * @param $subscription
 *   The subscription object for this specific unsubscribe action.
 *
 * @ingroup subscriber
 */
function hook_simplenews_unsubscribe_user($subscriber, $subscription) {

}

/**
 * Expose SimplenewsSource cache implementations.
 *
 * @return
 *   An array keyed by the name of the class that provides the implementation,
 *   the array value consists of another array with the keys label and
 *   description.
 *
 * @ingroup source
 */
function hook_simplenews_source_cache_info() {
  return array(
    'SimplenewsSourceCacheNone' => array(
      'label' => t('No caching'),
      'description' => t('This allows to theme each newsletter separately.'),
    ),
    'SimplenewsSourceCacheBuild' => array(
      'label' => t('Cached content source'),
      'description' => t('This caches the rendered content to be sent for multiple recipients. It is not possible to use subscriber specific theming but tokens can be used for personalization.'),
    ),
  );
}
