<?php

/**
 * @file
 * CiteProc-PHP.
 *
 *   Copyright (C) 2010 - 2011  Ron Jerome, all rights reserved.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 *
 */
class citeproc {
  public    $bibliography;
  public    $citation;
  public    $style;
  protected $macros;
  private   $info;
  protected $locale;
  protected $style_locale;
  public    $quash;
  private   $mapper;

  /**
   *
   */
  public function __construct($csl = NULL, $lang = 'en') {
    if ($csl) {
      $this->init($csl, $lang);
    }
  }

  /**
   *
   */
  public function init($csl, $lang) {
    $this->mapper = new csl_mapper();
    $this->quash = array();

    $csl_doc = new DOMDocument();

    if ($csl_doc->loadXML($csl)) {

      $style_nodes = $csl_doc->getElementsByTagName('style');
      if ($style_nodes) {
        foreach ($style_nodes as $style) {
          $this->style = new csl_style($style);
        }
      }

      $info_nodes = $csl_doc->getElementsByTagName('info');
      if ($info_nodes) {
        foreach ($info_nodes as $info) {
          $this->info = new csl_info($info);
        }
      }

      $this->locale = new csl_locale($lang);
      $this->locale->set_style_locale($csl_doc);

      $macro_nodes = $csl_doc->getElementsByTagName('macro');
      if ($macro_nodes) {
        $this->macros = new csl_macros($macro_nodes, $this);
      }

      $citation_nodes = $csl_doc->getElementsByTagName('citation');
      foreach ($citation_nodes as $citation) {
        $this->citation = new csl_citation($citation, $this);
      }

      $bibliography_nodes = $csl_doc->getElementsByTagName('bibliography');
      foreach ($bibliography_nodes as $bibliography) {
        $this->bibliography = new csl_bibliography($bibliography, $this);
      }
    }
  }

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $text = '';
    switch ($mode) {
      case 'citation':
        $text .= (isset($this->citation)) ? $this->citation->render($data) : '';
        break;

      case  'bibliography':
      default:
        $text .= (isset($this->bibliography)) ? $this->bibliography->render($data) : '';
        break;
    }
    return $text;
  }

  /**
   *
   */
  public function render_macro($name, $data, $mode) {
    return $this->macros->render_macro($name, $data, $mode);
  }

  /**
   *
   */
  public function get_locale($type, $arg1, $arg2 = NULL, $arg3 = NULL) {
    return $this->locale->get_locale($type, $arg1, $arg2, $arg3);
  }

  /**
   *
   */
  public function map_field($field) {
    if ($this->mapper) {
      return $this->mapper->map_field($field);
    }
    return ($field);
  }

  /**
   *
   */
  public function map_type($field) {
    if ($this->mapper) {
      return $this->mapper->map_type($field);
    }
    return ($field);
  }

}

/**
 *
 */
class csl_factory {

  /**
   *
   */
  public static function create($dom_node, $citeproc = NULL) {
    $class_name = 'csl_' . str_replace('-', '_', $dom_node->nodeName);
    if (class_exists($class_name)) {
      return new $class_name($dom_node, $citeproc);
    }
    else {
      return NULL;
    }
  }

}

/**
 *
 */
class csl_collection {
  protected  $elements = array();

  /**
   *
   */
  public function add_element($elem) {
    if (isset($elem)) {
      $this->elements[] = $elem;
    }
  }

  /**
   *
   */
  public function render($data, $mode = NULL) {}

  /**
   *
   */
  public function format($text) {
    return $text;
  }

}

/**
 *
 */
class csl_element extends csl_collection {
  protected $attributes = array();
  protected $citeproc;

  /**
   *
   */
  public function __construct($dom_node = NULL, $citeproc = NULL) {

    $this->citeproc = &$citeproc;
    $this->set_attributes($dom_node);
    $this->init($dom_node, $citeproc);

  }

  /**
   *
   */
  public function init($dom_node, $citeproc) {
    if (!$dom_node) {
      return;
    }

    foreach ($dom_node->childNodes as $node) {
      if ($node->nodeType == 1) {
        $this->add_element(csl_factory::create($node, $citeproc));
      }
    }
  }

  /**
   *
   */
  public function __set($name, $value) {
    $this->attributes[$name] = $value;
  }

  /**
   *
   */
  public function __isset($name) {
    return isset($this->attributes[$name]);
  }

  /**
   *
   */
  public function __unset($name) {
    unset($this->attributes[$name]);
  }

  /**
   *
   */
  public function &__get($name = NULL) {
    $null = NULL;
    if (array_key_exists($name, $this->attributes)) {
      return $this->attributes[$name];
    }
    if (isset($this->{$name})) {
      return $this->{$name};
    }
    return $null;

  }

  /**
   *
   */
  public function set_attributes($dom_node) {
    $att = array();
    $element_name = $dom_node->nodeName;
    if (isset($dom_node->attributes->length)) {
      for ($i = 0; $i < $dom_node->attributes->length; $i++) {
        $value = $dom_node->attributes->item($i)->value;
        $name = str_replace(' ', '_', $dom_node->attributes->item($i)->name);
        if ($name == 'type') {
          $value = $this->citeproc->map_type($value);
        }

        if (($name == 'variable'  || $name == 'is-numeric') && $element_name != 'label') {
          $value = $this->citeproc->map_field($value);
        }
        $this->{$name} = $value;
      }
    }
  }

  /**
   *
   */
  public function get_attributes() {
    return $this->attributes;
  }

  /**
   *
   */
  public function get_hier_attributes() {
    $hier_attr = array();
    $hier_names = array('and', 'delimiter-precedes-last', 'et-al-min', 'et-al-use-first',
      'et-al-subsequent-min', 'et-al-subsequent-use-first', 'initialize-with',
      'name-as-sort-order', 'sort-separator', 'name-form', 'name-delimiter',
      'names-delimiter',
    );
    foreach ($hier_names as $name) {
      if (isset($this->attributes[$name])) {
        $hier_attr[$name] = $this->attributes[$name];
      }
    }
    return $hier_attr;
  }

  /**
   *
   */
  public function name($name = NULL) {
    if ($name) {
      $this->name = $name;
    }
    else {
      return str_replace(' ', '_', $this->name);
    }
  }

}

/**
 *
 */
class csl_rendering_element extends csl_element {

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $text = '';
    $text_parts = array();

    $delim = $this->delimiter;
    foreach ($this->elements as $element) {
      $text_parts[] = $element->render($data, $mode);
    }
    // Insert the delimiter if supplied.
    $text = implode($delim, $text_parts);

    return $this->format($text);
  }

}

/**
 *
 */
class csl_format extends csl_rendering_element {
  protected $no_op;
  protected $format;

  /**
   *
   */
  public function __construct($dom_node = NULL, $citeproc = NULL) {
    parent::__construct($dom_node, $citeproc);
    $this->init_formatting();
  }

