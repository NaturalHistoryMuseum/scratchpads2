<?php
/**
 * Copy of core search-result.tpl.php but with the addition of entity/bundle.
 */
?>
<li class="<?php print $classes; ?>" <?php print $attributes; ?>>
  <?php print render($title_prefix)?>
  <h3 class="title" <?php print $title_attributes; ?>>
		<a href="<?php print $url; ?>"><?php print $title; ?></a> <span
			style="float: right"><?php print ucfirst(str_replace('_', ' ', $result['entity_type'])).'/'.ucfirst(str_replace('_', ' ', $result['bundle']));?></span>
	</h3>
  <?php print render($title_suffix); ?>
  <div class="search-snippet-info">
    <?php if ($snippet): ?>
      <p class="search-snippet" <?php print $content_attributes; ?>><?php print $snippet; ?></p>
    <?php endif; ?>
    <?php if ($info): ?>
      <p class="search-info"><?php print $info; ?></p>
    <?php endif; ?>
  </div>
</li>