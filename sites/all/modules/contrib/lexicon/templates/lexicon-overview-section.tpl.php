<?php
/**
 * @file
 * Default theme implementation for displaying a section of terms in the Lexicon
 * overview page.
 *
 * This template renders a section of Lexicon terms in the Lexicon overview
 * page.
 *
 * Available variables:
 * - $lexicon_section: section object.
 *    - $lexicon_section->letter: The letter the section is for.
 *    - $lexicon_section->id: The id to be used as the anchor of the section.
 * - $lexicon_overview_items: Lexicon overview items as rendered by
 *   lexicon-overview-item.tpl.php
 */
?>
<?php if (isset($lexicon_section)) : ?>
  <a id="<?php print $lexicon_section->id; ?>"></a>
  <h2 class="lexicon-letter"><?php print drupal_strtoupper($lexicon_section->letter); ?></h2>
<?php endif; ?>
<?php if (!empty($lexicon_overview_items)) : ?>
  <dl>
    <?php print $lexicon_overview_items; ?>
  </dl>
<?php else :?>
  <p>
    <?php print(t('There are no terms to display.')); ?>
  </p>
<?php endif; ?>