  /**
   *
   */
  public function init_formatting() {
    $this->no_op = TRUE;
    $this->format = '';
    if (isset($this->quotes)  && strtolower($this->quotes) == "true") {
      $this->quotes = array();
      $this->quotes['punctuation-in-quote'] = $this->citeproc->get_locale('style_option', 'punctuation-in-quote');
      $this->quotes['open-quote'] = $this->citeproc->get_locale('term', 'open-quote');
      $this->quotes['close-quote'] = $this->citeproc->get_locale('term', 'close-quote');
      $this->quotes['open-inner-quote'] = $this->citeproc->get_locale('term', 'open-inner-quote');
      $this->quotes['close-inner-quote'] = $this->citeproc->get_locale('term', 'close-inner-quote');
      $this->no_op = FALSE;
    }
    if (isset($this->{'prefix'})) {
      $this->no_op = FALSE;
    }
    if (isset($this->{'suffix'})) {
      $this->no_op = FALSE;
    }
    if (isset($this->{'display'})) {
      $this->no_op = FALSE;
    }

    $this->format .= (isset($this->{'font-style'})) ? 'font-style: ' . $this->{'font-style'} . ';' : '';
    $this->format .= (isset($this->{'font-family'})) ? 'font-family: ' . $this->{'font-family'} . ';' : '';
    $this->format .= (isset($this->{'font-weight'})) ? 'font-weight: ' . $this->{'font-weight'} . ';' : '';
    $this->format .= (isset($this->{'font-variant'})) ? 'font-variant: ' . $this->{'font-variant'} . ';' : '';
    $this->format .= (isset($this->{'text-decoration'})) ? 'text-decoration: ' . $this->{'text-decoration'} . ';' : '';
    $this->format .= (isset($this->{'vertical-align'})) ? 'vertical-align: ' . $this->{'vertical-align'} . ';' : '';
    // $this->format .= (isset($this->{'display'})  && $this->{'display'}  == 'indent')  ? 'padding-left: 25px;' : '';.
    if (isset($this->{'text-case'}) ||
        !empty($this->format) ||
        !empty($this->span_class) ||
        !empty($this->div_class)) {
      $this->no_op = FALSE;
    }

  }

  /**
   *
   */
  public function format($text) {

    if (empty($text) || $this->no_op) {
      return $text;
    }
    $quotes = $this->quotes;
    $quotes = is_array($quotes) ? $quotes : array();

    if (isset($this->{'text-case'})) {
      switch ($this->{'text-case'}) {
        case 'uppercase':
          $text = drupal_strtoupper($text);
          break;

        case 'lowercase':
          $text = drupal_strtolower($text);
          break;

        case 'capitalize-all':
        case 'title':
          $text = mb_convert_case($text, MB_CASE_TITLE);
          break;

        case 'capitalize-first':
          $text = drupal_ucfirst($text);
          break;
      }
    }

    $prefix = $this->prefix;
    $prefix .= isset($quotes['open-quote']) ? $quotes['open-quote'] : '';
    $suffix = $this->suffix;
    if (isset($quotes['close-quote']) && !empty($suffix) && isset($quotes['punctuation-in-quote'])) {
      if (strpos($suffix, '.') !== FALSE || strpos($suffix, ',') !== FALSE) {
        $suffix = $suffix . $quotes['close-quote'];
      }
    }
    elseif (isset($quotes['close-quote'])) {
      $suffix = $quotes['close-quote'] . $suffix;
    }
    // Gaurd against repeaded suffixes...
    if (!empty($suffix)) {
      $no_tags = strip_tags($text);
      if (strlen($no_tags) && ($no_tags[(strlen($no_tags) - 1)] == $suffix[0])) {
        $suffix = substr($suffix, 1);
      }
    }

    if (!empty($this->format) || !empty($this->span_class)) {
      $style = (!empty($this->format)) ? 'style="' . $this->format . '"' : '';
      $class = (!empty($this->span_class)) ? 'class="' . $this->span_class . '"' : '';
      $text = '<span ' . implode(' ', array($class, $style)) . '>' . $text . '</span>';
    }
    $div_class = $div_style = '';
    if (!empty($this->div_class)) {
      $div_class = (!empty($this->div_class)) ? 'class="' . $this->div_class . '"' : '';
    }
    if ($this->display == 'indent') {
      $div_style = 'style="text-indent: 0px; padding-left: 45px;"';
    }
    if ($div_class || $div_style) {
      return '<div ' . $div_class . $div_style . '>' . $prefix . $text . $suffix . '</div>';
    }

    return $prefix . $text . $suffix;
  }

}

/**
 *
 */
class csl_info {
  public $title;
  public $id;
  public $authors = array();
  public $links = array();

  /**
   *
   */
  public function __construct($dom_node) {
    $name = array();
    foreach ($dom_node->childNodes as $node) {
      if ($node->nodeType == 1) {
        switch ($node->nodeName) {
          case 'author':
          case 'contributor':
            foreach ($node->childNodes as $authnode) {
              if ($node->nodeType == 1) {
                $name[$authnode->nodeName] = $authnode->nodeValue;
              }
            }
            $this->authors[] = $name;
            break;

          case 'link':
            foreach ($node->attributes as $attribute) {
              $this->links[] = $attribute->value;
            }
            break;

          default:
            $this->{$node->nodeName} = $node->nodeValue;
        }
      }
    }

  }

}

/**
 *
 */
class csl_terms {

}

/**
 *
 */
class csl_name extends csl_format {
  private $name_parts = array();
  private $attr_init = FALSE;

  /**
   *
   */
  public function __construct($dom_node, $citeproc = NULL) {

    $tags = $dom_node->getElementsByTagName('name-part');
    if ($tags) {
      foreach ($tags as $tag) {
        $name_part = $tag->getAttribute('name');
        $tag->removeAttribute('name');
        for ($i = 0; $i < $tag->attributes->length; $i++) {
          $value = $tag->attributes->item($i)->value;
          $name = str_replace(' ', '_', $tag->attributes->item($i)->name);
          $this->name_parts[$name_part][$name] = $value;
        }
      }
    }

    parent::__construct($dom_node, $citeproc);
  }

  /**
   *
   */
  public function init_formatting() {
    $this->no_op = array();
    $this->format = array();
    $this->base = $this->get_attributes();
    $this->format['base'] = '';
    $this->format['family'] = '';
    $this->format['given'] = '';
    $this->no_op['base'] = TRUE;
    $this->no_op['family'] = TRUE;
    $this->no_op['given'] = TRUE;

    if (isset($this->prefix)) {
      $this->no_op['base'] = FALSE;
    }
    if (isset($this->suffix)) {
      $this->no_op['base'] = FALSE;
    }
    $this->init_format($this->base);

    if (!empty($this->name_parts)) {
      foreach ($this->name_parts as $name => $formatting) {
        $this->init_format($formatting, $name);
      }
    }
  }

  /**
   *
   */
  public function init_attrs($mode) {
    // $and = $this->get_attributes('and');.
    if (isset($this->citeproc)) {
      $style_attrs = $this->citeproc->style->get_hier_attributes();
      $mode_attrs = $this->citeproc->{$mode}->get_hier_attributes();
      $this->attributes = array_merge($style_attrs, $mode_attrs, $this->attributes);
    }
    if (isset($this->and)) {
      if ($this->and == 'text') {
        $this->and = $this->citeproc->get_locale('term', 'and');
      }
      elseif ($this->and == 'symbol') {
        $this->and = '&';
      }
    }
    if (!isset($this->delimiter)) {
      $this->delimiter = $this->{'name-delimiter'};
    }
    if (!isset($this->alnum)) {
      list($this->alnum, $this->alpha, $this->cntrl, $this->dash,
      $this->digit, $this->graph, $this->lower, $this->print,
      $this->punct, $this->space, $this->upper, $this->word,
      $this->patternModifiers) = $this->get_regex_patterns();
    }
    $this->dpl = $this->{'delimiter-precedes-last'};
    $this->sort_separator = isset($this->{'sort-separator'}) ? $this->{'sort-separator'} : ', ';

    $this->delimiter = isset($this->{'name-delimiter'}) ? $this->{'name-delimiter'} : (isset($this->delimiter) ? $this->delimiter : ', ');

    $this->form = isset($this->{'name-form'}) ? $this->{'name-form'} : (isset($this->form) ? $this->form : 'long');
    $this->attr_init = $mode;
  }

