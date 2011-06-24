<!-- <div class="clear-block <?php
print $class;
?>-wrapper">
  <div id="<?php
  print $id;
  ?>" class="<?php
  print $class;
  ?>-wrapper"</div>
  <?php
  if(isset($controls)):
    ?>
    <div id="<?php
    print $id;
    ?>-controls" class="<?php
    print $class;
    ?>-controls"></div>
  
  
  <?php endif;
  ?>
</div> -->

<?php
if(isset($slickgrid)):
  ?>
<div class="slickgrid-wrapper clear-block">
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