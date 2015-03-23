<?php
/**
 * @file
 * Jpegoptim integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_jpegoptim_info() {
  return array(
    'title' => t('Jpegoptim'),
    'url' => 'https://github.com/tjko/jpegoptim',
    'type' => 'jpeg',
    'settings' => array(
      'progressive' => '',
    ),
    'weight' => 0,
  );
}

/**
 * Jpegoptim ImageAPI Optimize form callback.
 */
function imageapi_optimize_binaries_jpegoptim_form($settings) {
  $form = array();

  $form['progressive'] = array(
    '#title' => t('Progressive'),
    '#type' => 'select',
    '#options' => array(
      '' => t('No change'),
      0 => t('Non-progressive'),
      1 => t('Progressive'),
    ),
    '#default_value' => $settings['progressive'],
    '#description' => t('If "No change" is select, the output will have the same as the input.'),
  );

  return $form;
}

/**
 * Jpegoptim ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_jpegoptim($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/jpeg') {
    $args = '';

    if (is_numeric($settings['progressive'])) {
      switch ($settings['progressive']) {
        case 0:
          $args .= '--all-normal ';
          break;

        case 1:
          $args .= '--all-progressive ';
          break;
      }
    }

    exec("$cmd -q --strip-all " . $args . escapeshellarg($dst));
  }
}
