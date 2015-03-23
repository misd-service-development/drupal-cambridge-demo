<?php

/**
 * @file
 * Documentation for Synonyms module.
 */

/**
 * Provide Synonyms module with names of synonyms extractor classes.
 *
 * Provide Synonyms module with names of classes that are able to extract
 * synonyms from fields. Each of the provided classes should extend
 * AbstractSynonymsExtractor base class.
 *
 * @return array
 *   Array of strings, where each value is a name of synonyms extractor class
 */
function hook_synonyms_extractor_info() {
  return array(
    // Please see below the definition of ApiSynonymsSynonymsExtractor class
    // for your reference.
    'ApiSynonymsSynonymsExtractor',
  );
}

/**
 * Dummy synonyms extractor class for documentation purposes.
 *
 * This is a copy of SynonymsSynonymsExtractor class providing an example of
 * how to write your own synonyms extractor class. See the definition of
 * AbstractSynonymsExtractor for reference and in code comments. For more
 * complicated examples take a look at EntityReferenceSynonymsExtractor class.
 */
class ApiSynonymsSynonymsExtractor extends AbstractSynonymsExtractor {

  /**
   * Return array of supported field types for synonyms extraction.
   *
   * @return array
   *   Array of Field API field types from which this class is able to extract
   *   synonyms
   */
  static public function fieldTypesSupported() {
    return array('text', 'number_integer', 'number_float', 'number_decimal');
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
  static public function synonymsExtract($items, $field, $instance, $entity, $entity_type) {
    $synonyms = array();

    foreach ($items as $item) {
      $synonyms[] = $item['value'];
    }

    return $synonyms;
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
   *   EntityFieldQuery object where you should add your conditions to
   * @param array $field
   *   Array of field definition according to Field API, autocomplete on which
   *   is fired
   * @param array $instance
   *   Array of instance definition according to Field API, autocomplete on
   *   which is fired
   */
  static public function processEntityFieldQuery($tag, EntityFieldQuery $query, $field, $instance) {
    $query->fieldCondition($field, 'value', '%' . $tag . '%', 'LIKE');
  }

  /**
   * Add an entity as a synonym into a field of another entity.
   *
   * Basically this method should be called when you want to add some entity
   * as a synonym to another entity (for example when you merge one entity
   * into another and besides merging want to add synonym of the merging
   * entity into the trunk entity). You should extract synonym value (according
   * to what value is expected in this field) and return it. We try to provide
   * you with as much context as possible, but normally you would only need
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
  static public function mergeEntityAsSynonym($items, $field, $instance, $synonym_entity, $synonym_entity_type) {
    $synonym = entity_label($synonym_entity_type, $synonym_entity);
    switch ($field['type']) {
      case 'text':
        break;

      // We add synonyms for numbers only if $synonym is a number.
      case 'number_integer':
      case 'number_float':
      case 'number_decimal':
        if (!is_numeric($synonym)) {
          return array();
        }
        break;

    }
    return array(array(
      'value' => $synonym,
    ));
  }
}
