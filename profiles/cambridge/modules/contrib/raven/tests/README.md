Running the test suite
----------------------

1. Make sure that [Composer](http://getcomposer.org/) is available.

2. Install the dependencies by executing `composer install`.

3. Configure a server (eg Apache) to make a path resolve (eg `/home/httpd/drupal` as `http://localhost/drupal`). Make sure that `.htaccess` files are allowed.

4. Configure Behat by letting it know that path and URL to use.

5. Run the suite by executing `bin/behat`.

(If in doubt, take a look at the Travis CI config.)