  /**
   *
   */
  public function init_format($attribs, $part = 'base') {
    if (!isset($this->{$part})) {
      $this->{$part} = array();
    }
    if (isset($attribs['quotes']) && strtolower($attribs['quotes']) == 'true') {
      $this->{$part}['open-quote'] = $this->citeproc->get_locale('term', 'open-quote');
      $this->{$part}['close-quote'] = $this->citeproc->get_locale('term', 'close-quote');
      $this->{$part}['open-inner-quote'] = $this->citeproc->get_locale('term', 'open-inner-quote');
      $this->{$part}['close-inner-quote'] = $this->citeproc->get_locale('term', 'close-inner-quote');
      $this->no_op[$part] = FALSE;
    }

    if (isset($attribs['prefix'])) {
      $this->{$part}['prefix'] = $attribs['prefix'];
    }
    if (isset($attribs['suffix'])) {
      $this->{$part}['suffix'] = $attribs['suffix'];
    }

    $this->format[$part] .= (isset($attribs['font-style'])) ? 'font-style: ' . $attribs['font-style'] . ';' : '';
    $this->format[$part] .= (isset($attribs['font-family'])) ? 'font-family: ' . $attribs['font-family'] . ';' : '';
    $this->format[$part] .= (isset($attribs['font-weight'])) ? 'font-weight: ' . $attribs['font-weight'] . ';' : '';
    $this->format[$part] .= (isset($attribs['font-variant'])) ? 'font-variant: ' . $attribs['font-variant'] . ';' : '';
    $this->format[$part] .= (isset($attribs['text-decoration'])) ? 'text-decoration: ' . $attribs['text-decoration'] . ';' : '';
    $this->format[$part] .= (isset($attribs['vertical-align'])) ? 'vertical-align: ' . $attribs['vertical-align'] . ';' : '';

    if (isset($attribs['text-case'])) {
      $this->no_op[$part] = FALSE;
      $this->{$part}['text-case'] = $attribs['text-case'];
    }
    if (!empty($this->format[$part])) {
      $this->no_op[$part] = FALSE;
    }

  }

  /**
   *
   */
  public function format($text, $part = 'base') {

    if (empty($text) || $this->no_op[$part]) {
      return $text;
    }
    if (isset($this->{$part}['text-case'])) {
      switch ($this->{$part}['text-case']) {
        case 'uppercase':
          $text = drupal_strtoupper($text);
          break;

        case 'lowercase':
          $text = drupal_strtolower($text);
          break;

        case 'capitalize-all':
          $text = mb_convert_case($text, MB_CASE_TITLE);
          break;

        case 'capitalize-first':
          $text = drupal_ucfirst($text);
          break;
      }
    }
    $open_quote = isset($this->{$part}['open-quote']) ? $this->{$part}['open-quote'] : '';
    $close_quote = isset($this->{$part}['close-quote']) ? $this->{$part}['close-quote'] : '';
    $prefix = isset($this->{$part}['prefix']) ? $this->{$part}['prefix'] : '';
    $suffix = isset($this->{$part}['suffix']) ? $this->{$part}['suffix'] : '';
    if ($text[(strlen($text) - 1)] == $suffix) {
      unset($suffix);
    }
    if (!empty($this->format[$part])) {
      $text = '<span style="' . $this->format[$part] . '">' . $text . '</span>';
    }
    return $prefix . $open_quote . $text . $close_quote . $suffix;
  }

  /**
   *
   */
  public function author_link($author) {
    $base = variable_get('biblio_base', 'biblio');
    $options = array();

    if (isset($_GET['sort'])) {
      $options['query']['sort'] = $_GET['sort'];
    }
    if (isset($_GET['order'])) {
      $options['query']['order'] = $_GET['order'];
    }

    $html = l(trim($author['name']), "$base/author/" . $author['cid'], $options);

    return $html;
  }

  /**
   *
   */
  public function render($names, $mode = NULL) {
    $text = '';
    $authors = array();
    $count = 0;
    $auth_count = 0;
    $et_al_triggered = FALSE;

    if (!$this->attr_init || $this->attr_init != $mode) {
      $this->init_attrs($mode);
    }

    $initialize_with = $this->{'initialize-with'};
    $options = array('html' => FALSE);

    foreach ($names as $rank => $name) {
      if (empty($name['literal'])) {
        if (!isset($name['lastname'])) {
          module_load_include('inc', 'biblio', '/includes/biblio.contributors');
          // This is needed for form preview to fill in all fields.
          $name = biblio_parse_author($name);
        }
        $count++;
        if (!empty($name['firstname']) && isset($initialize_with)) {
          $name['firstname'] = preg_replace("/([$this->upper])[$this->lower]+/$this->patternModifiers", '\\1', $name['firstname']);
          $name['firstname'] = preg_replace("/(?<=[-$this->upper]) +(?=[-$this->upper])/$this->patternModifiers", "", $name['firstname']);
          $name['initials'] = $name['firstname'] . $name['initials'];
        }
        if (isset($name['initials'])) {
          // Within initials, remove any dots:
          $name['initials'] = preg_replace("/([$this->upper])\.+/$this->patternModifiers", "\\1", $name['initials']);
          // Within initials, remove any spaces *between* initials:
          $name['initials'] = preg_replace("/(?<=[-$this->upper]) +(?=[-$this->upper])/$this->patternModifiers", "", $name['initials']);
          if (isset($this->citeproc->style)  && $this->citeproc->style->{'initialize-with-hyphen'} == 'false') {
            $name['initials'] = preg_replace("/-/", '', $name['initials']);
          }
          // Within initials, add a space after a hyphen, but only if ...
          // ... the delimiter that separates initials ends with a space.
          if (preg_match("/ $/", $initialize_with)) {
            $name['initials'] = preg_replace("/-(?=[$this->upper])/$this->patternModifiers", "- ", $name['initials']);
          }
          // then, separate initials with the specified delimiter:
          $name['initials'] = preg_replace("/([$this->upper])(?=[^$this->lower]+|$)/$this->patternModifiers", "\\1$initialize_with", $name['initials']);
          // $shortenInitials = (isset($options['numberOfInitialsToKeep'])) ? $options['numberOfInitialsToKeep'] : FALSE;
          //      if ($shortenInitials) $given = drupal_substr($given, 0, $shortenInitials);.
          if (isset($initialize_with)) {
            $name['firstname'] = $name['initials'];
            // If ($shortenInitials) $name['firstname'] = drupal_substr($name['firstname'], 0, $shortenInitials);.
          }
          elseif (!empty($name['firstname'])) {
            $name['firstname'] = $name['firstname'] . ' ' . $name['initials'];
          }
          elseif (empty($name['firstname'])) {
            $name['firstname'] = $name['initials'];
          }
        }
        $given = $this->format($name['firstname'], 'given');
        if (isset($name['lastname'])) {
          if (!empty($name['prefix'])) {
            $name['lastname'] = $name['prefix'] . ' ' . $name['lastname'];
          }
          if (!empty($name['suffix'])) {
            $name['lastname'] = $name['lastname'] . ', ' . $name['suffix'];
          }

          $name['lastname'] = $this->format($name['lastname'], 'family');
          if ($this->form == 'short') {
            $text = $name['lastname'];
          }
          else {
            switch ($this->{'name-as-sort-order'}) {
              case 'first' && $rank == 0:
              case 'all':
                $text = $name['lastname'] . $this->sort_separator . $given;
                break;

              default:
                $text = $given . ' ' . $name['lastname'];
            }
          }
          $text = $this->format($text);
          $name['name'] = $text;
          if (strstr($text, 'div') || strstr($text, 'span')) {
            $options = array('html' => TRUE);
          }
          else {
            $options = array('html' => FALSE);
          }
        }
      }
      if (variable_get('biblio_author_links', 1)) {
        $text = theme('biblio_author_link', array('author' => $name, 'options' => $options));
      }

      $authors[] = $text;

      if (isset($this->{'et-al-min'}) && $count >= $this->{'et-al-min'}) {
        break;
      }
    }
    if (isset($this->{'et-al-min'}) &&
      $count >= $this->{'et-al-min'} &&
      isset($this->{'et-al-use-first'}) &&
      $count >= $this->{'et-al-use-first'} &&
      count($names) > $this->{'et-al-use-first'}) {
      if ($this->{'et-al-use-first'} < $this->{'et-al-min'}) {
        for ($i = $this->{'et-al-use-first'}; $i < $count; $i++) {
          unset($authors[$i]);
        }
      }
      if ($this->etal) {
        $etal = $this->etal->render();
      }
      else {
        $etal = $this->citeproc->get_locale('term', 'et-al');
      }
      $et_al_triggered = TRUE;
    }

    if (!empty($authors) && !$et_al_triggered) {
      $auth_count = count($authors);
      if (isset($this->and) && $auth_count > 1) {
        // Stick an "and" in front of the last author if "and" is defined.
        $authors[$auth_count - 1] = $this->and . ' ' . $authors[$auth_count - 1];
      }
    }

    $text = implode($this->delimiter, $authors);

    if (!empty($authors) && $et_al_triggered) {
      switch ($this->{'delimiter-precedes-et-al'}) {
        case 'never':
          $text = $text . " $etal";
          break;

        case 'always':
          $text = $text . "$this->delimiter$etal";
          break;

        default:
          $text = count($authors) == 1 ? $text . " $etal" : $text . "$this->delimiter$etal";
      }
    }

    if ($this->form == 'count') {
      if (!$et_al_triggered) {
        return (int) count($authors);
      }
      else {
        return (int) (count($authors) - 1);
      }
    }
    // Strip out the last delimiter if not required.
    if (isset($this->and) && $auth_count > 1) {
      $last_delim = strrpos($text, $this->delimiter . $this->and);
      // Dpl == delimiter proceeds last.
      switch ($this->dpl) {
        case 'always':
          return $text;

        break;
        case 'never':
          return substr_replace($text, ' ', $last_delim, strlen($this->delimiter));

        break;
        case 'contextual':
        default:
          if ($auth_count < 3) {
            return substr_replace($text, ' ', $last_delim, strlen($this->delimiter));
          }
      }
    }
    return $text;
  }

