<?php

/**
 * @file
 * Views Slideshow: Template for pager field - item.
 *
 * - $variables: Contains theme variables.
 * - $classes: Classes.
 * - $count: The current row number.
 * - $view: The view the pager is attached to.
 * - $vss_id: The id of the slideshow.
 *
 * @ingroup vss_templates
 */
?>
<div id="views_slideshow_pager_field_item_<?php print $variables['location']; ?>_<?php print $vss_id; ?>_<?php print $count; ?>" class="<?php print $classes; ?>" aria-controls="views_slideshow_cycle_div_<?php print $variables['vss_id']; ?>_<?php print $variables['count']; ?>">
  <?php print $item; ?>
</div>
