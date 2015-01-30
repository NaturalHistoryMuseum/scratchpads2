Automatic Entity Labels Module
------------------------------
by Benedikt Forchhammer, b.forchhammer@mind2.de

Based on the Automatic Node Titles module by Wolfgang Ziegler, nuppla@zites.net
Initial port to entities by Vasi Chindris, https://drupal.org/user/342104

Description
-----------
This is a small and efficient module that allows hiding of entity label fields.
To prevent empty labels it can be configured to generate the label
automatically by a given pattern. The module can be used for any entity type
that has a label, including e.g. for node titles, comment subjects, taxonomy
term names and profile2 labels.

Patterns for automatic labels are constructed with the help of tokens. Drupal
core provides a basic set of tokens [1]. For a token selection widget install
the token module [2]. Some entity types (e.g. profile2) provide tokens via the
entity_token module, which is part of the entity module [3].

Advanced users can use PHP code for automatically generating labels. See below
for more information.


Installation 
------------
 * (optional) Download and install the token module in order to get token
   replacement help.
 * Copy the module's directory to your modules directory and activate the module.
 * For each content type you want to have an automatic title, configure the
   module bundle configuration page, e.g. 'admin/structure/types' for content
   types


Notes
-----
1) Due to the way the module works, it is currently not possible to make use
   of some replacement tokens which are not available before the content node
   is saved the first time, e.g. the node id ([node:nid]). See issue #1445124
   if you are interested in this behavior [4].

2) This module grew out of the Automatic node titles module and aims to be a
   full replacement. During installation all auto_nodetitle variables (ant_*)
   are automatically migrated to this module and deleted to prevent conflicts
   between both modules. There is currently no way to undo this process.

3) Automatic entity labels also works with title replacements provided by the
   Title module [5].

4) When you change the pattern for automatic labels, existing entities are not
   updated automatically. For 'node' entities you can use the 'batch update'
   feature on the 'admin/content' page to update the title of existing nodes to
   your new pattern. This is currently not possible for other entity types.


Advanced Use: PHP Code
----------------------

Users which have the 'use PHP for label patterns' permission can use PHP code
within patterns for automatic label genereration.

Here is a simple example, which just adds the node's author as title:

	<?php return "Author: $entity->name"; ?>

Two variables are available for use:
- $entity : the entity for which the label is generated
- $language : the intended language of the label

You can also combine tokens with PHP evaluation. Be aware that this can lead to
security holes if you use textual values provided by users. If in doubt, avoid
combining tokens with php evaluation.

Here is an example, which sets an entity label to the value of a field
(field_testnumber), or the entity bundle (node type) if the field value is
empty.
 
<?php
  $token = '[field_testnumber]';
  if (empty($token)) {
    return '[type]';
  }
  else {
    return $token;
  } 
?>


[1] http://drupal.org/documentation/modules/token
[2] http://drupal.org/project/token
[3] http://drupal.org/project/entity 
[4] http://drupal.org/node/1445124
[5] http://drupal.org/project/title
