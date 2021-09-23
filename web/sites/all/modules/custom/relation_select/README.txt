  ------------------------------------------------------------------------------------
                         ABOUT
  ------------------------------------------------------------------------------------

This module provides a form element & field widget for creating & editing relations (provided by http://drupal.org/project/relation)

The selection interface is based on views. Use exposed filters to search for entities, & the output of the view becomes 
a clickable drop down list to select from.

For every field instance you can use a different view - but these views must use the "Relation Select" view style & must have at least one exposed filter.

This module comes with two demo views based on EFQ Views (http://drupal.org/project/efq_views). Using EFQ Views allows you to expose
entity type as an filter, allowing for cross-entity type relations to be created. 

If a view has entity type / bundle type exposed filters, this module tries to alter the available options based on the relation being used. 
So if only "node" entities are allowed in the relation, the entity type filter will be set to "node" & hidden. 

By default the field is displayed as a link to the related entity. This can be customised based on entity type, bundle & field name through
the theme layer - see relation_select_preprocess_relation_select_field_view().



  ------------------------------------------------------------------------------------
                         USAGE
  ------------------------------------------------------------------------------------

There are two ways of using this module - using the field widget, or the add relation page. 

1. Field widget.

  * Define a relation type

  * Add a relation field to an entity type. Select "Relation Select" as the field widget.
  
  * For the field settings, select the relation type you want to create, and the view you want to use to select relations (only views with the style "Relation Select" will be available)
  
  * Save the field. When you create content, use the views exposed filters & search button to find entities to link to.
  
  
2. Add relation page

  * This module provides a form API element, which is used on the add relation page.
   
  * The add relation page can be accessed at relation-select/add/[entity-type]/[entity-id]/[relation-type](optional)
    Entity-type & entity-id are of the entity you want to create the relation from. A helper function theme_relation_select_link()
    is provided. You could use this to add an "Add relation to this page" link.
  

  ------------------------------------------------------------------------------------
                         INSTALLATION
  ------------------------------------------------------------------------------------
  
  
No special installation requirements, just download the required modules.
  
  
  ------------------------------------------------------------------------------------
                         REQUIREMENTS
  ------------------------------------------------------------------------------------
  
The following modules are required:

  * views
  * relation
  * subform
  * efq_views

  ------------------------------------------------------------------------------------
                         TODO
  ------------------------------------------------------------------------------------
  
This module is under active development - top of the list is improving the form field display. 
If you have any other suggestions as to how it can be improved please post them to the issue queue.
  
  