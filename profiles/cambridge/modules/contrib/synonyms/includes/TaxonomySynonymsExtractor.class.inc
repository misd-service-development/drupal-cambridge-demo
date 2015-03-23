<?php

/**
 * @file
 * Enables Taxonomy Term Reference field types to be source of synonyms.
 */

class TaxonomySynonymsExtractor extends AbstractSynonymsExtractor {

  public static function fieldTypesSupported() {
    return array('taxonomy_term_reference');
  }

  public static function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    $terms = array();
    foreach ($items as $item) {
      $terms[] = $item['tid'];
    }
    $terms = taxonomy_term_load_multiple($terms);
    foreach ($terms as $term) {
      $synonyms[] = entity_label('taxonomy_term', $term);
    }
    return $synonyms;
  }

  public static function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    // Unfortunately EntityFieldQuery does not currently support INNER JOINing
    // term entity that is referenced via taxonomy_term_reference field type.
    // Thus, we use an ugly solution -- going through all terms that exist in
    // vocabulary and trying to see if there is a match by term's name.
    $tids = array();

    foreach ($field['settings']['allowed_values'] as $settings) {
      $efd = new EntityFieldQuery();
      $efd->entityCondition('bundle', $settings['vocabulary'])
        ->entityCondition('entity_type', 'taxonomy_term')
        ->propertyCondition('name', '%' . $tag . '%', 'LIKE');
      $result = $efd->execute();
      if (isset($result['taxonomy_term'])) {
        foreach ($result['taxonomy_term'] as $tid) {
          $tids[] = $tid->tid;
        }
      }
    }

    // Now we have tids of terms from the referenced vocabulary which names
    // LIKE %$tag%, suggested are the terms that refer to any of these $tids.
    if (empty($tids)) {
      // No possible suggestions were found. We have to make sure $query yields
      // no results.
      self::emptyResultsCondition($query);
      return;
    }
    $query->fieldCondition($field, 'tid', $tids);
  }

  public static function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Taxonomy term reference supports only referencing of entity types
    // 'taxonomy_term'.. duh.
    if ($synonym_entity_type != 'taxonomy_term') {
      return array();
    }
    // Checking that $field is configured to reference the vocabulary of
    // $synonym_entity term.
    $is_allowed = FALSE;
    foreach ($field['settings']['allowed_values'] as $setting) {
      if ($synonym_entity->vocabulary_machine_name == $setting['vocabulary']) {
        if ($setting['parent'] == 0) {
          // No need to check parent property as there is no limitation on it.
          $is_allowed = TRUE;
          break;
        }
        else {
          foreach (taxonomy_get_parents_all($synonym_entity->tid) as $parent) {
            if ($parent->tid == $setting['parent']) {
              $is_allowed = TRUE;
              break(2);
            }
          }
        }
      }
    }
    if (!$is_allowed) {
      // Synonym term is from a vocabulary that is not expected by this field,
      // or under unexpected parent.
      return array();
    }
    return array(array(
      'tid' => $synonym_entity->tid,
    ));
  }
}
