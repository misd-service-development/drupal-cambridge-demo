<?php
/**
 * @file
 * cambridge_news.features.inc
 */

/**
 * Implements hook_views_api().
 */
function cambridge_news_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function cambridge_news_node_info() {
  $items = array(
    'news_article' => array(
      'name' => t('News Article'),
      'base' => 'node_content',
      'description' => t('Use <em>news article</em> to add news to your site.'),
      'has_title' => '1',
      'title_label' => t('Article title'),
      'help' => '',
    ),
  );
  return $items;
}
