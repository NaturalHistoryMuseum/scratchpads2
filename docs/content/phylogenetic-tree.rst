Phylogenetic tree
=================

*Scratchpads provide a custom field formatter for displaying NeXML as phylogenetic trees. NeXML is an exchange standard for representing phyloinformatic data.*

*This field formatter can be added to existing or custom content types - in this example we'll use a custom *Tree* content type.*


Creating a new ‘Tree’ Content Type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Structure*
2. Click *Content Types* then *Add content type*
3. Enter a *TITLE*, e.g. “Tree” and click *Save and add fields*
4. In the section *Add new field* provide a label, e.g. “Tree”
5. Select the *Field type* “Long Text” then click *Save*
6. Under *Manage display* you should set the *Format* to Newick/NeXML
   tree
7. Under *Display type* choose from either *Rectangular* or *Circular*
8. Click the *Save* button at the bottom left


Changing Phylogenetic Tree Display Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. From the :term:`Admin menu` go to *Structure*
2. Click *Content Types* then *manage display* for your tree content
   type
3. Set the *Display type* to either *Rectangular* or *Circular*


Creating a new Phylogenetic Tree
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

From the :term:`Admin menu` go to *Content > Tree > Add*

In the *Tree* field, enter some valid neXML data.  For more information and some example NeXML files, see `NeXML Github`_.

.. only:: training

    The :ref:`training-material` includes an example NeXML file *dogfish.nex.xml*, but if you have your own NeXML data please feel free to use it. 


.. _`NeXML Github`: https://github.com/nexml/nexml

