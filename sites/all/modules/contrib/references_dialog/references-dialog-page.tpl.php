<?php

/**
 * @file
 * Template file for page that is used in our references dialog.
 */
?>
<div id="references-dialog-page">
  <?php if (isset($messages)): print $messages; endif; ?>
  <?php print render($page['content']); ?>
</div>
