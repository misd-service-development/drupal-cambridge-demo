<?php
/**
 * @file
 * cambridge_questions_and_answers.features.inc
 */

/**
 * Implements hook_node_info().
 */
function cambridge_questions_and_answers_node_info() {
  $items = array(
    'questions_and_answers' => array(
      'name' => t('Questions and Answers'),
      'base' => 'node_content',
      'description' => t('Use <em>questions and answers</em> to create FAQ-style pages.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
