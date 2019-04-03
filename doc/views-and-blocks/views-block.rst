View blocks
===========

As we learnt in :doc:`/views-and-blocks/views` the views module allows you to create, manage, and display lists of content.*

These views of content can be a :term:`Block` - a small lists of information that can be embedded on other pages.

We will create a block to display the subordinate taxa of a classification term, and display it on species page.


Enable views interface
----------------------

If you haven't already, enable the views interface:

1. Go to *Structure* in the :term:`Admin menu`, click on *Tools*, enable ‘Views’ and save.


Create block view
-----------------

1. Within the :term:`Admin menu`, go to *Structure > Views*

2. Select *+ Add new view*

3. Enter the view name - for example "Species subordinate taxa"

4. In the *Show* section, select *Taxonomy terms*, and under *Of type* select the vocabulary you want to use.

5. Deselect *Create a page*

6. Select *Create a block*

7. Enter a block title - for example "Subordinate taxa"

8. Under display format, select *Table*

9. Select *Continue & edit*

10. Click *Save* to make changes permanent


Add relationship and contextual filter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We want to show taxa which are subordinate to the currently viewed taxon.  To do this we need to add a relationship and contextual filter, so we can display terms based on their parent term.

1. On the edit view form, in the *Relationships* section click *Add*.

2. Filter the relationship types by 'parent', and select *Taxonomy term: Parent term*.

    .. figure:: /_static/ViewsAddRelationship.png

3. Select *Apply*.

4. On the next screen leave the defaults (Identifier=Parent), and click *Apply*.

5. In the *Contextual filters* click *Add*.

6. Filter the contextual filters by 'parent', and select *Taxonomy term: Parent term*.

7. Under *Relationship*, select "Parent" (or the identifier value from *Step. 6* if you did not keep the defaults)

8. Select *Provide default value*

9. Select *Taxonomy term ID from URL*

10. Select *Apply*

11. Select *Save* to store your changes permanently.


The view is now ready to use, but we still need to add the block to the page. We'll see how to do this in the next section - :doc:`/views-and-blocks/enabling-blocks`






