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

There are a collection of docker images, mostly intended for development. If
you want to self-host a Scratchpad you can try using these, though we recommend
letting the NHM host your Scratchpad with all of the others, unless you have
some very specific requirements.

To get a development scratchpad up and running:

- [Install docker-sync](https://docker-sync.readthedocs.io/en/latest/getting-started/installation.html) (`gem install docker-sync`)
- Copy .env.template to .env and set some environment values
- Run `docker-sync start`
- Run `docker-compose up`
- Go to [`localhost:8080/install.php`](http://localhost:8080/install.php) to start the installation process (make sure you choose the "Scratchpad 2" profile)
- This repo will be mounted inside the docker, so you will be able to see any changes in real time (-ish)

If you want to help develop Scratchpads please don't hesitate to get in touch!

## User Support

We offer extensive support to users of the Scratchpads, whether using an NHM
maintained Scratchpad, or a local one. Support should be requested using the
"Issues..." tab which is visible when logged in to a Scratchpad.

## Docker

cat ../wallace.sql | docker exec -i scratchpads.apache drush sql-cli


## Troubleshooting

problem:
Error like: 'docker not found' or 'Docker command can't connect to Docker daemon'

solution:

1 - add your user to the docker group:
sudo usermod -aG docker $(whoami)

2 - reload user group assignments in your current terminal shell where you are trting to run docker
(this saves having to open a new shell or avoid logging out and back in)
su - $USER

credit:
1 - https://stackoverflow.com/a/33782459
2 - https://superuser.com/a/345051


problem:

Importing database dump fails with either or both of the below errors:


ERROR 1071 (42000) at line 59: Specified key was too long; max key length is 767 bytes
SQL client error occurred.


ERROR 1709 (HY000) at line 59: Index column size too large. The maximum column size is 767 bytes.
SQL client error occurred. 


solution:

ssh into ddev container, run sql cli, run several commands to configure db to accept the things it was rejecting and erroring on.

steps:

ddev ssh
mysql -uroot -proot
GRANT SUPER ON *.* TO 'db'@'localhost' IDENTIFIED BY 'db';
FLUSH PRIVILEGES;
SET @@global.innodb_large_prefix = 1;
set global innodb_file_format = BARRACUDA;
set global innodb_large_prefix = ON;

