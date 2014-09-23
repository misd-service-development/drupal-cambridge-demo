<?php

// Use SQLite database.
$databases = array(
  'default' =>
  array(
    'default' =>
    array(
      'database' => 'sites/default/files/.ht.sqlite',
      'driver' => 'sqlite',
      'prefix' => '',
    ),
  ),
);

// Cache in the filesystem to avoid problems when moving between locations/servers.
$conf['cache_backends'] = array(conf_path() . '/modules/filecache/filecache.inc');
$conf['cache_default_class'] = 'DrupalFileCache';
$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
$conf['filecache_directory'] = sys_get_temp_dir() . '/drupal-demo-' . hash('sha1', DRUPAL_ROOT);

// Also store sessions in the cache.
$conf['session_inc'] = conf_path() . '/modules/session_proxy/session.inc';
$conf['session_storage_class'] = 'SessionProxy_Storage_Cache';
$conf['session_storage_options']['cache_backend'] = 'DrupalFileCache';

// Prevent overwriting of the file temporary path to avoid problems when moving between OSes.
$conf['file_temporary_path'] = sys_get_temp_dir();

// In case URL rewriting isn't available, disable clean URLs.
$conf['clean_url'] = 0;

# Don't block any IP addresses.
$conf['blocked_ips'] = array();

# Stop Drupal trying to run cron.
$conf['cron_safe_threshold'] = 0;
