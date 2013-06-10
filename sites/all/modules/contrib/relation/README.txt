* Go to admin/structure/relation, and create a new relation type. Add fields if
  neccesary.
* Enable the relation_entity_collector block if it is not enabled on install - it
  tries to insert itself after the system management block if that one is enabled.
* To use the relation_entity_collector block, go to any page that loads entities,
  and the entity selector will appear.
* "Pick" as many entities as you need for your relation type (between min_ and
  max_arity in the appropriate relation bundle). Picks remain until cleared
  or the relation is created.
* Click "Create Relation", your relation will be created, and you will be given
  a link to the relation page.
* Here you can view the relation, and edit it to add or change field data.
* To see the relation later, the relation_dummy_field shows it on the entities
  belonging to the relation.

For details on why this works, and what the hell we were thinking, see CONCEPTS.txt

-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136
* Ned Haughton (naught101) - http://drupal.org/user/44216
* Karoly Negesi (chx) - http://drupal.org/user/9446
