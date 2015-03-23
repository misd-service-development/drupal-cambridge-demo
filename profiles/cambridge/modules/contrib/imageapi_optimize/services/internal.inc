<?php
/**
 * @file
 * Internal ImageAPI Optimize service.
 */

/**
 * Implements imageapi_optimize_TYPE_NAME_info().
 */
function imageapi_optimize_services_internal_info() {
  return array(
    'title' => t('Internal tools (configurable below)'),
    'weight' => -10,
  );
}

/**
 * Internal ImageAPI Optimize form callback.
 */
function imageapi_optimize_services_internal_form($info) {
  $form = array();
  $form['types'] = array(
    '#type' => 'vertical_tabs',
  );

  foreach ($info['binaries'] as $name => $binary) {
    $settings = variable_get('imageapi_optimize_' . $name, array());

    if (!isset($form['types'][$binary['type']])) {
      $form['types'][$binary['type']] = array(
        '#title' => t('@type Settings', array('@type' => drupal_strtoupper($binary['type']))),
        '#type' => 'fieldset',
        '#theme' => 'imageapi_optimize_services_internal_form',
      );
    }

    $form['types'][$binary['type']]['imageapi_optimize_' . $name] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    $form['types'][$binary['type']]['imageapi_optimize_' . $name]['status'] = array(
      '#title' => t('Enabled'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['status']) ? $settings['status'] : FALSE,
      '#title_display' => 'invisible',
    );

    $form['types'][$binary['type']]['imageapi_optimize_' . $name]['title'] = array(
      '#markup' => t('@title (@name)', array(
        '@title' => $binary['title'],
        '@name' => $name,
      )),
    );
    if (isset($binary['url'])) {
      $form['types'][$binary['type']]['imageapi_optimize_' . $name]['title']['#markup'] = l($form['types'][$binary['type']]['imageapi_optimize_' . $name]['title']['#markup'], $binary['url']);
    }

    $form['types'][$binary['type']]['imageapi_optimize_' . $name]['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to @name', array('@name' => $name)),
      '#default_value' => isset($settings['path']) ? $settings['path'] : '',
      '#element_validate' => array('imageapi_optimize_validate_path'),
      '#title_display' => 'invisible',
    );
    if (isset($binary['description'])) {
      $form['types'][$binary['type']]['imageapi_optimize_' . $name]['path']['#description'] = $binary['description'];
    }

    $op = '';
    if (!empty($binary['settings'])) {
      $op = l(t('Advanced settings'), 'admin/config/media/image-toolkit/imageapi_optimize/' . $name);

      $advanced_settings = isset($settings['settings']) ? $settings['settings'] : $binary['settings'];
      $form['types'][$binary['type']]['imageapi_optimize_' . $name]['settings'] = array(
        '#type' => 'value',
        '#value' => $advanced_settings,
      );
    }
    $form['types'][$binary['type']]['imageapi_optimize_' . $name]['op'] = array(
      '#markup' => $op,
    );

    $form['types'][$binary['type']]['imageapi_optimize_' . $name]['weight'] = array(
      '#title' => t('Weight'),
      '#type' => 'weight',
      '#default_value' => $binary['weight'],
      '#title_display' => 'invisible',
    );
  }

  return $form;
}

/**
 * Form callback for Internal binary settings.
 */
function imageapi_optimize_internal_binary_settings($form, $form_state, $name) {
  $info = imageapi_optimize_info();
  if (isset($info['binaries'][$name])) {
    $binary = $info['binaries'][$name];
    $settings = variable_get('imageapi_optimize_' . $name, array());

    // Include binary file if defined and available.
    if (isset($binary['file'])) {
      include_once $binary['file'];
    }

    $form['imageapi_optimize_' . $name] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    $form['imageapi_optimize_' . $name]['status'] = array(
      '#title' => t('Enabled'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['status']) ? $settings['status'] : FALSE,
    );

    $form['imageapi_optimize_' . $name]['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path to @name', array('@name' => $name)),
      '#default_value' => isset($settings['path']) ? $settings['path'] : '',
      '#element_validate' => array('imageapi_optimize_validate_path'),
    );
    if (isset($binary['description'])) {
      $form['imageapi_optimize_' . $name]['path']['#description'] = $binary['description'];
    }

    $form['imageapi_optimize_' . $name]['weight'] = array(
      '#type' => 'value',
      '#value' => $binary['weight'],
    );

    // Binary specific settings form.
    $advanced_settings = isset($settings['settings']) ? $settings['settings'] : $binary['settings'];
    $form['imageapi_optimize_' . $name]['settings'] = array(
      '#title' => t('Advanced settings'),
      '#type' => 'fieldset',
    ) + $binary['callbacks']['form']($advanced_settings);

    return system_settings_form($form);
  }

  // Binary doesn't exist.
  drupal_not_found();
  drupal_exit();
}

