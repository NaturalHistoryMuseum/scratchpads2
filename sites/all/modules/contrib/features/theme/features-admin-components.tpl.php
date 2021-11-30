<?php

/**
 * @file
 * Template for the 'features_admin_components' form.
 *
 * @var string $lock_feature
 * @var string $name
 * @var string $description
 * @var string $dependencies
 * @var string $components
 *
 * @see \features_admin_components()
 * @see \template_preprocess_features_admin_components()
 */
?>
<div class='clearfix features-components'>
  <div class='column'>
    <div class='info'>
      <?php print $lock_feature ?><h3><?php print $name ?></h3>
      <div class='description'><?php print $description ?></div>
      <?php print $dependencies ?>
    </div>
  </div>
  <div class='column'>
    <div class='components'>
      <?php print $components ?>
      <?php if (!empty($key)): ?>
        <div class='clearfix features-key'><?php print theme('links', array('links' => $key)) ?></div>
      <?php endif; ?>
      <?php if (!empty($buttons)): ?>
        <div class='buttons clearfix'><?php print $buttons ?></div>
      <?php endif; ?>
    </div>
  </div>
  <?php print drupal_render_children($form) ?>
</div>
