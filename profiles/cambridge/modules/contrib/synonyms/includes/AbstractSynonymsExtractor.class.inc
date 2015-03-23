<?php

/**
 * @file
 * Define interface required for extracting synonyms from field types.
 */

abstract class AbstractSynonymsExtractor {

  /**
   * Return array of supported field types for synonyms extraction.
   *
   * @return array
   *   Array of Field API field types from which this class is able to extract
   *   synonyms
   */
  public static function fieldTypesSupported() {
    return array();
  }

  /**
   * Extract synonyms from a field attached to an entity.
   *
   * We try to pass as many info about context as possible, however, normally
   * you will only need $items to extract the synonyms.
   *
   * @param array $items
   *   Array of items
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   * @param object $entity
   *   Fully loaded entity object to which the $field and $instance with $item
   *   values is attached to
   * @param string $entity_type
   *   Type of the entity $entity according to Field API definition of entity
   *   types
   *
   * @return array
   *   Array of synonyms extracted from $items
   */
  public static function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    return array();
  }

  /**
   * Allow you to hook in during autocomplete suggestions generation.
   *
   * Allow you to include entities for autocomplete suggestion that are possible
   * candidates based on your field as a source of synonyms. This method is
   * void, however, you have to alter and add your condition to $query
   * parameter.
   *
   * @param string $tag
   *   What user has typed in into autocomplete widget. Normally you would
   *   run LIKE '%$tag%' on your column
   * @param EntityFieldQuery $query
   *   EntityFieldQuery object where you should put your conditions to
   * @param array $field
   *   Array of field definition according to Field API
   * @param array $instance
   *   Array of instance definition according to Field API
   */
  public static function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {}

  /**
   * Add an entity as a synonym into a field of another entity.
   *
   * Basically this method should be called when you want to add some entity
   * as a synonym to another entity (for example when you merge one entity
   * into another and besides merging want to add synonym of the merging
   * entity into the trunk entity). You should extract synonym value (according
   * to what value is expected in this field) and return it. We try to provide
   * you with as much of context as possible, but normally you would only need
   * $synonym_entity and $synonym_entity_type parameters. Return an empty array
   * if entity of type $synonym_entity_type cannot be converted into a format
   * expected by $field.
   *
   * @param array $items
   *   Array items that already exist in the field into which new synonyms is to
   *   be added
   * @param array $field
   *   Field array definition according to Field API of the field into which new
   *   synonym is to be added
   * @param array $instance
   *   Instance array definition according to Field API of the instance into
   *   which new synonym is to be added
   * @param object $synonym_entity
   *   Fully loaded entity object which has to be added as synonym
   * @param string $synonym_entity_type
   *   Entity type of $synonym_entity
   *
   * @return array
   *   Array of extra items to be merged into the items that already exist in
   *   field values
   */
  public static function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    return array();
  }

  /**
   * Supportive method.
   *
   * Set such a condition on $query that it will always yield no results. Should
   * be called from $this->processEntityFieldQuery() when for whatever reason
   * the object can't alter $query to include matched synonyms. As a fallback it
   * should call this method to make sure it filtered everything out.
   *
   * @param EntityFieldQuery $query
   *   Query object passed to $this->processEntityFieldQuery() method
   */
  protected static function emptyResultsCondition(EntityFieldQuery $query) {
    $query->range(0, 0);
  }
}
