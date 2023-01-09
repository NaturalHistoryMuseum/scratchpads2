#!/usr/bin/env bash

# Add the Aegir package source
echo "deb http://debian.aegirproject.org stable main" | tee -a /etc/apt/sources.list.d/aegir-stable.list
wget -q http://debian.aegirproject.org/key.asc -O- | sudo apt-key add -

# Update
apt-get update
apt-get upgrade -y
apt-get install debconf-utils -y

# Set the various options for the packages that aegir will drag in with it (Postfix/MySQL/etc)
# MySQL
echo "mysql-server-5.5 mysql-server/root_password_again password vagrant" | debconf-set-selections
echo "mysql-server-5.5 mysql-server/root_password password vagrant" | debconf-set-selections
# Postfix
echo "postfix postfix/mailname string hostmaster.vagrant"|debconf-set-selections
echo "postfix postfix/main_mailer_type select Internet Site"|debconf-set-selections
echo "postfix postfix/destinations string hostmaster.vagrant, vagrant, localhost.localdomain, localhost"|debconf-set-selections
# Aegir
echo "aegir2-hostmaster aegir/db_password password vagrant"|debconf-set-selections
echo "aegir2-hostmaster aegir/db_user string root"|debconf-set-selections
echo "aegir2-hostmaster aegir/email string aegir@hostmaster.vagrant"|debconf-set-selections
echo "aegir2-hostmaster aegir/db_host string localhost"|debconf-set-selections
echo "aegir2-hostmaster aegir/makefile string "|debconf-set-selections
echo "aegir2-hostmaster aegir/site string hostmaster.vagrant"|debconf-set-selections
echo "aegir2-hostmaster aegir/webserver select apache2"|debconf-set-selections

# Install dependencies for the aegir2 package
apt-get -y install apache2 drush git-core libapache2-mod-php5 mysql-client mysql-server php5 php5-mysql postfix rsync unzip
# Install additional packages Scratchpads 2 require
apt-get -y install dnsutils memcached solr-tomcat php5-gd php5-curl php5-memcached php5-mcrypt php5-dev php5-gmp varnish

# Install a couple of PECL modules
pecl install mailparse
pecl install uploadprogress
echo "extension=mailparse.so" > /etc/php5/conf.d/mailparse.ini
echo "extension=uploadprogress.so" > /etc/php5/conf.d/uploadprogress.ini

# Increase the memory limit for php
sed "s/memory_limit\ =\ 128M/memory_limit = 256M/" -i /etc/php5/apache2/php.ini

# Convert all of the databases tables to MyISAM
#for i in $(echo "SELECT CONCAT(TABLE_SCHEMA, '.', TABLE_NAME) FROM information_schema.TABLES WHERE ENGINE='InnoDB'"|mysql -pvagrant|grep -v CONCAT)
#do
#	echo "ALTER TABLE "$i" ENGINE=MyISAM;" | mysql -pvagrant
#done

# Tweak MySQL to skip-innodb
echo "[mysqld]
innodb=OFF
default_storage_engine=MyISAM" > /etc/mysql/conf.d/skip-innodb.cnf
service mysql restart

# Tweak the memcached settings to allow large objects.
sed "s/^-m\s[0-9]*/-m 256/" /etc/memcached.conf -i
echo "# Increase the Maximum size of cached objects
-I 16M" >> /etc/memcached.conf
service memcached restart

# Point web-scratchpad-solr at the local machine so the local solr instance should just work
echo "127.0.0.1 web-scratchpad-solr.nhm.ac.uk" >> /etc/hosts
echo "<VirtualHost *:80>
 ServerName web-scratchpad-solr.nhm.ac.uk
 DocumentRoot  /var/www
 <Location /solr/scratchpads2-2>
  Order deny,allow
  Allow from all
  ProxyPass http://localhost:8080/solr nocanon connectiontimeout=30 timeout=60
  ProxyPassReverse http://localhost:8080/solr
 </Location>