  /**
   *
   */
  public function get_regex_patterns() {
    // Checks if PCRE is compiled with UTF-8 and Unicode support.
    if (!@preg_match('/\pL/u', 'a')) {
      // Probably a broken PCRE library.
      return $this->get_latin1_regex();
    }
    else {
      // Unicode safe filter for the value.
      return $this->get_utf8_regex();
    }
  }

  /**
   *
   */
  public function get_latin1_regex() {
    $alnum = "[:alnum:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 letters:
    $alpha = "[:alpha:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 control characters:
    $cntrl = "[:cntrl:]";
    // Matches ISO-8859-1 dashes & hyphens:
    $dash = "-–";
    // Matches ISO-8859-1 digits:
    $digit = "[\d]";
    // Matches ISO-8859-1 printing characters (excluding space):
    $graph = "[:graph:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 lower case letters:
    $lower = "[:lower:]äåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 printing characters (including space):
    $print = "[:print:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Matches ISO-8859-1 punctuation:
    $punct = "[:punct:]";
    // Matches ISO-8859-1 whitespace (separating characters with no visual representation):
    $space = "[\s]";
    // Matches ISO-8859-1 upper case letters:
    $upper = "[:upper:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆ";
    // Matches ISO-8859-1 "word" characters:
    $word = "_[:alnum:]ÄÅÁÀÂÃÇÉÈÊËÑÖØÓÒÔÕÜÚÙÛÍÌÎÏÆäåáàâãçéèêëñöøóòôõüúùûíìîïæÿß";
    // Defines the PCRE pattern modifier(s) to be used in conjunction with the above variables:
    // More info: <http://www.php.net/manual/en/reference.pcre.pattern.modifiers.php>
    $patternModifiers = "";

    return array($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
      $print, $punct, $space, $upper, $word, $patternModifiers,
    );

  }

  /**
   *
   */
  public function get_utf8_regex() {
    // Matches Unicode letters & digits:
    // Unicode-aware equivalent of "[:alnum:]".
    $alnum = "\p{Ll}\p{Lu}\p{Lt}\p{Lo}\p{Nd}";
    // Matches Unicode letters:
    // Unicode-aware equivalent of "[:alpha:]".
    $alpha = "\p{Ll}\p{Lu}\p{Lt}\p{Lo}";
    // Matches Unicode control codes & characters not in other categories:
    // Unicode-aware equivalent of "[:cntrl:]".
    $cntrl = "\p{C}";
    // Matches Unicode dashes & hyphens:
    $dash = "\p{Pd}";
    // Matches Unicode digits:
    // Unicode-aware equivalent of "[:digit:]".
    $digit = "\p{Nd}";
    // Matches Unicode printing characters (excluding space):
    // Unicode-aware equivalent of "[:graph:]".
    $graph = "^\p{C}\t\n\f\r\p{Z}";
    // Matches Unicode lower case letters:
    // Unicode-aware equivalent of "[:lower:]".
    $lower = "\p{Ll}\p{M}";
    // Matches Unicode printing characters (including space):
    // same as "^\p{C}", Unicode-aware equivalent of "[:print:]".
    $print = "\P{C}";
    // Matches Unicode punctuation (printing characters excluding letters & digits):
    // Unicode-aware equivalent of "[:punct:]".
    $punct = "\p{P}";
    // Matches Unicode whitespace (separating characters with no visual representation):
    // Unicode-aware equivalent of "[:space:]".
    $space = "\t\n\f\r\p{Z}";
    // Matches Unicode upper case letters:
    // Unicode-aware equivalent of "[:upper:]".
    $upper = "\p{Lu}\p{Lt}";
    // Matches Unicode "word" characters:
    // Unicode-aware equivalent of "[:word:]" (or "[:alnum:]" plus "_")
    $word = "_\p{Ll}\p{Lu}\p{Lt}\p{Lo}\p{Nd}";
    // Defines the PCRE pattern modifier(s) to be used in conjunction with the above variables:
    // More info: <http://www.php.net/manual/en/reference.pcre.pattern.modifiers.php>
    // the "u" (PCRE_UTF8) pattern modifier causes PHP/PCRE to treat pattern strings as UTF-8.
    $patternModifiers = "u";
    return array($alnum, $alpha, $cntrl, $dash, $digit, $graph, $lower,
      $print, $punct, $space, $upper, $word, $patternModifiers,
    );
  }

}

/**
 *
 */
class csl_names extends csl_format {
  private $substitutes;

  /**
   *
   */
  public function init_formatting() {
    $this->span_class = 'biblio-authors';
    parent::init_formatting();

  }

