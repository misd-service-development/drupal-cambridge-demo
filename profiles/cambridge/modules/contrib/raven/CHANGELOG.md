Changelog
=========

7.x-1.3
-------

23 December 2014.

* Make sure user accounts are saved when logging in, even when blocked.
* Hide the user login block on the backdoor login page.
* Add 'is Raven user' Rules condition.
* Prevent double language prefixing of URLs.
* Rewrite generated `user/login` paths to `raven/login` when Raven is enforced.

7.x-1.2
-------

10 September 2014.

* Log the real email address when creating a user in case another module has changed it (from the generic `CRSid@cam.ac.uk` form).
* Let Drupal construct the query string when redirecting to Raven.
* Fixed a typo on the help page.
* Fixed a bug where the `destination` query string parameter was not handled correctly.
* Made compatible with the [Redirect 403 to User Login](https://www.drupal.org/project/r4032login) module.
* Use sensible defaults for variables in case they are unavailable.
* Use php.net short URLs.

7.x-1.1
-------

23 April 2014.

* Made compatible with PHP 5.2.
* Fixed bug where certain characters in the website description (eg ampersands) broke the redirect to the Raven login page.
* Added option to authenticate [Raven for Life accounts](http://www.ucs.cam.ac.uk/accounts/ravenleaving).
* Added option to use the [Demo Raven service](https://demo.raven.cam.ac.uk/).
* Fixed bug where language prefixes in multilingual sites broke login page overriding.
* Fixed bug where disabling clean URLs broke the ability to log in.
* Fixed bug where saving the configuration form could stop the website description from appearing on the Raven login page.
* The login failure redirect path is now validated.

7.x-1.0
-------

13 January 2014.

* Initial release.
