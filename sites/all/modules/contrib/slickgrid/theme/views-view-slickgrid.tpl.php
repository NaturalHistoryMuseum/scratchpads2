<?php
if(isset($slickgrid)):
  ?>
<div class="slickgrid-wrapper clear-block <?php print $class; ?>">
<?php
if(isset($tabs)):
?>
<div class="slickgrid-tabs-wrapper">
<?php
print $tabs;
?>
</div>
<?php endif;
?>
<?php
print $slickgrid;
if(isset($controls)):
?>
<div id="controls">
<?php
print $controls;
?>
</div>
<?php endif;
?>
</div>
<?php endif;
?>