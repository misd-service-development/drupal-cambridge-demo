<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>

<?php
$teasers = FALSE;
$columnsInRow = 0;
$i = 0;
?>

<?php foreach ($rows as $id => $row): ?>
  <?php
  $i++;
  $columns = NULL;

  $attributes = array();

  if ($classes_array[$id]) {
    preg_match('/campl-column([0-9]+)/', $classes_array[$id], $matches);
    if (isset($matches[1])) {
      $columns = (int) $matches[1];
    }
    $attributes['class'][] = $classes_array[$id];
  }

  $teasers = (FALSE !== strpos($row, 'campl-horizontal-teaser')) + (FALSE !== strpos($row, 'campl-vertical-teaser'));

  if ((FALSE !== strpos($row, 'campl-focus-teaser')) + (FALSE !== strpos($row, 'campl-promo-teaser'))) {
    $teasers = FALSE; // avoid false positives
  }

  if ($columns) {
    if ($teasers) {
      $attributes['class'][] = 'campl-hide-teaser-divider campl-teasers-borders';
    }

    if (0 === $columnsInRow) {
      // start of a new row
      $attributes['class'][] .= 'campl-column-first';
      print '<div class="campl-row">';
    }
    elseif (($columnsInRow + $columns) > 12) {
      // we're going to overflow a row (ie it doesn't add up to 12), so start again
      $attributes['class'][] .= 'campl-column-last';

      print '</div>';

      if ($teasers) {
        print '<div class="campl-column12"><div class="campl-content-container campl-side-padding"><hr class="campl-teaser-divider"></div></div>';
      }

      print '<div class="campl-row">';
      $columnsInRow = 0;
    }
    elseif (($columnsInRow + $columns) == 12) {
      $attributes['class'][] .= 'campl-column-last';
      $attributes['class'][] .= 'campl-column-end';
    }

    if ($i === count($rows) && ($columnsInRow + $columns) < 12) {
      $attributes['class'][] .= 'campl-column-last';
    }
  }
  ?>
  <div<?php print drupal_attributes($attributes); ?>>
    <?php print $row; ?>
  </div>
  <?php

  if ($columns) {
    if (($columnsInRow + $columns) == 12) {
      print '</div>';
      if ($teasers) {
        print '<div class="campl-column12"><div class="campl-content-container campl-side-padding"><hr class="campl-teaser-divider"></div></div>';
      }
      $columnsInRow = 0;
    }
    else {
      $columnsInRow += $columns;
    }
  }

  ?>
<?php endforeach; ?>

<?php
if ($columnsInRow > 0) {
  // the last row is open, so close it
  print '</div>';
  if ($teasers) {
    print '<div class="campl-column12"><div class="campl-content-container campl-side-padding"><hr class="campl-teaser-divider"></div></div>';
  }
}
?>
