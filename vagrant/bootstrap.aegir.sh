#!/usr/bin/env bash

# Create the platform in Aegir
drush --root='/var/aegir/platforms/scratchpads-2' provision-save '@platform_scratchpads-2' --context_type='platform' --makefile='/vagrant/scratchpads2.make'
drush --root='/var/aegir/platforms/scratchpads' provision-save '@platform_scratchpads' --context_type='platform'
drush @platform_scratchpads-2 provision-verify
drush @platform_scratchpads provision-verify
drush @hostmaster hosting-import @platform_scratchpads-2
drush @hostmaster hosting-import @platform_scratchpads

# Create a site
drush provision-save '@scratchpads.vagrant' --context_type='site' --uri='scratchpads.vagrant' --platform='@platform_scratchpads' --server='@server_master' --db_server='@server_localhost' --profile='scratchpad_2_training' --client_name='admin'
drush @scratchpads.vagrant provision-install
drush @hostmaster hosting-task @platform_scratchpads verify
# Clear the cache on the site, to ensure it's working as expected
drush @scratchpads.vagrant cc all
drush @scratchpads.vagrant cron
