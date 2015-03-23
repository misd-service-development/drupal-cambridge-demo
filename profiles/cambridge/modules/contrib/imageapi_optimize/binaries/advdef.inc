<?php
/**
 * @file
 * AdvanceCOMP Deflate Compression Utility integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_binaries_advdef_info() {
  return array(
    'title' => t('AdvanceCOMP Deflate Compression Utility'),
    'url' => 'http://advancemame.sourceforge.net/comp-download.html',
    'type' => 'png',
    'settings' => array(
      'recompress' => TRUE,
      'mode' => 4,
    ),
    'weight' => 4,
  );
}

/**
 * AdvanceCOMP Deflate Compression Utility ImageAPI Optimize form callback.
 */
function imageapi_optimize_binaries_advdef_form($settings) {
  $form = array();

  $form['recompress'] = array(
    '#title' => t('Recompress'),
    '#type' => 'checkbox',
    '#default_value' => $settings['recompress'],
  );

  $form['mode'] = array(
    '#title' => t('Compression mode'),
    '#type' => 'select',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Fast'),
      2 => t('Normal'),
      3 => t('Extra'),
      4 => t('Insane'),
    ),
    '#default_value' => $settings['mode'],
  );

  return $form;
}

/**
 * AdvanceCOMP Deflate Compression Utility ImageAPI Optimize binary callback.
 */
function imageapi_optimize_binaries_advdef($image, $dst, $cmd, $settings) {
  if ($image->info['mime_type'] == 'image/png') {
    $args = '';

    if ($settings['recompress']) {
      $args .= 'z';
    }

    $args .= $settings['mode'];

    exec("{$cmd} -" . $args . 'q ' . escapeshellarg($dst), $return, $output);
  }
}
