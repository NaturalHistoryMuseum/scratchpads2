Managing comments
=================

Introduction
------------

The commenting functionality in Scratchpads is a valuable system that
enables registered and/or anonymous users to comment on a piece of
content (usually a `node`_). Maintainers and editors of a site can
control the comment functionality per content type and per user role.
This means that a Scratchpad site might have comments enabled for a
content type (e.g. taxon descriptions) but not for another content type
(e.g. specimens/observations).

Instructions
------------

Control comments access level
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Maintainers can change the roles that are permitted to submit comments
to a site.

1. From the :term:`Admin menu` go to *People*

2. Under *Comment* select the user groups, according to their roles,
   that you wish to be able to post comments

3. If *anonymous users* are selected. Visitors of the site will be able
   to submit comments to be published. By default comments by anonymous
   users need the maintainer’s approval before published in the site

4. Click *Save permissions*

Enable comments for specific content types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Enabling comments for specific content types will not override the
access level settings described in the previous section.

1. From the :term:`Admin menu` go to *Structure > Content types*
2. Click *edit* next to the content type you wish to modify comment
   settings
3. Select *Comment settings* from the vertical tab menu
4. Set specific properties for the comments of this content type

   -  Closed: No comments can be submitted
   -  Open: Anyone can submit a comment provided they have given access,
      as described in the previous section
   -  Hidden: Comments can be submitted, but they are not published

Comment notifications
~~~~~~~~~~~~~~~~~~~~~

You can modify the settings that define when someone should be notified,
via email, when a new comment is posted and also how the notification
email is drafted.

1. From the :term:`Admin menu` go to *Configuration > Comment notify*
2. Change settings accordingly and click *Save configuration*

Managing comments
~~~~~~~~~~~~~~~~~

Comments submitted by anonymous users are marked as unapproved by
default and do not get published until a maintainer has approved them.
To approve or delete comments go to

1. Admin menu > Content > Comments
2. All published comments are located and can be managed under
   *Published comments*

.. figure:: /_static/Comment_permissions.png

3. All unapproved comments can be managed (approved or deleted) under
   the *Unpublished comments* section
4. Numbers in brackets indicate the total number of comments under these
   two sections