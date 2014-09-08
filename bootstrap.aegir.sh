#!/usr/bin/env bash

# Create the platform in Aegir
echo -e "<?php\n\$aliases['platform_scratchpads'] = array (\n  'context_type' => 'platform',\n  'server' => '@server_master',\n  'web_server' => '@server_master',\n  'root' => '/var/aegir/platforms/scratchpads',\n  'makefile' => '',\n  'make_working_copy' => false,\n);" > /var/aegir/.drush/platform_scratchpads.alias.drushrc.php
drush @platform_scratchpads provision-verify
drush @hostmaster hosting-import @platform_scratchpads

# Create a site
#drush provision-save '@scratchpads.local' --context_type='site' --uri='scratchpads.local' --platform='@platform_scratchpads' --server='@server_master' --db_server='@server_localhost' --profile='scratchpads_2_training' --client_name='admin'
#drush @scratchpads.local provision-install
