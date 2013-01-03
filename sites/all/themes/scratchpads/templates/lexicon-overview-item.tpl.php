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
<?php if (isset($term->description) || isset($term->synonyms) || isset($term->field_illustration) || isset ($term->related)) : ?>
  <dd>
  <?php if (isset($term->description)) : ?>
    <?php print $term->description; ?>
  <?php endif; ?>
  <?php if (isset($term->field_illustration[LANGUAGE_NONE])) : ?>
    <p>
    <?php foreach($term->field_illustration[LANGUAGE_NONE] as $image):?>
      <a href="/file-colorboxed/<?php print $image['fid'];?>" class="colorbox" rel="group-<?php print $term->id; ?>" title="<?php print $image["title"]; ?>">
        <img src="<?php print image_style_url('thumbnail', $image["uri"]); ?>" alt="<?php print $image["alt"]; ?>" title="<?php print $image["title"]; ?>" />
      </a>
    <?php endforeach;?>
    </p>
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

