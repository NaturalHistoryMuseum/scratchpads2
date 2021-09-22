<?php

/**
 *
 */
class rtf {

  /**
   * {\colortbl;\red 0\green 0\blue 0;\red 255\green 0\ blue0;\red0 ...}.
   */
  public $colour_table = array();
  public $colour_rgb;
  /**
   * {\fonttbl{\f0}{\f1}{f...}}.
   */
  public $font_table = array();
  public $fonts_array = array();
  public $font_face;
  public $font_size;
  /**
   * {\info {\title <title>} {\author <author>} {\operator <operator>}}.
   */
  public $info_table = array();
  public $page_width;
  public $page_height;
  public $page_size;
  public $page_orientation;
  public $rtf_version;
  public $tab_width;

  public $document;
  public $buffer;
  public $inch;
  public $cm;
  public $mm;

  /**
   *
   */
  public function __construct() {
    $this->inch = 1440;
    $this->cm   = 567;
    $this->mm   = 56.7;

    $this->fonts_array[] = array(
      "name"    => "Arial",
      "family"  => "swiss",
      "charset" => 0,
    );

    $this->fonts_array[] = array(
      "name"    => "Times New Roman",
      "family"  => "roman",
      "charset" => 0,
    );

    $this->fonts_array[] = array(
      "name"    => "Verdana",
      "family"  => "swiss",
      "charset" => 0,
    );

    $this->fonts_array[] = array(
      "name"    => "Symbol",
      "family"  => "roman",
      "charset" => 2,
    );

    $this->setDefaultFontFace(0);
    $this->setDefaultFontSize(24);
    $this->setPaperSize(5);
    $this->setPaperOrientation(1);
    $this->rtf_version = 1;
    $this->tab_width = 360;
  }

  /**
   *
   */
  public function setDefaultFontFace($face) {
    // $font is interger.
    $this->font_face = $face;
  }

  /**
   *
   */
  public function setDefaultFontSize($size) {
    $this->font_size = $size;
  }

  /**
   *
   */
  public function setTitle($title = "") {
    $this->info_table["title"] = $title;
  }

  /**
   *
   */
  public function setAuthor($author = "") {
    $this->info_table["author"] = $author;
  }

  /**
   *
   */
  public function setOperator($operator = "") {
    $this->info_table["operator"] = $operator;
  }

  /**
   *
   */
  public function setPaperSize($size = 0) {

    // 1 => Letter (8.5 x 11 inch)
    // 2 => Legal (8.5 x 14 inch)
    // 3 => Executive (7.25 x 10.5 inch)
    // 4 => A3 (297 x 420 mm)
    // 5 => A4 (210 x 297 mm)
    // 6 => A5 (148 x 210 mm)
    // Orientation considered as Portrait.
    switch ($size) {
      case 1:
        $this->page_width = floor(8.5 * $this->inch);
        $this->page_height = floor(11 * $this->inch);
        $this->page_size = 1;
        break;

      case 2:
        $this->page_width = floor(8.5 * $this->inch);
        $this->page_height = floor(14 * $this->inch);
        $this->page_size = 5;
        break;

      case 3:
        $this->page_width = floor(7.25 * $this->inch);
        $this->page_height = floor(10.5 * $this->inch);
        $this->page_size = 7;
        break;

      case 4:
        $this->page_width = floor(297 * $this->mm);
        $this->page_height = floor(420 * $this->mm);
        $this->page_size = 8;
        break;

      case 5:
      default:
        $this->page_width = floor(210 * $this->mm);
        $this->page_height = floor(297 * $this->mm);
        $this->page_size = 9;
        break;

      case 6:
        $this->page_width = floor(148 * $this->mm);
        $this->page_height = floor(210 * $this->mm);
        $this->page_size = 10;
        break;
    }
  }

  /**
   *
   */
  public function setPaperOrientation($orientation = 0) {
    // 1 => Portrait
    // 2 => Landscape.
    switch ($orientation) {
      case 1:
      default:
        $this->page_orientation = 1;
        break;

      case 2:
        $this->page_orientation = 2;
        break;
    }
  }

  /**
   *
   */
  public function addColour($hexcode) {
    // Get the RGB values.
    $this->hex2rgb($hexcode);

    // Register in the colour table array.
    $this->colour_table[] = array(
      "red"    => $this->colour_rgb["red"],
      "green"  => $this->colour_rgb["green"],
      "blue"   => $this->colour_rgb["blue"],
    );
  }