  /**
   *
   */
  public function init($dom_node, $citeproc) {
    $etal = '';
    $tag = $dom_node->getElementsByTagName('substitute')->item(0);
    if ($tag) {
      $this->substitutes = csl_factory::create($tag, $citeproc);
      $dom_node->removeChild($tag);
    }

    $tag = $dom_node->getElementsByTagName('et-al')->item(0);
    if ($tag) {
      $etal = csl_factory::create($tag, $citeproc);
      $dom_node->removeChild($tag);
    }

    $var = $dom_node->getAttribute('variable');
    foreach ($dom_node->childNodes as $node) {
      if ($node->nodeType == 1) {
        $element = csl_factory::create($node, $citeproc);
        if (($element instanceof csl_label)) {
          $element->variable = $var;
        }
        if (($element instanceof csl_name) && $etal) {
          $element->etal = $etal;
        }
        $this->add_element($element);
      }
    }
  }

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $matches = array();
    $variable_parts = array();

    if (!isset($this->delimiter)) {
      $style_delimiter = $this->citeproc->style->{'names-delimiter'};
      $mode_delimiter = $this->citeproc->{$mode}->{'names-delimiter'};
      $this->delimiter = (isset($mode_delimiter)) ? $mode_delimiter : (isset($style_delimiter) ? $style_delimiter : '');
    }

    $variables = explode(' ', $this->variable);

    foreach ($variables as $var) {
      if (!$var || in_array($var, $this->citeproc->quash)) {
        continue;
      }
      list($contributor, $category) = explode(':', $var);
      if ((isset($data->{$contributor}) && !empty($data->{$contributor})) && $this->_get_category($data->{$contributor}, $category)) {
        $matches[] = $var;
      }
    }

    // We don't have any primary suspects, so lets check the substitutes...
    if (empty($matches)) {
      if (isset($this->substitutes)) {
        foreach ($this->substitutes->elements as $element) {
          // Test to see if any of the other names variables has content.
          if (($element instanceof csl_names)) {
            $sub_variables = explode(' ', $element->variable);
            foreach ($sub_variables as $var) {
              list($contributor, $category) = explode(':', $var);
              if ((isset($data->{$contributor}) && !empty($data->{$contributor})) && $this->_get_category($data->{$contributor}, $category)) {
                $matches[] = $var;
                $this->citeproc->quash[] = $var;
              }
            }
          }
          // If it's not a "names" element, just render it.
          else {
            $text = $element->render($data, $mode);
            $this->citeproc->quash[] = isset($element->variable) ? $element->variable : $element->var;
            if (!empty($text)) {
              $variable_parts[] = $text;
            }
          }
          if (!empty($matches)) {
            break;
          }
        }
      }
    }

    foreach ($matches as $var) {
      if (in_array($var, $this->citeproc->quash) && in_array($var, $variables)) {
        continue;
      }
      $text = '';
      list($contributor, $category) = explode(':', $var);
      if (!empty($contributor) && $authors = $this->_get_category($data->{$contributor}, $category)) {
        foreach ($this->elements as $element) {
          if (is_a($element, 'csl_label')) {
            $element->in_names = TRUE;
            $element->plural   = (count($authors) > 1) ? 'multiple' : 'single';
            $element->variable = $this->_get_csl_name_variable($category);
            $text             .= $element->render($authors, $mode);
          }
          elseif (is_a($element, 'csl_name')) {
            $text .= $element->render($authors, $mode);
          }
        }
      }
      if (!empty($text)) {
        $variable_parts[] = $text;
      }
    }

    if (!empty($variable_parts)) {
      $text = implode($this->delimiter, $variable_parts);
      return $this->format($text);
    }

    return;
  }

  /**
   *
   */
  private function _get_category($contributors, $category) {
    $authors = array();
    foreach ($contributors as $author) {
      if ($author['auth_category'] == $category) {
        $authors[] = $author;
      }
    }
    return count($authors) ? $authors : FALSE;
  }

  /**
   *
   */
  private function _get_csl_name_variable($category) {
    switch ($category) {
      case 1:
        return 'author';

      break;
      case 2:
        return 'editor';

      break;
      case 3:
        return 'translator';

      break;
      default:
    }
  }

}

/**
 *
 */
class csl_date extends csl_format {

  /**
   *
   */
  public function init($dom_node, $citeproc) {
    $locale_elements = array();

    if ($form = $this->form) {
      $local_date = $this->citeproc->get_locale('date_options', $form);
      $dom_elem = dom_import_simplexml($local_date[0]);
      if ($dom_elem) {
        foreach ($dom_elem->childNodes as $node) {
          if ($node->nodeType == 1) {
            $locale_elements[] = csl_factory::create($node, $citeproc);
          }
        }
      }
      foreach ($dom_node->childNodes as $node) {
        if ($node->nodeType == 1) {
          $element = csl_factory::create($node, $citeproc);

          foreach ($locale_elements as $key => $locale_element) {
            if ($locale_element->name == $element->name) {
              $locale_elements[$key]->attributes = array_merge($locale_element->attributes, $element->attributes);
              $locale_elements[$key]->format = $element->format;
              break;
            }

            else {
              $locale_elements[] = $element;
            }
          }
        }
      }
      if ($date_parts = $this->{'date-parts'}) {
        $parts = explode('-', $date_parts);
        foreach ($locale_elements as $key => $element) {
          if (array_search($element->name, $parts) === FALSE) {
            unset($locale_elements[$key]);
          }
        }
        if (count($locale_elements) != count($parts)) {
          foreach ($parts as $part) {
            $element = new csl_date_part();
            $element->name = $part;
            $locale_elements[] = $element;
          }
        }
        // Now re-order the elements.
        foreach ($parts as $part) {
          foreach ($locale_elements as $key => $element) {
            if ($element->name == $part) {
              $this->elements[] = $element;
              unset($locale_elements[$key]);
            }
          }
        }

      }
      else {
        $this->elements = $locale_elements;
      }
    }
    else {
      parent::init($dom_node, $citeproc);
    }

  }

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $date_parts = array();
    $text = '';

    if (($var = $this->variable) && isset($data->{$var})) {
      if (is_array($data->{$var})) {
        $date = $data->{$var};
      }
      else {
        $date = array($data->{$var});
      }
      foreach ($this->elements as $element) {
        $date_parts[] = $element->render($date, $mode);
      }
      $text = implode($this->delimiter, $date_parts);
    }
    // @code
    // else {
    //   $text = $this->citeproc->get_locale('term', 'no date');
    // }
    // @endcode
    return $this->format($text);
  }

}

/**
 *
 */
class csl_date_part extends csl_format {

  /**
   *
   */
  public function render($date, $mode = NULL) {
    $text = '';

    switch ($this->name) {
      case 'year':
        $text = (isset($date[0])) ? $date[0] : '';
        if ($text > 0 && $text < 500) {
          $text = $text . $this->citeproc->get_locale('term', 'ad');
        }
        elseif ($text < 0) {
          $text = $text * -1;
          $text = $text . $this->citeproc->get_locale('term', 'bc');
        }
        // Return ((isset($this->prefix))? $this->prefix : '') . $date[0] . ((isset($this->suffix))? $this->suffix : '');.
        break;

      case 'month':
        $text = (isset($date[1])) ? $date[1] : '';
        if (empty($text) || $text < 1 || $text > 12) {
          return;
        }
        // $form = $this->form;.
        switch ($this->form) {
          case 'numeric':
            break;

          case 'numeric-leading-zeros':
            if ($text < 10) {
              $text = '0' . $text;
              break;
            }
            break;

          case 'short':
            $month = 'month-' . sprintf('%02d', $text);
            $text = $this->citeproc->get_locale('term', $month, 'short');
            break;

          default:
            $month = 'month-' . sprintf('%02d', $text);
            $text = $this->citeproc->get_locale('term', $month);
            break;
        }
        break;

      case 'day':
        $text = (isset($date[2])) ? $date[2] : '';
        break;
    }

    return $this->format($text);
  }

}

