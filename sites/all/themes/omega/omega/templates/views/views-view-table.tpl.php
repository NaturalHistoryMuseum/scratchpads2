<?php

/**
 * @file
 * Template to display a view as a table.
 */
?>
<table<?php print $attributes; ?>>
  <?php if (!empty($title) || !empty($caption)): ?>
    <caption><?php print $caption . $title; ?></caption>
  <?php endif; ?>
  <?php if (!empty($header)): ?>
    <thead>
      <tr>
        <?php foreach ($header as $field => $label): ?>
          <th<?php print $header_attributes[$field]; ?>>
            <?php print $label; ?>
          </th>
        <?php endforeach; ?>
      </tr>
    </thead>
  <?php endif; ?>
  <tbody>
  <?php foreach ($rows as $delta => $row): ?>
    <tr<?php print $row_attributes[$delta]; ?>>
      <?php foreach ($row as $field => $content): ?>
        <td<?php print $field_attributes[$field][$delta]; ?>>
          <?php print $content; ?>
        </td>
      <?php endforeach; ?>
    </tr>
  <?php endforeach; ?>
  </tbody>
</table>
