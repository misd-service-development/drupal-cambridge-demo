<?php
/**
 * Handler which sorts the by the similarity.
 *
 * @ingroups views_sort_handlers
 */
class similarterms_handler_sort_similar extends views_handler_sort {

  function option_definition() {
    $options = parent::option_definition();
    $options['order'] = array('default' => 'DESC');
    return $options;
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_orderby($this->table_alias, 'nid', $this->options['order'], NULL, array('function' => 'count'));
  }

}
