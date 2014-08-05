Drupal 7 University of Cambridge teasers feature
================================================

This feature adds node view modes for horizontal, vertical, sidebar and focus on teasers, as well as for news listing items. (The horizontal teaser actually replaces Drupal's default teaser.)

To have an image appear in the teaser use the `field_image` base image field provided by the [Images Styles feature](https://github.com/misd-service-development/drupal-feature-image-styles).

For each content type you will need to set the display settings as follows.

For both horizontal and vertical teasers:

![Horizontal/vertical teaser display configuration](doc/horizontal_vertical_display.png)

For sidebar teasers:

![Horizontal/vertical teaser display configuration](doc/sidebar_display.png)

For focus on teasers:

![Focus on teaser display configuration](doc/focus_on_display.png)

For the 'News listing item' content type you will need to set the display settings to only show the (summary) body. This would typically look like:

![News listing item display configuration](doc/news_listing_item_display.png)

If you are using a `field_link` provided by the [Link feature](https://github.com/misd-service-development/drupal-feature-link) you also need to make your link field available (it doesn't matter what format setting you use). The teaser will then link to your custom URL rather than to the node.

Creating a list of horizontal teasers
-------------------------------------

To create a list of sidebar teasers produced by a view, have a block with the following format options:

![Horizontal teaser format options](doc/horizontal_view_format.png)

The view block then needs to be set to appear on the appropriate page(s) in the 'Content' region.

Creating a list of vertical teasers
-----------------------------------

To create a list of vertical teasers produced by a view, have a block with the following format options:

![Vertical teaser format options](doc/vertical_view_format.png)

Then change the format settings to:

![Vertical teaser format settings](doc/vertical_view_style_options.png)

This will create 2 columns. For 3 columns, for example, set the row class to `campl-column4`.

Finally, set CSS class in the Advanced section to:

![Vertical teaser CSS class](doc/vertical_view_advanced_css.png)

The view block then needs to be set to appear on the appropriate page(s) in the 'Content' region.

Creating a list of sidebar teasers
----------------------------------

To create a list of sidebar teasers produced by a view, have a block with the following format options:

![Sidebar teaser format options](doc/sidebar_view_format.png)

The view block then needs to be set to appear on the appropriate page(s) in the 'Sidebar' region.

Creating a list of focus on teasers
-----------------------------------

To create a list of focus on teasers produced by a view, have a block with the following format options:

![Focus on teaser format options](doc/focus_on_view_format.png)

Then change the format settings to:

![Focus on teaser format settings](doc/vertical_view_style_options.png)

This will create 2 columns. For 3 columns, for example, set the row class to `campl-column4`.

Finally, set CSS class in the Advanced section to:

![Focus on teaser CSS class](doc/vertical_view_advanced_css.png)

The view block then needs to be set to appear on the appropriate page(s) in the 'Content' region.

Creating a list of news items
-----------------------------

To create a list of news items produced by a view, have a block with the following format options:

![Format options](doc/news_listing_item_view_format.png)

The view block then needs to be set to appear on the appropriate page(s) in the 'Sub-content' region.

Creating a page of news items
---------------------------

For a page of news items you should use the [horizontal teaser](https://github.com/misd-service-development/drupal-feature-teasers) node view mode.
