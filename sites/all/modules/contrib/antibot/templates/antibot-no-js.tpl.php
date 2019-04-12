<?php

/**
 * @file
 * Template for printing a message to users without JavaScript enabled.
 *
 * Available variables:
 * - $message: The message to display.
 */
?>
<noscript>
  <style>form.antibot { display: none !important; }</style>
  <div class="antibot-no-js antibot-message antibot-message-warning messages warning">
    <?php print $message; ?>
  </div>
</noscript>
