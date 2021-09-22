<?php if ($prefix) : ?>
	 <div class="relation-select-field-prefix"><?php print $prefix; ?></div>
<?php endif; ?>

<<?php print $list_type; ?>>
	 <?php foreach ($items as $item): ?>
		  <li><?php print $item; ?></li>
	 <?php endforeach; ?>
</<?php print $list_type; ?>>

<?php if ($suffix) : ?>
	  <div class="relation-select-field-suffix"><?php print $suffix; ?></div>
<?php endif; ?>