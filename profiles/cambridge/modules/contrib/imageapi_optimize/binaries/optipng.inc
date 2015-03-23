<?php
/**
 * @file
 * OptiPNG: Advanced PNG Optimizer integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_optipng_info() {
  return array(
    'title' => t('OptiPNG: Advanced PNG Optimizer'),
    'url' => 'http://optipng.sourceforge.net/',
    'type' => 'png',
    'settings' => array(
      'level' => 5,
      'interlace' => '',
    ),
    'weight' => 1,
  );
}

/**
 * OptiPNG: Advanced PNG Optimizer ImageAPI Optimize form callback.
 */
function imageapi_optimize_binaries_optipng_form($settings) {
  $form = array();

  $form['level'] = array(
    '#title' => t('Optimization level'),
    '#type' => 'select',
    '#options' => range(0, 7),
    '#default_value' => $settings['level'],
  );

  $form['interlace'] = array(
    '#title' => t('Interlace'),
    '#type' => 'select',
    '#options' => array(
      '' => t('No change'),
      0 => t('Non-interlaced'),
      1 => t('Interlaced'),
    ),
    '#default_value' => $settings['interlace'],
    '#description' => t('If "No change" is select, the output will have the same interlace type as the input.'),
  );

  return $form;
}

/**
 * OptiPNG: Advanced PNG Optimizer ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_optipng($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/png') {
    $args = '';

    $args .= '-o' . $settings['level'] . ' ';

    if (is_numeric($settings['interlace'])) {
      $args .= '-i ' . $settings['interlace'] . ' ';
    }

    exec("$cmd -quiet " . $args . escapeshellarg($dst));
  }
}
