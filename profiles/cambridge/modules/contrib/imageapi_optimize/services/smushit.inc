<?php
/**
 * @file
 * Yahoo! Smush.it service integration.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_services_smushit_info() {
  return array(
    'title' => t('Yahoo! Smush.it'),
    'url' => 'http://smush.it',
  );
}

/**
 * Smush.it ImageAPI Optimize service callback.
 */
function imageapi_optimize_services_smushit($image, $dst) {
  if (!function_exists('json_decode')) {
    drupal_set_message(t('Required function, json_decode(), is not available.'), 'error');
    return FALSE;
  }
  $dst = drupal_realpath($dst);
  $url = 'http://www.smushit.com/ysmush.it/ws.php';
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, array('files' => '@' . $dst));
  $data = curl_exec($ch);
  curl_close($ch);

  $json = json_decode($data);

  // SmushIt returns an error if it cannot optimize the image. Otherwise, it
  // returns an object, with 'dest' (temporary file) and 'percent' (savings)
  // among other properties.
  if (!isset($json->error)) {
    $result = drupal_http_request($json->dest);
    if (!isset($result->error)) {
      file_unmanaged_save_data($result->data, $dst, FILE_EXISTS_REPLACE);
      return TRUE;
    }
  }
}