/**
 * Title callback for Internal binary settings.
 */
function imageapi_optimize_internal_binary_settings_title($name) {
  return t('@binary advanced settings', array('@binary' => $name));
}

/**
 * Internal ImageAPI Optimize service callback.
 */
function imageapi_optimize_services_internal($image, $dst) {
  $dst = drupal_realpath($dst);

  $info = imageapi_optimize_info();
  foreach ($info['binaries'] as $name => $binary) {
    $settings = variable_get('imageapi_optimize_' . $name, array());
    if (isset($settings['status']) && $settings['status'] && $settings['path']) {
      // Include binary file if defined and available.
      if (isset($binary['file'])) {
        include_once $binary['file'];
      }

      $advanced_settings = isset($settings['settings']) ? $settings['settings'] : $binary['settings'];
      $binary['callbacks']['process']($image, $dst, $settings['path'], $advanced_settings);
    }
  }

  return TRUE;
}

/**
 * Saves result of a command into file.
 */
function _imageapi_optimize_exec($cmd, $dst) {
  ob_start();
  passthru($cmd);
  $output = ob_get_contents();
  ob_end_clean();

  file_unmanaged_save_data($output, $dst, FILE_EXISTS_REPLACE);
}

/**
 * Validation callback for binary fields.
 */
function imageapi_optimize_validate_path($element) {
  $errors = _imageapi_optimize_check_path($element['#value']);
  if ($errors) {
    form_set_error($element['#parents'][0], implode('<br />', $errors));
    return FALSE;
  }
  return TRUE;
}

/**
 * Checks if a path exists and is executable
 *
 * Warning: it does not check if the command is harmful.
 * You should keep an eye on users with "administer imageapi" permission
 */
function _imageapi_optimize_check_path($path) {
  $errors = array();

  if (!empty($path)) {
    if (!is_file($path)) {
      $errors[] = t('The specified path %file does not exist.', array('%file' => $path));
    }
    if (!$errors && !is_executable($path)) {
      $errors[] = t('The specified path %file is not executable.', array('%file' => $path));
    }
    $open_basedir = ini_get('open_basedir');
    if ($errors && $open_basedir) {
      $errors[] = t('PHP\'s <a href="!open-basedir">open_basedir</a> security restriction is set to %open-basedir, which may be interfering with attempts to locate %file.', array('%file' => $path, '%open-basedir' => $open_basedir, '!info-link' => url('http://php.net/features.safe-mode#ini.open-basedir')));
    }
  }

  return $errors;
}

/**
 * Theme callback for Internal ImageAPI Optimize form.
 */
function theme_imageapi_optimize_services_internal_form($variables) {
  $output = '';
  $form = $variables['form'];
  list(, $type) = explode('-', $form['#id']);

  $rows = array();
  foreach (element_children($form) as $name) {
    $row = array();
    $form[$name]['weight']['#attributes']['class'] = array($type . '-binary-weight');

    $row[] = drupal_render($form[$name]['status']);
    $row[] = drupal_render($form[$name]['title']);
    $row[] = drupal_render($form[$name]['path']);
    $row[] = drupal_render($form[$name]['op']);
    $row[] = drupal_render($form[$name]['weight']);

    $rows[] = array(
      'data' => $row,
      'class' => array('draggable'),
    );
  }

  $header = array('', t('Binary'), t('Path'), '', t('Weight'));
  $id = $type . '-binaries-table';
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $id),
  ));

  drupal_add_tabledrag($id, 'order', 'subling', $type . '-binary-weight');

  return $output;
}
