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
            hide all but the View tab for terms that are non biological

            doing it here in this theme template/at rendering stage in theme
            seems to be the only place where this is possible in that the
            required variables are available make the decision on to display tab, are 
            available.
          */
          $tabs_items = &$tabs['#primary'];

           // if we are on user pages 
           //   (credit: https://drupal.stackexchange.com/a/83872)
           // then don't do below where check what tabs to display
          if (arg(0) == 'user') {
            foreach($tabs_items as $tab_id => $tab_value ) {
              if (array_key_exists('#link', $tabs_items[$tab_id])) {
                $tab_item = $tabs_items[$tab_id]['#link'];

                if (array_key_exists('title',$tab_item)) {
                  $tab_title = $tab_item['title'];

                  // if the tab being looked at is NOT a standard general Drupal tab 
                  // then we make a decision on whether to omit the tab, 
                  //   because we assume it is a Scratchpads domain-specific tab 
                  //    which is conditionally displayed if bio/non-bio
                  $standardTabsAsArray = array("View", "Edit", "Clone", "Clone content", "Clone", "Devel");
                  if (!in_array($tab_title, $standardTabsAsArray)){
                    if (array_key_exists('tab_root_href',$tab_item)) { 
                      $term_id = basename($tab_item['tab_root_href']);
                      $term_obj = taxonomy_term_load($term_id);
                      if ($term_obj) {
                        // is this a user page? if yes then we don't want to hide the tabs
                        // if not then we do
                        //
                        // check existence first before attempting to check value, avoiding undefined warnings
                        // we have enough of those in the code base
                        if (array_key_exists('system_main', $elements)) {
                          if (array_key_exists('#pre_render',$elements['system_main'])) {
                            if( array_key_exists('#entity_type',$elements['system_main']['#pre_render'])) {
                              if (!($elements['system_main']['#pre_render']['#entity_type'] == 'user')) {
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