</VirtualHost>" > /etc/apache2/sites-available/solr
cd /etc/solr/conf
cp /vagrant/sites/all/modules/contrib/apachesolr/solr-conf/solr-3.x/* .
a2ensite solr
a2enmod proxy proxy_http
service apache2 reload
service tomcat6 restart

# Install our own custom Aegir packages, as there is a bug in the
# standard package that results in the install attempting to ask
# the user for a password.
dpkg -i /vagrant/vagrant/debian-packages/aegir2_2.1_all.deb /vagrant/vagrant/debian-packages/aegir2-hostmaster_2.1_all_scratchpads.deb /vagrant/vagrant/debian-packages/aegir2-provision_2.1_all.deb
apt-get update -y
apt-get upgrade -y

# Set the password for the admin aegir user
echo "UPDATE users SET pass = MD5('vagrant') WHERE uid = 1;" | mysql -uroot -pvagrant hostmastervagran

VARNISHSECRET=`cat /etc/varnish/secret`

# Add a global aegir config file which enables memcache, varnish and other caches
echo "<?php
global \$conf;
\$conf['cron_safe_threshold'] = 0;
\$conf['preprocess_css'] = 1;
\$conf['preprocess_js'] = 1;
\$conf['jquery_update_compression_type'] = 'min';
\$conf['jquery_update_jquery_cdn'] = 'none';
\$conf['error_level'] = ERROR_REPORTING_HIDE;
\$conf['syslog_identity'] = \$_SERVER['HTTP_HOST'];

// Cache
\$conf['block_cache'] = TRUE;
\$conf['cache'] = TRUE;
\$conf['cache_lifetime'] = 3600;
\$conf['page_cache_maximum_age'] = 10800;

// Memcache
\$conf['memcache_key_prefix'] = md5(\$db_url['default']);
\$conf['memcache_servers'] = array('127.0.0.1:11211' => 'default');
\$conf['cache_backends'][] = 'sites/all/modules/contrib/memcache/memcache.inc';
\$conf['cache_default_class'] = 'MemCacheDrupal';

// Varnish
\$conf['reverse_proxy'] = TRUE;
\$conf['reverse_proxy_addresses'] = array('127.0.0.1');
\$conf['varnish_flush_cron'] = 0;
\$conf['varnish_version'] = 3;
\$conf['varnish_control_terminal'] = '127.0.0.1:6082';
\$conf['varnish_control_key'] = '"$VARNISHSECRET"';
\$conf['varnish_socket_timeout'] = 200;
\$conf['varnish_cache_clear'] = 0;
\$conf['varnish_bantype'] = 0;
\$conf['cache_backends'][] = 'sites/all/modules/contrib/varnish/varnish.cache.inc';
\$conf['cache_class_cache_page'] = 'VarnishCache';
\$conf['page_cache_invoke_hooks'] = FALSE;


// Not everybody can run updates.
\$update_free_access = 0;

// Set some PHP settings
@ini_set('pcre.backtrack_limit', 10000000);
@ini_set('pcre.recursion_limit', 10000000);
@ini_set('session.cookie_lifetime', 604800);
@ini_set('session.gc_maxlifetime', 604800);
@ini_set('session.use_cookies', 1);
@ini_set('mysql.default_socket', '/var/lib/mysql/mysql.sock');" > /var/aegir/config/includes/global.inc

# Create the Scratchpads web folder
cd /var/aegir/platforms
mkdir scratchpads
cd scratchpads
ln -s /vagrant/* /vagrant/.htaccess .
rm sites
mkdir sites
cd sites
ln -s /vagrant/sites/* .
rm all
mkdir all
cd all
ln -s /vagrant/sites/all/* .
rm drush
chown aegir:www-data /var/aegir/platforms/scratchpads/sites
chown aegir:www-data /var/aegir/platforms/scratchpads/sites/all
# Special copy for the scratchpads_twitter.ini file which may be in the /vagrant folder
mkdir -p /usr/local/share/scratchpads-global
cp /vagrant/scratchpads_twitter.ini /usr/local/share/scratchpads-global

# Restart Apache so that the new global file is read
service apache2 reload

# Add a record to the hosts file so that the site can access itself
echo "127.0.0.1 scratchpads.vagrant" >> /etc/hosts
# Create the Scratchpads platform
su -c /vagrant/vagrant/bootstrap.aegir.sh aegir

# Get the external IP address to inform people to add it to their hosts file.
IPADDRESS=`/sbin/ifconfig eth1 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'`

# Inform the user how they can login to the Aegir site.
echo "










Add the following entry to your 'hosts' file (http://www.rackspace.com/knowledge_center/article/how-do-i-modify-my-hosts-file)
"$IPADDRESS" scratchpads.vagrant hostmaster.vagrant

Then login to the Aegir interface:
http://hostmaster.vagrant/
Username: admin
Password: vagrant"
