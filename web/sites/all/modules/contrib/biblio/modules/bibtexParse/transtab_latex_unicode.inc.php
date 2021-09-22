<?php

/**
 * @file
 * Copyright:  Matthias Steffens
 *             This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.
 *             Please see the GNU General Public License for more details.
 * File:       ./includes/transtab_latex_unicode.inc.php
 * Created:    10-Aug-06, 23:55
 * Modified:   13-Aug-06, 16:16
 * This is a translation table for best-effort conversion from LaTeX to Unicode entities. It contains a comprehensive list of substitution strings for LaTeX characters,
 * which are used with the 'T1' font encoding. Uses commands from the 'textcomp' package. Unicode characters that can't be matched uniquely are commented out.
 * Adopted from 'transtab' by Markus Kuhn
 * (transtab.utf v1.8 2000-10-12 11:01:28+01 mgk25 Exp); see <http://www.cl.cam.ac.uk/~mgk25/unicode.html> for more info about Unicode and transtab.
 *
 * Modified:   2013-08-23 Christian Spitzlay:
 *             made sure closing braces match only if there are corresponding opening braces
 *             migrated most regular expressions to config arrays; regexps are generated on the fly now (more readable).
 *             added decoding of some missing greek symbols.
 */

/**
 * Creates a translation table for decoding of TeX symbols in BibTeX input.
 *
 * @return array An array with substitutions (regexps as keys, characters as values)
 *
 * @throws Exception Indicates that an internal check failed. Should only be thrown if someone broke a config array.
 */
