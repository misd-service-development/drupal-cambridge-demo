<?php
/**
 * @file
 * ImageAPI Optimize module integration.
 */

/**
 * Implements hook_imageapi_optimize_info().
 */
function imageapi_optimize_imageapi_optimize_info() {
  $binaries = _imageapi_optimize_info_scan('binaries');
  $services = _imageapi_optimize_info_scan('services');

  return array(
    'binaries' => $binaries,
    'services' => $services,
  );
}

/**
 * Helper function; Scan relevant directories and return info from includes.
 */
function _imageapi_optimize_info_scan($type) {
  $info = array();

  // Scan relevant directory for includes.
  $files = file_scan_directory(drupal_get_path('module', 'imageapi_optimize') . '/' . $type, '/\.inc/');
  foreach ($files as $file) {
    include_once $file->uri;

    // Get info from relevant include.
    $function = 'imageapi_optimize_' . $type . '_' . $file->name . '_info';
    if (function_exists($function)) {
      $return = $function();
      if (is_array($return)) {
        $info[$file->name] = $return + array(
          'file' => $file->uri,
        );
      }
    }
  }

  return $info;
}
