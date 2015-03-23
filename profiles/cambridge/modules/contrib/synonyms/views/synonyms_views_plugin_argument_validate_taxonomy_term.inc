<?php

/**
 * @file
 * An extended version of the 'taxonomy term' argument validator plugin, which
 * supports synonyms as arguments.
 */

/**
 * Validate whether an argument is an acceptable taxonomy term.
 */
class synonyms_views_plugin_argument_validate_taxonomy_term extends views_plugin_argument_validate_taxonomy_term {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // We just want to add yet another way of validation - synonyms friendly
    // validation.
    $form['type']['#options']['synonym'] = t('Term name or synonym');
    $form['type']['#options']['synonym_tid'] = t('Term name or synonym converted to Term ID');
  }

  function validate_argument($argument) {
    $vocabularies = array_filter($this->options['vocabularies']);
    $type = $this->options['type'];
    $transform = $this->options['transform'];

    switch ($type) {
      case 'synonym':
      case 'synonym_tid':
        // We are requested to do synonyms friendly validation. We will have to
        // go through each of allowed vocabularies, collecting terms whose
        // synonyms might look like the argument we have. However, it is not
        // that easy to achieve due to the fact that synonyms may be kept in
        // whatever format in whatever column and only corresponding Synonyms
        // extractor class has the knowledge how it is organized. Firstly we are
        // going to query database trying to find a term with our argument's
        // name. If we find one, it is great and we stop right there. Otherwise,
        // the nightmare starts: we are going to use
        // AbstractSynonymsExtractor::processEntityFieldQuery() method to find
        // synonyms that are similar to our argument. Then we will load those
        // terms and manually in PHP will determine whether it is a match.
        $query = db_select('taxonomy_term_data', 't')
          ->fields('t', array('tid', 'name'));
        if (!empty($vocabularies)) {
          $query->innerJoin('taxonomy_vocabulary', 'v', 't.vid = v.vid');
          $query->condition('v.machine_name', $vocabularies);
        }
        if ($transform) {
          $query->where("replace(t.name, ' ', '-') = :name", array(':name' => $argument));
        }
        else {
          $query->condition('t.name', $argument, '=');
        }
        $result = $query->range(0, 1)
          ->execute();
        if ($result->rowCount()) {
          // We have found a term, we are going to use it.
          $term = taxonomy_term_load($result->fetchField(0));
          $this->argument->argument = $this->synonyms_get_term_property($term);
          $this->argument->validated_title = check_plain(entity_label('taxonomy_term', $term));
          return TRUE;
        }

        if (empty($vocabularies)) {
          // At this point we want to convert an empty $vocabulary (implicitly
          // meaning "all") into actually a list of all fully loaded
          // vocabularies.
          $vocabularies = taxonomy_vocabulary_load_multiple(FALSE);
        }
        else {
          // We need to load the filtered vocabularies based on their machine
          // names.
          foreach ($vocabularies as $v) {
            $vocabularies[$v] = taxonomy_vocabulary_machine_name_load($v);
          }
        }

        // We haven't had much luck, seems like we have to do the hard part. We
        // will pull up terms that are similar to our argument using the same
        // functionality as for synonyms friendly autocomplete widget and then
        // will find those who actually match among this narrowed set of terms.
        // Since $match will be use as value for LIKE SQL operator, we are not
        // sure whether we need to substitute dash (-) with spacebar, or keep
        // the dash, to match both we will use underscore (_) since this symbol
        // matches one symbol in LIKE operator.
        $match = $transform ? str_replace('-', '_', $argument) : $argument;
        $tids = array();
        // Unfortunately we have to query multiple times the database for each
        // vocabulary for each synonyms-source field.
        foreach ($vocabularies as $vocabulary) {
          $bundle = field_extract_bundle('taxonomy_term', $vocabulary);
          foreach (synonyms_synonyms_fields($vocabulary) as $field) {
            $field = field_info_field($field);
            $instance = field_info_instance('taxonomy_term', $field['field_name'], $bundle);
            $query = new EntityFieldQuery();
            $query->entityCondition('entity_type', 'taxonomy_term')
              ->entityCondition('bundle', $vocabulary->machine_name);

            // We let the class that defines this field type as a source of
            // synonyms filter out and provide its suggestions based on this
            // field.
            $class = synonyms_extractor_info($field['type']);
            call_user_func(array($class, 'processEntityFieldQuery'), $match, $query, $field, $instance);
            if (!empty($tids)) {
              $query->entityCondition('entity_id', $tids, 'NOT IN');
            }
            $result = $query->execute();
            if (isset($result['taxonomy_term'])) {
              $tids = array_merge($tids, array_keys($result['taxonomy_term']));
            }
          }
        }
        $terms = taxonomy_term_load_multiple($tids);
        // We have an array of terms whose synonyms look like they could be
        // equal to our argument, let us iterate over each term's synonyms to
        // actually see if they match.
        $argument = mb_strtoupper($argument, 'UTF-8');
        foreach ($terms as $term) {
          foreach (synonyms_get_term_synonyms($term) as $synonym) {
            $synonym = mb_strtoupper($synonym['safe_value'], 'UTF-8');
            if ($transform) {
              $synonym = str_replace(' ', '-', $synonym);
            }
            if ($synonym == $argument) {
              // Finally we found a synonym that matches our argument. We are
              // going to use the corresponding term and there is no point to
              // continue searching.
              $this->argument->argument = $this->synonyms_get_term_property($term);
              $this->argument->validated_title = check_plain(entity_label('taxonomy_term', $term));
              return TRUE;
            }
          }
        }
        // We haven't found any synonym that matched our argument, so there is
        // no term to return.
        return FALSE;

      default:
        return parent::validate_argument($argument);
    }
  }

  /**
   * Return necessary property (per chosen validator) of encountered term.
   *
   * @param $term object
   *   Fully loaded taxonomy term from which necessary property should be
   *   returned
   *
   * @return mixed
   *   Necessary property (per chosen validator) of the provided term
   */
  function synonyms_get_term_property($term) {
    switch ($this->options['type']) {
      case 'synonym':
        return $term->name;

      case 'synonym_tid':
        return $term->tid;
    }
    return NULL;
  }
}
