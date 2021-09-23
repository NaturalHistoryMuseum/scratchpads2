<?php
/**
   * Theme a publication group for the pensoft_publication module
   *
   */
  if (!empty($variables['skip_group'])) {
    return;
  }
?>
<div class="publication-group clearfix">
	<label><?php echo $label; ?></label>
  <?php
  if($element_id == 'publication_pensoft_full_group_publication_auth_contrib'):
    ?>
    <div class='field field-publication-background'>
      <div class='field-items'>
        <?php echo implode(', ', $variables['authors']); ?>
      </div>
	</div>

	<div
	   class='field field-publication-background field-publication-secondary'>
		<div class='field-items'>
        <?php echo implode('', $variables['institutions']); ?>
      </div>
	</div>
  
    <?php if (!empty($variables['contributors'])): ?>
      <div class='field field-publication-background'>
	    <div class='field-items'>
		  <?php foreach($variables['contributors'] as $role => $name): ?>
		    <div class="field-item"><?php echo $role.': ' . $name; ?></div>
		  <?php endforeach; ?>
		</div>
      </div>
    <?php endif; ?>
   <?php elseif (TRUE && $element_id == 'publication_pensoft_full_group_pensoft_files'): ?>
     <?php /* References */ ?>
     <?php if (isset($element['field_publication_references']) && count(element_children($element['field_publication_references']))) : ?>
       <div class="field field-collection-container">
		<div class="field-label">References:</div>
		<div class="field-items">
         <?php
      foreach(element_children($element['field_publication_references']) as $key):
        echo "<div class='field'>" . render($element['field_publication_references'][$key]) . '</div>';
      endforeach
      ;
      ?>
         </div>
	</div>
     <?php endif; ?>
     <?php /* Figures */ ?>
     <?php if (count(element_children($element['field_publication_figures']))) : ?>
       <div class="field field-collection-container">
		<div class="field-label">Figures:</div>
		<div class="field-items">
           <?php
      foreach(element_children($element['field_publication_figures']) as $key):
        echo render($element['field_publication_figures'][$key]);
      endforeach
      ;
      ?>          
         </div>
	</div>
     <?php endif; ?>
     <?php /* Tables */ ?>
     <?php if (count(element_children($element['field_publication_tables']))) : ?>
       <div class="field field-collection-container">
		<div class="field-label">Tables:</div>
		<div class="field-items">
           <?php
      foreach(element_children($element['field_publication_tables']) as $key):
        echo render($element['field_publication_tables'][$key]);
      endforeach
      ;
      ?>
         </div>
	</div>
     <?php endif; ?>
  <?php elseif ($element_id == 'publication_pensoft_full_group_publication_systematics'): ?>
    <div class="group-publication-systematics">
      <?php echo $element['#children']; ?>
    </div>
  <?php else: ?>
    <?php echo $element['#children']; ?>
  <?php endif; ?>
</div>