<div<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <a id="main-content"></a>
    <?php if ($title): ?>
    <?php if ($title_hidden): ?><div class="element-invisible"><?php endif; ?>
    <?php print render($title_prefix); ?>
    <h1 class="title" id="page-title">
    <?php print (function_exists('scratchpads_tweaks_translate_after_check_plain') ? scratchpads_tweaks_translate_after_check_plain($title): $title); ?>
    <?php if ($subtitle): ?><span><?php print $subtitle; ?></span><?php endif; ?>
    </h1>
    <?php print render($title_suffix); ?>
    <?php if ($title_hidden): ?></div><?php endif; ?>
    <?php endif; ?>
    
    <?php if ($tabs): ?>
      <div class="tabs clearfix">
        <?php 

          /*
            specifically for taxonomy term pages
            
            hide non-biological tabs
            
            doing it here in this theme template/at rendering stage in theme
            seems to be the only place where this is possible in that the
            required variables are available make the decision on to display tab, are 
            available.
          */
          $tabs_items = &$tabs['#primary'];

          $systemTabsAsArray = array('View', 'Edit', 'Clone', 'Clone content');

          foreach($tabs_items as $tab_id => $tab_value ) {
            if (array_key_exists('#link', $tabs_items[$tab_id])) {
              $tab_item = $tabs_items[$tab_id]['#link'];

              if (array_key_exists('tab_root_href',$tab_item)) { 
                // ensure that doing the hiding only applies to taxonomy pages
                if (strpos($tab_item['tab_root_href'], 'term') !== false) {

                  // and for a term, allow Drupal system tabs to be displayed
                  // i.e. don't do the checking further inside the if
                  if (array_key_exists('title', $tab_item)) {
                    $tab_title = $tab_item['title'];
                    if (!in_array($tab_title, $systemTabsAsArray)) {
                      $term_id = basename($tab_item['tab_root_href']);
          
                      $term_obj = taxonomy_term_load($term_id);
                      // don't display term tabs that are not biological classifications
                      if ($term_obj) {
                        if (!(scratchpads_species_term_is_biological_classification($term_obj))) {
                          unset($tabs_items[$tab_id]);
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          print render($tabs); 
        ?>
      </div>
    <?php endif; ?>
    <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
    <?php print function_exists('scratchpads_tweaks_translate_after_check_plain') ? scratchpads_tweaks_translate_after_check_plain($content) : $content; ?>
    <?php if ($feed_icons): ?><div class="feed-icon clearfix"><?php print $feed_icons; ?></div><?php endif; ?>
  </div>
</div>