/**
 *
 */
class csl_number extends csl_format {

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $var = $this->variable;

    if (!$var || empty($data->$var)) {
      return;
    }

    // $form = $this->form;.
    switch ($this->form) {
      case 'ordinal':
        $text = $this->ordinal($data->$var);
        break;

      case 'long-ordinal':
        $text = $this->long_ordinal($data->$var);
        break;

      case 'roman':
        $text = $this->roman($data->$var);
        break;

      case 'numeric':
      default:
        $text = $data->$var;
        break;
    }
    return $this->format($text);
  }

  /**
   *
   */
  public function ordinal($num) {
    if (($num / 10) % 10 == 1) {
      $num .= $this->citeproc->get_locale('term', 'ordinal-04');
    }
    elseif ($num % 10 == 1) {
      $num .= $this->citeproc->get_locale('term', 'ordinal-01');
    }
    elseif ($num % 10 == 2) {
      $num .= $this->citeproc->get_locale('term', 'ordinal-02');
    }
    elseif ($num % 10 == 3) {
      $num .= $this->citeproc->get_locale('term', 'ordinal-03');
    }
    else {
      $num .= $this->citeproc->get_locale('term', 'ordinal-04');
    }
    return $num;

  }

  /**
   *
   */
  public function long_ordinal($num) {
    $num = sprintf("%02d", $num);
    $ret = $this->citeproc->get_locale('term', 'long-ordinal-' . $num);
    if (!$ret) {
      return $this->ordinal($num);
    }
    return $ret;
  }

  /**
   *
   */
  public function roman($num) {
    $ret = "";
    if ($num < 6000) {
      $ROMAN_NUMERALS = array(
      array("", "i", "ii", "iii", "iv", "v", "vi", "vii", "viii", "ix"),
      array("", "x", "xx", "xxx", "xl", "l", "lx", "lxx", "lxxx", "xc"),
      array("", "c", "cc", "ccc", "cd", "d", "dc", "dcc", "dccc", "cm"),
      array("", "m", "mm", "mmm", "mmmm", "mmmmm"),
      );
      $numstr = strrev($num);
      $len = strlen($numstr);
      for ($pos = 0; $pos < $len; $pos++) {
        $n = $numstr[$pos];
        $ret = $ROMAN_NUMERALS[$pos][$n] . $ret;
      }
    }

    return $ret;
  }

}

/**
 *
 */
class csl_text extends csl_format {
  public $source;
  protected $var;

  /**
   *
   */
  public function init($dom_node, $citeproc) {
    foreach (array('variable', 'macro', 'term', 'value') as $attr) {
      if ($dom_node->hasAttribute($attr)) {
        $this->source = $attr;
        if ($this->source == 'macro') {
          $this->var = str_replace(' ', '_', $dom_node->getAttribute($attr));
        }
        else {
          $this->var = $dom_node->getAttribute($attr);
        }
      }
    }
  }

  /**
   *
   */
  public function init_formatting() {
    if ($this->variable == 'title') {
      $this->span_class = 'biblio-title';
    }
    parent::init_formatting();

  }

  /**
   *
   */
  public function render($data = NULL, $mode = NULL) {
    $text = '';
    if (in_array($this->var, $this->citeproc->quash)) {
      return;
    }

    switch ($this->source) {
      case 'variable':
        if (!isset($data->{$this->variable}) || empty($data->{$this->variable}) || trim($data->{$this->variable}) == FALSE) {
          return;
        }
        if ($this->variable == 'biblio_url') {
          $text = l($data->{$this->variable}, $data->{$this->variable});
        }
        else {
          $text = $data->{$this->variable}; //$this->data[$this->var];  // include the contents of a variable
        }
        break;

      case 'macro':
        $macro = $this->var;
        // Trigger the macro process.
        $text = $this->citeproc->render_macro($macro, $data, $mode);
        break;

      case 'term':
        $form = (($form = $this->form)) ? $form : '';
        $text = $this->citeproc->get_locale('term', $this->var, $form);
        break;

      case 'value':
        // $this->var;  // dump the text verbatim.
        $text = $this->var;
        break;
    }

    if (empty($text)) {
      return;
    }
    $text = $this->format($text);
    if ($this->variable == 'title') {
      $url = biblio_get_title_url_info($data);
      $text = l($text, $url['link'], $url['options']);
    }
    return $text;
  }

}

/**
 *
 */
class csl_et_al extends csl_text {

  /**
   *
   */
  public function __construct($dom_node = NULL, $citeproc = NULL) {
    $this->var = 'et-al';
    $this->source = 'term';
    parent::__construct($dom_node, $citeproc);

  }

}

/**
 *
 */
class csl_label extends csl_format {
  private $pluralization;

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $text = '';
    $pluralization = NULL;
    $form = (($form = $this->form)) ? $form : 'long';

    // There are two possible locations for a label the first is within a names element which is a bit simpler than the general label case
    // within csl_names element case.
    if (isset($this->in_names)) {
      if (($term = $this->citeproc->get_locale('term', $this->variable, $form, $this->plural))) {
        $text = $term;
      }
    }
    // This is the general label case where we may need to determine if the variable is plural.
    else {
      $variables = explode(' ', $this->variable);
      switch ($this->plural) {
        case 'never':
          $pluralization = 'single';
          break;

        case 'always':
          $pluralization = 'multiple';
          break;

        case 'contextual':
        default:
      }
      foreach ($variables as $variable) {
        $field = $this->citeproc->map_field($variable);
        if (isset($data->{$field}) && !empty($data->{$field})) {
          if (!isset($this->plural) && empty($pluralization) && is_array($data->{$field})) {
            $count = count($data->{$field});
            if ($count == 1) {
              $pluralization = 'single';
            }
            elseif ($count > 1) {
              $pluralization = 'multiple';
            }
          }
          else {
            $pluralization = $this->evaluateStringPluralism($data, $variable);
          }
          if (($term = $this->citeproc->get_locale('term', $variable, $form, $pluralization))) {
            $text = $term;
            break;
          }
        }
      }
    }

    if (empty($text)) {
      return;
    }

    if ($this->{'strip-periods'}) {
      $text = str_replace('.', '', $text);
    }

    return $this->format($text);
  }

  /**
   *
   */
  public function evaluateStringPluralism($data, $variable) {
    $field = $this->citeproc->map_field($variable);
    $str = $data->{$field};
    $plural = 'single';

    if (!empty($str)) {
      // $regex = '/(?:[0-9],\s*[0-9]|\s+and\s+|&|([0-9]+)\s*[\-\x2013]\s*([0-9]+))/';.
      switch ($variable) {
        case 'page':
          $page_regex = "/([a-zA-Z]*)([0-9]+)\s*(?:–|-)\s*([a-zA-Z]*)([0-9]+)/";
          $err = preg_match($page_regex, $str, $m);
          if ($err !== FALSE && count($m) == 0) {
            $plural = 'single';
          }
          elseif ($err !== FALSE && count($m)) {
            $plural = 'multiple';
          }
          break;

        default:
      }
    }
    return $plural;
  }

}

/**
 *
 */
class csl_macro extends csl_format {

}

/**
 *
 */
class csl_macros extends csl_collection {

  /**
   *
   */
  public function __construct($macro_nodes, $citeproc) {
    foreach ($macro_nodes as $macro) {
      $macro = csl_factory::create($macro, $citeproc);
      $this->elements[$macro->name()] = $macro;
    }
  }

