<?php
/**
 * @file
 * Default theme implementation to display a term.
 *
 * Available variables:
 * - $name: the (sanitized) name of the term.
 * - $content: An array of items for the content of the term (fields and
 *   description). Use render($content) to print them all, or print a subset
 *   such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $term_url: Direct url of the current term.
 * - $term_name: Name of the current term.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - taxonomy-term: The current template type, i.e., "theming hook".
 *   - vocabulary-[vocabulary-name]: The vocabulary to which the term belongs to.
 *     For example, if the term is a "Tag" it would result in "vocabulary-tag".
 *
 * Other variables:
 * - $term: Full term object. Contains data that may not be safe.
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $page: Flag for the full page state.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the term. Increments each time it's output.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_taxonomy_term()
 * @see template_process()
 */
?>
<div id="taxonomy-term-<?php print $term->tid; ?>" class="<?php print $classes; ?>">
  <?php if (!$page): ?>
    <h2><a href="<?php print $term_url; ?>"><?php print $term_name; ?></a></h2>
  <?php endif; ?>
  <div class="content">
    <div class="field field-type-text field-label-inline clearfix">
      <div class="field-label"><?php
        if (!empty($field_rank[0]['value'])) {
          echo check_plain($field_rank[0]['value']);
        } else {
           echo t('Unranked');
        } ?>:</div>
      <div class="field-items">
        <div class="field-item" style="padding-left:3px;">
        <?php
          // Add author inline with name
          $full_name = theme('scratchpads_species_name', array('term' => $term));
          if (!empty($content['field_authors'][0]['#markup'])) {
            $full_name = $full_name . ' ' . $content['field_authors'][0]['#markup'];
            hide($content['field_authors']);
          }
          echo $full_name;
        ?>
        </div>
      </div>
    </div>
    <?php
      // Hide the rank - is already included in the title
      hide($content['field_rank']);
      // Move page number inline with reference
      if (!empty($content['field_reference'][0]['#title']) && !empty($content['field_page_number'][0]['#markup'])) {
        $content['field_reference'][0]['#suffix'] = ' ' . $content['field_page_number'][0]['#markup'];
        hide($content['field_page_number']);
      }
      foreach (array_keys((array)$term) as $term_field) {
        // Hide all unit name/indicators as they are included in the title
        if (preg_match('/^field_unit_(name|indicator)\d+$/', $term_field)) {
          hide($content[$term_field]);
          continue;
        }
        // Hide empty content
        $has_content = FALSE;
        if(isset($content[$term_field]) && is_array($content[$term_field])){
          foreach (element_children($content[$term_field]) as $key) {
            $elem = $content[$term_field][$key];
            if (!isset($elem['#markup']) || !empty($elem['#markup'])) {
              $has_content = TRUE;
              break;
            }
          }
        }
        if (!$has_content) {
          hide($content[$term_field]);
        }
      }
      print render($content); 
    ?>
  </div>
</div>
