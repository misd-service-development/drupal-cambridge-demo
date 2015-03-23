<?php

/**
 * @file
 * Views integration of Synonyms module.
 */

/**
 * Implements hook_views_plugins_alter().
 *
 * @see hook_views_plugins_alter()
 */
function synonyms_views_plugins_alter(&$plugins) {
  // Replace default taxonomy term argument validator with our extended version,
  // which can also handle a term synonym as an argument.
  $plugins['argument validator']['taxonomy_term']['handler'] = 'synonyms_views_plugin_argument_validate_taxonomy_term';
}
