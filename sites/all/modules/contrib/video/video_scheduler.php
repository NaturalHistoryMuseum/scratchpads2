#!/usr/bin/env php
<?php
/**
 * @file
 * Implement video rendering scheduling.
 * If you are not using sites/default/settings.php as your settings file,
 * add an optional parameter for the drupal site url:
 * "php video_scheduler.php http://example.com/" or
 * "php video_scheduler.php http://example.org/drupal/"
 *
 * @author Heshan Wanigasooriya <heshan at heidisoft dot com, heshanmw at gmail dot com>
 *
 */

/**
 * Drupal shell execution script
 */

$script = basename(array_shift($_SERVER['argv']));
$script_name = realpath($script);
$php_exec = realpath($_SERVER['PHP_SELF']);

$shortopts = 'hr:s:v';
$longopts = array('help', 'root:', 'site:', 'verbose');

$args = @getopt($shortopts, $longopts);

if (isset($args['h']) || isset($args['help'])) {
  echo <<<EOF

Video Scheduler.

Usage:        {$script} [OPTIONS]
Example:      {$script} 

All arguments are long options.

  -h, --help  This page.

  -r, --root  Set the working directory for the script to the specified path.
              To execute Drupal this has to be the root directory of your
              Drupal installation, f.e. /home/www/foo/drupal (assuming Drupal
              running on Unix). Current directory is not required.
              Use surrounding quotation marks on Windows.
  
  -s, --site  Used to specify with site will be used for the upgrade. If no
              site is selected then default will be used.
  
  -v, --verbose This option displays the options as they are set, but will
              produce errors from setting the session.

To run this script without --root argument invoke it from the root directory
of your Drupal installation with

  ./{$script}

\n
EOF;
  if (version_compare(phpversion(), '5.3.0', 'le')) {
    echo "Warning: This version of PHP doesn't support long options\n";
  }
  exit;
}

// define default settings
$_SERVER['HTTP_HOST']       = 'default';
$_SERVER['PHP_SELF']        = '/index.php';
$_SERVER['REMOTE_ADDR']     = '127.0.0.1';
$_SERVER['SERVER_SOFTWARE'] = 'PHP CLI';
$_SERVER['REQUEST_METHOD']  = 'GET';
$_SERVER['QUERY_STRING']    = '';
$_SERVER['PHP_SELF']        = $_SERVER['REQUEST_URI'] = '/index.php';
$_SERVER['SCRIPT_NAME']     = '/' . basename($_SERVER['SCRIPT_NAME']);
$_SERVER['HTTP_USER_AGENT'] = 'console';

// Starting directory
$cwd = realpath(getcwd());

// toggle verbose mode
$_verbose_mode = isset($args['v']) || isset($args['verbose']) ? TRUE : FALSE;

// parse invocation arguments
if (isset($args['r']) || isset($args['root'])) {
  // change working directory
  $path = isset($args['r']) ? $args['r'] : $args['root'];
  if (is_dir($path)) {
    chdir($path);
  }
  else {
    echo "\nERROR: {$path} not found.\n\n";
    exit(1);
  }
}
else {
  $path = $cwd;
  while ($path && !(file_exists($path . '/index.php') && file_exists($path . '/includes/bootstrap.inc'))) {
    $path = dirname($path);
  }
  
  if (!(file_exists($path . '/index.php') && file_exists($path . '/includes/bootstrap.inc'))) {
    echo "Unable to locate Drupal root, user -r option to specify path to Drupal root\n";
    exit(1);
  }
  chdir($path);
}

if (isset($args['s']) || isset($args['site'])) {
  $site = isset($args['s']) ? $args['s'] : $args['site'];
  if (file_exists('./sites/' . $site)) {
    $_SERVER['HTTP_HOST'] = $site;
  }
  else {
    echo "ERROR: Unable to locate site {$site}\n";
    exit(1);
  }
}
else if (preg_match('/' . preg_quote($path . '/sites/', '/') . '(.*?)\//i', $cwd, $matches)) {
  if ($matches[1] != 'all' && file_exists('./sites/' . $matches[1])) {
    $_SERVER['HTTP_HOST'] = $matches[1];
  }
}

define('DRUPAL_ROOT', realpath(getcwd()));

ini_set('display_errors', 0);
include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
ini_set('display_errors', 1);

// turn off the output buffering that drupal is doing by default.
ob_end_flush();

//include our conversion class (also contains our defines)
module_load_include('inc', 'video', 'includes/conversion');
$video_conversion = new video_conversion;
$video_conversion->run_queue();
