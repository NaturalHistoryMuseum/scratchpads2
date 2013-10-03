<?php print '<?xml version="1.0" encoding="UTF-8"?>'."\n"; ?>
<OAI-PMH xmlns="http://www.openarchives.org/OAI/2.0/"  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.openarchives.org/OAI/2.0/   http://www.openarchives.org/OAI/2.0/OAI-PMH.xsd">
 <?php print $oai_response_date ?>
  <?php print $oai_request ?>
<?php if (!empty($oai_errors)): ?>
  <?php print $oai_errors ?>
<?php else: ?>
  <<?php print $oai_verb ?>>
<?php endif; ?>
<?php print $oai_content ?>
<?php if (!empty($oai_resumption_token)): ?>
  <?php print $oai_resumption_token ?>
<?php endif; ?>
<?php if (!empty($oai_verb) && empty($oai_errors)):?>
  </<?php print $oai_verb?>>
<?php endif;?>
</OAI-PMH>