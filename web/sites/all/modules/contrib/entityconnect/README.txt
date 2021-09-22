Entityconnect expands the entity reference auto-complete field
by adding a add new content and edit current content button.

The "add a new button" will allowed a new entity to be added
via an add form and then return the user to the original form.

The edit button will take the user to the edit form of the referenced entity,
and return them when they are done editing.

Original concept and piece of code based on NodeConnect.

List of working entities on form:
  - Node
  - User

Installation
------------
1. Copy entityconnect into your modules directory and then enable on the admin
modules page
2. Define permissions on admin/people/permissions page
3. Go to you form which contain entityreference, you should see an add and/or
edit button on the right of line.


FAQ
---
Q. I have define "see add button" permissions but can't see the add button
on my entityreference which point on an content type.
A. When only one content type is defined on entityreference field, module
also check if user has the permission to "create N content" where N is
the content type or has the permission "administer nodes".


Next steps
----------

7.x-1.x-RC or Stable will come when all core entities will be supports.

Release 7.x-2.x goals are:
- Add Ctools support,
- Add Commerce Entities and reference fields for product reference fields,
- Add compatibility with Entites UI like
  1. ECK (Entity Construction Kit)(http://drupal.org/project/eck),
  2. Model Entities (http://drupal.org/project/model)
