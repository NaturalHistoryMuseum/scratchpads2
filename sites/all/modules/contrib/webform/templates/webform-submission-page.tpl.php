<?php

/**
 * @file
 * Customize the navigation shown when editing or viewing submissions.
 *
 * Available variables:
 * - $node: The node object for this webform.
 * - $submission: The Webform submission array.
 * - $submission_content: The contents of the webform submission.
 * - $submission_navigation: The previous submission ID.
 * - $submission_information: The next submission ID.
 */

drupal_add_css(drupal_get_path('module', 'webform') . '/css/webform-admin.css', array('group' => CSS_THEME, 'preprocess' => FALSE));
?>

<?php if ($submission_actions || $submission_navigation): ?>
  <div class="clear-block">
    <?php print $submission_actions; ?>
    <?php print $submission_navigation; ?>
  </div>
<?php endif; ?>

<?php print $submission_information; ?>

<div class="webform-submission">
  <?php print render($submission_content); ?>
</div>

<?php print $submission_navigation; ?>
