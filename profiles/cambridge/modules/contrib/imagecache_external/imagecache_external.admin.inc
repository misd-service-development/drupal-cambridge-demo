<?php
/*
 * @file
 *
 * Admin forms for this module.
 *
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands contact at rowlandsgroup dot com
 */

/**
 * Form builder.
 */
function imagecache_external_admin_form($form, $form_state) {
  $form = array();

  $form['imagecache_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Imagecache Directory'),
    '#required' => TRUE,
    '#description' => t('Where, withing the files directory, should the downloaded images be stored?'),
    '#default_value' => variable_get('imagecache_directory', 'externals'),
  );

  $form['imagecache_external_management'] = array(
    '#type' => 'radios',
    '#title' => t('How should Drupal handle the files?'),
    '#description' => t('Managed files can be re-used elsewhere on the site, for instance in the Media Library if you use the Media module. Unmanaged files are not saved to the database, but can be cached using Image Styles.'),
    '#options' => array(
      'unmanaged' => t('Unmanaged: Only save the images to the files folder to be able to cache them. This is  default.'),
      'managed' => t('Managed: Download the images and save its metadata to the database.'),
    ),
    '#default_value' => variable_get('imagecache_external_management', 'unmanaged'),
  );

  $form['imagecache_external_option'] = array(
    '#type' => 'radios',
    '#title' => t('Hosts list option'),
    '#description' => t('Choose the method to filter external urls (note admin user is not affected)'),
    '#options' => array(
      'black' => t('Blacklist: Only deny images from the listed hosts.'),
      'white' => t('Whitelist: Deny all images <b>except</b> the listed hosts.'),
    ),
    '#default_value' => variable_get('imagecache_external_option', 'white'),
  );

  $form['imagecache_external_hosts'] = array(
    '#type' => 'textarea',
    '#title' => t('Imagecache External hosts'),
    '#description' => t('Add one host per line. Ex: www.flickr.com'),
    '#default_value' => variable_get('imagecache_external_hosts', ''),
  );
  return system_settings_form($form);
}

function imagecache_external_admin_form_validate($form, $form_state) {
  $scheme = file_default_scheme();
  $directory = $scheme . '://' . $form_state['values']['imagecache_directory'];
  if (!file_prepare_directory($directory, FILE_CREATE_DIRECTORY)) {
    form_set_error('imagecache_directory', t('The directory %directory does not exist or is not writable.', array(
      '%directory' => $directory,
    )));
    watchdog('imagecache_external', 'The directory %directory does not exist or is not writable.', array('%directory' => $directory), WATCHDOG_ERROR);
  }
}

/**
 * Form builder.
 */
function imagecache_external_flush_form($form, $form_state) {
  return confirm_form($form,
    t('Flush all external images?'),
    'admin/config/media/imagecache_external',
    t('Are you sure? This cannot be undone.'),
    t('Flush'),
    t('Cancel')
  );
}

/**
 * Submit handler.
 */
function imagecache_external_flush_form_submit($form, &$form_state) {
  if (imagecache_external_flush_cache()) {
    drupal_set_message(t('Flushed external images'));
  }
  else {
    drupal_set_message(t('Could not flush external images'), 'error');
  }
  $form_state['redirect'] = 'admin/config/media/imagecache_external';
}
