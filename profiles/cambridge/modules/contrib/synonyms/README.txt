
-- SUMMARY --

The Synonyms module extends the Drupal core Taxonomy features. Currently
the module provides this additional functionality:
* support of synonyms through Field API. Any field, for which synonyms extractor
   is created, attached to a term can be enabled as source of synonyms.
* synonym-friendly autocomplete widget for taxonomy_term_reference fields
* integration with Drupal search functionality enabling searching content by
   synonyms of the terms that the content references
* integration with Search API. If you include synonyms of a term into your
   Search API search index, your clients will be able to find content with
   search keywords that contain synonyms and not actual names of terms.

-- REQUIREMENTS --

The Synonyms module requires the following modules:
* Taxonomy module
* Text module

-- SYNONYMS EXTRACTORS, SUPPORTED FIELD TYPES --

Module ships with ability to extract synonyms from the following field types:
* Text
* Taxonomy Term Reference
* Entity Reference
* Number
* Float
* Decimal

If you want to implement your own synonyms extractor that would enable support
for any other field type, please, refer to synonyms.api.php file for
instructions on how to do it, or file an issue against Synonyms module. We will
try to implement support for your field type too. If you have written your
synonyms extractor, please share by opening an issue, and it will be included
into this module.

-- INSTALLATION --

* Install as usual

-- CONFIGURATION --

* The module itself does not provide any configuration as of the moment.
Although during creation/editing of a Taxonomy vocabulary you will be able
to enable/disable for that particular vocabulary the additional functionality
this module provides, you will find additional fieldset at the bottom of
vocabulary edit page.

-- FUTURE DEVELOPMENT --

* If you are interested into converting this module from synonyms for Taxonomy
terms into synonyms for any entity types, please go to this issue
http://drupal.org/node/1194802 and leave a comment. Once we see some demand for
this great feature and the Synonyms module gets a little more mature, we will
try to make it happen.
