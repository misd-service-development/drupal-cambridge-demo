<?php foreach ($items as $delta => $item): ?>
  <h2 class="campl-questions-question">
    <span class="campl-questions-indicator">Q.</span>
    <?php print render($item); ?>
  </h2>
<?php endforeach; ?>