function get_transtab_latex_unicode() {
  $transtab = array(
    '\\$\\\\#\\$' => '#',
    '\\\\%' => '%',
    '\\\\&' => '&',
    '(?<!\\\\)~' => ' ',
    '\\{\\\\c\\\\ \\}' => '¸',

    '--' => '–',
    '---' => '—',

    '\\$\\^\\{0\\}\\$' => '⁰',
    '\\$\\^\\{4\\}\\$' => '⁴',
    '\\$\\^\\{5\\}\\$' => '⁵',
    '\\$\\^\\{6\\}\\$' => '⁶',
    '\\$\\^\\{7\\}\\$' => '⁷',
    '\\$\\^\\{8\\}\\$' => '⁸',
    '\\$\\^\\{9\\}\\$' => '⁹',
    '\\$\\^\\{+\\}\\$' => '⁺',
    '\\$\\^\\{-\\}\\$' => '⁻',
    '\\$\\^\\{=\\}\\$' => '⁼',
    '\\$\\^\\{n\\}\\$' => 'ⁿ',
    '\\$_\\{0\\}\\$' => '₀',
    '\\$_\\{1\\}\\$' => '₁',
    '\\$_\\{2\\}\\$' => '₂',
    '\\$_\\{3\\}\\$' => '₃',
    '\\$_\\{4\\}\\$' => '₄',
    '\\$_\\{5\\}\\$' => '₅',
    '\\$_\\{6\\}\\$' => '₆',
    '\\$_\\{7\\}\\$' => '₇',
    '\\$_\\{8\\}\\$' => '₈',
    '\\$_\\{9\\}\\$' => '₉',
    '\\$_\\{+\\}\\$' => '₊',
    '\\$_\\{-\\}\\$' => '₋',
    '\\$_\\{=\\}\\$' => '₌',
  );

  // Map for diacritics that do *not* require whitespace in the absence of curly braces around the letter;
  // diacritic code and letter are separated by a pipe symbol.
  $quoty_mapping = array(
    '`|A' => 'À',
    '\'|A' => 'Á',
    '^|A' => 'Â',
    '~|A' => 'Ã',
    '"|A' => 'Ä',
    '`|E' => 'È',
    '\'|E' => 'É',
    '^|E' => 'Ê',
    '"|E' => 'Ë',
    '`|I' => 'Ì',
    '\'|I' => 'Í',
    '^|I' => 'Î',
    '"|I' => 'Ï',
    '~|N' => 'Ñ',
    '\'|N' => 'Ń',
    '\'|n' => 'ń',
    '`|O' => 'Ò',
    '\'|O' => 'Ó',
    '^|O' => 'Ô',
    '~|O' => 'Õ',
    '"|O' => 'Ö',
    '`|U' => 'Ù',
    '\'|U' => 'Ú',
    '^|U' => 'Û',
    '"|U' => 'Ü',
    '\'|Y' => 'Ý',
    '`|a' => 'à',
    '\'|a' => 'á',
    '^|a' => 'â',
    '~|a' => 'ã',
    '"|a' => 'ä',
    '`|e' => 'è',
    '\'|e' => 'é',
    '^|e' => 'ê',
    '"|e' => 'ë',
    '`|i' => 'ì',
    '\'|i' => 'í',
    '^|i' => 'î',
    '"|i' => 'ï',
    '"|\\i' => 'ï',
    '~|n' => 'ñ',
    '`|o' => 'ò',
    '\'|o' => 'ó',
    '^|o' => 'ô',
    '~|o' => 'õ',
    '"|o' => 'ö',
    '=|o' => 'ō',
    '`|u' => 'ù',
    '\'|u' => 'ú',
    '^|u' => 'û',
    '"|u' => 'ü',
    '\'|y' => 'ý',
    '"|y' => 'ÿ',
    '\'|C' => 'Ć',
    '\'|c' => 'ć',
    '.|g' => 'ġ',
    '.|I' => 'İ',
    '\'|\\i' => 'í',
    '\'|L' => 'Ĺ',
    '\'|l' => 'ĺ',
    '\'|R' => 'Ŕ',
    '\'|r' => 'ŕ',
    '\'|S' => 'Ś',
    '\'|s' => 'ś',
    '"|Y' => 'Ÿ',
    '\'|Z' => 'Ź',
    '\'|z' => 'ź',
    '.|Z' => 'Ż',
    '.|z' => 'ż',
  );
  foreach ($quoty_mapping as $sequence => $character) {
    // Split sequence at the pipe symbol.
    $key = explode('|', $sequence);
    if (count($key) != 2) {
      throw new Exception('Internal error: Invalid sequence ' . check_plain($sequence) . ' for character ' . check_plain($character));
    }
    $pattern = '(\\{)?\\\\' . preg_quote($key[0], '/') . '(\s*\\{)?' . preg_quote($key[1], '/') . '(?(2)\\}|)(?(1)\\}|)';
    $transtab[$pattern] = $character;
  }

  // Map for diacritics that *require* whitespace in the absence of curly braces around the letter;
  // diacritic code and letter are separated by a pipe symbol.
  $lettery_mapping = array(
    'v|L' => 'Ľ',
    'v|l' => 'ľ',
    'r|A' => 'Å',
    'c|C' => 'Ç',
    'r|a' => 'å',
    'c|c' => 'ç',
    'u|A' => 'Ă',
    'u|a' => 'ă',
    'k|A' => 'Ą',
    'k|a' => 'ą',
    'v|C' => 'Č',
    'v|c' => 'č',
    'v|D' => 'Ď',
    'v|d' => 'ď',
    'k|E' => 'Ę',
    'k|e' => 'ę',
    'v|E' => 'Ě',
    'v|e' => 'ě',
    'u|e' => 'ĕ',
    'u|G' => 'Ğ',
    'u|g' => 'ğ',
    'v|N' => 'Ň',
    'v|n' => 'ň',
    'H|O' => 'Ő',
    'H|o' => 'ő',
    'v|R' => 'Ř',
    'v|r' => 'ř',
    'c|S' => 'Ş',
    'c|s' => 'ş',
    'v|S' => 'Š',
    'v|s' => 'š',
    'c|T' => 'Ţ',
    'c|t' => 'ţ',
    'v|T' => 'Ť',
    'v|t' => 'ť',
    'r|U' => 'Ů',
    'r|u' => 'ů',
    'H|U' => 'Ű',
    'H|u' => 'ű',
    'v|Z' => 'Ž',
    'v|z' => 'ž',
  );
  foreach ($lettery_mapping as $sequence => $character) {
    // Split sequence at the pipe symbol.
    $key = explode('|', $sequence);
    if (count($key) != 2) {
      throw new Exception('Internal error: Invalid sequence ' . check_plain($sequence) . ' for character ' . check_plain($character));
    }
    // Letter escapes require whitespace or quotes, or both.
    $pattern = '(\\{)?\\\\' . preg_quote($key[0], '/') . '((\s*\\{)?|\s+)' . preg_quote($key[1], '/') . '(?(3)\\}|)(?(1)\\}|)';
    $transtab[$pattern] = $character;
  }

  // Simple named sequences like greek letters
  // tex name without the backslash => unicode.
  $mapping = array(
    'alpha'               => 'α',
    'beta'                => 'β',
    'gamma'               => 'γ',
    'delta'               => 'δ',
    'epsilon'             => 'ε',
    'zeta'                => 'ζ',
    'eta'                 => 'η',
    'theta'               => 'θ',
    'iota'                => 'ι',
    'kappa'               => 'κ',
    'lambda'              => 'λ',
    'mu'                  => 'μ',
    'nu'                  => 'ν',
    // AFAICT there is no omicron sequence in TeX,
    // but the previous version had this replacement.
    'omicron'             => 'o',
    'xi'                  => 'ξ',
    'pi'                  => 'π',
    'rho'                 => 'ρ',
    'varsigma'            => 'ς',
    'sigma'               => 'σ',
    'tau'                 => 'τ',
    'upsilon'             => 'υ',
    'phi'                 => 'φ',
    'chi'                 => 'χ',
    'psi'                 => 'ψ',
    'omega'               => 'ω',
    'Gamma'               => 'Γ',
    'Delta'               => 'Δ',
    'Theta'               => 'Θ',
    'Lambda'              => 'Λ',
    'Xi'                  => 'Ξ',
    'Pi'                  => 'Π',
    'Sigma'               => 'Σ',
    'Upsilon'             => 'Υ',
    'Phi'                 => 'Φ',
    'Psi'                 => 'Ψ',
    'Omega'               => 'Ω',

    'AA'                  => 'Å',
    'aa'                  => 'å',
    'AE'                  => 'Æ',
    'ae'                  => 'æ',
    'DH'                  => 'Ð',
    'dh'                  => 'ð',
    'DJ'                  => 'Đ',
    'dj'                  => 'đ',
    'i'                   => 'ı',
    'L'                   => 'Ł',
    'l'                   => 'ł',
    'NG'                  => 'Ŋ',
    'ng'                  => 'ŋ',
    'O'                   => 'Ø',
    'o'                   => 'ø',
    'OE'                  => 'Œ',
    'oe'                  => 'œ',
    'TH'                  => 'Þ',
    'th'                  => 'þ',
    'ss'                  => 'ß',

    'texteuro'            => '€',
    'textcelsius'         => '℃',
    'textnumero'          => '№',
    'textcircledP'        => '℗',
    'textservicemark'     => '℠',
    'texttrademark'       => '™',
    'textohm'             => 'Ω',
    'textestimated'       => '℮',
    'textleftarrow'       => '←',
    'textuparrow'         => '↑',
    'textrightarrow'      => '→',
    'textdownarrow'       => '↓',
    'infty'               => '∞',
    'textlangle'          => '〈',
    'textrangle'          => '〉',
    'textvisiblespace'    => '␣',
    'textopenbullet'      => '◦',
    'textflorin'          => 'ƒ',
    'textasciicircum'     => 'ˆ',
    'textacutedbl'        => '˝',
    'textendash'          => '–',
    'textemdash'          => '—',
    'textbardbl'          => '‖',
    'textunderscore'      => '‗',
    'textquoteleft'       => '‘',
    'textquoteright'      => '’',
    'quotesinglbase'      => '‚',
    'textquotedblleft'    => '“',
    'textquotedblright'   => '”',
    'quotedblbase'        => '„',
    'textdagger'          => '†',
    'textdaggerdbl'       => '‡',
    'textbullet'          => '•',
    'textellipsis'        => '…',
    'textperthousand'     => '‰',
    'guilsinglleft'       => '‹',
    'guilsinglright'      => '›',
    'textfractionsolidus' => '⁄',
    'textdiv'             => '÷',
    'textexclamdown'      => '¡',
    'textcent'            => '¢',
    'textsterling'        => '£',
    'textyen'             => '¥',
    'textbrokenbar'       => '¦',
    'textsection'         => '§',
    'textasciidieresis'   => '¨',
    'textcopyright'       => '©',
    'textordfeminine'     => 'ª',
    'guillemotleft'       => '«',
    'textlnot'            => '¬',
    'textregistered'      => '®',
    'textasciimacron'     => '¯',
    'textdegree'          => '°',
    'textpm'              => '±',
    'texttwosuperior'     => '²',
    'textthreesuperior'   => '³',
    'textasciiacute'      => '´',
    'textmu'              => 'µ',
    'textparagraph'       => '¶',
    'textperiodcentered'  => '·',
    'textonesuperior'     => '¹',
    'textordmasculine'    => 'º',
    'guillemotright'      => '»',
    'textonequarter'      => '¼',
    'textonehalf'         => '½',
    'textthreequarters'   => '¾',
    'textquestiondown'    => '¿',
    'texttimes'           => '×',
    'textgreater'         => '>',
    'textless'            => '<',
  );
  foreach ($mapping as $name => $character) {
    // Consume pairs of $ signs and curly braces, if any;
    // if neither brace nor $ is present then whitespace or a backslash is required to end a sequence.
    $pattern = '(\\$)?(\\{)?\\\\' . $name . '(?(2)\\}|(\\s+|(?=\\$)|(?=\\\\)))(?(1)\\s*\\$|)';
    $transtab[$pattern] = $character;
  }

  // Decode escaped underscores.
  $transtab['\\\\_'] = '_';

  // finally, handle escaped space.
  $transtab['\\\\ '] = ' ';

  // drupal_set_message('<pre>'.check_plain(print_r($transtab, TRUE)).'</pre>');.
  return $transtab;
}
