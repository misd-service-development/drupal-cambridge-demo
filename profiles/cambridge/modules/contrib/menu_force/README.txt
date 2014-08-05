
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * Known Issues
 * How can you contribute?


INTRODUCTION
------------

Creator: BarisW <http://drupal.org/user/107229>

This module enables you to make Menu Settings required on specific content types

It forces a node from one or more content types to be included in the menu system 
before the content will be saved successfully. This can be useful in a number of 
situations, e.g. when using [menupath-raw] in the pathauto settings, which expects
a node to live in the menu system. This module makes sure it does.
 
Credits for @Keyz for inspiring me with his PHP snippet (http://drupal.org/node/466620)


INSTALLATION
------------

See http://drupal.org/getting-started/install-contrib for instructions on
how to install or update Drupal modules.

- Enable the module
- Navigate to Content Management -> Content Types -> YOUR CONTENT TYPE
- You can force menu settings in the fieldset 'Menu settings'
- When enabled, adding or editing nodes of this content type will be impossible 
  without adding a menu entry.


KNOWN ISSUES
------------

- There are no known issues at this time.


HOW CAN YOU CONTRIBUTE?
---------------------

- Write a review for this module at drupalmodules.com.
  http://drupalmodules.com/module/menu-force

- Help translate this module.
  http://localize.drupal.org/translate/projects/menu_force

- Report any bugs, feature requests, etc. in the issue tracker.
  http://drupal.org/project/issues/menu_force
