<?php

$plugin = array(
  'form' => 'feeds_tamper_absolute_url_form',
  'callback' => 'feeds_tamper_absolute_url_callback',
  'name' => 'Make URLs absolute',
  'category' => 'HTML',
  'description' => 'feeds_tamper_absolute_url_description',
  'machine_name' => 'feeds_tamper_absolute_url_machine_name',
);

function feeds_tamper_absolute_url_form($importer, $element_key, $settings) {
  $form = array();
  $links = htmlentities('<a href="/stuff/things"> to <a href="http://example.com/stuff/things">');
  $form['info'] = array(
    '#markup' => t('Make URLs in markup absolute. (i.e. ' . $links . ').'),
  );
  return $form;
}

function feeds_tamper_absolute_url_description($settings) {
  return 'Make URLs absolute.';
}

function feeds_tamper_absolute_url_machine_name($settings) {
  return 'make_urls_absolute';
}

function feeds_tamper_absolute_url_callback($result, $item_key, $element_key, &$field, $settings) {
  static $b;
  static $dom;

  if ($b === FALSE) {
    return;
  }

  if (!isset($b)) {
    // parse_url doesn't properly handle urls without a scheme.
    $pos = strpos($result->link, '://');
    // This allows for ftp, ftps, http, https. Not sure if that's necessary but
    // hey.
    if ($pos !== 3 && $pos !== 4 && $pos !== 5) {
      // Assume http if scheme isn't set.
      $b = parse_url('http://' . $result->link);
    }
    else {
      $b = parse_url($result->link);
    }
    // If the host isn't set for the base url then there's nothing we could do.
    // We set $b to false so the next call will know it's pointless.
    if ($b === FALSE || empty($b['host'])) {
      $b = FALSE;
      return;
    }
    // Save the DOMDocument for reuse between calls.
    $dom = new DOMDocument();
  }

  $dom->loadHTML($field);
  $urls = array();
  $tags = array('a' => 'href', 'img' => 'src', 'iframe' => 'src',
                'script' => 'src', 'object' => 'codebase', 'link' => 'href',
                'applet' => 'code', 'base' => 'href');
  foreach ($tags as $tag => $attr) {
    foreach ($dom->getElementsByTagName($tag) as $t) {
      $value = trim($t->getAttribute($attr));
      _feeds_tamper_absolute_url($value, $urls, $b);
    }
  }
  $field = strtr($field, $urls);
}

function _feeds_tamper_absolute_url($r_url, &$urls, $b) {
  $r = parse_url($r_url);
  if ($r == FALSE) {
    return;
  }
  if (!empty($r['scheme']) || !empty($r['host'])) {
    return;
  }

  $r['scheme'] = $b['scheme'];

  unset($r['port']);
  unset($r['user']);
  unset($r['pass']);

  // Copy base authority.
  $r['host'] = $b['host'];
  if (isset($b['port'])) {
    $r['port'] = $b['port'];
  }
  if (isset($b['user'])) {
    $r['user'] = $b['user'];
  }
  if (isset($b['pass'])) {
    $r['pass'] = $b['pass'];
  }

  // If relative URL has no path, use base path
  if (empty($r['path'])) {
    if (!empty($b['path'])) {
      $r['path'] = $b['path'];
    }
    if (!isset($r['query']) && isset($b['query'])) {
      $r['query'] = $b['query'];
    }
    $urls[$r_url] = _feeds_tamper_join_url($r);
    return;
  }

  // If relative URL path doesn't start with /, merge with base path
  if ($r['path'][0] != '/') {
    $base = mb_strrchr($b['path'], '/', TRUE, 'UTF-8');
    if ($base === FALSE) {
      $base = '';
    }
    $r['path'] = $base . '/' . $r['path'];
  }
  $urls[$r_url] = _feeds_tamper_join_url($r);
}

function _feeds_tamper_join_url($parts) {
  $url = '';
  if (!empty($parts['scheme'])) {
    $url .= $parts['scheme'] . ':';
  }
  if (isset($parts['host'])) {
    $url .= '//';
    if (isset($parts['user'])) {
      $url .= $parts['user'];
      if (isset($parts['pass'])) {
        $url .= ':' . $parts['pass'];
      }
      $url .= '@';
    }
    if (preg_match('/!^[\da-f]*:[\da-f.:]+$!ui/', $parts['host'])) {
      $url .= '[' . $parts['host'] . ']'; // IPv6
    }
    else
      $url .= $parts['host'];             // IPv4 or name
    if (isset($parts['port'])) {
      $url .= ':' . $parts['port'];
    }
    if (!empty($parts['path']) && $parts['path'][0] != '/') {
      $url .= '/';
    }
  }
  if (!empty($parts['path'])) {
    $url .= $parts['path'];
  }
  if (isset($parts['query'])) {
    $url .= '?' . $parts['query'];
  }
  if (isset($parts['fragment'])) {
    $url .= '#' . $parts['fragment'];
  }
  return $url;
}
