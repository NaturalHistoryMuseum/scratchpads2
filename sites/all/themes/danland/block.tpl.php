<?php
// $Id: block.tpl.php,v 1.3.4.1 2010/11/11 13:52:44 danprobo Exp $
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

<?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?> class="block-title"><?php print $block->subject ?></h2>
<?php endif;?>
<?php print render($title_suffix); ?>

<div class="content"<?php print $content_attributes; ?>>
  <?php print $content ?>
</div> <!-- end block content -->
</div> <!-- end block -->
