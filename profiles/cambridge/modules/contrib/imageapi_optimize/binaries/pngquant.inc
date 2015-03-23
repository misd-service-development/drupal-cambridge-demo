<?php
/**
 * @file
 * pngquant integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_pngquant_info() {
  return array(
    'title' => t('pngquant'),
    'url' => 'http://pngquant.org/',
    'type' => 'png',
    'weight' => 0,
  );
}

/**
 * pngquant ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_pngquant($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/png') {
    exec("$cmd --speed=1 --quality=90-99 --force --ext .png " . escapeshellarg($dst));
  }
}
