<?php

hide($content['links']);

if (array_key_exists('field_link', $content)):
  hide($content['field_link']);
  $url = $content['field_link']['#items'][0]['url'];
  $read_more = $content['field_link']['#items'][0]['title'];

  if (substr($read_more, 0, 80) == substr($url, 0, 80)):
    $read_more = t('Read more');
  endif;

  if (array_key_exists('field_image', $content)):
    $content['field_image'][0]['#path'] = array('path' => $url);
  endif;
else:
  $url = $node_url;
  $read_more = t('Read more');
endif;

$has_image = isset($content['field_image']);

?>

<div class="campl-content-container <?php print $classes; ?>" <?php print $attributes; ?>>
  <div class="campl-vertical-teaser campl-teaser campl-promo-teaser">
    <div class="campl-content-container campl-vertical-teaser-txt">
      <?php print render($title_prefix); ?>
      <p class='campl-teaser-title'><a href="<?php print $url; ?>"><?php print $title; ?></a></p>
      <?php print render($title_suffix); ?>
    </div>
    <?php if ($has_image): ?>
      <div class="campl-content-container campl-vertical-teaser-img">
        <?php print render($content['field_image']); ?>
      </div>
    <?php endif; ?>
    <div class="campl-content-container<?php if (!$has_image): print ' campl-no-top-padding'; endif; ?> campl-vertical-teaser-txt clearfix">
      <?php if ($display_submitted): ?>
        <p class="campl-datestamp"><?php print $date; ?></p>
      <?php endif; ?>
      <?php print render($content); ?>
      <a href="<?php print $url; ?>" class="campl-secondary-cta campl-float-right"><?php print $read_more; ?></a>
    </div>
  </div>
</div>
