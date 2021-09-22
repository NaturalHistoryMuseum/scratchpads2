## IMPORTANT NOTE ##

This file contains installation instructions for the 7.x-1.x version of the
Drupal Memcache module. Configuration differs between 7.x and 6.x versions
of the module, so be sure to follow the 6.x instructions if you are configuring
the 6.x-1.x version of this module!

## REQUIREMENTS ##

- PHP 5.1 or greater
- Availability of a memcached daemon: http://memcached.org/
- One of the two PECL memcache packages:
  - http://pecl.php.net/package/memcache (recommended)
  - http://pecl.php.net/package/memcached (latest versions require PHP 5.2 or
    greater)

## INSTALLATION ##

These are the steps you need to take in order to use this software. Order
is important.

 1. Install the memcached binaries on your server and start the memcached
    service. Follow best practices for securing the service; for example,
    lock it down so only your web servers can make connections.
 2. Install your chosen PECL memcache extension -- this is the memcache client
    library which will be used by the Drupal memcache module to interact with
    the memcached server(s). Generally PECL memcache (3.0.6+) is recommended,
    but PECL memcached (2.0.1+) also works well for some people. There are
    known issues with older version.
 3. Put your site into offline mode.
 4. Download and install the memcache module.
 5. If you have previously been running the memcache module, run update.php.
 6. Optionally edit settings.php to configure the servers, clusters and bins
    for memcache to use. If you skip this step the Drupal module will attempt to
    talk to the memcache server on port 11211 on the local host, storing all
    data in a single bin. This is sufficient for most smaller, single-server
    installations.
 7. Edit settings.php to make memcache the default cache class, for example:
      $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
      $conf['cache_default_class'] = 'MemCacheDrupal';
    The cache_backends path needs to be adjusted based on where you installed
    the module.
 8. Make sure the following line also exists, to ensure that the special
    cache_form bin is assigned to non-volatile storage:
      $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
 9. Optionally also add the following two lines to tell Drupal not to bootstrap
    the database when serving cached pages to anonymous visitors:
      $conf['page_cache_without_database'] = TRUE;
      $conf['page_cache_invoke_hooks'] = FALSE;
    If setting page_cache_without_database to TRUE, you also have to set
    page_cache_invoke_hooks to FALSE or you'll see an error like "Fatal error:
    Call to undefined function module_list()".
10. Bring your site back online.

For more detailed instructions on (1) and (2) above, please see the
documentation online on drupal.org which includes links to external
walk-throughs for various operating systems.

## Advanced Configuration ##

This module is capable of working with one memcached instance or with multiple
memcached instances run across one or more servers. The default is to use one
server accessible on localhost port 11211. If that meets your needs, then the
configuration settings outlined above are sufficient for the module to work.
If you want to use multiple memcached instances, or if you are connecting to a
memcached instance located on a remote machine, further configuration is
required.

The available memcached servers are specified in $conf in settings.php. If you
do not specify any servers, memcache.inc assumes that you have a memcached
instance running on localhost:11211. If this is true, and it is the only
memcached instance you wish to use, no further configuration is required.

If you have more than one memcached instance running, you need to add two arrays
to $conf; memcache_servers and memcache_bins. The arrays follow this pattern:

'memcache_servers' => array(
  server1:port => cluster1,
  server2:port => cluster2,
  serverN:port => clusterN,
  'unix:///path/to/socket' => clusterS
)

'memcache_bins' => array(
   bin1 => cluster1,
   bin2 => cluster2,
   binN => clusterN,
   binS => clusterS
)

The bin/cluster/server model can be described as follows:

- Servers are memcached instances identified by host:port.

- Clusters are groups of servers that act as a memory pool. Each cluster can
  contain one or more servers.

- Bins are groups of data that get cached together and map 1:1 to the $table
  parameter of cache_set(). Examples from Drupal core are cache_filter and
  cache_menu. The default is 'cache'.

- Multiple bins can be assigned to a cluster.

