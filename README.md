Scratchpads 2.x
===============

## Overview

Scratchpads are an online virtual research environment for biodiversity,
allowing anyone to share their data and create their own research networks.
Sites are hosted at the Natural History Museum London, and offered freely to
any scientist that completes an online [registration
form](http://get.scratchpads.eu).

Sites can focus on specific taxonomic groups, or the biodiversity of a
biogeographic region, or indeed any aspect of natural history. Scratchpads are
also suitable for societies or for managing and presenting projects. Key
features of Scratchpads (see also Scratchpads feature list) include: tools to
manage biological classifications, bibliography management, media (images,
video and audio), rich taxon pages (with structured descriptions, specimen
records, and distribution data), and character matrices.

Scratchpads support various ways of communicating with site members and
visitors such as blogs, forums, newsletters and a commenting system.

For more information about what scratchpads are, see [http://scratchpads.eu](http://scratchpads.eu)

## Install dependencies

You can use [drush](https://docs.drush.org/en/8.x/) and [composer](https://getcomposer.org/) to install dependencies.

There are two types of dependencies to install:
 - Drupal dependencies: `drush make --no-core scratchpads2.make.yml .`
 - Composer dependencies: `composer install`

You can also install contrib modules using `drush dl {module_name}`.

## Development & Docker

There are a collection of docker images, mostly intended for development. If you want to self-host a Scratchpad you can try using these, though we recommend letting the NHM host your Scratchpad with all of the others, unless you have some very specific requirements.

To get a development scratchpad up and running:

- [Install docker-sync](https://docker-sync.readthedocs.io/en/latest/getting-started/installation.html) (`gem install docker-sync`)
- Copy .env.template to .env and set some environment values
- Run `docker-sync start`
- Run `docker-compose up -d apache mysql solr`
  (excludes varnish service - only for production, not supported on local)
- Go to [`localhost:8080/install.php`](http://localhost:8080/install.php) to start the installation process (make sure you choose the "Scratchpad 2" profile)
- This repo will be mounted inside the docker, so you will be able to see any changes in real time (-ish)

If you want to help develop Scratchpads please don't hesitate to get in touch!

# ddev

This is a defacto community standard for a local developer setup - as in the web application/site running locally on a developer's personal machine.

## Installation:
https://ddev.readthedocs.io/en/stable/

Then once installed, assuming using a command line terminal setup, e.g. Ubuntu, macOS or WSL2, start ddev.

This will use the ddev setup created in .ddev for our purposes. This setup contains the php and mysql versions that are closest to the original dev setup before ddev and on production.  A brand new .ddev setup can be created by doing ddev config before ddev start, but ddev will select more recent php and mysql versions as defaults.

e.g. for reference, running ddev config setup looks like this:

```
ddev config
Creating a new ddev project config in the current directory (/home/robd/Work/projects/scratchpads/sites/scratchpads-dev) 
Once completed, your configuration will be written to /home/robd/Work/projects/scratchpads/sites/scratchpads-dev/.ddev/config.yaml
 
Project name (scratchpads-dev): 

The docroot is the directory from which your site is served.
This is a relative path from your project root at /home/robd/Work/projects/scratchpads/sites/scratchpads-dev 
You may leave this value blank if your site files are in the project root 
Docroot Location (current directory): 
Found a drupal7 codebase at /home/robd/Work/projects/scratchpads/sites/scratchpads-dev. 
Project Type [backdrop, drupal6, drupal7, drupal8, drupal9, laravel, magento, magento2, php, shopware6, typo3, wordpress] (drupal7): 
Ensuring write permissions for scratchpads-dev 
No settings.php file exists, creating one 
Existing settings.php file includes settings.ddev.php 
Configuration complete. You may now run 'ddev start'. 
```


## User Support

We offer extensive support to users of the Scratchpads, whether using an NHM maintained Scratchpad, or a local one. Support should be requested using the "Issues..." tab which is visible when logged in to a Scratchpad.

## To import a database

cat ../wallace.sql | docker exec -i scratchpads.apache drush sql-cli


## Troubleshooting

### problem:
Error like: 'docker not found' or 'Docker command can't connect to Docker daemon'

#### solution:

1 - add your user to the docker group:
`sudo usermod -aG docker $(whoami)`

2 - reload user group assignments in your current terminal shell where you are trting to run docker(this saves having to open a new shell or avoid logging out and back in)

`su - $USER`

#### credit:
1 - https://stackoverflow.com/a/33782459
2 - https://superuser.com/a/345051


### problem:

Importing database dump fails with either or both of the below errors:

```
ERROR 1071 (42000) at line 59: Specified key was too long; max key length is 767 bytes
SQL client error occurred.
```

```
ERROR 1709 (HY000) at line 59: Index column size too large. The maximum column size is 767 bytes.
SQL client error occurred. 
```

#### solution:

ssh into ddev container, run sql cli, run several commands to configure db to accept the things it was rejecting and erroring on.

#### steps: 

```
ddev ssh
mysql -uroot -proot
GRANT SUPER ON *.* TO 'db'@'localhost' IDENTIFIED BY 'db';
FLUSH PRIVILEGES;
SET @@global.innodb_large_prefix = 1;
set global innodb_file_format = BARRACUDA;
set global innodb_large_prefix = ON;
```