  /**
   *
   */
  public function render_macro($name, $data, $mode) {
    return $this->elements[$name]->render($data, $mode);
  }

}

/**
 *
 */
class csl_group extends csl_format {

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $text = '';
    $text_parts = array();

    $terms = $variables = $have_variables = $element_count = 0;
    foreach ($this->elements as $element) {
      $element_count++;
      if (($element instanceof csl_text) &&
          ($element->source == 'term' ||
           $element->source == 'value')) {
        $terms++;
      }
      if (($element instanceof csl_label)) {
        $terms++;
      }
      if ($element->source == 'variable' &&
          isset($element->variable) &&
          !empty($data->{$element->variable})
         ) {
        $variables++;
      }

      $text = $element->render($data, $mode);

      $delimiter = $this->delimiter;
      if (!empty($text)) {
        if ($delimiter && ($element_count < count($this->elements))) {
          // Check to see if the delimiter is already the last character of the text string
          // if so, remove it so we don't have two of them when we paste together the group.
          $stext = strip_tags(trim($text));
          if ((strrpos($stext, $delimiter[0]) + 1) == drupal_strlen($stext) && drupal_strlen($stext) > 1) {
            $text = str_replace($stext, '----REPLACE----', $text);
            $stext = drupal_substr($stext, 0, -1);
            $text = str_replace('----REPLACE----', $stext, $text);
          }
        }
        $text_parts[] = $text;
        if ($element->source == 'variable' || isset($element->variable)) {
          $have_variables++;
        }
        if ($element->source == 'macro') {
          $have_variables++;
        }
      }
    }

    if (empty($text_parts)) {
      return;
    }
    if ($variables  && !$have_variables) {
      // There has to be at least one other none empty value before the term is output.
      return;
    }
    if (count($text_parts) == $terms) {
      // There has to be at least one other none empty value before the term is output.
      return;
    }

    $delimiter = $this->delimiter;
    // Insert the delimiter if supplied.
    $text = implode($delimiter, $text_parts);

    return $this->format($text);
  }

}

/**
 *
 */
class csl_layout extends csl_format {

  /**
   *
   */
  public function init_formatting() {
    // $this->div_class = 'csl-entry';.
    parent::init_formatting();
  }

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $text = '';
    $parts = array();
    // $delimiter = $this->delimiter;.
    foreach ($this->elements as $element) {
      $parts[] = $element->render($data, $mode);
    }

    $text = implode($this->delimiter, $parts);

    if ($mode == 'bibliography') {
      return $this->format($text);
    }
    else {
      return $text;
    }

  }

}

/**
 *
 */
class csl_citation extends csl_format {
  private $layout = NULL;

  /**
   *
   */
  public function init($dom_node, $citeproc) {
    $options = $dom_node->getElementsByTagName('option');
    foreach ($options as $option) {
      $value = $option->getAttribute('value');
      $name = $option->getAttribute('name');
      $this->attributes[$name] = $value;
    }

    $layouts = $dom_node->getElementsByTagName('layout');
    foreach ($layouts as $layout) {
      $this->layout = new csl_layout($layout, $citeproc);
    }
  }

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $this->citeproc->quash = array();

    $text = $this->layout->render($data, 'citation');

    return $this->format($text);
  }

}

/**
 *
 */
class csl_bibliography extends csl_format {
  private $layout = NULL;

  /**
   *
   */
  public function init($dom_node, $citeproc) {
    $hier_name_attr = $this->get_hier_attributes();
    $options = $dom_node->getElementsByTagName('option');
    foreach ($options as $option) {
      $value = $option->getAttribute('value');
      $name = $option->getAttribute('name');
      $this->attributes[$name] = $value;
    }

    $layouts = $dom_node->getElementsByTagName('layout');
    foreach ($layouts as $layout) {
      $this->layout = new csl_layout($layout, $citeproc);
    }

  }

  /**
   *
   */
  public function init_formatting() {
    // $this->div_class = 'csl-bib-body';.
    parent::init_formatting();
  }

  /**
   *
   */
  public function render($data, $mode = NULL) {
    $this->citeproc->quash = array();
    $text = $this->layout->render($data, 'bibliography');
    if ($this->{'hanging-indent'} == 'true') {
      $text = '<div style="  text-indent: -25px; padding-left: 25px;">' . $text . '</div>';
    }
    $text = str_replace('?.', '?', str_replace('..', '.', $text));
    return $this->format($text);
  }

}

/**
 *
 */
class csl_option {
  private $name;
  private $value;

  /**
   *
   */
  public function get() {
    return array($this->name => $this->value);
  }

}

/**
 *
 */
class csl_options extends csl_element {

}

/**
 *
 */
class csl_sort extends csl_element {

}

/**
 *
 */
class csl_style extends csl_element {

  /**
   *
   */
  public function __construct($dom_node = NULL, $citeproc = NULL) {
    if ($dom_node) {
      $this->set_attributes($dom_node);
    }
  }

}

/**
 *
 */
class csl_choose extends csl_element {

  /**
   *
   */
  public function render($data, $mode = NULL) {
    foreach ($this->elements as $choice) {
      if ($choice->evaluate($data)) {
        return $choice->render($data, $mode);
      }
    }
  }

}

/**
 *
 */
class csl_if extends csl_rendering_element {

  /**
   *
   */
  public function evaluate($data) {
    $match = (($match = $this->match)) ? $match : 'all';
    if (($types = $this->type)) {
      $types = explode(' ', $types);
      $matches = 0;
      foreach ($types as $type) {
        if (isset($data->biblio_type)) {
          if ($data->biblio_type == $type && $match == 'any') {
            return TRUE;
          }
          if ($data->biblio_type != $type && $match == 'all') {
            return FALSE;
          }
          if ($data->biblio_type == $type) {
            $matches++;
          }
        }
      }
      if ($match == 'all' && $matches == count($types)) {
        return TRUE;
      }
      if ($match == 'none' && $matches == 0) {
        return TRUE;
      }
      return FALSE;
    }
    if (($variables = $this->variable)) {
      $variables = explode(' ', $variables);
      $matches = 0;
      foreach ($variables as $var) {
        if (isset($data->$var) && !empty($data->$var) && $match == 'any') {
          return TRUE;
        }
        if ((!isset($data->$var) || empty($data->$var)) && $match == 'all') {
          return FALSE;
        }
        if (isset($data->$var) && !empty($data->$var)) {
          $matches++;
        }
      }
      if ($match == 'all' && $matches == count($variables)) {
        return TRUE;
      }
      if ($match == 'none' && $matches == 0) {
        return TRUE;
      }
      return FALSE;
    }
    if (($is_numeric = $this->{'is-numeric'})) {
      $variables = explode(' ', $is_numeric);
      $matches = 0;
      foreach ($variables as $var) {
        if (isset($data->$var)) {
          if (is_numeric($data->$var) && $match == 'any') {
            return TRUE;
          }
          if (!is_numeric($data->$var)) {
            if (preg_match('/(?:^\d+|\d+$)/', $data->$var)) {
              $matches++;
            }
            elseif ($match == 'all') {
              return FALSE;
            }
          }
          if (is_numeric($data->$var)) {
            $matches++;
          }
        }
      }
      if ($match == 'all' && $matches == count($variables)) {
        return TRUE;
      }
      if ($match == 'none' && $matches == 0) {
        return TRUE;
      }
      return FALSE;
    }
    if (isset($this->locator)) {
      $test = explode(' ', $this->type);
    }

    return FALSE;
  }

}

/**
 *
 */
class csl_else_if extends csl_if {

}

/**
 *
 */
