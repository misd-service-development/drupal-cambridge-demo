Drupal 7 University of Cambridge image styles feature
=====================================================

This feature provides the following images styles:

Name           | Width | Height | Usage
---------------|-------|--------|-----------
Carousel       | 885px |  432px | Carousels.
Leading        | 590px |  288px | Top of content pages.
Inline/teaser  | 250px |  250px | Horizontal/vertical teasers and with the body of content pages.
Sidebar teaser | 349px |  125px | Sidebar teasers and on the University homepage teasers.
Small          | 153px |  153px | Events, search results and profile pictures, and on the University homepage focus on teasers.

These styles use the [Image JavasScript Crop module](https://drupal.org/project/imagecrop) which allows the end user to scale and crop the image for each size, rather than automatically doing so. This provides nicer results.

The feature also provides a `field_image` base image field. This should be reused across content types where there is a 1-to-1 mapping between content and image, which then provides a consistent name for templates to use.
