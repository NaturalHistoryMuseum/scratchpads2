Scratchpads Docker
=================

Run private instances of scratchpads2 sites; useful for development and deploy.

A basic scratchpads infrastructure is made of:
 - Apache instance with PHP & Drupal
 - MySQL server
 - Solr server
 - Mailserver

This directory provides the first three, and recommends a standard image (tianon/exim4) for the last.

Requirements
------------

You'll need a recent version of docker installed.

Build
-----

If you want to work with the development versions of these images you'll have to build them.
You can do this by running `./build.sh`, which creates three images with the following tags:

 - `naturalhistorymuseum/scratchpad`: the apache server
 - `naturalhistorymuseum/scratchpad-mysql`: the mysql server
 - `naturalhistorymuseum/scratchpad-solr`: the solr server

In addition to these tags,  The `scratchpad` image is tagged with the scratchpads source code version and release date:

 - `naturalhistorymuseum/scratchpad:2.x.x`
 - `naturalhistorymuseum/scratchpad:2.x.x-YYYY-MM-DD`

The mysql and solr images are also tagged with the release date:

 - `naturalhistorymuseum/scratchpad-mysql:YYYY-MM-DD`
 - `naturalhistorymuseum/scratchpad-solr:YYYY-MM-DD`

Usage
-----

See each image's readme for more information.



Commands
--------

docker exec -i -t scratchpads.apache /bin/bash


