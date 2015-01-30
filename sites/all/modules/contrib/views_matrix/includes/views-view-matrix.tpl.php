<?php
/**
 * @file
 *   Views View Matrix Template
 *
 * Variables:
 *   - $header
 *   - $rows
 *   - $row_classes
 *   - $matrix_attributes
 */
?>
<table <?php print drupal_attributes($matrix_attributes); ?>>
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>

  <thead>
    <tr>
      <?php foreach ($header as $col => $col_header): ?>
        <th <?php print drupal_attributes($col_header['attributes']); ?>><?php print $col_header['data']; ?></th>
      <?php endforeach; ?>
    </tr>
  </thead>

  <tbody>
    <?php foreach ($rows as $row_index => $row): ?>
    <tr class="<?php print implode(' ', $row_classes[$row_index]); ?>">
      <?php foreach ($row as $col_index => $content): ?>
        <t<?php print ($col_index === 'header') ? 'h' : 'd'; ?><?php  print drupal_attributes($content['attributes']); ?>>
          <?php print !empty($content) ? $content['data'] : ''; ?>
        </t<?php print ($col_index === 'header') ? 'h' : 'd'; ?>>
      <?php endforeach; ?>
    </tr>
    <?php endforeach; ?>
  </tbody>
</table>