- The default cluster is 'default'.

## LOCKING ##

The memcache-lock.inc file included with this module can be used as a drop-in
replacement for the database-mediated locking mechanism provided by Drupal
core. To enable, define the following in your settings.php:

  $conf['lock_inc'] = 'sites/all/modules/memcache/memcache-lock.inc';

Locks are written in the 'semaphore' table, which will map to the 'default'
memcache cluster unless you explicitly configure a 'semaphore' cluster.

## STAMPEDE PROTECTION ##

Memcache includes stampede protection for rebuilding expired and invalid cache
items.  To enable stampede protection, define the following in settings.php:

  $conf['memcache_stampede_protection'] = TRUE;

To avoid lock stampedes, it is important that you enable the memcache lock
implementation when enabling stampede protection -- enabling stampede protection
without enabling the Memcache lock implementation can cause worse performance and
can result in dropped locks due to key-length truncation.

Memcache stampede protection is primarily designed to benefit the following
caching pattern: a miss on a cache_get() for a specific cid is immediately
followed by a cache_set() for that cid. Of course, this is not the only caching
pattern used in Drupal, so stampede protection can be selectively disabled for
optimal performance.  For example, a cache miss in Drupal core's
module_implements() won't execute a cache_set until drupal_page_footer()
calls module_implements_write_cache() which can occur much later in page
generation.  To avoid long hanging locks, stampede protection should be
disabled for these delayed caching patterns.

Memcache stampede protection can be disabled for entire bins, specific cid's in
specific bins, or cid's starting with a specific prefix in specific bins. For
example:

  $conf['memcache_stampede_protection_ignore'] = array(
    // Ignore stampede protection for the entire 'cache_example' bin.
    'cache_example',
    // Ignore some cids in 'cache_bootstrap'.
    'cache_bootstrap' => array(
      'module_implements',
      'variables',
      'schema:runtime:*',
      'theme_registry:runtime:*',
    ),
    // Ignore all cids in the 'cache' bin starting with 'i18n:string:'
    'cache' => array(
      'i18n:string:*',
    ),
  );

Only change the following stampede protection tunables if you're sure you know
what you're doing, which requires first reading the memcache.inc code.

The value passed to lock_acquire, defaults to '15':
  $conf['memcache_stampede_semaphore'] = 15;

The value passed to lock_wait, defaults to 5:
  $conf['memcache_stampede_wait_time'] = 5;

The maximum number of calls to lock_wait() due to stampede protection during a
single request, defaults to 3:
  $conf['memcache_stampede_wait_limit'] = 3;

When adjusting these variables, be aware that:
 - there is unlikely to be a good use case for setting wait_time higher
   than stampede_semaphore;
 - wait_time * wait_limit is designed to default to a number less than
   standard web server timeouts (i.e. 15 seconds vs. apache's default of
   30 seconds).

## EXAMPLES ##

Example 1:

First, the most basic configuration which consists of one memcached instance
running on localhost port 11211 and all caches except for cache_form being
stored in memcache. We also enable stampede protection, and the memcache
locking mechanism. Finally, we tell Drupal to not bootstrap the database when
serving cached pages to anonymous visitors.

  $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
  $conf['lock_inc'] = 'sites/all/modules/memcache/memcache-lock.inc';
  $conf['memcache_stampede_protection'] = TRUE;
  $conf['cache_default_class'] = 'MemCacheDrupal';

  // The 'cache_form' bin must be assigned to non-volatile storage.
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

  // Don't bootstrap the database when serving pages from the cache.
  $conf['page_cache_without_database'] = TRUE;
  $conf['page_cache_invoke_hooks'] = FALSE;

Note that no servers or bins are defined.  The default server and bin
configuration which is used in this case is equivalant to setting:

  $conf['memcache_servers'] = array('localhost:11211' => 'default');


Example 2:

