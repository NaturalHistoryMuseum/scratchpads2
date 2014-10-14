VERSION: CVS HEAD, compatible with D7, dev release use at own risk.

OVERVIEW
--------

Tac_lite stands for Taxonomy Access Control Lite.  This module
restricts access so that some users may see content that is
hidden from others.  A simple scheme based on taxonomy, roles and
users controls which content is hidden.

As the name implies, this module shares some functionality with an
earlier module called Taxonomy Access Control (TAC).  If you are
shopping around for an access control module to use, consider that one
as you may find that it suits your needs.  In my case, I wanted access
control but without some of the complexity introduced by TAC.  I also
wanted more flexibility in granting access on a per user basis.

Here are some key features of tac_lite:

* Designed to be as simple as possible in installation and administration.

* Uses Drupal's node_access table, db_rewrite_sql hook and
  taxonomy module to leave the smallest possible footprint while doing
  it's job.  For example, it introduces no new database tables.

* Grant permissions based on roles.

* Grant permissions per user.  (Give a specific user access beyond
  what his/her roles allow).

* Supports view, update and delete permissions.

USE CASE
--------

Here's how I originally used this module.  This description might make
it easier to understand why one might prefer tac_lite over TAC.

My website helps me manage my work projects.  I use Drupal's project
module to track issues.  Some of my projects are for the public to see
(i.e. Drupal modules) others are limited to my clients and partners.
These restricted projects should be visible only to me, the client in
question, and partner(s) working on that particular project.

I've defined a vocabulary for my projects (same one used by
project.module) and I've defined a client role and a partner role.
Partners can contribute to the website, while clients can read content
but post only issues.

Using TAC (or as far as I know all other access control modules) I
would have to create a new role for each project/role combination.
That is, for the Acme project I'd have to create roles 'Acme Client'
and 'Acme Partner' in order to assign permissions just the way I want
them.

Using tac_lite, I simply associate each user with the project(s) they
are allowed to see.  That is, I associate some clients and some
partners with Acme.  Their role (client or partner) controls what they
can do, and the associations through tac_lite control what they can
see.

INSTALL
-------

Enable taxonomy module.  It's required.

Install this package the normal way.
- put this file in a subdirectory of the modules directory.
- enable using admin interface
- no database tables to install.


USAGE
-----

Log in as an administrator. (uid==1, or a user with
administer_tac_lite permission)

Create a vocabulary which you will use to categorize private nodes.
You may want to create a vocabulary called "Privacy" with terms like
"public", "private", and "administers only".

Associate the vocabulary with node types, as you would normally do.

Go to administer >> user management >> access control >> access
control by taxonomy.

Select the category you created in the earlier step ("Privacy").

Create some content.  Choose a node type you've associated with "Privacy".

Note that you can view the content you just created.  Other users cannot.

Edit the account of another user.  Go to the tac_lite access tab under edit.

Select a term you selected when creating the node and submit changes.

Now the user can also access the node you created.


NOTES
-----

If behavior of this or any other access control module seems to be
incorrect, try rebuilding the node access table. This may be done
under administer >> content management >> post settings.  There is a
button there labelled "rebuild permissions"

Another useful tool is a sub-module of the devel module, called
devel_node_access which can give you some insight into the contents of
your node_access table.  Recommended for troubleshooting.


AUTHOR
------

Dave Cohen <http://drupal.org/user/18468>
http://www.dave-cohen.com
