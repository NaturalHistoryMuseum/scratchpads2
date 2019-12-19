<?php

/**
 * @file
 * Default theme implementation to display voting form for a poll.
 *
 * - $choice: The radio buttons for the choices in the poll.
 * - $title: The title of the poll.
 * - $block: True if this is being displayed as a block.
 * - $vote: The vote button
 * - $rest: Anything else in the form that may have been added via
 *   form_alter hooks.
 *
 * @see template_preprocess_poll_vote()
 *
 * @ingroup themeable
 */
?>
<div class="poll-vote-form">
  <div class="poll-vote-form__choices">
    <?php if ($block): ?>
      <h4><?php print $title; ?></h4>
    <?php endif; ?>
    <?php print $choice; ?>
  </div>
  <?php print $vote; ?>
</div>
<?php // This is the 'rest' of the form, in case items have been added. ?>
<?php print $rest ?>
