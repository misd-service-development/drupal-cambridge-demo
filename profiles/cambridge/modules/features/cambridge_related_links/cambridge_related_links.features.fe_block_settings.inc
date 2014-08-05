<?php
/**
 * @file
 * cambridge_related_links.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function cambridge_related_links_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['views-related_links-block'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'related_links-block',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '',
    'roles' => array(),
    'themes' => array(
      'cambridge_theme' => array(
        'region' => 'sidebar',
        'status' => 1,
        'theme' => 'cambridge_theme',
        'weight' => -10,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 0,
  );

  return $export;
}