  /**
   * Convert HEX to RGB (#FFFFFF => r255 g255 b255)
   */
  public function hex2rgb($hexcode) {
    $hexcode = str_replace("#", "", $hexcode);
    $rgb = array();
    $rgb["red"] = hexdec(substr($hexcode, 0, 2));
    $rgb["green"] = hexdec(substr($hexcode, 2, 2));
    $rgb["blue"] = hexdec(substr($hexcode, 4, 2));

    $this->colour_rgb = $rgb;
  }

  /**
   * Convert newlines into \par.
   */
  public function nl2par($text) {
    $text = str_replace("\n", "\\par ", $text);

    return $text;
  }

  /**
   * Add a text string to the document buffer.
   */
  public function addText($text) {
    $text = str_replace("\n", "", $text);
    $text = str_replace("\t", "", $text);
    $text = str_replace("\r", "", $text);

    $this->document .= $text;
  }

  /**
   * Ouput the RTF file.
   */
  public function getDocument() {
    $this->buffer .= "{";
    // Header.
    $this->buffer .= $this->getHeader();
    // Font table.
    $this->buffer .= $this->getFontTable();
    // Colour table.
    $this->buffer .= $this->getColourTable();
    // File Information.
    $this->buffer .= $this->getInformation();
    // Default font values.
    $this->buffer .= $this->getDefaultFont();
    // Page display settings.
    $this->buffer .= $this->getPageSettings();
    // Parse the text into RTF.
    $this->buffer .= $this->parseDocument();
    $this->buffer .= "}";

    header("Content-Type: text/enriched\n");
    header("Content-Disposition: attachment; filename=rtf.rtf");
    echo $this->buffer;
  }

  /**
   * Header.
   */
  public function getHeader() {
    $header_buffer = "\\rtf{$this->rtf_version}\\ansi\\deff0\\deftab{$this->tab_width}\n\n";

    return $header_buffer;
  }

  /**
   * Font table.
   */
  public function getFontTable() {

    $font_buffer = "{\\fonttbl\n";
    foreach ($this->fonts_array as $fnum => $farray) {
      $font_buffer .= "{\\f{$fnum}\\f{$farray['family']}\\fcharset{$farray['charset']} {$farray['name']}}\n";
    }
    $font_buffer .= "}\n\n";

    return $font_buffer;
  }

  /**
   * Colour table.
   */
  public function getColourTable() {
    $colour_buffer = "";
    if (count($this->colour_table) > 0) {
      $colour_buffer = "{\\colortbl;\n";
      foreach ($this->colour_table as $cnum => $carray) {
        $colour_buffer .= "\\red{$carray['red']}\\green{$carray['green']}\\blue{$carray['blue']};\n";
      }
      $colour_buffer .= "}\n\n";
    }

    return $colour_buffer;
  }

  /**
   * Information.
   */
  public function getInformation() {
    $info_buffer = "";
    if (count($this->info_table) > 0) {
      $info_buffer = "{\\info\n";
      foreach ($this->info_table as $name => $value) {
        $info_buffer .= "{\\{$name} {$value}}";
      }
      $info_buffer .= "}\n\n";
    }

    return $info_buffer;
  }

  /**
   * Default font settings.
   */
  public function getDefaultFont() {
    $font_buffer = "\\f{$this->font_face}\\fs{$this->font_size}\n";

    return $font_buffer;
  }

  /**
   * Page display settings.
   */
  public function getPageSettings() {
    if ($this->page_orientation == 1) {
      $page_buffer = "\\paperw{$this->page_width}\\paperh{$this->page_height}\n";
    }
    else {
      $page_buffer = "\\paperw{$this->page_height}\\paperh{$this->page_width}\\landscape\n";
    }

    $page_buffer .= "\\pgncont\\pgndec\\pgnstarts1\\pgnrestart\n";

    return $page_buffer;
  }

  /**
   * Convert special characters to ASCII.
   */
  public function specialCharacters($text) {
    $text_buffer = "";
    for ($i = 0; $i < strlen($text); $i++) {
      $text_buffer .= $this->escapeCharacter($text[$i]);
    }

    return $text_buffer;
  }

