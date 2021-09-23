This module allows facetting to happen across multiple domains.

1. Enable the module

2. Enabling the environment
Enable the environment you want (eg.: select multisite support in
the settings)

3. Existing facets
All your existing facets will still work but they are indexed using an ID.
The Solr module also indexed them using their name and this module already ships
with 3 default multisite enabled facets
- Tags
- Authors
- Content Types (Bundles)
It also ships with a facet to switch between the sites (Hash/Site)

3. Custom Facets
Create a custom module that enables extra cross-site facets. In case of a
a custom vocabulary this will be named sm_vid_NAME. You can view all the fields
in solr by going to your Drupal reports and select Solr Search Index
Copy the apachesolr_multisitesearch_facetapi_facet_info into your module and
get started.

