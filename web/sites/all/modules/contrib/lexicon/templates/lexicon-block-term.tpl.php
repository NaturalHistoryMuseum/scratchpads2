<?php
/**
 * @file
 * Default theme implementation for displaying a single Lexicon term in the
 * Lexicon random term block.
 *
 * This template renders a single Lexicon term in the Lexicon random term block.
 *
 * Available variables:
 * - $term: term object.
 *    - $term->tid: the term id.
 *    - $term->id: the id to be used as the anchor name.
 *    - $term->name: the term name.
 *    - $term->safe_name: the sanitized term name (use to prevent XSS
 *       vulnerability)
 *    - $term->image: the term image (if any) as a named array containing
 *      image["uri"], image["alt"] and image["title"].
 *    - $term->link: the link to the term (if enabled) as a named array
 *      containing link["path"] and link["fragment"].
 *    - $term->description: the term description (if any)
 *    - $term->safe_description: the sanitized term description (if any; use to
 *      prevent XSS vulnerability).
 *    - $term->related: the related terms (if any) as an array containing
 *      related terms as an array containing ["name"] and ["link"] as a named
 *      array containing "path" and "fragment".
 *    - $term->synonyms: the term synonyms (if any) in an array
 *    - $term->adminlinks: the adminlinks (if any) in an array containing
 *      ["name"], ["path"] and ["attributes"] per element.
 */
?>
<div class="lexicon-block-term">
  <?php if (isset($term->image)) : ?>
    <div class="lexicon-block-term-image">
      <img src="<?php print image_style_url('thumbnail', $term->image["uri"]); ?>" alt="<?php print $term->image["alt"]; ?>" title="<?php print $term->image["title"]; ?>" />
    </div>
  <?php endif; ?>
  <div class="lexicon-block-term-name">
    <?php if (isset($term->link)) : ?>
      <?php print l($term->safe_name, $term->link["path"], array('fragment' => $term->link["fragment"])); ?>
    <?php else : ?>
      <?php print $term->safe_name; ?>
    <?php endif; ?>
    <?php if (isset($term->adminlinks)) : ?>
      <span class="lexicon-adminlinks">
      <?php foreach ($term->adminlinks as $adminlink) : ?>
        <?php print l($adminlink["name"], $adminlink["path"], $adminlink["attributes"]); ?>
      <?php endforeach; ?>
      </span>
    <?php endif; ?>
  </div>
  <?php if (isset($term->safe_description)) : ?>
    <div class="lexicon-block-term-description">
      <?php print $term->safe_description; ?>
    </div>
  <?php endif; ?>
  <?php if (isset($term->related)) : ?>
    <div class="lexicon-related">
      <?php print t('See also') . ': '; ?>
      <?php foreach ($term->related as $related_term) : ?>
        <?php if (isset($related_term["link"])) : ?>
          <?php print l($related_term["name"], $related_term["link"]["path"], array('fragment' => $related_term["link"]["fragment"])); ?>
        <?php else : ?>
          <?php print $related_term["name"]; ?>
        <?php endif; ?>
      <?php endforeach; ?>
    </div>
  <?php endif; ?>
  <?php if (isset($term->synonyms)) : ?>
    <div class="lexicon-synonyms">
      <?php print t('Synonyms') . ': '; ?>
      <?php foreach ($term->synonyms as $synonym) : ?>
        <?php print $synonym; ?>
      <?php endforeach; ?>
    </div>
  <?php endif; ?>
</div>
