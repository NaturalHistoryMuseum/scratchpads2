<?php
/**
 * TODO documantation
 *
 * - group
 * - items
 *  - delta + field_name + value render_array or null if empty
 */
?>
<div class="field-group-multiple field-group-multiple-label-value clearfix">
  <div class="field-group-multiple-items">
  <?php foreach ($entries as $delta => $entry): ?>
    <div class="field-group-multiple-item <?php print ($delta % 2 == 1 ? 'odd' : 'even'); ?>">
    <?php if (!is_null($entry)): ?>
      <?php print render($entry); ?>
    <?php else: ?>
      <span class="field-is-empty"> </span>
    <?php endif; ?>
    </div>
  <?php endforeach; ?>
  </div>
</div>