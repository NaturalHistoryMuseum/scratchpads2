// $Id: README.txt,v 1.7 2011/01/13 22:42:19 agentken Exp $

Menu Node API
Manages relationships between node ids and menu links.

CONTENTS
--------

1. Introduction
2. Installation
3. Use Cases
4. Developer Notes
5. API Hooks
6. The {menu_node} Table

----
1. Introduction

The Menu Node API project fills in one of the "missing tables" in Drupal 6.
This module does nothing on its own. Instead, it creates and maintains
a {menu_node} table. This table maps the relationship between a node
and its menu link (if any).

The module requires the optional core Menu module to be enabled.

By itself, this module has no functionality. When nodes are assigned to a
site menu, records are created to capture that relationship. If the node or
menu link is deleted, the record is likewise deleted.

The module does fire a set of internal hooks when node-based menus are
created, edited, or deleted.  See section 4 for details.

For Drupal 7, you might also investigate the following modules, which provide
similar functionality:

  http://drupal.org/project/entity_index
  http://drupal.org/project/menu_link

----
2. Installation

To install the module, simply place the folder in your modules directory
(typically 'sites/all/modules'.

Then go to the Module administration page and activate the module. It
can be found listed under the 'Other' category.

----
3. Use Cases

The two most obvious uses for this module are:

-- To build an access control module that leverages the menu hierarchy.
-- To integrate menu hierarchies into the Views module.

The purpose, in both cases, is to allow developers and site builders to use
the menu system as the sole metaphor for a site's structure. For many
small to mid-size Drupal sites, this approach pffers advantages over being
forced to use a taxonomy-based (or other) solution).

----
4. Developer Notes

Aside from the core CRUD functions, this module is of limited use. There
are a few handy functions included, such as menu_node_tree(), but they
may not be suitable for all uses. Feel free to develop your own functions
and tricks.

----
5. API Hooks

At the core of the module are three simple CRUD hooks. They each pass
the same arguments:

  -- hook_menu_node_insert($link, $node)
  -- hook_menu_node_update($link, $node)
  -- hook_menu_node_delete($link, $node)

These functions pass the arguments:

  -- $link
  The menu link being acted upon. (Taken from the {menu_links} table as an
  object).
  -- $node
  The complete node object being acted upon.

NOTE: Using menu_get_item() here returns a router item, not the data
found in {menu_links}. So instead, we load the $item from the {menu_links}
table directly.

NOTE: Nodes can be assigned to multiple menu links. In these cases, the
hook should fire once for each menu link.

There are additional functions that you may find useful. Check the inline
Doxygen comments for details.

----
6. The {menu_node} Table

The {menu_node} table is deliberately simple. It contains a primary key
and two columns:

  -- nid
  Integer, unsigned.
  Foreign key to the {node} table.
  
  -- mlid
  Primary key. Integer, unsigned.
  Foreigh key to the {menu_links} table.