  /**
   * Convert special characters to ASCII.
   */
  public function escapeCharacter($character) {
    $escaped = "";
    if (ord($character) >= 0x00 && ord($character) < 0x20) {
      $escaped = "\\'" . dechex(ord($character));
    }

    if ((ord($character) >= 0x20 && ord($character) < 0x80) || ord($character) == 0x09 || ord($character) == 0x0A) {
      $escaped = $character;
    }

    if (ord($character) >= 0x80 and ord($character) < 0xFF) {
      $escaped = "\\'" . dechex(ord($character));
    }

    switch (ord($character)) {
      case 0x5C:
      case 0x7B:
      case 0x7D:
        $escaped = "\\" . $character;
        break;
    }

    return $escaped;
  }

  /**
   * Parse the text input to RTF.
   */
  public function parseDocument() {
    // $doc_buffer = $this->specialCharacters(html_entity_decode($this->document));.
    $doc_buffer = html_entity_decode($this->document, ENT_QUOTES, "utf-8");
    $doc_buffer = utf8_decode($doc_buffer);
    $doc_buffer = $this->specialCharacters($doc_buffer);
    if (preg_match("/<ul>(.*?)<\/ul>/mi", $doc_buffer)) {
      $doc_buffer = str_replace("<ul>", "", $doc_buffer);
      $doc_buffer = str_replace("</ul>", "", $doc_buffer);
      $doc_buffer = preg_replace("/<li>(.*?)<\/li>/mi", "\\f3\\'B7\\tab\\f{$this->font_face} \\1\\par", $doc_buffer);
    }

    $doc_buffer = preg_replace("/<p>(.*?)<\/p>/mi", "\\1\\par ", $doc_buffer);
    $doc_buffer = preg_replace("/<strong>(.*?)<\/strong>/mi", "\\b \\1\\b0 ", $doc_buffer);
    $doc_buffer = preg_replace("/<em>(.*?)<\/em>/mi", "\\i \\1\\i0 ", $doc_buffer);
    $doc_buffer = preg_replace("/<i>(.*?)<\/i>/mi", "\\i \\1\\i0 ", $doc_buffer);
    $doc_buffer = preg_replace("/<u>(.*?)<\/u>/mi", "\\ul \\1\\ul0 ", $doc_buffer);
    $doc_buffer = preg_replace("/<strike>(.*?)<\/strike>/mi", "\\strike \\1\\strike0 ", $doc_buffer);
    $doc_buffer = preg_replace("/<sub>(.*?)<\/sub>/mi", "{\\sub \\1}", $doc_buffer);
    $doc_buffer = preg_replace("/<sup>(.*?)<\/sup>/mi", "{\\super \\1}", $doc_buffer);

    // $doc_buffer = preg_replace("/<H1>(.*?)<\/H1>/mi", "\\pard\\qc\\fs40 \\1\\par\\pard\\fs{$this->font_size} ", $doc_buffer);
    // $doc_buffer = preg_replace("/<H2>(.*?)<\/H2>/mi", "\\pard\\qc\\fs32 \\1\\par\\pard\\fs{$this->font_size} ", $doc_buffer);.
    $doc_buffer = preg_replace("/<h1>(.*?)<\/h1>/mi", "\\fs48\\b \\1\\b0\\fs{$this->font_size}\\par ", $doc_buffer);
    $doc_buffer = preg_replace("/<h2>(.*?)<\/h2>/mi", "\\fs36\\b \\1\\b0\\fs{$this->font_size}\\par ", $doc_buffer);
    $doc_buffer = preg_replace("/<h3>(.*?)<\/h3>/mi", "\\fs27\\b \\1\\b0\\fs{$this->font_size}\\par ", $doc_buffer);

    $doc_buffer = preg_replace("/<hr(.*?)>/i", "\\brdrb\\brdrs\\brdrw30\\brsp20 \\pard\\par ", $doc_buffer);
    $doc_buffer = str_replace("<br>", "\\par ", $doc_buffer);
    $doc_buffer = str_replace("<br>", "\\par ", $doc_buffer);
    $doc_buffer = str_replace("<tab>", "\\tab ", $doc_buffer);

    $doc_buffer = $this->nl2par($doc_buffer);

    return $doc_buffer;
  }

}
