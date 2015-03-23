<?php
/**
 * @file
 * Jfifremove integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_jfifremove_info() {
  return array(
    'title' => t('Jfifremove'),
    'url' => 'https://github.com/kormoc/imgopt/blob/master/jfifremove.c',
    'type' => 'jpeg',
    'weight' => 1,
  );
}

/**
 * Jfifremove ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_jfifremove($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/jpeg') {
    _imageapi_optimize_exec("$cmd < " . escapeshellarg($dst), $dst);
  }
}
