<?php

/**
 * @file
 * Sample file for handling redirection from old to new URIs. Use an Apache
 * rewrite rule (or equivalent) to map legacy requests to this file. To use,
 *   copy or symlink this file to the root of your drupal site. Customize this
 *   file to your needs.
 *
 * CREATE TABLE `migrate_source_uri_map` (
 * `source_uri` varchar(255) NOT NULL DEFAULT '',
 * `migration_name` varchar(255) NOT NULL,
 *  `source_id` int(11) NOT NULL, -- can be varchar for some migrations
 * PRIMARY KEY (`source_uri`)
 * )
 *
 */

// For security, this script is disabled by default.
die('Comment out this line when you are ready to use this script');

// Based on custom patterns, build the destination_uri for given source_uri
function migrate_build_url($destid1, $migration_name) {
  global $base_url;

  // TODO: Add an entry for each migration that we need to redirect.
  $patterns = variable_get('migrate_patterns', array(
    'BeerTerm' => 'taxonomy/term/:source_id',
    'BlogEntries' => 'node/:source_id',
    'Slideshows' => 'node/:source_id',
    'TagTerm' => 'taxonomy/term/:source_id',
  ));
  $pattern = $patterns[$migration_name];

  // Swap in the destination ID.
  $destination_uri = str_replace(':source_id', $destid1, $pattern);

  // For speed, we go right to aliases table rather than more bootstrapping.
  if ($uri_clean = db_query("SELECT alias FROM {url_alias} WHERE source = :destination_uri", array(':destination_uri' => $destination_uri))->fetchField()) {
    $destination_uri = $uri_clean;
  }

  // Build absolute url for 301 redirect.
  return $base_url . '/' . $destination_uri;
}

define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';

// Only bootstrap to DB so we are as fast as possible. Much of the Drupal API
// is not available to us.
drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

// You must populate this querystring param from a rewrite rule or $_SERVER
// On Apache, we could likely use _SERVER['REDIRECT_URL']. nginx?
if (!$source_uri = $_GET['migrate_source_uri']) {
  print '$_GET[migrate_source_uri] was not found on the request.';
  exit();
}

// This is a tall table mapping legacy URLs to source_id and migration_name.
// If you can already know the migration name and source_id based on the URI,
// then the first lookup is not needed.
$uri_table = variable_get('migrate_source_uri_table', 'migrate_source_uri_map');

if ($uri_map = db_query("SELECT migration_name, source_id FROM $uri_table WHERE source_uri = :source_uri", array(':source_uri' => $source_uri))->fetchObject()) {
  // Hurray, we do recognize this URI.
  // Consult migrate_map_x table to determine corresponding Drupal nid/tid/cid/etc.
  $map_table = 'migrate_map_' . drupal_strtolower($uri_map->migration_name);
  $sql = "SELECT destid1 FROM $map_table WHERE sourceid1 = :source_id";
  if ($destid1 = $migrate_map = db_query($sql, array(':source_id' => $uri_map->source_id))->fetchField()) {
    // Hurray. We already migrated this content. Go there.
    header('Location: ' . migrate_build_url($destid1, $uri_map->migration_name), TRUE, 301);
  }
  else {
    // We recognize URI but don't have the content in Drupal. Very unlikely.
  }
}
else {
  // Can't find the source URI. TODO: Make nice 404 page.
  header('Status=Not Found', TRUE, 404);
  print 'Sorry folks. Park is closed.';
}
