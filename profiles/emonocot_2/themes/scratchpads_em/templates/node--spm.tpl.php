<?php

//What biblio nids are referenced from this page
$referenced_nids = array();
$referenced_field = NULL;

foreach ($content['group_vertical_tabs'] as $tab => $values){
	foreach ($values as $field => $fvalues) {
		if (substr($field, -3) == 'ref'){
			if (is_null($referenced_field)){
			  $referenced_field['ref_field'] = $content['group_vertical_tabs'][$tab][$field];
			}
			$temp = $content['group_vertical_tabs'][$tab][$field]['#object']->$field;
			$referenced_nids[] = $temp['und'][0]['nid'];
		}
	}
}

$unique_nids = sizeof(array_unique($referenced_nids));

if ($unique_nids == 1){
  //Remove existing links
  foreach ($content['group_vertical_tabs'] as $tab => $values){
	foreach ($values as $field => $fvalues) {
		if (substr($field, -3) == 'ref') {
			unset($content['group_vertical_tabs'][$tab][$field]);
		}
	}
  }
  //Add overall
  $referenced_field['ref_field']['#title'] = 'Reference for all descriptions on this page';
  $content['group_vertical_tabs'][] = $referenced_field;
}
?>

<article<?php print $attributes; ?>>
  <?php if ($title): ?>
  <header>
    <?php print render($title_prefix); ?>
    <?php if ($view_mode == 'species' || $view_mode == 'linked_node'): ?>
    <h2<?php print $title_attributes; ?>><?php print $title ?></h2>
    <?php elseif(!$page): ?>
    <h3<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h3>
    <?php endif; ?>
    
    <?php print render($title_suffix); ?>
  </header>
  <?php endif; ?> 
  
  <div<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
    <?php if ($display_submitted): ?>
  <footer class="submitted"><?php print $date; ?> -- <?php print $name . $user_picture; ?></footer>
  <?php endif; ?> 
  
  <div class="clearfix">
    <?php if (!empty($content['links'])): ?>
      <nav class="links node-links clearfix"><?php print render($content['links']); ?></nav>
    <?php endif; ?>

    <?php print render($content['comments']); ?>
  </div>
</article>