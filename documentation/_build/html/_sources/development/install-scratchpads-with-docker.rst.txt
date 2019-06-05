Installing Scratchpads with Docker Compose
==========================================

This chapter is a tutorial on how to install Scratchpads with Docker
Compose. The Docker Compose configuration was added in Scratchpads
2.9.2, so please use master branch or a release >= 2.9.2.

Step 1 — Install Docker & Docker Compose
----------------------------------------

For Centos 7, please follow the instructions at [[Install Docker and
Docker Compose (Centos 7)|Install Docker and Docker Compose (Centos 7)]]

For Ubuntu, install Docker following the official `Docker CE`_ and
`Docker Compose`_ installation guidelines.

Step 2 — Install Scratchpads
----------------------------

Either clone the Scratchpads project or `download a release >= 2.9.2`_
from Github.

::

   git clone https://github.com/NaturalHistoryMuseum/scratchpads2.git /var/lib/scratchpads2

Step 3 — Update environment variables
-------------------------------------

Copy /var/lib/scratchpads2/.env.template to /var/lib/scratchpads2/.env
and set passwords and other sensitive or customisable variables.

::

   cp /var/lib/scratchpads2/.env.template /var/lib/scratchpads2/.env

The defaults will be sufficient for a local development environment, but
must be updated for a production environment.

Step 4 — Run Docker images
--------------------------

::

   cd /var/lib/scratchpads2
   make up

This installation includes `Drush`_, a command line interface for
Drupal. You can access it at:

::

   make drush --help

.. _Docker CE: https://docs.docker.com/install/linux/docker-ce/ubuntu/
.. _Docker Compose: https://docs.docker.com/compose/install/
.. _download a release >= 2.9.2: https://github.com/NaturalHistoryMuseum/scratchpads2/releases
.. _Drush: https://github.com/drush-ops/drush
