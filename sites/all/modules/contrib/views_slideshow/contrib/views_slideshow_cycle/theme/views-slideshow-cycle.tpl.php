<?php

/**
 * @file
 * Views Slideshow: Single Frame template file.
 */
?>

<div class="skin-<?php print $settings['views_slideshow_cycle']['skin']; ?>">
  <?php if (isset($top_widget_rendered)): ?>
    <div class="views-slideshow-controls-top clearfix">
      <?php print $top_widget_rendered; ?>
    </div>
  <?php endif; ?>

  <?php print $slideshow; ?>

  <?php if (isset($bottom_widget_rendered)): ?>
    <div class="views-slideshow-controls-bottom clearfix">
      <?php print $bottom_widget_rendered; ?>
    </div>
  <?php endif; ?>
</div>
