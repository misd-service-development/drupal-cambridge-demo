<?php

if (array_key_exists('field_link', $content)):
  $url = $content['field_link']['#items'][0]['url'];
else:
  $url = $node_url;
endif;

if (!isset($content['field_image'][0]['#path']['path'])) {
  $content['field_image'][0]['#path'] = array('path' => $url);
}

?>

<?php print render($title_prefix); ?>
<?php print render($title_suffix); ?>
<div class="image-container">
  <?php print render($content['field_image']); ?>
</div>
<div class="campl-slide-caption">
  <a href="<?php print $url; ?>"><span class="campl-slide-caption-txt"><?php print $title; ?></span></a>
</div>
