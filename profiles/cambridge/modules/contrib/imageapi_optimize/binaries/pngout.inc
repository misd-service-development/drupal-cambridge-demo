<?php
/**
 * @file
 * PNGOUT integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_pngout_info() {
  return array(
    'title' => t('PNGOUT'),
    'url' => 'http://advsys.net/ken/utils.htm',
    'type' => 'png',
    'weight' => 5,
  );
}

/**
 * pngquant ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_pngout($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/png') {
    exec("$cmd " . escapeshellarg($dst));
  }
}
