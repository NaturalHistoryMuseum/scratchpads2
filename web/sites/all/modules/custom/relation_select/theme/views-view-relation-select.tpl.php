<div <?php if ($classes) { print 'class="'. $classes . '" '; } ?>>
  <?php if (!empty($title)) : ?>
    <div class="group"><span><?php print $title; ?></span></div>
  <?php endif; ?>
	<table <?php print $attributes; ?>>
	  <tbody>
	    <?php foreach ($rows as $count => $row): ?>
	      <tr data-entity="<?php print $row_entity[$count]['entity_type']; ?>:<?php print $row_entity[$count]['entity_id']; ?>" data-bundle="<?php print $row_entity[$count]['bundle']; ?>" class="<?php print implode(' ', $row_classes[$count]); ?>">
	        <?php foreach ($row as $field => $content): ?>
	          <td class="<?php print $field; ?>">
	            <?php print $content; ?>
	          </td>
	        <?php endforeach; ?>
	      </tr>
	    <?php endforeach; ?>
	  </tbody>
	</table>
</div>
