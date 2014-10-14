<?php
/**
 * - group
 * - items
 *  - delta + field_name + value render_array or null if empty
 */
?>
<div class="field-group-multiple-container clearfix">
  <div class="field-group-multiple-items">
  <?php foreach ($entries as $delta => $entry): ?>
    <div class="multiple-inline-element <?php print ($delta % 2 == 1 ? 'odd' : 'even'); ?> clearfix">
    <?php foreach ($entry as $field_name => $field): ?>
      <div class="multiple-element field-item-<?php print $field_name; ?>">
      <?php if (!is_null($field)): ?>
        <?php print render($field); ?>
      <?php else: ?>
        <span class="field-is-empty"> </span>
      <?php endif; ?>
      </div>
    <?php endforeach; ?>
    </div>
  <?php endforeach; ?>
  </div>
</div>
