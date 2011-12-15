<?php
function _endnote8_XML_export($node, $part = 'record') {
  $style_attr = 'face="normal" font="default" size="100%"';
  switch ($part) {
    case 'begin':
      $xml = '<?xml version="1.0" encoding="UTF-8"?>';
      $xml .= "<xml><records>";
      break;
    case 'record':
      $xml = "<record>";
      $xml .= '<source-app name="Biblio" version="7.x">Drupal-Biblio</source-app>';
      $xml .= "<ref-type>" . _endnote8_type_map($node->biblio_type) . "</ref-type>";
      unset($node->biblio_type);
      //<!-- Author information -->
      $xml .= en8_add_contributors($node);
      $xml .= en8_add_titles($node);
      $xml .= en8_add_keywords($node);
      $xml .= en8_add_dates($node);
      $xml .= en8_add_urls($node);

      foreach ($node as $key => $value) {
        $entag = en8_field_map($key);
        if (!empty($entag) && !empty($value)) {
          $xml .=  "<" . $entag . '><style face="normal" font="default" size="100%">' . htmlspecialchars($value) . "</style></$entag>";
        }
      }
      $xml .= "</record>";
      break;
    case 'end':
      $xml = '</records></xml>';
      break;
  }
    return $xml;
}

function en8_encode_font_faces(&$node) {
  $search = array('<b>', '<i>', '<u>', '<sup>', '<sub>', '</b>', '</i>', '</u>', '</sup>', '</sub>');
  $replace = array(
    '<style face="bold" font="default" size="100%">',
    '<style face="italic" font="default" size="100%">',
    '<style face="underline" font="default" size="100%">',
    '<style face="superscript" font="default" size="100%">',
    '<style face="subscript" font="default" size="100%">',
    '</sytle>',
    '</sytle>',
    '</sytle>',
    '</sytle>',
    '</sytle>',
  );
  foreach ($node as $key => $value) {
    $node->$key = str_ireplace($search, $replace, $value);
  }
}

function en8_add_titles(&$node) {
  $xml = '<titles>';
  $xml .= (!empty ($node->title)) ? '<title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->title) . "</style></title>" :'';
  $xml .= (!empty ($node->biblio_secondary_title)) ? '<secondary-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_secondary_title) . "</style></secondary-title>" :'';
  $xml .= (!empty ($node->biblio_tertiary_title)) ? '<tertiary-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_tertiary_title) . "</style></tertiary-title>" :'';
  $xml .= (!empty ($node->biblio_alternate_title)) ? '<alt-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_alternate_title) . "</style></alt-title>" :'';
  $xml .= (!empty ($node->biblio_short_title)) ? '<short-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_short_title) . "</style></short-title>" :'';
  $xml .= (!empty ($node->biblio_translated_title)) ? '<translated-title><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_translated_title) . "</style></translated-title>" :'';
  $xml .= '</titles>';
  unset($node->title);
  unset($node->biblio_secondary_title);
  unset($node->biblio_tertiary_title);
  unset($node->biblio_alternate_title);
  unset($node->biblio_short_title);
  unset($node->biblio_translated_title);

  return $xml;
}
function en8_add_urls(&$node) {
  global $base_path;
  $xml = '';
  // TODO: fix URLS
  if (!empty($node->biblio_url)) {
    $xml .= "<web-urls>";
    $xml .= '<url><style face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_url) . "</style></url>";
    $xml .= "</web-urls>";
  }
  unset($node->biblio_url);
  if (!empty ($node->upload) && count($node->upload['und'])  && user_access('view uploaded files')) {
      $xml .= "<related-urls>";
    foreach ($node->upload['und'] as $file) {
      $xml .= '<url><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(file_create_url($file['uri']));
      $xml .= "</style></url>";
    }
    $xml .= "</related-urls>";
  }
  unset($node->upload['und']);
  if (!empty($xml)) return "<urls>$xml</urls>";
  return ;
}

function en8_add_dates(&$node) {
  $xml = '';
  if (!empty($node->biblio_year) || !empty($node->biblio_date) ) {
    $xml .= '<dates>';
    $xml .=  (!empty($node->biblio_year)) ? '<year><style  face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_year) . "</style></year>":'';
    $xml .=  (!empty($node->biblio_date)) ? '<pub-dates><date><style  face="normal" font="default" size="100%">' . htmlspecialchars($node->biblio_date) . "</style></date></pub-dates>":'';
    $xml .= "</dates>";
  }
  unset($node->biblio_year);
  unset($node->biblio_date);
  return $xml;
}

function en8_add_keywords(&$node) {
  $kw_array = array();
  $xml = '';
  if (!empty($node->biblio_keywords)) {
    foreach ($node->biblio_keywords as $term) {
      $kw_array[] = trim($term);
    }
  }
  if (!empty($kw_array)) {
    $kw_array = array_unique($kw_array);
    $xml .= '<keywords>';
    foreach ($kw_array as $word) {
      $xml .= '<keyword><style  face="normal" font="default" size="100%">' . htmlspecialchars(trim($word)) . "</style></keyword>";
    }
    $xml .= "</keywords>";
  }
  unset($node->biblio_keywords);
  return $xml;
}

function en8_add_contributors(&$node) {
  $xml = '<contributors>';
  $authors = biblio_get_contributor_category($node->biblio_contributors, 1);
  if (!empty($authors)) {
    $xml .= "<authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 2);
  if (!empty($authors)) {
    $xml .= "<secondary-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</secondary-authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 3);
  if (!empty($authors)) {
    $xml .= "<tertiary-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</tertiary-authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 4);
  if (!empty($authors)) {
    $xml .= "<subsidiary-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</subsidiary-authors>";
  }
  $authors = biblio_get_contributor_category($node->biblio_contributors, 5);
  if (!empty($authors)) {
    $xml .= "<translated-authors>";
    foreach ($authors as $auth) {
      $xml .= '<author><style face="normal" font="default" size="100%">';
      $xml .= htmlspecialchars(trim($auth['name'])); // insert author here.
      $xml .= "</style></author>";
    }
    $xml .= "</translated-authors>";
  }
  $xml .= '</contributors>';
  unset($node->biblio_contributors);
  return $xml;
}

function en8_field_map($biblio_field) {
  static $fmap = array();
  if (empty($fmap)) {
    $fmap = biblio_get_map('field_map', 'endnote8');
  }
  return ($en8_field = array_search($biblio_field, $fmap)) ? $en8_field : '';
}

function _endnote8_type_map($bibliotype) {
  static $map = array();
  if (empty($map)) {
    $map = biblio_get_map('type_map', 'endnote8');
  }
  return ($en8_type = array_search($bibliotype, $map)) ? $en8_type : 13; //return the biblio type or 129 (Misc) if type not found
}
