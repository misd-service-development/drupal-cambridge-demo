<?php
/**
 * @file
 * cambridge_news.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function cambridge_news_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['views-news-block'] = array(
    'cache' => -1,
    'custom' => 0,
    'delta' => 'news-block',
    'module' => 'views',
    'node_types' => array(),
    'pages' => '<front>',
    'roles' => array(),
    'themes' => array(
      'cambridge_theme' => array(
        'region' => 'sub_content',
        'status' => 1,
        'theme' => 'cambridge_theme',
        'weight' => 0,
      ),
      'seven' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'seven',
        'weight' => 0,
      ),
    ),
    'title' => '',
    'visibility' => 1,
  );

  return $export;
}
