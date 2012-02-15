QUICK START GUIDE
-----------------
1. Create a View.
2. Add a "Bulk operations" field if available (see technical details below).
3. Configure the field. There's a "Views Bulk Operations" fieldset where the
actions visible to the user are selected.
4. Go to the View page. VBO functionality should be present.

Note that your old (D6, pre-alpha D7) views that used the VBO style plugin
instead of the VBO field will need to be recreated.

TECHNICAL DETAILS
-----------------
The module relies on the Views 3 "form" feature available in all Views releases after May 28th 2011.
The selection field (checkbox / radio) is provided as a views field called "Bulk operations".
The field can use a relationship, allowing you to have a node view with
operations on node authors, for example.
Note that currently a View can only have one VBO field.
Even if the view has several VBO fields, only the first one found will be used.

The "Bulk operations" views field is available in Views for all entity base tables,
since VBO relies on entities directly and no longer has its own "object" abstraction.

There is Drush integration available, allowing you to run an action against the
resultset of any View that has the VBO field in its master display.

Operations are gathered from two sources:
1) Drupal core actions (hook_action_info() and advanced actions added through the Actions UI)
2) Rules 2
Note that VBO no longer supports hook_node_operations() and hook_user_operations()
which were supported in previous releases.

RULES 2 INTEGRATION
-------------------
The module can execute any created Rules component (rule, ruleset, action set)
that accepts an entity (example types: "node", "entity") or a list entity type
(example types: "list<node>", "list<entity>") as the first parameter.
As a reminder, Rules components can be created at admin/config/workflow/rules/components.

A Rules action is provided that loads a list of entities from a VBO View.
That list can then be iterated on in Rules, and used in further actions.
The purpose of this action is to replace the old "Execute VBO" action with a
more elegant solution.
There is also a Rules condition for checking the number of results returned
by a VBO View.

AGGREGATION
-----------
By default, VBO passes only one entity at a time to the operation.
This allows the entity loading to be segmented into chunks, avoiding memory
limits and timeouts. However, some operations need all selected entities to be
passed at once, which requires aggregation to be turned on.
For core actions that means setting "aggregate" => TRUE in your hook_action_info()
implementation. For Rules components, that means requiring a list entity type
such as list<node> as the first parameter.

Loading can't be segmented when aggregation is on, so the usual methods of
executing (Batch API, Drupal Queue) are bypassed and all entities are loaded
at once, making it possible to hit the memory limit.
That's why aggregation should only be enabled for actions that require a smaller
amount of items to be selected.
See the VBO action "Pass ids as arguments to a page" in
actions/argument_selector.action.inc for an example implementation.

CONTEXT
-------
By default (for performance reasons), VBO doesn't pass the selected views rows
to actions.

However, when a Drupal core action declares 'pass rows' => TRUE in its
definition (hook_action_info()), VBO does pass the full rows through the $context array.
So $context['rows'] has an array of selected rows in the form of $row_index => $views_row.
If the action is using aggregation, $context['rows'] will include all selected
rows. Otherwise, only the current row (that is being operated on) will be included.

Using this feature has a memory cost and is not recommended for actions
that process a big number of rows. Also, if all rows on all pages are selected,
only the rows from the first page will be passed through. This is a known issue.

Right now there is no way for a Rules 2 component to receive context, but there
are plans to change that.

EXECUTION METHODS
-----------------
When configuring the VBO field, the following setting can be seen:
"Number of entities to load at once", set to 10 by default.
When the number of selected items is less than that, the entities are laoded
all at once, and appropriate operations are fired.
When the number of selected items is more than that, Batch API is used to load
the entities in smaller groups (the size of which is taken from that setting),
and a progress bar is shown.

Alternatively, the user can choose to use the Drupal Queue, by enabling the
"Enqueue the operation instead of executing it directly" checkbox for each
desired action in the VBO field settings.
The entities and their operation are then enqueued one by one, to be processed
by the queue worker (which usually happens when cron is run).
This is useful for postponing long running operations.

EXAMPLE VIEWS
-------------
VBO comes with two default views, reimplementing the Content and User listings.
They are disabled by default. After enabling them at admin/structure/views
they can be accessed at admin/content2 and admin/people2.

ACTIONS PERMISSIONS
-------------------
A module called actions_permissions is included in the package.
This module generates a permission for each core action, and VBO honors those
permissions before showing or executing the corresponding actions.
This is useful if you want to provide your VBO to several groups of users with
different privileges: the same view will accommodate those different groups,
showing to each the actions that they are permitted to see.

Rules components still don't support something like this, but there's an
open feature request in the Rules issue queue: http://drupal.org/node/1217128
