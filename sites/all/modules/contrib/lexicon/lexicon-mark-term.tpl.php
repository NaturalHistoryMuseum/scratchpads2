<?php
/**
 * @file lexicon-mark-term.tpl.php
 * Default theme implementation for marking a term in content.
 *
 * This template renders the marking of a term in the content.
 *
 * Available variables:
 * - $term: Array containing the term that is marked and additional information for creating an optional link.
 *	 	- ["term"]: Term object of the term that is marked.
 *		- ["absolute_link"]: to use in the l() function. Filled according to Lexicon configuration setting.
 *		- ["linkto"]: the path to the term. Filled according to Lexicon configuration setting.
 *		- ["fragment"]: the fragment to use in the l() function. Filled according to Lexicon configuration setting.
 *		- ["term_class"]: the class to use for the wrapper around the marked term. Filled according to Lexicon configuration setting.
 * - $text: term to mark.
 *
 * Example use for creating a simple hyperlink with a class and title:
 * <?php print l($text, $term["linkto"], array(
 *   'attributes' => array(
 *     'title' => drupal_html_to_text($term["term"]->description),
 *   	 'class' => array($term["term_class"]),
 *     ),
 *   'fragment' => $term["fragment"],
 *   'absolute' => $term["absolute_link"]
 *   ));?>
 */
?>
<?php print l($text, $term["linkto"], array(
	'attributes' => array(
		'title' => check_plain($term["term"]->description),
  	'class' => array($term["term_class"]),
    ),
  'fragment' => $term["fragment"],
  'absolute' => $term["absolute_link"]
  ));?>