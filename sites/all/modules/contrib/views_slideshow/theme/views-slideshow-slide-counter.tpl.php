<?php

/**
 * @file
 * Views Slideshow: Template for slide counter.
 *
 * - $variables: Contains theme variables.
 * - $classes: Classes.
 * - $slide_count: Slide count.
 *
 * @ingroup vss_templates
 */
?>
<div id="views_slideshow_slide_counter_<?php print $variables['vss_id']; ?>" class="<?php print $classes; ?>">
  <span class="num">1</span> <?php print t('of'); ?> <span class="total"><?php print $slide_count; ?></span>
</div>
