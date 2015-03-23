<?php
/**
 * @file
 * JPEGTran integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_jpegtran_info() {
  return array(
    'title' => t('JPEGTran'),
    'url' => 'http://ijg.org/',
    'type' => 'jpeg',
    'settings' => array(
      'progressive' => FALSE,
    ),
    'weight' => 0,
  );
}

/**
 * JPEGTran ImageAPI Optimize form callback.
 */
function imageapi_optimize_binaries_jpegtran_form($settings) {
  $form = array();

  $form['progressive'] = array(
    '#title' => t('Progressive'),
    '#type' => 'checkbox',
    '#default_value' => $settings['progressive'],
  );

  return $form;
}

/**
 * JPEGTran ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_jpegtran($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/jpeg') {
    $args = '';

    if ($settings['progressive']) {
      $args .= '-progressive ';
    }

    _imageapi_optimize_exec("$cmd -copy none -optimize " . $args . escapeshellarg($dst), $dst);
  }
}
