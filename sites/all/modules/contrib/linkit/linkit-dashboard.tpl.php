<?php

/**
 * @file
 * Linkit dashboard template file
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
</head>
<body>
  <div id="linkit">
    <?php if (!empty($messages)): print $messages; endif; ?>
    <?php if (!empty($help)): print $help; endif; ?>
    <div id="content-content" class="clearfix">
      <?php print $form; ?>
    </div>
  </div>
</body>
</html>