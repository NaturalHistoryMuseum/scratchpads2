Scratchpads 2.x
===============

1. Overview
-----------
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

2. Installation
---------------
The Scratchpads are based upon [Drupal](http://drupal.org/), and therefore have 
many of the same dependencies as it does. We recommend installing Scratchpads 
using [MySQL](http://www.mysql.com) only, preferably on a Debian based server. 
We are currently working on a [Vagrant/Chef 
setup](https://github.com/NaturalHistoryMuseum/scratchpads2-chef) to allow 
users to get their 
own copy of the Scratchpads up and running easily. We expect this to be ready 
by July 2015.

3. Installation Support
-----------------------
Only limited support is offered for external installations. You should only 
attempt to get your own Scratchpads installation running if you feel 
comfortable with all of the technologies involved (Linux, Apache, MySQL, PHP, 
Solr, Memcache, Varnish). If you do not feel comfortable running your own 
installation, then please apply for a site at the link above.

4. User Support
---------------
We offer extensive support to users of the Scratchpads, whether using an NHM 
maintained Scratchpad, or a local one. Support should be requested using the 
"Issues..." tab which is visible when logged in to a Scratchpad.

5. Development
--------------
If you're reading this, you've either already downloaded the Scratchpads code, 
or are on our Github page. As we have just moved to using Github, we are 
relatively new to the concept of *Pull requests*, but we are certainly willing 
to consider them. If you are developing on top of the Scratchpads, please get 
in touch to let us know what you are working on.
