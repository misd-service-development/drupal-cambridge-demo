========================
ABOUT
------------------------
Imagecache External is a utility module that allows you to programatically
process external images through image styles.


========================
INSTALLATION
------------------------
  1. Extract the module to sites/all/modules or sites/xx/modules depending on
     whether or not you have a multisite installation.
  2. Enable the module at admin/modules or use drush.


========================
CONFIGURATION
------------------------
The module's default configuration is very restrictive. Exclude admin user
or users with the 'Bypass black/white list' permission - the default
configuration of the module is to deny all requests to fetch external images.

To get the module to work, you need to visit
admin/config/media/imagecache_external and either:

 - Add some domains to the whitelist -or-
 - Switch the mode of operation from whitelist to blacklist


========================
 USAGE INSTRUCTIONS
------------------------
 In your module or theme, you may call the following theme function to
 process an image via Imagecache External:

 <?php
  print theme('imagecache_external', array(
    'path' => 'https://drupal.org/files/druplicon.large_.png',
    'style_name'=> 'thumbnail',
    'alt' => 'Drupalicon'
  ));

You can also use external images without coding at all by adding an Text or
Link field to a Node Type and then use the Imagecache External Image formatter.


========================
ADDITIONAL RESOURCES
------------------------
View the Imagecache External project page for additional information
https://drupal.org/project/imagecache_external
