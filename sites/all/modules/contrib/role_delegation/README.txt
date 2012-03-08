
README file for the Role Delegation Drupal module.


Description
***********

This module allows site administrators to grant some roles the authority to
assign selected roles to users, without them needing the 'administer
permissions' permission.

For each role, Role Delegation provides a new 'assign <ROLE> role' permission to
allow the assignment of that role.

The module also adds an 'assign all roles' permission. Enabling this permission
for a role is a convenient way to allow the assignment of any other role without
having to check all the 'assign <ROLE> role' permissions in the Permissions
page.

If an administrator has the 'administer users' permission, a role assignment
widget gets displayed in the account creation or editing form, and bulk
add/remove role operations become available on the user administration page.
Otherwise, if s/he has at least the 'access user profiles' permission, the
module adds its own 'Roles' tab to the user profile so that roles can be
assigned.


Installation
************

1. Extract the 'role_delegation' module directory, including all its
   subdirectories, into your Drupal modules directory.

2. Go to the Administer > Site building > Modules page, and enable the module.

3. Go to the Administer > User management > Permissions and scroll down to
   the role_delegation group of permissions. Each role now has a corresponding
   'assign <ROLE> role' permission. Grant this permission to roles that shall have
   the power to assign role ROLE to users.

