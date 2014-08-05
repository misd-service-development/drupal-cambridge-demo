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

<div class="campl-content-container campl-side-padding <?php print $classes; ?>" <?php print $attributes; ?>>
  <div class="campl-horizontal-teaser campl-teaser clearfix campl-focus-teaser">
    <div class="campl-focus-teaser-img">
      <div class="campl-content-container campl-horizontal-teaser-img">
        <?php if ($has_image): ?>
          <?php print render($content['field_image']); ?>
        <?php endif; ?>
      </div>
    </div>
    <div class="campl-focus-teaser-txt">
      <div class="campl-content-container campl-horizontal-teaser-txt">
        <?php print render($title_prefix); ?>
        <h3 class="campl-teaser-title"><a href="<?php print $url; ?>"><?php print $title; ?></a></h3>
        <?php print render($title_suffix); ?>
        <a href="<?php print $url; ?>" class="ir campl-focus-link"><?php print $read_more; ?></a>
      </div>
    </div>
  </div>
</div>