In this example we define three memcached instances, two accessed over the
network, and one on a Unix socket -- please note this is only an illustration of
what is possible, and is not a recommended configuration as it's highly unlikely
you'd want to configure memcache to use both sockets and network addresses like
this, instead you'd consistently use one or the other.

The instance on port 11211 belongs to the 'default' cluster where everything
gets cached that isn't otherwise defined. (We refer to it as a "cluster", but in
this example our "clusters" involve only one instance.) The instance on port
11212 belongs to the 'pages' cluster, with the 'cache_page' table mapped to
it -- so the Drupal page cache is stored in this cluster.  Finally, the instance
listening on a socket is part of the 'blocks' cluster, with the 'cache_block'
table mapped to it -- so the Drupal block cache is stored here. Note that
sockets do not have ports.

  $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
  $conf['lock_inc'] = 'sites/all/modules/memcache/memcache-lock.inc';
  $conf['memcache_stampede_protection'] = TRUE;
  $conf['cache_default_class'] = 'MemCacheDrupal';

  // The 'cache_form' bin must be assigned no non-volatile storage.
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

  // Don't bootstrap the database when serving pages from the cache.
  $conf['page_cache_without_database'] = TRUE;
  $conf['page_cache_invoke_hooks'] = FALSE;

  // Important to define a default cluster in both the servers
  // and in the bins. This links them together.
  $conf['memcache_servers'] = array('10.1.1.1:11211' => 'default',
                                    '10.1.1.1:11212' => 'pages',
                                    'unix:///path/to/socket' => 'blocks');
  $conf['memcache_bins'] = array('cache' => 'default',
                                 'cache_page' => 'pages',
                                 'cache_block' => 'blocks');


Example 3:

Here is an example configuration that has two clusters, 'default' and
'cluster2'. Five memcached instances running on four different servers are
divided up between the two clusters. The 'cache_filter' and 'cache_menu' bins
go to 'cluster2'. All other bins go to 'default'.

  $conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
  $conf['lock_inc'] = 'sites/all/modules/memcache/memcache-lock.inc';
  $conf['memcache_stampede_protection'] = TRUE;
  $conf['cache_default_class'] = 'MemCacheDrupal';

  // The 'cache_form' bin must be assigned no non-volatile storage.
  $conf['cache_class_cache_form'] = 'DrupalDatabaseCache';

  // Don't bootstrap the database when serving pages from the cache.
  $conf['page_cache_without_database'] = TRUE;
  $conf['page_cache_invoke_hooks'] = FALSE;

  $conf['memcache_servers'] = array('10.1.1.6:11211' => 'default',
                                    '10.1.1.6:11212' => 'default',
                                    '10.1.1.7:11211' => 'default',
                                    '10.1.1.8:11211' => 'cluster2',
                                    '10.1.1.9:11211' => 'cluster2');

  $conf['memcache_bins'] = array('cache' => 'default',
                                 'cache_filter' => 'cluster2',
                                 'cache_menu' => 'cluster2');
  );

## PREFIXING ##

If you want to have multiple Drupal installations share memcached instances,
you need to include a unique prefix for each Drupal installation in the $conf
array of settings.php:

$conf['memcache_key_prefix'] = 'something_unique';

## MAXIMUM LENGTHS ##

If the length of your prefix + key + bin combine to be more than 250 characters,
they will be automatically hashed. Memcache only supports key lengths up to 250
bytes. You can optionally configure the hashing algorithm used, however sha1 was
selected as the default because it performs quickly with minimal collisions.

Visit http://www.php.net/manual/en/function.hash-algos.php to learn more about
which hash algorithms are available.

$conf['memcache_key_hash_algorithm'] = 'sha1';

You can also tune the maximum key length BUT BE AWARE this doesn't affect
memcached's server-side limitations -- this value is primarily exposed to allow
you to further shrink the length of keys to optimize network performance.
Specifying a length larger than 250 will almost certainly lead to problems
unless you know what you're doing.

$conf['memcache_key_max_length'] = 250;

