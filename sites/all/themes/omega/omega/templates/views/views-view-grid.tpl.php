<?php

/**
 * @file
 * Template to display rows in a grid.
 */
?>
<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<table<?php print $attributes; ?>>
  <?php if (!empty($caption)): ?>
    <caption><?php print $caption; ?></caption>
  <?php endif; ?>
  <tbody>
  <?php foreach ($rows as $delta => $columns): ?>
    <tr<?php print $row_attributes[$delta]; ?>>
      <?php foreach ($columns as $column => $item): ?>
        <td<?php print $column_attributes[$delta][$column]; ?>>
          <?php print $item; ?>
        </td>
      <?php endforeach; ?>
    </tr>
  <?php endforeach; ?>
  </tbody>
</table>
