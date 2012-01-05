<div class='context-block-browser clearfix'>

  <div class='categories'><?php print render($categories) ?></div>

  <?php foreach ($blocks as $module => $module_blocks): ?>

  <?php if (!empty($module_blocks)): ?>
  <div class='category category-<?php print $module ?> clearfix'>
    <?php foreach ($module_blocks as $block): ?>
      <?php print theme('context_block_browser_item', array('block' => $block)); ?>
    <?php endforeach; ?>
  </div>
  <?php endif; ?>

  <?php endforeach; ?>

</div>
