<?php
/**
 * @file lexicon-overview-item.tpl.php
 * Default theme implementation for displaying a single Lexicon term in the Lexicon overview page.
 *
 * This template renders a single Lexicon term in the Lexicon overview page.
 *
 * Available variables:
 * - $term: term object.
 *    - $term->tid: the term id.
 *    - $term->id: the id to be used as the anchor name/fragment.
 *    - $term->name: the term name.
 *    - $term->image: the term image (if any) as a named array containing image["uri"], image["alt"] and image["title"].
 *    - $term->description: the term description (if any).
 *    - $term->related: the related terms (if any) as an array containing related terms as an array containing ["name"] and ["link"] as a named array containing "path" and "fragment".
 *    - $term->synonyms: the term synonyms (if any) in an array.
 *    - $term->extralinks: the extralinks (if any) in an array containing ["name"], ["path"] and ["attributes"] per element.
 */
?>
<dt>
	<a id="<?php print $term->id; ?>"></a>
  <?php print $term->name; ?>
  <?php if (isset($term->extralinks)) : ?>
    <span class="lexicon-extralinks">
    <?php foreach ($term->extralinks as $link) : ?>
      <?php print l($link["name"], $link["path"], $link["attributes"]); ?>
    <?php endforeach; ?>
    </span>
  <?php endif; ?>
</dt>
<?php if (isset($term->description) || isset($term->synonyms) || isset($term->image) || isset ($term->related)) : ?>
<dd>
  <?php foreach($term as $key => $array) : ?>
    <?php if (is_array($array) && isset($array[LANGUAGE_NONE][0]['fid'])) : ?>
      <?php $entities = array(); ?>
      <?php foreach($array[LANGUAGE_NONE] as $image) : ?>
        <?php $entities[] = $image['fid']; ?>
      <?php endforeach ?>
      <?php $entities = entity_load('file', $entities); ?>
      <div class="clearfix">
        <?php print drupal_render(entity_view('file', $entities, 'file_styles_square_thumbnail')); ?>
      </div>
    <?php endif; ?>
  <?php endforeach; ?>
  <?php if (isset($term->description)) : ?>
    <?php print $term->description; ?>
  <?php endif; ?>
  <?php if (isset($term->related)) : ?>
  <p class="lexicon-related">
    <?php print t('See also') . ': '; ?>
    <?php foreach ($term->related as $related_term) : ?>
      <?php if (isset($related_term["link"])) : ?>
        <?php print l($related_term["name"], $related_term["link"]["path"], array('fragment' => $related_term["link"]["fragment"])); ?>
      <?php else : ?>
        <?php print $related_term["name"]; ?>
      <?php endif; ?>
    <?php endforeach; ?>
  </p>
  <?php endif; ?>
  <?php if (isset($term->synonyms)) : ?>
  <p class="lexicon-synonyms">
    <?php print t('Synonyms') . ': '; ?>
    <?php foreach ($term->synonyms as $synonym) : ?>
      <?php print $synonym; ?>
    <?php endforeach; ?>
  </p>
  <?php endif; ?>
  </dd>
<?php endif; ?>