By default, the memcached server can store objects up to 1 MiB in size. It's
possible to increase the memcached page size to support larger objects, but this
can also lead to wasted memory. Alternatively, the Drupal memcache module splits
these large objects into smaller pieces. By default, the Drupal memcache module
splits objects into 1 MiB sized pieces. You can modify this with the following
tunable to match any special server configuration you may have. NOTE: Increasing
this value without making changes to your memcached server can result in
failures to cache large items.

(Note: 1 MiB = 1024 x 1024 = 1048576.)

$conf['memcache_data_max_length'] = 1048576;

It is generally undesirable to store excessively large objects in memcache as
this can result in a performance penalty. Because of this, by default the Drupal
memcache module logs any time an object is cached that has to be split into
multiple pieces. If this is generating too many watchdog logs, you should first
understand why these objects are so large and if anything can be done to make
them smaller. If you determine that the large size is valid and is not causing
you any unnecessary performance penalty, you can tune the following variable to
minimize or disable this logging. Set the value to a positive integer to only
log when an object is split into this many or more pieces. For example, if
memcache_data_max_length is set to 1048576 and memcache_log_data_pieces is set
to 5, watchdog logs will only be written when an object is split into 5 or more
pieces (objects >4 MiB in size). Or, to to completely disable logging set
memcache_log_data_pieces to 0 or FALSE.

$conf['memcache_log_data_pieces'] = 2;

## MULTIPLE SERVERS ##

To use this module with multiple memcached servers, it is important that you set
the hash strategy to consistent. This is controlled in the PHP extension, not
the Drupal module.

If using PECL memcache:
Edit /etc/php.d/memcache.ini (path may changed based on package/distribution)
and set the following:
memcache.hash_strategy=consistent

You need to reload apache httpd after making that change.

If using PECL memcached:
Memcached options can be controlled in settings.php.  The following setting is
needed:
$conf['memcache_options'] = array(
  Memcached::OPT_DISTRIBUTION => Memcached::DISTRIBUTION_CONSISTENT,
);

## SESSIONS ##

NOTE: Session.inc is not yet ported to Drupal 7 and is not recommended for use
in production.

Here is a sample config that uses memcache for sessions. Note you MUST have
a session and a users server set up for memcached sessions to work.

$conf['cache_backends'][] = 'sites/all/modules/memcache/memcache.inc';
$conf['cache_default_class'] = 'MemCacheDrupal';

// The 'cache_form' bin must be assigned no non-volatile storage.
$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
$conf['session_inc'] = 'sites/all/modules/memcache/unstable/memcache-session.inc';

$conf['memcache_servers'] = array(
    '10.1.1.1:11211' => 'default',
    '10.1.1.1:11212' => 'filter',
    '10.1.1.1:11213' => 'menu',
    '10.1.1.1:11214' => 'page',
    '10.1.1.1:11215' => 'session',
    '10.1.1.1:11216' => 'users',
);
$conf['memcache_bins'] = array(
    'cache' => 'default',
    'cache_filter' => 'filter',
    'cache_menu' => 'menu',
    'cache_page' => 'page',
    'session' => 'session',
    'users' => 'users',
);

## TROUBLESHOOTING ##

PROBLEM:
 Error:
  Failed to load required file memcache/dmemcache.inc

SOLUTION:
You need to enable memcache in settings.php. Search for "Example 1" above
for a basic configuration example.

PROBLEM:
 Error:
  PECL !extension version %version is unsupported. Please update to
  %recommended or newer.

SOLUTION:
Upgrade to the latest available PECL extension release. Older PECL extensions
have known bugs and cause a variety of problems when using the memcache module.

PROBLEM:
 Error:
  Failed to connect to memcached server instance at <IP ADDRESS>.

SOLUTION:
Verify that the memcached daemon is running at the specified IP and PORT. To
debug you can try to telnet directly to the memcache server from your web
servers, example:
   telnet localhost 11211

PROBLEM:
 Error:
  Failed to store to then retrieve data from memcache.

