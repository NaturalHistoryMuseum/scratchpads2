Views OAI-PMH
=============

This is a Views plug-in module which creates a OAI-PMH data provider using any 
fields which the Views module has access to. It currently only exposes metadata 
in Dublin Core format, but others may be added in the future.

Prerequisite software
=====================

This module requires Views 3.x for both Drupal 6 and Drupal 7.  
It will not work with Views 2.x.


Using the "Views OAI-PMH" module
====================================

  1. Add a new "OAI-PMH" display to your view.
  2. Change its "Style/Format" to the desired metadata format (currently, only dc is supported)
  3. Configure the Drupal “field” to OAI “element” mapping using the “Dublin Core Fields” 
     row style settings control.
  4. Give it a path such as "oai".
  5. Optionally, you can test your repository by going to http://re.cs.uct.ac.za/ 
     and entering the full url and path (http://example.com/oai) in the 
     “Enter the OAI baseURL :” box and then clicking on 
     “Test the specified/selected baseURL” (on the right side of the page).


Default Views
=============

A default view called "Biblio OAI-PMH" is provided as an example.  This view emulates the oai2 module.