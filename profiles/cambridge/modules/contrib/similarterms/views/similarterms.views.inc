<?php

/**
 * Implements hook_views_data()
 **/
function similarterms_views_data_alter(&$data) {
  $data['similarterms']['similarterms']['moved to'] = array('node', 'similarterms');
  $data['node']['similarterms'] = array(
    'title' => t('Similarity'),
    'group' => t('Similar By Terms'),
    'help' => t('Percentage/count of terms which node has in common with node given as argument.'),
    'field' => array(
      'handler' => 'similarterms_handler_field_similar',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'similarterms_handler_sort_similar',
    ),
  );
  $data['similarterms']['nid']['moved to'] = array('node', 'similar_nid');
  $data['node']['similar_nid'] = array(
    'title' => t('Nid'),
    'group' => t('Similar By Terms'),
    'help' => t('ID of content item(s). Passes term ids to Similar By Terms.'), // The help that appears on the UI,
    'argument' => array(
      'handler' => 'similarterms_handler_argument_node_nid',
      // 'parent' => 'views_handler_argument_numeric', // make sure parent is included
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
  );
}