SOLUTION:
Carefully review your settings.php configuration against the above
documentation. This error simply does a cache_set followed by a cache_get
and confirms that what is written to the cache can then be read back again.
This test was added in the 7.x-1.1 release.

The following code is what performs this test -- you can wrap this in a <?php
tag and execute as a script with 'drush scr' to perform further debugging.

        $cid = 'memcache_requirements_test';
        $value = 'OK';
        // Temporarily store a test value in memcache.
        cache_set($cid, $value);
        // Retreive the test value from memcache.
        $data = cache_get($cid);
        if (!isset($data->data) || $data->data !== $value) {
          echo t('Failed to store to then retrieve data from memcache.');
        }
        else {
          // Test a delete as well.
          cache_clear_all($cid, 'cache');
        }

PROBLEM:
 Error:
  Unexpected failure when testing memcache configuration.

SOLUTION:
Be sure the memcache module is properly installed, and that your settings.php
configuration is correct. This error means an exception was thrown when
attempting to write to and then read from memcache.

PROBLEM:
 Error:
  Failed to set key: Failed to set key: cache_page-......

SOLUTION:
Upgrade your PECL library to PECL package (2.2.1) (or higher).

WARNING:
Zlib compression at the php.ini level and Memcache conflict.
See http://drupal.org/node/273824

## MEMCACHE ADMIN ##

A module offering a UI for memcache is included. It provides aggregated and
per-page statistics for memcache.

## Memcached PECL Extension Support

We also support the Memcached PECL extension. This extension backends
to libmemcached and allows you to use some of the newer advanced features in
memcached 1.4.

NOTE: It is important to realize that the memcache php.ini options do not impact
the memcached extension, this new extension doesn't read in options that way.
Instead, it takes options directly from Drupal. Because of this, you must
configure memcached in settings.php. Please look here for possible options:

http://us2.php.net/manual/en/memcached.constants.php

An example configuration block is below, this block also illustrates our
default options (selected through performance testing). These options will be
set unless overridden in settings.php.

  $conf['memcache_options'] = array(
    Memcached::OPT_COMPRESSION => FALSE,
    Memcached::OPT_DISTRIBUTION => Memcached::DISTRIBUTION_CONSISTENT,
  );

These are as follows:

 * Turn off compression, as this takes more CPU cycles than it's worth for most
   users
 * Turn on consistent distribution, which allows you to add/remove servers
   easily

Other options you could experiment with:
 + Memcached::OPT_BINARY_PROTOCOL => TRUE,
    * This enables the Memcache binary protocol (only available in Memcached
      1.4 and later). Note that some users have reported SLOWER performance
      with this feature enabled. It should only be enabled on extremely high
      traffic networks where memcache network traffic is a bottleneck.
      Additional reading about the binary protocol:
        http://code.google.com/p/memcached/wiki/MemcacheBinaryProtocol

 + Memcached::OPT_TCP_NODELAY => TRUE,
    * This enables the no-delay feature for connecting sockets; it's been
      reported that this can speed up the Binary protocol (see above). This
      tells the TCP stack to send packets immediately and without waiting for
      a full payload, reducing per-packet network latency (disabling "Nagling").

It's possible to enable SASL authentication as documented here:
  http://php.net/manual/en/memcached.setsaslauthdata.php
  https://code.google.com/p/memcached/wiki/SASLHowto

SASL authentication requires a memcached server with SASL support (version 1.4.3
or greater built with --enable-sasl and started with the -S flag) and the PECL
memcached client version 2.0.0 or greater also built with SASL support. Once
these requirements are satisfied you can then enable SASL support in the Drupal
memcache module by enabling the binary protocol and setting
memcache_sasl_username and memcache_sasl_password in settings.php. For example:

  $conf['memcache_options'] = array(
    Memcached::OPT_BINARY_PROTOCOL => TRUE,
  );
  $conf['memcache_sasl_username'] = 'yourSASLUsername';
  $conf['memcache_sasl_password'] = 'yourSASLPassword';
