Session Proxy
=============

Session proxy is not a module: it is a replacement for Drupal core session
handling API.

It acts as a proxy between fixed core procedural API towards a fully object
oriented, more flexible and easier to extend session management API.

Basically, it strongly encapsulate the original API, separating user management
against raw session storage: by using it you are able to implement any session
storage engine (memcache, redis, ... ) without handling cookies and by users
yourself.

Provided session engines
------------------------

It ships three different session managing implementations:

 1. Database session handling: core default, ported to the object oriented
    API. It comes with an additional SQL query done more than core session.inc
    default implementation due to the API strong encapsulation.

 2. Native PHP session handling: by selecting this session management mode,
    you explicitely remove any core session management, and let PHP manage
    it. This can be useful if you intend to use low-level PHP extensions,
    such as Memcache or PhpRedis to handle session in PHP low-level layer
    instead of core. This allow to use any PHP native session storage to
    work gracefully: some of them are high performance.

 3. Cache based storage engine: Using the same session handling than core,
    but deporting raw session storage into a cache backend instance. You can
    use any already-functionning cache backend to store sessions. Sessions
    will use their own bin, and won't be cleared at cache clear all time.

Installation and configuration
------------------------------

If you intend to use the Drupal default cache implementation (database cache)
for session handling, you need to enable and install the "session_proxy" module
to ensure the database table (bin) for the sessions storage cache backend will
be created. For all other cases, this is unnecessary. 

All configuration will reside into your settings.php file, see the documented
example below for details:

  // Replace core session management with Session Proxy API.
  $conf['session_inc'] = 'sites/all/modules/session_proxy/session.inc';

  // If you set this to TRUE, session management will be PHP native session
  // management. By doing this, all other parameters below will be ignored.
  $conf['session_storage_force_default'] = FALSE;

  // PHP class to use as session storage engine. Default is the database
  // implementation (port of the actual core session management). By setting
  // this parameter, all others settings except 'session_storage_force_default'
  // or 'session_storage_options' will be ignored.
  $conf['session_storage_class'] = 'SessionProxy_Storage_Database';

  // Here is an example of usage the cache storage engine, using the Redis
  // module for storing sessions.
  $conf['session_storage_class'] = 'SessionProxy_Storage_Cache';
  // Everything into 'session_storage_options' are arbitrary key value pairs,
  // each storage backend will define its own keys.
  // For cache backend, the only mandatory one is the class name that to use
  // as cache backend. This class must implement DrupalCacheInterface. If you
  // do not set this class 'DrupalDatabaseCache' will be used. 
  $conf['session_storage_options']['cache_backend'] = 'Redis_Cache';
  // Tell Drupal to load the Redis backend properly, see the Redis module
  // documentation for details about this.
  $conf['cache_backends'][] = 'sites/all/modules/redis/redis.autoload.inc';
  $conf['redis_client_interface'] = 'PhpRedis';

Notes
-----

If you download and properly install the Autoloader Early module, you may
experience better autoloading performances. You can download it at:

  http://drupal.org/project/autoloaderearly

Some cache backend links:

  * Redis - http://drupal.org/project/redis

  * Memcache API and Integration - http://drupal.org/project/memcache
