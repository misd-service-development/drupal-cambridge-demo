<?php

/**
 * @file
 * Enables Entity Reference field type to be source of synonyms.
 */

class EntityReferenceSynonymsExtractor extends AbstractSynonymsExtractor {

  public static function fieldTypesSupported() {
    return array('entityreference');
  }

  public static function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    // For speading up loading all the entities at once.
    $target_tids = array();
    foreach ($items as $item) {
      $target_tids[] = $item['target_id'];
    }
    $entities = entity_load($field['settings']['target_type'], $target_tids);
    foreach ($entities as $entity) {
      $synonyms[] = entity_label($field['settings']['target_type'], $entity);
    }

    return $synonyms;
  }

  public static function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    // Unfortunately EntityFieldQuery does not currently support INNER JOINing
    // referenced entities via any field type.
    // Thus, we use an ugly solution -- going through all entities that exist
    // in such entity type trying to see if there is a match by entity's label.
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', $field['settings']['target_type']);
    // Additionally we have to figure out which column in the entity table
    // represents entity label.
    $entity_info = entity_get_info($field['settings']['target_type']);
    if (!isset($entity_info['entity keys']['label'])) {
      // We can't get any matches if we do not know what column to query
      // against. So we add a condition to $query which will 100% yield empty
      // results.
      self::emptyResultsCondition($query);
      return;
    }
    $efq->propertyCondition($entity_info['entity keys']['label'], '%' . $tag . '%', 'LIKE');
    $result = $efq->execute();

    if (!isset($result[$field['settings']['target_type']]) || !is_array($result[$field['settings']['target_type']])) {
      self::emptyResultsCondition($query);
      return;
    }
    $result = $result[$field['settings']['target_type']];
    $query->fieldCondition($field, 'target_id', array_keys($result));
  }

  public static function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    // Firstly validating that this entity reference is able to reference to
    // that type of entity.
    $expected_synonym_entity_type = $field['settings']['target_type'];
    if ($expected_synonym_entity_type != $synonym_entity_type) {
      return array();
    }
    $synonym_entity_id = entity_id($synonym_entity_type, $synonym_entity);
    return array(array(
      'target_id' => $synonym_entity_id,
    ));
  }
}
