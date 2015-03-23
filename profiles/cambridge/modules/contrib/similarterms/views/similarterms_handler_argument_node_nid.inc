<?php
/**
 * Argument handler to accept a node id.
 * based on node_handler_argument_node_nid except that it doesn't
 * add a where clause to the query.
 *
 * @ingroup views_argument_handlers
 */
class similarterms_handler_argument_node_nid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the node.
   */
  function title_query() {
    $titles = array();

    $select = db_select('node', 'n');
    $select->addField('n', 'title');
    $select->condition('n.nid', $this->value, 'IN');
    $select->addTag('node_access');
    $result = $select->execute();

    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }

    return $titles;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['vocabularies'] = array('default' => array());
    $options['include_args'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['not']);

    $vocabularies = array();
    $result = db_query('SELECT vid, name FROM {taxonomy_vocabulary} ORDER BY weight');
    foreach ($result as $vocabulary) {
      $vocabularies[$vocabulary->vid] = $vocabulary->name;
    }

    $form['similarterms'] = array(
      '#type' => 'fieldset',
      '#title' => t('Similarity by Terms'),
      '#collapsible' => FALSE,
    );
    $form['similarterms']['vocabularies'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Limit similarity to terms within these vocabularies'),
      '#description' => t('Choosing any vocabularies here will limit the terms used to calculate similarity. It is usually best NOT to limit the terms, but in some cases this is necessary. Leave all checkboxes unselected to not limit terms.'),
      '#options' => $vocabularies,
      '#default_value' => empty($this->options['vocabularies']) ? array() : $this->options['vocabularies'],
    );

    $form['similarterms']['include_args'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include argument node(s) in results'),
      '#description' => t('If selected, the node(s) passed as the argument will be included in the view results.'),
      '#default_value' => !empty($this->options['include_args']),
    );

  }

  function validate_arg($arg) {
    // first run the inherited arg validation
    if (!parent::validate_arg($arg)) {
      return FALSE;
    }

    // hmmm... @todo: wildcard validation?
    // see views_handler_argument.inc for possible code

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    // $vocabulary_vids is array of vocabulary ids (a.k.a. vids, confusing right?)
    $vocabulary_vids = empty($this->options['vocabularies']) ? array() : $this->options['vocabularies'];
    foreach ($vocabulary_vids as $key => $val) {
      if ($val == 0) {
        unset($vocabulary_vids[$key]);
      }
    }

    $select = db_select('taxonomy_index', 'ti');
    $select->addField('ti', 'tid');

    if (count($vocabulary_vids)) {
      $select->join('taxonomy_term_data', 'td', 'ti.tid = td.tid');
      $select->condition('td.vid', $vocabulary_vids, 'IN');
    }
    $select->condition('ti.nid', $this->value, 'IN');
    $result = $select->execute();

    $this->tids = array();
    foreach ($result as $row) {
      $this->tids[$row->tid] = $row->tid;
    }
    $this->view->tids = $this->tids;

    if (count($this->tids) == 0) {
      // there are no terms ... we need to cancel the query and bail out
      return FALSE;
    }

    return TRUE;
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();

    $this->query->add_table('taxonomy_index', NULL, NULL, 'similarterms_taxonomy_index');
    $this->query->add_where(0, "similarterms_taxonomy_index.tid", $this->tids, 'IN');

    // exclude the current node(s)
    if (empty($this->options['include_args'])) {
      $this->query->add_where(0, "node.nid", $this->value, 'NOT IN');
    }
  }

}
