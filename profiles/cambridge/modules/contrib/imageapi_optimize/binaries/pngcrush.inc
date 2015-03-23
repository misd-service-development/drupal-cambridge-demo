<?php
/**
 * @file
 * Pngcrush integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_pngcrush_info() {
  return array(
    'title' => t('Pngcrush'),
    'url' => 'http://pmt.sourceforge.net/pngcrush/',
    'type' => 'png',
    'weight' => 2,
  );
}

/**
 * Pngcrush ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_pngcrush($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/png') {
    $temp = drupal_realpath(drupal_tempnam('temporary://', 'file'));
    exec("$cmd -rem alla -reduce -brute -q " . escapeshellarg($dst) . " " . escapeshellarg($temp) . " && mv " . escapeshellarg($temp) . " " . escapeshellarg($dst));
  }
}
