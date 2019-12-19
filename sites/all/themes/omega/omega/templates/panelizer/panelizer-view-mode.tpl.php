<div<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if (!empty($title)): ?>
  <<?php print $title_element;?> <?php print $title_attributes; ?>>
    <?php if (!empty($entity_url)): ?>
      <a href="<?php print $entity_url; ?>"><?php print $title; ?></a>
    <?php else: ?>
      <?php print $title; ?>
    <?php endif; ?>
  </<?php print $title_element;?>>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php print $content; ?>
</div>
