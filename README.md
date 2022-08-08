Scratchpads 2.x
===============

# Overview

Scratchpads are an online virtual research environment for biodiversity,
allowing anyone to share their data and create their own research networks.

Sites can focus on specific taxonomic groups, or the biodiversity of a
biogeographic region, or indeed any aspect of natural history. 

Scratchpads are also suitable for societies or for managing and presenting projects. 

Key features of Scratchpads (see also Scratchpads feature list) include: 

- tools to manage biological classifications, 
- bibliography management, media (images, video and audio), 
- rich taxon pages (with structured descriptions, specimen records, and distribution data), 
- and character matrices.

Scratchpads support various ways of communicating with site members and
visitors such as blogs, forums, newsletters and a commenting system.

For more information about what scratchpads are, see the online brochure site [scratchpads.org](https://scratchpads.org)

Want to make your own Scratchpads site? See [Official ReadTheDocs site builders' user guide to Scratchpads](https://scratchpads.readthedocs.io/)

# User Support

We offer extensive support to users of the Scratchpads, whether using an NHM maintained Scratchpad, or a local one.

Please raise an issue on [Github](https://github.com/NaturalHistoryMuseum/scratchpads2/issues), or using the "Issues..." tab which is visible when logged in to a Scratchpad (and will post your issue to the [Github issue queue](https://github.com/NaturalHistoryMuseum/scratchpads2/issues)).

# Hosted Scratchpads

Scratchpads are hosted at the Natural History Museum London, and offered freely to
any scientist that completes an online [registration
form](http://get.scratchpads.org).


# Running your own Scratchpad

We recommend running Scratchpads in production using Docker.

### Requirements:

  - [Docker CE](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
  - [Docker Compose](https://docs.docker.com/compose/install/)

### Usage:

To get a production Scratchpad running on Ubuntu:

- Install [Docker CE](https://docs.docker.com/install/linux/docker-ce/ubuntu/) and [Docker Compose](https://docs.docker.com/compose/install/)

- clone the Scratchpads project or [download a release >= 2.9.2](https://github.com/NaturalHistoryMuseum/scratchpads2/releases) from Github

- Copy .env.template to .env and set environment values (MYSQL credentials etc.,))

- Run daemonized docker compose:

    ```docker-compose -f docker-compose.yml -f docker-compose.production.yml up -d```

# To turn off warnings

/admin/config/development/logging

Set error messages to display to 'None'.

# Developing Scratchpads

If you want to help develop Scratchpads please don't hesitate to get in touch! You can raise an issue on the [Github issue queue](https://github.com/NaturalHistoryMuseum/scratchpads2/issues), or contact the development team at the Natural History Museum scratchpads@nhm.ac.uk.

We recommend developing Scratchpads with [DDEV](https://ddev.readthedocs.io/en/stable/), a defacto community standard for local developer setup.

## DDEV

### Requirements:

  - [DDEV](https://ddev.readthedocs.io/en/stable/)

### Usage:

A DDEV config file is included with Scratchpads. To start the DDEV Scratchpad enviornment, run:

```
ddev start
```

After start up, you will need to:

- Run ```install.php``` to start the installation process (make sure you choose the "Scratchpad 2" profile)
- Change the SOLR SERVER URL (admin/config/search/apachesolr/settings/solr/edit) to http://127.0.0.1:18983

### Notes:

To access the SQL database, use ```ddev mysql```

For more information, please see our [development wiki](https://github.com/NaturalHistoryMuseum/scratchpads2/wiki/DDEV)

## Docker

### Requirements:

  - [Docker](https://www.docker.com/)
  - [Docker Compose](https://docs.docker.com/compose/install/)
  - [Docker SYNC](https://docker-sync.readthedocs.io/en/latest/)

### Usage:

- Copy .env.template to .env and set environment values
- Run `docker-sync start`
- Run `docker-compose up -d apache mysql solr` (excludes varnish service - only for production, not supported on local).

After start up, you will need to:

- Go to [`localhost:8080/install.php`](http://localhost:8080/install.php) to start the installation process (make sure you choose the "Scratchpad 2" profile)
- This repo will be mounted inside the docker, so you will be able to see any changes in real time (-ish)

### More info:

Please see our [development wiki](https://github.com/NaturalHistoryMuseum/scratchpads2/wiki/Installing-Scratchpads-with-Docker-Compose).