class csl_else extends csl_if {

  /**
   *
   */
  public function evaluate($data = NULL) {
    // The last else always returns TRUE.
    return TRUE;
  }

}

/**
 *
 */
class csl_substitute extends csl_element {

}

/**
 *
 */
class csl_locale {
  protected $locale_xmlstring = NULL;
  protected $style_locale_xmlstring = NULL;
  protected $locale = NULL;
  protected $style_locale = NULL;
  private   $module_path;

  /**
   *
   */
  public function __construct($lang = 'en') {
    $this->module_path = drupal_get_path('module', 'biblio_citeproc');
    $this->locale = new SimpleXMLElement($this->get_locales_file_name($lang));
    if ($this->locale) {
      $this->locale->registerXPathNamespace('cs', 'http://purl.org/net/xbiblio/csl');
    }
  }

  /**
   * SimpleXML objects cannot be serialized, so we must convert to an XML string prior to serialization.
   */
  public function __sleep() {
    $this->locale_xmlstring       = ($this->locale) ? $this->locale->asXML() : '';
    $this->style_locale_xmlstring = ($this->style_locale) ? $this->style_locale->asXML() : '';
    return array('locale_xmlstring', 'style_locale_xmlstring');
  }

  /**
   * SimpleXML objects cannot be serialized, so when un-serializing them, they must rebuild from the serialized XML string.
   */
  public function __wakeup() {
    $this->style_locale = (!empty($this->style_locale_xmlstring)) ? new SimpleXMLElement($this->style_locale_xmlstring) : NULL;
    $this->locale       = (!empty($this->locale_xmlstring)) ? new SimpleXMLElement($this->locale_xmlstring) : NULL;
    if ($this->locale) {
      $this->locale->registerXPathNamespace('cs', 'http://purl.org/net/xbiblio/csl');
    }
  }

  /**
   *
   */
  public function get_locales_file_name($lang) {
    $lang_bases = array(
      "af" => "af-ZA",
      "ar" => "ar-AR",
      "bg" => "bg-BG",
      "ca" => "ca-AD",
      "cs" => "cs-CZ",
      "da" => "da-DK",
      "de" => "de-DE",
      "el" => "el-GR",
      "en" => "en-US",
      "es" => "es-ES",
      "et" => "et-EE",
      "fa" => "fa-IR",
      "fi" => "fi-FI",
      "fr" => "fr-FR",
      "he" => "he-IL",
      "hu" => "hu-HU",
      "is" => "is-IS",
      "it" => "it-IT",
      "ja" => "ja-JP",
      "km" => "km-KH",
      "ko" => "ko-KR",
      "mn" => "mn-MN",
      "nb" => "nb-NO",
      "nl" => "nl-NL",
      "nn" => "nn-NO",
      "pl" => "pl-PL",
      "pt" => "pt-PT",
      "ro" => "ro-RO",
      "ru" => "ru-RU",
      "sk" => "sk-SK",
      "sl" => "sl-SI",
      "sr" => "sr-RS",
      "sv" => "sv-SE",
      "th" => "th-TH",
      "tr" => "tr-TR",
      "uk" => "uk-UA",
      "vi" => "vi-VN",
      "zh" => "zh-CN",
    );
    return (isset($lang_bases[$lang])) ? file_get_contents($this->module_path . '/locale/locales-' . $lang_bases[$lang] . '.xml') : file_get_contents($this->module_path . '/locale/locales-en-US.xml');
  }

  /**
   *
   */
  public function get_locale($type, $arg1, $arg2 = NULL, $arg3 = NULL) {
    switch ($type) {
      case 'term':
        $term = '';
        $form = $arg2 ? " and @form='$arg2'" : '';
        $plural = $arg3 ? "/cs:$arg3" : '';
        if ($arg2 == 'verb' || $arg2 == 'verb-short') {
          $plural = '';
        }
        if ($this->style_locale) {
          $term = @$this->style_locale->xpath("//locale[@xml:lang='en']/terms/term[@name='$arg1'$form]$plural");
          if (!$term) {
            $term = @$this->style_locale->xpath("//locale/terms/term[@name='$arg1'$form]$plural");
          }
        }
        if (!$term) {
          $term = $this->locale->xpath("//cs:term[@name='$arg1'$form]$plural");
        }
        if (isset($term[0])) {
          if (isset($arg3) && isset($term[0]->{$arg3})) {
            return (string) $term[0]->{$arg3};
          }
          if (!isset($arg3) && isset($term[0]->single)) {
            return (string) $term[0]->single;
          }
          return (string) $term[0];
        }
        break;

      case 'date_option':
        $attribs = array();
        if ($this->style_locale) {
          $date_part = $this->style_locale->xpath("//date[@form='$arg1']/date-part[@name='$arg2']");
        }
        if (!isset($date_part)) {
          $date_part = $this->locale->xpath("//cs:date[@form='$arg1']/cs:date-part[@name='$arg2']");
        }
        if (isset($date_part)) {
          foreach ($$date_part->attributes() as $name => $value) {
            $attribs[$name] = (string) $value;
          }
        }
        return $attribs;

      break;
      case 'date_options':
        $options = array();
        if ($this->style_locale) {
          $options = $this->style_locale->xpath("//locale[@xml:lang='en']/date[@form='$arg1']");
          if (!$options) {
            $options = $this->style_locale->xpath("//locale/date[@form='$arg1']");
          }
        }
        if (!$options) {
          $options = $this->locale->xpath("//cs:date[@form='$arg1']");
        }
        if (isset($options[0])) {
          return $options[0];
        }
        break;

      case 'style_option':
        $attribs = array();
        if ($this->style_locale) {
          $option = $this->style_locale->xpath("//locale[@xml:lang='en']/style-options[@$arg1]");
          if (!$option) {
            $option = $this->style_locale->xpath("//locale/style-options[@$arg1]");
          }
        }
        if (isset($option) && !empty($option)) {
          $attribs = $option[0]->attributes();
        }
        if (empty($attribs)) {
          $option = $this->locale->xpath("//cs:style-options[@$arg1]");
        }
        foreach ($option[0]->attributes() as $name => $value) {
          if ($name == $arg1) {
            return (string) $value;
          }
        }
        break;
    }
  }

  /**
   *
   */
  public function set_style_locale($csl_doc) {
    $xml = '';
    $locale_nodes = $csl_doc->getElementsByTagName('locale');
    if ($locale_nodes) {
      $xml_open = '<style-locale>';
      $xml_close = '</style-locale>';
      foreach ($locale_nodes as $key => $locale_node) {
        $xml .= $csl_doc->saveXML($locale_node);
      }
      if (!empty($xml)) {
        $this->style_locale = new SimpleXMLElement($xml_open . $xml . $xml_close);
      }
    }
  }

}

/**
 *
 */
class csl_mapper {

  /**
   *
   */
  public function map_field($field) {
    if (!isset($this->field_map)) {
      $this->field_map = biblio_get_map('field_map', 'csl');
    }

    $vars = explode(' ', $field);
    foreach ($vars as $key => $value) {
      $vars[$key] = (!empty($this->field_map[$value])) ? $this->field_map[$value] : '';
    }

    return implode(' ', $vars);
  }

  /**
   *
   */
  public function map_type($types) {
    if (!isset($this->type_map)) {
      $this->type_map = biblio_get_map('type_map', 'csl');
    }
    $vars = explode(' ', $types);
    foreach ($vars as $key => $value) {
      $vars[$key] = (!empty($this->type_map[$value])) ? $this->type_map[$value] : '';
    }

    return implode(' ', $vars);

  }

}
