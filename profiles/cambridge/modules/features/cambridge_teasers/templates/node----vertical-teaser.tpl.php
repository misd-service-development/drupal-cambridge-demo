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

?>

<div class="campl-content-container campl-vertical-padding <?php print $classes; ?>" <?php print $attributes; ?>>
  <div class="campl-vertical-teaser campl-teaser campl-teaser-border campl-content-container campl-side-padding">
    <?php if (array_key_exists('field_image', $content)): ?>
      <div class="campl-content-container campl-vertical-teaser-img">
        <?php print render($content['field_image']); ?>
      </div>
    <?php endif; ?>
    <div class="campl-content-container campl-vertical-teaser-txt">
      <?php print render($title_prefix); ?>
      <h3 class='campl-teaser-title'><a href="<?php print $url; ?>"><?php print $title; ?></a></h3>
      <?php print render($title_suffix); ?>
      <?php if ($display_submitted): ?>
        <p class="campl-datestamp"><?php print $date; ?></p>
      <?php endif; ?>
      <?php print render($content); ?>
      <a href="<?php print $url; ?>" class="campl-primary-cta"><?php print $read_more; ?></a>
    </div>

  </div>
</div>

<div class="campl-content-container campl-side-padding">
  <hr class="campl-teaser-divider">
</div>
