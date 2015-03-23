<?php
/**
 * @file
 * cambridge_image_styles.features.inc
 */

/**
 * Implements hook_image_default_styles().
 */
function cambridge_image_styles_image_default_styles() {
  $styles = array();

  // Exported image style: carousel.
  $styles['carousel'] = array(
    'label' => 'Carousel',
    'effects' => array(
      7 => array(
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => 885,
          'height' => 432,
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 0,
          'downscaling' => 0,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: inline.
  $styles['inline'] = array(
    'label' => 'Inline/teaser',
    'effects' => array(
      9 => array(
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => 250,
          'height' => 250,
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 0,
          'downscaling' => 0,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: leading.
  $styles['leading'] = array(
    'label' => 'Leading',
    'effects' => array(
      8 => array(
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => 590,
          'height' => 288,
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 0,
          'downscaling' => 0,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 0,
        ),
        'weight' => 0,
      ),
    ),
  );

  // Exported image style: sidebar_teaser.
  $styles['sidebar_teaser'] = array(
    'label' => 'Sidebar Teaser',
    'effects' => array(
      10 => array(
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => 349,
          'height' => 125,
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 0,
          'downscaling' => 0,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: small.
  $styles['small'] = array(
    'label' => 'Small',
    'effects' => array(
      1 => array(
        'name' => 'imagecrop_javascript',
        'data' => array(
          'width' => 153,
          'height' => 153,
          'xoffset' => 'center',
          'yoffset' => 'center',
          'resizable' => 0,
          'downscaling' => 0,
          'aspect_ratio' => 'CROP',
          'disable_if_no_data' => 0,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}
