Taxonomy Server
===============
Publishes Drupal vocabularies in a way suitable for re-importation by other tools.
===============


When taxonomy_server is enabled, a new *suggested* menu item becomes available
for the path /taxonomy/vocabulary. From there, direct dumps of your current
taxonomy trees are visible, eg /taxonomy/vocabulary/3

Access to these pages is controlled through a new permission.
Administration of the server is controlled through the usual 'administer taxonomy' permission.

From these 'vocabulary' pages, an RDF-only dump of data describing the vocab
is also available. 

This can be retrieved from URLs like
/taxonomy/vocabulary/3/rdf and should return SKOS-like RDF.
These pages are accessed either through content-negotiation 
 - Request /taxonomy/vocabulary/3 as an RDF-XML compatible client 
   and you'll be sent to the RDF page.
or from link hints in the page.
 - a <link rel="alternate" type="application/rdf+xml" /> is inserted into the 
   page header.
 
Direct RDF dumps are also published to supplement individual term pages, such as
/taxonomy/term/66
This RDF is likewise available either through content-negotiation or through
link rel="alternate" hinting, and provides the data underneath URLs like 
/taxonomy/term/66/rdf

