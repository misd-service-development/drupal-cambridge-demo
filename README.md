Demo University of Cambridge Drupal 7 site
==========================================

This is a complete Drupal site that is designed to show how a University of Cambridge site can be put together using publically-available modules and University-specific releases. It uses the [University of Cambridge full install profile](https://github.com/misd-service-development/drupal-cambridge-profile), which provides and configures a lot of common functionality (including the [theme](https://github.com/misd-service-development/drupal-cambridge-theme)).

What it contains
----------------

- Latest version of Drupal (currently [7.31](https://www.drupal.org/drupal-7.31-release-notes)).
- Latest version of the University of Cambridge full install profile (currently [1.1](https://github.com/misd-service-development/drupal-cambridge-profile/releases/tag/7.x-1.1-full)).
- Demo content/configuration.

The Drupal core and the install profile will be kept up to date. More content/examples will also be added over time; if you have any suggestions please email webmaster@admin.cam.ac.uk or ideally [open an issue](https://github.com/misd-service-development/drupal-cambridge-demo/issues).

How to install
--------------

The demo site can be downloaded from GitHub (as a [ZIP](https://github.com/misd-service-development/drupal-cambridge-demo/archive/master.zip) or a [compressed TAR](https://github.com/misd-service-development/drupal-cambridge-demo/archive/master.tar.gz)).

It can be installed anywhere [Drupal 7 can normally run](https://www.drupal.org/requirements) (for the database it uses SQLite). To install, simply put it somewhere behind a server.

For example, if you have Apache running locally you can put it in a folder called `drupal-demo` in your document root, then simply open `http://localhost/drupal-demo/` in your browser.

In you're using PHP 5.4 or later, you can use the built-in web server instead. Having executed `php -S localhost:8888` in the directory, for example, it would then be accessible at `http://localhost:8888/`

How to use
----------

The site contains one user account (username 'admin', password 'password') which can be used to see how the parts are put together (eg see the content types, blocks, Views).

It is not designed to be a starting point for a site (choices have been made to make it easily distributable which would vastly decrease performance as a production system), but instead to be used an inspiration.

It also isn't designed to show the 'best way' to achieve a particular outcome, as there are often many ways of doing so. It does, however, show reasonably simplistic ways of doing common things, such as how a list of teasers can be produced using a Nodequeue-powered View.

Notes
-----

As this site is designed to be easily distributable, it uses a SQLite database contained in the filesystem (and so is part of the Git repository). It uses the [File Cache](https://www.drupal.org/project/filecache) and [Session Proxy](https://www.drupal.org/project/session_proxy) modules to try and stop volatile data from being stored in the database. These modules aren't particularly mature, so *might* create problems on some systems (though testing on a variety of OSes hasn't shown any).

Clearly this setup is not recommended for production systems, it has purely been used to allow the site to be downloaded and viewed anywhere without having to perform any other tasks.
