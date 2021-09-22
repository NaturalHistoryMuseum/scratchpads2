<?php

/**
 * hook_citethispage_info
 * 
 * Returns information about services this module can provide
 * to the Cite This Page functionality. There are two types of
 * services: 
 * - archiving, for modules that provide an archiving backend
 * - parsing, for modules that can parse a given path and return
 *   citation-relevant information (such as authors)
 * 
 * Modules should return an array indexed by the module's name,
 * and that can provide (depending on what functionality the module
 * provides) the following definition:
 * 
 * 'archive' => array(
 *   '<name>' => array(
 *     'title' => '...',            // Title of the archiving backend
 *     'description' => '...',      // Description of the archiving backend
 *     'callback' => '...'          // Function to call when the archiving backend is invoked
 *   ),
 *   '<another>' => array(
 *     // ...
 *   )
 * ),
 * 
 * 'parse' => array(
 *   '<a/path/%>' => array(
 *     'title' => '...',         // Title of this parser
 *     'description' => '...',   // Description of this parser
 *     'callback' => '...'       // Function to call when invoking this parser
 *   ),
 *   '<another/path>' => array(
 *     // ...
 *   )
 * )
 * 
 * The archive callback should expect two parameters:
 * - the page path,
 * - the result array provided by parsers.
 * 
 * It is the backend's responsibility to provide the user with the
 * archiving's resulting link. As such, this callback may require more
 * information from the user, redirect the page if needed, and so on.
 * 
 * The parse callback should expect one parameter:
 * - the page path
 * 
 * The parse callback should return an array of parsed values. It may return
 * an empty array. A value that is strictly equal to FALSE will mean that the
 * parser cannot parse the specific path - in which case the default parser
 * may be invoked.
 * 
 * Note that neither of these callbacks should expect the current path to
 * be the same as the path that needs processing.
 * 
 */
function hook_citethispage_info(){
  // Provide parsing info for taxonomy terms
  $parsers = array(
    'taxonomy/term/%' => array(
      'title' => t('Taxonomy term page parser'),
      'description' => t('Parses the authors of a taxonomy page'),
      'callback' => 'hook_taxonomy_term_parser'
    )
  );
  // Provide a backend for an online archiving service
  $archivers = array(
    'anonlinearchivingservice' => array(
      'title' => t('An online archiving service'),
      'description' => t('This stores the snapshot of your page on http://anonlinearchivingservice'),
      'callback' => 'myothermodule_archive_callback'
    )
  );
  return array(
    'mymodule' => array(
      'parse' => $parsers,
      'archive' => $archivers
    )
  );
}

/**
 * hook_citethispage_info_alter(&$info)
 * 
 * Alter the gathered Cite This Page info
 */
function hook_citethispage_info_alter(&$info){
  // Don't provide parsing for file pages
  unset($info['citethispage']['parse']['file/%']);
}

/**
 * hook_citethispage_parse_alter
 * 
 * Alter the parsed result for a given path
 */
function hook_citethispage_parse_alter($path_pattern, &$results){
  // Only provide author initials for file pages
  if($path_pattern == 'file/%' && !empty($results['authors'])){
    foreach($results['authors'] as $author_index => $author_name){
      $results['authors'][$author_index] = preg_replace('/(\w)\w*/', '$1.', $author_name);
    }
  }
}