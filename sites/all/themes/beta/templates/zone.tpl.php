<?php
/**
 * @file
 * Default theme implementation to display a region.
 *
 * Available variables:
 * - $regions: Renderable array of regions associated with this zone
 * - $enabled: Flag to detect if the zone was enabled/disabled via theme settings
 * - $wrapper: Flag set to display a full browser width wrapper around the 
 *      container zone allowing items/backgrounds to be themed outside the 
 *      960pixel container size.
 * - $zid: the zone id of the zone being rendered. This is a text value.
 * - $container_width: the container width (12, 16, 24) of the zone
 * - $attributes: a string containing the relevant class & id data for a container
 * 
 * Helper Variables
 * - $attributes_array: an array of attributes for the container zone
 * 
 * @see template_preprocess()
 * @see template_preprocess_zone()
 * @see template_process()
 * @see template_process_zone()
 */
?>

<?php if($enabled && $populated): ?>
	<?php if($wrapper): ?><div id="<?php print $zid;?>-outer-wrapper" class="clearfix"><?php endif; ?>  
	  <div class="<?php print $classes; ?>" <?php print $attributes;?>>
	    <?php print render($regions); ?>
	  </div>
	<?php if($wrapper): ?></div><?php endif; ?>
<?php endif; ?>