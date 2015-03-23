
-- SUMMARY --

Small module that allows users to view menu links to unpublished nodes.

The menu system from Drupal core hides menu links that link to nodes that are
unpublished. This modules provides a permission that undoes this behavior, but
only if the user has access to view the (unpublished) node.

-- INSTALLATION --

* Install the module as usual, see
  http://drupal.org/documentation/install/modules-themes/modules-7

* Goto Administration » Users » Permissions, and grant the "menu view unpublished"
  permission to the roles you like (e.g. an editor role).

-- RELATED MODULES--

* View unpublished
  http://drupal.org/project/view_unpublished
  This module allows you to grant permissions to users to view unpublished nodes.
